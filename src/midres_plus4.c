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

#ifdef __PLUS4__

#define SET_VIDEO( _screen ) \
        *((unsigned char*)0xff14) = (*((unsigned char*)0xff14) & 0x07 ) | ( (_screen) << 3 );

#define SET_BACKGROUND_COLOR( _color ) \
        *((unsigned char*)0xff15) = _color;

#define SET_BORDER_COLOR( _color ) \
        *((unsigned char*)0xff19) = _color;

#define WAIT_VBL( ) \
    while ((*((unsigned char*)0xff1c) & 1 ) == 0) {}

#define SET_CHARSET( _tileset ) \
    *((unsigned char*)0xff12) = 0; \
    *((unsigned char *)0xff13) = ( (_tileset) << 2 ); \
    *((unsigned char *)0xff07) = (*((unsigned char*)0xff07) | 0x80);

#define SET_MULTICOLOR_MODE( ) \
    *((unsigned char*)0xff07) = (*((unsigned char*)0xff07) | 0x10);

#define SET_MONOCOLOR_MODE( ) \
    *((unsigned char*)0xff07) = (*((unsigned char*)0xff07) & ~0x10);

void mr_init_base_hd() {

    SET_BACKGROUND_COLOR(MR_COLOR_BLACK);
    SET_VIDEO(MR_SCREEN_DEFAULT);

    // SET_CHARSET(MR_TILESET_DEFAULT);

    MR_VISIBLE_SCREEN = MR_SCREEN_DEFAULT;
    MR_ENABLED_SCREEN = MR_SCREEN_DEFAULT;

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
        mr_set_background_color_hd(_color);
        break;
    case MR_TILE_COLOR0:
        *((unsigned char*)0xff16) = _color;
        break;
    case MR_TILE_COLOR1:
        *((unsigned char*)0xff17) = _color;
        break;
    case MR_TILE_COLOR2:
        *((unsigned char*)0xff18) = _color;
        break;
    }
}

void mr_tile_setcolors_hd(unsigned char _colors[4]) {
    // *((unsigned char*)0x0000) = _colors[MR_TILE_BACKGROUND];
    *((unsigned char*)0xff16) = _colors[MR_TILE_COLOR0];
    *((unsigned char*)0xff17) = _colors[MR_TILE_COLOR1];
    *((unsigned char*)0xff18) = _colors[MR_TILE_COLOR2];
}

void mr_show_hd(unsigned char _screen) {

    SET_VIDEO(_screen);

    MR_VISIBLE_SCREEN = _screen;
    MR_ENABLED_SCREEN = _screen;

}

void mr_cleanup_hd() {

    SET_BACKGROUND_COLOR(MR_COLOR_WHITE);
    SET_VIDEO(MR_SCREEN_DEFAULT);

}

void mr_wait_vbl() {
    WAIT_VBL();
}

void mr_doublebuffer_switch_hd(unsigned char _screen) {
    unsigned char _other = (_screen == MR_DB1) ? MR_DB2 : MR_DB1;

    memcpy(MR_SM(_screen), MR_SM(_other), MR_SCREEN_RAM_SIZE >> 1);
    memcpy(MR_CM(_screen), MR_CM(_other), MR_SCREEN_RAM_SIZE >> 1);
}

void mr_tileset_visible_hd(unsigned char _tileset) {
    SET_CHARSET(_tileset);
}

unsigned char mr_key_pressed_hd() {
    return (*(unsigned char*)0xc6) != 0x40;
}

unsigned char mr_get_key_pressed_hd() {
    return (*(unsigned char*)0xc6);
}

void mr_wait_hd(unsigned char _seconds) {

    unsigned char now = *((unsigned char*)0xa4);

    while ((*((unsigned char*)0xa4) - now) < _seconds) {
        ; // nop!
    }

}

void mr_wait_jiffies_hd(unsigned char _jiffies) {

    unsigned char now = *((unsigned char*)0xa5);

    while ((*((unsigned char*)0xa5) - now) < _jiffies) {
        ; // nop!
    }

}

// Hardware dependent sound library
void mr_sound_start_hd(unsigned char _channel, unsigned char _number) {

    //@todo: support _number under plus4 for mr_sound_start_hd
    _number = 0;

    switch ((_channel & 0x01)) {
        case 0:
            *((unsigned char*)0xff0e) = 769 & 0xff;
            *((unsigned char*)0xff12) = ((int)((unsigned char*)0xff12) & ~0x03) | (((769 & 0x300) >> 8) & 0x03);
            break;
        case 1:
            *((unsigned char*)0xff0f) = 769 & 0xff;
            *((unsigned char*)0xff10) = ((int)((unsigned char*)0xff10) & ~0x03) | (((769 & 0x300) >> 8) & 0x03);
    }

    *((unsigned char*)0xff11) = 0x14;

}

// Hardware dependent sound library
void mr_sound_change_hd(unsigned char _channel, int _parameter) {

    switch ((_channel & 0x01)) {
        case 0:
            *((unsigned char*)0xff0e) = _parameter & 0xff;
            *((unsigned char*)0xff12) = ((int)((unsigned char*)0xff12) & ~0x03) | (((_parameter & 0x300) >> 8) & 0x03);
            break;
        case 1:
            *((unsigned char*)0xff0f) = _parameter & 0xff;
            *((unsigned char*)0xff10) = ((int)((unsigned char*)0xff10) & ~0x03) | (((_parameter & 0x300) >> 8) & 0x03);
    }

}

// Hardware dependent sound library
void mr_sound_stop_hd(unsigned char _channel) {
    *((unsigned char*)0xff11) = *((unsigned char*)0xff11) && ~(1 << (4 + (_channel & 0x01)));
}

void mr_set_background_color_hd(unsigned char _color) {
    SET_BACKGROUND_COLOR(_color);
}

void mr_set_border_color_hd(unsigned char _color) {
    SET_BORDER_COLOR(_color);
}

unsigned char storedJiffy = 0;

void mr_start_frame_hd() {

    storedJiffy = *((unsigned char*)0xa5);

}

void mr_end_frame_hd(unsigned char _jiffies) {

    while ((*((unsigned char*)0xa5) - storedJiffy) < _jiffies) {
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

#endif