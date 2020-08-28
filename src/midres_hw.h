/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

#ifndef _MIDRES_HW_H_
#define _MIDRES_HW_H_

#ifdef __C64__
#include "midres_hw_c64.h"
#elif __VIC20__
#ifdef __24K__
#include "midres_hw_vic20_32k.h"
#else
#include "midres_hw_vic20.h"
#endif
#elif __PLUS4__
#include "midres_hw_plus4.h"
#elif __C16__
#include "midres_hw_c16.h"
#else
#include "midres_hw_vanilla.h"
#endif

/************************************************************************
 * ------ FUNCTIONS
 ************************************************************************/

// Hardware dependent initialization.
void mr_init_hd();

// Hardware dependent screen management.
void mr_show_hd(unsigned char _screen);

// Hardware dependent cleanup.
void mr_cleanup_hd();

// Hardware dependend waiting for VBL
void mr_wait_vbl();

// Hardware dependent double buffer switch
void mr_doublebuffer_switch_hd(unsigned char _screen);

// Hardware dependent tile set
void mr_tileset_visible_hd(unsigned char _tileset);

// Hardware dependent key press listener
unsigned char mr_key_pressed_hd();
#endif