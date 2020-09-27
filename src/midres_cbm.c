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

#include "midres.h"

  /****************************************************************************
   ** RESIDENT VARIABLES SECTION
   ****************************************************************************/

#if defined(__IMG2MIDRES__) || defined(__CBM__)

unsigned char RENDERED_MIXELS_CBM[16] = {
   0x20, 0x6c, 0x7b, 0x62,
   0x7c, 0xe1, 0xff, 0xfe,
   0x7e, 0x7f, 0x61, 0xfc,
   0xe2, 0xfb, 0xec, 0xa0
};

#endif