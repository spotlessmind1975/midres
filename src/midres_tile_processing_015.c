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

// Roll horizontally a tile
void mr_tile_roll_horizontal_on_place_memory_mapped(mr_tileset _tileset, mr_tile _destination, mr_direction _direction, mr_tile _place, mr_position* _index) {

    mr_mixel* source;
    mr_mixel* destination;
    mr_position b;

    source = (mr_mixel*)(MR_TM(_tileset) + (_destination * 8) + (*_index) * 8);
    destination = (mr_mixel*)(MR_TM(_tileset) + _place * 8);
    if (_direction == mr_direction_right) {
        ++(*_index);
        if ((*_index) > 8) {
            *_index = 0;
        }
    }
    else {
        --(*_index);
        if ((*_index) == 255) {
            *_index = 7;
        }
    }
    for (b = 0; b < 8; ++b, ++source, ++destination) {
        mr_mixel d = *((mr_mixel*)source);
        *destination = d;
    }

}
#endif
