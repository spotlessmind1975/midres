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

#ifdef __DEMO_TILE__

/****************************************************************************
 ** INCLUDE SECTION
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"
#include "midres.h"

#include "rawdata.h"

/****************************************************************************
 ** RESIDENT VARIABLES SECTION
 ****************************************************************************/

// All variables defined as global within the resident body of the code are 
// accessible from all modules, both resident and changing ones.

/****************************************************************************
 ** RESIDENT FUNCTIONS SECTION
 ****************************************************************************/

// All the functions defined within the resident body of the code are 
// accessible from all modules, both resident and changing ones.

/****************************************************************************
 ** RESIDENT MAIN FUNCTION
 ****************************************************************************/

#ifdef __CBM__

#define		TILE_START			0

#elif __ATARI__

#define		TILE_START			8

#else

#define		TILE_START			16

#endif

// Tile with the ground dawing in "tiles.bin" (currently not used)
#define		TILE_GROUND			TILE_START

// Tile with the first frame of rotating wheel
#define		TILE_WHEEL_BEGIN	TILE_START+1

// Tile with the last frame of rotating wheel
#define		TILE_WHEEL_END		TILE_START+4

// Tile with the dawing of a small drop in "tiles.bin"
#define		TILE_DROP			TILE_START+5

// Tiles with the dawing of a large drop in "tiles.bin" (currently not used)
#define		TILE_BDROP_TL		TILE_START+6
#define		TILE_BDROP_TR		TILE_START+7
#define		TILE_BDROP_BL		TILE_START+8
#define		TILE_BDROP_BR		TILE_START+9

// Number of tiles available on "tiles.bin"
#define		TILE_COUNT			10

// First tile for drop movement: it starts from the last
// used, and move forward of one tile.
#define		TILE_DROP_VERTICAL	TILE_START + TILE_COUNT + 1

// Number of drops to animate -- currently, there is
// space for one drop for column. The first and last
// column is occupied by "in place" animation of wheel.
#define		DROP_COUNT			( MR_SCREEN_WIDTH - 2 )

// Vertical position of each drop on the screen.
mr_tile_position drops[DROP_COUNT];

// Color assigned to each drop on the screen.
mr_color dropsColor[DROP_COUNT];

// Index of the animation of the frame.
mr_tile wheels[4];

unsigned char* mr_translate_file_user(mr_file _file) {

	switch (_file) {
		case FILE_TILES_BIN:
			return "tiles.bin";
		case FILE_ZELTILES_BIN:
			return "zeltiles.bin";
	}

	return 0;

}

void demo_tile() {

	// Used as iterator.
	mr_tileset i;

	// Initialize random number generator
	srand(clock());

	// Initialize graphical subsystem
	mr_init();

	// Clear screen bitmap.
	mr_clearv();

	// Load the tiles from the disk on the first tileset space.
	// We load all tiles (TILE_COUNT) from first position (0).
	mr_tileset_load_file(FILE_TILES_BIN, MR_TILESET_0, TILE_START, TILE_COUNT);

	// Precalculate the vertical movement for drop tile.
	mr_tile_prepare_vertical(MR_TILESET_0, TILE_DROP, TILE_DROP_VERTICAL);

	// Enable custom font.
	mr_tileset_visible(MR_TILESET_0);

	// Initialize the frames for animation of each wheel.
	for (i = 0; i < 4; ++i) {
		wheels[i] = TILE_WHEEL_BEGIN + i;
	}

	// Initialize the vertical position of each drop, and its color.
	for (i = 0; i < DROP_COUNT; ++i) {
		drops[i] = ( rand() & 0x1f ) + ( rand() & 0x1f );
		dropsColor[i] = (rand() & 0x03)+(rand() & 0x03);
	}

	// Endless loop...
	while (1) {

		mr_start_frame();

		// Update frames for wheels.
		for (i = 0; i < 4; ++i) {
			++wheels[i];
			if (wheels[i] >= TILE_WHEEL_END) {
				wheels[i] = TILE_WHEEL_BEGIN;
			}
		}

		// Draw rotating wheels at the angles of the screen.
		mr_puttile(MR_SCREEN_DEFAULT, 0, 0, wheels[0], MR_COLOR_RED);
		mr_puttile(MR_SCREEN_DEFAULT, MR_SCREEN_WIDTH-1, 0, wheels[1], MR_COLOR_RED);
		mr_puttile(MR_SCREEN_DEFAULT, 0, MR_SCREEN_HEIGHT-1, wheels[2], MR_COLOR_RED);
		mr_puttile(MR_SCREEN_DEFAULT, MR_SCREEN_WIDTH-1, MR_SCREEN_HEIGHT-1, wheels[3], MR_COLOR_RED);

		// For each drop...
		for (i = 1; i < DROP_COUNT; ++i) {

			// Move drop on the screen.
			mr_tile_moveto_vertical(MR_SCREEN_DEFAULT, i*8, drops[i], TILE_DROP_VERTICAL, MR_COLOR_WHITE+dropsColor[i]);

			// Update drop's vertical position. If the drop arrives to
			// the bottom of the screen, let's move it on the top.
			++drops[i];
			if (drops[i] >= MR_SCREEN_HEIGHT * 8 ) {
				drops[i] = 0;
			}
		}

		mr_end_frame(0);

	}

}

#endif
