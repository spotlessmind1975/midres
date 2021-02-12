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

#if defined(__SN76489__)

#define CSG_OUT         0x06
#define CSG_IN          0x03

#define CSG_F1           00
#define CSG_A1           10
#define CSG_F2           20
#define CSG_A2           30
#define CSG_F3           40
#define CSG_A3           50
#define CSG_NC           60
#define CSG_NA           70

unsigned int MIDRES_SOUND_FREQUENCIES_SN76489[] = {
    255,		255,		255,		255,		255,		255,		255,		255,		255,		255,
    255,		255,		255,		255,		255,		255,		255,		255,		255,		255,
    255,		255,		255,        255,        255,        255,        255,        255,        255,        255,
    255,        240,        227,        214,        202,        191,        180,        170,        160,        151,
    143,        135,        127,        120,        113,        107,        101,        95,         90,         85,
    80,         75,         71,         67,         63,         60,         56,         53,         50,         47,
    45,         42,         40,         37,         35,         33,         31,         30,         28,         26,
    25,         23,         22,         21,         20,         18,         17,         16,         15,         15,
    14,         13,         12,         11,         11,         10,         10,         9,          8,          8,
    7,          7,          7,          6,          6,          5,          5,          5,          5,          4,
    4,          4,          3,          3,          3,          3,          3,          2,          2,          2,
    2,          2,          2,          2,          1,          1,          1,          1,          1           
};

// Hardware dependent sound library
void mr_sound_start_hd(unsigned char _channel, unsigned char _number) {
    unsigned char value;
    switch ((_channel & 0x3)) {
    case 0:
    case 3:
        io_put(CSG_OUT, 0X80 | CSG_F1 | 0);
        value = io_get(CSG_IN);
        io_put(CSG_OUT,        CSG_F1 | _number & 0x3f);
        value = io_get(CSG_IN);
        io_put(CSG_OUT, 0X80 | CSG_A1 | 0x00);
        value = io_get(CSG_IN);
        break;
    case 1:
        io_put(CSG_OUT, 0X80 | CSG_F2 | 0);
        value = io_get(CSG_IN);
        io_put(CSG_OUT, CSG_F2 | _number & 0x3f);
        value = io_get(CSG_IN);
        io_put(CSG_OUT, 0X80 | CSG_A2 | 0x00);
        value = io_get(CSG_IN);
        break;
    case 2:
        io_put(CSG_OUT, 0X80 | CSG_F3 | 0);
        value = io_get(CSG_IN);
        io_put(CSG_OUT, CSG_F3 | _number & 0x3f);
        value = io_get(CSG_IN);
        io_put(CSG_OUT, 0X80 | CSG_A3 | 0x00);
        value = io_get(CSG_IN);
        break;
    }
}

// Hardware dependent sound library
void mr_sound_change_hd(unsigned char _channel, int _parameter) {
    unsigned long g = 4000000 / ( 32 * _parameter);
    unsigned int f = (unsigned int)g;
    unsigned char value;

    switch ((_channel & 0x3)) {
    case 0:
    case 3:
        io_put(CSG_OUT, 0X80 | CSG_F1 | ( (f & 0x3ff) >> 6 ) );
        value = io_get(CSG_IN);
        io_put(CSG_OUT, CSG_F1 | (f & 0x3f));
        value = io_get(CSG_IN);
        io_put(CSG_OUT, 0X80 | CSG_A1 | 0x00);
        value = io_get(CSG_IN);
        break;
    case 1:
        io_put(CSG_OUT, 0X80 | CSG_F2 | ( (f & 0x3ff) >> 6 ) );
        value = io_get(CSG_IN);
        io_put(CSG_OUT, CSG_F2 | (f & 0x3f));
        value = io_get(CSG_IN);
        io_put(CSG_OUT, 0X80 | CSG_A2 | 0x00);
        value = io_get(CSG_IN);
        break;
    case 2:
        io_put(CSG_OUT, 0X80 | CSG_F3 | ( (f & 0x3ff) >> 6) ) ;
        value = io_get(CSG_IN);
        io_put(CSG_OUT, CSG_F3 | (f & 0x3f));
        value = io_get(CSG_IN);
        io_put(CSG_OUT, 0X80 | CSG_A3 | 0x00);
        value = io_get(CSG_IN);
        break;
    }
}

// Hardware dependent sound library
void mr_sound_stop_hd(unsigned char _channel) {
    unsigned char value;
    switch ((_channel & 0x3)) {
    case 0:
    case 3:
        io_put(CSG_OUT, 0X80 | CSG_A1 | 0x0f);
        value = io_get(CSG_IN);

        break;
    case 1:
        io_put(CSG_OUT, 0X80 | CSG_A2 | 0x0f);
        value = io_get(CSG_IN);

        break;
    case 2:
        io_put(CSG_OUT, 0X80 | CSG_A3 | 0x0f);
        value = io_get(CSG_IN);

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
        io_put(CSG_OUT, 0X80 | CSG_F1 | ((_frequency & 0x3ff) >> 6));
        value = io_get(CSG_IN);
        io_put(CSG_OUT, CSG_F1 | (_frequency & 0x3f));
        value = io_get(CSG_IN);
        io_put(CSG_OUT, 0X80 | CSG_A1 | 0x00);
        value = io_get(CSG_IN);
        break;
    case 1:
        io_put(CSG_OUT, 0X80 | CSG_F2 | ((_frequency & 0x3ff) >> 6));
        value = io_get(CSG_IN);
        io_put(CSG_OUT, CSG_F2 | (_frequency & 0x3f));
        value = io_get(CSG_IN);
        io_put(CSG_OUT, 0X80 | CSG_A2 | 0x00);
        value = io_get(CSG_IN);
        break;
    case 2:
        io_put(CSG_OUT, 0X80 | CSG_F3 | ((_frequency & 0x3ff) >> 6));
        value = io_get(CSG_IN);
        io_put(CSG_OUT, CSG_F3 | (_frequency & 0x3f));
        value = io_get(CSG_IN);
        io_put(CSG_OUT, 0X80 | CSG_A3 | 0x00);
        value = io_get(CSG_IN);
        break;
    }

}

#endif