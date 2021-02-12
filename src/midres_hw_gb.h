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

#ifndef _MIDRES_HW_GB_H_
#define _MIDRES_HW_GB_H_

#define MR_RENDERED_MIXELS MR_RENDERED_MIXELS_GB

#define MR_SCREEN_WIDTH				20
#define MR_SCREEN_HEIGHT			18
#define MR_SCREEN_ROW_WIDTH			32

#define MR_SCREEN_RAM_SIZE			0x0400

// List of available screens:
#define MR_SCREEN_0					0
#define MR_SCREEN_1					1
#define MR_SCREEN_COUNT				2
#define MR_SCREEN_DEFAULT			MR_SCREEN_0

// The four screens used for double buffering
#define	MR_SCREEN_DB1				MR_SCREEN_0
#define	MR_SCREEN_DB2				MR_SCREEN_1

// List of available tilesets:
#define MR_TILESET_0					0
#define MR_TILESET_1					1
#define MR_TILESET_COUNT				2
#define MR_TILESET_DEFAULT				MR_TILESET_0
#define MR_TILESET_TILE_COUNT			256

// List of available areas for bit blits:
#define MR_AUX_0						1
#define MR_AUX_1						0
#define MR_AUX_COUNT					2
#define MR_AUX_DEFAULT					MR_AUX_0

// Offset for correct brightness.
#define MR_COLOR_LOWBRIGHTNESS			0
#define MR_COLOR_BRIGHTNESS				0
#define MR_COLOR_HIGHBRIGHTNESS			0

// Default palette.
#define MR_COLOR_BLACK					3
#define MR_COLOR_WHITE					0
#define MR_COLOR_RED					1
#define MR_COLOR_CYAN					2
#define MR_COLOR_VIOLET					3
#define MR_COLOR_GREEN					1
#define MR_COLOR_BLUE					2
#define MR_COLOR_YELLOW					3
#define MR_COLOR_ORANGE					1
#define MR_COLOR_BROWN					2
#define MR_COLOR_LIGHT_RED				3
#define MR_COLOR_DARK_GREY				1
#define MR_COLOR_GREY					2
#define MR_COLOR_LIGHT_GREEN			3
#define MR_COLOR_LIGHT_BLUE				1
#define MR_COLOR_LIGHT_GREY				2

#define MR_COLOR_DARK_BLUE				3
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

#define MR_NEXT_COLOR( c )				( ( ( ( c & 0x03 ) + 1 ) & 0x03 ) )

#define MR_COLOR_COUNT					3

#define MR_KEY_NONE						64

#define MR_JOYSTICK_0					0
#define MR_JOYSTICK_1					1
#define MR_JOYSTICK_COUNT				2
#define MR_JOYSTICK_UP					1
#define MR_JOYSTICK_DOWN				2
#define MR_JOYSTICK_LEFT				4
#define MR_JOYSTICK_RIGHT				8
#define MR_JOYSTICK_FIRE				16

#define MR_TILE_WIDTH_IN_PIXEL			(8)
#define MR_TILE_WIDTH_FACTOR			(3)
#define MR_TILE_BACKGROUND				3
#define MR_TILE_COLOR0					0
#define MR_TILE_COLOR1					1
#define MR_TILE_COLOR2					2

// 3	BG Tile Map Display Select	0=9800-9BFF, 1=9C00-9FFF
#define MR_SM(_screen)					((mr_mixel*)(0x9800+(0x0400*(_screen))))
#define MR_CM(_screen)					0x0000*_screen
#define MR_AM(_screen)					((mr_mixel*)(0x9800+(0x0400*(_screen))))
#define MR_TM(_tileset)					((mr_mixel*)(0x8000+(0x0800*(_tileset))))

#define MR_WRITE_TILE_LUMINANCE(_screen, _offset, _tile) \
		_screen[(_offset)] = (_tile);

void MR_DI();
void MR_EI();

#define MR_WAIT_VRAM() \
		while((*((unsigned char*)0xff41)& 0x03) != 0x1 ) { \
			unsigned char port = *((unsigned char*)0xff41); \
		}

#define MR_WRITE_TILE(_screen, _colormap, _offset, _tile, _color) \
		MR_DI(); \
		MR_WAIT_VRAM(); \
		_screen[(_offset)] = (_tile); \
		MR_EI();

#define MR_PROTECTED_ACCESS_VRAM(f) \
		{ \
			MR_DI(); \
			MR_WAIT_VRAM(); \
			f; \
			MR_EI(); \
		}

#define MR_READ_TILE(_screen, _offset) _screen[(_offset)]
#define MR_READ_TILE_COLOR(_colormap, _offset) _colormap[(_offset)]

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
// #define MIDRES_STANDALONE_FILE				1
#define MIDRES_EMBEDDED_FILES				1
#define MIDRES_STANDALONE_MUSIC				1

#include "midres_tile_hd.h"

#include <time.h>
clock_t clock();

extern unsigned int MIDRES_SOUND_FREQUENCIES_GB[];
#define MIDRES_SOUND_FREQUENCIES MIDRES_SOUND_FREQUENCIES_GB

#endif