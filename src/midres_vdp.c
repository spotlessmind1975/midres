/*****************************************************************************
 * MIDRES Library - an isomorphic gamelib for retrocomputers                 *
 *****************************************************************************
 * Copyright 2020-2021 Marco Spedaletti (asimov@mclink.it)
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

#include "midres.h"

#if defined(__VDP__)

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include "rawdata.h"

  /****************************************************************************
   ** RESIDENT VARIABLES SECTION
   ****************************************************************************/

void mr_init_vdp_hd() {

#ifdef FRAME_BUFFER
    memset(&frameBuffer[0], 0, MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT);
    memset(&colorBuffer[0], 0, MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT);
#endif

#ifdef GRAPHIC_MODE_I
    mr_vdp_out(VDP_R0, 0x00);
#if __LM80C__ 
    // Avoid enabling IRQ (under LM80C is connected to Z80 NMI)
    mr_vdp_out(VDP_R1, 0xc0);
#else
    mr_vdp_out(VDP_R1, 0xe0);
#endif
    mr_vdp_out(VDP_RNAME, MR_SCREEN_DEFAULT);
    mr_vdp_out(VDP_RCOLORTABLE, 0x80);
    mr_vdp_out(VDP_RPATTERN, MR_TILESET_DEFAULT);
#else
    mr_vdp_out(VDP_R0, 0x02);
    mr_vdp_out(VDP_R1, 0xc0);
    mr_vdp_out(VDP_RNAME, 0x0e);
    mr_vdp_out(VDP_RCOLORTABLE, 0xff);
    mr_vdp_out(VDP_RPATTERN, 0x03);

    mr_vdp_put(&_includedFiles[0][0], 0x0000, 128);
    mr_vdp_put(&_includedFiles[0][0], 0x0800, 128);
    mr_vdp_put(&_includedFiles[0][0], 0x1000, 128);
#endif
    mr_vdp_out(VDP_RSPRITEA, 0x76);
    mr_vdp_out(VDP_RSPRITEP, 0x03);
    mr_vdp_out(VDP_RCOLOR, 0x0f);

    mr_vdp_put(&_includedFiles[0][0], 0x0800, 128);

}

void mr_init_multicolor_vdp_hd() {

#ifdef FRAME_BUFFER
    memset(&frameBuffer[0], 0, MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT);
    memset(&colorBuffer[0], 0, MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT);
#endif

#ifdef GRAPHIC_MODE_I
    mr_vdp_out(VDP_R0, 0x00);
#if __LM80C__ 
    // Avoid enabling IRQ (under LM80C is connected to Z80 NMI)
    mr_vdp_out(VDP_R1, 0xc0);
#else
    mr_vdp_out(VDP_R1, 0xe0);
#endif
    mr_vdp_out(VDP_RNAME, MR_SCREEN_DEFAULT);
    mr_vdp_out(VDP_RCOLORTABLE, 0x80);
    mr_vdp_out(VDP_RPATTERN, MR_TILESET_DEFAULT);
#else
    mr_vdp_out(VDP_R0, 0x02);
    mr_vdp_out(VDP_R1, 0xc0);
    mr_vdp_out(VDP_RNAME, 0x0e);
    mr_vdp_out(VDP_RCOLORTABLE, 0xff);
    mr_vdp_out(VDP_RPATTERN, 0x03);
#endif
    mr_vdp_out(VDP_RSPRITEA, 0x76);
    mr_vdp_out(VDP_RSPRITEP, 0x03);
    mr_vdp_out(VDP_RCOLOR, 0x0f);

#ifdef GRAPHIC_MODE_I
    mr_vdp_put(&_includedFiles[0][0], 0x0800, 128);
#else
    mr_vdp_put(&_includedFiles[0][0], 0x0000, 128);
    mr_vdp_put(&_includedFiles[0][0], 0x0800, 128);
    mr_vdp_put(&_includedFiles[0][0], 0x1000, 128);
#endif

}

