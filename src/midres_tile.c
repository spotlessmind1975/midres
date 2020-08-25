/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * BITBLIT LIBRARY                                                          *
 ****************************************************************************/

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>

#include "midres.h"

#if ( !defined(__OVERLAY__MIDRES__) && defined(MIDRES_STANDALONE_TILE) ) || defined(__OVERLAY__MIDRES__)

  // Overlay management is driven by the definition of the appropriate 
  // compilation symbol (__OVERLAY__). In this case, we enable or disable the 
  // compilation of the relevant code.

#ifdef __OVERLAY__MIDRES__

    // In general, the compiler places the executable code in the "CODE" 
    // segment, while the non-modifiable data in the "RODATA" segment.
    // In order to save space, both of these segments are moved to the 
    // overlay segment, so that both the code and the data it uses are 
    // removed from the resident module.
    #pragma code-name ("OVERLAY6");
    #pragma rodata-name ("OVERLAY6");

#endif

/****************************************************************************
 ** OVERLAYED FUNCTIONS SECTION
 ****************************************************************************/

 // The functions defined at this level can only be called up if the current
 // module has been loaded into memory. On the other hand, they can call any 
 // function declared at the resident module level.

// Set the visible tilesect.
void mr_tileset_visible(mr_tileset _tileset) {
    mr_tileset_visible_hd(_tileset);
}

// Copy a tileset over another.
void mr_tileset_copy(mr_tileset _source, mr_tileset _destination) {
    mr_position w = 255, b = 0;
    mr_mixel* source = TM(_source);
    mr_mixel* destination = TM(_destination);
    for (--w; w != 255; --w) {
        for (b = 0; b < 8; ++b, ++source, ++destination) {
            *destination = *source;
        }
    }
}

// Redefine a tile using the given data.
void mr_tile_redefine(mr_tileset _tileset, mr_tile _tile, mr_mixel* _data) {
    mr_mixel* destination = (mr_mixel*)(TM(_tileset) + _tile*8);
    mr_position b = 0;
    for (b = 0; b < 8; ++b, ++destination, ++_data) {
        *destination = *_data;
    }
}

// Redefine a subset of N tiles by "shifting" horizontally a tile
void mr_tile_prepare_horizontal(mr_tileset _tileset, mr_tile _source, mr_tile _destination) {
    mr_mixel* source = (mr_mixel*)(TM(_tileset) + _source*8);
    mr_mixel* destination = (mr_mixel*)(TM(_tileset) + _destination * 8);

    mr_position i, b;
    
    for (i = 0; i < 9; ++i) {
        for (b = 0; b < 8; ++b, ++source, ++destination) {
            mr_mixel d = *((mr_mixel*)source);
            mr_mixel m = d >> i;
            *destination = m;
        }
        source -= 8;
    }

    for (i = 1; i < 9; ++i) {
        for (b = 0; b < 8; ++b, ++source, ++destination) {
            mr_mixel d = *((mr_mixel*)source);
            mr_mixel n = d & (0xff >> (8 - i));
            *destination = (n << (8 - i));
        }
        source -= 8;
    }

}

// Redefine a subset of N tiles by "shifting" vertically a tile
void mr_tile_prepare_vertical(mr_tileset _tileset, mr_tile _source, mr_tile _destination) {
    mr_tile* source = (mr_tile*)(TM(_tileset) + _source*8);
    mr_tile* destination = (mr_tile*)(TM(_tileset) + _destination * 8);

    mr_position i, b;

    for (i = 0; i < 9; ++i) {
        for (b = 0; b < i; ++b, ++destination) {
            *destination = 0x00;
        }
        for (b = 0; b < (8 - i); ++b, ++source, ++destination) {
            *destination = *(source);
        }
        source -= b;
    }

    for (i = 8; i != 255; --i) {
        for (b = 0; b < (8 - i); ++b, ++source, ++destination) {
            *destination = *(source + i);
        }
        source -= b;
        for (; b < 8; ++b, ++destination) {
            *destination = 0x00;
        }
    }
}

// Redefine a subset of N tiles by "rolling" horizontally a tile
void mr_tile_hrol(mr_tileset _tileset, mr_tile _source, mr_tile _destination) {
    mr_tile* source = (mr_tile*)(TM(_tileset) + _source * 8);
    mr_tile* destination = (mr_tile*)(TM(_tileset) + _destination * 8);

    mr_position i, b;

    for (i = 0; i < 8; ++i) {
        for (b = 0; b < 8; ++b, ++source, ++destination) {
            mr_mixel d = *((mr_tile*)source);
            mr_mixel m = d >> i, n = d & (0xff >> (8 - i));
            *destination = m | (n << (8 - i));
        }
        source -= 8;
    }

}

// Redefine a subset of N tiles by "rolling" vertically a tile
void mr_tile_vrol(mr_tileset _tileset, mr_tile _source, mr_tile _destination) {
    mr_tile* source = (mr_tile*)(TM(_tileset) + _source * 8);
    mr_tile* destination = (mr_tile*)(TM(_tileset) + _destination * 8);

    mr_position i, b;

    for (i = 0; i < 8; ++i) {
        for (b = 0; b < (8 - i); ++b, ++source, ++destination) {
            *destination = *(source + i);
        }
        source -= b;
        for (b = 0; b < i; ++b, ++source, ++destination) {
            *destination = *(source);
        }
        source -= b;
    }
}

// Writes a tile into a bitmap.
void _mr_puttile(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile, mr_color _color) {

    int offset;

    offset = _y * MR_SCREEN_WIDTH + _x;

    _screen[offset] = _tile;
    _colormap[offset] = _color;

}

// Writes a tile into a bitmap at *precise* horizontal position.
void _mr_tile_moveto_horizontal(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_color _color) {

    int offset;

    offset = (_y>>3) * MR_SCREEN_WIDTH + (_x>>3);

    _screen[offset] = _tile + (_x & 0x07) + 1;
    _colormap[offset] = _color;
    if (( (_x>>3) + 1 ) < MR_SCREEN_WIDTH) {
        _screen[offset + 1] = _tile + (_x & 0x07) + 9;
        _colormap[offset + 1] = _color;
    }

}

// Writes a tile into a bitmap at *precise* vertical position.
void _mr_tile_moveto_vertical(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_color _color) {

    int offset;

    offset = (_y >> 3) * MR_SCREEN_WIDTH + (_x >> 3);

    _screen[offset] = _tile + (_y & 0x07) + 1;
    _colormap[offset] = _color;
    if (( offset + MR_SCREEN_WIDTH) < MR_SCREEN_RAM_SIZE) {
        _colormap[offset + MR_SCREEN_WIDTH] = _color;
        _screen[offset + MR_SCREEN_WIDTH] = _tile + (_y & 0x07) + 10;
    }
}

// Clear a tile of a bitmap.
void _mr_cleartile(mr_mixel* _screen, mr_position _x, mr_position _y) {

    int offset;

    offset = _y * MR_SCREEN_WIDTH + _x;

    _screen[offset] = RENDERED_MIXELS[0];

}

// Reads a tile from a bitmap.
mr_color _mr_gettile(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y) {

    int offset;

    offset = _y * MR_SCREEN_WIDTH + _x;

    return _screen[offset];

}

// Load a tileset (or part of it)
void mr_tileset_load(unsigned char* _filename, mr_tileset _tileset, mr_tile _starting, mr_tile _count) {

    FILE* f = fopen(_filename, "rb");
    if (f == NULL) {
        return;
    }
    fread(TM(_tileset) + 8 * _starting, _count * 8, 1, f);
    fclose(f);

}
#endif
