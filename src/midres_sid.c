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

#if defined(__SID__)

unsigned int MIDRES_SOUND_FREQUENCIES_SID[] = {
    0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
    0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
    0,		0,		0,		268,	284,	301,	318,	337,	358,	379,
    401,	425,	451,	477,	506,	536,	568,	602,	637,	675,
    716,	758,	803,	851,	902,	955,	1012,	1072,	1136,	1204,
    1275,	1351,	1432,	1517,	1607,	1703,	1804,	1911,	2025,	2145,
    2273,	2408,	2551,	2703,	2864,	3034,	3215,	3406,	3608,	3823,
    4050,	4291,	4547,	4817,	5103,	5407,	5728,	6069,	6430,	6812,
    7217,	7647,	8101,	8583,	9094,	9634,	10207,	10814,	11457,	12139,
    12860,	13625,	14435,	15294,	16203,	17167,	18188,	19269,	20415,	21629,
    22915,	24278,	25721,	27251,	28871,	30588,	32407,	34334,	36376,	38539,
    40830,	43258,	45830,	48556,	51443,	54502,	57743,	61176,	64814
};

// Hardware dependent sound library
void mr_sound_start_hd(unsigned char _channel, unsigned char _number) {

    //@todo: manage _number in mr_sound_start_hd on C64
    _number = 0;

    switch ((_channel & 0x03)) {
    case 0: case 3:
        *((unsigned char*)0xd405) = 97;
        *((unsigned char*)0xd406) = 200;
        *((unsigned char*)0xd404) = 33; // 65, 129
        break;
    case 1:
        *((unsigned char*)0xd40c) = 97;
        *((unsigned char*)0xd40d) = 200;
        *((unsigned char*)0xd40b) = 33; // 65, 129
        break;
    case 2:
        *((unsigned char*)0xd413) = 97;
        *((unsigned char*)0xd414) = 200;
        *((unsigned char*)0xd412) = 33; // 65, 129
        break;
    }

    *((unsigned char*)0xd418) = 15;

}

// Hardware dependent sound library
void mr_sound_change_hd(unsigned char _channel, int _parameter) {

    switch ((_channel & 0x03)) {
    case 0: case 3:
        *((unsigned char*)0xd400) = _parameter & 0xff;
        *((unsigned char*)0xd401) = _parameter >> 8;
        break;
    case 1:
        *((unsigned char*)0xd407) = _parameter & 0xff;
        *((unsigned char*)0xd408) = _parameter >> 8;
        break;
    case 2:
        *((unsigned char*)0xd40e) = _parameter & 0xff;
        *((unsigned char*)0xd40f) = _parameter >> 8;
        break;
    }

}

void mr_sound_control_channel_hd(unsigned char _channel, unsigned char _parameter, unsigned char _value) {
    //printf("mr_sound_control_channel_hd(%d %d %d)\n", _channel, _parameter, _value);
    //switch ((_channel & 0x03)) {
    //case 0: case 3:
    //    *((unsigned char*)0xd405) = 0x22;
    //    *((unsigned char*)0xd406) = 0xc8;
    //    *((unsigned char*)0xd404) = 0x20; // 65, 129
    //    break;
    //case 1:
    //    *((unsigned char*)0xd40c) = 0x22;
    //    *((unsigned char*)0xd40d) = 0xc8;
    //    *((unsigned char*)0xd404) = 0x20; // 65, 129
    //    break;
    //case 2:
    //    *((unsigned char*)0xd413) = 0x22;
    //    *((unsigned char*)0xd414) = 0xc8;
    //    *((unsigned char*)0xd404) = 0x20; // 65, 129
    //    break;
    //}
    * ((unsigned char*)0xd418) = 15;
}

unsigned char shadowRegisters[4];