void mr_tileset_copy_hd(unsigned char _source, unsigned char _dest) {
    unsigned char i;
    for (i = 0; i < (MR_TILESET_TILE_COUNT - 1); ++i) {
        mr_position w = 255, b = 0;
        unsigned int source = MR_TM(_source);
        unsigned int destination = MR_TM(_dest);
        for (--w; w != 255; --w) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_vdp_fill8(mr_vdp_get(source), destination, 1);
#ifdef GRAPHIC_MODE_II
                mr_vdp_fill8(mr_vdp_get(source), destination + 0x800, 1);
                mr_vdp_fill8(mr_vdp_get(source), destination + 0x1000, 1);
#endif
            }
        }
    }
}

void mr_tileset_multicolor_to_monocolor_hd(unsigned char _source, unsigned char _starting, unsigned char _count) {
    mr_position w = _count, b = 0;
    unsigned int source = MR_TM(_source) + _starting * 8;
    for (--w; w != 255; --w) {
        for (b = 0; b < 8; ++b, ++source) {
            unsigned char s = mr_vdp_get(source);
            s = s | (s >> 1);
            mr_vdp_fill8(s, source, 1);
#ifdef GRAPHIC_MODE_II
            mr_vdp_fill8(s, source + 0x800, 1);
            mr_vdp_fill8(s, source + 0x1000, 1);
#endif
        }
    }
}

void mr_tile_redefine_hd(unsigned char _tileset, unsigned char _tile, unsigned char* _data) {
    unsigned int destination = (MR_TM(_tileset) + _tile * 8);
    mr_position b = 0;
    for (b = 0; b < 8; ++b, ++destination, ++_data) {
        mr_vdp_fill8(*_data, destination, 1);
#ifdef GRAPHIC_MODE_II
        mr_vdp_fill8(*_data, destination + 0x800, 1);
        mr_vdp_fill8(*_data, destination + 0x1000, 1);
#endif
    }
}

void mr_tile_redefine_fill_hd(unsigned char _tileset, unsigned char _tile, unsigned char _data) {
    unsigned int destination = (MR_TM(_tileset) + _tile * 8);
    unsigned char b = 0;
    for (b = 0; b < 8; ++destination, ++b) {
        mr_vdp_fill8(_data, destination, 1);
#ifdef GRAPHIC_MODE_II
        mr_vdp_fill8(_data, destination + 0x800, 1);
        mr_vdp_fill8(_data, destination + 0x1000, 1);
#endif
    }
}

void mr_tile_prepare_horizontal_monocolor_hd(unsigned char _tileset, unsigned char _source, unsigned char _destination) {

    unsigned int source = (MR_TM(_tileset) + _source * 8);
    unsigned int destination = (MR_TM(_tileset) + _destination * 8);

    mr_position i, b;

    for (i = 0; i < 9; ++i) {
        for (b = 0; b < 8; ++b, ++source, ++destination) {
            mr_mixel d = mr_vdp_get(source);
            mr_mixel m = d >> i;
            mr_vdp_fill8(m, destination, 1);
#ifdef GRAPHIC_MODE_II
            mr_vdp_fill8(m, destination + 0x800, 1);
            mr_vdp_fill8(m, destination + 0x1000, 1);
#endif
        }
        source -= 8;
    }

    for (i = 0; i < 8; ++i) {
        for (b = 0; b < 8; ++b, ++source, ++destination) {
            mr_mixel d = mr_vdp_get(source);
            mr_mixel n = d & (0xff >> (7 - i));
            mr_vdp_fill8((n << (7 - i)), destination, 1);
#ifdef GRAPHIC_MODE_II
            mr_vdp_fill8((n << (7 - i)), destination + 0x800, 1);
            mr_vdp_fill8((n << (7 - i)), destination + 0x1000, 1);
#endif
        }
        source -= 8;
    }

}

