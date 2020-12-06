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

#if ( !defined(__OVERLAY__MIDRES__) && defined(MIDRES_STANDALONE_TILE_PROCESSING) ) || defined(__OVERLAY__MIDRES__)

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

void mr_tile_prepare_horizontal_multicolor(mr_tileset _tileset, mr_tile _source, mr_tile _destination);
void mr_tile_prepare_horizontal_extended_multicolor(mr_tileset _tileset, mr_tile _source, mr_tile _w, mr_tile _h, mr_tile _destination);
void mr_tile_prepare_roll_horizontal_multicolor(mr_tileset _tileset, mr_tile _source, mr_tile _destination);

// The functions defined at this level can only be called up if the current
// module has been loaded into memory. On the other hand, they can call any 
// function declared at the resident module level.

// Redefine a subset of N tiles by "shifting" horizontally a tile
void mr_tile_prepare_horizontal_monocolor_memory_mapped(mr_tileset _tileset, mr_tile _source, mr_tile _destination) {
    mr_mixel* source = (mr_mixel*)(MR_TM(_tileset) + _source * 8);
    mr_mixel* destination = (mr_mixel*)(MR_TM(_tileset) + _destination * 8);

    mr_position i, b;

    for (i = 0; i < 9; ++i) {
        for (b = 0; b < 8; ++b, ++source, ++destination) {
            mr_mixel d = *((mr_mixel*)source);
            mr_mixel m = d >> i;
            *destination = m;
        }
        source -= 8;
    }

    for (i = 0; i < 8; ++i) {
        for (b = 0; b < 8; ++b, ++source, ++destination) {
            mr_mixel d = *((mr_mixel*)source);
            mr_mixel n = d & (0xff >> (7 - i));
            *destination = (n << (7 - i));
        }
        source -= 8;
    }
}

void mr_tile_prepare_horizontal_monocolor(mr_tileset _tileset, mr_tile _source, mr_tile _destination) {
    mr_tile_prepare_horizontal_monocolor_hd(_tileset, _source, _destination);
}

void mr_tile_prepare_horizontal(mr_tileset _tileset, mr_tile _source, mr_tile _destination) {

#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    if (MR_MULTICOLOR) {
        mr_tile_prepare_horizontal_multicolor(_tileset, _source, _destination);
    }
    else {
#endif
        mr_tile_prepare_horizontal_monocolor(_tileset, _source, _destination);
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    }
#endif

}

// Redefine a subset of N tiles by "shifting" horizontally a tile
void mr_tile_prepare_horizontal_extended_monocolor_memory_mapped(mr_tileset _tileset, mr_tile _source, mr_tile _w, mr_tile _h, mr_tile _destination) {
    mr_mixel* source = (mr_mixel*)(MR_TM(_tileset) + _source * 8);
    mr_mixel* destination = (mr_mixel*)(MR_TM(_tileset) + _destination * 8);

    mr_position i, b;

    mr_position j, k;

    for (j = 0; j < _h; ++j) {
        for (i = 0; i < 9; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel e = *((mr_mixel*)source);
                mr_mixel m = e >> i;
                *destination = m;
            }
            source -= 8;
        }

        for (k = 0; k < (_w - 1); ++k) {
            for (i = 0; i < 9; ++i) {
                for (b = 0; b < 8; ++b, ++source, ++destination) {
                    mr_mixel d = *((mr_mixel*)source);
                    mr_mixel e = *((mr_mixel*)source + 8);
                    mr_mixel m = (e >> i) | (d << (8 - i));
                    *destination = m;
                }
                source -= 8;
            }
            source += 8;
        }

        for (i = 0; i < 9; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel d = *((mr_mixel*)source);
                mr_mixel n = d & (0xff >> (8 - i));
                *destination = (n << (8 - i));
            }
            source -= 8;
        }
        source += 8;
    }

}

void mr_tile_prepare_horizontal_extended_monocolor(mr_tileset _tileset, mr_tile _source, mr_tile _w, mr_tile _h, mr_tile _destination) {
    mr_tile_prepare_horizontal_extended_monocolor_hd(_tileset, _source, _w, _h, _destination);
}

void mr_tile_prepare_horizontal_extended(mr_tileset _tileset, mr_tile _source, mr_tile _w, mr_tile _h, mr_tile _destination) {
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    if (MR_MULTICOLOR) {
        mr_tile_prepare_horizontal_extended_multicolor(_tileset, _source, _w, _h, _destination);
    }
    else {
#endif
        mr_tile_prepare_horizontal_extended_monocolor(_tileset, _source, _w, _h, _destination);
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    }
#endif
}

