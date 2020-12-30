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

#ifndef _MIDRES_HW_ATMOS_H_
#define _MIDRES_HW_ATMOS_H_

#define MR_RENDERED_MIXELS MR_RENDERED_MIXELS_ATMOS

#define MR_SCREEN_WIDTH					40
#define MR_SCREEN_HEIGHT				28
#define MR_SCREEN_ROW_WIDTH				MR_SCREEN_WIDTH
#define MR_SCREEN_RAM_SIZE				0x0400

  // List of available screens:
#define MR_SCREEN_0						0
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
#define MR_TILESET_0					4
#define MR_TILESET_COUNT				5
#define MR_TILESET_DEFAULT				MR_TILESET_ROM_2
#define MR_TILESET_TILE_COUNT			128

// List of available bitblits:
#define MR_AUX_COUNT					0

#define MR_COLOR_BRIGHTNESS				0

#define	MR_SCREEN_DB1					MR_SCREEN_0
#define	MR_SCREEN_DB2					MR_SCREEN_0

#define MR_COLOR_BLACK					0
#define MR_COLOR_WHITE					0
#define MR_COLOR_RED					0
#define MR_COLOR_CYAN					0
#define MR_COLOR_VIOLET					0
#define MR_COLOR_GREEN					0
#define MR_COLOR_BLUE					0
#define MR_COLOR_YELLOW					0	
#define MR_COLOR_ORANGE					0
#define MR_COLOR_BROWN					0
#define MR_COLOR_LIGHT_RED				0
#define MR_COLOR_DARK_GREY				0
#define MR_COLOR_GREY					0
#define MR_COLOR_LIGHT_GREEN			0
#define MR_COLOR_LIGHT_BLUE				0
#define MR_COLOR_LIGHT_GREY				0	

#define MR_COLOR_MAGENTA				MR_COLOR_RED
#define MR_COLOR_PURPLE					MR_COLOR_VIOLET
#define MR_COLOR_TAN       		        MR_COLOR_BROWN
#define MR_COLOR_OLIVE_GREEN       		MR_COLOR_GREEN
#define MR_COLOR_PEACH       			MR_COLOR_PINK

#define MR_COLOR_COUNT					16
#define MR_TILE_WIDTH_IN_PIXEL			(8>>(MR_MULTICOLOR))
#define MR_TILE_WIDTH_FACTOR			(3-(MR_MULTICOLOR))

#define MR_KEY_NONE						255

#define MR_TILE_BACKGROUND				0
#define MR_TILE_COLOR0					1
#define MR_TILE_COLOR1					2
#define MR_TILE_COLOR2					3

#define MR_SM(_screen)						((mr_mixel*)((0xbb80)))
#define MR_CM(_screen)						((mr_color*)((0x9800)))
#define MR_AM(_screen)						((mr_mixel*)((0x9c00)))
#define MR_TM(_tileset)					((mr_mixel*)((0xb400)))

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
// #define MIDRES_STANDALONE_FILE				1
#define MIDRES_EMBEDDED_FILES				1

#include "midres_tile.h"
#include "midres_tile_processing.h"

#endif