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

#define PSG_AP          0x40
#define PSG_WP          0x41
#define PSG_RP          0x41

#define PSG_R0           0
#define PSG_R1           1
#define PSG_R2           2
#define PSG_R3           3
#define PSG_R4           4
#define PSG_R5           5
#define PSG_R6           6
#define PSG_R7           7
#define PSG_R8           8
#define PSG_R9           9
#define PSG_R10         10
#define PSG_R11         11
#define PSG_R12         12
#define PSG_R13         13
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

// Hardware dependent sound library
void mr_sound_start_hd(unsigned char _channel, unsigned char _number) {
    unsigned char value;
    switch ((_channel & 0x3)) {
    case 0:
    case 3:
        io_put(PSG_AP, PSG_R7);
        value = io_get(PSG_RP);
        io_put(PSG_AP, PSG_R7);
        io_put(PSG_WP, value & 0xfe);
        break;
    case 1:
        io_put(PSG_AP, PSG_R7);
        value = io_get(PSG_RP);
        io_put(PSG_AP, PSG_R7);
        io_put(PSG_WP, value & 0xfd);
        break;
    case 2:
        io_put(PSG_AP, PSG_R7);
        value = io_get(PSG_RP);
        io_put(PSG_AP, PSG_R7);
        io_put(PSG_WP, value & 0xfb);
        break;
    }
}

// Hardware dependent sound library
void mr_sound_change_hd(unsigned char _channel, int _parameter) {
    int f = 111861 / _parameter;
    switch ((_channel & 0x3)) {
    case 0:
    case 3:
        io_put(PSG_AP, PSG_R0);
        io_put(PSG_WP, f & 0xff);
        io_put(PSG_AP, PSG_R1);
        io_put(PSG_WP, (f >> 8) & 0x0f);
        io_put(PSG_AP, PSG_R8);
        io_put(PSG_WP, 0x0f);
        break;
    case 1:
        io_put(PSG_AP, PSG_R2);
        io_put(PSG_WP, f & 0xff);
        io_put(PSG_AP, PSG_R3);
        io_put(PSG_WP, (f >> 8) & 0x0f);
        io_put(PSG_AP, PSG_R9);
        io_put(PSG_WP, 0x0f);
        break;
    case 2:
        io_put(PSG_AP, PSG_R4);
        io_put(PSG_WP, f & 0xff);
        io_put(PSG_AP, PSG_R5);
        io_put(PSG_WP, (f >> 8) & 0x0f);
        io_put(PSG_AP, PSG_R10);
        io_put(PSG_WP, 0x0f);
        break;
    }
}

// Hardware dependent sound library
void mr_sound_stop_hd(unsigned char _channel) {
    unsigned char value;
    switch ((_channel & 0x3)) {
    case 0:
    case 3:
        io_put(PSG_AP, PSG_R7);
        value = io_get(PSG_RP);
        io_put(PSG_AP, PSG_R7);
        io_put(PSG_WP, (value & 0xfe) | 0x01);
        io_put(PSG_AP, PSG_R8);
        io_put(PSG_WP, 0x00);
        break;
    case 1:
        io_put(PSG_AP, PSG_R7);
        value = io_get(PSG_RP);
        io_put(PSG_AP, PSG_R7);
        io_put(PSG_WP, (value & 0xfd) | 0x02);
        io_put(PSG_AP, PSG_R9);
        io_put(PSG_WP, 0x00);
        break;
    case 2:
        io_put(PSG_AP, PSG_R7);
        value = io_get(PSG_RP);
        io_put(PSG_AP, PSG_R7);
        io_put(PSG_WP, (value & 0xfb) | 0x04);
        io_put(PSG_AP, PSG_R10);
        io_put(PSG_WP, 0x00);
        break;
    }
}

unsigned char* mr_translate_file_hd(mr_file _file) {

}

void mr_read_file_hd(unsigned int _file, unsigned int _offset, unsigned char* _dest, unsigned int _size) {
    memcpy(_dest, &_includedFiles[_file][_offset], _size);
}

unsigned char mr_joy_hd(unsigned char _number) {
    return 0;
}

unsigned char storedJiffy = 0;

void mr_start_frame_hd() {
    storedJiffy = (*((unsigned int*)0x81CE));
}

void mr_end_frame_hd(unsigned char _jiffies) {
    while (((*((unsigned int*)0x81CE)) - storedJiffy) < (_jiffies*2)) {

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