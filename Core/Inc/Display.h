/*
 * Display.h
 *
 *  Created on: Oct 7, 2021
 *      Author: Szymo
 */
#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_
/*
#include "Clock.h"
#include "BMP280.h"
*/


#define PD_BIT 0 //when 0, chip is active
#define V_BIT 0 //when 0, horizontal addressing is selected
#define LCD_y_size 6 //number of ram rows addresses  in horizotal adressing
#define LCD_x_size 84 //number of columns of LCD in horizotal adressing
#define CONTRAST 0x37

enum displayOption
{
	BlankMode = 0x00,
	NormalMode = 0x04,
	AllOnMode = 0x01,
	InverseMode = 0x05
};

struct settings_s {
	uint8_t clockSystem; //can be equal to 12 or 24
	//enum fonts_enum font;
	uint8_t date_mode;
	enum t_scale t_scale_option;
	enum p_scale p_scale_option;
	uint32_t LED_turn_of_time; //time the led will be turned on after single push of the button
};

struct possition_s
{
	uint8_t x;
	uint8_t y;
}pos;

struct pos_init_final
{
	struct possition_s init;
	struct possition_s finish;
};


typedef uint8_t buffer_template[6][84];


SPI_HandleTypeDef *LCD_hspi_p;

void LCD_reset();
void LCD_function_set(uint8_t H_BIT);
void LCD_display_control(uint8_t diplay_option);
void LCD_set_y_address(uint8_t y);
void LCD_set_x_address(uint8_t x);
void LCD_bias_system();
void LCD_set_V_op();
void LCD_temperature_controll();
void LCD_Vopp_set(uint8_t Vopp_value);
void LCD_Bias_set(uint8_t bias);
void LCD_write(uint8_t *data, uint8_t data_lenght);
void LCD_write_data(uint8_t *data, uint8_t data_lenght);
void LCD_write_settings(uint8_t *data, uint8_t data_lenght);
void LCD_refresh();
void LCD_buffer_clear(buffer_template *local_buffer);
//struct possition_s LCD_write_text_to_the_buffer(struct font_s* my_font, char* my_string, struct possition_s* my_pos, uint8_t *local_buffer, uint8_t x_space);


#endif /* INC_DISPLAY_H_ */
