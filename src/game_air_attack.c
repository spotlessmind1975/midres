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
#ifdef __PLUS4__
#include "game_air_attack_tiles4.h"
#elif __VIC20__
#include "game_air_attack_tiles20.h"
#else
#include "game_air_attack_tiles.h"
#endif

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

#ifdef __VIC20__
	// Number of buildings to draw in a screen (it depends on width)
	#define BUILDINGS_COUNT         7
#else
	// Number of buildings to draw in a screen (it depends on width)
	#define BUILDINGS_COUNT         13
#endif

// Width of a building
#define BUILDINGS_WIDTH         3

#ifdef TILE_AIRPLANE
	#define TILE_AIRPLANE_STATIC		TILE_AIRPLANE
	#define TILE_AIRPLANE_STATIC_WIDTH	TILE_AIRPLANE_WIDTH
	#define TILE_AIRPLANE_STATIC_HEIGHT	TILE_AIRPLANE_HEIGHT
#else
	#define TILE_AIRPLANE_STATIC		TILE_SMALLAIRPLANE
	#define TILE_AIRPLANE_STATIC_WIDTH	TILE_SMALLAIRPLANE_WIDTH
	#define TILE_AIRPLANE_STATIC_HEIGHT	TILE_SMALLAIRPLANE_HEIGHT
#endif

// Starting of scratch area to generate animated tiles
#define TILE_SPARE_AREA			TILE_COUNT + 1
#define TILE_MOVING_AIRPLANE	TILE_SPARE_AREA
#define TILE_MOVING_BOMB		TILE_MOVING_AIRPLANE + mr_calculate_prepared_tile_size( TILE_AIRPLANE_STATIC_WIDTH, TILE_AIRPLANE_STATIC_HEIGHT )

// Height of each building, in tiles.
unsigned char buildingHeights[BUILDINGS_COUNT];

// Hashmap between horizontal position and building.
unsigned char buildingMap[MR_SCREEN_WIDTH];

// Index of animating frame for the roofs' flaming.
unsigned char buildingFlaming[BUILDINGS_COUNT];

// Count the buildings destroyed.
unsigned char buildingsDestroyedCount = 0;

// Used as temporary indexes.
mr_position i = 0;
mr_position j = 0;

// Position of the airplane on the screen.
mr_tile_position airplane_x, airplane_y;

// Position of the bomb on the screen.
mr_tile_position bomb_x, bomb_y;

// Which building has been hit by bomb?
mr_position bomb_building;

// Which building we are refreshing?
mr_position building_index = 0;

// Store the score of the player
mr_position score[4];

// Store the bombs dropped by the player
mr_position drops[4];

// Store the level
mr_position level[2] = { 1, 0 };

// Draw the _number-nth building of _height bricks, using
// the a specific tile for wall and roof.
void draw_building(mr_position _number, mr_position _height, mr_tile _tile_wall, mr_tile _tile_roof) {

	// Starting position on the screen for that buildings.
	mr_position start = _number * BUILDINGS_WIDTH;

	// Draw the roof.
	mr_puttile(MR_SCREEN_DEFAULT, start, _height, _tile_roof, MR_COLOR_LIGHT_RED);
	mr_puttile(MR_SCREEN_DEFAULT, start+1, _height, _tile_roof +1, MR_COLOR_RED);
	mr_puttile(MR_SCREEN_DEFAULT, start+2, _height, _tile_roof + 2, MR_COLOR_RED);

	// Draw the walls.
	mr_vtiles(MR_SCREEN_DEFAULT,
		start, 
		_height+1,
		MR_SCREEN_HEIGHT - 1,
		_tile_wall,
		MR_COLOR_LIGHT_GREY);

	mr_vtiles(MR_SCREEN_DEFAULT,
		start +1,
		_height+1,
		MR_SCREEN_HEIGHT - 1,
		_tile_wall + 1,
		MR_COLOR_GREY);

	mr_vtiles(MR_SCREEN_DEFAULT,
		start +2,
		_height+1,
		MR_SCREEN_HEIGHT - 1,
		_tile_wall + 2,
		MR_COLOR_GREY);

}

