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

void mr_unpack(mr_screen _source, mr_screen _destination, mr_half_screen _half_screen) {

    int i;
    mr_mixel* source = MR_SM(_source);
    mr_mixel* destination = MR_SM(_destination);

    if (_half_screen == mr_half_up) {
        source += (MR_SCREEN_RAM_SIZE >> 1);
    }

    for (i = 0; i < MR_SCREEN_WIDTH * (MR_SCREEN_HEIGHT >> 1); ++i) {
        destination[(i << 1)] = MR_RENDERED_MIXELS[mr_unpack_pmixel_lo(source[i])];
        destination[(i << 1) + 1] = MR_RENDERED_MIXELS[mr_unpack_pmixel_hi(source[i])];
    }

}

#endif