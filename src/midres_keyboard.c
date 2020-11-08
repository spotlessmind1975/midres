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

mr_boolean mr_wait_or_keypress(unsigned char _seconds) {
    int jiffies = _seconds * 60;
    while (jiffies) {
        mr_wait_jiffies(10);
        jiffies -= 10;
        if (mr_key_pressed_hd()) {
            return mr_true;
        }
    }
    return mr_false;
}


mr_boolean mr_wait_or_get_keypressed(unsigned char _seconds) {
    int jiffies = _seconds * 60;
    unsigned char key = MR_KEY_NONE;
    while (jiffies) {
        mr_wait_jiffies(10);
        jiffies -= 10;
        key = mr_get_key_pressed_hd();
        if (key != MR_KEY_NONE) {
            return key;
        }
    }
    return MR_KEY_NONE;
}

mr_boolean mr_wait_jiffies_or_keypress(unsigned char _jiffies) {
    while (_jiffies) {
        mr_wait_jiffies(10);
        _jiffies -= 10;
        if (mr_key_pressed_hd()) {
            return mr_true;
        }
    }
    return mr_false;
}

void mr_sound_start(unsigned char _number) {
    mr_sound_start_hd(0, _number);
};

void mr_sound_change(int _parameter) {
    mr_sound_change_hd(0, _parameter);
}

void mr_sound_stop() {
    mr_sound_stop_hd(0);
};

void mr_start_frame() {
    mr_start_frame_hd();
}

void mr_end_frame(unsigned char _jiffies) {
    mr_end_frame_hd(_jiffies);
}

void mr_sound_start_channel(unsigned char _channel, unsigned char _number) {
    mr_sound_start_hd(_channel, _number);
}

void mr_sound_change_channel(unsigned char _channel, int _parameter) {
    mr_sound_change_hd(_channel, _parameter);
}

void mr_sound_stop_channel(unsigned char _channel) {
    mr_sound_stop_hd(_channel);
}

unsigned char mr_joy(unsigned char _number) {
    return mr_joy_hd(_number);
}


#endif