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

#ifndef _MIDRES_HW_SVI_H_
#define _MIDRES_HW_SVI_H_

#define MR_KEY_NONE						64
#define MR_KEY_0						35
#define MR_KEY_1						56
#define MR_KEY_2						59
#define MR_KEY_3						8
#define MR_KEY_4						11
#define MR_KEY_5						16
#define MR_KEY_6						19
#define MR_KEY_7						24
#define MR_KEY_8						27
#define MR_KEY_9						32
#define MR_KEY_A						10
#define MR_KEY_ASTERISK					49
#define MR_KEY_AT						46
#define MR_KEY_B						28
#define MR_KEY_C						20
#define MR_KEY_CLEAR					51
#define MR_KEY_COLON					45
#define MR_KEY_COMMA					47
#define MR_KEY_COMMODORE				61
#define MR_KEY_CONTROL					58
#define MR_KEY_CRSR_LEFT_RIGHT			2
#define MR_KEY_CRSR_UP_DOWN				7
#define MR_KEY_D						18
#define MR_KEY_DELETE					0
#define MR_KEY_E						14
#define MR_KEY_EQUAL					53
#define MR_KEY_F						21
#define MR_KEY_F1						4
#define MR_KEY_F2						4
#define MR_KEY_F3						5
#define MR_KEY_F4						5
#define MR_KEY_F5						6
#define MR_KEY_F6						6
#define MR_KEY_F7						3
#define MR_KEY_F8						3
#define MR_KEY_G						26
#define MR_KEY_H						29
#define MR_KEY_HOME						51
#define MR_KEY_I						33
#define MR_KEY_INSERT					0
#define MR_KEY_J						34
#define MR_KEY_K						37
#define MR_KEY_L						42
#define MR_KEY_LEFT_ARROW				57
#define MR_KEY_LEFT_SHIFT				15
#define MR_KEY_M						36
#define MR_KEY_MINUS					43
#define MR_KEY_N						39
#define MR_KEY_O						38
#define MR_KEY_P						41
#define MR_KEY_PERIOD					44
#define MR_KEY_PLUS						40
#define MR_KEY_POUND					48
#define MR_KEY_Q						62
#define MR_KEY_R						17
#define MR_KEY_RETURN					1
#define MR_KEY_RIGHT_SHIFT				52
#define MR_KEY_RUNSTOP					63
#define MR_KEY_S						13
#define MR_KEY_SEMICOLON				50
#define MR_KEY_SLASH					55
#define MR_KEY_SPACE					60
#define MR_KEY_T						22
#define MR_KEY_U						30
#define MR_KEY_UP_ARROW					54
#define MR_KEY_V						31
#define MR_KEY_W						9
#define MR_KEY_X						23
#define MR_KEY_Y						25
#define MR_KEY_Z						12
#define MR_KEY_BACKSLASH                128
#define MR_KEY_SQUARE_OPEN              129
#define MR_KEY_SQUARE_CLOSED            130
#define MR_KEY_SEMICOMMA                131
#define MR_KEY_QUOTE                    132
#define MR_KEY_APIX                     133
#define MR_KEY_DEAD                     134
#define MR_KEY_SHIFT                    135
#define MR_KEY_CTRL                     136
#define MR_KEY_GRAPH                    137
#define MR_KEY_CAPS                     138
#define MR_KEY_CODE                     139
#define MR_KEY_ESC                      140
#define MR_KEY_TAB                      141
#define MR_KEY_STOP                     142
#define MR_KEY_BS                       143
#define MR_KEY_SELECT                   144
#define MR_KEY_RET                      145
#define MR_KEY_INS                      146
#define MR_KEY_DEL                      147
#define MR_KEY_LEFT                     148
#define MR_KEY_UP                       149
#define MR_KEY_DOWN                     150
#define MR_KEY_RIGHT                    151
#define MR_KEY_DIVISION                 154

#define MR_JOYSTICK_0					0
#define MR_JOYSTICK_1					1
#define MR_JOYSTICK_COUNT				2
#define MR_JOYSTICK_UP					1
#define MR_JOYSTICK_DOWN				2
#define MR_JOYSTICK_LEFT				4
#define MR_JOYSTICK_RIGHT				8
#define MR_JOYSTICK_FIRE				16
#define MR_JOYSTICK_FIREA				16
#define MR_JOYSTICK_FIREB				32

#define MR_TILE_WIDTH_IN_PIXEL			(8>>(MR_MULTICOLOR))
#define MR_TILE_WIDTH_FACTOR			(3-(MR_MULTICOLOR))

#define MR_TILE_BACKGROUND				0
#define MR_TILE_COLOR0					1
#define MR_TILE_COLOR1					2
#define MR_TILE_COLOR2					3

#define MIDRES_STANDALONE					1
//#define MIDRES_STANDALONE_BITBLIT			1	
//#define MIDRES_STANDALONE_DRAWING			1
//#define MIDRES_STANDALONE_DRAWING2			1
#define MIDRES_STANDALONE_SCREEN			1
//#define MIDRES_STANDALONE_SCREEN2			1
#define MIDRES_STANDALONE_TILE				1
#define MIDRES_STANDALONE_TILE_PROCESSING	1
//#define MIDRES_STANDALONE_TILE_MULTICOLOR	1
#define MIDRES_STANDALONE_KEYBOARD			1
// #define MIDRES_STANDALONE_FILE				1
#define MIDRES_EMBEDDED_FILES				1

#include "midres_tile_hd.h"

#endif