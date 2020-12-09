#ifdef __MSX__

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
#include <time.h>
#include <stdlib.h>

#include "midres.h"
#include "rawdata.h"

#define VDP_R0          0x80
#define VDP_R1          0x81
#define VDP_RNAME       0x82
#define VDP_RCOLORTABLE 0x83
#define VDP_RPATTERN    0x84
#define VDP_RSPRITEA    0x85
#define VDP_RSPRITEP    0x86
#define VDP_RCOLOR      0x87

#define PSG_R14         0x14
#define PSG_R15         0x15

unsigned char MR_RENDERED_MIXELS_MSX[16] = {
   0x00, 0x01, 0x02, 0x03,
   0x04, 0x05, 0x06, 0x07,
   0x08, 0x09, 0x10, 0x11,
   0x12, 0x13, 0x14, 0x15
};

/****************************************************************************
 ** RESIDENT VARIABLES SECTION
 ****************************************************************************/

 /****************************************************************************
  ** RESIDENT FUNCTIONS SECTION
  ****************************************************************************/

  // The functions defined at this level can only be called up if the current
  // module has been loaded into memory. On the other hand, they can call any 
  // function declared at the resident module level.

#ifdef FRAME_BUFFER
unsigned char frameBuffer[MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT];
unsigned char colorBuffer[MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT];
#endif

void mr_init_hd() {

#ifdef FRAME_BUFFER
    memset(&frameBuffer[0], 0, MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT);
    memset(&colorBuffer[0], 0, MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT);
#endif

    vdp_port(0x98);
#ifdef GRAPHIC_MODE_I
    vdp_out(VDP_R0, 0x00);
    vdp_out(VDP_R1, 0xe0);
    vdp_out(VDP_RNAME, MR_SCREEN_DEFAULT);
    vdp_out(VDP_RCOLORTABLE, 0x80);
    vdp_out(VDP_RPATTERN, MR_TILESET_DEFAULT);

    vdp_put(&_includedFiles[0][0], 0x0800, 128);
#else
    vdp_out(VDP_R0, 0x02);
    vdp_out(VDP_R1, 0xc0);
    vdp_out(VDP_RNAME, 0x0e);
    vdp_out(VDP_RCOLORTABLE, 0xff);
    vdp_out(VDP_RPATTERN, 0x03);

    //vdp_put(&_includedFiles[0][0], 0x0000, 128);
    //vdp_put(&_includedFiles[0][0], 0x0800, 128);
    //vdp_put(&_includedFiles[0][0], 0x1000, 128);
#endif
    vdp_out(VDP_RSPRITEA, 0x76);
    vdp_out(VDP_RSPRITEP, 0x03);
    vdp_out(VDP_RCOLOR, 0x0f);

}

void mr_init_multicolor_hd() {

#ifdef FRAME_BUFFER
    memset(&frameBuffer[0], 0, MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT);
    memset(&colorBuffer[0], 0, MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT);
#endif

    vdp_port(0x98);
#ifdef GRAPHIC_MODE_I
    vdp_out(VDP_R0, 0x00);
    vdp_out(VDP_R1, 0xe0);
    vdp_out(VDP_RNAME, MR_SCREEN_DEFAULT);
    vdp_out(VDP_RCOLORTABLE, 0x80);
    vdp_out(VDP_RPATTERN, MR_TILESET_DEFAULT);

    vdp_put(&_includedFiles[0][0], 0x0800, 128);
#else
    vdp_out(VDP_R0, 0x02);
    vdp_out(VDP_R1, 0xc0);
    vdp_out(VDP_RNAME, 0x0e);
    vdp_out(VDP_RCOLORTABLE, 0xff);
    vdp_out(VDP_RPATTERN, 0x03);

    vdp_put(&_includedFiles[0][0], 0x0000, 128);
    vdp_put(&_includedFiles[0][0], 0x0800, 128);
    vdp_put(&_includedFiles[0][0], 0x1000, 128);
#endif
    vdp_out(VDP_RSPRITEA, 0x76);
    vdp_out(VDP_RSPRITEP, 0x03);
    vdp_out(VDP_RCOLOR, 0x0f);


}

void mr_tile_setcolor_hd(unsigned char _index, unsigned char _color) {

}

void mr_tile_setcolors_hd(unsigned char _colors[4]) {

}

void mr_show_hd(unsigned char _screen) {
    vdp_out(VDP_RNAME, _screen);
#ifdef GRAPHIC_MODE_I
    vdp_out(VDP_RCOLORTABLE, 0x80 + (_screen - MR_SCREEN_DEFAULT));
#else
    vdp_out(VDP_RCOLORTABLE, (_screen==MR_SCREEN_0)?0xff:0x7f);
#endif
    MR_VISIBLE_SCREEN = _screen;
    MR_ENABLED_SCREEN = _screen;
}

