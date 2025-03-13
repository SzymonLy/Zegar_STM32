/*
 * Display_fonts.c
 *
 *  Created on: Jan 27, 2023
 *      Author: Szymo
 */
#include <main.h>

#include "Display_fonts.h"

void copy_array(struct font_s *my_struct, int x_max, int y_max, const uint8_t LCD_Font[x_max][y_max])
{
	int x=0,y=0;
	for( x=0;x<x_max;x++)
	{
		for(y=0;y<y_max;y++)
			my_struct->letters[x][y] = LCD_Font[x][y];
	}
}

void font_init()
{
	Font3x5.xsize = 3;
	Font3x5.ysize = 5;

	Font5x7.xsize = 5;
	Font5x7.ysize = 7;

	copy_array(&Font5x7, 95, 5, LCD_Font5x7); //this should be done with pointers but I struggled implementing them for too long.
	copy_array(&Font3x5, 95, 3, LCD_Font3x5);

	Font7x16.ysize = 16;
	Font7x16.xsize = 7;
	copy_array(&Font7x16, 0x1c, 14, LCD_Font7x16);

	Font14x24.ysize = 24;
	Font14x24.xsize = 14;
	copy_array(&Font14x24, 0x1b, 42, LCD_Font14x24);

}
