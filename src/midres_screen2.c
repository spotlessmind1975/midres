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

#ifdef __CBM__
#include <cc65.h>
#include <cbm.h>
#include <device.h>
#else

#endif

#if defined(MIDRES_STANDALONE_SCREEN2)

unsigned char mr_save(char* _filename, mr_screen _screen) {

#ifdef MIDRES_STANDALONE_FILE

#ifdef __ATARI__

    FILE* f = fopen(_filename, "wb");
    if (f == NULL) {
        return 0;
    }
    fwrite(MR_SM(_screen), (unsigned)MR_SCREEN_RAM_SIZE, 1, f);
    fclose(f);
    return 1;

#elif __CBM__

    return cbm_save(_filename, getcurrentdevice(), MR_SM(_screen), MR_SCREEN_RAM_SIZE);

#endif

#endif

}

unsigned char mr_save_color(char* _filename, mr_screen _screen) {

#ifdef MIDRES_STANDALONE_FILE

#ifdef __ATARI__

    FILE* f = fopen(_filename, "wb");
    if (f == NULL) {
        return 0;
    }
    fwrite(MR_CM(_screen), (unsigned)MR_SCREEN_RAM_SIZE, 1, f);
    fclose(f);
    return 1;

#elif __CBM__

    _screen = 0;
    return cbm_save(_filename, getcurrentdevice(), MR_CM(_screen), MR_SCREEN_RAM_SIZE);

#endif

#endif
}

void mr_compress(mr_screen _source, mr_screen _destination) {

    mr_mixel* source = MR_SM(_source);
    mr_mixel* destination = MR_SM(_destination);
    mr_color* sourceColor = MR_CM(_source);

    int i;
    for (i = 0; i < MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT; ++i) {
        destination[i] = mr_pack_mixels(sourceColor[i], get_mixel_bits(source[i]));
    }

}

void mr_pack(mr_screen _source, mr_screen _destination, mr_half_screen _half_screen) {

    int i;
    mr_mixel* source = MR_SM(_source);
    mr_mixel* destination = MR_SM(_destination);

    if (_half_screen == mr_half_up) {
        destination += MR_SCREEN_RAM_SIZE >> 1;
    }

    for (i = 0; i < MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT; i += 2) {
        destination[i / 2] = mr_pack_mixels(get_mixel_bits(source[i + 1]), get_mixel_bits(source[i]));
    }

}

#endif