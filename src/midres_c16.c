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

#ifdef __C16__

void mr_init_hd() {

    unsigned char* tedRegister2 = (unsigned char*)0xff15;

    *tedRegister2 = MR_COLOR_BLACK;

}

void mr_show_hd(unsigned char _screen) {

    VISIBLE_SCREEN = _screen;
    ENABLED_SCREEN = _screen;

}

void mr_cleanup_hd() {

    unsigned char* tedRegister2 = (unsigned char*)0xff15;

    *tedRegister2 = MR_COLOR_BLUE;

}

#endif