/*****************************************************************************
 * MIDRES Library - an isomorphic gamelib for retrocomputers                 *
 *****************************************************************************
 * Copyright 2020 Marco Spedaletti (asimov@mclink.it)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *----------------------------------------------------------------------------
 * Concesso in licenza secondo i termini della Licenza Apache, versione 2.0
 * (la "Licenza"); � proibito usare questo file se non in conformit� alla
 * Licenza. Una copia della Licenza � disponibile all'indirizzo:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Se non richiesto dalla legislazione vigente o concordato per iscritto,
 * il software distribuito nei termini della Licenza � distribuito
 * "COS� COM'�", SENZA GARANZIE O CONDIZIONI DI ALCUN TIPO, esplicite o
 * implicite. Consultare la Licenza per il testo specifico che regola le
 * autorizzazioni e le limitazioni previste dalla medesima.
 ****************************************************************************/

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>

#include "midres.h"

#if defined(MIDRES_STANDALONE_TILE)

void _mr_tile_moveto_horizontal_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_color _color);
void _mr_tile_moveto_horizontal_extended_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_position _w, mr_position _h, mr_color _color);
void _mr_tile_moveto_vertical_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_color _color);
void _mr_tile_moveto_vertical_extended_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_position _w, mr_position _h, mr_color _color);
void _mr_puttile_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile, mr_color _color);
void _mr_puttiles_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile_start, mr_tile _tile_count, mr_color _color);
void _mr_putetiles_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile_start, mr_position _w, mr_position _h, mr_color _color);
void _mr_putftiles_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile_start, mr_position _w, mr_position _h, mr_color _color);
void _mr_vtiles_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y1, mr_position _y2, mr_tile _tile, mr_color _color);
void _mr_htiles_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x1, mr_position _x2, mr_position _y, mr_tile _tile, mr_color _color);

// Set the visible tilesect.
void mr_tileset_visible(mr_tileset _tileset) {
    mr_tileset_visible_hd(_tileset);
}

void mr_tileset_copy_memory_mapped(mr_tileset _source, mr_tileset _destination) {
    mr_position w = 255, b = 0;
    mr_mixel* source = MR_TM(_source);
    mr_mixel* destination = MR_TM(_destination);
    for (--w; w != 255; --w) {
        for (b = 0; b < 8; ++b, ++source, ++destination) {
            *destination = *source;
        }
    }
}

// Downgrade a tileset from multicolor to monocolor.
void mr_tileset_multicolor_to_monocolor_memory_mapped(mr_tileset _source, mr_position _starting, mr_position _count) {
    mr_position w = _count, b = 0;
    mr_mixel* source = MR_TM(_source)+_starting*8;
    for (--w; w != 255; --w) {
        for (b = 0; b < 8; ++b, ++source) {
            *source = (*source) | (*source >> 1);
        }
    }
}

// Redefine a tile using the given data.
void mr_tile_redefine_memory_mapped(mr_tileset _tileset, mr_tile _tile, mr_mixel* _data) {
    mr_mixel* destination = (mr_mixel*)(MR_TM(_tileset) + _tile*8);
    mr_position b = 0;
    for (b = 0; b < 8; ++b, ++destination, ++_data) {
        *destination = *_data;
    }
}

// Writes a tile into a bitmap.
void _mr_puttile_monocolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile, mr_color _color) {

    int offset;

    offset = _y * MR_SCREEN_WIDTH + _x;

    MR_WRITE_TILE(_screen, _colormap, offset, _tile, _color);

}

// Writes a tile into a bitmap.
void _mr_puttile(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile, mr_color _color) {
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    if (MR_MULTICOLOR) {
        _mr_puttile_multicolor(_screen, _colormap, _x, _y, _tile, _color);
    } else {
#endif
        _mr_puttile_monocolor(_screen, _colormap, _x, _y, _tile, _color);
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    }
#endif
}

