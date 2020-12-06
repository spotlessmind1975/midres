#ifdef __ATARI__

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

#if defined(__IMG2MIDRES__) || defined(__ATARI__)

unsigned char MR_RENDERED_MIXELS_ATARI[16] = {
   0x00, 0x01, 0x02, 0x03,
   0x04, 0x05, 0x06, 0x07,
   0x87, 0x86, 0x85, 0x84,
   0x83, 0x82, 0x81, 0x80
};

#endif

mr_mixel* screenRam = NULL;

void mr_init_base_hd(unsigned char _mode) {

    unsigned char* dlist = (unsigned char*)(*((unsigned char*)(560)) + (*((unsigned char*)(561)) * 256));
    unsigned char i;

    *((unsigned char*)0xd301) = *((unsigned char*)0xd301) | 0x02;
    *(unsigned char*)(0x2E4) = 0xc0;
    *(unsigned char*)(0x6a) = 0xc0;

    dlist += 3;
    *(dlist) = _mode;

    ++dlist;
    screenRam = *(dlist)+((mr_mixel*)(*(dlist + 1) * 256));

    dlist += 2;

    i = MR_SCREEN_HEIGHT-1;

    for (; i != 0; --i, ++dlist) {
        *dlist = _mode - 64;
    }

    memset(MR_TM(MR_TILESET_0), 0, 0x400);

    mr_tileset_load("ztiles.bin", MR_TILESET_0, 0, 8);
    mr_tileset_visible(MR_TILESET_0);
    mr_show_hd(MR_SCREEN_0);

    (*(unsigned char*)0x02fc) = 0xff;

}

void mr_init_hd() {
#ifdef __LORES__
    mr_init_base_hd(70);
#else
    mr_init_base_hd(66);
#endif
}

void mr_init_multicolor_hd() {
    mr_init_base_hd(68);
}

void mr_tile_setcolor_hd(unsigned char _index, unsigned char _color) {
    switch (_index) {
        case MR_TILE_BACKGROUND:
            *((unsigned char*)0x2c8) = _color;
            break;
        case MR_TILE_COLOR0:
            *((unsigned char*)0x2c4) = _color;
            break;
        case MR_TILE_COLOR1:
            *((unsigned char*)0x2c5) = _color;
            break;
        case MR_TILE_COLOR2:
            *((unsigned char*)0x2c6) = _color;
            break;
    }
}

void mr_tile_setcolors_hd(unsigned char _colors[4]) {
    *((unsigned char*)0x2c8) = _colors[MR_TILE_BACKGROUND];
    *((unsigned char*)0x2c4) = _colors[MR_TILE_COLOR0];
    *((unsigned char*)0x2c5) = _colors[MR_TILE_COLOR1];
    *((unsigned char*)0x2c6) = _colors[MR_TILE_COLOR2];
}

void mr_show_hd(unsigned char _screen) {

    unsigned char* dlist = (unsigned char*)(*((unsigned char*)(560)) + (*((unsigned char*)(561)) * 256));

    MR_VISIBLE_SCREEN = _screen;
    MR_ENABLED_SCREEN = _screen;
    
    dlist += 4;

    /* *(dlist) = ( (int)(MR_SM(_screen)) & 0xff );
    ++dlist;
    *(dlist) = ( ( (int)(MR_SM(_screen)) >> 8 ) & 0xff );*/

}

void mr_cleanup_hd() {
    mr_show_hd(MR_SCREEN_DEFAULT);
}

void mr_wait_vbl() {

}

void mr_doublebuffer_switch_hd(unsigned char _screen) {
    unsigned char _other = (_screen == MR_DB1) ? MR_DB2 : MR_DB1;

    memcpy(MR_SM(_screen), MR_SM(_other), 0x400);
}

void mr_tileset_visible_hd(unsigned char _tileset) {
    *(unsigned char*)(0x2F4) = (unsigned char)((((int)(MR_TM(_tileset))) >> 8) & 0xff);
}

unsigned char mr_key_pressed_hd() {
    return (((*(unsigned char*)0xd20f)) & 0x04) != 0x04;
}

unsigned char mr_get_key_pressed_hd() {
    if (mr_key_pressed_hd()) {
        unsigned char k = (*(unsigned char*)0x02f2);
        return k;
    } else {
        return MR_KEY_NONE;
    }
}

void mr_wait_hd(unsigned char _seconds) {

    while (_seconds) {
        mr_wait_jiffies_hd(60);
        --_seconds;
    }

}

void mr_wait_jiffies_hd(unsigned char _jiffies) {

    unsigned char now = 0;
    
    *((unsigned char*)0x14) = 0;
    
    while ((*((unsigned char*)0x14) - now) < _jiffies) {
        ; // nop!
    }
}

