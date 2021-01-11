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

#if defined(__AY8910__)

#ifdef __MSX__
#define PSG_AP          0xa0
#define PSG_WP          0xa1
#define PSG_RP          0xa2
#elif __LM80C__
#define PSG_AP          0x40
#define PSG_WP          0x41
#define PSG_RP          0x41
#endif

#define PSG_R0           0
#define PSG_R1           1
#define PSG_R2           2
#define PSG_R3           3
#define PSG_R4           4
#define PSG_R5           5
#define PSG_R6           6
#define PSG_R7           7
#define PSG_R8           8
#define PSG_R9           9
#define PSG_R10         10
#define PSG_R11         11
#define PSG_R12         12
#define PSG_R13         13
#define PSG_R14         14
#define PSG_R15         15

unsigned int MIDRES_SOUND_FREQUENCIES_AY8910[] = {
    6841,		6841,		6841,		6841,		6841,		6841,		6841,		6841,		6841,		6841,
    6841,		6841,		6841,		6841,		6841,		6841,		6841,		6841,		6841,		6841,
    6841,		6841,		6841,       6841,       6457,       6095,       5753,       5430,       5125,       4837,
    4566,       4310,       4068,       3839,       3624,       3420,       3229,       3047,       2876,       2715,
    2562,       2419,       2283,       2155,       2034,       1920,       1812,       1710,       1614,       1524,
    1438,       1357,       1281,       1209,       1141,       1077,       1017,       960,        906,        855,
    807,        762,        719,        679,        641,        605,        571,        539,        508,        480,
    453,        428,        404,        381,        360,        339,        320,        302,        285,        269,
    254,        240,        226,        214,        202,        190,        180,        170,        160,        151,
    143,        135,        127,        120,        113,        107,        101,        95,         90,         85,
    80,         76,         71,         67,         64,         60,         57,         53,         50,         48,
    45,         42,         40,         38,         36,         34,         32,         30,         28
};

// Hardware dependent sound library
void mr_sound_start_hd(unsigned char _channel, unsigned char _number) {
    unsigned char value;
    switch ((_channel & 0x3)) {
    case 0:
    case 3:
        io_put(PSG_AP, PSG_R7);
        value = io_get(PSG_RP);
        io_put(PSG_AP, PSG_R7);
#ifdef __MSX__
        io_put(PSG_WP, (value & 0xfe));
#elif __LM80C__
        io_put(PSG_WP, 0xf8 | (value & 0x06));
#endif
        break;
    case 1:
        io_put(PSG_AP, PSG_R7);
        value = io_get(PSG_RP);
        io_put(PSG_AP, PSG_R7);
#ifdef __MSX__
        io_put(PSG_WP, (value & 0xfd));
#elif __LM80C__
        io_put(PSG_WP, 0xf8 | (value & 0x05));
#endif
        break;
    case 2:
        io_put(PSG_AP, PSG_R7);
        value = io_get(PSG_RP);
        io_put(PSG_AP, PSG_R7);
#ifdef __MSX__
        io_put(PSG_WP, (value & 0xfb));
#elif __LM80C__
        io_put(PSG_WP, 0xf8 | (value & 0x03));
#endif        
        break;
    }
}

// Hardware dependent sound library
void mr_sound_change_hd(unsigned char _channel, int _parameter) {
    int f = 111861 / _parameter;
    switch ((_channel & 0x3)) {
    case 0:
    case 3:
        io_put(PSG_AP, PSG_R0);
        io_put(PSG_WP, f & 0xff);
        io_put(PSG_AP, PSG_R1);
        io_put(PSG_WP, (f >> 8) & 0x0f);
        io_put(PSG_AP, PSG_R8);
        io_put(PSG_WP, 0x08);
        break;
    case 1:
        io_put(PSG_AP, PSG_R2);
        io_put(PSG_WP, f & 0xff);
        io_put(PSG_AP, PSG_R3);
        io_put(PSG_WP, (f >> 8) & 0x0f);
        io_put(PSG_AP, PSG_R9);
        io_put(PSG_WP, 0x08);
        break;
    case 2:
        io_put(PSG_AP, PSG_R4);
        io_put(PSG_WP, f & 0xff);
        io_put(PSG_AP, PSG_R5);
        io_put(PSG_WP, (f >> 8) & 0x0f);
        io_put(PSG_AP, PSG_R10);
        io_put(PSG_WP, 0x08);
        break;
    }
}

