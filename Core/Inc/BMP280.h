/*
 * BMP280.h
 *
 *  Created on: Dec 11, 2022
 *      Author: Szymo
 */

#ifndef INC_BMP280_H_
#define INC_BMP280_H_


#define BMP_TEMP_REG 0xFA
#define BMP_PRES_REG 0xF9
#define BMP_ID_REG 0xD0
#define BMP_ADDRESS 0x58
#define BMP_RESET_REG 0xE0
#define BMP_STATUS_REG 0xF3
#define BMP_CTRL_REG 0xF4
#define BMP_CONFIG_REG 0xF5
#define BMP_PRESS_REG 0xF7
#define BMP_I2C_ADDRESS 0xEC //0x76<<1 or 0x77


uint32_t bmp280_compensate_P_int32(int adc_P);
int bmp280_compensate_T_int32(int adc_T);
I2C_HandleTypeDef *BMP_i2c_p;

typedef struct bar_or_temp {
	int temperature; // in C/100
	uint32_t pressure; // in hPa/100
	uint8_t temperature_flag;
	uint8_t pressure_flag;
};
#define T_OPTIONS 3
typedef enum t_scale{
	celcius_scale,
	fahrenheit_scale,
	kelvin_scale,
};
#define P_OPTIONS 4
typedef enum p_scale{
	hectopascals,
	pascals,
	mmHg,
	torr,
};



#endif /* INC_BMP280_H_ */
