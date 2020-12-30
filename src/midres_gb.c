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

#ifdef __GAMEBOY__

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>
#include <string.h>

#include "midres.h"

#include "rawdata.h"

void MR_DI() {
#asm
di
#endasm
}

void MR_EI() {
#asm
ei
#endasm
}

void mr_init_hd() {

    /*
    7	LCD Display Enable	0=Off, 1=On
    6	Window Tile Map Display Select	0=9800-9BFF, 1=9C00-9FFF
    5	Window Display Enable	0=Off, 1=On
    4	BG & Window Tile Data Select	0=8800-97FF, 1=8000-8FFF
    3	BG Tile Map Display Select	0=9800-9BFF, 1=9C00-9FFF
    2	OBJ (Sprite) Size	0=Off, 1=On
    1	OBJ (Sprite) Display Enable	0=Off, 1=On
    0	BG/Window Display/Priority	0=Off, 1=On
    */
    // 10010001
    * ((unsigned char*)(0xff40)) = 0x91;

}

void mr_init_multicolor_hd() {
    /*
    7	LCD Display Enable	0=Off, 1=On
    6	Window Tile Map Display Select	0=9800-9BFF, 1=9C00-9FFF
    5	Window Display Enable	0=Off, 1=On
    4	BG & Window Tile Data Select	0=8800-97FF, 1=8000-8FFF
    3	BG Tile Map Display Select	0=9800-9BFF, 1=9C00-9FFF
    2	OBJ (Sprite) Size	0=Off, 1=On
    1	OBJ (Sprite) Display Enable	0=Off, 1=On
    0	BG/Window Display/Priority	0=Off, 1=On
    */
    // 10010001
    * ((unsigned char*)(0xff40)) = 0x91;

}

void mr_tile_setcolor_hd(unsigned char _index, unsigned char _color) {

}

void mr_tile_setcolors_hd(unsigned char _colors[4]) {

}

void mr_show_hd(unsigned char _screen) {
    /*
7	LCD Display Enable	0=Off, 1=On
6	Window Tile Map Display Select	0=9800-9BFF, 1=9C00-9FFF
5	Window Display Enable	0=Off, 1=On
4	BG & Window Tile Data Select	0=8800-97FF, 1=8000-8FFF
3	BG Tile Map Display Select	0=9800-9BFF, 1=9C00-9FFF
2	OBJ (Sprite) Size	0=Off, 1=On
1	OBJ (Sprite) Display Enable	0=Off, 1=On
0	BG/Window Display/Priority	0=Off, 1=On
*/
// 10010001
    * ((unsigned char*)(0xff40)) = ( (*((unsigned char*)(0xff40))) & ~0x48 ) | ( _screen * 0x48);

}

void mr_cleanup_hd() {

}

void mr_wait_vbl() {

}

void mr_doublebuffer_switch_hd(unsigned char _screen) {

}

void mr_tileset_visible_hd(unsigned char _tileset) {
    /*
7	LCD Display Enable	0=Off, 1=On
6	Window Tile Map Display Select	0=9800-9BFF, 1=9C00-9FFF
5	Window Display Enable	0=Off, 1=On
4	BG & Window Tile Data Select	0=8800-97FF, 1=8000-8FFF
3	BG Tile Map Display Select	0=9800-9BFF, 1=9C00-9FFF
2	OBJ (Sprite) Size	0=Off, 1=On
1	OBJ (Sprite) Display Enable	0=Off, 1=On
0	BG/Window Display/Priority	0=Off, 1=On
*/
// 10010001
    * ((unsigned char*)(0xff40)) = ((*((unsigned char*)(0xff40))) & ~0x10) | ((1-_tileset) * 0x10);

}

unsigned char mr_key_pressed_hd() {
    *((unsigned char*)0xff00) = *(((unsigned char*)0xff00)) & ~0x10;
    unsigned char a = *((unsigned char*)0xff00);
    a = *((unsigned char*)0xff00);
    a = *((unsigned char*)0xff00);
    a = *((unsigned char*)0xff00);
    a = ~a;
    a = a & 0x0f;
    *((unsigned char*)0xff00) = *(((unsigned char*)0xff00)) & ~0x08;
    unsigned char b = *((unsigned char*)0xff00);
    b = *((unsigned char*)0xff00);
    b = *((unsigned char*)0xff00);
    b = *((unsigned char*)0xff00);
    b = ~b;
    b = b & 0x0f;
    unsigned char port = (a << 4) | b;
    return port != 0;
}
    
