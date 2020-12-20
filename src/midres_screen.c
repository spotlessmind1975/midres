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

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>
#include <string.h>

#include "midres.h"

#ifdef __CBM__
    #include <cc65.h>
    #include <cbm.h>
    #include <device.h>
#else

#endif

#if defined(MIDRES_STANDALONE_SCREEN)

void mr_clear(mr_screen _screen) {

    int i;
    mr_mixel* screen = MR_SM(_screen);
    mr_color* color = MR_CM(_screen);

    for (i = 0; i < MR_SCREEN_HEIGHT * MR_SCREEN_WIDTH; ++i) {
        MR_WRITE_TILE(screen, color, i, MR_RENDERED_MIXELS[0], MR_COLOR_BLACK)
    }
}

void mr_show(mr_screen _screen) {

    mr_show_hd(_screen);

    MR_VISIBLE_SCREEN = _screen;
    MR_ENABLED_SCREEN = _screen;

}

void mr_enable(mr_screen _screen) {

    MR_ENABLED_SCREEN = _screen;

}

// Setup the double buffer (using screens SCREEN_DB1 and SCREEN_DB2)
void mr_doublebuffer_init() {
    MR_VISIBLE_SCREEN = MR_DB1;
    MR_ENABLED_SCREEN = MR_DB2;
}

// Switch double buffering
void mr_doublebuffer_switch() {
    
    mr_wait_vbl();

    if (MR_VISIBLE_SCREEN == MR_DB1) {
        mr_show(MR_DB2);
        mr_doublebuffer_switch_hd(MR_DB1);
        mr_enable(MR_DB1);
    }
    else {
        mr_show(MR_DB1);
        mr_doublebuffer_switch_hd(MR_DB2);
        mr_enable(MR_DB2);
    }
}

unsigned char mr_load(char* _filename, mr_screen _screen) {

#ifdef MIDRES_STANDALONE_FILE

#if __ATARI__

    FILE* f = fopen(_filename, "rb");
    if (f == NULL) {
        return 0;
    }
    fread(MR_SM(_screen), MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT, 1, f);
    fclose(f);
    return 1;

#elif __CBM__

    return cbm_load(_filename, getcurrentdevice(), MR_SM(_screen));

#endif

#endif

}

unsigned char mr_load_screen(mr_file _index, mr_screen _screen) {
    mr_read_file(_index, MR_SM(_screen), MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT);
    return 0;
}

unsigned char mr_load_color(char* _filename, mr_screen _screen) {

#ifdef MIDRES_STANDALONE_FILE

#if __ATARI__

    FILE* f = fopen(_filename, "rb");
    if (f == NULL) {
        return 0;
    }
    fread(MR_CM(_screen), MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT, 1, f);
    fclose(f);
    return 1;

#elif __CBM__

    _screen = 0;
    return cbm_load(_filename, getcurrentdevice(), MR_CM(_screen));

#endif

#endif

}

#ifdef MIDRES_STANDALONE_FILE

unsigned char mr_load_screen_color(mr_file _index, mr_screen _screen) {
    mr_read_file(_index, MR_CM(_screen), MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT);
    _screen=0;
    return 0;
}

#endif

void mr_uncompress(mr_screen _source, mr_screen _destination) {

    mr_mixel* source = MR_SM(_source);
    mr_mixel* destination = MR_SM(_destination);
    mr_color* destinationColor = MR_CM(_destination);

    int i;
    for (i = 0; i < MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT; ++i) {
        destination[i] = MR_RENDERED_MIXELS[mr_unpack_pmixel_lo(source[i])];
        destinationColor[i] = mr_unpack_pmixel_hi(source[i]) + MR_BRIGHTNESS;
    }

}

void mr_unpack(mr_screen _source, mr_screen _destination, mr_half_screen _half_screen) {

    int i;
    mr_mixel* source = MR_SM(_source);
    mr_mixel* destination = MR_SM(_destination);

    if (_half_screen == mr_half_up) {
        source += (MR_SCREEN_RAM_SIZE >> 1);
    }

    for (i = 0; i < MR_SCREEN_WIDTH * ( MR_SCREEN_HEIGHT >> 1 ); ++i) {
        destination[(i << 1)] = MR_RENDERED_MIXELS[mr_unpack_pmixel_lo(source[i])];
        destination[(i << 1) + 1] = MR_RENDERED_MIXELS[mr_unpack_pmixel_hi(source[i])];
    }

}

void mr_set_background_color(mr_color _color) {
    mr_set_background_color_hd(_color);
}

void mr_set_border_color(mr_color _color) {
    mr_set_border_color_hd(_color);
}

#endif