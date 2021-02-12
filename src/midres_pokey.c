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

#include "midres.h"

#if defined(__POKEY__)

unsigned int MIDRES_SOUND_FREQUENCIES_POKEY[] = {
    256,		256,		256,		256,		256,		256,		256,		256,		256,		256,
    256,		256,		256,		256,		256,		256,		256,		256,		256,		256,
    256,		256,		256,		256,        256,        256,        256,        256,        256,        256,
    256,        256,        256,        256,        256,        256,        256,        256,        256,        256,
    256,        256,        256,        256,        256,        256,        256,        256,        256,        256,
    256,        256,        256,        256,        256,        256,        256,        256,        256,        243,
    230,        217,        204,        193,        182,        172,        162,        153,        144,        136,
    128,        121,        114,        108,        102,        96,         91,         85,         81,         76,
    72,         68,         64,         60,         57,         53,         50,         47,         45,         42,
    40,         37,         35,         33,         31,         30,         28,         26,         25,         23,
    22,         21,         19,         18,         17,         16,         15,         14,         13,         12,
    11,         10,         9,          8,          7,          6,          5,          4,          3,          2,
    1
};

// Hardware dependent sound library
void mr_sound_start_hd(unsigned char _channel, unsigned char _number) {

    //@todo: support _number under plus4 for mr_sound_start_hd
    _number = 0;

    switch ((_channel & 0x03)) {
        case 0:
            *((unsigned char*)0xd200) = 0xff;
            *((unsigned char*)0xd201) = 0x1f;
            break;
        case 1:
            *((unsigned char*)0xd202) = 0xff;
            *((unsigned char*)0xd203) = 0x1f;
            break;
        case 2:
            *((unsigned char*)0xd204) = 0xff;
            *((unsigned char*)0xd205) = 0x1f;
            break;
        case 3:
            *((unsigned char*)0xd206) = 0xff;
            *((unsigned char*)0xd207) = 0x1f;
            break;
    }
}

// Hardware dependent sound library
void mr_sound_change_hd(unsigned char _channel, int _parameter) {

    unsigned char freq = 0xff - (_parameter >> 8);

    switch ((_channel & 0x03)) {
        case 0:
            *((unsigned char*)0xd200) = freq;
            *((unsigned char*)0xd201) = 0xef;
            break;
        case 1:
            *((unsigned char*)0xd202) = freq;
            *((unsigned char*)0xd203) = 0xef;
            break;
        case 2:
            *((unsigned char*)0xd204) = freq;
            *((unsigned char*)0xd205) = 0xef;
            break;
        case 3:
            *((unsigned char*)0xd206) = freq;
            *((unsigned char*)0xd207) = 0xef;
            break;
    }

}

// Hardware dependent sound library
void mr_sound_stop_hd(unsigned char _channel) {
    switch ((_channel & 0x03)) {
    case 0:
        *((unsigned char*)0xd201) = 0x00;
        break;
    case 1:
        *((unsigned char*)0xd203) = 0x00;
        break;
    case 2:
        *((unsigned char*)0xd205) = 0x00;
        break;
    case 3:
        *((unsigned char*)0xd207) = 0x00;
        break;
    }
}


void mr_sound_program_change_channel_hd(unsigned char _channel, unsigned char _instrument) {

}

#include <stdio.h>

void mr_sound_frequency_channel_hd(unsigned char _channel, unsigned int _frequency, unsigned char _amplitude) {

    switch ((_channel & 0x03)) {
    case 0:
        *((unsigned char*)0xd200) = _frequency;
        *((unsigned char*)0xd201) = 0xe0 | ((_amplitude >> 4) & 0x0f);
        break;
    case 1:
        *((unsigned char*)0xd202) = _frequency;
        *((unsigned char*)0xd203) = 0xe0 | ((_amplitude >> 4) & 0x0f);
        break;
    case 2:
        *((unsigned char*)0xd204) = _frequency;
        *((unsigned char*)0xd205) = 0xe0 | ((_amplitude >> 4) & 0x0f);
        break;
    case 3:
        *((unsigned char*)0xd206) = _frequency;
        *((unsigned char*)0xd207) = 0xe0 | ((_amplitude >> 4) & 0x0f);
        break;
    }

    mr_pokey_set_frequency(_channel, _frequency);

}

#endif