// Let's prepare everything we need, from graphical point of view.
void prepare_graphics() {

	// Load the tileset from disk.
	mr_tileset_load("aatiles.bin", MR_TILESET_0, TILE_START, TILE_COUNT);

	// Prepare animation for airplane.
	mr_tile_prepare_horizontal_extended(MR_TILESET_0, TILE_AIRPLANE_STATIC, TILE_AIRPLANE_STATIC_WIDTH, TILE_AIRPLANE_STATIC_HEIGHT, TILE_MOVING_AIRPLANE);

	// Prepare animation for the bomb.
	mr_tile_prepare_vertical(MR_TILESET_0, TILE_BOMB, TILE_MOVING_BOMB);

	// Replace the "empty" tile of midres with the one redefined.
	RENDERED_MIXELS[0] = TILE_EMPTY;

	// Enable the custom tileset for viewing.
	mr_tileset_visible(MR_TILESET_0);
	
}

// This method will draw a set of random buildings on the playfield, and
// it will initialize the relative game status variables.
void draw_random_buildings() {

	// Used as temporary tiles.
	mr_tile w = 0;
	mr_tile r = 0;

	// For each building...
	for (i = 0; i < BUILDINGS_COUNT; ++i) {

		// Initialize the hashmap: associate the horizontal
		// position of the three bricks of the wall to the
		// index of the building.
		for (j = 0; j < 3; ++j) {
			buildingMap[i * BUILDINGS_WIDTH + j] = i;
		}

		// No flames over the roof!
		buildingFlaming[i] = 0;

		// Generate a random numer for the height. In order
		// to avoid the use of modulus or subdivision,
		// we use the bitmask tecnique, by splitting the
		// number generation in a sum of smaller random numbers,
		// obtained by masking the given result for the call.
		// The offset is given by the level number.
		buildingHeights[i] = (rand() & 0x0f) + (rand() & 0x03) - (level[0] + 10 * level[1]);

		// The maximum height of a building is MR_SCREEN_HEIGHT - 3!
		if (buildingHeights[i] > MR_SCREEN_HEIGHT || buildingHeights[i] < 3 ) {
			buildingHeights[i] = 3;
		}

#ifdef TILE_WALL2
		// Let's choose the kind of wall and roof.
		// There are 3 kind of walls and 2 kind of roof.
		// So, by generating a random number between 0 and 6,
		// we have to assign the same configuration to the first.
		switch (rand() & 0x07) {
			case 0: case 6:
				w = TILE_WALL1;
				r = TILE_ROOF1;
				break;
			case 1: default:
				w = TILE_WALL1;
				r = TILE_ROOF2;
				break;
			case 2:
				w = TILE_WALL2;
				r = TILE_ROOF1;
				break;
			case 3:
				w = TILE_WALL2;
				r = TILE_ROOF2;
				break;
			case 4:
				w = TILE_WALL3;
				r = TILE_ROOF1;
				break;
			case 5:
				w = TILE_WALL3;
				r = TILE_ROOF2;
				break;
		}
#else
		w = TILE_WALL1;
		r = TILE_ROOF1;
#endif
		// Let's draw the building.
		draw_building(i, buildingHeights[i], w, r);
	}

}

void increase(mr_position *points) {
	mr_position carry = 1, index = 0;
	while (carry) {
		carry = 0;
		++points[index];
		if (points[index] > 9) {
			points[index] = 0;
			carry = 1;
		}
		++index;
		if (index > 3) {
			break;
		}
	}
}

