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

#include "midres.h"

#if defined(__VIC__)

unsigned int MIDRES_SOUND_FREQUENCIES_VIC[] = {
    0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
    0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
    0,		0,		0,		0,      5,      19,     32,     45,     57,     68,
    78,     88,     98,     106,    115,    123,    130,    137,    144,    150,
    156,    161,    167,    172,    176,    181,    185,    189,    193,    196,
    199,    202,    205,    208,    211,    213,    216,    218,    220,    222,
    224,    226,    227,    229,    230,    232,    233,    234,    235,    236,
    237,    238,    239,    240,    241,    242,    243,    243,    244,    245,
    245,    246,    246,    247,    247,    248,    248,    248,    249,    249,
    249,    250,    250,    250,    251,    251,    251,    251,    252,    252,
    252,    252,    252,    252,    253,    253,    253,    253,    253,    253,
    253,    253,    253,    254,    254,    254,    254,    254,    254    
};

// Hardware dependent sound library
void mr_sound_start_hd(unsigned char _channel, unsigned char _number) {

    _channel = 0;
    _number = 0;

    *((unsigned char*)0x900e) = 5;

}

// Hardware dependent sound library
void mr_sound_change_hd(unsigned char _channel, int _parameter) {

    switch ((_channel & 0x03)) {
    case 0: case 3:
        *((unsigned char*)0x900a) = 128 + (_parameter >> 9);
        break;
    case 1:
        *((unsigned char*)0x900b) = 128 + (_parameter >> 9);
        break;
    case 2:
        *((unsigned char*)0x900c) = 128 + (_parameter >> 9);
        break;
    }

}

// Hardware dependent sound library
void mr_sound_stop_hd(unsigned char _channel) {

    // *((unsigned char*)0x900e) = 0;

    switch ((_channel & 0x03)) {
    case 0: case 3:
        *((unsigned char*)0x900a) = 0;
        break;
    case 1:
        *((unsigned char*)0x900b) = 0;
        break;
    case 2:
        *((unsigned char*)0x900c) = 0;
        break;
    }

}

void mr_sound_program_change_channel_hd(unsigned char _channel, unsigned char _instrument) {

}

void mr_sound_frequency_channel_hd(unsigned char _channel, unsigned int _frequency, unsigned char _amplitude) {

    switch ((_channel & 0x03)) {
    case 0: case 3:
        *((unsigned char*)0x900a) = 128 | _frequency;
        break;
    case 1:
        *((unsigned char*)0x900b) = 128 | _frequency;
        break;
    case 2:
        *((unsigned char*)0x900c) = 128 | _frequency;
        break;
    }

    *((unsigned char*)0x900e) = 5;
}

#endif