unsigned char mr_get_key_pressed_hd() {
    return MR_KEY_NONE;
}

void mr_wait_hd(unsigned char _seconds) {
    sleep(_seconds);
}

void mr_wait_jiffies_hd(unsigned char _jiffies) {
    unsigned char actualJiffies = *((unsigned char*)0xff04);
    unsigned char c;
    while ((*((unsigned char*)0xff04) - actualJiffies) < _jiffies) {
        c = *((unsigned char*)0xff04);
    }
}

// Hardware dependent sound library
void mr_sound_start_hd(unsigned char _channel, unsigned char _number) {

}

// Hardware dependent sound library
void mr_sound_change_hd(unsigned char _channel, int _parameter) {

}

// Hardware dependent sound library
void mr_sound_stop_hd(unsigned char _channel) {

}

void mr_set_background_color_hd(unsigned char _color) {

}

void mr_set_border_color_hd(unsigned char _color) {

}

void mr_start_frame_hd() {
    *((unsigned char*)0xff04) = 0;
}

void mr_end_frame_hd(unsigned char _jiffies) {
    while ((*((unsigned char*)0xff04)) < _jiffies ) {
        ;
    }
}

unsigned char mr_joy_hd(unsigned char _number) {
    *((unsigned char*)0xff00) = *(((unsigned char*)0xff00)) & ~0x10;
    unsigned char a = *((unsigned char*)0xff00);
    a = *((unsigned char*)0xff00);
    a = *((unsigned char*)0xff00);
    a = *((unsigned char*)0xff00);
    a = ~a;
    a = a & 0x0f;
    *((unsigned char*)0xff00) = *(((unsigned char*)0xff00)) & ~0x08;
    unsigned char b = *((unsigned char*)0xff00);
    b = *((unsigned char*)0xff00);
    b = *((unsigned char*)0xff00);
    b = *((unsigned char*)0xff00);
    b = ~b;
    b = b & 0x0f;
    unsigned char port = (a << 4) | b;

    if (port & 0x20) {
        return MR_JOYSTICK_FIRE;
    }
    if (port & 0x10) {
        if (port & 0x08) {
            return MR_JOYSTICK_DOWN;
        }
        if (port & 0x04) {
            return MR_JOYSTICK_UP;
        }
        if (port & 0x02) {
            return MR_JOYSTICK_LEFT;
        }
        if (port & 0x01) {
            return MR_JOYSTICK_RIGHT;
        }
    }

    return 0;

}

void mr_tileset_copy_hd(unsigned char _source, unsigned char _dest) {
    unsigned char i,b;
    unsigned char* source = MR_TM(_source);
    unsigned char* destination = MR_TM(_dest);
    for (i = 0; i < (MR_TILESET_TILE_COUNT - 1); ++i) {
        for (b = 0; b < 16; ++b, ++source, ++destination) {
            MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, MR_READ_VRAM(source) ) );            
        }
    }
}

void mr_tileset_multicolor_to_monocolor_hd(unsigned char _source, unsigned char _starting, unsigned char _count) {
    mr_position w = _count, b = 0;
    unsigned char *source = (unsigned int* )(MR_TM(_source) + _starting * 16);
    for (--w; w != 255; --w) {
        for (b = 0; b < 16; ++b, ++source) {
            unsigned int s;
            MR_PROTECTED_ACCESS_VRAM( s = MR_READ_VRAM(source) );
            s = s | (s >> 1);
            MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(source, s) );
        }
    }
}

void mr_tile_redefine_hd(unsigned char _tileset, unsigned char _tile, unsigned char* _data) {
    unsigned char* destination = (MR_TM(_tileset) + _tile * 16);
    mr_position b = 0;
    for (b = 0; b < 8; ++b, ++_data) {
        unsigned char d = *_data;
        MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, d) );
        ++destination;
        MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, d ) );
        ++destination;
    }
}

