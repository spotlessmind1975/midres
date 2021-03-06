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

#ifdef __VIC20__

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

/*
    The following table shows how the bottom four bits of memory
    location $9005 specify the character table address.

        X       Location        TILESET_xx
        Value  HEX     Decimal

        0    $8000   32768      32  Upper case normal characters
        1    $8400   33792      33  Upper case reversed characters
        2    $8800   34816      34  Lower case normal character
        3    $8C00   35840      35  Lower case reversed characters
        4    $9000   36864      36  unavailable
        5    $9400   37888      37  unavailable
        6    $9800   38912      38  unavailable
        7    $9C00   39936      39  unavailable
        8    $0000       0       0  unavailable
        9    $0400    1024       1  3K expansion
        10    $0800    2048       2  3K expansion
        11    $0C00    3072       3  3K expansion
        12    $1000    4096       4  RAM
        13    $1400    5120       5  RAM
        14    $1800    6144       6  RAM
        15    $1C00    7168       7  RAM
*/
#define SET_CHARSET( _tileset ) \
    *((unsigned char*)0x9005) = (*((unsigned char*)0x9005) & 0xf0) | \
        ( \
            ( (_tileset) < 8 ) ?  \
              ( (_tileset) + 8 ) : \
              ( (_tileset) - 32 ) \
        ) & 0x0f;

/*
    The following table shows how the top four bits of location $9005
    and the top bit of location $9002 are used to give the address of
    screen memory. The top bit of location $9005 will almost always
    be 1, so for this reason I have ignored this bit in the table below.

        Y1 = ($9005) AND 112
        Y2 = ($9002) AND 128
        Address = 4*Y2 + 64*Y1

        Y1       Y2        Address      SCREEN_xx

        128       0             0          0
        128     128          $200          1
        144       0          $400          2
        144     128          $600          3
        160       0          $800          4
        160     128          $A00          5
        176       0          $C00          6
        176     128          $E00          7
        192       0         $1000          8
        192     128         $1200          9
        208       0         $1400         10
        208     128         $1600         11
        224       0         $1800         12
        224     128         $1A00         13
        240       0         $1C00         14
        240     128         $1E00         15

*/
#define SET_VIDEO( _screen ) \
        *((unsigned char*)0x9002) = (*((unsigned char*)0x9002) & 0x7f) | (((_screen) & 1) << 7); \
        *((unsigned char*)0x9005) = (*((unsigned char*)0x9005) & 0x8f) | ((((_screen) >> 1) & 0x7) << 4);

#define SET_BACKGROUND_COLOR( _color ) \
        *((unsigned char*)0x900f) = (*((unsigned char*)0x900f) & 0x0f) | ( _color << 4 );

#define SET_BORDER_COLOR( _color ) \
        *((unsigned char*)0x900f) = (*((unsigned char*)0x900f) & 0xf0) | 0x08 | (_color & 0x08 ) ;

#define WAIT_VBL( ) \
    while (*((unsigned char*)0x9004) < 27) { };

#define SET_MULTICOLOR_MODE( ) \
    *((unsigned char*)0xd016) = (*((unsigned char*)0xd016) | 0x10);

#define SET_MONOCOLOR_MODE( ) \
    *((unsigned char*)0xd016) = (*((unsigned char*)0xd016) & ~0x10);

void mr_init_base_hd() {

    int i = 0;
        
    SET_CHARSET(MR_TILESET_DEFAULT);
    SET_VIDEO(MR_SCREEN_DEFAULT);
    SET_BACKGROUND_COLOR(MR_COLOR_BLACK);

    MR_VISIBLE_SCREEN = MR_SCREEN_DEFAULT;
    MR_ENABLED_SCREEN = MR_SCREEN_DEFAULT;

    for (i = 0; i < MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT; ++i) {
        putchar(' ');
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

}

void mr_cleanup_hd() {

    SET_CHARSET(MR_TILESET_DEFAULT);
    SET_VIDEO(MR_SCREEN_DEFAULT);
    SET_BACKGROUND_COLOR(MR_COLOR_WHITE);

}

void mr_wait_vbl() {

    WAIT_VBL();

}

void mr_doublebuffer_switch_hd(unsigned char _screen) {
    unsigned char _other = (_screen == MR_DB1) ? MR_DB2 : MR_DB1;

    memcpy(MR_SM(_screen), MR_SM(_other), MR_SCREEN_RAM_SIZE);

}

void mr_tileset_visible_hd(unsigned char _tileset) {
    SET_CHARSET(_tileset);
}

unsigned char mr_key_pressed_hd() {
    return (*(unsigned char*)0x00c5) != 0x40;
}

unsigned char mr_get_key_pressed_hd() {
    return (*(unsigned char*)0x00c5);
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

unsigned int storedJiffy = 0;

void mr_start_frame_hd() {

    storedJiffy = (*((unsigned char*)0xa1) << 8) + *((unsigned char*)0xa2);

}

int mr_get_start_frame_int_hd() {
    return storedJiffy;
}

void mr_end_frame_hd(unsigned char _jiffies) {

    while ((((*((unsigned char*)0xa1) << 8) + *((unsigned char*)0xa2)) - storedJiffy) < _jiffies) {
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

unsigned char* availableMemoryAddress = 0x7000;
unsigned int availableMemorySize = 0x1000;

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

    unsigned char port = 0;

    switch (_number) {
        case MR_JOYSTICK_0:
            port = (*((unsigned char*)0x911f));
            (*((unsigned char*)0x9122)) = 0x7f;
            port |= ( (*((unsigned char*)0x9120)) & 0x80 );
            (*((unsigned char*)0x9122)) = 0xff;
            break;
    }

    return ~port;
}

#endif