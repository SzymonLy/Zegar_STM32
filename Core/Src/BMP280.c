/*
 * BMP280.c
 *
 *  Created on: Jan 27, 2023
 *      Author: Szymo
 */

#include <main.h>
#include "BMP280.h"



uint16_t dig_T1, dig_P1;
int16_t dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

uint8_t BMP_Read_Register(uint8_t internal_adr) {
	uint8_t trasmit_buff[1];
	uint8_t recive_buff[1];

	trasmit_buff[0] = internal_adr;

	HAL_I2C_Master_Transmit(BMP_i2c_p, BMP_I2C_ADDRESS, &trasmit_buff[0], 1, 10000);
	HAL_I2C_Master_Receive(BMP_i2c_p, BMP_I2C_ADDRESS + 1, &recive_buff[0], 1, 10000);

	return recive_buff[0];
}

void BMP_Write_Register(uint8_t internal_adr, uint8_t data) {
	uint8_t trasmit_buff[2];

	trasmit_buff[0] = internal_adr;
	trasmit_buff[1] = data;

	HAL_I2C_Master_Transmit(BMP_i2c_p, BMP_I2C_ADDRESS, trasmit_buff, 2, 10000);
}

enum oversampling{
	skiped = 0b000,
	oversampling_x1 = 1,
	oversampling_x2 = 2,
	oversampling_x4 = 3,
	oversampling_x8 = 4,
	oversampling_x16 = 5,
};
enum BMP_modes{
	sleep_mode = 0,
	forced_mode = 1,
	normal_mode = 3,
};
uint8_t BMP_mode = normal_mode;

#define BMP_CTRL_REG 0xF4
#define BMP_CONFIG_REG 0xF5

void BMP_write_ctrl()
{
	uint8_t osrs_t = oversampling_x1;
	uint8_t osrs_p = oversampling_x1;
	uint8_t transmit_buff = osrs_t<<5 | osrs_p<<2 | BMP_mode;
	HAL_I2C_Mem_Write(BMP_i2c_p, BMP_I2C_ADDRESS, BMP_CTRL_REG, 1, &transmit_buff, 1, 100);
}
void BMP_write_config()
{
	uint8_t t_sb = 0; // t_st * 62.5ms = standby time. NOT important in forced mode
	uint8_t filter = 2;
	uint8_t spi3w_en = 0;
	uint8_t transmit_buff = t_sb<<5 | filter<<2 | spi3w_en;
	HAL_I2C_Mem_Write(BMP_i2c_p, BMP_I2C_ADDRESS, BMP_CONFIG_REG, 1, &transmit_buff, 1, 100);
}
void BMP_get_calibration_values() {
	uint8_t receive_buff[24], starting_address = 0x88;

	HAL_I2C_Master_Transmit(BMP_i2c_p, BMP_I2C_ADDRESS, &starting_address, 1,100);
	HAL_I2C_Master_Receive(BMP_i2c_p, BMP_I2C_ADDRESS + 1, &receive_buff[0], 24,100);


	dig_T1 = (receive_buff[0]) + (receive_buff[1] << 8);
	dig_T2 = (receive_buff[2]) + (receive_buff[3] << 8);
	dig_T3 = (receive_buff[4]) + (receive_buff[5] << 8);
	dig_P1 = (receive_buff[6]) + (receive_buff[7] << 8);
	dig_P2 = (receive_buff[8]) + (receive_buff[9] << 8);
	dig_P3 = (receive_buff[10]) + (receive_buff[11] << 8);
	dig_P4 = (receive_buff[12]) + (receive_buff[13] << 8);
	dig_P5 = (receive_buff[14]) + (receive_buff[15] << 8);
	dig_P6 = (receive_buff[16]) + (receive_buff[17] << 8);
	dig_P7 = (receive_buff[18]) + (receive_buff[19] << 8);
	dig_P8 = (receive_buff[20]) + (receive_buff[21] << 8);
	dig_P9 = (receive_buff[22]) + (receive_buff[23] << 8);
}
void BMP280_init()
{
	BMP_write_ctrl();
	BMP_write_config();
	BMP_get_calibration_values();
}



