/*
 * Display.c
 *
 *  Created on: Jan 27, 2023
 *      Author: Szymo
 */
#include <main.h>
#include "Display_fonts.h"
#include "BMP280.h"
#include "Display.h"

void LCD_Init(){
	font_init();
	LCD_reset();
	LCD_function_set(1); //should send b0010 0001
	LCD_Bias_set(0x04); //should send b0001 0100
	LCD_Vopp_set(CONTRAST);
	LCD_function_set(0); //back to the normal instruction set, b0010 0000
	LCD_display_control(NormalMode);
	//LCD_buffer_clear(LCD_buffer);
	//LCD_refresh(LCD_buffer);
//	LCD_set_y_address(0);
//	LCD_set_x_address(0);
}


void LCD_reset(){
	HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);
}

void LCD_example(){
	LCD_Init(0x38); //had been u
	uint8_t data_buffer[10] = {31,5,7,0,31,4,31,15};
	LCD_write_data(&data_buffer[0], 7);
	HAL_Delay(1000);
	LCD_display_control(InverseMode);
}

void LCD_function_set(uint8_t H_BIT){
	uint8_t data = 1<<5 | PD_BIT<<2 | V_BIT<<1 | H_BIT;
	LCD_write_settings(&data, 1);
}

void LCD_Bias_set(uint8_t bias){
	if(bias>0x07) bias = 0x07;
	uint8_t data = 1<<4 | bias;
	LCD_write_settings(&data, 1);
}

void LCD_Vopp_set(uint8_t Vopp_value){
	if (Vopp_value > 0x7F) Vopp_value = 0x7F;
	uint8_t data = 1<<7 | Vopp_value;
	LCD_write_settings(&data, 1);
}

void LCD_display_control(enum displayOption DiplayMode){
	uint8_t data = 1<<3 | DiplayMode;
	LCD_write_settings(&data, 1);
}

void LCD_set_y_address(uint8_t y){
	y = y%LCD_y_size; //max y =5
	uint8_t data = 1<<6 | y;
	LCD_write_settings(&data, 1);
}

void LCD_set_x_address(uint8_t x){
	x = x%LCD_x_size; // max x = 83
	uint8_t data = 1<<7 | x;
	LCD_write_settings(&data, 1);
}

void LCD_write_possition(uint8_t x, uint8_t y){
	pos.x = x, pos.y = y;
}


void LCD_write(uint8_t *data, uint8_t data_lenght)
{
	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(LCD_hspi_p, data, data_lenght, (uint32_t)100);
	//HAL_SPI_Transmit_DMA(LCD_hspi_p, data, data_lenght);
	//HAL_SPI_Transmit_IT(LCD_hspi_p, data, data_lenght);
	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_SET);
}


void LCD_write_data(uint8_t *data, uint8_t data_lenght){

	HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
	LCD_write(data, data_lenght);
}

void LCD_write_settings(uint8_t *data, uint8_t data_lenght)
{
	HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);
	LCD_write(data, data_lenght);
}

void LCD_clear_area(buffer_template *local_buffer, struct possition_s pos_init, struct possition_s pos_final)
{
	if(pos_init.y>LCD_y_size-1||pos_init.x>LCD_x_size-1){  //indexing of x and y from 0, that why there is -1
		return;
	}
	for(int x=pos_init.x;x<pos_final.x;x++){
		for(int y=pos_init.y;y<pos_final.y;y++)
			(*local_buffer)[y][x] = 0;
	}
}
void LCD_buffer_clear(buffer_template *local_buffer){
	int x,y;
	for(y=0;y<6;y++){
		for(x=0;x<84;x++)
			(*local_buffer)[y][x]=0;
	}
}
void LCD_refresh(buffer_template *local_buffer)
{
	int y=0;
	for(y=0;y<6;y++)
	{
		LCD_set_y_address(y);
		LCD_set_x_address(0);
		LCD_write_data( &((*local_buffer)[y][0]), 84);
	}
}

//refresh rectangle starting at  from pos_init to pos_finish
void LCD_refresh_part(buffer_template *local_buffer, struct pos_init_final *local_pos){
	int y;
	if(local_pos->init.x<local_pos->finish.x)
	{
		local_pos->finish.x =LCD_x_size-1;
	}
	for(y=local_pos->init.y;y<=local_pos->finish.y;y++)
	{
		LCD_set_y_address(y);
		LCD_set_x_address(local_pos->init.x);
		LCD_write_data(&((*local_buffer)[y][local_pos->init.x]), local_pos->finish.x-local_pos->init.x);
	}
}