void mr_cleanup_hd() {

}

void mr_wait_vbl() {

}

void mr_doublebuffer_switch_hd(unsigned char _screen) {

}

void mr_tileset_visible_hd(unsigned char _tileset) {
#ifdef GRAPHIC_MODE_I
    vdp_out(VDP_RPATTERN, _tileset);
#endif
}

unsigned char mr_key_pressed_hd() {
    return mr_get_key_pressed_hd() != MR_KEY_NONE;
}

unsigned char mr_get_key_pressed_hd() {
    
    unsigned char key;
    
    key = *((unsigned char*)0xFBE5);
    if (key != 0xff) {
        if (key == 0xfe) { return MR_KEY_0; };
        if (key == 0xfd) { return MR_KEY_1; };
        if (key == 0xfb) { return MR_KEY_2; };
        if (key == 0xf7) { return MR_KEY_3; };
        if (key == 0xef) { return MR_KEY_4; };
        if (key == 0xdf) { return MR_KEY_5; };
        if (key == 0xbf) { return MR_KEY_6; };
        if (key == 0x7f) { return MR_KEY_7; };
    }
    key = *((unsigned char*)0xFBE6);
    if (key != 0xff) {
        if (key == 0xfe) { return MR_KEY_8; };
        if (key == 0xfd) { return MR_KEY_9; };
        if (key == 0xfb) { return MR_KEY_MINUS; };
        if (key == 0xf7) { return MR_KEY_EQUAL; };
        if (key == 0xef) { return MR_KEY_BACKSLASH; };
        if (key == 0xdf) { return MR_KEY_SQUARE_OPEN; };
        if (key == 0xbf) { return MR_KEY_SQUARE_CLOSED; };
        if (key == 0x7f) { return MR_KEY_SEMICOMMA; };
    }
    key = *((unsigned char*)0xFBE7);
    if (key != 0xff) {
        if (key == 0xfe) { return MR_KEY_QUOTE; };
        if (key == 0xfd) { return MR_KEY_APIX; };
        if (key == 0xfb) { return MR_KEY_COMMA; };
        if (key == 0xf7) { return MR_KEY_PERIOD; };
        if (key == 0xef) { return MR_KEY_SLASH; };
        if (key == 0xdf) { return MR_KEY_DEAD; };
        if (key == 0xbf) { return MR_KEY_A; };
        if (key == 0x7f) { return MR_KEY_B; };
    }
    key = *((unsigned char*)0xFBE8);
    if (key != 0xff) {
        if (key == 0xfe) { return MR_KEY_C; };
        if (key == 0xfd) { return MR_KEY_D; };
        if (key == 0xfb) { return MR_KEY_E; };
        if (key == 0xf7) { return MR_KEY_F; };
        if (key == 0xef) { return MR_KEY_G; };
        if (key == 0xdf) { return MR_KEY_H; };
        if (key == 0xbf) { return MR_KEY_I; };
        if (key == 0x7f) { return MR_KEY_J; };
    }
    key = *((unsigned char*)0xFBE9);
    if (key != 0xff) {
        if (key == 0xfe) { return MR_KEY_K; };
        if (key == 0xfd) { return MR_KEY_L; };
        if (key == 0xfb) { return MR_KEY_M; };
        if (key == 0xf7) { return MR_KEY_N; };
        if (key == 0xef) { return MR_KEY_O; };
        if (key == 0xdf) { return MR_KEY_P; };
        if (key == 0xbf) { return MR_KEY_Q; };
        if (key == 0x7f) { return MR_KEY_R; };
    }
    key = *((unsigned char*)0xFBEA);
    if (key != 0xff) {
        if (key == 0xfe) { return MR_KEY_S; };
        if (key == 0xfd) { return MR_KEY_T; };
        if (key == 0xfb) { return MR_KEY_U; };
        if (key == 0xf7) { return MR_KEY_V; };
        if (key == 0xef) { return MR_KEY_W; };
        if (key == 0xdf) { return MR_KEY_X; };
        if (key == 0xbf) { return MR_KEY_Y; };
        if (key == 0x7f) { return MR_KEY_Z; };
    }
    key = *((unsigned char*)0xFBEB);
    if (key != 0xff) {
        if (key == 0xfe) { return MR_KEY_SHIFT; };
        if (key == 0xfd) { return MR_KEY_CTRL; };
        if (key == 0xfb) { return MR_KEY_GRAPH; };
        if (key == 0xf7) { return MR_KEY_CAPS; };
        if (key == 0xef) { return MR_KEY_CODE; };
        if (key == 0xdf) { return MR_KEY_F1; };
        if (key == 0xbf) { return MR_KEY_F2; };
        if (key == 0x7f) { return MR_KEY_F3; };
    }
    key = *((unsigned char*)0xFBEC);
    if (key != 0xff) {
        if (key == 0xfe) { return MR_KEY_F4; };
        if (key == 0xfd) { return MR_KEY_F5; };
        if (key == 0xfb) { return MR_KEY_ESC; };
        if (key == 0xf7) { return MR_KEY_TAB; };
        if (key == 0xef) { return MR_KEY_STOP; };
        if (key == 0xdf) { return MR_KEY_BS; };
        if (key == 0xbf) { return MR_KEY_SELECT; };
        if (key == 0x7f) { return MR_KEY_RET; };
    }
    key = *((unsigned char*)0xFBED);
    if (key != 0xff) {
        if (key == 0xfe) { return MR_KEY_F4; };
        if (key == 0xfd) { return MR_KEY_F5; };
        if (key == 0xfb) { return MR_KEY_ESC; };
        if (key == 0xf7) { return MR_KEY_TAB; };
        if (key == 0xef) { return MR_KEY_STOP; };
        if (key == 0xdf) { return MR_KEY_BS; };
        if (key == 0xbf) { return MR_KEY_SELECT; };
        if (key == 0x7f) { return MR_KEY_RET; };
    }
    key = *((unsigned char*)0xFBEE);
    if (key != 0xff) {
        if (key == 0xfe) { return MR_KEY_SPACE; };
        if (key == 0xfd) { return MR_KEY_HOME; };
        if (key == 0xfb) { return MR_KEY_INS; };
        if (key == 0xf7) { return MR_KEY_DEL; };
        if (key == 0xef) { return MR_KEY_LEFT; };
        if (key == 0xdf) { return MR_KEY_UP; };
        if (key == 0xbf) { return MR_KEY_DOWN; };
        if (key == 0x7f) { return MR_KEY_RIGHT; };
    }
    key = *((unsigned char*)0xFBEF);
    if (key != 0xff) {
        if (key == 0xfe) { return MR_KEY_ASTERISK; };
        if (key == 0xfd) { return MR_KEY_PLUS; };
        if (key == 0xfb) { return MR_KEY_DIVISION; };
        if (key == 0xf7) { return MR_KEY_0; };
        if (key == 0xef) { return MR_KEY_1; };
        if (key == 0xdf) { return MR_KEY_2; };
        if (key == 0xbf) { return MR_KEY_3; };
        if (key == 0x7f) { return MR_KEY_4; };
    }
    key = *((unsigned char*)0xFBF0);
    if (key != 0xff) {
        if (key == 0xfe) { return MR_KEY_5; };
        if (key == 0xfd) { return MR_KEY_6; };
        if (key == 0xfb) { return MR_KEY_7; };
        if (key == 0xf7) { return MR_KEY_8; };
        if (key == 0xef) { return MR_KEY_9; };
        if (key == 0xdf) { return MR_KEY_MINUS; };
        if (key == 0xbf) { return MR_KEY_COMMA; };
        if (key == 0x7f) { return MR_KEY_PERIOD; };
    }
    return MR_KEY_NONE;
}

