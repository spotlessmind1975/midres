/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * KEYBOARD LIBRARY                                                         *
 ****************************************************************************/

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "midres.h"

#if ( !defined(__OVERLAY__MIDRES__) && defined(MIDRES_STANDALONE_KEYBOARD) ) || defined(__OVERLAY__MIDRES__)

// Overlay management is driven by the definition of the appropriate 
// compilation symbol (__OVERLAY__). In this case, we enable or disable the 
// compilation of the relevant code.

#ifdef __OVERLAY__MIDRES__

    // In general, the compiler places the executable code in the "CODE" 
    // segment, while the non-modifiable data in the "RODATA" segment.
    // In order to save space, both of these segments are moved to the 
    // overlay segment, so that both the code and the data it uses are 
    // removed from the resident module.
    #pragma code-name ("OVERLAY6");
    #pragma rodata-name ("OVERLAY6");

#endif

/****************************************************************************
 ** OVERLAYED FUNCTIONS SECTION
 ****************************************************************************/

 // The functions defined at this level can only be called up if the current
 // module has been loaded into memory. On the other hand, they can call any 
 // function declared at the resident module level.

unsigned char mr_key_pressed() {
    return mr_key_pressed_hd();
}

unsigned char mr_get_key_pressed() {
    return mr_get_key_pressed_hd();
}

void mr_wait(unsigned char _seconds) {

    mr_wait_hd(_seconds);

}

void mr_wait_jiffies(unsigned char _jiffies) {

    mr_wait_jiffies_hd(_jiffies);

}

void mr_sound_start(unsigned char _number) {
    mr_sound_start_hd(_number);
};

void mr_sound_change(int _parameter) {
    mr_sound_change_hd(_parameter);
}

void mr_sound_stop() {
    mr_sound_stop_hd();
};

void mr_start_frame() {
    mr_start_frame_hd();
}

void mr_end_frame(unsigned char _jiffies) {
    mr_end_frame_hd(_jiffies);
}


#endif