void mr_tile_redefine_fill_hd(unsigned char _tileset, unsigned char _tile, unsigned char _data) {
    unsigned char *destination = (MR_TM(_tileset) + _tile * 16);
    unsigned char b = 0;
    for (b = 0; b < 8; ++b) {
        MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, _data) );
        ++destination;
        MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, _data) );
        ++destination;
    }
}

void mr_tile_prepare_horizontal_monocolor_hd(unsigned char _tileset, unsigned char _source, unsigned char _destination) {

    unsigned char *source = (MR_TM(_tileset) + _source * 16);
    unsigned char *destination = (MR_TM(_tileset) + _destination * 15);

    mr_position i, b;

    for (i = 0; i < 9; ++i) {
        for (b = 0; b < 8; ++b) {
            mr_mixel d;
            MR_PROTECTED_ACCESS_VRAM( d = MR_READ_VRAM(source) );
            mr_mixel m = d >> i;
            MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, m ) );
            ++destination; ++source;
            //MR_PROTECTED_ACCESS_VRAM( d = *source );
            //m = d >> i;
            MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, m ) );
            ++destination; ++source;
        }
        source -= 16;
    }

    for (i = 0; i < 8; ++i) {
        for (b = 0; b < 8; ++b) {
            mr_mixel d;
            MR_PROTECTED_ACCESS_VRAM( d = MR_READ_VRAM(source) );
            mr_mixel n = d & (0xff >> (7 - i));
            n = (n << (7 - i));
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, n ) );
            ++destination; ++source;
            //MR_PROTECTED_ACCESS_VRAM( d = *source );
            //n = d & (0xff >> (7 - i));
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, n) );
            ++destination; ++source;
        }
        source -= 16;
    }

}

void mr_tile_prepare_horizontal_extended_monocolor_hd(unsigned char _tileset, unsigned char _source, unsigned char _w, unsigned char _h, unsigned char _destination) {
    unsigned char* source = (MR_TM(_tileset) + _source * 16);
    unsigned char* destination = (MR_TM(_tileset) + _destination * 16);

    mr_position i, b;

    mr_position j, k;

    for (j = 0; j < _h; ++j) {
        for (i = 0; i < 9; ++i) {
            for (b = 0; b < 8; ++b) {
                mr_mixel e;
                MR_PROTECTED_ACCESS_VRAM(e = MR_READ_VRAM(source));
                mr_mixel m = e >> i;
                MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, m));
                ++source; ++destination;
                //MR_PROTECTED_ACCESS_VRAM( e = *(source) );
                //m = e >> i;
                MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, m));
                ++source; ++destination;
            }
            source -= 16;
        }

        for (k = 0; k < (_w - 1); ++k) {
            for (i = 0; i < 9; ++i) {
                for (b = 0; b < 8; ++b) {
                    mr_mixel d,e;
                    MR_PROTECTED_ACCESS_VRAM(d = MR_READ_VRAM(source));
                    MR_PROTECTED_ACCESS_VRAM(e = MR_READ_VRAM(source + 16));
                    mr_mixel m = (e >> i) | (d << (8 - i));
                    MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, m));
                    ++source, ++destination;
                    //MR_PROTECTED_ACCESS_VRAM( d = *source );
                    //e = *(source + 16);
                    //m = (e >> i) | (d << (8 - i));
                    MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, m));
                    ++source, ++destination;
                }
                source -= 16;
            }
            source += 16;
        }

        for (i = 0; i < 9; ++i) {
            for (b = 0; b < 8; ++b) {
                mr_mixel d;
                MR_PROTECTED_ACCESS_VRAM(d = MR_READ_VRAM(source));
                mr_mixel n = d & (0xff >> (8 - i));
                n = (n << (8 - i));
                MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, n ) );
                ++source; ++destination;
                //MR_PROTECTED_ACCESS_VRAM( d = *source );
                //n = d & (0xff >> (8 - i));
                MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, n ) );
                ++source; ++destination;
            }
            source -= 16;
        }
        source += 16;
    }
}