// Hardware dependent sound library
void mr_sound_start_hd(unsigned char _channel, unsigned char _number) {
    *((unsigned char*)0xd200+(_channel & 0x03)) = 0xff-_number;
    *((unsigned char*)0xd201+(_channel & 0x03)) = 15;
}

// Hardware dependent sound library
void mr_sound_change_hd(unsigned char _channel, int _parameter) {
    *((unsigned char*)0xd200+(_channel & 0x03)) = 0xff-(_parameter>>8);
}

// Hardware dependent sound library
void mr_sound_stop_hd(unsigned char _channel) {
    *((unsigned char*)0xd201+(_channel & 0x03)) = 0;
}

void mr_set_background_color_hd(unsigned char _color) {
    *((unsigned char*)0x2c8) = _color;
}

void mr_set_border_color_hd(unsigned char _color) {
    *((unsigned char*)0x2c8) = _color;
}

unsigned char storedJiffy = 0;

void mr_start_frame_hd() {

    storedJiffy = *((unsigned char*)0x14);

}

void mr_end_frame_hd(unsigned char _jiffies) {

    while ((*((unsigned char*)0x14) - storedJiffy) < _jiffies) {
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
    fseek(f, _offset, SEEK_SET);
    fread(_dest, _size, 1, f);
    fclose(f);
}

#endif

unsigned char mr_joy_hd(unsigned char _number) {

    unsigned char port;

    switch (_number) {
        case MR_JOYSTICK_0:
            port = (*((unsigned char*)0xd300));
            port = port & 0x0f;
            port |= (((*((unsigned char*)0xd010)) & 0x01) * MR_JOYSTICK_FIRE);
            break;
        case MR_JOYSTICK_1:
            port = (*((unsigned char*)0xd300));
            port = ( port & 0xf0 ) >> 4;
            port |= (((*((unsigned char*)0xd011)) & 0x01) * MR_JOYSTICK_FIRE);
            break;
        case MR_JOYSTICK_2:
            port = (*((unsigned char*)0xd301));
            port = port & 0x0f;
            port |= (((*((unsigned char*)0xd012)) & 0x01) * MR_JOYSTICK_FIRE);
            break;
        case MR_JOYSTICK_3:
            port = (*((unsigned char*)0xd300));
            port = (port & 0xf0) >> 4;
            port |= (((*((unsigned char*)0xd013)) & 0x01) * MR_JOYSTICK_FIRE);
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

void mr_tile_prepare_horizontal_monocolor_hd(unsigned char _tileset, unsigned char _source, unsigned char _destination) {
    mr_tile_prepare_horizontal_monocolor_memory_mapped(_tileset, _source, _destination);
}

void mr_tile_prepare_horizontal_extended_monocolor_hd(unsigned char _tileset, unsigned char _source, unsigned char _w, unsigned char _h, unsigned char _destination) {
    mr_tile_prepare_horizontal_extended_monocolor_memory_mapped(_tileset, _source, _w, _h, _destination);
}

void mr_tile_prepare_vertical_hd(unsigned char _tileset, unsigned char _source, unsigned char _destination) {
    void mr_tile_prepare_vertical_memory_mapped(_tileset, _source, _destination);
}

void mr_tile_prepare_vertical_extended_hd(unsigned char _tileset, unsigned char _source, unsigned char _w, unsigned char _h, unsigned char _destination) {
    mr_tile_prepare_vertical_extended_memory_mapped(_tileset, _source, _w, _h, _destination)
}

void mr_tile_prepare_roll_horizontal_hd(unsigned char _tileset, unsigned char _source, unsigned char _destination) {
    mr_tile_prepare_roll_horizontal_monocolor_memory_mapped(_tileset, _source, _destination);
}

void mr_tile_roll_horizontal_hd(unsigned char _tileset, unsigned char _destination, unsigned char _direction) {
    mr_tile_roll_horizontal_memory_mapped(_tileset, _destination, _direction);
}

void mr_tile_roll_horizontal_on_place_hd(unsigned char _tileset, unsigned char _destination, unsigned char _direction, unsigned char _place, unsigned char* _index) {
    mr_tile_roll_horizontal_on_place_memory_mapped(_tileset, _destination, _direction, _place, *_index);
}

void mr_tile_prepare_roll_vertical_hd(unsigned char _tileset, unsigned char _source, unsigned char _destination) {
    mr_tile_prepare_roll_vertical_memory_mapped(_tileset, _source, _destination);
}

void mr_tile_roll_vertical_hd(unsigned char _tileset, unsigned char _destination, unsigned char _direction) {
    mr_tile_roll_vertical_memory_mapped(_tileset, _destination, _direction);
}

#endif