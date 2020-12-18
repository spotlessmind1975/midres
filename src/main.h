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

// Overlay management is driven by the definition of the appropriate 
// compilation symbol (__OVERLAY__). In this case, we enable or disable the 
// compilation of the relevant code.

#ifdef __OVERLAY__

    // These variables are defined by the linker, and allow to identify the 
    // address where to store the pieces of codeand data loaded from the 
    // mass memories.
    extern void _OVERLAY1_LOAD__[], _OVERLAY1_SIZE__[];
    extern void _OVERLAY2_LOAD__[], _OVERLAY2_SIZE__[];
    extern void _OVERLAY3_LOAD__[], _OVERLAY3_SIZE__[];
    extern void _OVERLAY4_LOAD__[], _OVERLAY4_SIZE__[];

    unsigned char load_overlay(char* module_name, void* overlay_address, 
                                                        void* overlay_size);

#endif

// RESIDENT FUNCTIONS
void demo_slideshow();
void demo_drawing();
void demo_bitblit();
void demo_tile();
void demo_test_card();

void game_air_attack();
void game_totto();
void game_alien_storm();
void game_elevator();

void tutorial_mctile();
void tutorial_protothread1();
void tutorial_protothread2();

void utility_joycheck();

// OVERLAYED FUNCTIONS (MODULE 1)
#include "midres.h"

// OVERLAYED FUNCTIONS (MODULE 2)

// OVERLAYED FUNCTIONS (MODULE 3)

// OVERLAYED FUNCTIONS (MODULE 4)
