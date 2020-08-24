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

#include <stdio.h>

#include "midres.h"

#if ( !defined(__OVERLAY__MIDRES__) && defined(MIDRES_STANDALONE_DRAWING) ) || defined(__OVERLAY__MIDRES__)

  // Overlay management is driven by the definition of the appropriate 
  // compilation symbol (__OVERLAY__). In this case, we enable or disable the 
  // compilation of the relevant code.

#ifdef __OVERLAY__MIDRES__

    // In general, the compiler places the executable code in the "CODE" 
    // segment, while the non-modifiable data in the "RODATA" segment.
    // In order to save space, both of these segments are moved to the 
    // overlay segment, so that both the code and the data it uses are 
    // removed from the resident module.
    #pragma code-name ("OVERLAY2");
    #pragma rodata-name ("OVERLAY2");

#endif

/****************************************************************************
 ** OVERLAYED FUNCTIONS SECTION
 ****************************************************************************/

 // The functions defined at this level can only be called up if the current
 // module has been loaded into memory. On the other hand, they can call any 
 // function declared at the resident module level.

void mr_psetop(mr_mixel* _screen, mr_position _x, mr_position _y, mr_pop _pop) {

    mr_position mx, my;
    mr_mixelbits abcd;

    // Find relative mixel coordinates.

    mx = _x >> 1;
    my = _y >> 1;
    abcd = ((1 - (_x & 1)) + (1 - (_y & 1)) * 2);

    // Draw on the correct place the new mixel combination

    _screen[my * WIDTH + mx] = calculate_mixel(_screen[my * WIDTH + mx], abcd, _pop);

}

void mr_pcolorop(mr_color* _screen, mr_position _x, mr_position _y, mr_color _color) {

    mr_position mx, my;

    // Find relative mixel coordinates.

    mx = _x >> 1;
    my = _y >> 1;

    _screen[my * WIDTH + mx] = _color + BRIGHTNESS;

}

void mr_ink(mr_screen _screen, mr_color _color) {

    SCREEN_INK[_screen] = _color + BRIGHTNESS;

}

#endif