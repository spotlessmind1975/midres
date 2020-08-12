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

#ifdef __CBM__
#include <cc65.h>
#include <cbm.h>
#include <device.h>
#else

#endif

  // Overlay management is driven by the definition of the appropriate 
  // compilation symbol (__OVERLAY__). In this case, we enable or disable the 
  // compilation of the relevant code.

#ifdef __OVERLAY__MIDRES__

    // In general, the compiler places the executable code in the "CODE" 
    // segment, while the non-modifiable data in the "RODATA" segment.
    // In order to save space, both of these segments are moved to the 
    // overlay segment, so that both the code and the data it uses are 
    // removed from the resident module.
    #pragma code-name ("OVERLAY3");
    #pragma rodata-name ("OVERLAY3");

#endif

/****************************************************************************
 ** RESIDENT VARIABLES SECTION
 ****************************************************************************/

 /****************************************************************************
  ** OVERLAYED FUNCTIONS SECTION
  ****************************************************************************/

  // The functions defined at this level can only be called up if the current
  // module has been loaded into memory. On the other hand, they can call any 
  // function declared at the resident module level.

void mr_enable(mr_screen _screen) {

    ENABLED_SCREEN = _screen;

}

unsigned char mr_save(char* _filename, mr_screen _screen) {

#ifndef __CBM__

    FILE* f = fopen(_filename, "wb");
    if (f == NULL) {
        return 0;
    }
    fwrite((unsigned char*)SCREEN_RAM_START_ADDRESS + _screen * SCREEN_RAM_SIZE, (unsigned)SCREEN_RAM_SIZE, 1, f);
    fclose(f);
    return 1;

#else

    return cbm_save(_filename, getcurrentdevice(), (unsigned char*)SCREEN_RAM_START_ADDRESS + _screen * SCREEN_RAM_SIZE, SCREEN_RAM_SIZE);

#endif


}

unsigned char mr_save_color(char* _filename, mr_screen _screen) {

#ifndef __CBM__

    FILE* f = fopen(_filename, "wb");
    if (f == NULL) {
        return 0;
    }
    fwrite((unsigned char*)COLOUR_RAM_START_ADDRESS, (unsigned)SCREEN_RAM_SIZE, 1, f);
    fclose(f);
    return 1;

#else

    return cbm_save(_filename, getcurrentdevice(), (unsigned char*)COLOUR_RAM_START_ADDRESS, SCREEN_RAM_SIZE);

#endif

}

void mr_compress(mr_screen _source, mr_screen _destination) {

    mr_mixel* source = (mr_mixel*)(SCREEN_RAM_START_ADDRESS + _source * SCREEN_RAM_SIZE);
    mr_mixel* destination = (mr_mixel*)(SCREEN_RAM_START_ADDRESS + _destination * SCREEN_RAM_SIZE);
    mr_color* sourceColor = (mr_color*)COLOUR_RAM_START_ADDRESS;

    int i;
    for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
        destination[i] = mr_pack_mixels(sourceColor[i], get_mixel_bits(source[i]));
    }

}

void mr_pack(mr_screen _source, mr_screen _destination, mr_half_screen _half_screen) {

    int i;
    mr_mixel* source = (mr_mixel*)(SCREEN_RAM_START_ADDRESS + _source * SCREEN_RAM_SIZE);
    mr_mixel* destination = (mr_mixel*)(SCREEN_RAM_START_ADDRESS + _destination * SCREEN_RAM_SIZE);

    if (_half_screen == mr_half_up) {
        destination += SCREEN_RAM_SIZE / 2;
    }

    for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i += 2) {
        destination[i / 2] = mr_pack_mixels(get_mixel_bits(source[i + 1]), get_mixel_bits(source[i]));
    }

}