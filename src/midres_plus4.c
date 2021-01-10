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

#ifdef __PLUS4__

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>
#include <cc65.h>
#include <string.h>
#include <6502.h>

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

int mr_get_jiffies_int_hd() {
    return (*((unsigned char*)0xa4) << 8) + *((unsigned char*)0xa5);
}

unsigned char mr_get_jiffies_hd() {
    return *((unsigned char*)0xa5);
}

void mr_wait_jiffies_hd(unsigned char _jiffies) {

    unsigned int now = ((*((unsigned char*)0xa4) << 8) + *((unsigned char*)0xa5));

    while ((((*((unsigned char*)0xa4) << 8) + *((unsigned char*)0xa5)) - now) < _jiffies) {
        ; // nop!
    }

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
    availableMemoryAddress -= _projected_size;
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

    if (_number == MR_JOYSTICK_0) {
        (*(unsigned char*)0xfd30) = 0xff;
        (*(unsigned char*)0xff08) = 0x02;
        port = 0;
        port = (*(unsigned char*)0xff08);
        (*(unsigned char*)0xff08) = 0x02;
        port = 0;
        port = (*(unsigned char*)0xff08);
    } else {
        (*(unsigned char*)0xfd30) = 0xff;
        (*(unsigned char*)0xff08) = 0x04;
        port = 0;
        port = (*(unsigned char*)0xff08);
        (*(unsigned char*)0xff08) = 0x04;
        port = 0;
        port = (*(unsigned char*)0xff08);
    }

    return ~port;
}

#endif