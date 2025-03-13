/*
 * Clock_callendar.c
 *
 *  Created on: Jan 27, 2023
 *      Author: Szymo
 */
#include <main.h>

#include "Clock_callendar.h"

#include "Display_fonts.h"
#include "BMP280.h"
#include "Clock.h"
#include "Display.h"
#include "Strings.h"
#include "Template_design.h"

void Clock_init()
{
	clock_pos.hours.init.x			= 0; clock_pos.hours.init.y 		= 0;
	clock_pos.minutes.init.x 		= 42; clock_pos.minutes.init.y		= 0;
	clock_pos.seconds.init.x 		= 70; clock_pos.seconds.init.y 		= 0;
	clock_pos.am_pm.init.x 			= 70; clock_pos.am_pm.init.y 		= 2;

/*
	clock_pos.date.init.x 			= 0;  clock_pos.date.init.y 		= 3;
	clock_pos.week.init.x 			= 0;  clock_pos.week.init.y 	= 3;
	clock_pos.pressure.init.x    	= 100;clock_pos.pressure.init.y  = 3;
	clock_pos.temperature.init.x    = 0;  clock_pos.temperature.init.y  = 3;
*/
	clock_fonts.hours = 		&Font14x24;
	clock_fonts.minutes = 		&Font14x24;
	clock_fonts.seconds = 		&Font5x7;
	clock_fonts.am_pm = 		&Font5x7;
	clock_fonts.pressure =	 	&Font5x7;
	clock_fonts.temperature= 	&Font5x7;
	clock_fonts.date = 			&Font5x7;
	clock_fonts.week = 			&Font5x7;
	clock_fonts.pressure =		&Font5x7;
	clock_fonts.temperature = 	&Font5x7;
}

void Clock_change_mode(struct template_possitions *local_pos,
	struct templete_fonts *local_fonts, enum ClockMode mode, enum ClockHorizontalMode horiz_mode)
{
	if(horiz_mode == left){
		clock_pos.date.init.x 			= 0;
		clock_pos.week.init.x 			= 0;
		clock_pos.pressure.init.x    	= 0;
		clock_pos.temperature.init.x    = 0;
	}else if(horiz_mode == center){
		clock_pos.date.init.x 			= 12;
		clock_pos.week.init.x 			= 16;
		clock_pos.pressure.init.x    	= 18;
		clock_pos.temperature.init.x    = 24;
	}else if(horiz_mode == right){
		clock_pos.date.init.x 			= 24;
		clock_pos.week.init.x 			= 24;
		clock_pos.pressure.init.x    	= 28;
		clock_pos.temperature.init.x    = 40;
	}
	if(mode == WTP){
		clock_pos.date.init.y 			= 100;
		clock_pos.week.init.y 			= 3;
		clock_pos.pressure.init.y  		= 4;
		clock_pos.temperature.init.y  	= 5;
	}else if(mode == DTP){
		clock_pos.date.init.y 			= 3;
		clock_pos.week.init.y 			= 100;
		clock_pos.pressure.init.y  		= 4;
		clock_pos.temperature.init.y  	= 5;
	}else if(mode == WDT){
		clock_pos.date.init.y 			= 4;
		clock_pos.week.init.y 			= 3;
		clock_pos.pressure.init.y  		= 100;
		clock_pos.temperature.init.y  	= 5;
	}else if(mode == WDP){
		clock_pos.date.init.y 			= 4;
		clock_pos.week.init.y 			= 3;
		clock_pos.pressure.init.y  		= 5;
		clock_pos.temperature.init.y  	= 100;
	}else if(mode == DWT){
		clock_pos.date.init.y 			= 3;
		clock_pos.week.init.y 			= 4;
		clock_pos.pressure.init.y  		= 100;
		clock_pos.temperature.init.y  	= 5;
	}else if(mode == DWP){
		clock_pos.date.init.y 			= 3;
		clock_pos.week.init.y 			= 4;
		clock_pos.pressure.init.y  		= 5;
		clock_pos.temperature.init.y  	= 100;
	}
}

void Change_clock_horizontal_mode(){
	clock_horizontal_mode++;
	if(clock_horizontal_mode>=CLOCK_HORIZ_MODES)
		clock_horizontal_mode = 0;
}

void Clock_template_init(buffer_template *local_buffer, struct time_s *current_time,
	struct bar_or_temp *weather, struct settings_s *settings, struct template_possitions *local_pos,
	struct templete_fonts *local_fonts)
{
	if(change_clock_system == 1)// for the stopper to work correctly in am_pm mode
	{

	}

	Template_init(local_buffer, current_time, weather, settings, local_pos, local_fonts);
	char date_string[30], temperature[30], pressure[30], week[30];
	get_pressure_string(weather, settings, pressure, &time_set_blink);
	get_temperature_string(weather, settings, temperature, &time_set_blink);
	get_date_string(current_time, settings, date_string, &time_set_blink);
	get_week_string(current_time, settings, week, &time_set_blink);

	LCD_write_text_to_the_buffer(local_fonts->temperature, temperature, &local_pos->temperature,local_buffer, 1);
	LCD_write_text_to_the_buffer(local_fonts->pressure, pressure, &local_pos->pressure,local_buffer, 1);
	LCD_write_text_to_the_buffer(local_fonts->date, date_string, &local_pos->date,local_buffer, 1);
	LCD_write_text_to_the_buffer(local_fonts->week, week,&local_pos->week, local_buffer, 1);


	if(settings->clockSystem==12 || currentMode == TimeSetMode){
		char am_pm_string[3];
		get_am_pm_string(current_time, settings, am_pm_string, &time_set_blink);
		LCD_write_text_to_the_buffer(local_fonts->am_pm, am_pm_string, &local_pos->am_pm, local_buffer, 1);
	}
}

void Clock_template_init_and_send(buffer_template *local_buffer, struct time_s *current_time,
	struct bar_or_temp *weather, struct settings_s *settings, struct template_possitions *local_pos,
	struct templete_fonts *local_fonts){
	Clock_change_mode(local_pos, local_fonts, clock_mode, clock_horizontal_mode);
	Clock_template_init(local_buffer, current_time, weather, settings, local_pos, local_fonts);
	LCD_refresh(local_buffer);
}


void Callendar_template_update_and_send(buffer_template *local_buffer, struct time_s *current_time,
		struct bar_or_temp *weather, struct settings_s *settings, struct template_possitions *local_pos,
		struct templete_fonts *local_fonts)
{
	Template_update_and_send(local_buffer,current_time,weather,settings,local_pos,local_fonts);
	if(weather->temperature_flag){
		weather->temperature_flag = 0;
		char temperature[30];
		get_temperature_string(weather, settings, temperature, &time_set_blink);
		LCD_write_text_to_the_buffer(local_fonts->temperature, temperature, &local_pos->temperature,local_buffer, 1);
		LCD_refresh_part(local_buffer,&local_pos->temperature);
	}
	if(weather->pressure_flag){
		weather->pressure_flag = 0;
		char pressure[30];
		get_pressure_string(weather, settings, pressure, &time_set_blink);
		LCD_write_text_to_the_buffer(local_fonts->pressure, pressure, &local_pos->pressure,local_buffer, 1);
		LCD_refresh_part(local_buffer,&local_pos->pressure);
	}
}
