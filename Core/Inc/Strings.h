/*
 * Strings.h
 *
 *  Created on: Dec 11, 2022
 *      Author: Szymo
 */
#ifndef INC_STRINGS_H_
#define INC_STRINGS_H_

//#include "Display.h"

#define BLINKING_ADD_VALUE 100

const char days_of_the_week[7][10] = { "Monday", "Tuesday", "Wednesday", "Thursday",
		"Friday", "Saturday", "Sunday" };


struct blinking_values{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t am_pm;
	uint8_t day;
	uint8_t month;
	uint8_t year;
	uint8_t pressure;
	uint8_t temperature;
	uint8_t date;
};



#define DATE_OPTIONS 6
typedef enum date_format{
	DMY1,
	DMY2,
	YMD1,
	YMD2,
	MDY1,
	MDY2,

	MDY_f,
	DMY_f
};
/*
 * date formats:
 * 23.12.2022
 * 23-12-2022
 * 2022.12.23
 * 2022-12-23
 *
 * Dec 15,2018
 * 14-Dec-2022 27
 *
 * Full date formats:
 * December 23, 2022
 * 23 December, 2022
 */
/*
void add_3_strings(char *destination, const char *string2, const char *string3, const char *string4);
void get_hour_string(struct time_s *local_time, struct settings_s *settings,
		char *hour_string, struct blinking_values *blink);
char* get_minute_string(struct time_s *local_time, struct settings_s *settings);
char* get_time_string(struct time_s *local_time, struct settings_s *settings,
		char *time_string, struct blinking_values *blink);
void get_seconds_string(struct time_s *local_time, struct settings_s *settings,
		char *seconds, struct blinking_values *blink);
void get_day_string(struct time_s *local_time, struct settings_s *settings,
		char *day, struct blinking_values *blink);
void get_month_string(struct time_s *local_time, struct settings_s *settings,
		char *month, struct blinking_values *blink);
void get_year_string(struct time_s *local_time, struct settings_s *settings,
		char *year, struct blinking_values *blink);
char* get_date_string(struct time_s *local_time, struct settings_s *settings,
		char *date_string, struct blinking_values *blink);
void get_week_string(struct time_s *local_time, struct settings_s *settings,
		char *weekday, struct blinking_values *blink);
void get_am_pm_string(struct time_s *local_time, struct settings_s *settings,
		char *am_or_pm_string, struct blinking_values *blink);
void unify_temp_or_pressure(char *string_units, int local_value, char *string_dest);
void get_pressure_string(struct bar_or_temp *weather,
		struct settings_s *settings, char *pressure, struct blinking_values *blink);
void get_temperature_string(struct bar_or_temp *weather,
		struct settings_s *settings, char *temperature, struct blinking_values *blink);
void get_temperature_and_pressure_string(struct bar_or_temp *weather,
		struct settings_s *settings, char *temperature_and_pressure, struct blinking_values *blink );
char* get_milliseconds_string(struct time_s *local_time, struct settings_s *settings, char *milliseconds_string);
*/
#endif /* INC_STRINGS_H_ */