void mr_tile_prepare_vertical_hd(unsigned char _tileset, unsigned char _source, unsigned char _destination) {
    unsigned char *source = (MR_TM(_tileset) + _source * 16);
    unsigned char* destination = (MR_TM(_tileset) + _destination * 16);

    mr_position i, b;

    for (i = 0; i < 9; ++i) {
        for (b = 0; b < i; ++b) {
            MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, 0) );
            ++destination;
            MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, 0) );
            ++destination;
        }
        for (b = 0; b < (8 - i); ++b) {
            unsigned char s;
            MR_PROTECTED_ACCESS_VRAM(s = MR_READ_VRAM(source));
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, s ) );
            ++source; ++destination;
            MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, s ) );
            ++source; ++destination;
        }
        source -= (b*2);
    }

    for (i = 8; i > -1; --i) {
        for (b = 0; b < (8 - i); ++b) {
            unsigned char s;
            MR_PROTECTED_ACCESS_VRAM(s=MR_READ_VRAM(source + i));
            MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, s ) );
            ++source; ++destination;
            MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, s ) );
            ++source; ++destination;
        }
        source -= (b*2);
        for (; b < 8; ++b) {
            MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, 0) );
            ++destination;
            MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, 0) );
            ++destination;
        }
    }
}

void mr_tile_prepare_vertical_extended_hd(unsigned char _tileset, unsigned char _source, unsigned char _w, unsigned char _h, unsigned char _destination) {
    unsigned char *source = (MR_TM(_tileset) + _source * 16);
    unsigned char* destination = (MR_TM(_tileset) + _destination * 16);

    int i, b;
    int w = _w, k = 0;

    for (; w != 0; --w) {
        for (i = 0; i < 9; ++i) {
            for (b = 0; b < i; ++b) {
                MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, 0) );
                ++destination;
                MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, 0) );
                ++destination;
            }
            for (b = 0; b < (8 - i); ++b ) {
                unsigned char s;
                MR_PROTECTED_ACCESS_VRAM(s = MR_READ_VRAM(source));
                MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, s));
                ++source; ++destination;
                MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, s));
                ++source; ++destination;
            }
            source -= (b*2);
        }

        for (k = 0; k < (_h - 1); ++k) {
            for (i = 0; i < 9; ++i) {
                for (b = 0; b < i; ++b) {
                    unsigned char s;
                    MR_PROTECTED_ACCESS_VRAM(s = MR_READ_VRAM(source + (k * _w * 8) + (8 - i)));
                    MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, s ) );
                    ++source; ++destination;
                    MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, s ) );
                    ++source; ++destination;
                }
                source -= (b*2);
                for (b = 0; b < (8 - i); ++b) {
                    unsigned char s;
                    MR_PROTECTED_ACCESS_VRAM(s = MR_READ_VRAM(source + ((k + 1) * _w * 8)));
                    MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, s ) );
                    ++source; ++destination;
                    MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, s ) );
                    ++source; ++destination;
                }
                source -= (b*2);
            }
        }

        for (i = 0; i < 9; ++i) {
            for (b = 0; b < i; ++b) {
                unsigned char s;
                MR_PROTECTED_ACCESS_VRAM(s = MR_READ_VRAM(source + ((_h - 1) * _w * 8) + (8 - i)));
                MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, s ) );
                ++source; ++destination;
                MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, s ) );
                ++source; ++destination;
            }
            source -= (b*2);
            for (b = 0; b < (8 - i); ++b) {
                MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, 0));
                ++destination;
                MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, 0));
                ++destination;
            }
        }
        source += 16;
    }
}

void mr_tile_prepare_roll_horizontal_monocolor_hd(unsigned char _tileset, unsigned char _source, unsigned char _destination) {
    unsigned char *source = (MR_TM(_tileset) + _source * 16);
    unsigned char* destination = (MR_TM(_tileset) + _destination * 16);

    mr_position i, b;

    for (i = 0; i < 8; ++i) {
        for (b = 0; b < 8; ++b) {
            mr_mixel d;
            MR_PROTECTED_ACCESS_VRAM( d = MR_READ_VRAM(source) );
            mr_mixel m = d >> i, n = d & (0xff >> (8 - i));
            m = m | (n << (8 - i));
            MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, m ) );
            ++source; ++destination;
            //MR_PROTECTED_ACCESS_VRAM( d = *source );
            //m = d >> i, n = d & (0xff >> (8 - i));
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, m ) );
            ++source; ++destination;
        }
        source -= 16;
    }
}

