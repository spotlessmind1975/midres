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

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>

#include "midres.h"

#if defined(MIDRES_STANDALONE_TILE)

// Load a tileset (or part of it)
void mr_tileset_load(unsigned char* _filename, mr_tileset _tileset, mr_tile _starting, mr_tile _count) {

#ifdef MIDRES_STANDALONE_FILE

#if defined(__ATARI__) || defined(__CBM__) 

    FILE* f = fopen(_filename, "rb");
    if (f == NULL) {
        return;
    }
    fread(MR_TM(_tileset) + 8 * _starting, _count * 8, 1, f);
    fclose(f);

#ifndef MIDRES_STANDALONE_TILE_MULTICOLOR 
    if (MR_MULTICOLOR) {
        mr_tileset_multicolor_to_monocolor(_tileset, _starting, _count);
    }
#endif

#endif

#endif

}
#endif
