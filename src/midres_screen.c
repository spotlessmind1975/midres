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
#include <string.h>

#include "midres.h"

#ifdef __CBM__
    #include <cc65.h>
    #include <cbm.h>
    #include <device.h>
#else

#endif

#if ( !defined(__OVERLAY__MIDRES__) && defined(MIDRES_STANDALONE_SCREEN) ) || defined(__OVERLAY__MIDRES__)

  // Overlay management is driven by the definition of the appropriate 
  // compilation symbol (__OVERLAY__). In this case, we enable or disable the 
  // compilation of the relevant code.

#ifdef __OVERLAY__MIDRES__

    // In general, the compiler places the executable code in the "CODE" 
    // segment, while the non-modifiable data in the "RODATA" segment.
    // In order to save space, both of these segments are moved to the 
    // overlay segment, so that both the code and the data it uses are 
    // removed from the resident module.
    #pragma code-name ("OVERLAY1");
    #pragma rodata-name ("OVERLAY1");

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

void mr_clear(mr_screen _screen) {

    int i;
    mr_mixel* screen = SM(_screen);
    mr_color* color = CM(_screen);

    for (i = 0; i < MR_SCREEN_HEIGHT * MR_SCREEN_WIDTH; ++i) {
        screen[i] = RENDERED_MIXELS[0];
        color[i] = MR_COLOR_BLACK;
    }
}

void mr_show(mr_screen _screen) {

    mr_show_hd(_screen);

    VISIBLE_SCREEN = _screen;
    ENABLED_SCREEN = _screen;

}

void mr_enable(mr_screen _screen) {

    ENABLED_SCREEN = _screen;

}

// Setup the double buffer (using screens SCREEN_DB1 and SCREEN_DB2)
void mr_doublebuffer_init() {
    VISIBLE_SCREEN = MR_DB1;
    ENABLED_SCREEN = MR_DB2;
}

// Switch double buffering
void mr_doublebuffer_switch() {
    
    mr_wait_vbl();

    if (VISIBLE_SCREEN == MR_DB1) {
        mr_show(MR_DB2);
        mr_doublebuffer_switch_hd(MR_DB1);
        mr_enable(MR_DB1);
    }
    else {
        mr_show(MR_DB1);
        mr_doublebuffer_switch_hd(MR_DB2);
        mr_enable(MR_DB2);
    }
}

unsigned char mr_load(char* _filename, mr_screen _screen) {

#if __ATARI__

    FILE* f = fopen(_filename, "rb");
    if (f == NULL) {
        return 0;
    }
    fread(SM(_screen), MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT, 1, f);
    fclose(f);
    return 1;

#elif __CBM__

    return cbm_load(_filename, getcurrentdevice(), SM(_screen));

#endif

}

#ifdef MIDRES_STANDALONE_FILE

unsigned char mr_load_screen(mr_file _index, mr_screen _screen) {
    mr_read_file(_index, SM(_screen), MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT);
    return 0;
}

#endif

unsigned char mr_load_color(char* _filename, mr_screen _screen) {

#if __ATARI__

    FILE* f = fopen(_filename, "rb");
    if (f == NULL) {
        return 0;
    }
    fread(CM(_screen), MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT, 1, f);
    fclose(f);
    return 1;

#elif __CBM__

    _screen = 0;
    return cbm_load(_filename, getcurrentdevice(), CM(_screen));

#endif

}

#ifdef MIDRES_STANDALONE_FILE

unsigned char mr_load_screen_color(mr_file _index, mr_screen _screen) {
    mr_read_file(_index, CM(_screen), MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT);
    _screen=0;
    return 0;
}

#endif

void mr_uncompress(mr_screen _source, mr_screen _destination) {

    mr_mixel* source = SM(_source);
    mr_mixel* destination = SM(_destination);
    mr_color* destinationColor = CM(_destination);

    int i;
    for (i = 0; i < MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT; ++i) {
        destination[i] = RENDERED_MIXELS[mr_unpack_pmixel_lo(source[i])];
        destinationColor[i] = mr_unpack_pmixel_hi(source[i]) + MR_BRIGHTNESS;
    }

}

void mr_unpack(mr_screen _source, mr_screen _destination, mr_half_screen _half_screen) {

    int i;
    mr_mixel* source = SM(_source);
    mr_mixel* destination = SM(_destination);

    if (_half_screen == mr_half_up) {
        source += (MR_SCREEN_RAM_SIZE >> 1);
    }

    for (i = 0; i < MR_SCREEN_WIDTH * ( MR_SCREEN_HEIGHT >> 1 ); ++i) {
        destination[(i << 1)] = RENDERED_MIXELS[mr_unpack_pmixel_lo(source[i])];
        destination[(i << 1) + 1] = RENDERED_MIXELS[mr_unpack_pmixel_hi(source[i])];
    }

}

void mr_set_background_color(mr_color _color) {
    mr_set_background_color_hd(_color);
}

void mr_set_border_color(mr_color _color) {
    mr_set_border_color_hd(_color);
}

#endif