extern mr_mixel rollBuffer[16];

void mr_tile_roll_horizontal_hd(unsigned char _tileset, unsigned char _destination, unsigned char _direction) {
    unsigned char *source;
    unsigned char* destination;
    unsigned char* temp = &rollBuffer[0];
    mr_position i, b;

    if (_direction == mr_direction_right) {
        source = (MR_TM(_tileset) + (_destination + 1) * 16);
        destination = (MR_TM(_tileset) + _destination * 16);

        for (b = 0; b < 8; ++b) {
            mr_mixel d;
            MR_PROTECTED_ACCESS_VRAM( d = MR_READ_VRAM(source - 8) );
            *temp = d;
            ++source; ++temp;
            //MR_PROTECTED_ACCESS_VRAM( d = *(source - 8) );
            *temp = d;
            ++source; ++temp;
        }
        temp -= 16;
        source -= 16;
        for (i = 0; i < 6; ++i) {
            for (b = 0; b < 8; ++b) {
                mr_mixel d;
                MR_PROTECTED_ACCESS_VRAM( d = MR_READ_VRAM(source) );
                MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, d ) );
                ++source; ++destination;
                //MR_PROTECTED_ACCESS_VRAM( d = *(source) );
                MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, d));
                ++source; ++destination;
            }
        }
        for (b = 0; b < 8; ++b) {
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, *temp));
            ++temp; ++destination;
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, *temp));
            ++temp; ++destination;
        }
    }
    else {
        source = (MR_TM(_tileset) + (_destination + 6) * 16);
        destination = (MR_TM(_tileset) + (_destination + 7) * 16);
        for (b = 0; b < 8; ++b) {
            mr_mixel d;
            MR_PROTECTED_ACCESS_VRAM( d = MR_READ_VRAM(destination) );
            *temp = d;
            ++destination; ++temp;
            //MR_PROTECTED_ACCESS_VRAM( d = *(destination) );
            *temp = d;
            ++destination; ++temp;
        }
        temp -= 16;
        destination -= 16;
        for (i = 0; i < 7; ++i) {
            for (b = 0; b < 8; ++b) {
                mr_mixel d;
                MR_PROTECTED_ACCESS_VRAM( d = MR_READ_VRAM(source) );
                MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, d) );
                ++source; ++destination;
                //MR_PROTECTED_ACCESS_VRAM( d = *(source) );
                MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, d) );
                ++source; ++destination;
            }
            source -= 16;
            destination -= 16;
        }
        for (b = 0; b < 8; ++b) {
            MR_PROTECTED_ACCESS_VRAM(*temp = MR_READ_VRAM(destination));
            ++destination; ++temp;
            MR_PROTECTED_ACCESS_VRAM(*temp = MR_READ_VRAM(destination));
            ++destination; ++temp;
        }
    }
}

void mr_tile_roll_horizontal_on_place_hd(unsigned char _tileset, unsigned char _destination, unsigned char _direction, unsigned char _place, unsigned char* _index) {
    unsigned char *source;
    unsigned char* destination;
    mr_position b;

    source = (MR_TM(_tileset) + (_destination * 16) + (*_index) * 16);
    destination = (MR_TM(_tileset) + _place * 16);
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
    for (b = 0; b < 8; ++b) {
        mr_mixel d;
        MR_PROTECTED_ACCESS_VRAM(d = MR_READ_VRAM(source));
        MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, d) );
        ++source; ++destination;
        //MR_PROTECTED_ACCESS_VRAM( d = *(source) );
        MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, d));
        ++source; ++destination;
    }
}

