/*
 * String.c
 *
 *  Created on: Jan 27, 2023
 *      Author: Szymo
 */
#include "main.h"

#include "BMP280.h"
#include "Clock.h"
#include "Display.h"
#include "Strings.h"


void Templates_init() {
	months[0].days = 31;
	months[1].days = 28;
	months[2].days = 31;
	months[3].days = 30;
	months[4].days = 31;
	months[5].days = 30;
	months[6].days = 31;
	months[7].days = 31;
	months[8].days = 30;
	months[9].days = 31;
	months[10].days = 30;
	months[11].days = 31;

	months[0].name = "January";
	months[1].name = "February";
	months[2].name = "March";
	months[3].name = "April";
	months[4].name = "May";
	months[5].name = "June";
	months[6].name = "July";
	months[7].name = "August";
	months[8].name = "September";
	months[9].name = "October";
	months[10].name = "November";
	months[11].name = "December";
}


void add_3_strings(char *destination, const char *string2, const char *string3, const char *string4){
	strcpy(destination,string2);
	strcat(destination,string3);
	strcat(destination,string4);
}

void get_hour_string(struct time_s *local_time, struct settings_s *settings,
		char *hour_string, struct blinking_values *blink) {
	uint8_t temp_hour = local_time->hours; //temp_hour is a varieble that will be display as hour
//	char *hour_local = "   ";
	if (settings->clockSystem == 12) {
		if (temp_hour == 0)
			temp_hour = 12;
		else if (temp_hour > 12)
			temp_hour = temp_hour - 12;
	}

	if (temp_hour < 10)
		sprintf(hour_string, "0%d:\0", temp_hour);
	else
		sprintf(hour_string, "%d:\0", temp_hour);

	if (blink->hours)	//To blink during time_set hour must be biggg
		sprintf(hour_string, "  :\0");
}

char* get_minute_string(struct time_s *local_time, struct settings_s *settings,
		char *minute, struct blinking_values *blink) {
	if (local_time->minutes < 10)
		sprintf(minute, "0%d\0", local_time->minutes);
	else
		sprintf(minute, "%d\0", local_time->minutes);

	if (blink->minutes)//To blink during time_set minute must be temporary set as something more than 60
		sprintf(minute, "  \0");
	return minute;
}

char* get_time_string(struct time_s *local_time, struct settings_s *settings,
		char *time_string, struct blinking_values *blink) {
	char hour[8], minutes[3];
	get_hour_string(local_time, settings, hour, blink);
	get_minute_string(local_time, settings, minutes, blink);
	strcpy(time_string, strcat(hour, minutes));
	return time_string;
}

void get_seconds_string(struct time_s *local_time, struct settings_s *settings,
		char *seconds, struct blinking_values *blink) {
	if (local_time->seconds < 10)
		sprintf(seconds, "0%d", local_time->seconds);
	else
		sprintf(seconds, "%d", local_time->seconds);
	if (blink->seconds)//To blink during time_set second must be temporary set as something more than 60
		sprintf(seconds, "  \0");
}

void get_day_string(struct time_s *local_time, struct settings_s *settings,
		char *day, struct blinking_values *blink) {
	if (local_time->day < 10)
		sprintf(day, "0%d", local_time->day);
	else
		sprintf(day, "%d", local_time->day);
	if (blink->day)
		sprintf(day, "  ");
}


void get_month_string(struct time_s *local_time, struct settings_s *settings,
		char *month, struct blinking_values *blink) {

	if (local_time->month < 10)
		sprintf(month, "0%d\0", local_time->month);
	else
		sprintf(month, "%d\0", local_time->month);
	if (blink->month)		//To blink
		sprintf(month, "  \0");

	/*
	 		strcpy(month, months[local_time->month - 1].name);
		if(blink->month){
			uint8_t i;
			for(i = 0; i<months[local_time->month-1].name[i];i++)
				strcpy(month, " ");
			strcpy(month, "\0"); //should be implemented automaticly
		}
	 */
}

void get_year_string(struct time_s *local_time, struct settings_s *settings,
		char *year, struct blinking_values *blink) {
	//if (settings->date_mode == 0) {
		sprintf(year, "%d\0", local_time->year);
		if(blink->year)
			sprintf(year, "    \0");
	/*} else if (settings->date_mode == 1) {
		sprintf(year, "%d\0", local_time->year - 2000);
		if(blink->year)
			year = "  ";
	}*/
}


