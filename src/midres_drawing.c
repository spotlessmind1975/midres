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

#include "midres.h"

#if ( !defined(__OVERLAY__MIDRES__) && defined(MIDRES_STANDALONE_DRAWING) ) || defined(__OVERLAY__MIDRES__)

  // Overlay management is driven by the definition of the appropriate 
  // compilation symbol (__OVERLAY__). In this case, we enable or disable the 
  // compilation of the relevant code.

#ifdef __OVERLAY__MIDRES__

    // In general, the compiler places the executable code in the "CODE" 
    // segment, while the non-modifiable data in the "RODATA" segment.
    // In order to save space, both of these segments are moved to the 
    // overlay segment, so that both the code and the data it uses are 
    // removed from the resident module.
    #pragma code-name ("OVERLAY2");
    #pragma rodata-name ("OVERLAY2");

#endif

/****************************************************************************
 ** OVERLAYED FUNCTIONS SECTION
 ****************************************************************************/

 // The functions defined at this level can only be called up if the current
 // module has been loaded into memory. On the other hand, they can call any 
 // function declared at the resident module level.

void mr_psetop(mr_mixel* _screen, mr_position _x, mr_position _y, mr_pop _pop) {

    mr_position mx, my;
    mr_mixelbits abcd;

    // Find relative mixel coordinates.

    mx = _x >> 1;
    my = _y >> 1;
    abcd = ((1 - (_x & 1)) + (1 - (_y & 1)) * 2);

    // Draw on the correct place the new mixel combination

    MR_WRITE_TILE_LUMINANCE(
            _screen, 
            my * MR_WIDTH + mx, 
            calculate_mixel(MR_READ_TILE(_screen, my * MR_WIDTH + mx), abcd, _pop)
    );

}

void mr_pcolorop(mr_color* _screen, mr_position _x, mr_position _y, mr_color _color) {

    mr_position mx, my;

    // Find relative mixel coordinates.

    mx = _x >> 1;
    my = _y >> 1;

    MR_WRITE_TILE_COLOR(_screen, my * MR_WIDTH + mx, _color + MR_BRIGHTNESS);

}

void mr_ink(mr_screen _screen, mr_color _color) {

    MR_SCREEN_INK[_screen] = _color + MR_BRIGHTNESS;

}

// Clears the bitmap to color BLACK.
void _mr_clear_bitmap(mr_mixel* _screen, mr_color* _colormap) {

    int i;

    for (i = 0; i < MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT; ++i) {
        MR_WRITE_TILE(_screen, _colormap, i, MR_RENDERED_MIXELS[0], MR_COLOR_BLACK);
    }

}

#endif