void mr_tile_prepare_roll_vertical_hd(unsigned char _tileset, unsigned char _source, unsigned char _destination) {
    unsigned char *source = (MR_TM(_tileset) + _source * 16);
    unsigned char* destination = (MR_TM(_tileset) + _destination * 16);

    mr_position i, b;

    for (i = 0; i < 8; ++i) {
        for (b = 0; b < (8 - i); ++b, ++source, ++destination) {
            unsigned char s;
            MR_PROTECTED_ACCESS_VRAM( s = MR_READ_VRAM(source + i));
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, s));
            ++source; ++destination;
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, s));
            ++source; ++destination;
        }
        source -= (b*2);
        for (b = 0; b < i; ++b, ++source, ++destination) {
            unsigned char s;
            MR_PROTECTED_ACCESS_VRAM(s = MR_READ_VRAM(source));
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, s));
            ++source; ++destination;
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, s));
            ++source; ++destination;
        }
        source -= (b*2);
    }
}

void mr_tile_roll_vertical_hd(unsigned char _tileset, unsigned char _destination, unsigned char _direction) {
    unsigned char *source;
    unsigned char* destination;
    mr_mixel* temp = &rollBuffer[0];
    mr_position i, b;

    if (_direction == mr_direction_up) {
        source = (MR_TM(_tileset) + (_destination + 1) * 8);
        destination = (MR_TM(_tileset) + _destination * 8);

        for (b = 0; b < 8; ++b) {
            mr_mixel d;
            MR_PROTECTED_ACCESS_VRAM( d = MR_READ_VRAM(source - 16) );
            *temp = d;
            ++source; ++temp;
            //MR_PROTECTED_ACCESS_VRAM( d = *(source - 16) );
            *temp = d;
            ++source; ++temp;
        }
        temp -= 16;
        source -= 16;
        for (i = 0; i < 6; ++i) {
            for (b = 0; b < 8; ++b) {
                mr_mixel d;
                MR_PROTECTED_ACCESS_VRAM( d = MR_READ_VRAM(source) );
                MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, d));
                ++source; ++destination;
                //MR_PROTECTED_ACCESS_VRAM( d = *(source) );
                MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, d));
                ++source; ++destination;
            }
        }
        for (b = 0; b < 8; ++b) {
            unsigned char t = *temp;
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, t));
            ++destination; ++temp;
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, t));
            ++destination; ++temp;
        }
    }
    else {
        source = (MR_TM(_tileset) + (_destination + 6) * 8);
        destination = (MR_TM(_tileset) + (_destination + 7) * 8);
        for (b = 0; b < 8; ++b) {
            mr_mixel d;
            MR_PROTECTED_ACCESS_VRAM( d = MR_READ_VRAM(destination) );
            *temp = d;
            ++destination; ++temp;
            //MR_PROTECTED_ACCESS_VRAM( d = *(destination) );
            *temp = d;
            ++destination; ++temp;
        }
        temp -= 16;
        destination -= 16;
        for (i = 0; i < 7; ++i) {
            for (b = 0; b < 8; ++b) {
                mr_mixel d;
                MR_PROTECTED_ACCESS_VRAM( d = MR_READ_VRAM(source) );
                MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(destination, d ) );
                ++source; ++destination;
                //MR_PROTECTED_ACCESS_VRAM( d = *(source) );
                MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, d));
                ++source; ++destination;
            }
            source -= 16;
            destination -= 16;
        }
        for (b = 0; b < 8; ++b) {
            unsigned char t = *temp;
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, t));
            ++destination; ++temp;
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(destination, t));
            ++destination; ++temp;
        }
    }
}

void mr_tileset_load_file_hd(unsigned int _index, unsigned char _tileset, unsigned char _starting, unsigned char _count) {
    int i;
    unsigned char* tileData = MR_TM(_tileset) + (_starting * 16);
    for (i = 0; i < (_count*8); ++i) {
        mr_mixel d = _includedFiles[_index][i];
        MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(tileData, d) );
        ++tileData;
        MR_PROTECTED_ACCESS_VRAM( MR_WRITE_VRAM(tileData, d) );
        ++tileData;
    }
}

clock_t clock() {
    return *((unsigned int*)0xFF04);
}

void mr_read_file_hd(unsigned int _file, unsigned int _offset, unsigned char* _dest, unsigned int _size) {
    memcpy(_dest, &_includedFiles[_file][_offset], _size);
}

#endif