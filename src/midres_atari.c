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
#include <cc65.h>
#include <string.h>

#include "midres.h"

  /****************************************************************************
   ** RESIDENT VARIABLES SECTION
   ****************************************************************************/

   /****************************************************************************
    ** RESIDENT FUNCTIONS SECTION
    ****************************************************************************/

    // The functions defined at this level can only be called up if the current
    // module has been loaded into memory. On the other hand, they can call any 
    // function declared at the resident module level.

#ifdef __ATARI__

unsigned char RENDERED_MIXELS[16] = {
   0x00, 0x01, 0x02, 0x03,
   0x04, 0x05, 0x06, 0x07,
   0x08, 0x09, 0x10, 0x11,
   0x12, 0x13, 0x14, 0x15
};

mr_mixel* screenRam = NULL;

void mr_init_hd() {

    unsigned char* dlist = (unsigned char*)(*((unsigned char*)(560)) + (*((unsigned char*)(561))*256) );
    
    dlist += 4;

    screenRam = *(dlist)+( *(dlist + 1) * 256 );

    *(unsigned char*)(0x2E5) = 0xb0;

    memset(TM(MR_TILESET_0), 0, 0x400);

    mr_tileset_load("ztiles.bin", MR_TILESET_0, 0, 16);
    mr_tileset_visible(MR_TILESET_0);

}

void mr_show_hd(unsigned char _screen) {

    unsigned char* dlist = (unsigned char*)(*((unsigned char*)(560)) + (*((unsigned char*)(561)) * 256));

    VISIBLE_SCREEN = _screen;
    ENABLED_SCREEN = _screen;
    
    dlist += 5;

    /*dlist += 5;
    *(dlist) = ( (int)(SM(_screen)) & 0xff );
    ++dlist;
    *(dlist) = ( ( (int)(SM(_screen)) >> 8 ) & 0xff );*/

}

void mr_cleanup_hd() {
    mr_show_hd(MR_SCREEN_DEFAULT);
}

void mr_wait_vbl() {

}

void mr_doublebuffer_switch_hd(unsigned char _screen) {
    unsigned char _other = (_screen == DB1) ? DB2 : DB1;

    memcpy(SM(_screen), SM(_other), 0x400);
}

void mr_tileset_visible_hd(unsigned char _tileset) {
    *(unsigned char*)(0x2F4) = (unsigned char)((((int)(TM(_tileset))) >> 8) & 0xff);
}

unsigned char mr_key_pressed_hd() {
    unsigned char k = (*(unsigned char*)0x02fc);
    (*(unsigned char*)0x02fc) = 0x09;
    return k != 0x09;
}

unsigned char mr_get_key_pressed_hd() {
    unsigned char k = (*(unsigned char*)0x02fc);
    (*(unsigned char*)0x02fc) = 0x09;
    return k;
}

void mr_wait_hd(unsigned char _seconds) {

    while (_seconds) {
        mr_wait_jiffies_hd(60);
        --_seconds;
    }

}

void mr_wait_jiffies_hd(unsigned char _jiffies) {
    unsigned char now = *((unsigned char*)0x14);

    while ((*((unsigned char*)0x14) - now) < _jiffies) {
        ; // nop!
    }
}

// Hardware dependent sound library
void mr_sound_start_hd(unsigned char _number) {

}

// Hardware dependent sound library
void mr_sound_change_hd(int _parameter) {

}

// Hardware dependent sound library
void mr_sound_stop_hd() {

}

void mr_set_background_color_hd(unsigned char _color) {

}

unsigned char storedJiffy = 0;

void mr_start_frame_hd() {

    storedJiffy = *((unsigned char*)0x14);

}

void mr_end_frame_hd(unsigned char _jiffies) {

    while ((*((unsigned char*)0x14) - storedJiffy) < _jiffies) {
        ; // nop!
    }

}


#endif