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
#elif __ATARI__
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

#define FILENAME_TILES_BIN			"zztiles.bin"
#define FILENAME_INTRO_MPIC			"zzintro.pic"

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

// Flag to know if player wins.
unsigned char playerWin = 0;

// Used as temporary indexes.
mr_position i = 0;
mr_position j = 0;

// Position of the airplane on the screen.
mr_tile_position airplane_x, airplane_y;

// Position of the bomb on the screen.
mr_tile_position bomb_x, bomb_y;

// Which building we are refreshing?
mr_position building_index = 0;

// Store the score of the player
mr_position score[4] = { 0, 0, 0, 0 };

// Store the bombs dropped by the player
mr_position drops[4] = { 0, 0, 0, 0 };

// Store the level
mr_position level[2] = { 1, 0 };

// Draw the _number-nth building of _height bricks, using
// the a specific tile for wall and roof.
void draw_building(mr_position _number, mr_position _height, mr_tile _tile_wall, mr_tile _tile_roof) {

	// Starting position on the screen for that buildings.
	mr_position start = _number * BUILDINGS_WIDTH;

	// Draw the roof.
	mr_puttilev(start, _height, _tile_roof, MR_COLOR_LIGHT_RED);
	mr_puttilev(start+1, _height, _tile_roof +1, MR_COLOR_RED);
	mr_puttilev(start+2, _height, _tile_roof + 2, MR_COLOR_RED);

	// Draw the walls.
	mr_vtilesv(
		start, 
		_height+1,
		MR_SCREEN_HEIGHT - 1,
		_tile_wall,
		MR_COLOR_LIGHT_GREY);

	mr_vtilesv(
		start +1,
		_height+1,
		MR_SCREEN_HEIGHT - 1,
		_tile_wall + 1,
		MR_COLOR_GREY);

	mr_vtilesv(
		start +2,
		_height+1,
		MR_SCREEN_HEIGHT - 1,
		_tile_wall + 2,
		MR_COLOR_GREY);

}

// Let's prepare everything we need, from graphical point of view.
void prepare_graphics() {

	// Load the tileset from disk.
	mr_tileset_load(FILENAME_TILES_BIN, MR_TILESET_0, TILE_START, TILE_COUNT);

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

	// Set to zero the number of building destroyed.
	buildingsDestroyedCount = 0;

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
		buildingHeights[i] = 5 + (rand() & 0x0f) + (rand() & 0x03) - (level[0] + 10 * level[1]);

		if (buildingHeights[i] > MR_SCREEN_HEIGHT ) {
			buildingHeights[i] = MR_SCREEN_HEIGHT - 5;
		} else if (buildingHeights[i] < 3) {
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
		// There is only one tile per element: use it.
		w = TILE_WALL1;
		r = TILE_ROOF1;
#endif

		// Let's draw the building.
		draw_building(i, buildingHeights[i], w, r);
	}

}

// Increase the given multibyte (max size = 4 bytes) by 1.
void increase(unsigned char *points) {
	mr_position carry = 1, index = 0;
	while (carry) {
		carry = 0;
		++points[index];
		if (points[index] > 9) {
			points[index] = 0;
			carry = 1;
		}
		++index;
		if (index > 4) {
			break;
		}
	}
}

// Decrease the given multibyte (max size = 4 bytes) by 1,
// and return 1 if the value is zero.
unsigned char decrease(unsigned char* points) {
	mr_position carry = 1, index = 0, zero = 0;
	while (carry) {
		zero = 0;
		carry = 0;
		--points[index];
		if (points[index] == 255) {
			points[index] = 9;
			carry = 1;
			zero = 0;
		}
		else if (points[index] == 0 && index == 0) {
			zero = 1;
		}
		++index;
		if (index > 4) {
			break;
		}
	}
	return zero;
}

// Returns 1 if the multibyte value is 0.
unsigned char zero(unsigned char* _points, unsigned char _size) {
	mr_position i;
	for (i = 0; i < _size; ++i) {
		if (_points[i] != 0) return 0;
	}
	return 1;
}