void mr_tile_prepare_horizontal_extended_monocolor_hd(unsigned char _tileset, unsigned char _source, unsigned char _w, unsigned char _h, unsigned char _destination) {
    unsigned int source = (MR_TM(_tileset) + _source * 8);
    unsigned int destination = (MR_TM(_tileset) + _destination * 8);

    mr_position i, b;

    mr_position j, k;

    for (j = 0; j < _h; ++j) {
        for (i = 0; i < 9; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel e = mr_vdp_get(source);
                mr_mixel m = e >> i;
                mr_vdp_fill8(m, destination, 1);
#ifdef GRAPHIC_MODE_II
                mr_vdp_fill8(m, destination + 0x800, 1);
                mr_vdp_fill8(m, destination + 0x1000, 1);
#endif
            }
            source -= 8;
        }

        for (k = 0; k < (_w - 1); ++k) {
            for (i = 0; i < 9; ++i) {
                for (b = 0; b < 8; ++b, ++source, ++destination) {
                    mr_mixel d = mr_vdp_get(source);
                    mr_mixel e = mr_vdp_get(source + 8);
                    mr_mixel m = (e >> i) | (d << (8 - i));
                    mr_vdp_fill8(m, destination, 1);
#ifdef GRAPHIC_MODE_II
                    mr_vdp_fill8(m, destination + 0x800, 1);
                    mr_vdp_fill8(m, destination + 0x1000, 1);
#endif
                }
                source -= 8;
            }
            source += 8;
        }

        for (i = 0; i < 9; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel d = mr_vdp_get(source);
                mr_mixel n = d & (0xff >> (8 - i));
                mr_vdp_fill8((n << (8 - i)), destination, 1);
#ifdef GRAPHIC_MODE_II
                mr_vdp_fill8((n << (8 - i)), destination + 0x800, 1);
                mr_vdp_fill8((n << (8 - i)), destination + 0x1000, 1);
#endif
            }
            source -= 8;
        }
        source += 8;
    }
}

void mr_tile_prepare_vertical_hd(unsigned char _tileset, unsigned char _source, unsigned char _destination) {
    unsigned int source = (MR_TM(_tileset) + _source * 8);
    unsigned int destination = (MR_TM(_tileset) + _destination * 8);

    mr_position i, b;

    for (i = 0; i < 9; ++i) {
        for (b = 0; b < i; ++b, ++destination) {
            mr_vdp_fill8(0x00, destination, 1);
#ifdef GRAPHIC_MODE_II
            mr_vdp_fill8(0x00, destination + 0x800, 1);
            mr_vdp_fill8(0x00, destination + 0x1000, 1);
#endif
        }
        for (b = 0; b < (8 - i); ++b, ++source, ++destination) {
            mr_vdp_fill8(mr_vdp_get(source), destination, 1);
#ifdef GRAPHIC_MODE_II
            mr_vdp_fill8(mr_vdp_get(source), destination + 0x800, 1);
            mr_vdp_fill8(mr_vdp_get(source), destination + 0x1000, 1);
#endif
        }
        source -= b;
    }

    for (i = 8; i > -1; --i) {
        for (b = 0; b < (8 - i); ++b, ++source, ++destination) {
            mr_vdp_fill8(mr_vdp_get(source + i), destination, 1);
#ifdef GRAPHIC_MODE_II
            mr_vdp_fill8(mr_vdp_get(source + i), destination + 0x800, 1);
            mr_vdp_fill8(mr_vdp_get(source + i), destination + 0x1000, 1);
#endif
        }
        source -= b;
        for (; b < 8; ++b, ++destination) {
            mr_vdp_fill8(0x00, destination, 1);
#ifdef GRAPHIC_MODE_II
            mr_vdp_fill8(0x00, destination + 0x800, 1);
            mr_vdp_fill8(0x00, destination + 0x1000, 1);
#endif
        }
    }
}