void mr_wait_hd(unsigned char _seconds) {
    sleep(_seconds);
}

void mr_wait_jiffies_hd(unsigned char _jiffies) {
    unsigned int actual = *((unsigned int*)0xFC9E);
    while ( (*((unsigned int*)0xFC9E) - actual ) < (unsigned int)_jiffies) {

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
    vdp_out(VDP_RCOLOR, _color & 0x0f );
}

unsigned int storedJiffy = 0;

void mr_start_frame_hd() {
    storedJiffy = *((unsigned int*)0xFC9E);
}

void mr_end_frame_hd(unsigned char _jiffies) {
    while ((*((unsigned int*)0xFC9E) - storedJiffy) < _jiffies) {

    }
#ifdef FRAME_BUFFER
#ifdef GRAPHIC_MODE_I
    vdp_put(&frameBuffer[0], MR_VISIBLE_SCREEN * 0x400, MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT);
#else
    vdp_put(&frameBuffer[0], MR_VISIBLE_SCREEN == MR_SCREEN_0 ? 0x3800 : 0x4000, MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT);
    vdp_put8(&colorBuffer[0], MR_VISIBLE_SCREEN == MR_SCREEN_0 ? 0x2000 : 0x0000, MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT);
#endif
#endif
}

unsigned char* mr_translate_file_hd(mr_file _file) {

}

void mr_read_file_hd(unsigned int _file, unsigned int _offset, unsigned char* _dest, unsigned int _size) {
    memcpy(_dest, &_includedFiles[_file][_offset], _size);
}

unsigned char mr_joy_hd(unsigned char _number) {

    unsigned char r14;
    
    io_put(PSG_R15, ( io_get(PSG_R15) & ~0xdf) | (0x20 * _number));
    r14 = io_get(PSG_R14);

    return ~r14;
}

void mr_tileset_copy_hd(unsigned char _source, unsigned char _dest) {
    unsigned char i;
    for (i = 0; i < (MR_TILESET_TILE_COUNT - 1); ++i) {
        mr_position w = 255, b = 0;
        unsigned int source = MR_TM(_source);
        unsigned int destination = MR_TM(_dest);
        for (--w; w != 255; --w) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                vdp_fill8(vdp_get(source), destination, 1);
            }
        }
    }
}

