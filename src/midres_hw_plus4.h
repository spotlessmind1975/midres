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

#ifndef _MIDRES_HW_PLUS4_H_
#define _MIDRES_HW_PLUS4_H_

 /******************************************************************
  * ------ COMMODORE PLUS/4 PLATFORM
  ******************************************************************/

  // The maximum resolution is 80 x 50 pixels, equivalent to 
  // 40 x 25 mixels.
#define MR_SCREEN_WIDTH					40
#define MR_SCREEN_HEIGHT				25
#define MR_SCREEN_ROW_WIDTH				MR_SCREEN_WIDTH
#define MR_SCREEN_RAM_SIZE				0x0400

// List of available screens:
#define MR_SCREEN_0						1
#define MR_SCREEN_1						2
#define MR_SCREEN_2						3
#define MR_SCREEN_3						4
#define MR_SCREEN_4						5
#define MR_SCREEN_5						6
#define MR_SCREEN_6						7
#define MR_SCREEN_COUNT					7
#define MR_SCREEN_DEFAULT				MR_SCREEN_0

// The screens used for double buffering
#define	MR_SCREEN_DB1					MR_SCREEN_0
#define	MR_SCREEN_DB2					MR_SCREEN_1

// List of available tilesets:
#define MR_TILESET_ROM_0				52
#define MR_TILESET_0					6
#define MR_TILESET_1					8
#define MR_TILESET_2					10
#define MR_TILESET_3					12
#define MR_TILESET_4					14
#define MR_TILESET_COUNT				6
#define MR_TILESET_DEFAULT				MR_TILESET_ROM_0
#define MR_TILESET_TILE_COUNT			256

// List of available areas for bit blits:
#define MR_AUX_0						2
#define MR_AUX_1						3
#define MR_AUX_2						4
#define MR_AUX_3						5
#define MR_AUX_4						6
#define MR_AUX_5						7
#define MR_AUX_COUNT					6
#define MR_AUX_DEFAULT					MR_AUX_2

// Offset for correct brightness.
#define MR_COLOR_LOWBRIGHTNESS			(3<<4)
#define MR_COLOR_BRIGHTNESS				(5<<4)
#define MR_COLOR_HIGHBRIGHTNESS			(7<<4)

// Default palette.
#define MR_COLOR_BLACK					(0|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_WHITE					(1|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_RED					(2|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_CYAN					(3|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_PURPLE					(4|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_GREEN					(5|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_BLUE					(6|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_YELLOW					(7|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_ORANGE					(8|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_BROWN					(9|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_YELLOW_GREEN			(10|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_PINK					(11|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_BLUE_GREEN				(12|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_LIGHT_BLUE				(13|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_DARK_BLUE				(14|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_LIGHT_GREEN			(15|MR_COLOR_BRIGHTNESS)

#define MR_COLOR_VIOLET					4
#define MR_COLOR_LIGHT_RED				(2|MR_COLOR_HIGHBRIGHTNESS)
#define MR_COLOR_DARK_GREY				1
#define MR_COLOR_GREY					(1|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_LIGHT_GREY				(1|MR_COLOR_HIGHBRIGHTNESS)
#define MR_COLOR_MAGENTA				MR_COLOR_RED
#define MR_COLOR_LAVENDER       		MR_COLOR_VIOLET
#define MR_COLOR_GOLD       			MR_COLOR_YELLOW
#define MR_COLOR_TURQUOISE       		MR_COLOR_LIGHT_BLUE
#define MR_COLOR_TAN       		        MR_COLOR_BROWN
#define MR_COLOR_OLIVE_GREEN       		MR_COLOR_GREEN
#define MR_COLOR_PEACH       			MR_COLOR_PINK

#define MR_NEXT_COLOR( c )				( c & 0xf0 | ( ( ( c & 0x0f ) + 1 ) & 0x0f ) )

#define MR_COLOR_COUNT					16

