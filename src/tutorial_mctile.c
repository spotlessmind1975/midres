#ifdef __TUTORIAL_MCTILE__

/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Multicolor tiles tutorial                                                *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 ****************************************************************************/

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

		if ((x >> 2) > MR_SCREEN_WIDTH) {
			x = -TILE_GHOST_WIDTH*4;
		}

		mr_end_frame(4);

	}

}

#endif