void mr_tileset_multicolor_to_monocolor_hd(unsigned char _source, unsigned char _starting, unsigned char _count) {
    mr_position w = _count, b = 0;
    unsigned int source = MR_TM(_source) + _starting * 8;
    for (--w; w != 255; --w) {
        for (b = 0; b < 8; ++b, ++source) {
            unsigned char s = vdp_get(source);
            s = s | (s >> 1);
            vdp_fill8(s, source, 1);
        }
    }
}

void mr_tile_redefine_hd(unsigned char _tileset, unsigned char _tile, unsigned char* _data) {
    unsigned int destination = (MR_TM(_tileset) + _tile * 8);
    mr_position b = 0;
    for (b = 0; b < 8; ++b, ++destination, ++_data) {
        vdp_fill8(*_data, destination, 1);
    }
}

void mr_tile_redefine_fill_hd(unsigned char _tileset, unsigned char _tile, unsigned char _data) {
    unsigned int destination = (MR_TM(_tileset) + _tile * 8);
    unsigned char b = 0;
    for (b = 0; b < 8; ++destination, ++b) {
        vdp_fill8(_data, destination, 1);
    }
}

void mr_tile_prepare_horizontal_monocolor_hd(unsigned char _tileset, unsigned char _source, unsigned char _destination) {

    unsigned int source = (MR_TM(_tileset) + _source * 8);
    unsigned int destination = (MR_TM(_tileset) + _destination * 8);

    mr_position i, b;

    for (i = 0; i < 9; ++i) {
        for (b = 0; b < 8; ++b, ++source, ++destination) {
            mr_mixel d = vdp_get(source);
            mr_mixel m = d >> i;
            vdp_fill8(m, destination, 1);
        }
        source -= 8;
    }

    for (i = 0; i < 8; ++i) {
        for (b = 0; b < 8; ++b, ++source, ++destination) {
            mr_mixel d = *((mr_mixel*)source);
            mr_mixel n = d & (0xff >> (7 - i));
            vdp_fill8((n << (7 - i)), destination, 1);
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
                mr_mixel e = vdp_get(source);
                mr_mixel m = e >> i;
                vdp_fill8(m, destination, 1);
            }
            source -= 8;
        }

        for (k = 0; k < (_w - 1); ++k) {
            for (i = 0; i < 9; ++i) {
                for (b = 0; b < 8; ++b, ++source, ++destination) {
                    mr_mixel d = vdp_get(source);
                    mr_mixel e = vdp_get(source + 8);
                    mr_mixel m = (e >> i) | (d << (8 - i));
                    vdp_fill8(m, destination, 1);
                }
                source -= 8;
            }
            source += 8;
        }

        for (i = 0; i < 9; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel d = vdp_get(source);
                mr_mixel n = d & (0xff >> (8 - i));
                vdp_fill8((n << (8 - i)), destination, 1);
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
            vdp_fill8(0x00, destination, 1);
        }
        for (b = 0; b < (8 - i); ++b, ++source, ++destination) {
            vdp_fill8(vdp_get(source), destination, 1);
        }
        source -= b;
    }

    for (i = 8; i != 255; --i) {
        for (b = 0; b < (8 - i); ++b, ++source, ++destination) {
            vdp_fill8(vdp_get(source + i), destination, 1);
        }
        source -= b;
        for (; b < 8; ++b, ++destination) {
            vdp_fill8(0x00, destination, 1);
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
                vdp_fill8(0x00, destination, 1);
            }
            for (b = 0; b < (8 - i); ++b, ++source, ++destination) {
                vdp_fill8(vdp_get(source), destination, 1);
            }
            source -= b;
        }

        for (k = 0; k < (_h - 1); ++k) {
            for (i = 0; i < 9; ++i) {
                for (b = 0; b < i; ++b, ++destination, ++source) {
                    vdp_fill8(vdp_get(source + (k * _w * 8) + (8 - i)), destination, 1);
                }
                source -= b;
                for (b = 0; b < (8 - i); ++b, ++source, ++destination) {
                    vdp_fill8(vdp_get(source + ((k + 1) * _w * 8)), destination, 1);
                }
                source -= b;
            }
        }

        for (i = 0; i < 9; ++i) {
            for (b = 0; b < i; ++b, ++destination, ++source) {
                vdp_fill8(vdp_get(source + ((_h - 1) * _w * 8) + (8 - i)), destination, 1);
            }
            source -= b;
            for (b = 0; b < (8 - i); ++b, ++destination) {
                vdp_fill8(0x00, destination, 1);
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
            mr_mixel d = vdp_get(source);
            mr_mixel m = d >> i, n = d & (0xff >> (8 - i));
            vdp_fill8(m | (n << (8 - i)), destination, 1);
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
            mr_mixel d = vdp_get(source - 8);
            *temp = d;
        }
        temp -= 8;
        source -= 8;
        for (i = 0; i < 6; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel d = vdp_get(source);
                vdp_fill8(d, destination, 1);
            }
        }
        for (b = 0; b < 8; ++b, ++destination, ++temp) {
            vdp_fill8(*temp, destination, 1);
        }
    }
    else {
        source = (MR_TM(_tileset) + (_destination + 6) * 8);
        destination = (MR_TM(_tileset) + (_destination + 7) * 8);
        for (b = 0; b < 8; ++b, ++destination, ++temp) {
            mr_mixel d = vdp_get(destination);
            *temp = d;
        }
        temp -= 8;
        destination -= 8;
        for (i = 0; i < 7; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel d = vdp_get(source);
                vdp_fill8(d, destination, 1);
            }
            source -= 16;
            destination -= 16;
        }
        for (b = 0; b < 8; ++b, ++destination, ++temp) {
            vdp_fill8(*temp, destination, 1);
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
        mr_mixel d = vdp_get(source);
        vdp_fill8(d, destination, 1);
    }
}

