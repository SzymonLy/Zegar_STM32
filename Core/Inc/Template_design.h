/*
 * Display_designs.h
 *
 *  Created on: Nov 12, 2022
 *      Author: Szymo
 */
#ifndef INC_DISPLAY_DESIGNS_H_
#define INC_DISPLAY_DESIGNS_H_

#include "Strings.h"

//typedef uint8_t buffer_template[6][84];
uint8_t change_clock_system = 0; // for the stopper to work correctly in am_pm mode
buffer_template LCD_buffer;
buffer_template Clock_callendar;
buffer_template Stopper_template;
buffer_template LCD_second_buffer;

struct blinking_values time_set_blink;

typedef enum DeviceMode {
	ClockMode,
	TimeSetMode,
	StopperMode,
	TimerMode,
	AllarmMode,
	TemperatureMode,
	PressureMode
};
enum DeviceMode currentMode;


typedef struct templete_fonts{
	struct font_s *miliseconds;
	struct font_s *seconds;
	struct font_s *minutes;
	struct font_s *hours;
	struct font_s *date;
	struct font_s *temperature;
	struct font_s *pressure;
	struct font_s *am_pm;
	struct font_s *week;
};



typedef struct template_possitions {
	struct pos_init_final miliseconds;
	struct pos_init_final miliseconds_n1;
	struct pos_init_final miliseconds_n2;
	struct pos_init_final miliseconds_n3;
	struct pos_init_final seconds;
	struct pos_init_final minutes;
	struct pos_init_final hours;
	struct pos_init_final date;
	struct pos_init_final week;
	struct pos_init_final temperature;
	struct pos_init_final pressure;
	struct pos_init_final am_pm;
};


#endif /* INC_DISPLAY_DESIGNS_H_ */