// Writes a tile into a bitmap at *precise* horizontal position.
void _mr_tile_moveto_horizontal_monocolor(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_color _color) {

    int offset;

    offset = (_y >> 3) * MR_SCREEN_WIDTH + (_x >> 3);

    if (_x >= 0 && ((_x >> 3)) < MR_SCREEN_WIDTH) {
        MR_WRITE_TILE(_screen, _colormap, offset, _tile + (_x & 0x07), _color);
    }

    if ((_x+8) >= 0  && ((_x >> 3) + 1) < MR_SCREEN_WIDTH) {
        MR_WRITE_TILE(_screen, _colormap, offset + 1, _tile + (_x & 0x07) + 8, _color);
    }
}

// Writes a tile into a bitmap at *precise* horizontal position.
void _mr_tile_moveto_horizontal(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_color _color) {
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    if (MR_MULTICOLOR) {
        _mr_tile_moveto_horizontal_multicolor(_screen, _colormap, _x, _y, _tile, _color);
    } else {
#endif
        _mr_tile_moveto_horizontal_monocolor(_screen, _colormap, _x, _y, _tile, _color);
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    }
#endif
}

// Writes a tile into a bitmap at *precise* horizontal position.
void _mr_tile_moveto_horizontal_extended_monocolor(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_position _w, mr_position _h, mr_color _color) {

    int offset;

    signed char i, j;

    offset = (_y >> 3) * MR_SCREEN_WIDTH + (_x >> 3);

    for (i = 0; i < _h; ++i) {
        for (j = 0; j < _w+1; ++j) {
            if (((_x >> 3) + j) >= 0) {
                if (((_x >> 3) + j) < MR_SCREEN_WIDTH) {
                    MR_WRITE_TILE(_screen, _colormap, offset, _tile + (_x & 0x07), _color);
                }
            }
            ++offset;
            _tile += 9;
        }
        /*if (((_x >> 3) + j + 1) < MR_SCREEN_WIDTH) {
            _screen[offset + 1] = _tile + (_x & 0x07) + 1;
            _colormap[offset + 1] = _color;
            _tile += 9;
        }*/
        offset += MR_SCREEN_WIDTH - (_w + 1);
    }

}

// Writes a tile into a bitmap at *precise* horizontal position.
void _mr_tile_moveto_horizontal_extended(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_position _w, mr_position _h, mr_color _color) {
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    if (MR_MULTICOLOR) {
        _mr_tile_moveto_horizontal_extended_multicolor(_screen, _colormap, _x, _y, _tile, _w, _h, _color);
    } else {
#endif
        _mr_tile_moveto_horizontal_extended_monocolor(_screen, _colormap, _x, _y, _tile, _w, _h, _color);
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    }
#endif
}

// Writes a tile into a bitmap at *precise* vertical position.
void _mr_tile_moveto_vertical_monocolor(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_color _color) {

    int offset;

    offset = (_y >> 3) * MR_SCREEN_WIDTH + (_x >> 3);

    // _screen[offset] = _tile + (_y & 0x07) + 1;
    // _colormap[offset] = _color;
    MR_WRITE_TILE(_screen, _colormap, offset, _tile + (_y & 0x07) + 1, _color);
    if ((offset + MR_SCREEN_WIDTH) < MR_SCREEN_RAM_SIZE) {
        MR_WRITE_TILE(_screen, _colormap, offset + MR_SCREEN_WIDTH, _tile + (_y & 0x07) + 10, _color);
    }

}

void _mr_tile_moveto_vertical(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_color _color) {
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    if (MR_MULTICOLOR) {
        _mr_tile_moveto_vertical_multicolor(_screen, _colormap, _x, _y, _tile, _color);
    } else {
#endif
        _mr_tile_moveto_vertical_monocolor(_screen, _colormap, _x, _y, _tile, _color);
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    }
#endif
}

// Writes a tile into a bitmap at *precise* vertical position.
void _mr_tile_moveto_vertical_extended_monocolor(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_position _w, mr_position _h, mr_color _color) {

    int offset;

    mr_position w = _w;
    mr_position i, j;

    offset = (_y >> 3) * MR_SCREEN_WIDTH + (_x >> 3);

    for (j = 0; j < _w; ++j) {
        for (i = 0; i <= _h; ++i) {
            if ((offset + MR_SCREEN_WIDTH) < MR_SCREEN_RAM_SIZE) {
                MR_WRITE_TILE(_screen, _colormap, offset, _tile + (_y & 0x07), _color);
            }
            offset += MR_SCREEN_WIDTH;
            _tile += 9;
        }
        offset -= (( _h + 1) * MR_SCREEN_WIDTH);
        ++offset;
    }

}

