#ifdef __GAME_AIR_ATTACK__

/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * RESIDENT MODULE                                                          *
 ****************************************************************************/

/****************************************************************************
 ** INCLUDE SECTION
 ****************************************************************************/

#include <stdio.h>
#include <cc65.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"
#include "midres.h"

 // Number of buildings to draw in a screen (it depends on width)
#define BUILDINGS_COUNT         12

// Height of the root of the buildings (from the top of the screen)
#define BUILDINGS_HEIGHT        5

// Distance (in mixels) between buildings
#define BUILDINGS_DISTANCE      MR_SCREEN_WIDTH / BUILDINGS_COUNT

// Width of a building (in mixels)
#define BUILDINGS_WIDTH         3

// Number of windows on a single building.
#define BUILDINGS_WINDOWS       ( MR_SCREEN_HEIGHT*2 - BUILDINGS_HEIGHT ) / (BUILDINGS_WINDOW_SIZE*3)

unsigned char buildingHeights[BUILDINGS_COUNT];

#include "game_air_attack_tiles.h"

#define TILE_SPARE_AREA			TILE_COUNT + 1
#define TILE_MOVING_AIRPLANE	TILE_SPARE_AREA
#define TILE_MOVING_BOMB		TILE_MOVING_AIRPLANE + mr_calculate_prepared_tile_size( TILE_AIRPLANE_WIDTH, TILE_AIRPLANE_HEIGHT )

// Draw the _number-nth building.
void draw_building(mr_position _number, mr_position _height, mr_tile _tile_wall, mr_tile _tile_roof) {

	// Used as index
	mr_position j = 0;
	mr_position start = _number * BUILDINGS_DISTANCE;

	mr_puttile(MR_SCREEN_DEFAULT, start, _height - 1, _tile_roof, MR_COLOR_LIGHT_RED);
	mr_puttile(MR_SCREEN_DEFAULT, start+1, _height - 1, _tile_roof +1, MR_COLOR_RED);
	mr_puttile(MR_SCREEN_DEFAULT, start+2, _height-1, _tile_roof + 2, MR_COLOR_RED);

	mr_vtiles(MR_SCREEN_DEFAULT,
		start, 
		_height,
		MR_SCREEN_HEIGHT - 1,
		_tile_wall,
		MR_COLOR_LIGHT_GREY);

	mr_vtiles(MR_SCREEN_DEFAULT,
		start +1,
		_height,
		MR_SCREEN_HEIGHT - 1,
		_tile_wall + 1,
		MR_COLOR_GREY);

	mr_vtiles(MR_SCREEN_DEFAULT,
		start +2,
		_height,
		MR_SCREEN_HEIGHT - 1,
		_tile_wall + 2,
		MR_COLOR_GREY);

}

void titles() {

}

void playfield() {

}

int x, y, i;
mr_tile_position bx, by;

void game_air_attack() {

	// Initialize random number generator
	srand(clock());

	// Initialize graphical subsystem
	mr_init();

	// Clear screen bitmap.
	mr_clear_bitmap(MR_SCREEN_DEFAULT);

	// Load the tiles from the disk on the first tileset space.
	// We load all tiles (TILE_COUNT) from first position (0).
	mr_tileset_load("aatiles.bin", MR_TILESET_0, TILE_START, TILE_COUNT);

	mr_tile_prepare_horizontal_extended(MR_TILESET_0, TILE_AIRPLANE, TILE_AIRPLANE_WIDTH, TILE_AIRPLANE_HEIGHT, TILE_MOVING_AIRPLANE);
	mr_tile_prepare_vertical(MR_TILESET_0, TILE_BOMB, TILE_MOVING_BOMB);

	mr_tileset_visible(MR_TILESET_0);

	for (i = 0; i < BUILDINGS_COUNT; ++i) {
		mr_tile w = 0;
		mr_tile r = 0;
		buildingHeights[i] = 4 + (rand() & 0x0f) + (rand() & 0x03);
		switch (rand()&0x03) {
			case 0:
				w = TILE_WALL1;
				r = TILE_ROOF1;
				break;
			case 1:
				w = TILE_WALL2;
				r = TILE_ROOF2;
				break;
			case 2:
				w = TILE_WALL3;
				r = TILE_ROOF1;
				break;
			case 3:
				w = TILE_WALL3;
				r = TILE_ROOF2;
				break;
		}
		draw_building(i, buildingHeights[i], w, r);
	}

	getchar();

	y = 0;
	x = -4 * 8;

	while (1) {
		mr_tile_moveto_horizontal_extended(MR_SCREEN_DEFAULT, x, y * 8, TILE_MOVING_AIRPLANE, TILE_AIRPLANE_WIDTH, TILE_AIRPLANE_HEIGHT, MR_COLOR_YELLOW + (x & 03));
		if (by > 0) {
			mr_tile_moveto_vertical(MR_SCREEN_DEFAULT, bx, by, TILE_MOVING_BOMB, MR_COLOR_YELLOW);
			++by;
			if (by >= MR_SCREEN_HEIGHT*8) {
				by = 0;
			}
		}
		++x;
		if (x >= MR_SCREEN_WIDTH * 8) {
			++y;
			x = -4*8;
		}
		if (mr_key_pressed()) {
			if (by == 0) {
				by = y + (8*(TILE_AIRPLANE_WIDTH>>1));
				bx = x + (8*(TILE_AIRPLANE_HEIGHT >> 1));
			}
		}

		mr_wait_vbl();
	}

	getchar();

}

#endif
