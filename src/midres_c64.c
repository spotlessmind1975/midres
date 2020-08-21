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

#ifdef __C64__

    void mr_init_hd() {

        unsigned char* vicRegister2 = (unsigned char*)0xd021;
        unsigned char* vicRegister3 = (unsigned char*)0xdd00;
        unsigned char* vicRegister4 = (unsigned char*)0xdd02;
        unsigned char* vicRegister = (unsigned char*)0xd018;
        unsigned char* basicRegister = (unsigned char*)0x0288;
        int i;
        unsigned char* dst = (unsigned char* )0x8c00, * src = (unsigned char*)0xd800;

        *vicRegister2 = MR_COLOR_BLACK;
        *vicRegister4 = (*vicRegister4) | 3;
        *vicRegister3 = ((*vicRegister3) & 0xfc) | 1; // bank 2
        *vicRegister = ((*vicRegister) & 0x0f) | (1 << 4); // screen 1
        *basicRegister = 0x80;

        for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
            *(dst + i) = *(src + i);
        }

    }

    void mr_show_hd(unsigned char _screen) {
        
        unsigned char* vicRegister = (unsigned char*)0xd018;
        unsigned char* vicRegister3 = (unsigned char*)0xdd00;
        unsigned char* basicRegister = (unsigned char*)0x0288;
        
        VISIBLE_SCREEN = _screen;
        ENABLED_SCREEN = _screen;

        *vicRegister = ((*vicRegister) & 0x0f) | (_screen << 4);
        *basicRegister = _screen << 2;

    }

    void mr_cleanup_hd() {

        unsigned char* vicRegister2 = (unsigned char*)0xd021;
        unsigned char* vicRegister3 = (unsigned char*)0xdd00;
        int i;
        unsigned char* dst = (unsigned char*)0xd800, * src = (unsigned char*)0x8c00;

        for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
            *(dst + i) = *(src + i);
        }

        *vicRegister2 = MR_COLOR_BLUE;
        *vicRegister3 = ((*vicRegister3) & 0xfc); // bank 3

        mr_show(1);

    }

    void mr_wait_vbl() {
        while (*((unsigned char*)0xd012) != 0xff) {}
    }

    void mr_doublebuffer_switch_hd(unsigned char _screen) {
        unsigned char _other = (_screen == DB1) ? DB2 : DB1;

        memcpy(SM(_screen), SM(_other), SCREEN_RAM_SIZE);
    }

    void mr_tileset_visible_hd(unsigned char _tileset) {

        /* Address 53272 ($D018)is a VIC - II register that generally tells the graphics 
            chip where to "look for graphics", in conjunction with both the text screenand with 
            bitmap graphics.

            When in text screen mode, the VIC - II looks to 53272 for information on where 
            the character setand text screen character RAM is located :

            The four most significant bits form a 4 - bit number in the range 0 thru 15 : 
                Multiplied with 1024 this gives the start address for the screen character RAM.
            Bits 1 thru 3 (weights 2 thru 8) form a 3 - bit number in the range 0 thru 7 : 
                Multiplied with 2048 this gives the start address for the character set.*/

        if (_tileset == 15) {
            _tileset = 4;
        }
        else {
            _tileset = 2;
        }

        *((unsigned char*)0xd018) = (*((unsigned char*)0xd018) & 0xf1) | (( _tileset & 0x07 )<<1);

    }

#endif