// Writes a tile into a bitmap at *precise* vertical position.
void _mr_tile_moveto_vertical_extended(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_position _w, mr_position _h, mr_color _color) {
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    if (MR_MULTICOLOR) {
        _mr_tile_moveto_vertical_extended_multicolor(_screen, _colormap, _x, _y, _tile, _w, _h, _color);
    } else {
#endif
        _mr_tile_moveto_vertical_extended_monocolor(_screen, _colormap, _x, _y, _tile, _w, _h, _color);
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    }
#endif
}

// Clear a tile of a bitmap.
void _mr_cleartile(mr_mixel* _screen, mr_position _x, mr_position _y) {

    int offset;
    offset = _y * MR_SCREEN_WIDTH + _x;
    
    // _screen[offset] = MR_RENDERED_MIXELS[0];
    MR_WRITE_TILE_LUMINANCE(_screen, offset, MR_RENDERED_MIXELS[0]);

}

// Reads a tile from a bitmap.
mr_color _mr_gettile(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y) {

    int offset;

    offset = _y * MR_SCREEN_WIDTH + _x;

    _colormap = 0;

    return MR_READ_TILE( _screen, offset );

}

// Load a tileset (or part of it)
void mr_tileset_load(unsigned char* _filename, mr_tileset _tileset, mr_tile _starting, mr_tile _count) {

#ifdef MIDRES_STANDALONE_FILE

#if defined(__ATARI__) || defined(__CBM__) 

    FILE* f = fopen(_filename, "rb");
    if (f == NULL) {
        return;
    }
    fread(MR_TM(_tileset) + 8 * _starting, _count * 8, 1, f);
    fclose(f);

#ifndef MIDRES_STANDALONE_TILE_MULTICOLOR 
    if (MR_MULTICOLOR) {
        mr_tileset_multicolor_to_monocolor(_tileset, _starting, _count);
    }
#endif

#endif

#endif

}

void mr_tileset_load_file_memory_mapped(mr_file _index, mr_tileset _tileset, mr_tile _starting, mr_tile _count) {
#ifdef MIDRES_STANDALONE_FILE
    mr_read_file(_index, MR_TM(_tileset) + 8 * _starting, _count * 8);
#endif

#ifndef MIDRES_STANDALONE_TILE_MULTICOLOR 
    if (MR_MULTICOLOR) {
        mr_tileset_multicolor_to_monocolor(_tileset, _starting, _count);
    }
#endif
    _index = 0;
    _tileset = 0;
    _starting = 0;
    _count = 0;

}

void _mr_putetiles_monocolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile_start, mr_position _w, mr_position _h, mr_color _color) {

    int offset;
    mr_position w;

    offset = _y * MR_SCREEN_WIDTH + _x;
    w = _w;

    for (; _h != 0; --_h) {
        mr_position w = _w;
        for (; w != 0; --w, ++_tile_start) {
            MR_WRITE_TILE(_screen, _colormap, offset, _tile_start, _color);
            ++offset;
        }
        offset += MR_SCREEN_WIDTH - _w;
    }

}

void _mr_putetiles(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile_start, mr_position _w, mr_position _h, mr_color _color) {
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    if (MR_MULTICOLOR) {
        _mr_putetiles_multicolor(_screen, _colormap, _x, _y, _tile_start, _w, _h, _color);
    } else {
#endif
        _mr_putetiles_monocolor(_screen, _colormap, _x, _y, _tile_start, _w, _h, _color);
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    }
#endif
}