void draw_scores() {
	mr_puttile(MR_SCREEN_DEFAULT, 0, 0, TILE_BOMB, MR_COLOR_WHITE);
	for (i = 0; i < 4; ++i) {
		mr_puttile(MR_SCREEN_DEFAULT, 1 + ( 4 - i ), 0, TILE_DIGIT0 + drops[i], MR_COLOR_WHITE);
	}

	mr_puttile(MR_SCREEN_DEFAULT, MR_SCREEN_WIDTH - 6, 0, TILE_SCORE, MR_COLOR_WHITE);
	for (i = 0; i < 4; ++i) {
		mr_puttile(MR_SCREEN_DEFAULT, MR_SCREEN_WIDTH - 1 - i, 0, TILE_DIGIT0 + score[i], MR_COLOR_WHITE);
	}
}

void draw_level() {
	
	mr_position ofs = ( MR_SCREEN_WIDTH - ((TILE_LEVEL_WIDTH + 3)) ) >> 1;

	mr_puttiles(MR_SCREEN_DEFAULT, ofs, 0, TILE_LEVEL, TILE_LEVEL_WIDTH, MR_COLOR_WHITE);
	for (i = 0; i < 2; ++i) {
		mr_puttile(MR_SCREEN_DEFAULT, ofs + TILE_LEVEL_WIDTH + 3 - i, 0, TILE_DIGIT0 + level[i], MR_COLOR_WHITE);
	}

}

