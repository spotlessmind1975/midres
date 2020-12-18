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

#include "main.h"
#include "midres.h"

/****************************************************************************
 ** RESIDENT VARIABLES SECTION
 ****************************************************************************/

// All variables defined as global within the resident body of the code are 
// accessible from all modules, both resident and changing ones.

/****************************************************************************
 ** RESIDENT FUNCTIONS SECTION
 ****************************************************************************/

// All the functions defined within the resident body of the code are 
// accessible from all modules, both resident and changing ones.

/****************************************************************************
 ** RESIDENT MAIN FUNCTION
 ****************************************************************************/

// This is the main function body. The purpose is to call the various 
// functions present in the modules, taking care to load the relevant 
// code / data into memory(in the "overlay" area).

#include <stdio.h>

void main(void) {

#ifdef __DEMO_SLIDESHOW__

    demo_slideshow();

#endif

#ifdef __DEMO_DRAWING__

    demo_drawing();

#endif

#ifdef __DEMO_BITBLIT__

#ifndef __OVERLAY__MIDRES__

    demo_bitblit();

#endif

#endif

#ifdef __DEMO_TILE__

#ifndef __OVERLAY__MIDRES__

    demo_tile();

#endif

#endif

#ifdef __DEMO_TEST_CARD__

    demo_test_card();

#endif

#ifdef __AIRATTACK__

    game_air_attack();

#endif

#ifdef __TOTTO__

    game_totto();

#endif

#ifdef __TUTORIAL_MCTILE__

    tutorial_mctile();

#endif

#ifdef __TUTORIAL_PROTOTHREAD1__

    tutorial_protothread1();

#endif

#ifdef __TUTORIAL_PROTOTHREAD2__

    tutorial_protothread2();

#endif

#ifdef __ALIENSTORM__

    game_alien_storm();

#endif

#ifdef __ELEVATOR__

    game_elevator();

#endif

#ifdef __JOYCHECK__

    utility_joycheck();

#endif

}