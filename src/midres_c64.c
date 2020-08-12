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

#ifdef __C64__

    void mr_init_hd() {

        unsigned char* vicRegister2 = (unsigned char*)0xd021;
        int i;
        unsigned char* dst = (unsigned char* )0x3c00, * src = (unsigned char*)0xd800;

        *vicRegister2 = MR_COLOR_BLACK;

        for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
            *(dst + i) = *(src + i);
        }

    }

    void mr_show_hd(unsigned char _screen) {

        unsigned char* vicRegister = (unsigned char*)0xd018;
        unsigned char* basicRegister = (unsigned char*)0x0288;
        
        VISIBLE_SCREEN = _screen;
        ENABLED_SCREEN = _screen;

        *vicRegister = ((*vicRegister) & 0x0f) | (_screen << 4);
        *basicRegister = _screen << 2;

    }

    void mr_cleanup_hd() {

        unsigned char* vicRegister2 = (unsigned char*)0xd021;
        int i;
        unsigned char* dst = (unsigned char*)0xd800, * src = (unsigned char*)0x3c00;

        for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
            *(dst + i) = *(src + i);
        }

        *vicRegister2 = MR_COLOR_BLUE;

        mr_show(1);

    }

#endif