void _mr_putftiles_monocolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile_start, mr_position _w, mr_position _h, mr_color _color) {

    int offset;
    mr_position w;

    offset = _y * MR_SCREEN_WIDTH + _x;
    w = _w;

    for (; _h != 0; --_h) {
        mr_position w = _w;
        for (; w != 0; --w) {
            MR_WRITE_TILE(_screen, _colormap, offset, _tile_start, _color);
            ++offset;
        }
        offset += MR_SCREEN_WIDTH - _w;
    }

}

void _mr_putftiles(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile_start, mr_position _w, mr_position _h, mr_color _color) {
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    if (MR_MULTICOLOR) {
        _mr_putftiles_multicolor(_screen, _colormap, _x, _y, _tile_start, _w, _h, _color);
    }
    else {
#endif
        _mr_putftiles_monocolor(_screen, _colormap, _x, _y, _tile_start, _w, _h, _color);
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    }
#endif
}

void _mr_puttiles_monocolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile_start, mr_tile _tile_count, mr_color _color) {

    int offset;

    offset = _y * MR_SCREEN_WIDTH + _x;

    for (; _tile_count != 0; --_tile_count, ++_tile_start) {
        MR_WRITE_TILE(_screen, _colormap, offset, _tile_start, _color);
        ++offset;
    }

}

void _mr_puttiles(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile_start, mr_tile _tile_count, mr_color _color) {
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    if (MR_MULTICOLOR) {
        _mr_puttiles_multicolor(_screen, _colormap, _x, _y, _tile_start, _tile_count, _color);
    } else {
#endif
        _mr_puttiles_monocolor(_screen, _colormap, _x, _y, _tile_start, _tile_count, _color);
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    }
#endif

}

// Draws a vertical line onto the bitmap.
void _mr_vtiles_monocolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y1, mr_position _y2, mr_tile _tile, mr_color _color) {

    mr_position y1 = _y1;
    int offset = _y1 * MR_SCREEN_WIDTH + _x;

    for (; y1 <= _y2; ++y1) {
        MR_WRITE_TILE(_screen, _colormap, offset, _tile, _color);
        offset += MR_SCREEN_WIDTH;
    }

}

// Draws a vertical line onto the bitmap.
void _mr_vtiles(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y1, mr_position _y2, mr_tile _tile, mr_color _color) {
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    if (MR_MULTICOLOR) {
        _mr_vtiles_multicolor(_screen, _colormap, _x, _y1, _y2, _tile, _color);
    } else {
#endif
        _mr_vtiles_monocolor(_screen, _colormap, _x, _y1, _y2, _tile, _color);
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    }
#endif
}

// Draws a horizontal line onto the bitmap.
void _mr_htiles_monocolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x1, mr_position _x2, mr_position _y, mr_tile _tile, mr_color _color) {

    mr_position x1 = _x1;
    int offset = _y * MR_SCREEN_WIDTH + x1;

    for (; x1 <= _x2; ++x1 ) {
        MR_WRITE_TILE(_screen, _colormap, offset, _tile, _color);
        ++offset;
    }

}

// Draws a horizontal line onto the bitmap.
void _mr_htiles(mr_mixel* _screen, mr_color* _colormap, mr_position _x1, mr_position _x2, mr_position _y, mr_tile _tile, mr_color _color) {
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    if (MR_MULTICOLOR) {
        _mr_htiles_multicolor(_screen, _colormap, _x1, _x2, _y, _tile, _color);
    } else {
#endif
        _mr_htiles_monocolor(_screen, _colormap, _x1, _x2, _y, _tile, _color);
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    }
#endif
}

// Set a single color for multicolor tiles
void mr_tile_setcolor(mr_position _index, mr_color _color) {
    mr_tile_setcolor_hd(_index, _color);
}

// Set a set of colors for multicolor tiles
void mr_tile_setcolors(mr_color _colors[4]) {
    mr_tile_setcolors_hd(_colors);
}

void mr_tile_redefine_fill_memory_mapped(mr_tileset _tileset, mr_tile _tile, mr_mixel _data) {
    mr_mixel* destination = (mr_mixel*)(MR_TM(_tileset) + _tile * 8);
    unsigned char b = 0;
    for (b = 0; b < 8; ++destination, ++b) {
        *destination = _data;
    }
}

#endif
