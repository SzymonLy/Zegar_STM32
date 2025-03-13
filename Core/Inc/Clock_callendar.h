/*
 * Clock_callendar.h
 *
 *  Created on: Dec 11, 2022
 *      Author: Szymo
 */
#ifndef INC_CLOCK_CALLENDAR_H_
#define INC_CLOCK_CALLENDAR_H_

#include "Template_design.h"
struct templete_fonts clock_fonts;
struct template_possitions clock_pos;



// W means day of the week, T means temperature, P means pressure, D means date
typedef enum ClockMode {
	WTP,
	DTP,
	WDT,
	WDP,
	DWT,
	DWP
};
#define CLOCK_MODES 6

typedef enum ClockHorizontalMode {
	left,
	center,
	right,
};
#define CLOCK_HORIZ_MODES 3

enum ClockMode clock_mode = DTP; // starting mode
enum ClockHorizontalMode clock_horizontal_mode = center;

/*
void Clock_init();
void Change_clock_horizontal_mode();
void Clock_template_init(buffer_template *local_buffer, struct time_s *current_time,
	struct bar_or_temp *weather, struct settings_s *settings, struct template_possitions *local_pos,
	struct templete_fonts *local_fonts);
void Clock_template_init_and_send(buffer_template *local_buffer, struct time_s *current_time,
	struct bar_or_temp *weather, struct settings_s *settings, struct template_possitions *local_pos,
	struct templete_fonts *local_fonts);
void Callendar_template_update_and_send(buffer_template *local_buffer, struct time_s *current_time,
		struct bar_or_temp *weather, struct settings_s *settings, struct template_possitions *local_pos,
		struct templete_fonts *local_fonts);
*/
#endif /* INC_CLOCK_CALLENDAR_H_ */