// Redefine a subset of N tiles by "shifting" vertically a tile
void mr_tile_prepare_vertical_memory_mapped(mr_tileset _tileset, mr_tile _source, mr_tile _destination) {

    mr_tile* source = (mr_tile*)(MR_TM(_tileset) + _source * 8);
    mr_tile* destination = (mr_tile*)(MR_TM(_tileset) + _destination * 8);

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

// Redefine a subset of N tiles by "shifting" vertically a tile
void mr_tile_prepare_vertical(mr_tileset _tileset, mr_tile _source, mr_tile _destination) {
    mr_tile_prepare_vertical_hd(_tileset, _source, _destination);
}

// Redefine a subset of N tiles by "shifting" vertically a tile
void mr_tile_prepare_vertical_extended_memory_mapped(mr_tileset _tileset, mr_tile _source, mr_position _w, mr_position _h, mr_tile _destination) {
    mr_tile* source = (mr_tile*)(MR_TM(_tileset) + _source * 8);
    mr_tile* destination = (mr_tile*)(MR_TM(_tileset) + _destination * 8);

    int i, b;
    int w = _w, k = 0;

    for (; w != 0; --w) {
        for (i = 0; i < 9; ++i) {
            for (b = 0; b < i; ++b, ++destination) {
                *destination = 0x00;
            }
            for (b = 0; b < (8 - i); ++b, ++source, ++destination) {
                *destination = *(source);
            }
            source -= b;
        }

        for (k = 0; k < (_h - 1); ++k) {
            for (i = 0; i < 9; ++i) {
                for (b = 0; b < i; ++b, ++destination, ++source) {
                    *destination = *(source + (k * _w * 8) + (8 - i));
                }
                source -= b;
                for (b = 0; b < (8 - i); ++b, ++source, ++destination) {
                    *destination = *(source + ((k + 1) * _w * 8));
                }
                source -= b;
            }
        }

        for (i = 0; i < 9; ++i) {
            for (b = 0; b < i; ++b, ++destination, ++source) {
                *destination = *(source + ((_h - 1) * _w * 8) + (8 - i));
            }
            source -= b;
            for (b = 0; b < (8 - i); ++b, ++destination) {
                *destination = 0;
            }
        }
        source += 8;
    }
}

void mr_tile_prepare_vertical_extended(mr_tileset _tileset, mr_tile _source, mr_position _w, mr_position _h, mr_tile _destination) {
    mr_tile_prepare_vertical_extended_hd(_tileset, _source, _w, _h, _destination);
}

// Redefine a subset of N tiles by "rolling" horizontally a tile
void mr_tile_prepare_roll_horizontal_monocolor_memory_mapped(mr_tileset _tileset, mr_tile _source, mr_tile _destination) {
    mr_tile* source = (mr_tile*)(MR_TM(_tileset) + _source * 8);
    mr_tile* destination = (mr_tile*)(MR_TM(_tileset) + _destination * 8);

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

void mr_tile_prepare_roll_horizontal_monocolor(mr_tileset _tileset, mr_tile _source, mr_tile _destination) {
    mr_tile_prepare_roll_horizontal_monocolor_hd(_tileset, _source, _destination);
}

// Redefine a subset of N tiles by "rolling" horizontally a tile
void mr_tile_prepare_roll_horizontal(mr_tileset _tileset, mr_tile _source, mr_tile _destination) {
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    if (MR_MULTICOLOR) {
        mr_tile_prepare_roll_horizontal_multicolor(_tileset, _source, _destination);
    }
    else {
#endif
        mr_tile_prepare_roll_horizontal_monocolor(_tileset, _source, _destination);
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    }
#endif
}

mr_mixel rollBuffer[8];

// Roll horizontally a tile
void mr_tile_roll_horizontal_memory_mapped(mr_tileset _tileset, mr_tile _destination, mr_direction _direction) {

    mr_mixel* source;
    mr_mixel* destination;
    mr_mixel* temp = &rollBuffer[0];
    mr_position i, b;

    if (_direction == mr_direction_right) {
        source = (mr_mixel*)(MR_TM(_tileset) + (_destination + 1) * 8);
        destination = (mr_mixel*)(MR_TM(_tileset) + _destination * 8);

        for (b = 0; b < 8; ++b, ++source, ++temp) {
            mr_mixel d = *((mr_mixel*)source - 8);
            *temp = d;
        }
        temp -= 8;
        source -= 8;
        for (i = 0; i < 6; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel d = *((mr_mixel*)source);
                *destination = d;
            }
        }
        for (b = 0; b < 8; ++b, ++destination, ++temp) {
            *((mr_mixel*)destination) = *temp;
        }
    }
    else {
        source = (mr_mixel*)(MR_TM(_tileset) + (_destination + 6) * 8);
        destination = (mr_mixel*)(MR_TM(_tileset) + (_destination + 7) * 8);
        for (b = 0; b < 8; ++b, ++destination, ++temp) {
            mr_mixel d = *((mr_mixel*)destination);
            *temp = d;
        }
        temp -= 8;
        destination -= 8;
        for (i = 0; i < 7; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel d = *((mr_mixel*)source);
                *destination = d;
            }
            source -= 16;
            destination -= 16;
        }
        for (b = 0; b < 8; ++b, ++destination, ++temp) {
            *((mr_mixel*)destination) = *temp;
        }
    }

}