char* get_date_string(struct time_s *local_time, struct settings_s *settings,
		char *date_string, struct blinking_values *blink) {
	char day[15], month[15], year[15], date[30];
	get_day_string(local_time, settings, day, blink);
	get_month_string(local_time, settings, month, blink);
	get_year_string(local_time, settings, year, blink);
	char *dot = ".";
	char *dash = "-";
	if(settings->date_mode == DMY1){
		strcat(day,dot);
		strcat(month,dot);
		add_3_strings(date,day,month,year);
	}else if (settings->date_mode == YMD1){
		strcat(year,dot);
		strcat(month,dot);
		add_3_strings(date,year,month,day);
	}else if(settings->date_mode == DMY2){
		strcat(day,dash);
		strcat(month,dash);
		add_3_strings(date,day,month,year);
	}else if(settings->date_mode == YMD2){
		strcat(year,dash);
		strcat(month,dash);
		add_3_strings(date,year,month,day);
	}else if (settings->date_mode == MDY1){
		strcat(day,dash);
		strcat(month,dash);
		add_3_strings(date,month,day,year);
	}else if (settings->date_mode == MDY2){
		strcat(day,dot);
		strcat(month,dot);
		add_3_strings(date,month,day,year);
	}
	strcpy(date_string,date);
}

void get_week_string(struct time_s *local_time, struct settings_s *settings,
		char *weekday, struct blinking_values *blink)
{
	strcpy(weekday, days_of_the_week[local_time->day_of_the_week-1]);
}

void get_am_pm_string(struct time_s *local_time, struct settings_s *settings,
		char *am_or_pm_string, struct blinking_values *blink) {
	char *am_pm_string_local = "  ";
	if(blink->am_pm)
		am_pm_string_local = "  ";
	else{
		if(settings->clockSystem==12){
			if (local_time->hours < 13 && local_time->hours > 0) {
				am_pm_string_local = "AM";
			} else{
				am_pm_string_local = "PM";
			}
		}else
			am_pm_string_local = "24";
	}
	strcpy(am_or_pm_string, am_pm_string_local);
}
void unify_temp_or_pressure(char *string_units, int local_value, char *string_dest)
{
	char p_value[6];
	sprintf(p_value, "%d\0", local_value);
	int p_value_lenght = (uint8_t)strlen(p_value);
	p_value[p_value_lenght-1] = p_value[p_value_lenght-2];
	p_value[p_value_lenght-2] = '.';
	strcpy(string_dest,strcat(p_value,string_units));
}


void get_pressure_string(struct bar_or_temp *weather,
		struct settings_s *settings, char *pressure, struct blinking_values *blink) {

	char *p_units;
	int conv_p = 0; //Default value = 0, converted pressure
	if (settings->p_scale_option == hectopascals){
		conv_p = weather->pressure;
		p_units = "hPa\0"; //^ symbol is displayed as circle
	}else if(settings->p_scale_option == mmHg){
		conv_p = weather->pressure*0.75006;
		p_units = "mmHg\0";
	}else if(settings->p_scale_option == torr){
		conv_p = weather->pressure*0.75006;
		p_units = "torrs";
	}
	if(blink->pressure)
		p_units = "    \0";

	unify_temp_or_pressure(p_units, conv_p, pressure);
}


//works only for temperatures between -99 and 999
//weather->temperature =2530 means that temperature = 25.30 C
void get_temperature_string(struct bar_or_temp *weather,
		struct settings_s *settings, char *temperature, struct blinking_values *blink) {
	char *t_units = "    ";
	int conv_t = 0; //Default value
	if (settings->t_scale_option == celcius_scale){
		conv_t = weather->temperature;
		t_units = "*C"; //^ symbol is displayed as circle
	}else if(settings->t_scale_option == fahrenheit_scale){
		conv_t = 1.8*weather->temperature+3200;
		t_units = "*F";
	}else if(settings->t_scale_option == kelvin_scale){
		conv_t = weather->temperature+27300;
		t_units = "K\0";
	}
	if(blink->temperature)
		t_units = "   \0";

	unify_temp_or_pressure(t_units, conv_t, temperature);
}
void get_temperature_and_pressure_string(struct bar_or_temp *weather,
		struct settings_s *settings, char *temperature_and_pressure, struct blinking_values *blink ) {
	char pressure[15], temperature[20];
	get_pressure_string(weather, settings, pressure, blink);
	get_temperature_string(weather, settings, temperature, blink);
	strcat(temperature, pressure);
	strcpy(temperature_and_pressure, temperature);
}

char* get_milliseconds_string(struct time_s *local_time, struct settings_s *settings, char *milliseconds_string)
{
	if(local_time->milisecond<10)
		sprintf(milliseconds_string, ":00%d\0", local_time->milisecond);
	else if( local_time->milisecond<100)
		sprintf(milliseconds_string, ":0%d\0", local_time->milisecond);
	else if ( local_time->milisecond<1000)
		sprintf(milliseconds_string, ":%d\0", local_time->milisecond);
	else//blinking
		sprintf(milliseconds_string, ":  \0");
}
