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

#ifdef __LM80C__

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "midres.h"
#include "rawdata.h"

#define PSG_R14         14
#define PSG_R15         15

  /****************************************************************************
   ** RESIDENT VARIABLES SECTION
   ****************************************************************************/

   /****************************************************************************
    ** RESIDENT FUNCTIONS SECTION
    ****************************************************************************/

    // The functions defined at this level can only be called up if the current
    // module has been loaded into memory. On the other hand, they can call any 
    // function declared at the resident module level.

void mr_init_hd() {

    mr_vdp_data_port(0x30);
    mr_vdp_control_port(0x32);

    mr_init_vdp_hd();

}

void mr_init_multicolor_hd() {

    mr_vdp_data_port(0x30);
    mr_vdp_control_port(0x32);

    mr_init_multicolor_vdp_hd();

}

void mr_tile_setcolor_hd(unsigned char _index, unsigned char _color) {

}

void mr_tile_setcolors_hd(unsigned char _colors[4]) {

}

void mr_show_hd(unsigned char _screen) {
    mr_vdp_out(VDP_RNAME, _screen);
#ifdef GRAPHIC_MODE_I
    mr_vdp_out(VDP_RCOLORTABLE, 0x80 + (_screen - MR_SCREEN_DEFAULT));
#else
    mr_vdp_out(VDP_RCOLORTABLE, (_screen == MR_SCREEN_0) ? 0xff : 0x7f);
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
    mr_vdp_out(VDP_RPATTERN, _tileset);
#endif
}

unsigned char mr_key_pressed_hd() {
    return mr_get_key_pressed_hd() != MR_KEY_NONE;
}

unsigned char mr_get_key_pressed_hd() {
    return *((unsigned char*)0x822c);
}

void mr_wait_hd(unsigned char _seconds) {
    sleep(_seconds);
}

void mr_wait_jiffies_hd(unsigned char _jiffies) {
    unsigned int actual = *((unsigned int*)0x81CE);
    while ((*((unsigned int*)0x81CE) - actual) < (unsigned int)(_jiffies*2)) {

    }
}

unsigned char* mr_translate_file_hd(mr_file _file) {

}

void mr_read_file_hd(unsigned int _file, unsigned int _offset, unsigned char* _dest, unsigned int _size) {
    memcpy(_dest, &_includedFiles[_file][_offset], _size);
}

unsigned char* mr_map_file_hd(mr_file _file, unsigned int _projected_size) {
    return &_includedFiles[_file][0];
}

unsigned char mr_joy_hd(unsigned char _number) {
    return 0;
}

int mr_get_jiffies_int_hd() {
    return (*((unsigned int*)0x81CE)>>1);
}

unsigned char mr_get_jiffies_hd() {
    return (*((unsigned int*)0x81CE)>>1);
}

unsigned char storedJiffy = 0;

void mr_start_frame_hd() {
    storedJiffy = (*((unsigned int*)0x81CE)) >> 1;
}

int mr_get_start_frame_int_hd() {
    return storedJiffy;
}

void mr_end_frame_hd(unsigned char _jiffies) {
    while ((((*((unsigned int*)0x81CE)>>1)) - storedJiffy) < (_jiffies)) {

    }
#ifdef FRAME_BUFFER
#ifdef GRAPHIC_MODE_I
    mr_vdp_put(&frameBuffer[0], MR_VISIBLE_SCREEN * 0x400, MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT);
    // mr_vdp_put(&colorBuffer[0], MR_VISIBLE_SCREEN == MR_SCREEN_0 ? 0x2000 : 0x0000, 32);
#else
    mr_vdp_put(&frameBuffer[0], MR_VISIBLE_SCREEN == MR_SCREEN_0 ? 0x3800 : 0x4000, MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT);
    mr_vdp_put8(&colorBuffer[0], MR_VISIBLE_SCREEN == MR_SCREEN_0 ? 0x2000 : 0x0000, MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT);
#endif
#endif
}

clock_t clock() {
    return *((unsigned int*)0x52BA);
}

#endif