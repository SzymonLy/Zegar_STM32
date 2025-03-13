/*
 * Display_design.c
 *
 *  Created on: Jan 27, 2023
 *      Author: Szymo
 */

#include "main.h"

#include "Template_design.h"

#include "BMP280.h"
#include "Clock.h"
#include "Display.h"
#include "Strings.h"

void Template_init(buffer_template *local_buffer, struct time_s *current_time,
		struct bar_or_temp *weather, struct settings_s *settings, struct template_possitions *local_pos,
		struct templete_fonts *local_fonts)
{

	char hours_string[4], minutes_string[3], seconds_string[4];
	get_hour_string(current_time, settings, hours_string, &time_set_blink);
	get_minute_string(current_time, settings, minutes_string, &time_set_blink);
	get_seconds_string(current_time, settings, seconds_string, &time_set_blink);


	LCD_buffer_clear(local_buffer); //without it there are problems with changing day of weeks and months]
	LCD_write_text_to_the_buffer(local_fonts->hours, hours_string,&local_pos->hours, local_buffer, 0);
	LCD_write_text_to_the_buffer(local_fonts->minutes, minutes_string,&local_pos->minutes, local_buffer, 0);
	LCD_write_text_to_the_buffer(local_fonts->seconds, seconds_string, &local_pos->seconds,local_buffer, 1);

}

void Templete_update_week(buffer_template *local_buffer, struct time_s *current_time, struct settings_s *settings,
		struct pos_init_final *local_pos, struct font_s* local_font, uint8_t additional_space){
	char week[20];
	get_week_string(current_time, settings, week, &time_set_blink);
	LCD_write_text_to_the_buffer(local_font, week,local_pos, local_buffer, additional_space);
}
void Template_update_date(buffer_template *local_buffer, struct time_s *current_time, struct settings_s *settings,
		struct pos_init_final *local_pos, struct font_s* local_font, uint8_t additional_space){
	char date_string[20];
	get_date_string(current_time, settings, date_string, &time_set_blink);
	LCD_write_text_to_the_buffer(local_font, date_string,local_pos, local_buffer, additional_space);
}

void Template_update_minutes(buffer_template *local_buffer, struct time_s *current_time, struct settings_s *settings,
		struct pos_init_final *local_pos, struct font_s* local_font, uint8_t additional_space){
	char minutes_string[3];
	get_minute_string(current_time, settings, minutes_string, &time_set_blink);
	LCD_write_text_to_the_buffer(local_font, minutes_string,local_pos, local_buffer, additional_space);
}

void Template_update_hours(buffer_template *local_buffer, struct time_s *current_time, struct settings_s *settings,
		struct pos_init_final *local_pos, struct font_s* local_font, uint8_t additional_space){
	char hour_string[4];
	get_hour_string(current_time, settings, hour_string, &time_set_blink);
	LCD_write_text_to_the_buffer(local_font, hour_string,local_pos, local_buffer, additional_space);
}

void Template_update_seconds(buffer_template *local_buffer, struct time_s *current_time, struct settings_s *settings,
		struct pos_init_final *local_pos, struct font_s* local_font, uint8_t additional_space){
	char Seconds_string[3];
	get_seconds_string(current_time, settings, Seconds_string, &time_set_blink);
	LCD_write_text_to_the_buffer(local_font, Seconds_string ,local_pos, local_buffer, additional_space);
}


void Template_update_and_send(buffer_template *local_buffer, struct time_s *local_time, struct bar_or_temp *weather,
		struct settings_s *settings, struct template_possitions *local_pos, struct templete_fonts *local_fonts){
	Template_update_seconds(local_buffer,local_time, settings, &local_pos->seconds, local_fonts->seconds, 1);
	LCD_refresh_part(local_buffer,&local_pos->seconds);
	local_time->seconds_update_flag = 0;
	if(local_time->minutes_update_flag){
		Template_update_minutes(local_buffer, local_time, settings, &local_pos->minutes, local_fonts->minutes, 0);
		LCD_refresh_part(local_buffer,&local_pos->minutes);
		local_time->minutes_update_flag = 0;
		if(local_time->hour_update_flag){
			Template_update_hours(local_buffer,local_time, settings, &local_pos->hours, local_fonts->hours, 0);
			LCD_refresh_part(local_buffer,&local_pos->hours);
			local_time->hour_update_flag = 0;
			if(local_time->date_update_flag){
				Template_update_date(local_buffer,local_time,settings,&local_pos->date, local_fonts->date,1);
				Templete_update_week(local_buffer,local_time,settings,&local_pos->date, local_fonts->date,1);
				LCD_refresh_part(local_buffer,&local_pos->date);
				LCD_refresh_part(local_buffer,&local_pos->week);
				local_time->date_update_flag = 0;
			}
		}
	}
}
