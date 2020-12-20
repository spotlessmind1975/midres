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
 * (la "Licenza"); è proibito usare questo file se non in conformità alla
 * Licenza. Una copia della Licenza è disponibile all'indirizzo:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Se non richiesto dalla legislazione vigente o concordato per iscritto,
 * il software distribuito nei termini della Licenza è distribuito
 * "COSÌ COM'È", SENZA GARANZIE O CONDIZIONI DI ALCUN TIPO, esplicite o
 * implicite. Consultare la Licenza per il testo specifico che regola le
 * autorizzazioni e le limitazioni previste dalla medesima.
 ****************************************************************************/

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>

#include "midres.h"

#if defined(MIDRES_STANDALONE_TILE_MULTICOLOR)

void mr_tile_prepare_horizontal_multicolor(mr_tileset _tileset, mr_tile _source, mr_tile _destination) {
    mr_mixel* source = (mr_mixel*)(MR_TM(_tileset) + _source * 8);
    mr_mixel* destination = (mr_mixel*)(MR_TM(_tileset) + _destination * 8);

    mr_position i, b;

    for (i = 0; i < 9; i += 2) {
        for (b = 0; b < 8; ++b, ++source, ++destination) {
            mr_mixel d = *((mr_mixel*)source);
            mr_mixel m = d >> i;
            *destination = m;
        }
        source -= 8;
    }

    for (i = 0; i < 8; i += 2) {
        for (b = 0; b < 8; ++b, ++source, ++destination) {
            mr_mixel d = *((mr_mixel*)source);
            mr_mixel n = d & (0xff >> (7 - i));
            *destination = (n << (7 - i));
        }
        source -= 8;
    }

}

void mr_tile_prepare_horizontal_extended_multicolor(mr_tileset _tileset, mr_tile _source, mr_tile _w, mr_tile _h, mr_tile _destination) {
    mr_mixel* source = (mr_mixel*)(MR_TM(_tileset) + _source * 8);
    mr_mixel* destination = (mr_mixel*)(MR_TM(_tileset) + _destination * 8);

    mr_position i, b;

    mr_position j, k;

    for (j = 0; j < _h; ++j) {
        for (i = 0; i < 9; i += 2) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel e = *((mr_mixel*)source);
                mr_mixel m = e >> i;
                *destination = m;
            }
            source -= 8;
        }

        for (k = 0; k < (_w - 1); ++k) {
            for (i = 0; i < 9; i += 2) {
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

        for (i = 0; i < 9; i += 2) {
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

// Redefine a subset of N tiles by "rolling" horizontally a tile
void mr_tile_prepare_roll_horizontal_multicolor(mr_tileset _tileset, mr_tile _source, mr_tile _destination) {
    mr_tile* source = (mr_tile*)(MR_TM(_tileset) + _source * 8);
    mr_tile* destination = (mr_tile*)(MR_TM(_tileset) + _destination * 8);

    mr_position i, b;

    for (i = 0; i < 8; i += 2) {
        for (b = 0; b < 8; ++b, ++source, ++destination) {
            mr_mixel d = *((mr_tile*)source);
            mr_mixel m = d >> i, n = d & (0xff >> (8 - i));
            *destination = m | (n << (8 - i));
        }
        source -= 8;
    }

}

// Writes a tile into a bitmap.
void _mr_puttile_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile, mr_color _color) {

    int offset;

    offset = _y * MR_SCREEN_WIDTH + _x;

    _screen[offset] = _tile;
    _colormap[offset] = ( 0x08 | ( _color & 0x07 ) );

}

// Writes a tile into a bitmap at *precise* horizontal position.
void _mr_tile_moveto_horizontal_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_color _color) {

    int offset;

    offset = (_y >> 3) * MR_SCREEN_WIDTH + (_x >> 2);

    if (_x >= 0 && ((_x >> 2)) < MR_SCREEN_WIDTH) {
        _screen[offset] = _tile + (_x & 0x03);
        _colormap[offset] = ( 0x08 | ( _color & 0x07 ) );
    }

    if ((_x + 8) >= 0 && ((_x >> 2) + 1) < MR_SCREEN_WIDTH) {
        _screen[offset + 1] = _tile + (_x & 0x03) + 8;
        _colormap[offset + 1] = ( 0x08 | ( _color & 0x07 ) );
    }
}

// Writes a tile into a bitmap at *precise* horizontal position.
void _mr_tile_moveto_horizontal_extended_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_position _w, mr_position _h, mr_color _color) {

    int offset;

    signed char i, j;

    offset = (_y >> 3) * MR_SCREEN_WIDTH + (_x >> 2);

    for (i = 0; i < _h; ++i) {
        for (j = 0; j < _w + 1; ++j) {
            if (((_x >> 2) + j) >= 0 && (((_x >> 2) + j) < MR_SCREEN_WIDTH)) {
                _screen[offset] = _tile + (_x & 0x03);
                _colormap[offset] = ( 0x08 | ( _color & 0x07 ) );
            }
            ++offset;
            _tile += 5;
        }
        offset += MR_SCREEN_WIDTH - (_w + 1);
    }

}

