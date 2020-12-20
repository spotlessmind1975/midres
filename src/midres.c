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
 
void mr_init() {

    MR_MULTICOLOR = mr_false;

    MR_WIDTH = MR_SCREEN_WIDTH;
    MR_HEIGHT = MR_SCREEN_HEIGHT;
    MR_BRIGHTNESS = MR_COLOR_BRIGHTNESS;

    mr_init_hd();

}

void mr_init_multicolor() {

#ifdef MIDRES_STANDALONE_TILE_MULTICOLOR 

    MR_MULTICOLOR = mr_true;

    MR_WIDTH = MR_SCREEN_WIDTH;
    MR_HEIGHT = MR_SCREEN_HEIGHT;
    MR_BRIGHTNESS = MR_COLOR_BRIGHTNESS;

    mr_init_multicolor_hd();

#else

    MR_MULTICOLOR = mr_true;
    mr_init();

#endif

}

void mr_cleanup() {

    mr_cleanup_hd();

}