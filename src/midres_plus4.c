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

#include "midres_hw.h"
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

#ifdef __PLUS4__
#ifndef __C16__

#define SET_VIDEO( _screen ) \
        *((unsigned char*)0xff14) = (*((unsigned char*)0xff14) & 0x07 ) | ( (_screen) << 3 );

#define SET_BACKGROUND_COLOR( _color ) \
        *((unsigned char*)0xff15) = _color;

#define WAIT_VBL( ) \
    while ((*(unsigned char*)0xff1c) & 1 == 0) {}

#define SET_CHARSET( _tileset ) \
    *((unsigned char *)0xff12) = (*((unsigned char*)0xff12) | 0xfb ) \
    *((unsigned char *)0xff07) = (*((unsigned char*)0xff07) | 0x80 ) \
    *((unsigned char *)0xff13) = ( *((unsigned char *)0xff13) & 0x03 ) | ( (_tileset) << 2 );

void mr_init_hd() {

    //SET_BACKGROUND_COLOR(MR_COLOR_BLACK);
    //SET_VIDEO(MR_SCREEN_DEFAULT);
    //SET_CHARSET(MR_TILESET_DEFAULT);

    VISIBLE_SCREEN = MR_SCREEN_DEFAULT;
    ENABLED_SCREEN = MR_SCREEN_DEFAULT;

}

void mr_show_hd(unsigned char _screen) {

    SET_VIDEO(_screen);

    VISIBLE_SCREEN = _screen;
    ENABLED_SCREEN = _screen;

}

void mr_cleanup_hd() {

    SET_BACKGROUND_COLOR(MR_COLOR_WHITE);
    SET_VIDEO(MR_SCREEN_DEFAULT);

}

void mr_wait_vbl() {
    WAIT_VBL();
}

void mr_doublebuffer_switch_hd(unsigned char _screen) {
    unsigned char _other = (_screen == DB1) ? DB2 : DB1;

    memcpy(SM(_screen), SM(_other), MR_SCREEN_RAM_SIZE >> 1);
    memcpy(CM(_screen), CM(_other), MR_SCREEN_RAM_SIZE >> 1);
}

void mr_tileset_visible_hd(unsigned char _tileset) {
    //SET_CHARSET(_tileset);
}

unsigned char mr_key_pressed_hd() {
    return (*(unsigned char*)0x00c5) != 0x40;
}

#endif
#endif