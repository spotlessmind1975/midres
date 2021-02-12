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

// Redefine a subset of N tiles by "shifting" horizontally a tile
void mr_tile_prepare_horizontal_extended_monocolor_memory_mapped(mr_tileset _tileset, mr_tile _source, mr_tile _w, mr_tile _h, mr_tile _destination) {
    mr_mixel* source = (mr_mixel*)(MR_TM(_tileset) + _source * 8);
    mr_mixel* destination = (mr_mixel*)(MR_TM(_tileset) + _destination * 8);

    mr_position i, b;

    mr_position j, k;

    for (j = 0; j < _h; ++j) {
        for (i = 0; i < 9; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel e = *((mr_mixel*)source);
                mr_mixel m = e >> i;
                *destination = m;
            }
            source -= 8;
        }

        for (k = 0; k < (_w - 1); ++k) {
            for (i = 0; i < 9; ++i) {
                for (b = 0; b < 8; ++b, ++source, ++destination) {
                    mr_mixel d = *((mr_mixel*)source);
                    mr_mixel e = *((mr_mixel*)source + 8);
                    mr_mixel m = (e >> i) | (d << (8 - i));
                    *destination = m;
                }
                source -= 8;
            }
            source += 8;
        }

        for (i = 0; i < 9; ++i) {
            for (b = 0; b < 8; ++b, ++source, ++destination) {
                mr_mixel d = *((mr_mixel*)source);
                mr_mixel n = d & (0xff >> (8 - i));
                *destination = (n << (8 - i));
            }
            source -= 8;
        }
        source += 8;
    }

}
#endif