void mr_tile_prepare_vertical_extended_hd(unsigned char _tileset, unsigned char _source, unsigned char _w, unsigned char _h, unsigned char _destination) {
    unsigned int source = (MR_TM(_tileset) + _source * 8);
    unsigned int destination = (MR_TM(_tileset) + _destination * 8);

    int i, b;
    int w = _w, k = 0;

    for (; w != 0; --w) {
        for (i = 0; i < 9; ++i) {
            for (b = 0; b < i; ++b, ++destination) {
                mr_vdp_fill8(0x00, destination, 1);
#ifdef GRAPHIC_MODE_II
                mr_vdp_fill8(0x00, destination + 0x800, 1);
                mr_vdp_fill8(0x00, destination + 0x1000, 1);
#endif
            }
            for (b = 0; b < (8 - i); ++b, ++source, ++destination) {
                mr_vdp_fill8(mr_vdp_get(source), destination, 1);
#ifdef GRAPHIC_MODE_II
                mr_vdp_fill8(mr_vdp_get(source), destination + 0x800, 1);
                mr_vdp_fill8(mr_vdp_get(source), destination + 0x1000, 1);
#endif
            }
            source -= b;
        }

        for (k = 0; k < (_h - 1); ++k) {
            for (i = 0; i < 9; ++i) {
                for (b = 0; b < i; ++b, ++destination, ++source) {
                    mr_vdp_fill8(mr_vdp_get(source + (k * _w * 8) + (8 - i)), destination, 1);
#ifdef GRAPHIC_MODE_II
                    mr_vdp_fill8(mr_vdp_get(source + (k * _w * 8) + (8 - i)), destination + 0x800, 1);
                    mr_vdp_fill8(mr_vdp_get(source + (k * _w * 8) + (8 - i)), destination + 0x1000, 1);
#endif
                }
                source -= b;
                for (b = 0; b < (8 - i); ++b, ++source, ++destination) {
                    mr_vdp_fill8(mr_vdp_get(source + ((k + 1) * _w * 8)), destination, 1);
#ifdef GRAPHIC_MODE_II
                    mr_vdp_fill8(mr_vdp_get(source + ((k + 1) * _w * 8)), destination + 0x800, 1);
                    mr_vdp_fill8(mr_vdp_get(source + ((k + 1) * _w * 8)), destination + 0x1000, 1);
#endif
                }
                source -= b;
            }
        }

        for (i = 0; i < 9; ++i) {
            for (b = 0; b < i; ++b, ++destination, ++source) {
                mr_vdp_fill8(mr_vdp_get(source + ((_h - 1) * _w * 8) + (8 - i)), destination, 1);
#ifdef GRAPHIC_MODE_II
                mr_vdp_fill8(mr_vdp_get(source + ((_h - 1) * _w * 8) + (8 - i)), destination + 0x800, 1);
                mr_vdp_fill8(mr_vdp_get(source + ((_h - 1) * _w * 8) + (8 - i)), destination + 0x1000, 1);
#endif
            }
            source -= b;
            for (b = 0; b < (8 - i); ++b, ++destination) {
                mr_vdp_fill8(0x00, destination, 1);
#ifdef GRAPHIC_MODE_II
                mr_vdp_fill8(0x00, destination + 0x800, 1);
                mr_vdp_fill8(0x00, destination + 0x1000, 1);
#endif
            }
        }
        source += 8;
    }
}

void mr_tile_prepare_roll_horizontal_monocolor_hd(unsigned char _tileset, unsigned char _source, unsigned char _destination) {
    unsigned int source = (MR_TM(_tileset) + _source * 8);
    unsigned int destination = (MR_TM(_tileset) + _destination * 8);

    mr_position i, b;

    for (i = 0; i < 8; ++i) {
        for (b = 0; b < 8; ++b, ++source, ++destination) {
            mr_mixel d = mr_vdp_get(source);
            mr_mixel m = d >> i, n = d & (0xff >> (8 - i));
            mr_vdp_fill8(m | (n << (8 - i)), destination, 1);
#ifdef GRAPHIC_MODE_II
            mr_vdp_fill8(m | (n << (8 - i)), destination + 0x800, 1);
            mr_vdp_fill8(m | (n << (8 - i)), destination + 0x1000, 1);
#endif
        }
        source -= 8;
    }
}

