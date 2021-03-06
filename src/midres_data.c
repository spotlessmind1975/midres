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

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include "midres.h"

#if ( !defined(__OVERLAY__MIDRES__) && defined(MIDRES_STANDALONE) ) || defined(__OVERLAY__MIDRES__)

  /****************************************************************************
   ** RESIDENT VARIABLES SECTION
   ****************************************************************************/

unsigned char MR_RENDERED_MIXEL_BITS[4] = {
    0x08,
    0x02,
    0x04,
    0x01
};

#if defined(__IMG2MIDRES__) || defined(__ATARI__)
unsigned char MR_RENDERED_MIXELS_ATARI[16] = {
   0x00, 0x01, 0x02, 0x03,
   0x04, 0x05, 0x06, 0x07,
   0x87, 0x86, 0x85, 0x84,
   0x83, 0x82, 0x81, 0x80
};
#endif

#if defined(__IMG2MIDRES__) || defined(__VANILLA__)
unsigned char MR_RENDERED_MIXELS_VANILLA[16] = {
   0x00, 0x01, 0x02, 0x03,
   0x04, 0x05, 0x06, 0x07,
   0x08, 0x09, 0x0a, 0x0b,
   0x0c, 0x0d, 0x0e, 0x0f
};
#endif

#if defined(__IMG2MIDRES__) || defined(__VDP__)
unsigned char MR_RENDERED_MIXELS_VDP[16] = {
   0x00, 0x01, 0x02, 0x03,
   0x04, 0x05, 0x06, 0x07,
   0x08, 0x09, 0x0a, 0x0b,
   0x0c, 0x0d, 0x0e, 0x0f
};
#endif

#if defined(__IMG2MIDRES__) || defined(__GAMEBOY__)
unsigned char MR_RENDERED_MIXELS_GB[16] = {
   0x00, 0x01, 0x02, 0x03,
   0x04, 0x05, 0x06, 0x07,
   0x08, 0x09, 0x0a, 0x0b,
   0x0c, 0x0d, 0x0e, 0x0f
};
#endif

#ifdef FRAME_BUFFER
unsigned char frameBuffer[MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT];
unsigned char colorBuffer[MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT];
#endif

/***
 * This value is used to check if the screen memory is corrupted.
 */

#define MIXEL_THRESHOLD	0xff

mr_color MR_SCREEN_INK[MR_SCREEN_COUNT];

mr_screen MR_VISIBLE_SCREEN = MR_SCREEN_DEFAULT;

mr_screen MR_ENABLED_SCREEN = MR_SCREEN_DEFAULT;

mr_mixel get_mixel_bits(mr_mixel _current) {

    register unsigned char i = 0;

    for (i = 0; i < sizeof(MR_RENDERED_MIXELS); ++i) {
        if (MR_RENDERED_MIXELS[i] == _current) {
            return i;
        }
    }

    return MIXEL_THRESHOLD;

}

mr_mixel calculate_mixel(mr_mixel _current, mr_mixelbits _abcd, mr_pop _pop) {

    mr_mixelbits currentMixelbits = MIXEL_THRESHOLD;
    int i = 0;

    currentMixelbits = get_mixel_bits(_current);

    if (currentMixelbits != MIXEL_THRESHOLD) {
        switch (_pop) {
        case mr_pixel_on:
            currentMixelbits = currentMixelbits | (1 << _abcd);
            break;
        case mr_pixel_off:
            currentMixelbits = currentMixelbits & ~(1 << _abcd);
            break;
        case mr_pixel_invert:
            currentMixelbits = currentMixelbits ^ (1 << _abcd);
            break;
        }
        return MR_RENDERED_MIXELS[currentMixelbits];
    }
    else {
        return _current;
    }


}


mr_position MR_WIDTH;

mr_position MR_HEIGHT;

mr_color MR_BRIGHTNESS;

mr_screen MR_DB1 = MR_SCREEN_DB1;

mr_screen MR_DB2 = MR_SCREEN_DB2;

mr_boolean MR_MULTICOLOR = mr_false;

#if defined(MIDRES_STANDALONE_TILE_PROCESSING)

#ifdef __GAMEBOY__
    mr_mixel rollBuffer[16];
#else
    mr_mixel rollBuffer[8];
#endif

#endif

#endif