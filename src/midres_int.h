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

#ifndef _MIDRES_INT_H_
#define _MIDRES_INT_H_

    #ifdef __OVERLAY__MIDRES__

         // These variables are defined by the linker, and allow to identify the 
         // address where to store the pieces of codeand data loaded from the 
         // mass memories.
        extern void _OVERLAY1_LOAD__[], _OVERLAY1_SIZE__[];
        extern void _OVERLAY2_LOAD__[], _OVERLAY2_SIZE__[];
        extern void _OVERLAY3_LOAD__[], _OVERLAY3_SIZE__[];
        extern void _OVERLAY4_LOAD__[], _OVERLAY4_SIZE__[];
        extern void _OVERLAY4_LOAD__[], _OVERLAY4_SIZE__[];

        unsigned char load_overlay(char* module_name, void* overlay_address,
            void* overlay_size);

    #endif

    /***
     * This value is used to check if the screen memory is corrupted.
     */

    #define MIXEL_THRESHOLD	0xff

    /************************************************************************
     * ------ INTERNAL FUNCTIONS
     ************************************************************************/

    mr_mixel get_mixel_bits(mr_mixel _current);
    mr_mixel calculate_mixel(mr_mixel _current, mr_mixelbits _abcd, mr_pop _pop);

#endif