#define     CALCULATE_BASE_ADDRESS( channel )                   (unsigned char*)(0xd400 + (((channel&0x3)==3)?0:((channel&0x03)*7)))
#define     PROGRAM_FREQUENCY( baseAddr, shadowRegister, f )    *baseAddr = ((f) & 0xff); ++baseAddr; *baseAddr = ((f>>8) & 0xff); baseAddr += 3; *(baseAddr) = (shadowRegister) | 0x01;
#define     SKIP_FREQUENCY( baseAddr )                          baseAddr += 2;
#define     PROGRAM_PULSE( baseAddr, shadowRegister, p )        *baseAddr = ((p) & 0xff); ++baseAddr; *baseAddr = ((p>>8) & 0x0f); ++baseAddr; shadowRegister = 0x40; *baseAddr = 0x40; ++baseAddr; 
#define     SKIP_PULSE( baseAddr )                              baseAddr += 2;
#define     PROGRAM_NOISE( baseAddr, shadowRegister )           shadowRegister = 0x80; *baseAddr = 0x80; ++baseAddr; 
#define     PROGRAM_SAW( baseAddr, shadowRegister )             shadowRegister = 0x20; *baseAddr = 0x20; ++baseAddr; 
#define     PROGRAM_TRIANGLE( baseAddr, shadowRegister )        shadowRegister = 0x10; *baseAddr = 0x10; ++baseAddr; 
#define     PROGRAM_SAW_TRIANGLE( baseAddr, shadowRegister )    shadowRegister = 0x30; *baseAddr = 0x30; ++baseAddr; 
#define     PROGRAM_ATTACK_DECAY( baseAddr, a, d )              *baseAddr = ( a & 0xf ) << 4 | ( d & 0xf ); ++baseAddr; 
#define     PROGRAM_SUSTAIN_RELEASE( baseAddr, s, r )           *baseAddr = ( s & 0xf ) << 4 | ( r & 0xf ); ++baseAddr; 
#define     STOP_FREQUENCY( baseAddr, shadowRegister )          baseAddr += 4; shadowRegister = shadowRegister & 0xfe; *(baseAddr) = shadowRegister;

