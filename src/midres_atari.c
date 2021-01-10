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

#if defined(__ATARI__)

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>
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

int mr_get_jiffies_int_hd() {
    return (*((unsigned char*)0x13) << 8) + *((unsigned char*)0x14);
}

unsigned char mr_get_jiffies_hd() {
    return *((unsigned char*)0x14);
}

void mr_wait_hd(unsigned char _seconds) {

    mr_wait_jiffies_hd(60*_seconds);

}

void mr_wait_jiffies_hd(unsigned char _jiffies) {

    unsigned char now = (*((unsigned char*)0x13) << 8) + *((unsigned char*)0x14);
    
    while ((((*((unsigned char*)0x13) << 8) + *((unsigned char*)0x14)) - now) < _jiffies) {
        ; // nop!
    }
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

    if (_jiffies > 0) {
        while ((*((unsigned char*)0x14) - storedJiffy) < _jiffies) {
            ; // nop!
        }
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
            if (_offset > 30) {
                fread((unsigned char*)(0x0461), 30, 1, f);
                _offset -= 30;
            }
            else {
                fread((unsigned char*)(0x0461), _offset, 1, f);
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

#endif