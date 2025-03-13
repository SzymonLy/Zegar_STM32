/*
 * Time_set_mode.c
 *
 *  Created on: Jan 27, 2023
 *      Author: Szymo
 */
#include "main.h"
//
#include "BMP280.h"
#include "Clock.h"
#include "Display.h"
#include "Strings.h"
#include "Time_set_mode.h"

void Time_set_init(){
	time_set_blink.hours = 0;
	time_set_blink.minutes= 0;
	time_set_blink.seconds = 0;
	time_set_blink.am_pm = 0;
	time_set_blink.day = 0;
	time_set_blink.month = 0;
	time_set_blink.year = 0;
	time_set_blink.pressure = 0;
	time_set_blink.temperature = 0;
	time_set_blink.date = 0;
}

void Time_set_blink_part(struct time_s *local_time, struct settings_s *settings, uint8_t N_of_B3pressed){
	if (N_of_B3pressed == SET_HOUR){ //setting hours
		time_set_blink.hours = 1;
	} else if (N_of_B3pressed == SET_MINUTE){ //setting minutes
		time_set_blink.minutes = 1;
	} else if (N_of_B3pressed == SET_SECOND){ //setting seconds
		time_set_blink.seconds = 1;
	} else if (N_of_B3pressed == SET_AM_PM){ //setting am or pm or 24h system
		time_set_blink.am_pm = 1;
	} else if (N_of_B3pressed == SET_DAY){ //setting day
		time_set_blink.day = 1;
	} else if (N_of_B3pressed == SET_MONTH){ //setting month
		time_set_blink.month = 1;
	} else if (N_of_B3pressed == SET_YEAR){ //setting year
		time_set_blink.year = 1;
	}else if (N_of_B3pressed == SET_PRESSURE){ //setting pressure
		time_set_blink.pressure = 1;
	}else if (N_of_B3pressed == SET_TEMPERATURE){ //setting temperature
		time_set_blink.temperature = 1;
	}else if (N_of_B3pressed == SET_DATE){ //setting date format
		time_set_blink.day = 1;
		time_set_blink.month = 1;
		time_set_blink.year = 1;
		time_set_blink.date = 1; // Probably not used
	}
}


void Time_set_blink(buffer_template *local_buffer, struct time_s *local_time, struct settings_s *settings,
		struct bar_or_temp *weather, struct template_possitions *local_pos,
		struct templete_fonts *local_fonts, uint8_t N_of_B3pressed) {
	Time_set_blink_part(local_time, settings,N_of_B3pressed);
	Clock_template_init(local_buffer,local_time, weather, settings, local_pos, local_fonts);
	Time_set_init();
}


void change_time(struct time_s *local_time, struct settings_s *settings, uint8_t N_of_B3pressed) {
	uint8_t temp; //to copy data
	if (N_of_B3pressed == SET_HOUR){ //setting hours
		local_time->hours++;
		if (local_time->hours > 23)
			local_time->hours = 0;
	} else if (N_of_B3pressed == SET_MINUTE){ //setting minutes
		local_time->minutes++;
		if (local_time->minutes > 59)
			local_time->minutes = 1;
	} else if (N_of_B3pressed == SET_SECOND){ //setting seconds
		local_time->seconds++;
		if (local_time->seconds > 59)
			local_time->seconds = 0;
	} else if (N_of_B3pressed == SET_AM_PM){ //setting am or pm. Starting from 12clock system, hour 13.32 first push will chenge the clock system and hour to 1.32am, second will chenge the clock system to 13.32
		if(settings->clockSystem!=12){
			settings->clockSystem=12;
		}else{
			local_time->hours = (local_time->hours + 12); //change hour +- 12 hours to set pm or am
			if(local_time->hours>24){
				local_time->hours = local_time->hours%24;
				settings->clockSystem=24;
			}
		}
	} else if (N_of_B3pressed == SET_DAY){ //setting day
		temp = local_time->hours; //copy value
		add_one_day(local_time);
		local_time->hours = temp;
	}else if(N_of_B3pressed == SET_MONTH){ //setting month
		local_time->day_of_the_week = (local_time->day_of_the_week + months[local_time->month-1].days)%7+1;//-1 because array is indexed from 0, not from 1 like months
		local_time->month++;
		if (local_time->month > 12)
			local_time->month = 1;
	} else if (N_of_B3pressed == SET_YEAR){ //setting year
		local_time->year++;
		if(local_time->year%4)
			local_time->day_of_the_week = (local_time->day_of_the_week+365)%7+1;
		else
			local_time->day_of_the_week = (local_time->day_of_the_week+366)%7+1;
		//local_time->day_of_the_week = (local_time->day_of_the_week + (local_time->year%4)?366:365 )%7+1;
		if (local_time->year > 2099)
			local_time->year = 2022;
	} else if (N_of_B3pressed == SET_TEMPERATURE){
		settings->t_scale_option++;
		if(settings->t_scale_option>T_OPTIONS-1)
			settings->t_scale_option = 0;
	} else if (N_of_B3pressed == SET_PRESSURE){
		settings->p_scale_option++;
		if(settings->p_scale_option>P_OPTIONS-1)
			settings->p_scale_option = 0;
	} else if (N_of_B3pressed == SET_DATE){
		settings->date_mode++;
		if(settings->date_mode>DATE_OPTIONS-1)
			settings->date_mode = 0;
	}
}