extern mr_mixel rollBuffer[8];

void mr_tile_roll_horizontal_hd(unsigned char _tileset, unsigned char _destination, unsigned char _direction) {
    unsigned int source;
    unsigned int destination;
    unsigned char* temp = &rollBuffer[0];
    mr_position i, b;

    if (_direction == mr_direction_right) {
        source = (MR_TM(_tileset) + (_destination + 1) * 8);
        destination = (MR_TM(_tileset) + _destination * 8);

        for (b = 0; b < 8; ++b, ++source, ++temp) {
            mr_mixel d = mr_vdp_get(source - 8);
            *temp = d;
        }
        temp -= 8;
        source -= 8;
        for (i = 0; i < 6; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel d = mr_vdp_get(source);
                mr_vdp_fill8(d, destination, 1);
#ifdef GRAPHIC_MODE_II
                mr_vdp_fill8(d, destination + 0x800, 1);
                mr_vdp_fill8(d, destination + 0x1000, 1);
#endif
            }
        }
        for (b = 0; b < 8; ++b, ++destination, ++temp) {
            mr_vdp_fill8(*temp, destination, 1);
#ifdef GRAPHIC_MODE_II
            mr_vdp_fill8(*temp, destination + 0x800, 1);
            mr_vdp_fill8(*temp, destination + 0x1000, 1);
#endif
        }
    }
    else {
        source = (MR_TM(_tileset) + (_destination + 6) * 8);
        destination = (MR_TM(_tileset) + (_destination + 7) * 8);
        for (b = 0; b < 8; ++b, ++destination, ++temp) {
            mr_mixel d = mr_vdp_get(destination);
            *temp = d;
        }
        temp -= 8;
        destination -= 8;
        for (i = 0; i < 7; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel d = mr_vdp_get(source);
                mr_vdp_fill8(d, destination, 1);
#ifdef GRAPHIC_MODE_II
                mr_vdp_fill8(d, destination + 0x800, 1);
                mr_vdp_fill8(d, destination + 0x1000, 1);
#endif
            }
            source -= 16;
            destination -= 16;
        }
        for (b = 0; b < 8; ++b, ++destination, ++temp) {
            mr_vdp_fill8(*temp, destination, 1);
#ifdef GRAPHIC_MODE_II
            mr_vdp_fill8(*temp, destination + 0x800, 1);
            mr_vdp_fill8(*temp, destination + 0x1000, 1);
#endif
        }
    }
}

void mr_tile_roll_horizontal_on_place_hd(unsigned char _tileset, unsigned char _destination, unsigned char _direction, unsigned char _place, unsigned char* _index) {
    unsigned int source;
    unsigned int destination;
    mr_position b;

    source = (MR_TM(_tileset) + (_destination * 8) + (*_index) * 8);
    destination = (MR_TM(_tileset) + _place * 8);
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
        mr_mixel d = mr_vdp_get(source);
        mr_vdp_fill8(d, destination, 1);
#ifdef GRAPHIC_MODE_II
        mr_vdp_fill8(d, destination + 0x800, 1);
        mr_vdp_fill8(d, destination + 0x1000, 1);
#endif
    }
}

void mr_tile_prepare_roll_vertical_hd(unsigned char _tileset, unsigned char _source, unsigned char _destination) {
    unsigned int source = (MR_TM(_tileset) + _source * 8);
    unsigned int destination = (MR_TM(_tileset) + _destination * 8);

    mr_position i, b;

    for (i = 0; i < 8; ++i) {
        for (b = 0; b < (8 - i); ++b, ++source, ++destination) {
            mr_vdp_fill8(mr_vdp_get(source + i), destination, 1);
#ifdef GRAPHIC_MODE_II
            mr_vdp_fill8(mr_vdp_get(source + i), destination + 0x800, 1);
            mr_vdp_fill8(mr_vdp_get(source + i), destination + 0x1000, 1);
#endif
        }
        source -= b;
        for (b = 0; b < i; ++b, ++source, ++destination) {
            mr_vdp_fill8(mr_vdp_get(source), destination, 1);
#ifdef GRAPHIC_MODE_II
            mr_vdp_fill8(mr_vdp_get(source), destination + 0x800, 1);
            mr_vdp_fill8(mr_vdp_get(source), destination + 0x1000, 1);
#endif
        }
        source -= b;
    }
}

