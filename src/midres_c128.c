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

#ifdef __C128__

/****************************************************************************
 ** INCLUDE SECTION
 ****************************************************************************/

#include <stdio.h>
#include <cc65.h>
#include <string.h>
#include <stdlib.h>

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

    unsigned int now = (*((unsigned char*)0xa1) << 8) + *((unsigned char*)0xa2);

    while ((((*((unsigned char*)0xa1) << 8) + *((unsigned char*)0xa2)) - now) < (_seconds * 60)) {
        ; // nop!
    }
}

void mr_wait_jiffies_hd(unsigned char _jiffies) {
    unsigned int now = (*((unsigned char*)0xa1) << 8) + *((unsigned char*)0xa2);
    while ((((*((unsigned char*)0xa1) << 8) + *((unsigned char*)0xa2)) - now) < _jiffies) {
        ; // nop!
    }
}

int mr_get_jiffies_int_hd() {
    return (*((unsigned char*)0xa1) << 8) + *((unsigned char*)0xa2);
}

unsigned char mr_get_jiffies_hd() {
    return *((unsigned char*)0xa2);
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

int mr_get_start_frame_int_hd() {
    return storedJiffy;
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

unsigned char* availableMemoryAddress = MR_AM(MR_AUX_DEFAULT);
unsigned int availableMemorySize = MR_AUX_COUNT * MR_SCREEN_RAM_SIZE;

unsigned char* mr_map_file_hd(mr_file _file, unsigned int _projected_size) {
    FILE* f = NULL;
    unsigned char* storage = NULL;
    if (availableMemorySize < _projected_size) {
        return NULL;
    }
    f = fopen(mr_translate_file_hd(_file), "rb");
    if (f == NULL) {
        return NULL;
    }
    storage = availableMemoryAddress;
    availableMemoryAddress += _projected_size;
    availableMemorySize -= _projected_size;
    fread(storage, _projected_size, 1, f);
    fclose(f);
    return storage;
}


#else

#include "rawdata.h"

unsigned char* mr_translate_file_hd(mr_file _file) {

}

void mr_read_file_hd(unsigned int _file, unsigned int _offset, unsigned char* _dest, unsigned int _size) {
    memcpy(_dest, &_includedFiles[_file][_offset], _size);
}

unsigned char* mr_map_file_hd(mr_file _file, unsigned int _projected_size) {
    return &_includedFiles[_file][0];
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

#endif