// Draw the number of bombs dropped at specific position.
void draw_bombs(mr_position _x, mr_position _y) {
	mr_puttilev(_x, _y, TILE_BOMB, MR_COLOR_WHITE);
	for (i = 0; i < 4; ++i) {
		mr_puttilev(_x + 1 + (4 - i), _y, TILE_DIGIT0 + drops[i], MR_COLOR_WHITE);
	}
}

// Draw the score at specific position.
void draw_score(mr_position _x, mr_position _y) {
	mr_puttilev(_x, _y, TILE_SCORE, MR_COLOR_WHITE);
	for (i = 0; i < 4; ++i) {
		mr_puttilev(_x + 1 + (4 - i), _y, TILE_DIGIT0 + score[i], MR_COLOR_WHITE);
	}
}

// Draw the current level at specific position.
void draw_level(mr_position _y) {
	mr_position ofs = ( MR_SCREEN_WIDTH - ((TILE_LEVEL_WIDTH + 3)) ) >> 1;
	mr_puttilesv( ofs, _y, TILE_LEVEL, TILE_LEVEL_WIDTH, MR_COLOR_WHITE);
	for (i = 0; i < 2; ++i) {
		mr_puttilev(ofs + TILE_LEVEL_WIDTH + 3 - i, _y, TILE_DIGIT0 + level[i], MR_COLOR_WHITE);
	}
}

