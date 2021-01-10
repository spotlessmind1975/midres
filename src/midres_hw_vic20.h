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

#ifndef _MIDRES_HW_VIC20_H_
#define _MIDRES_HW_VIC20_H_

/******************************************************************
 * ------ VIC 20 PLATFORM
 ******************************************************************/

// The maximum resolution is 44 x 46 pixels, equivalent to 
// 22 x 23 mixels.
#define MR_SCREEN_WIDTH					22
#define MR_SCREEN_HEIGHT				23
#define MR_SCREEN_ROW_WIDTH				MR_SCREEN_WIDTH
#define MR_SCREEN_RAM_SIZE				0x0200

// List of available screens:
#define MR_SCREEN_0						15
#define MR_SCREEN_COUNT					1
#define MR_SCREEN_DEFAULT				MR_SCREEN_0

// The two screens used for double buffering
#define	MR_SCREEN_DB1					MR_SCREEN_0
#define	MR_SCREEN_DB2					MR_SCREEN_0

// List of available tilesets:
#define MR_TILESET_ROM_0				0
#define MR_TILESET_ROM_1				1
#define MR_TILESET_ROM_2				2
#define MR_TILESET_ROM_3				3
#define MR_TILESET_0					15
#define MR_TILESET_COUNT				5
#define MR_TILESET_DEFAULT				MR_TILESET_ROM_2
#define MR_TILESET_TILE_COUNT			256

// List of available bitblits:
#define MR_AUX_0						15
#define MR_AUX_COUNT					0
#define MR_AUX_DEFAULT					MR_AUX_0

// Offset for correct brightness.
#define MR_COLOR_LOWBRIGHTNESS			0
#define MR_COLOR_BRIGHTNESS				0
#define MR_COLOR_HIGHBRIGHTNESS			0

// Default palette.
#define MR_COLOR_BLACK					0
#define MR_COLOR_WHITE					1
#define MR_COLOR_RED					2
#define MR_COLOR_CYAN					3
#define MR_COLOR_VIOLET					4
#define MR_COLOR_GREEN					5
#define MR_COLOR_BLUE					6
#define MR_COLOR_YELLOW					7
#define MR_COLOR_ORANGE					MR_COLOR_YELLOW
#define MR_COLOR_BROWN					MR_COLOR_RED
#define MR_COLOR_LIGHT_RED				MR_COLOR_RED
#define MR_COLOR_DARK_GREY				MR_COLOR_BLACK
#define MR_COLOR_GREY					MR_COLOR_WHITE
#define MR_COLOR_LIGHT_GREEN			MR_COLOR_GREEN
#define MR_COLOR_LIGHT_BLUE		 		MR_COLOR_BLUE
#define MR_COLOR_LIGHT_GREY				MR_COLOR_WHITE

#define MR_COLOR_DARK_BLUE				6
#define MR_COLOR_MAGENTA				MR_COLOR_RED
#define MR_COLOR_PURPLE					MR_COLOR_VIOLET
#define MR_COLOR_LAVENDER       		MR_COLOR_VIOLET
#define MR_COLOR_GOLD       			MR_COLOR_YELLOW
#define MR_COLOR_TURQUOISE       		MR_COLOR_LIGHT_BLUE
#define MR_COLOR_TAN       		        MR_COLOR_BROWN
#define MR_COLOR_YELLOW_GREEN       	MR_COLOR_YELLOW
#define MR_COLOR_OLIVE_GREEN       		MR_COLOR_GREEN
#define MR_COLOR_PINK       			MR_COLOR_LIGHT_RED
#define MR_COLOR_PEACH       			MR_COLOR_PINK

#define MR_NEXT_COLOR( c )				( ( ( ( c & 0x07 ) + 1 ) & 0x07 ) )

#define MR_COLOR_COUNT					16