void mr_tile_roll_horizontal(mr_tileset _tileset, mr_tile _destination, mr_direction _direction) {
    mr_tile_roll_horizontal_hd(_tileset, _destination, _direction);
}

// Roll horizontally a tile
void mr_tile_roll_horizontal_on_place_memory_mapped(mr_tileset _tileset, mr_tile _destination, mr_direction _direction, mr_tile _place, mr_position* _index) {

    mr_mixel* source;
    mr_mixel* destination;
    mr_position b;

    source = (mr_mixel*)(MR_TM(_tileset) + (_destination * 8) + (*_index) * 8);
    destination = (mr_mixel*)(MR_TM(_tileset) + _place * 8);
    if (_direction == mr_direction_right) {
        ++(*_index);
        if ((*_index) > 8) {
            *_index = 0;
        }
    }
    else {
        --(*_index);
        if ((*_index) == 255) {
            *_index = 7;
        }
    }
    for (b = 0; b < 8; ++b, ++source, ++destination) {
        mr_mixel d = *((mr_mixel*)source);
        *destination = d;
    }

}

void mr_tile_roll_horizontal_on_place(mr_tileset _tileset, mr_tile _destination, mr_direction _direction, mr_tile _place, mr_position* _index) {
    mr_tile_roll_horizontal_on_place_hd(_tileset, _destination, _direction, _place, _index);
}

// Redefine a subset of N tiles by "rolling" vertically a tile
void mr_tile_prepare_roll_vertical_memory_mapped(mr_tileset _tileset, mr_tile _source, mr_tile _destination) {
    mr_tile* source = (mr_tile*)(MR_TM(_tileset) + _source * 8);
    mr_tile* destination = (mr_tile*)(MR_TM(_tileset) + _destination * 8);

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

void mr_tile_prepare_roll_vertical(mr_tileset _tileset, mr_tile _source, mr_tile _destination) {
    mr_tile_prepare_roll_vertical_hd(_tileset, _source, _destination);
}

// Roll vertically a tile
void mr_tile_roll_vertical_memory_mapped(mr_tileset _tileset, mr_tile _destination, mr_direction _direction) {

    mr_mixel* source;
    mr_mixel* destination;
    mr_mixel* temp = &rollBuffer[0];
    mr_position i, b;

    if (_direction == mr_direction_up) {
        source = (mr_mixel*)(MR_TM(_tileset) + (_destination + 1) * 8);
        destination = (mr_mixel*)(MR_TM(_tileset) + _destination * 8);

        for (b = 0; b < 8; ++b, ++source, ++temp) {
            mr_mixel d = *((mr_mixel*)source - 8);
            *temp = d;
        }
        temp -= 8;
        source -= 8;
        for (i = 0; i < 6; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel d = *((mr_mixel*)source);
                *destination = d;
            }
        }
        for (b = 0; b < 8; ++b, ++destination, ++temp) {
            *((mr_mixel*)destination) = *temp;
        }
    }
    else {
        source = (mr_mixel*)(MR_TM(_tileset) + (_destination + 6) * 8);
        destination = (mr_mixel*)(MR_TM(_tileset) + (_destination + 7) * 8);
        for (b = 0; b < 8; ++b, ++destination, ++temp) {
            mr_mixel d = *((mr_mixel*)destination);
            *temp = d;
        }
        temp -= 8;
        destination -= 8;
        for (i = 0; i < 7; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel d = *((mr_mixel*)source);
                *destination = d;
            }
            source -= 16;
            destination -= 16;
        }
        for (b = 0; b < 8; ++b, ++destination, ++temp) {
            *((mr_mixel*)destination) = *temp;
        }
    }


}

void mr_tile_roll_vertical(mr_tileset _tileset, mr_tile _destination, mr_direction _direction) {
    mr_tile_roll_vertical_hd(_tileset, _destination, _direction);
}

// Roll vertically a tile
void mr_tile_roll_vertical_on_place(mr_tileset _tileset, mr_tile _destination, mr_direction _direction, mr_tile _place, mr_position* _index) {
    mr_tile_roll_horizontal_on_place(_tileset, _destination, _direction, _place, _index);
}

#endif
