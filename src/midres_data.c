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

#if ( !defined(__OVERLAY__MIDRES__) && defined(MIDRES_STANDALONE) ) || defined(__OVERLAY__MIDRES__)

  /****************************************************************************
   ** RESIDENT VARIABLES SECTION
   ****************************************************************************/

unsigned char MR_RENDERED_MIXEL_BITS[4] = {
    0x08,
    0x02,
    0x04,
    0x01
};

/***
 * This value is used to check if the screen memory is corrupted.
 */

#define MIXEL_THRESHOLD	0xff

mr_color MR_SCREEN_INK[MR_SCREEN_COUNT];

mr_screen MR_VISIBLE_SCREEN = MR_SCREEN_DEFAULT;

mr_screen MR_ENABLED_SCREEN = MR_SCREEN_DEFAULT;

mr_mixel get_mixel_bits(mr_mixel _current) {

    register unsigned char i = 0;

    for (i = 0; i < sizeof(MR_RENDERED_MIXELS); ++i) {
        if (MR_RENDERED_MIXELS[i] == _current) {
            return i;
        }
    }

    return MIXEL_THRESHOLD;

}

mr_mixel calculate_mixel(mr_mixel _current, mr_mixelbits _abcd, mr_pop _pop) {

    mr_mixelbits currentMixelbits = MIXEL_THRESHOLD;
    int i = 0;

    currentMixelbits = get_mixel_bits(_current);

    if (currentMixelbits != MIXEL_THRESHOLD) {
        switch (_pop) {
        case mr_pixel_on:
            currentMixelbits = currentMixelbits | (1 << _abcd);
            break;
        case mr_pixel_off:
            currentMixelbits = currentMixelbits & ~(1 << _abcd);
            break;
        case mr_pixel_invert:
            currentMixelbits = currentMixelbits ^ (1 << _abcd);
            break;
        }
        return MR_RENDERED_MIXELS[currentMixelbits];
    }
    else {
        return _current;
    }


}


mr_position WIDTH;

mr_position HEIGHT;

mr_color MR_BRIGHTNESS;

mr_screen MR_DB1 = MR_SCREEN_DB1;

mr_screen MR_DB2 = MR_SCREEN_DB2;

mr_boolean MR_MULTICOLOR = mr_false;

#endif