// Hardware dependent sound library
void mr_sound_stop_hd(unsigned char _channel) {
    unsigned char value;
    switch ((_channel & 0x3)) {
    case 0:
    case 3:
        io_put(PSG_AP, PSG_R7);
        value = io_get(PSG_RP);
        io_put(PSG_AP, PSG_R7);
#ifdef __MSX__
        io_put(PSG_WP, (value & 0xfe) | 0x01);
#elif __LM80C__
        io_put(PSG_WP, 0xf8 | (value & 0x07) | 0x01);
#endif

        break;
    case 1:
        io_put(PSG_AP, PSG_R7);
        value = io_get(PSG_RP);
        io_put(PSG_AP, PSG_R7);
#ifdef __MSX__
        io_put(PSG_WP, (value & 0xfd) | 0x02);
#elif __LM80C__
        io_put(PSG_WP, 0xf8 | (value & 0x07) | 0x02);
#endif
        break;
    case 2:
        io_put(PSG_AP, PSG_R7);
        value = io_get(PSG_RP);
        io_put(PSG_AP, PSG_R7);
#ifdef __MSX__
        io_put(PSG_WP, (value & 0xfb) | 0x04);
#elif __LM80C__
        io_put(PSG_WP, 0xf8 | (value & 0x07) | 0x04);
#endif
        break;
    }
}

void mr_sound_program_change_channel_hd(unsigned char _channel, unsigned char _instrument) {

}

#include <stdio.h>

void mr_sound_frequency_channel_hd(unsigned char _channel, unsigned int _frequency, unsigned char _amplitude) {

    unsigned char value;

    switch ((_channel & 0x3)) {
    case 0:
    case 3:
        io_put(PSG_AP, PSG_R7);
        value = io_get(PSG_RP);
        io_put(PSG_AP, PSG_R7);
#ifdef __MSX__
        io_put(PSG_WP, (value & 0xfe));
#elif __LM80C__
        io_put(PSG_WP, 0xf8 | (value & 0x06));
#endif
        io_put(PSG_AP, PSG_R0);
        io_put(PSG_WP, _frequency & 0xff);
        io_put(PSG_AP, PSG_R1);
        io_put(PSG_WP, (_frequency >> 8) & 0x0f);
        io_put(PSG_AP, PSG_R8);
        io_put(PSG_WP, 0x0e);
        break;
    case 1:
        io_put(PSG_AP, PSG_R7);
        value = io_get(PSG_RP);
        io_put(PSG_AP, PSG_R7);
#ifdef __MSX__
        io_put(PSG_WP, (value & 0xfd));
#elif __LM80C__
        io_put(PSG_WP, 0xf8 | (value & 0x05));
#endif
        io_put(PSG_AP, PSG_R2);
        io_put(PSG_WP, _frequency & 0xff);
        io_put(PSG_AP, PSG_R3);
        io_put(PSG_WP, (_frequency >> 8) & 0x0f);
        io_put(PSG_AP, PSG_R9);
        io_put(PSG_WP, 0x0e);
        break;
    case 2:
        io_put(PSG_AP, PSG_R7);
        value = io_get(PSG_RP);
        io_put(PSG_AP, PSG_R7);
#ifdef __MSX__
        io_put(PSG_WP, (value & 0xfb));
#elif __LM80C__
        io_put(PSG_WP, 0xf8 | (value & 0x03));
#endif
        io_put(PSG_AP, PSG_R4);
        io_put(PSG_WP, _frequency & 0xff);
        io_put(PSG_AP, PSG_R5);
        io_put(PSG_WP, (_frequency >> 8) & 0x0f);
        io_put(PSG_AP, PSG_R10);
        io_put(PSG_WP, 0x0e);
        break;
    }

    //mr_pokey_set_frequency(_channel, _frequency);

}

#endif