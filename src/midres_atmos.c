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

#ifdef __ATMOS__

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>
#include <string.h>

#include "rawdata.h"
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

#if defined(__IMG2MIDRES__) || defined(__ATMOS__)

unsigned char MR_RENDERED_MIXELS_ATMOS[16] = {
   0x20, 0x01, 0x02, 0x03,
   0x04, 0x05, 0x06, 0x07,
   0x08, 0x09, 0x10, 0x11,
   0x12, 0x13, 0x14, 0x15
};

#endif

void mr_init_hd() {

}

void mr_init_multicolor_hd() {

}

void mr_tile_setcolor_hd(unsigned char _index, unsigned char _color) {

}

void mr_tile_setcolors_hd(unsigned char _colors[4]) {

}

void mr_show_hd(unsigned char _screen) {

}

void mr_cleanup_hd() {

}

void mr_wait_vbl() {

}

void mr_doublebuffer_switch_hd(unsigned char _screen) {

}

void mr_tileset_visible_hd(unsigned char _tileset) {

}

unsigned char mr_key_pressed_hd() {

}

unsigned char mr_get_key_pressed_hd() {

}

void mr_wait_hd(unsigned char _seconds) {

}

void mr_wait_jiffies_hd(unsigned char _jiffies) {

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


unsigned char storedJiffy = 0;

void mr_start_frame_hd() {

}

int mr_get_start_frame_int_hd() {
    return storedJiffy;
}

void mr_end_frame_hd(unsigned char _jiffies) {

}

unsigned char* mr_translate_file_hd(mr_file _file) {
    return NULL;
}

void mr_read_file_hd(unsigned int _file, unsigned int _offset, unsigned char* _dest, unsigned int _size) {
    memcpy(_dest, (&_includedFiles[_file][0])+_offset, _size);
}

unsigned char* mr_map_file_hd(mr_file _file, unsigned int _projected_size) {
    return &_includedFiles[_file][0];
}

unsigned char mr_joy_hd(unsigned char _number) {
    return 0;
}

#endif