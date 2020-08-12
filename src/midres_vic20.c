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

#ifdef __VIC20__

    void mr_init_hd() {

        int i = 0;

        *((unsigned char*)0x900f)= 8;

        for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
            putchar(' ');
        }

    }

    void mr_show_hd(unsigned char _screen) {

        (unsigned char)_screen;
        // unsigned char* vicRegister1 = (unsigned char*)0x9005;
        // unsigned char* vicRegister2 = (unsigned char*)0x9002;

        // *vicRegister1 = ((*vicRegister1) & 0x0f) | ( ( 0x1100 | _screen ) << 4 );
        // *vicRegister2 = ((*vicRegister2) & 0x7f);

    }

    void mr_cleanup_hd() {

    }

#endif