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

#if defined(__IMG2MIDRES__) || defined(__VANILLA__)

unsigned char RENDERED_MIXELS_VANILLA[16] = {
   0x00, 0x01, 0x02, 0x03,
   0x04, 0x05, 0x06, 0x07,
   0x08, 0x09, 0x10, 0x11,
   0x12, 0x13, 0x14, 0x15
};

unsigned char* mr_translate_file_hd(mr_file _file) {
    return mr_translate_file_user(_file);
}

void mr_read_file(unsigned int _file, unsigned int _offset, unsigned char* _dest, unsigned int _size) {

}

#endif
