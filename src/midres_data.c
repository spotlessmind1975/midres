/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * DRAWING LIBRARY                                                          *
 ****************************************************************************/

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include "midres.h"

  /****************************************************************************
   ** RESIDENT VARIABLES SECTION
   ****************************************************************************/

unsigned char RENDERED_MIXELS[16] = {
   0x20, 0x6c, 0x7b, 0x62,
   0x7c, 0xe1, 0xff, 0xfe,
   0x7e, 0x7f, 0x61, 0xfc,
   0xe2, 0xfb, 0xec, 0xa0
};

/***
 * This value is used to check if the screen memory is corrupted.
 */

#define MIXEL_THRESHOLD	0xff

mr_color SCREEN_INK[MAX_SCREENS];

mr_screen VISIBLE_SCREEN = DEFAULT_SCREEN;

mr_screen ENABLED_SCREEN = DEFAULT_SCREEN;

mr_mixel get_mixel_bits(mr_mixel _current) {

    register unsigned char i = 0;

    for (i = 0; i < sizeof(RENDERED_MIXELS); ++i) {
        if (RENDERED_MIXELS[i] == _current) {
            return i;
        }
    }

    return MIXEL_THRESHOLD;

}

mr_position WIDTH;

mr_position HEIGHT;

mr_color BRIGHTNESS;