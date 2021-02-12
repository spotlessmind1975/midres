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

#ifdef __TUTORIAL_MCTILE__

#include <stdio.h>

#include "midres.h"
#include "tutorial_mctile.h"

#define		TILE_GHOST_MOVING		TILE_START + TILE_COUNT + 1

void tutorial_mctile() {

	mr_tile_position x = 0;
	
	// Initialize graphical subsystem
	mr_init_multicolor();

	// Clear screen bitmap.
	mr_clear_bitmap(MR_SCREEN_DEFAULT);

	mr_tile_setcolor(MR_TILE_BACKGROUND, TILE_COLOR0);
	mr_tile_setcolor(MR_TILE_COLOR0, TILE_COLOR1);
	mr_tile_setcolor(MR_TILE_COLOR1, TILE_COLOR2);
	mr_tile_setcolor(MR_TILE_COLOR2, TILE_COLOR3);

	// Load the tiles from the disk on the first tileset space.
	// We load all tiles (TILE_COUNT) from first position (0).
	mr_tileset_load("mctile.bin", MR_TILESET_0, TILE_START, TILE_COUNT);

	// Precalculate the horizontal movement
	mr_tile_prepare_horizontal_extended(MR_TILESET_0, TILE_GHOST, TILE_GHOST_WIDTH, TILE_GHOST_HEIGHT, TILE_GHOST_MOVING);

	// Enable custom font.
	mr_tileset_visible(MR_TILESET_0);

	// Endless loop...
	while (1) {

		mr_start_frame();

		++x;

		// Move drop on the screen.
		mr_tile_moveto_horizontal_extended(MR_SCREEN_DEFAULT, x, MR_SCREEN_HEIGHT >> 1, TILE_GHOST_MOVING, TILE_GHOST_WIDTH, TILE_GHOST_HEIGHT, MR_COLOR_LIGHT_BLUE);

		if ((x >> MR_TILE_WIDTH_FACTOR) > MR_SCREEN_WIDTH) {
			x = -TILE_GHOST_WIDTH*MR_TILE_WIDTH_IN_PIXEL;
		}

		mr_end_frame(4);

	}

}

#endif