// Writes a tile into a bitmap at *precise* vertical position.
void _mr_tile_moveto_vertical_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_color _color) {

    int offset;

    offset = (_y >> 3) * MR_SCREEN_WIDTH + (_x >> 3);

    _screen[offset] = _tile + (_y & 0x07) + 1;
    _colormap[offset] = ( 0x08 | ( _color & 0x07 ) );
    if ((offset + MR_SCREEN_WIDTH) < MR_SCREEN_RAM_SIZE) {
        _colormap[offset + MR_SCREEN_WIDTH] = ( 0x08 | ( _color & 0x07 ) );
        _screen[offset + MR_SCREEN_WIDTH] = _tile + (_y & 0x07) + 10;
    }

}

// Writes a tile into a bitmap at *precise* vertical position.
void _mr_tile_moveto_vertical_extended_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_position _w, mr_position _h, mr_color _color) {

    int offset;

    mr_position w = _w;
    mr_position i, j;

    offset = (_y >> 3) * MR_SCREEN_WIDTH + (_x >> 3);

    for (j = 0; j < _w; ++j) {
        for (i = 0; i <= _h; ++i) {
            if ((offset + MR_SCREEN_WIDTH) < MR_SCREEN_RAM_SIZE) {
                _screen[offset] = _tile + (_y & 0x07);
                _colormap[offset] = ( 0x08 | ( _color & 0x07 ) );
            }
            offset += MR_SCREEN_WIDTH;
            _tile += 9;
        }
        offset -= ((_h + 1) * MR_SCREEN_WIDTH);
        ++offset;
    }

}

void _mr_putetiles_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile_start, mr_position _w, mr_position _h, mr_color _color) {

    int offset;
    mr_position w;

    offset = _y * MR_SCREEN_WIDTH + _x;
    w = _w;

    for (; _h != 0; --_h) {
        mr_position w = _w;
        for (; w != 0; --w, ++_tile_start) {
            _screen[offset] = _tile_start;
            _colormap[offset] = ( 0x08 | ( _color & 0x07 ) );
            ++offset;
        }
        offset += MR_SCREEN_WIDTH - _w;
    }

}

void _mr_putftiles_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile_start, mr_position _w, mr_position _h, mr_color _color) {

    int offset;
    mr_position w;

    offset = _y * MR_SCREEN_WIDTH + _x;
    w = _w;

    for (; _h != 0; --_h) {
        mr_position w = _w;
        for (; w != 0; --w) {
            _screen[offset] = _tile_start;
            _colormap[offset] = (0x08 | (_color & 0x07));
            ++offset;
        }
        offset += MR_SCREEN_WIDTH - _w;
    }

}

void _mr_puttiles_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile_start, mr_tile _tile_count, mr_color _color) {

    int offset;

    offset = _y * MR_SCREEN_WIDTH + _x;

    for (; _tile_count != 0; --_tile_count, ++_tile_start) {
        _screen[offset] = _tile_start;
        _colormap[offset] = ( 0x08 | ( _color & 0x07 ) );
        ++offset;
    }

}

// Draws a vertical line onto the bitmap.
void _mr_vtiles_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y1, mr_position _y2, mr_tile _tile, mr_color _color) {

    mr_position y1 = _y1;
    int offset = _y1 * MR_SCREEN_WIDTH + _x;

    for (; y1 <= _y2; ++y1) {
        _screen[offset] = _tile;
        _colormap[offset] = ( 0x08 | ( _color & 0x07 ) );
        offset += MR_SCREEN_WIDTH;
    }

}

// Draws a horizontal line onto the bitmap.
void _mr_htiles_multicolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x1, mr_position _x2, mr_position _y, mr_tile _tile, mr_color _color) {

    mr_position x1 = _x1;
    int offset = _y * MR_SCREEN_WIDTH + x1;

    for (; x1 <= _x2; ++x1) {
        _screen[offset] = _tile;
        _colormap[offset] = ( 0x08 | ( _color & 0x07 ) );
        ++offset;
    }

}

#endif
