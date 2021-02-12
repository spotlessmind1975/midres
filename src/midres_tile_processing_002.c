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

void mr_tile_prepare_horizontal_multicolor(mr_tileset _tileset, mr_tile _source, mr_tile _destination);
void mr_tile_prepare_horizontal_monocolor(mr_tileset _tileset, mr_tile _source, mr_tile _destination);

void mr_tile_prepare_horizontal(mr_tileset _tileset, mr_tile _source, mr_tile _destination) {

#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    if (MR_MULTICOLOR) {
        mr_tile_prepare_horizontal_multicolor(_tileset, _source, _destination);
    }
    else {
#endif
        mr_tile_prepare_horizontal_monocolor(_tileset, _source, _destination);
#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR
    }
#endif

}

#endif
