#if defined(__IMG2MIDRES__) || defined(__VANILLA__)

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

unsigned char MR_RENDERED_MIXELS_VANILLA[16] = {
   0x00, 0x01, 0x02, 0x03,
   0x04, 0x05, 0x06, 0x07,
   0x08, 0x09, 0x10, 0x11,
   0x12, 0x13, 0x14, 0x15
};

unsigned char* mr_translate_file_hd(mr_file _file) {
    return mr_translate_file_user(_file);
}

void mr_read_file_hd(unsigned int _file, unsigned int _offset, unsigned char* _dest, unsigned int _size) {
    FILE* f = fopen(mr_translate_file_hd(_file), "rb");
    if (f == NULL) {
        return;
    }
    fseek(f, _offset, SEEK_SET);
    fread(_dest, _size, 1, f);
    fclose(f);
}

void mr_tileset_copy_hd(unsigned char _source, unsigned char _dest) {
    mr_tileset_copy_memory_mapped(_source, _dest);
}

void mr_tileset_multicolor_to_monocolor_hd(unsigned char _source, unsigned char _starting, unsigned char _count) {
    mr_tileset_multicolor_to_monocolor_memory_mapped(_source, _starting, _count);
}

void mr_tile_redefine_hd(unsigned char _tileset, unsigned char _tile, unsigned char* _data) {
    mr_tile_redefine_memory_mapped(_tileset, _tile, _data);
}

void mr_tile_redefine_fill_hd(unsigned char _tileset, unsigned char _tile, unsigned char _data) {
    mr_tile_redefine_fill_memory_mapped(_tileset, _tile, _data);
}


#endif
