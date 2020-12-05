#ifdef __C128__

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
#include <cc65.h>
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

#define SET_BACKGROUND_COLOR( _color ) \
    *((unsigned char*)0xd021) = _color;

#define SET_BORDER_COLOR( _color ) \
    *((unsigned char*)0xd020) = _color;

#define SET_DATA_DIRECTION( ) \
    *((unsigned char*)0x00d8) = 0xff; \
    *((unsigned char*)0xdd02) = ((*((unsigned char*)0xdd02)) ) | 3;

#define SET_BANK( _bank ) \
    *((unsigned char*)0xdd00) = ((*((unsigned char*)0xdd00)) & 0xfc) | (3 - _bank); // bank 2

#define SET_VIDEO( _video ) \
    *((unsigned char*)0xd018) = ((*((unsigned char*)0xd018))& 0x0f) | (_video << 4);

#define SET_BASIC_VIDEO( _video ) \
    *((unsigned char*)0x0288) = ( (int)(MR_SM(_video)) >> 8 ) & 0xff;

#define SET_CHARSET( _tileset ) \
    *((unsigned char*)0xd018) = (*((unsigned char*)0xd018) & 0xf1) | (( _tileset & 0x07 )<<1);

#define SET_MULTICOLOR_MODE( ) \
    *((unsigned char*)0xd016) = (*((unsigned char*)0xd016) | 0x10);

#define SET_MONOCOLOR_MODE( ) \
    *((unsigned char*)0xd016) = (*((unsigned char*)0xd016) & ~0x10);

void mr_init_base_hd() {

    int i;
    unsigned char* dst = (unsigned char*)0x8c00, * src = (unsigned char*)0xd800;

    memset(MR_SM(MR_SCREEN_DEFAULT), 32, 0x400);

    SET_DATA_DIRECTION();
    SET_BANK(2);
    SET_BACKGROUND_COLOR(MR_COLOR_BLACK);
    SET_VIDEO(MR_SCREEN_DEFAULT);
    SET_BASIC_VIDEO(MR_SCREEN_DEFAULT);
    SET_CHARSET(MR_TILESET_DEFAULT);

    for (i = 0; i < MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT; ++i) {
        *(dst + i) = *(src + i);
    }

}

void mr_init_hd() {
    mr_init_base_hd();
    SET_MONOCOLOR_MODE();
}

void mr_init_multicolor_hd() {
    mr_init_base_hd();
    SET_MULTICOLOR_MODE();
}

void mr_tile_setcolor_hd(unsigned char _index, unsigned char _color) {
    switch (_index) {
    case MR_TILE_BACKGROUND:
        // currently ignored!
        break;
    case MR_TILE_COLOR0:
        *((unsigned char*)0xd022) = _color;
        break;
    case MR_TILE_COLOR1:
        *((unsigned char*)0xd023) = _color;
        break;
    case MR_TILE_COLOR2:
        // currently ignored!
        break;
    }
}

void mr_tile_setcolors_hd(unsigned char _colors[4]) {
    // *((unsigned char*)0x0000) = _colors[MR_TILE_BACKGROUND];
    *((unsigned char*)0xd022) = _colors[MR_TILE_COLOR0];
    *((unsigned char*)0xd023) = _colors[MR_TILE_COLOR1];
    // *((unsigned char*)0xff18) = _colors[MR_TILE_COLOR2];
}

void mr_show_hd(unsigned char _screen) {

    MR_VISIBLE_SCREEN = _screen;
    MR_ENABLED_SCREEN = _screen;

    SET_VIDEO(_screen);
    SET_BASIC_VIDEO(_screen);

}

void mr_cleanup_hd() {

    unsigned char* dst = (unsigned char*)0xd800, * src = (unsigned char*)0x8c00;

    /*for (i = 0; i < MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT; ++i) {
        *(dst + i) = *(src + i);
    }*/

    SET_BACKGROUND_COLOR(MR_COLOR_BLUE);
    // SET_DATA_DIRECTION();
    SET_BANK(0);
    SET_VIDEO(MR_SCREEN_DEFAULT);
    SET_BASIC_VIDEO(MR_SCREEN_DEFAULT);
    SET_CHARSET(MR_TILESET_DEFAULT);

    MR_VISIBLE_SCREEN = MR_SCREEN_DEFAULT;
    MR_ENABLED_SCREEN = MR_SCREEN_DEFAULT;

}

void mr_wait_vbl() {
    while (*((unsigned char*)0xd012) != 0xff) {}
}

void mr_doublebuffer_switch_hd(unsigned char _screen) {
    unsigned char _other = (_screen == MR_DB1) ? MR_DB2 : MR_DB1;

    memcpy(MR_SM(_screen), MR_SM(_other), 0x400);
}