#define MR_KEY_NONE						64
#define MR_KEY_ESC						52
#define MR_KEY_1						56
#define MR_KEY_2						59
#define MR_KEY_3						8
#define MR_KEY_4						11
#define MR_KEY_5						16
#define MR_KEY_6						19
#define MR_KEY_7						24
#define MR_KEY_8						27
#define MR_KEY_9						32
#define MR_KEY_LEFT						48
#define MR_KEY_RIGHT					51
#define MR_KEY_UP						43
#define MR_KEY_DOWN						40
#define MR_KEY_INS						0
#define MR_KEY_DEL						0
#define MR_KEY_Q						62
#define MR_KEY_W						9
#define MR_KEY_E						14
#define MR_KEY_R						17
#define MR_KEY_T						22
#define MR_KEY_Y						25
#define MR_KEY_U						30
#define MR_KEY_I						33
#define MR_KEY_O						38
#define MR_KEY_P						41
#define MR_KEY_AT						7
#define MR_KEY_PLUS						54
#define MR_KEY_MINUS					46
#define MR_KEY_CLEAR					57
#define MR_KEY_HOME						57
#define MR_KEY_F1						4
#define MR_KEY_F2						5
#define MR_KEY_F3						6
#define MR_KEY_HELP						3
#define MR_KEY_A						10
#define MR_KEY_S						13
#define MR_KEY_D						18
#define MR_KEY_F						21
#define MR_KEY_G						26
#define MR_KEY_H						29
#define MR_KEY_J						34
#define MR_KEY_K						37
#define MR_KEY_L						42
#define MR_KEY_SEMICOLON				45
#define MR_KEY_SEMICOMMA				50
#define MR_KEY_ASTERISK					49
#define MR_KEY_RETURN					1
#define MR_KEY_Z						12
#define MR_KEY_X						23
#define MR_KEY_C						20
#define MR_KEY_V						31
#define MR_KEY_B						28
#define MR_KEY_N						34
#define MR_KEY_M						36
#define MR_KEY_COMMA					47
#define MR_KEY_PERIOD					44
#define MR_KEY_SLASH					55
#define MR_KEY_POUND					2
#define MR_KEY_EQUAL					53
#define MR_KEY_SPACE					60

#define MR_JOYSTICK_0					0
#define MR_JOYSTICK_1					1
#define MR_JOYSTICK_COUNT				2
#define MR_JOYSTICK_UP					1
#define MR_JOYSTICK_DOWN				2
#define MR_JOYSTICK_LEFT				4
#define MR_JOYSTICK_RIGHT				8
#define MR_JOYSTICK_FIRE				64

#define MR_TILE_WIDTH_IN_PIXEL			(8>>(MR_MULTICOLOR))
#define MR_TILE_WIDTH_FACTOR			(3-(MR_MULTICOLOR))
#define MR_TILE_BACKGROUND				3
#define MR_TILE_COLOR0					0
#define MR_TILE_COLOR1					1
#define MR_TILE_COLOR2					2

#define MR_SM(_screen)						((mr_mixel*)((0x0800*(_screen))+0x0400))
#define MR_CM(_screen)						((mr_color*)((0x0800*(_screen))))
#define MR_AM(_screen)						((mr_mixel*)((0x0800*(_screen))))
#define MR_TM(_tileset)					((mr_mixel*)((0x0400*(_tileset))))

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
#define MIDRES_STANDALONE_BITBLIT			1
#define MIDRES_STANDALONE_DRAWING			1
#define MIDRES_STANDALONE_DRAWING2			1
#define MIDRES_STANDALONE_SCREEN			1
#define MIDRES_STANDALONE_SCREEN2			1
#define MIDRES_STANDALONE_TILE				1
#define MIDRES_STANDALONE_TILE_PROCESSING	1
#define MIDRES_STANDALONE_TILE_MULTICOLOR	1
#define MIDRES_STANDALONE_KEYBOARD			1
#define MIDRES_STANDALONE_FILE				1
// #define MIDRES_EMBEDDED_FILES				1
#define MIDRES_STANDALONE_MUSIC				1

#include "midres_tile.h"
#include "midres_tile_processing.h"

#endif