void LCD_write_text_to_the_buffer(struct font_s* my_font, char* my_string,
		struct pos_init_final* my_pos, buffer_template *local_buffer, uint8_t x_space)
{
	uint8_t x= my_pos->init.x, y = my_pos->init.y;// column and row on the lcd.

	if(y>LCD_y_size-1||x>LCD_x_size-1){  //indexing of x and y from 0, that why there is -1
		return;
	}

	uint8_t z = 0; // character of the my_string, from 0 to my_font->xsize*my_font->ysize
	uint8_t i = 0; // font's column
	uint8_t j = 0; // font's row
	uint8_t s = 0; // additional spaces between the letters
	uint8_t n_of_rows = (my_font->ysize-1)/8+1; //number of bytes single x column of letter require.

	while(my_string[z]!='\0')
	{
		if( (uint8_t)my_string[z] == '\n')// When the new line symbol will appear, another will be selected
		{
			y += n_of_rows;
			if(y>LCD_y_size-1)
				y=0;
			x=0;
			z++;//next character in the string.
		}
		for(j=0; j<n_of_rows; j++) //starting from the lowest row (because it's easier to generate fonts printable this way)
		{
			for(i=0;i<my_font->xsize; i++) //column by column, from left to right copy values from the font's letter
				(*local_buffer)[y+n_of_rows-1-j][x+i] =  my_font->letters[(uint8_t)my_string[z]-' '][i*n_of_rows+j];
			//*(local_buffer+x+i+ (y+n_of_rows-1-j)*LCD_x_size) = my_font->letters[(uint8_t)my_string[z]-' '][i*n_of_rows+j];
			for(s=0;s<x_space;s++) //additional space between the letters, which is require for some fonts.
				(*local_buffer)[y+j][x+i+s] = 0;
				//*(local_buffer+x+i+s+  (y+j)*LCD_x_size) = 0; // local_buffer[x+i+s][y+j]=0
		}
		z++;
		x = x + my_font->xsize + s;
		if(x > LCD_x_size-1)
		{
			y = y + n_of_rows;
			if(y>LCD_y_size-1)
				y=0;
			x=0;
		}
	}
	my_pos->finish.x = x;
	my_pos->finish.y = my_pos->init.y+n_of_rows-1;
}

//this functions is supposed to enable writing data anywhere.
/*
uint8_t priority_encoder(uint8_t data)
{
	uint8_t output= 0, power2 = 1;
	for(int i=0;i<data;i++)
	{
		output = output + power2;
		pow2 = 2*pow2;
	}
	return output;
}
void LCD_write_text_to_the_buffer_anywhere(struct font_s* my_font, char* my_string, struct possition_s* my_pos, uint8_t *local_buffer, uint8_t x_space, uint8_t y_space)
{
	uint8_t x= my_pos->x, y = my_pos->y;// column and row on the lcd.
	uint8_t z = 0; // character of the my_string, from 0 to my_font->xsize*my_font->ysize
	uint8_t i = 0; // font's column
	uint8_t j = 0; // font's row
	uint8_t s = 0; // additional spaces between the letters
	uint8_t n_of_rows = (my_font->ysize-1)/8+1; //number of bytes single x column of letter require.
	possition_s end_possition;
	uint8_t y_mask = priority_encoder(my_font->ysize); //wrong
	while(my_string[z]!='\0')
	{
		if( (uint8_t)my_string[z] == '\n')
		{
			y = y + my_font->ysize + y_space;
			if(y>5)
				y=0;
			x=0;
			z++;
		}
		for(j=0; j<n_of_rows; j++) //lower row is first.
		{
			for(i=0;i<my_font->xsize; i++)
				//local_buffer[3*x+i][y+n_of_rows-1-j] = LCD_Font3x5[(uint8_t)my_string[z]]-' '][i*n_of_rows+j];
				*(local_buffer+x+i+(y+n_of_rows-1-j)*84) = my_font->letters[(uint8_t)my_string[z]-' '][i*n_of_rows+j];
			for(s=0;s<x_space;s++)
				*(local_buffer+x+i+s+(y+j)*84) = 0^y_mask;
		}

		z++;
		x = x + my_font->xsize + s;
		if(x > 84)
		{
			y = y + my_font->ysize + y_space;
			if(y>5)
				y=0;
			x=0;
		}
	}

	end_possition.x = x;
	end_possition.y = y;
	// end_possition.row = row;
	return(my_pos);
}
*/