// This is the main gameloop. We repeat the loop until an
// end condition is met.
void gameloop() {

	// When starts, the player has not won.
	playerWin = 0;

	// The bomb has not dropped
	bomb_x = 0;
	bomb_y = 0;

	// Update level on the screen.
	draw_level(0);

	// Update bomb number on the screen.
	draw_bombs(0, 0);

	// Update score on the screen.
	draw_score(MR_SCREEN_WIDTH-6,0);

	// Set the airplane starting position: it is on the top
	// of the screen and OUTSIDE the screen itself.
	airplane_x = - ( TILE_AIRPLANE_STATIC_WIDTH * 8 );
	airplane_y = 8;

	// Endless loop...
	while (1) {

		mr_start_frame();

		// First of all, animate the airplane. This means that
		// we have to move and draw the airplane orizontally.
		// If buildings are all destroyed, move faster!
		if (buildingsDestroyedCount >= BUILDINGS_COUNT) {
			mr_cleartilev(( airplane_x >> 3)-1, airplane_y >> 3);
			mr_cleartilev((airplane_x >> 3), airplane_y >> 3);
			mr_cleartilev((airplane_x >> 3) +1, airplane_y >> 3);
			if (TILE_AIRPLANE_STATIC_HEIGHT > 1) {
				mr_cleartilev((airplane_x >> 3) -1, (airplane_y >> 3) + 1);
				mr_cleartilev((airplane_x >> 3), (airplane_y >> 3) + 1);
				mr_cleartilev((airplane_x >> 3), (airplane_y >> 3) + 1);
			}
			airplane_x += 10;
		} else {
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
				playerWin = 1;
				break;
			}
		}

		// Draw the airplane.
		mr_tile_moveto_horizontal_extendedv(
				airplane_x, airplane_y, 
				TILE_MOVING_AIRPLANE, 
				TILE_AIRPLANE_STATIC_WIDTH, TILE_AIRPLANE_STATIC_HEIGHT,
				MR_COLOR_YELLOW);

		if (buildingsDestroyedCount < BUILDINGS_COUNT) {
			// If the airplane ordinate is greater or equal than 16, it means that
			// it could touch buildings.
			if (airplane_y >= 16) {

				// Retrieve the building's index by ascissa.
				mr_position building = buildingMap[(airplane_x >> 3) + TILE_AIRPLANE_STATIC_WIDTH];

				// Retrieve the building's height.
				mr_position building_height = buildingHeights[building];

				// Does the airplane hit a building?
				if (((airplane_y >> 3) + ( TILE_AIRPLANE_STATIC_HEIGHT - 1 ) ) >= building_height) {

					// Yes, so: player has lost!
					playerWin = 0;

					break;
				}
			}
		}

		// If the bomb ordinate is greater than 0, it means that
		// it has been dropped. 
		if (bomb_y > 0) {

			// Retrieve the building's index by ascissa.
			mr_position building = buildingMap[(bomb_x >> 3)];

			// Retrieve the building's height.
			mr_position building_height = buildingHeights[building];

			// Does the bomb hit a building?
			if (( (bomb_y >>3) + 1 ) >= building_height) {

				mr_sound_stop();

				// Clear the bomb.
				mr_cleartilev((bomb_x >> 3), (bomb_y >> 3));
				mr_cleartilev((bomb_x >> 3), (bomb_y >> 3) + 1);

				// Reset its position.
				bomb_y = 0;

				// If the building is not destroyed...
				if (buildingHeights[building] < MR_SCREEN_HEIGHT - 1) {

					// Calculate the number of floors destroyed (1..4).
					mr_position floors_destroyed = 1 + (rand() & 0x3);

					// Update the current building's heigth.
					buildingHeights[building] += floors_destroyed;

					// Remove from 1 to 4 floors.
					for (; floors_destroyed != 0; --floors_destroyed, ++building_height) {

						// Avoid to go over the video buffer!
						if (building_height < MR_SCREEN_HEIGHT) {
							mr_cleartilev(
								building * BUILDINGS_WIDTH,
								building_height);
							mr_cleartilev(
								building * BUILDINGS_WIDTH + 1,
								building_height);
							mr_cleartilev(
								building * BUILDINGS_WIDTH + 2,
								building_height);
						}

						// Increase the score, accordingly.
						increase(score);

					}

					// If the building has been destroyed
					if (buildingHeights[building] >= MR_SCREEN_HEIGHT - 1) {
						// ... update heigth and increase the 
						// count of destroyed building.
						buildingHeights[building] = MR_SCREEN_HEIGHT - 1;
						buildingsDestroyedCount++;
					}

					// Activate the flaming effect!
					buildingFlaming[buildingMap[(bomb_x >> 3)]] = 1 + (rand() & 0x1);

					// Update bomb drops' count.
					draw_bombs(0, 0);

					// Update score.
					draw_score(MR_SCREEN_WIDTH - 6, 0);

				}

			} else if ( ( bomb_y >> 3 ) < MR_SCREEN_HEIGHT ) {

				mr_sound_change(( MR_SCREEN_HEIGHT * 8 - bomb_y ) * 16);

				// Continue to move the bomb vertically (by two steps).
				bomb_y += 2;
				mr_tile_moveto_verticalv(
						bomb_x, bomb_y, 
						TILE_MOVING_BOMB, 
						MR_COLOR_YELLOW);

				// Disable the bomb if it reaches the bottom of the screen.
				if (bomb_y >= MR_SCREEN_HEIGHT * 8) {
					bomb_y = 0;
				}

			} else {

				mr_sound_stop();

				// Disable the bomb.
				bomb_y = 0;
			}

		}

		// If the player did not drop a bomb, 
		// let's see if it has pressed a key.
		if ( (bomb_y == 0) && (airplane_x>0) && ((airplane_x>>3)<(MR_SCREEN_WIDTH-TILE_AIRPLANE_STATIC_WIDTH)) ) {
			if (mr_key_pressed()) {

				// DROP THE BOMB!
				bomb_y = airplane_y + TILE_AIRPLANE_STATIC_HEIGHT * 8 + 1 ;
				bomb_x = airplane_x + (8 * (TILE_AIRPLANE_STATIC_WIDTH >> 1));

				// Increase the 
				increase(drops);
				draw_bombs(0, 0);
				draw_score(MR_SCREEN_WIDTH - 6, 0);

				mr_sound_start(0);

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
			mr_puttilev(building_index * BUILDINGS_WIDTH, h, t, c1);
			mr_puttilev(building_index * BUILDINGS_WIDTH + 1, h, t + 1, c2);
			mr_puttilev(building_index * BUILDINGS_WIDTH + 2, h, t + 2, c3);

			// Increase the animation frame.
			++buildingFlaming[building_index];
			if (buildingFlaming[building_index] > 3) {
				buildingFlaming[building_index] = 1;
			}
		}

		mr_end_frame(1);

	}

	mr_sound_stop();

}

