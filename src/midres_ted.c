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

#if defined(__TED__)

unsigned int MIDRES_SOUND_FREQUENCIES_TED[] = {
    0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
    0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
    0,		0,		0,		-5816,  -5432,  -5070,  -4728,  -4405,  -4100,  -3812,
    -3541,  -3285,  -3043,  -2815,  -2599,  -2396,  -2204,  -2023,  -1852,  -1690,
    -1538,  -1394,  -1258,  -1130,  -1009,  -895,   -788,   -686,   -590,   -499,
    -414,   -333,   -257,   -185,   -117,   -53,    7,      64,     118,    169,
    217,    262,    305,    345,    383,    419,    453,    485,    516,    544,
    571,    597,    621,    643,    665,    685,    704,    722,    739,    755,
    770,    784,    798,    810,    822,    834,    844,    854,    864,    873,
    881,    889,    897,    904,    911,    917,    923,    929,    934,    939,
    944,    948,    953,    957,    960,    964,    967,    971,    974,    976,
    979,    982,    984,    986,    988,    990,    992,    994,    996
};

 // Hardware dependent sound library
void mr_sound_start_hd(unsigned char _channel, unsigned char _number) {

    //@todo: support _number under plus4 for mr_sound_start_hd
    _number = 0;

    switch ((_channel & 0x01)) {
    case 0:
        *((unsigned char*)0xff0e) = 769 & 0xff;
        *((unsigned char*)0xff12) = ((int)((unsigned char*)0xff12) & ~0x03) | (((769 & 0x300) >> 8) & 0x03);
        break;
    case 1:
        *((unsigned char*)0xff0f) = 769 & 0xff;
        *((unsigned char*)0xff10) = ((int)((unsigned char*)0xff10) & ~0x03) | (((769 & 0x300) >> 8) & 0x03);
    }

    *((unsigned char*)0xff11) = 0x14;

}

// Hardware dependent sound library
void mr_sound_change_hd(unsigned char _channel, int _parameter) {

    int v = 1024 - (111841 / _parameter);

    switch ((_channel & 0x01)) {
    case 0:
        *((unsigned char*)0xff0e) = v & 0xff;
        *((unsigned char*)0xff12) = ((int)((unsigned char*)0xff12) & ~0x03) | (((v & 0x300) >> 8) & 0x03);
        break;
    case 1:
        *((unsigned char*)0xff0f) = v & 0xff;
        *((unsigned char*)0xff10) = ((int)((unsigned char*)0xff10) & ~0x03) | (((v & 0x300) >> 8) & 0x03);
    }

}

// Hardware dependent sound library
void mr_sound_stop_hd(unsigned char _channel) {
    *((unsigned char*)0xff11) = *((unsigned char*)0xff11) && ~(1 << (4 + (_channel & 0x01)));
}


void mr_sound_program_change_channel_hd(unsigned char _channel, unsigned char _instrument) {

}

void mr_sound_frequency_channel_hd(unsigned char _channel, unsigned int _frequency, unsigned char _amplitude) {

    //@todo: manage _number in mr_sound_start_hd on C64
    switch ((_channel & 0x01)) {
        case 0:
            *((unsigned char*)0xff0e) = _frequency & 0xff;
            *((unsigned char*)0xff12) = (*((unsigned char*)0xff12) & ~0x03) | (((_frequency & 0x300) >> 8) & 0x03);
            break;
        case 1:
            *((unsigned char*)0xff0f) = _frequency & 0xff;
            *((unsigned char*)0xff10) = (*((unsigned char*)0xff10) & ~0x03) | (((_frequency & 0x300) >> 8) & 0x03);
            break;
    }

    mr_ted_set_frequency(_channel, _frequency);
    *((unsigned char*)0xff11) = 0x14;

}

#endif