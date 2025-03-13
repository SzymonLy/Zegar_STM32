/*
 * Stopper.c
 *
 *  Created on: Jan 27, 2023
 *      Author: Szymo
 */
#include "main.h"
#include "Display_fonts.h"
#include "BMP280.h"
#include "Clock.h"
#include "Display.h"
#include "Strings.h"
#include "Stopper.h"

void Stopper_init(){
	stopper_pos.hours.init.x =			0; stopper_pos.hours.init.y = 			0;
	stopper_pos.minutes.init.x =	 	42; stopper_pos.minutes.init.y = 		0;
	stopper_pos.seconds.init.x = 	 	0; stopper_pos.seconds.init.y = 		3;
	stopper_pos.miliseconds.init.x =    16; stopper_pos.miliseconds.init.y =	3;
	stopper_pos.temperature.init.x = 	0; stopper_pos.temperature.init.y = 	5;
	stopper_pos.miliseconds_n1.init.x = 40; stopper_pos.miliseconds_n1.init.y = 3;
	stopper_pos.miliseconds_n2.init.x = 32; stopper_pos.miliseconds_n2.init.y = 3;
	stopper_pos.miliseconds_n3.init.x = 24; stopper_pos.miliseconds_n3.init.y = 3;
	stopper_pos.miliseconds_n1.finish.x = 47; stopper_pos.miliseconds_n1.finish.y = 4;
	stopper_pos.miliseconds_n2.finish.x = 39; stopper_pos.miliseconds_n2.finish.y = 4;
	stopper_pos.miliseconds_n3.finish.x = 31; stopper_pos.miliseconds_n3.finish.y = 4;
	stopper_pos.temperature.init.x 	  = 0; stopper_pos.temperature.init.y 	  = 5;

	stopper_fonts.hours = 		&Font14x24;
	stopper_fonts.minutes = 	&Font14x24;
	stopper_fonts.seconds = 	&Font7x16;
	stopper_fonts.miliseconds = &Font7x16;
	stopper_fonts.pressure = 	&Font5x7;
	stopper_fonts.temperature = &Font5x7;
}

void Stopper_template_init(buffer_template *local_buffer, struct time_s *current_time, struct bar_or_temp *weather,
		struct settings_s *settings, struct template_possitions *local_pos, struct templete_fonts *local_fonts){
//struct template_possitions local_pos
	char  milliseconds_string[5];
	if(settings->clockSystem==12)
	{
		settings->clockSystem=24;
		change_clock_system = 1;
	}

	Template_init(local_buffer, current_time, weather, settings, local_pos, local_fonts);
	get_milliseconds_string(current_time, settings, milliseconds_string);
	LCD_write_text_to_the_buffer(local_fonts->miliseconds, milliseconds_string,&local_pos->miliseconds, local_buffer, 1);
}


//lenght is from 1 to 3 and tells how many numbers has to be updated
void Template_update_milisecond_n_numbers(buffer_template *local_buffer, struct time_s *local_time,
		struct settings_s *settings, struct pos_init_final *local_pos, struct font_s *local_font){
	char milliseconds_string[5]; //:001\0
	get_milliseconds_string(local_time, settings, milliseconds_string);
	int lenght = 4-local_time->miliseconds_update_flag;
	//there should be some check if the lenght is beetween 1 and 3, but it would slow down the function
	local_pos->init.x +=8*lenght;
	LCD_write_text_to_the_buffer(local_font, &milliseconds_string[lenght],local_pos, local_buffer, 1);
	local_pos->init.x -=8*lenght;


}


void Stopper_update_and_send(buffer_template *local_buffer,struct time_s *local_time,
		struct settings_s *settings, struct bar_or_temp *weather,struct template_possitions *local_pos){


/*
 * Template_update_milisecond_n_numbers(local_buffer,local_time,settings, &local_pos->miliseconds, stopper_fonts.miliseconds);
	LCD_refresh_part(local_buffer,&local_pos->miliseconds_n1);
	if(local_time->miliseconds_update_flag>=2){
		LCD_refresh_part(local_buffer,&local_pos->miliseconds_n2);
		if(local_time->miliseconds_update_flag==3){
			LCD_refresh_part(local_buffer,&local_pos->miliseconds_n3);
			if(local_time->seconds_update_flag){
				Template_update_and_send(local_buffer, local_time, weather, settings, local_pos, &stopper_fonts);
			}
		}
		local_time->miliseconds_update_flag=1;
	}*/
	char  milliseconds_string[5];
	get_milliseconds_string(local_time, settings, milliseconds_string);
	LCD_write_text_to_the_buffer(stopper_fonts.miliseconds, milliseconds_string,&local_pos->miliseconds, local_buffer, 1);
	LCD_refresh_part(local_buffer,&local_pos->miliseconds);
	if(local_time->seconds_update_flag){
		Template_update_and_send(local_buffer, local_time, weather, settings, local_pos, &stopper_fonts);
	}
}
