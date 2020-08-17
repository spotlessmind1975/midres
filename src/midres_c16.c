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

#ifdef __C16__

    void mr_init_hd() {

        *((unsigned char*)0xff15) = MR_COLOR_BLACK;
        *((unsigned char*)0xff14) = (*((unsigned char*)0xff14) & 0x07 ) | ( (DEFAULT_SCREEN) << 3 );

    }

    void mr_show_hd(unsigned char _screen) {

        VISIBLE_SCREEN = _screen;
        ENABLED_SCREEN = _screen;
        *((unsigned char*)0xff14) = (*((unsigned char*)0xff14) & 0x07) | ( (_screen) << 3);

    }

    void mr_cleanup_hd() {

        *((unsigned char*)0xff15) = MR_COLOR_WHITE;

    }

    void mr_wait_vbl() {
        while (*((unsigned char*)0xff1c) & 1 == 0) { }
    }

    void mr_doublebuffer_switch_hd(unsigned char _screen) {
        unsigned char _other = (_screen == DB1) ? DB2 : DB1;

        memcpy(SM(_screen), SM(_other), SCREEN_RAM_SIZE>>1);
        memcpy(CM(_screen), CM(_other), SCREEN_RAM_SIZE>>1);
    }

#endif