// This is the main gameloop. We repeat the loop until an
// end condition is met.
void gameloop() {

	draw_level( );
	draw_scores();

	// Set the airplane starting position: it is on the top
	// of the screen and OUTSIDE the screen itself.
	airplane_x = - ( TILE_AIRPLANE_STATIC_WIDTH * 8 );
	airplane_y = 8;

	// Endless loop...
	while (1) {

		// First of all, animate the airplane. This means that
		// we have to move and draw the airplane orizontally.
		// If buildings are all destroyed, move faster!
		if (buildingsDestroyedCount >= BUILDINGS_COUNT) {
			mr_cleartile(MR_SCREEN_DEFAULT, ( airplane_x >> 3)-1, airplane_y >> 3);
			mr_cleartile(MR_SCREEN_DEFAULT, (airplane_x >> 3), airplane_y >> 3);
			mr_cleartile(MR_SCREEN_DEFAULT, (airplane_x >> 3) +1, airplane_y >> 3);
			if (TILE_AIRPLANE_STATIC_HEIGHT > 1) {
				mr_cleartile(MR_SCREEN_DEFAULT, (airplane_x >> 3) -1, (airplane_y >> 3) + 1);
				mr_cleartile(MR_SCREEN_DEFAULT, (airplane_x >> 3), (airplane_y >> 3) + 1);
				mr_cleartile(MR_SCREEN_DEFAULT, (airplane_x >> 3), (airplane_y >> 3) + 1);
			}
			airplane_x += 10;
		}
		else {
			// Move to right of a number of pixel related to the current
			// level of difficulty.
			airplane_x += ( level[1] + 1 );
		}

		// If the airplane reached the end of the screen, we have
		// to move it down of one row (and outside of the visible area).
		if (airplane_x >= MR_SCREEN_WIDTH * 8) {
			airplane_y += 8;
			airplane_x = -(TILE_AIRPLANE_STATIC_WIDTH * 8);
			
			// If the airplane has reached the bottom line of the
			// screen, we can just stop: the player wins!
			if (airplane_y >= (MR_SCREEN_HEIGHT - TILE_AIRPLANE_STATIC_HEIGHT) * 8) {
				break;
			}
		}

		// Draw the airplane.
		mr_tile_moveto_horizontal_extended(MR_SCREEN_DEFAULT,
				airplane_x, airplane_y, 
				TILE_MOVING_AIRPLANE, 
				TILE_AIRPLANE_STATIC_WIDTH, TILE_AIRPLANE_STATIC_HEIGHT,
				MR_COLOR_YELLOW);

		// If the airplane ordinate is greater than 16, it means that
		// it could touch buildings.
		if (airplane_y > 16) {
			mr_position building = buildingMap[(airplane_x >> 3)+ TILE_AIRPLANE_STATIC_WIDTH];
			mr_position building_height = buildingHeights[building];

			// Does the airplane hit a building?
			if (((airplane_y >> 3) + 1) >= building_height) {
				break;
			}
		}

		// If the bomb ordinate is greater than 0, it means that
		// it has been dropped. 
		if (bomb_y > 0) {

			mr_position building = buildingMap[(bomb_x >> 3)];
			mr_position building_height = buildingHeights[building];

			// Does the bomb hit a building?
			if (( (bomb_y >>3) + 1 ) >= building_height) {
				
				// Clear the bomb.
				mr_cleartile(MR_SCREEN_DEFAULT, (bomb_x >> 3), (bomb_y >> 3));

				// Reset its position.
				bomb_y = 0;

				if (buildingHeights[building] < MR_SCREEN_HEIGHT - 1) {

					mr_position floors_destroyed = 1 + (rand() & 0x3);

					buildingHeights[building] += floors_destroyed;

					// Remove from 1 to 3 floors.
					for (; floors_destroyed != 0; --floors_destroyed, ++building_height) {
						if (building_height < MR_SCREEN_HEIGHT) {
							mr_cleartile(MR_SCREEN_DEFAULT,
								building * BUILDINGS_WIDTH,
								building_height);
							mr_cleartile(MR_SCREEN_DEFAULT,
								building * BUILDINGS_WIDTH + 1,
								building_height);
							mr_cleartile(MR_SCREEN_DEFAULT,
								building * BUILDINGS_WIDTH + 2,
								building_height);
						}
						increase(score);
					}

					if (buildingHeights[building] >= MR_SCREEN_HEIGHT - 1) {
						buildingHeights[building] = MR_SCREEN_HEIGHT - 1;
						buildingsDestroyedCount++;
					}

					// Activate the flaming effect!
					buildingFlaming[buildingMap[(bomb_x >> 3)]] = 1 + (rand() & 0x1);

					draw_scores();

				}

			} else if ( ( bomb_y >> 3 ) < MR_SCREEN_HEIGHT ) {

				// Continue to move the bomb vertically.
				bomb_y += 2;
				mr_tile_moveto_vertical(MR_SCREEN_DEFAULT,
						bomb_x, bomb_y, 
						TILE_MOVING_BOMB, 
						MR_COLOR_YELLOW);

				// Disable the bomb if it reaches the bottom of the screen.
				if (bomb_y >= MR_SCREEN_HEIGHT * 8) {
					bomb_y = 0;
				}
			} else {
				bomb_y = 0;
			}
		}

		// If the player did not drop a bomb, 
		// let's see if it has pressed a key.
		if (bomb_y == 0) {
			if (mr_key_pressed()) {

				// DROP THE BOMB!
				bomb_y = airplane_y + TILE_AIRPLANE_STATIC_HEIGHT * 8 + 1 ;
				bomb_x = airplane_x + (8 * (TILE_AIRPLANE_STATIC_WIDTH >> 1));
				bomb_building = buildingMap[(bomb_x>>3)];

				increase(drops);
				draw_scores();
			}
		}

		// Increase the index for update other buildings.
		++building_index;
		if (building_index >= BUILDINGS_COUNT) {
			building_index = 0;
		}

		// If the flame effect is enabled for that building...
		if (buildingFlaming[building_index]) {

			// Used to store the frame for the flame effect.
			mr_tile t;

			// Used to store the colors for the flame effect.
			mr_color c1,c2,c3;

			// Used to store the current height of the building.
			mr_position h;
			
#ifdef TILE_FLAME2 
			// Select the flame animation frame.
			switch (buildingFlaming[building_index]) {
				case 1:
					t = TILE_FLAME1;
					break;
				case 2:
					t = TILE_FLAME2;
					break;
				case 3:
					t = TILE_FLAME3;
					break;
			}
#else
			t = TILE_FLAME1;
#endif

			// Select the random colors.
			c1 = rand() & 0x01 ? MR_COLOR_RED : MR_COLOR_LIGHT_RED;
			c2 = rand() & 0x01 ? MR_COLOR_RED : MR_COLOR_LIGHT_RED;
			c3 = rand() & 0x01 ? MR_COLOR_RED : MR_COLOR_LIGHT_RED;

			// Retrieve the height of the building.
			h = buildingHeights[building_index];

			// Draw the flame effect.
			mr_puttile(MR_SCREEN_DEFAULT, building_index * BUILDINGS_WIDTH, h, t, c1);
			mr_puttile(MR_SCREEN_DEFAULT, building_index * BUILDINGS_WIDTH + 1, h, t + 1, c2);
			mr_puttile(MR_SCREEN_DEFAULT, building_index * BUILDINGS_WIDTH + 2, h, t + 2, c3);

			// Increase the animation frame.
			++buildingFlaming[building_index];
			if (buildingFlaming[building_index] >= 3) {
				buildingFlaming[building_index] = 1;
			}
		}

		// Avoid flickering -- wait for VBL before continue
		mr_wait_vbl();
#ifdef __VIC20__
		for (j = 0; j < 200; ++j) { j; };
		for (j = 0; j < 200; ++j) { j; };
#endif
#ifdef __PLUS4__
		for (j = 0; j < 200; ++j) { j; };
		for (j = 0; j < 200; ++j) { j; };
		for (j = 0; j < 200; ++j) { j; };
#endif

	}

}