#define MR_KEY_NONE						64
#define MR_KEY_1						0
#define MR_KEY_3						1
#define MR_KEY_5						2
#define MR_KEY_7						3
#define MR_KEY_9						4
#define MR_KEY_PLUS						5
#define MR_KEY_POUND					6
#define MR_KEY_DEL						7
#define MR_KEY_ARROW_LEFT				8
#define MR_KEY_W						9
#define MR_KEY_R						10
#define MR_KEY_Y						11
#define MR_KEY_I						12
#define MR_KEY_P						13
#define MR_KEY_ASTERISK					14
#define MR_KEY_RETURN					15
#define MR_KEY_NONE2					16
#define MR_KEY_A						17
#define MR_KEY_D						18
#define MR_KEY_G						19
#define MR_KEY_J						20
#define MR_KEY_L						21
#define MR_KEY_SEMICOMMA				22
#define MR_KEY_CURSOR_LEFT_RIGHT		23
#define MR_KEY_STOP						24
#define MR_KEY_NONE3					25
#define MR_KEY_X						26
#define MR_KEY_V						27
#define MR_KEY_N						28
#define MR_KEY_COMMA					29
#define MR_KEY_DIVISION					30
#define MR_KEY_CURSORS_UP_DOWN			31
#define MR_KEY_SPACE					32
#define MR_KEY_Z						33
#define MR_KEY_C						34
#define MR_KEY_B						35
#define MR_KEY_M						36
#define MR_KEY_PERIOD					37
#define MR_KEY_NONE4					38
#define MR_KEY_F1						39
#define MR_KEY_NONE5					40
#define MR_KEY_S						41
#define MR_KEY_F						42
#define MR_KEY_H						43
#define MR_KEY_K						44
#define MR_KEY_COLON					45
#define MR_KEY_EQUAL					46
#define MR_KEY_F3						47
#define MR_KEY_Q						48
#define MR_KEY_E						49
#define MR_KEY_T						50
#define MR_KEY_U						51
#define MR_KEY_O						52
#define MR_KEY_AT						53
#define MR_KEY_UP_ARROW					54
#define MR_KEY_F5						55
#define MR_KEY_2						56
#define MR_KEY_4						57
#define MR_KEY_6						58
#define MR_KEY_8						59
#define MR_KEY_0						60
#define MR_KEY_MINUS					61
#define MR_KEY_HOME						62
#define MR_KEY_F7						63

#define MR_JOYSTICK_0					0
#define MR_JOYSTICK_COUNT				1
#define MR_JOYSTICK_UP					4
#define MR_JOYSTICK_DOWN				8
#define MR_JOYSTICK_LEFT				16
#define MR_JOYSTICK_RIGHT				128
#define MR_JOYSTICK_FIRE				32

#define MR_TILE_WIDTH_IN_PIXEL			(8>>(MR_MULTICOLOR))
#define MR_TILE_WIDTH_FACTOR			(3-(MR_MULTICOLOR))
#define MR_TILE_BACKGROUND				3
#define MR_TILE_COLOR0					0
#define MR_TILE_COLOR1					1
#define MR_TILE_COLOR2					2

#define MR_SM(_screen)						((mr_mixel*)(0x1e00))
#define MR_CM(_screen)						((mr_color*)(0x9600))
#define MR_TM(_tileset)					((mr_mixel*)(0x8000 + (_tileset) * 0x0400))
#define MR_AM(_bitblit)					((mr_mixel*)(0x0000))

#define MR_WRITE_TILE_LUMINANCE(_screen, _offset, _tile) \
		_screen[(_offset)] = (_tile);

#define MR_WRITE_TILE(_screen, _colormap, _offset, _tile, _color) \
		_screen[(_offset)] = (_tile); \
		_colormap[(_offset)] = (_color);

#define MR_READ_TILE(_screen, _offset) _screen[(_offset)]
#define MR_READ_TILE_COLOR(_colormap, _offset) _colormap[(_offset)]

#define MR_PROTECTED_ACCESS_VRAM( f ) \
		{ \
			f; \
		}

#define MR_WRITE_VRAM(x,v)             MR_WRITE_VRAM_MM(x,v)
#define MR_READ_VRAM(x)				MR_READ_VRAM_MM(x)

#define MIDRES_STANDALONE					1
// #define MIDRES_STANDALONE_BITBLIT			1
// #define MIDRES_STANDALONE_DRAWING			1
// #define MIDRES_STANDALONE_DRAWING2			1
// #define MIDRES_STANDALONE_SCREEN				1
// #define MIDRES_STANDALONE_SCREEN2			1
#define MIDRES_STANDALONE_TILE				1
// #define MIDRES_STANDALONE_TILE_PROCESSING	1
// #define MIDRES_STANDALONE_TILE_MULTICOLOR	1
// #define MIDRES_STANDALONE_KEYBOARD			1
#define MIDRES_STANDALONE_FILE				1
// #define MIDRES_EMBEDDED_FILES				1
#define MIDRES_STANDALONE_MUSIC				1

#include "midres_tile.h"
#include "midres_tile_processing.h"

#endif