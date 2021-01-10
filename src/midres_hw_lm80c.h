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

#ifndef _MIDRES_HW_LM80C_H_
#define _MIDRES_HW_LM80C_H_

 //#define FRAME_BUFFER        1
 //#define GRAPHIC_MODE_I      1
 //#define GRAPHIC_MODE_II     1

#define MR_RENDERED_MIXELS MR_RENDERED_MIXELS_VDP

#define MR_NEXT_COLOR( c )				( ( ( ( c & 0x0f ) + 1 ) & 0x0f ) )

#define MR_COLOR_COUNT					16

#define MR_KEY_NONE						0
#define MR_KEY_0						'0'
#define MR_KEY_1						'1'
#define MR_KEY_2						'2'
#define MR_KEY_3						'3'
#define MR_KEY_4						'4'
#define MR_KEY_5						'5'
#define MR_KEY_6						'6'
#define MR_KEY_7						'7'
#define MR_KEY_8						'8'
#define MR_KEY_9						'9'
#define MR_KEY_A						'a'
#define MR_KEY_ASTERISK					'*'
#define MR_KEY_AT						'@'
#define MR_KEY_B						'b'
#define MR_KEY_C						'c'
#define MR_KEY_CLEAR					255
#define MR_KEY_COLON					':'
#define MR_KEY_COMMA					'.'
#define MR_KEY_COMMODORE				255
#define MR_KEY_CONTROL					255
#define MR_KEY_CRSR_LEFT_RIGHT			255
#define MR_KEY_CRSR_UP_DOWN				255
#define MR_KEY_D						'd'
#define MR_KEY_DELETE					8
#define MR_KEY_E						'e'
#define MR_KEY_EQUAL					'='
#define MR_KEY_F						'f'
#define MR_KEY_F1						1
#define MR_KEY_F2						2
#define MR_KEY_F3						4
#define MR_KEY_F4						5
#define MR_KEY_F5						6
#define MR_KEY_F6						22
#define MR_KEY_F7						23
#define MR_KEY_F8						24
#define MR_KEY_G						'g'
#define MR_KEY_H						'h'
#define MR_KEY_HOME						255
#define MR_KEY_I						'i'
#define MR_KEY_INSERT					8
#define MR_KEY_J						'j'
#define MR_KEY_K						'k'
#define MR_KEY_L						'l'
#define MR_KEY_LEFT_ARROW				28
#define MR_KEY_LEFT_SHIFT				255
#define MR_KEY_M						'm'
#define MR_KEY_MINUS					'-'
#define MR_KEY_N						'n'
#define MR_KEY_O						'o'
#define MR_KEY_P						'p'
#define MR_KEY_PERIOD					'.'
#define MR_KEY_PLUS						'+'
#define MR_KEY_POUND					'£'
#define MR_KEY_Q						'q'
#define MR_KEY_R						'r'
#define MR_KEY_RETURN					13
#define MR_KEY_RIGHT_SHIFT				255
#define MR_KEY_RUNSTOP					255
#define MR_KEY_S						's'
#define MR_KEY_SEMICOLON				';'
#define MR_KEY_SLASH					'/'
#define MR_KEY_SPACE					' '
#define MR_KEY_T						't'
#define MR_KEY_U						'u'
#define MR_KEY_UP_ARROW					30
#define MR_KEY_V						'v'
#define MR_KEY_W						'w'
#define MR_KEY_X						'x'
#define MR_KEY_Y						'y'
#define MR_KEY_Z						'z'
#define MR_KEY_BACKSLASH                '\\'
#define MR_KEY_SQUARE_OPEN              '['
#define MR_KEY_SQUARE_CLOSED            ']'
#define MR_KEY_SEMICOMMA                ';'
#define MR_KEY_QUOTE                    34
#define MR_KEY_APIX                     '^'
#define MR_KEY_DEAD                     255
#define MR_KEY_SHIFT                    255
#define MR_KEY_CTRL                     255
#define MR_KEY_GRAPH                    255
#define MR_KEY_CAPS                     255
#define MR_KEY_CODE                     255
#define MR_KEY_ESC                      27
#define MR_KEY_TAB                      3
#define MR_KEY_STOP                     255
#define MR_KEY_BS                       8
#define MR_KEY_SELECT                   255
#define MR_KEY_RET                      13
#define MR_KEY_INS                      8
#define MR_KEY_DEL                      8
#define MR_KEY_LEFT                     28
#define MR_KEY_UP                       30
#define MR_KEY_DOWN                     31
#define MR_KEY_RIGHT                    29
#define MR_KEY_DIVISION                 '/'

#define MR_JOYSTICK_0					0
#define MR_JOYSTICK_COUNT				0
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
#define MIDRES_STANDALONE_MUSIC				1

#include "midres_tile_hd.h"

#endif