void mr_tileset_visible_hd(unsigned char _tileset) {
    SET_CHARSET(_tileset);
}

unsigned char mr_key_pressed_hd() {
    return (*(unsigned char*)0x00d4) != 88;
}

unsigned char mr_get_key_pressed_hd() {
    return (*(unsigned char*)0x00d4);
}

void mr_wait_hd(unsigned char _seconds) {

    unsigned char now = *((unsigned char*)0xa1);

    while ((*((unsigned char*)0xa1) - now) < _seconds) {
        ; // nop!
    }
}

void mr_wait_jiffies_hd(unsigned char _jiffies) {
    unsigned char now = *((unsigned char*)0xa2);
    while ((*((unsigned char*)0xa2) - now) < _jiffies) {
        ; // nop!
    }
}

// Hardware dependent sound library
void mr_sound_start_hd(unsigned char _channel, unsigned char _number) {

    //@todo: implement support for _number of mr_sound_start_hd of c128
    _number = 0;

    switch ((_channel & 0x03)) {
    case 0: case 3:
        *((unsigned char*)0xd405) = 97;
        *((unsigned char*)0xd406) = 200;
        *((unsigned char*)0xd404) = 33; // 65, 129
        break;
    case 1:
        *((unsigned char*)0xd40c) = 97;
        *((unsigned char*)0xd40d) = 200;
        *((unsigned char*)0xd40b) = 33; // 65, 129
        break;
    case 2:
        *((unsigned char*)0xd413) = 97;
        *((unsigned char*)0xd414) = 200;
        *((unsigned char*)0xd412) = 33; // 65, 129
        break;
    }

    *((unsigned char*)0xd418) = 15;

}

// Hardware dependent sound library
void mr_sound_change_hd(unsigned char _channel, int _parameter) {

    switch ((_channel & 0x03)) {
    case 0: case 3:
        *((unsigned char*)0xd400) = _parameter & 0xff;
        *((unsigned char*)0xd401) = _parameter >> 8;
        break;
    case 1:
        *((unsigned char*)0xd407) = _parameter & 0xff;
        *((unsigned char*)0xd408) = _parameter >> 8;
        break;
    case 2:
        *((unsigned char*)0xd40e) = _parameter & 0xff;
        *((unsigned char*)0xd40f) = _parameter >> 8;
        break;
    }

}

// Hardware dependent sound library
void mr_sound_stop_hd(unsigned char _channel) {

    switch ((_channel & 0x03)) {
    case 0: case 3:
        *((unsigned char*)0xd404) = *((unsigned char*)0xd404) & (~1);
        break;
    case 1:
        *((unsigned char*)0xd40b) = *((unsigned char*)0xd40b) & (~1);
        break;
    case 2:
        *((unsigned char*)0xd412) = *((unsigned char*)0xd412) & (~1);
        break;
    }

    // *((unsigned char*)0xd418) = 0;

}

void mr_set_background_color_hd(unsigned char _color) {
    SET_BACKGROUND_COLOR(_color);
}

void mr_set_border_color_hd(unsigned char _color) {
    SET_BORDER_COLOR(_color);
}

unsigned char storedJiffy = 0;

void mr_start_frame_hd() {

    storedJiffy = *((unsigned char*)0xa2);

}

void mr_end_frame_hd(unsigned char _jiffies) {

    while ((*((unsigned char*)0xa2) - storedJiffy) < _jiffies) {
        ; // nop!
    }

}

#ifdef MIDRES_STANDALONE_FILE

unsigned char* mr_translate_file_hd(mr_file _file) {
    return mr_translate_file_user(_file);
}

void mr_read_file_hd(unsigned int _file, unsigned int _offset, unsigned char* _dest, unsigned int _size) {
    FILE* f = fopen(mr_translate_file_hd(_file), "rb");
    if (f == NULL) {
        return;
    }
    if (_offset > 0) {
        while (_offset != 0) {
            if (_offset > 192) {
                fread((unsigned char*)(0x033c), 192, 1, f);
                _offset -= 192;
            }
            else {
                fread((unsigned char*)(0x033c), _offset, 1, f);
                _offset = 0;
            }
        }
    }
    fread(_dest, _size, 1, f);
    fclose(f);

}

#endif

unsigned char mr_joy_hd(unsigned char _number) {

    unsigned char port;

    switch (_number) {
    case MR_JOYSTICK_0:
        port = (*(unsigned char*)0xdc01);
        break;
    case MR_JOYSTICK_1:
        port = (*(unsigned char*)0xdc00);
        break;
    }

    return ~port;
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