void mr_sound_program_change_channel_hd(unsigned char _channel, unsigned char _instrument) {

    unsigned char* baseAddr = CALCULATE_BASE_ADDRESS(_channel);

    switch (_instrument) {

    case IMF_INSTRUMENT_PAD_5_BOWED:
    case IMF_INSTRUMENT_PAD_6_METALLIC:
    case IMF_INSTRUMENT_PAD_7_HALO:
    case IMF_INSTRUMENT_PAD_8_SWEEP:
    case IMF_INSTRUMENT_ACOUSTIC_GRAND_PIANO:
    case IMF_INSTRUMENT_BRIGHT_ACOUSTIC_PIANO:
    case IMF_INSTRUMENT_ELECTRIC_GRAND_PIANO:
    case IMF_INSTRUMENT_HONKY_TONK_PIANO:
    case IMF_INSTRUMENT_ELECTRIC_PIANO1:
    case IMF_INSTRUMENT_ELECTRIC_PIANO2:
        SKIP_FREQUENCY(baseAddr);
        //PROGRAM_PULSE(baseAddr, shadowRegisters[_channel&0x03], 128);
        SKIP_PULSE(baseAddr);
        //PROGRAM_NOISE(baseAddr, shadowRegisters[_channel&0x03]);
        //PROGRAM_SAW(baseAddr, shadowRegisters[_channel & 0x03]);
        PROGRAM_TRIANGLE(baseAddr, shadowRegisters[_channel & 0x03]);
        PROGRAM_ATTACK_DECAY(baseAddr, 4, 2);
        PROGRAM_SUSTAIN_RELEASE(baseAddr, 14, 10);
        break;

    case IMF_INSTRUMENT_HARPSICHORD:
    case IMF_INSTRUMENT_CLAVI:
    case IMF_INSTRUMENT_CELESTA:
        SKIP_FREQUENCY(baseAddr);
        PROGRAM_PULSE(baseAddr, shadowRegisters[_channel & 0x03], 1024);
        // SKIP_PULSE(baseAddr);
        //PROGRAM_NOISE(baseAddr, shadowRegisters[_channel&0x03]);
        //PROGRAM_SAW(baseAddr, shadowRegisters[_channel & 0x03]);
        //PROGRAM_TRIANGLE(baseAddr, shadowRegisters[_channel&0x03]);
        PROGRAM_ATTACK_DECAY(baseAddr, 3, 3);
        PROGRAM_SUSTAIN_RELEASE(baseAddr, 14, 3);
        break;

    case IMF_INSTRUMENT_LEAD_3_CALLIOPE:
    case IMF_INSTRUMENT_GLOCKENSPIEL:
    case IMF_INSTRUMENT_MUSIC_BOX:
    case IMF_INSTRUMENT_VIBRAPHONE:
    case IMF_INSTRUMENT_MARIMBA:
    case IMF_INSTRUMENT_XYLOPHONE:
    case IMF_INSTRUMENT_TUBULAR_BELLS:
    case IMF_INSTRUMENT_DULCIMER:
        SKIP_FREQUENCY(baseAddr);
        //PROGRAM_PULSE(baseAddr, shadowRegisters[_channel & 0x03], 1024);
        SKIP_PULSE(baseAddr);
        //PROGRAM_NOISE(baseAddr, shadowRegisters[_channel&0x03]);
        //PROGRAM_SAW(baseAddr, shadowRegisters[_channel & 0x03]);
        PROGRAM_TRIANGLE(baseAddr, shadowRegisters[_channel & 0x03]);
        PROGRAM_ATTACK_DECAY(baseAddr, 2, 10);
        PROGRAM_SUSTAIN_RELEASE(baseAddr, 12, 14);
        break;

    default:
    case IMF_INSTRUMENT_PAD_3_POLYSYNTH:
    case IMF_INSTRUMENT_DRAWBAR_ORGAN:
    case IMF_INSTRUMENT_PERCUSSIVE_ORGAN:
    case IMF_INSTRUMENT_ROCK_ORGAN:
    case IMF_INSTRUMENT_CHURCH_ORGAN:
    case IMF_INSTRUMENT_REED_ORGAN:
    case IMF_INSTRUMENT_ACCORDION:
    case IMF_INSTRUMENT_HARMONICA:
    case IMF_INSTRUMENT_TANGO_ACCORDION:
        SKIP_FREQUENCY(baseAddr);
        //PROGRAM_PULSE(baseAddr, shadowRegisters[_channel&0x03], p);
        SKIP_PULSE(baseAddr);
        //PROGRAM_NOISE(baseAddr, shadowRegisters[_channel&0x03]);
        //PROGRAM_SAW(baseAddr, shadowRegisters[_channel & 0x03]);
        PROGRAM_TRIANGLE(baseAddr, shadowRegisters[_channel & 0x03]);
        PROGRAM_ATTACK_DECAY(baseAddr, 3, 3);
        PROGRAM_SUSTAIN_RELEASE(baseAddr, 14, 14);
        break;

    case IMF_INSTRUMENT_ACOUSTIC_GUITAR_NYLON:
    case IMF_INSTRUMENT_ACOUSTIC_GUITAR_STEEL:
    case IMF_INSTRUMENT_ELECTRIC_GUITAR_JAZZ:
    case IMF_INSTRUMENT_ELECTRIC_GUITAR_CLEAN:
    case IMF_INSTRUMENT_OVERDRIVEN_GUITAR:
    case IMF_INSTRUMENT_DISTORTION_GUITAR:
    case IMF_INSTRUMENT_GUITAR_HARMONICS:
        SKIP_FREQUENCY(baseAddr);
        PROGRAM_PULSE(baseAddr, shadowRegisters[_channel & 0x03], 128);
        //SKIP_PULSE(baseAddr);
        //PROGRAM_NOISE(baseAddr, shadowRegisters[_channel&0x03]);
        //PROGRAM_SAW(baseAddr, shadowRegisters[_channel & 0x03]);
        //PROGRAM_TRIANGLE(baseAddr, shadowRegisters[_channel&0x03]);
        PROGRAM_ATTACK_DECAY(baseAddr, 10, 10);
        PROGRAM_SUSTAIN_RELEASE(baseAddr, 14, 10);
        break;

    case IMF_INSTRUMENT_ELECTRIC_GUITAR_MUTED:
        SKIP_FREQUENCY(baseAddr);
        PROGRAM_PULSE(baseAddr, shadowRegisters[_channel & 0x03], 128);
        // SKIP_PULSE(baseAddr);
        //PROGRAM_NOISE(baseAddr, shadowRegisters[_channel&0x03]);
        //PROGRAM_SAW(baseAddr, shadowRegisters[_channel & 0x03]);
        //PROGRAM_TRIANGLE(baseAddr, shadowRegisters[_channel&0x03]);
        PROGRAM_ATTACK_DECAY(baseAddr, 1, 2);
        PROGRAM_SUSTAIN_RELEASE(baseAddr, 4, 3);
        break;

    case IMF_INSTRUMENT_LEAD_8_BASS_LEAD:
    case IMF_INSTRUMENT_ACOUSTIC_BASS:
    case IMF_INSTRUMENT_ELECTRIC_BASS_FINGER:
    case IMF_INSTRUMENT_ELECTRIC_BASS_PICK:
    case IMF_INSTRUMENT_FRETLESS_BASS:
    case IMF_INSTRUMENT_SLAP_BASS_1:
    case IMF_INSTRUMENT_SLAP_BASS_2:
    case IMF_INSTRUMENT_SYNTH_BASS_1:
    case IMF_INSTRUMENT_SYNTH_BASS_2:
        SKIP_FREQUENCY(baseAddr);
        //PROGRAM_PULSE(baseAddr, shadowRegisters[_channel & 0x03], 1024);
        SKIP_PULSE(baseAddr);
        //PROGRAM_NOISE(baseAddr, shadowRegisters[_channel&0x03]);
        //PROGRAM_SAW(baseAddr, shadowRegisters[_channel & 0x03]);
        PROGRAM_TRIANGLE(baseAddr, shadowRegisters[_channel & 0x03]);
        PROGRAM_ATTACK_DECAY(baseAddr, 2, 10);
        PROGRAM_SUSTAIN_RELEASE(baseAddr, 12, 14);
        break;

    case IMF_INSTRUMENT_LEAD_1_SQUARE:
    case IMF_INSTRUMENT_VIOLIN:
    case IMF_INSTRUMENT_VIOLA:
    case IMF_INSTRUMENT_CELLO:
    case IMF_INSTRUMENT_CONTRABASS:
    case IMF_INSTRUMENT_TREMOLO_STRINGS:
    case IMF_INSTRUMENT_PIZZICATO_STRINGS:
    case IMF_INSTRUMENT_ORCHESTRAL_HARP:
    case IMF_INSTRUMENT_STRING_ENSEMBLE_1:
    case IMF_INSTRUMENT_STRING_ENSEMBLE_2:
    case IMF_INSTRUMENT_SYNTHSTRINGS_1:
    case IMF_INSTRUMENT_SYNTHSTRINGS_2:
        SKIP_FREQUENCY(baseAddr);
        PROGRAM_PULSE(baseAddr, shadowRegisters[_channel & 0x03], 128);
        //SKIP_PULSE(baseAddr);
        //PROGRAM_NOISE(baseAddr, shadowRegisters[_channel&0x03]);
        //PROGRAM_SAW(baseAddr, shadowRegisters[_channel & 0x03]);
        //PROGRAM_TRIANGLE(baseAddr, shadowRegisters[_channel&0x03]);
        PROGRAM_ATTACK_DECAY(baseAddr, 10, 10);
        PROGRAM_SUSTAIN_RELEASE(baseAddr, 14, 10);
        break;

    case IMF_INSTRUMENT_PAD_4_CHOIR:
    case IMF_INSTRUMENT_CHOIR_AAHS:
    case IMF_INSTRUMENT_VOICE_OOHS:
    case IMF_INSTRUMENT_SYNTH_VOICE:
    case IMF_INSTRUMENT_LEAD_4_CHIFF:
    case IMF_INSTRUMENT_LEAD_5_CHARANG:
    case IMF_INSTRUMENT_LEAD_6_VOICE:
    case IMF_INSTRUMENT_LEAD_7_FIFTHS:
    case IMF_INSTRUMENT_FX_1_RAIN:
    case IMF_INSTRUMENT_FX_2_SOUNDTRACK:
    case IMF_INSTRUMENT_FX_3_CRYSTAL:
    case IMF_INSTRUMENT_FX_4_ATMOSPHERE:
    case IMF_INSTRUMENT_FX_5_BRIGHTNESS:
    case IMF_INSTRUMENT_FX_6_GOBLINS:
    case IMF_INSTRUMENT_FX_7_ECHOES:
    case IMF_INSTRUMENT_FX_8_SCI_FI:
    case IMF_INSTRUMENT_TIMPANI:
    case IMF_INSTRUMENT_ORCHESTRA_HIT:
    case IMF_INSTRUMENT_APPLAUSE:
        SKIP_FREQUENCY(baseAddr);
        //PROGRAM_PULSE(baseAddr, shadowRegisters[_channel&0x03], 128);
        SKIP_PULSE(baseAddr);
        PROGRAM_NOISE(baseAddr, shadowRegisters[_channel & 0x03]);
        //PROGRAM_SAW(baseAddr, shadowRegisters[_channel & 0x03]);
        //PROGRAM_TRIANGLE(baseAddr, shadowRegisters[_channel & 0x03]);
        PROGRAM_ATTACK_DECAY(baseAddr, 1, 14);
        PROGRAM_SUSTAIN_RELEASE(baseAddr, 14, 14);
        break;

    case IMF_INSTRUMENT_LEAD_2_SAWTOOTH:
    case IMF_INSTRUMENT_PAD_1_NEW_AGE:
    case IMF_INSTRUMENT_PAD_2_WARM:
    case IMF_INSTRUMENT_TRUMPET:
    case IMF_INSTRUMENT_TROMBONE:
    case IMF_INSTRUMENT_TUBA:
    case IMF_INSTRUMENT_MUTED_TRUMPET:
    case IMF_INSTRUMENT_FRENCH_HORN:
    case IMF_INSTRUMENT_BRASS_SECTION:
    case IMF_INSTRUMENT_SYNTHBRASS_1:
    case IMF_INSTRUMENT_SYNTHBRASS_2:
    case IMF_INSTRUMENT_SOPRANO_SAX:
    case IMF_INSTRUMENT_ALTO_SAX:
    case IMF_INSTRUMENT_TENOR_SAX:
    case IMF_INSTRUMENT_BARITONE_SAX:
    case IMF_INSTRUMENT_OBOE:
    case IMF_INSTRUMENT_ENGLISH_HORN:
    case IMF_INSTRUMENT_BASSOON:
    case IMF_INSTRUMENT_CLARINET:
    case IMF_INSTRUMENT_PICCOLO:
    case IMF_INSTRUMENT_FLUTE:
    case IMF_INSTRUMENT_RECORDER:
    case IMF_INSTRUMENT_PAN_FLUTE:
    case IMF_INSTRUMENT_BLOWN_BOTTLE:
    case IMF_INSTRUMENT_SHAKUHACHI:
    case IMF_INSTRUMENT_WHISTLE:
    case IMF_INSTRUMENT_OCARINA:
        SKIP_FREQUENCY(baseAddr);
        //PROGRAM_PULSE(baseAddr, shadowRegisters[_channel&0x03], p);
        SKIP_PULSE(baseAddr);
        //PROGRAM_NOISE(baseAddr, shadowRegisters[_channel&0x03]);
        PROGRAM_SAW(baseAddr, shadowRegisters[_channel & 0x03]);
        //PROGRAM_TRIANGLE(baseAddr, shadowRegisters[_channel&0x03]);
        PROGRAM_ATTACK_DECAY(baseAddr, 3, 3);
        PROGRAM_SUSTAIN_RELEASE(baseAddr, 14, 14);
        break;

    case IMF_INSTRUMENT_SITAR:
    case IMF_INSTRUMENT_BANJO:
    case IMF_INSTRUMENT_SHAMISEN:
    case IMF_INSTRUMENT_KOTO:
    case IMF_INSTRUMENT_KALIMBA:
    case IMF_INSTRUMENT_BAG_PIPE:
    case IMF_INSTRUMENT_FIDDLE:
    case IMF_INSTRUMENT_SHANAI:
    case IMF_INSTRUMENT_TINKLE_BELL:
    case IMF_INSTRUMENT_AGOGO:
    case IMF_INSTRUMENT_STEEL_DRUMS:
    case IMF_INSTRUMENT_WOODBLOCK:
    case IMF_INSTRUMENT_TAIKO_DRUM:
    case IMF_INSTRUMENT_MELODIC_TOM:
    case IMF_INSTRUMENT_SYNTH_DRUM:
    case IMF_INSTRUMENT_REVERSE_CYMBAL:
    case IMF_INSTRUMENT_GUITAR_FRET_NOISE:
    case IMF_INSTRUMENT_BREATH_NOISE:
    case IMF_INSTRUMENT_SEASHORE:
    case IMF_INSTRUMENT_BIRD_TWEET:
    case IMF_INSTRUMENT_TELEPHONE_RING:
    case IMF_INSTRUMENT_HELICOPTER:
    case IMF_INSTRUMENT_GUNSHOT:

        SKIP_FREQUENCY(baseAddr);
        //PROGRAM_PULSE(baseAddr, shadowRegisters[_channel&0x03], p);
        SKIP_PULSE(baseAddr);
        //PROGRAM_NOISE(baseAddr, shadowRegisters[_channel&0x03]);
        PROGRAM_SAW(baseAddr, shadowRegisters[_channel & 0x03]);
        //PROGRAM_TRIANGLE(baseAddr, shadowRegisters[_channel&0x03]);
        PROGRAM_ATTACK_DECAY(baseAddr, 3, 3);
        PROGRAM_SUSTAIN_RELEASE(baseAddr, 14, 14);
        break;
    }

}

void mr_sound_frequency_channel_hd(unsigned char _channel, unsigned int _frequency, unsigned char _amplitude) {
    //printf("mr_sound_frequency_channel_hd(%d %d %d)\n", _channel, _frequency, _amplitude);
    //@todo: manage _number in mr_sound_start_hd on C64

    unsigned char* baseAddr = CALCULATE_BASE_ADDRESS(_channel);

    PROGRAM_FREQUENCY(baseAddr, shadowRegisters[_channel & 0x03], _frequency);

    *((unsigned char*)0xd418) = 15;

    while (_channel > 14) {
        _channel -= 14;
    }
    mr_sid_set_frequency(_channel, _frequency);

}

// Hardware dependent sound library
void mr_sound_stop_hd(unsigned char _channel) {

    unsigned c;

    unsigned char* baseAddr = CALCULATE_BASE_ADDRESS(_channel);

    STOP_FREQUENCY(baseAddr, shadowRegisters[_channel & 0x03]);

    //mr_sid_set_frequency((_channel & 0x03) == 3 ? 0 : (_channel & 0x03), 0);

    //if (_channel > 11) {
    //    c = 0;
    //}
    //mr_sid_set_frequency(c, 0);

}

#endif