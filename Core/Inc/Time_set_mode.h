/*
 * Time_set_mode.h
 *
 *  Created on: Nov 21, 2022
 *      Author: Szymo
 */
#ifndef INC_TIME_SET_MODE_H_
#define INC_TIME_SET_MODE_H_
#include "Template_design.h"

#define SET_HOUR 0
#define SET_MINUTE 1
#define SET_SECOND 2
#define SET_AM_PM 3
#define SET_DAY 4
#define SET_MONTH 5
#define SET_YEAR 6
#define SET_TEMPERATURE 7
#define SET_PRESSURE 8
#define SET_DATE 9

uint8_t N_of_B3pressed = 0; //how many times B3 has been pressed in time chenging mode
uint8_t N_of_chenging_value = 10; // how many modes there is

void Time_set_init();
void Time_set_blink_part(struct time_s *local_time, struct settings_s *settings, uint8_t N_of_B3pressed);

#endif /* INC_TIME_SET_MODE_H_ */
