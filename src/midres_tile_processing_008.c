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

#if defined(MIDRES_STANDALONE_TILE_PROCESSING)

// Redefine a subset of N tiles by "shifting" vertically a tile
void mr_tile_prepare_vertical_extended_memory_mapped(mr_tileset _tileset, mr_tile _source, mr_position _w, mr_position _h, mr_tile _destination) {
    mr_tile* source = (mr_tile*)(MR_TM(_tileset) + _source * 8);
    mr_tile* destination = (mr_tile*)(MR_TM(_tileset) + _destination * 8);

    int i, b;
    int w = _w, k = 0;

    for (; w != 0; --w) {
        for (i = 0; i < 9; ++i) {
            for (b = 0; b < i; ++b, ++destination) {
                *destination = 0x00;
            }
            for (b = 0; b < (8 - i); ++b, ++source, ++destination) {
                *destination = *(source);
            }
            source -= b;
        }

        for (k = 0; k < (_h - 1); ++k) {
            for (i = 0; i < 9; ++i) {
                for (b = 0; b < i; ++b, ++destination, ++source) {
                    *destination = *(source + (k * _w * 8) + (8 - i));
                }
                source -= b;
                for (b = 0; b < (8 - i); ++b, ++source, ++destination) {
                    *destination = *(source + ((k + 1) * _w * 8));
                }
                source -= b;
            }
        }

        for (i = 0; i < 9; ++i) {
            for (b = 0; b < i; ++b, ++destination, ++source) {
                *destination = *(source + ((_h - 1) * _w * 8) + (8 - i));
            }
            source -= b;
            for (b = 0; b < (8 - i); ++b, ++destination) {
                *destination = 0;
            }
        }
        source += 8;
    }
}

#endif