int t_fine;


void BMP280_read_values(struct bar_or_temp *weather) {

	uint8_t receive_buff[6], starting_address = BMP_PRESS_REG;
	int adc_T, adc_P;

	if (BMP_mode == forced_mode)
		BMP_write_config();
	HAL_I2C_Master_Transmit(BMP_i2c_p, BMP_I2C_ADDRESS , &starting_address, 1,100);
	HAL_I2C_Master_Receive(BMP_i2c_p, BMP_I2C_ADDRESS , &receive_buff[0], 6, 100);
	uint32_t temp[3];
	temp[2] = receive_buff[3];
	temp[1] = receive_buff[4];
	temp[0] = receive_buff[5];
	adc_T = (temp[2] << 12) + (temp[1] << 4) + (temp[0] >> 4);

	temp[2] = receive_buff[0];
	temp[1] = receive_buff[1];
	temp[0] = receive_buff[2];
	adc_P = (temp[2] << 12) + (temp[1] << 4) + (temp[0] >> 4);

	weather->temperature = bmp280_compensate_T_int32(adc_T);
	weather->pressure =  bmp280_compensate_P_int32(adc_P);
	//altitude = 44330.0f * (1 - powf(pressure / 101325.0f, 1.0f / 5.255f)); //altitude=((powf(101325.0/pressure, 1/5.257f)-1)*(temperature+273.15f))/0.0065f;
}


void BMP280_read_values_dynamic(struct bar_or_temp *weather, uint8_t *temp_counter){
	(*temp_counter)++;
	if((*temp_counter) >= 5){ // dynamic change of temperature readout rate
		struct bar_or_temp last_weather;
		last_weather.temperature = weather->temperature;
		last_weather.pressure = weather->pressure;
		weather->pressure_flag = 1;
		weather->temperature_flag=1;
		BMP280_read_values(weather);
		if(	weather->temperature+20 < last_weather.temperature ||
			weather->temperature-20 > last_weather.temperature ||
			weather->pressure+20 	< last_weather.pressure ||
			weather->pressure-20 	> last_weather.pressure){
			*temp_counter = 10;
		}else{
			*temp_counter = 0;
		}
	}
}

void HAL_I2C_MasterRxCpltCallback (I2C_HandleTypeDef *hi2c){

}




//2 functions are copied from the bmp280 datasheet

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
// t_fine carries fine temperature as global value

//all dig_Tn are values from the calibration registers.
uint64_t a;

int bmp280_compensate_T_int32(int adc_T)
{
	int var1, var2, T;
	var1 = ((((adc_T>>3) - ((int)dig_T1<<1))) * ((int)dig_T2)) >> 11;
	var2 = (((((adc_T>>4) - ((int)dig_T1)) * ((adc_T>>4) - ((int)dig_T1)))>> 12) *((int)dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T; // for example T=2573 = 25.73C
}

uint32_t bmp280_compensate_P_int32(int adc_P) {
	int var1, var2;
	uint32_t p;
	var1 = (((int)t_fine)>>1) - (int)64000;
	var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int) dig_P6);
	var2 = var2 + ((var1 * ((int) dig_P5)) << 1);
	var2 = (var2 >> 2) + (((int) dig_P4) << 16);
	var1 = (((dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int) dig_P2) * var1) >> 1)) >> 18;
	var1 = ((((32768 + var1)) * ((int) dig_P1)) >> 15);
	if (var1 == 0) {
		return 0; // avoid exception caused by division by zero
	}
	p = (((uint32_t)(((int) 1048576) - adc_P) - (var2 >> 12))) * 3125;
	if (p < 0x80000000) {
		p = (p << 1) / ((uint32_t) var1);
	} else {
		p = (p / (uint32_t) var1) * 2;
	}
	var1 = (((int) dig_P9)* ((int)(((p >> 3) * (p >> 3)) >> 13))) >> 12;
	var2 = (((int)(p >> 2)) * ((int) dig_P8)) >> 13;
	p = (uint32_t)((int) p + ((var1 + var2 + dig_P7) >> 4));
	return p;
}