void titles() {

}

void playfield() {

}

void game_air_attack() {

	// Initialize random number generator
	srand(clock());

	// Initialize graphical subsystem
	mr_init();

	// Clear screen bitmap.
	mr_clear_bitmap(MR_SCREEN_DEFAULT);

	mr_load("aaintro.mpic", MR_AUX_DEFAULT);

	mr_uncompress(MR_AUX_DEFAULT, MR_SCREEN_DEFAULT);

	prepare_graphics();

	mr_clear_bitmap(MR_SCREEN_DEFAULT);

#ifdef TILE_PRESSANYKEY

	while (!mr_key_pressed()) {
		i = i ^ 1;
		if (i == 0) {
			mr_puttiles(MR_SCREEN_DEFAULT, ( MR_SCREEN_WIDTH - TILE_PRESSANYKEY_WIDTH ) >> 1, ( MR_SCREEN_HEIGHT - 1 ) >> 1, TILE_PRESSANYKEY, TILE_PRESSANYKEY_WIDTH, MR_COLOR_WHITE);
		}
		else {
			for (j = 0; j < TILE_PRESSANYKEY_WIDTH; ++j) {
				mr_cleartile(MR_SCREEN_DEFAULT, ((MR_SCREEN_WIDTH - TILE_PRESSANYKEY_WIDTH) >> 1 ) + j, (MR_SCREEN_HEIGHT - 1) >> 1);
			}
		}
		mr_wait_vbl();
	}

	mr_clear_bitmap(MR_SCREEN_DEFAULT);

#endif

	draw_random_buildings();

	gameloop();

	mr_clear_bitmap(MR_SCREEN_DEFAULT);

#ifdef TILE_GAMEOVER

	i = 0;
	while (!mr_key_pressed()) {
		i = i ^ 1;
		if (i == 0) {
			mr_puttiles(MR_SCREEN_DEFAULT, ( MR_SCREEN_WIDTH - TILE_GAMEOVER_WIDTH ) >> 1, ( MR_SCREEN_HEIGHT - 1 ) >> 1, TILE_GAMEOVER, TILE_GAMEOVER_WIDTH, MR_COLOR_WHITE);
		}
		else {
			for (j = 0; j < TILE_GAMEOVER_WIDTH; ++j) {
				mr_cleartile(MR_SCREEN_DEFAULT, ((MR_SCREEN_WIDTH - TILE_GAMEOVER_WIDTH) >> 1 ) + j, (MR_SCREEN_HEIGHT - 1) >> 1);
			}
		}
		mr_wait_vbl();
	}

#endif

}

#endif
