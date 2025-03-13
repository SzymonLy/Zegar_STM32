/*
 * clock.h
 *
 *  Created on: Nov 18, 2022
 *      Author: Szymo
 */
#ifndef INC_CLOCK_H_
#define INC_CLOCK_H_



struct time_s {
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint16_t milisecond; //will be useful for further applications, like a stopper.
	uint8_t day; // from 1 to 31
	uint8_t month; // from 1 to 12
	uint16_t year; // from 1000 to 9999
	uint8_t day_of_the_week; // from 1 to 7

	uint8_t miliseconds_update_flag; //can be from 0 (all 3 numbers has not changed) to 3 (all 3 numbers has changed)
	uint8_t seconds_update_flag;
	uint8_t minutes_update_flag;
	uint8_t hour_update_flag;
	uint8_t date_update_flag;
} current_time;


struct months_s {
	uint8_t days;
	char *name;
};
struct months_s months[24];

void Subtract_one_second(struct time_s *local_time);
void Subtract_one_hour(struct time_s *local_time);
void Subtract_one_minute(struct time_s *local_time);



#endif /* INC_CLOCK_H_ */
