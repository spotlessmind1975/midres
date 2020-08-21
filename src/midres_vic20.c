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

#ifdef __VIC20__

    void mr_init_hd() {

        int i = 0;

        *((unsigned char*)0x900f) = (*((unsigned char*)0x900f) & 0x0f) | ( MR_COLOR_BLACK << 4 );
        // bit 7 is part of screen address
        *((unsigned char*)0x9002) = (*((unsigned char*)0x9002) & 0x7f) | ( (DEFAULT_SCREEN & 1) << 8 );
        // bits 4 - 7 is rest of video address
        *((unsigned char*)0x9005) = (*((unsigned char*)0x9005) & 0xcf) | (((DEFAULT_SCREEN >> 1) & 0x3) << 4);

        // *((unsigned char*)0x0288) = ((SCREEN_RAM_START_ADDRESS + DEFAULT_SCREEN * SCREEN_RAM_SIZE) & 0xff00) >> 8;

        //*((unsigned char*)0x0288)= 0x80;
        //*((unsigned char*)0x900f)= 8;

        for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
            putchar(' ');
        }

    }

    void mr_show_hd(unsigned char _screen) {

        VISIBLE_SCREEN = _screen;
        ENABLED_SCREEN = _screen;

        /*
            B 7-4   Bit 7   Screen Map	Colour Map
            1100	0	    $1000 (4096)	$9400 (37888)
            1100	1	    $1200 (4608)	$9600 (38400)
            1101	0	    $1400 (5120)	$9400 (37888)
            1101	1	    $1600 (5632)	$9600 (38400)
            1110	0	    $1800 (6144)	$9400 (37888)
            1110	1	    $1A00 (6656)	$9600 (38400)
            1111	0	    $1C00 (7168)	$9400 (37888)
            1111	1	    $1E00 (7680)	$9600 (38400)
        */

        // bit 7 is part of screen address
        *((unsigned char*)0x9002) = (*((unsigned char*)0x9002) & 0x7f) | ((_screen & 1) << 8);
        // bits 4 - 7 is rest of video address
        *((unsigned char*)0x9005) = ( *((unsigned char*)0x9005) & 0xcf ) | ( ( ( _screen >> 1 ) & 0x3 ) << 4 );

        // *((unsigned char*)0x0288) = ( ( SCREEN_RAM_START_ADDRESS + _screen * SCREEN_RAM_SIZE ) & 0xff00 ) >> 8;

    }

    void mr_cleanup_hd() {

        *((unsigned char*)0x900f) = (*((unsigned char*)0x900f) & 0x0f) | (MR_COLOR_WHITE << 4);

    }

    void mr_wait_vbl() {
        while (*((unsigned char*)0x9004) < 27) {}
    }

    void mr_doublebuffer_switch_hd(unsigned char _screen) {
        unsigned char _other = (_screen == DB1) ? DB2 : DB1;

        memcpy(SM(_screen), SM(_other), SCREEN_RAM_SIZE);
        memcpy(CM(_screen), CM(_other), SCREEN_RAM_SIZE);
    }

    void mr_tileset_visible_hd(unsigned char _tileset) {

        /*
            CHARACTER TABLE / SCREEN MEMORY ADDRESSES

            $9002 : Y------ -
            $9005 : YYYYXXXX

            X = chartable address
            Y = screen memory address


            The following table shows how the bottom four bits of memory location $9005
            specify the character table address.

            X       Location                  Contents
            Value  HEX     Decimal

            0    $8000   32768       Upper case normal characters
            1    $8400   33792       Upper case reversed characters
            2    $8800   34816       Lower case normal character
            3    $8C00   35840       Lower case reversed characters
            4    $9000   36864       unavailable
            5    $9400   37888       unavailable
            6    $9800   38912       unavailable
            7    $9C00   39936       unavailable
            8    $0000       0       unavailable
            9    $0400    1024       3K expansion
            10    $0800    2048       3K expansion
            11    $0C00    3072       3K expansion
            12    $1000    4096       RAM
            13    $1400    5120       RAM
            14    $1800    6144       RAM
            15    $1C00    7168       RAM

            */

        *((unsigned char*)0x9005) = (*((unsigned char*)0x9005) & 0xf0) | ( ( _tileset ) & 0xf );

    }

#endif