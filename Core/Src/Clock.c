/*
 * clock.c
 *
 *  Created on: Jan 27, 2023
 *      Author: Szymo
 */
#include <main.h>
#include "Clock.h"


void add_one_year(struct time_s *local_time) {
	local_time->month = 1;
	local_time->year++;
}
void add_one_month(struct time_s *local_time) {
	local_time->day = 1;
	local_time->month++;
	if (local_time->month > 12)
		add_one_year(local_time);
}
void add_one_day(struct time_s *local_time) {
	local_time->hours = 0;
	local_time->day++;
	local_time->day_of_the_week++;
	if (local_time->day_of_the_week > 7)
		local_time->day_of_the_week = 1;
	if (local_time->day > months[local_time->month - 1].days)
		if ( !(( (local_time->month==2)&&(local_time->year%4==0)) && local_time->day == 29)) //Leap years
			add_one_month(local_time);
}
void add_one_hour(struct time_s *local_time) {
	local_time->minutes = 0;
	local_time->hours++;
	if (local_time->hours > 23) {
		add_one_day(local_time);
		local_time->date_update_flag = 1;
	}
}

void add_one_minute(struct time_s *local_time) {
	local_time->seconds = 0;
	local_time->minutes++;
	if (local_time->minutes > 59)
	{
		add_one_hour(local_time);
		local_time->hour_update_flag = 1;
	}

}

void add_one_second(struct time_s *local_time) {
	local_time->seconds++;
	if (local_time->seconds > 59)
	{
		local_time->minutes_update_flag = 1;
		add_one_minute(local_time);
	}
}

void add_one_millisecond(struct time_s *local_time) {
	local_time->milisecond++;
	 // cant be set as equal to 1 becouse in out case I want to display only part of the data
	if(local_time->milisecond%10==0)
	{
		local_time->miliseconds_update_flag = 2;
		if(local_time->milisecond%100==0)
		{
			local_time->miliseconds_update_flag = 3;
			if (local_time->milisecond > 999) {
				local_time->milisecond = 0;
				add_one_second(local_time);
				local_time->seconds_update_flag = 1;
			}
		}
	}
	if( local_time->miliseconds_update_flag==0 ){
		local_time->miliseconds_update_flag = 1;
	}
}
/*
void add_one_millisecond(struct time_s *local_time) {
	local_time->milisecond++;
	if (local_time->milisecond > 99) {
		local_time->milisecond = 0;
		add_one_second(local_time);
		local_time->seconds_update_flag = 1;
	}
}
*/
void add_n_milliseconds(struct time_s *local_time, uint8_t n){
	uint8_t old_time = local_time->milisecond;
	local_time->milisecond+=n;

			if (local_time->milisecond > 999) {
				local_time->milisecond = 0;
				add_one_second(local_time);
				local_time->seconds_update_flag = 1;
			}
}


void Subtract_one_minute(struct time_s *local_time){
	local_time->minutes--;
	if (local_time->minutes < 0)
	{
		local_time->hour_update_flag = 1;
		Subtract_one_hour(local_time);
		local_time->minutes = 59;
	}
}

void Subtract_one_second(struct time_s *local_time){
	local_time->seconds--;
	if (local_time->seconds < 0)
	{
		local_time->minutes_update_flag = 1;
		Subtract_one_minute(local_time);
		local_time->seconds = 59;
	}
}

void Subtract_one_hour(struct time_s *local_time){
	local_time->hours--;
	if (local_time->hours < 0)
	{
		local_time->date_update_flag = 1;
		Subtract_one_hour(local_time);//
	}
}


void clear_time(struct time_s *local_time){
	local_time->day = 1;
	local_time->day_of_the_week=1;
	local_time->month = 1;
	local_time->hours = 0;
	local_time->milisecond = 0;
	local_time->minutes = 0;
	local_time->seconds=0;
	local_time->year = 2010;
}