void mr_tile_prepare_roll_vertical_hd(unsigned char _tileset, unsigned char _source, unsigned char _destination) {
    unsigned int source = (MR_TM(_tileset) + _source * 8);
    unsigned int destination = (MR_TM(_tileset) + _destination * 8);

    mr_position i, b;

    for (i = 0; i < 8; ++i) {
        for (b = 0; b < (8 - i); ++b, ++source, ++destination) {
            vdp_fill8(vdp_get(source + i), destination, 1);
        }
        source -= b;
        for (b = 0; b < i; ++b, ++source, ++destination) {
            vdp_fill8(vdp_get(source), destination, 1);
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
            mr_mixel d = vdp_get(source - 8);
            *temp = d;
        }
        temp -= 8;
        source -= 8;
        for (i = 0; i < 6; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel d = vdp_get(source);
                vdp_fill8(d, destination, 1);
            }
        }
        for (b = 0; b < 8; ++b, ++destination, ++temp) {
            vdp_fill8(*temp, destination, 1);
        }
    }
    else {
        source = (MR_TM(_tileset) + (_destination + 6) * 8);
        destination = (MR_TM(_tileset) + (_destination + 7) * 8);
        for (b = 0; b < 8; ++b, ++destination, ++temp) {
            mr_mixel d = vdp_get(destination);
            *temp = d;
        }
        temp -= 8;
        destination -= 8;
        for (i = 0; i < 7; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel d = vdp_get(source);
                vdp_fill8(d, destination, 1);
            }
            source -= 16;
            destination -= 16;
        }
        for (b = 0; b < 8; ++b, ++destination, ++temp) {
            vdp_fill8(*temp, destination, 1);
        }
    }
}

void mr_tileset_load_file_hd(unsigned char _index, unsigned char _tileset, unsigned char _starting, unsigned char _count) {
    vdp_put(&_includedFiles[_index][0], MR_TM(_tileset) + (_starting * 8), _count * 8);
#ifdef GRAPHIC_MODE_II
    vdp_put(&_includedFiles[_index][0], 0x800 + MR_TM(_tileset) + (_starting * 8), _count * 8);
    vdp_put(&_includedFiles[_index][0], 0x1000 + MR_TM(_tileset) + (_starting * 8), _count * 8);
#endif
}

#endif