void game_air_attack() {

	// Initialize random number generator
	srand(clock());

	// Initialize graphical subsystem
	mr_init();

	// Clear screen bitmap.
	mr_clear_bitmapv();

#ifdef __ATARI__

	// Load compressed screen on the auxiliary space
	mr_load(FILENAME_INTRO_MPIC, MR_SCREEN_DEFAULT);

	mr_wait(2);

#else

	// Load compressed screen on the auxiliary space
	mr_load(FILENAME_INTRO_MPIC, MR_AUX_DEFAULT);

	// Show titles.
	mr_uncompressv(MR_AUX_DEFAULT);

#endif

	// Prepare graphics (it can take some time).
	prepare_graphics();

	// Endless loop...
	while (1) {

		// Clear the screen.
		mr_clear_bitmapv();

		// Show the "press any key" interstitial screen:
		// this screen is available only where the relative
		// "press any key" tile is available.

#ifdef TILE_PRESSANYKEY

		while (!mr_key_pressed()) {
			i = i ^ 1;
			if (i == 0) {
				mr_puttilesv((MR_SCREEN_WIDTH - TILE_PRESSANYKEY_WIDTH) >> 1, (MR_SCREEN_HEIGHT - 1) >> 1, TILE_PRESSANYKEY, TILE_PRESSANYKEY_WIDTH, MR_COLOR_WHITE);
			}
			else {
				for (j = 0; j < TILE_PRESSANYKEY_WIDTH; ++j) {
					mr_cleartilev(((MR_SCREEN_WIDTH - TILE_PRESSANYKEY_WIDTH) >> 1) + j, (MR_SCREEN_HEIGHT - 1) >> 1);
				}
			}
			mr_wait_jiffies(4);
			mr_wait_vbl();
		}

#else

		mr_wait(2);

#endif

		// Repeat the loop while player win.
		do {

			// Clear the screen.
			mr_clear_bitmapv();

			// Show level
			draw_level(MR_SCREEN_HEIGHT >> 1);

			mr_wait(2);

			// Clear the screen.
			mr_clear_bitmapv();

			// Draw the playfield
			draw_random_buildings();

			// Play the game.
			gameloop();

			if (playerWin) {
				increase(level);

				// Clear the screen.
				mr_clear_bitmapv();

				// Show bombs
				draw_bombs((MR_SCREEN_WIDTH >> 1) - 3, (MR_SCREEN_HEIGHT >> 1) - 1);

				// Show score
				draw_score((MR_SCREEN_WIDTH >> 1) - 3, (MR_SCREEN_HEIGHT >> 1) + 1);

				while (zero(drops, 4)==0) {

					decrease(drops);

					// Show bombs
					draw_bombs((MR_SCREEN_WIDTH >> 1) - 3, (MR_SCREEN_HEIGHT >> 1) - 1);

					// Show score
					draw_score((MR_SCREEN_WIDTH >> 1) - 3, (MR_SCREEN_HEIGHT >> 1) + 1);

					if (zero(score,4) == 0) {

						decrease(score);

						// Show score
						draw_score((MR_SCREEN_WIDTH >> 1) - 3, (MR_SCREEN_HEIGHT >> 1) + 1);

						mr_wait_jiffies(10);

					} else {
						break;
					}

				}

				drops[0] = 0; drops[1] = 0;

				mr_wait(2);

			}

		} while (playerWin);

		// Show "GAME OVER" only if the relative tile is present.
#ifdef TILE_GAMEOVER

		mr_clear_bitmapv();

		// Show score
		draw_score(( MR_SCREEN_WIDTH >> 1) - 3, (MR_SCREEN_HEIGHT >> 1) + 2);

		i = 0;
		while (!mr_key_pressed()) {
			i = i ^ 1;
			if (i == 0) {
				mr_puttilesv((MR_SCREEN_WIDTH - TILE_GAMEOVER_WIDTH) >> 1, (MR_SCREEN_HEIGHT - 1) >> 1, TILE_GAMEOVER, TILE_GAMEOVER_WIDTH, MR_COLOR_WHITE);
			}
			else {
				for (j = 0; j < TILE_GAMEOVER_WIDTH; ++j) {
					mr_cleartilev(((MR_SCREEN_WIDTH - TILE_GAMEOVER_WIDTH) >> 1) + j, (MR_SCREEN_HEIGHT - 1) >> 1);
				}
			}
			mr_wait_vbl();
		}

#endif

	}


}

#endif