void mr_tile_roll_vertical_hd(unsigned char _tileset, unsigned char _destination, unsigned char _direction) {
    unsigned int source;
    unsigned int destination;
    mr_mixel* temp = &rollBuffer[0];
    mr_position i, b;

    if (_direction == mr_direction_up) {
        source = (MR_TM(_tileset) + (_destination + 1) * 8);
        destination = (MR_TM(_tileset) + _destination * 8);

        for (b = 0; b < 8; ++b, ++source, ++temp) {
            mr_mixel d = mr_vdp_get(source - 8);
            *temp = d;
        }
        temp -= 8;
        source -= 8;
        for (i = 0; i < 6; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel d = mr_vdp_get(source);
                mr_vdp_fill8(d, destination, 1);
#ifdef GRAPHIC_MODE_II
                mr_vdp_fill8(d, destination + 0x800, 1);
                mr_vdp_fill8(d, destination + 0x1000, 1);
#endif
            }
        }
        for (b = 0; b < 8; ++b, ++destination, ++temp) {
            mr_vdp_fill8(*temp, destination, 1);
#ifdef GRAPHIC_MODE_II
            mr_vdp_fill8(*temp, destination + 0x800, 1);
            mr_vdp_fill8(*temp, destination + 0x1000, 1);
#endif
        }
    }
    else {
        source = (MR_TM(_tileset) + (_destination + 6) * 8);
        destination = (MR_TM(_tileset) + (_destination + 7) * 8);
        for (b = 0; b < 8; ++b, ++destination, ++temp) {
            mr_mixel d = mr_vdp_get(destination);
            *temp = d;
        }
        temp -= 8;
        destination -= 8;
        for (i = 0; i < 7; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel d = mr_vdp_get(source);
                mr_vdp_fill8(d, destination, 1);
#ifdef GRAPHIC_MODE_II
                mr_vdp_fill8(d, destination + 0x800, 1);
                mr_vdp_fill8(d, destination + 0x1000, 1);
#endif
            }
            source -= 16;
            destination -= 16;
        }
        for (b = 0; b < 8; ++b, ++destination, ++temp) {
            mr_vdp_fill8(*temp, destination, 1);
#ifdef GRAPHIC_MODE_II
            mr_vdp_fill8(*temp, destination + 0x800, 1);
            mr_vdp_fill8(*temp, destination + 0x1000, 1);
#endif
        }
    }
}

void mr_tileset_load_file_hd(unsigned int _index, unsigned char _tileset, unsigned char _starting, unsigned char _count) {
    mr_vdp_put(&_includedFiles[_index][0], MR_TM(_tileset) + (_starting * 8), _count * 8);
#ifdef GRAPHIC_MODE_II
    mr_vdp_put(&_includedFiles[_index][0], 0x800 + MR_TM(_tileset) + (_starting * 8), _count * 8);
    mr_vdp_put(&_includedFiles[_index][0], 0x1000 + MR_TM(_tileset) + (_starting * 8), _count * 8);
#endif
}

void mr_set_background_color_hd(unsigned char _color) {
#ifdef GRAPHIC_MODE_II
#else
    unsigned char i;
    for (i = 0; i < 32; ++i) {
        unsigned char v = mr_vdp_get(0x2000 + i) & 0xf0;
        mr_vdp_fill8(( v ) | ( _color & 0x0f ), 0x2000 + i, 1);
    }
#endif
}

void mr_set_border_color_hd(unsigned char _color) {
    mr_vdp_out(VDP_RCOLOR, _color & 0x0f);
}

#endif