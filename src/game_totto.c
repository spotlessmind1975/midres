#ifdef __GAME_TOTTO__

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
#include "main.h"
#include "midres.h"
#include "game_totto_tiles.h"
#include "game_totto_tiles1.h"

/****************************************************************************
 ** RESIDENT VARIABLES SECTION
 ****************************************************************************/

// All variables defined as global within the resident body of the code are 
// accessible from all modules, both resident and changing ones.

// Starting of scratch areas to generate animated tiles
#define TILE_SPARE_AREA			TILE_COUNT + 1

// Scratch area for moving bird (frame 1)
#define TILE_MOVING_BIRD1		TILE_SPARE_AREA

// Scratch area for moving wayside (down)
#define TILE_MOVING_WAYSIDE1	TILE_MOVING_BIRD1 + mr_calculate_prepared_tile_size_vertical( TILE_BIRD1_WIDTH, TILE_BIRD1_HEIGHT )

// Scratch area for moving wayside (up)
#define TILE_MOVING_WAYSIDE2	TILE_MOVING_WAYSIDE1 + mr_calculate_prepared_tile_size_horizontal( TILE_WAYSIDE1_WIDTH, TILE_WAYSIDE1_HEIGHT )

// Scratch area for moving column
#define TILE_MOVING_COLUMN		TILE_MOVING_WAYSIDE2 + mr_calculate_prepared_tile_size_horizontal( TILE_WAYSIDE2_WIDTH, TILE_WAYSIDE2_HEIGHT )

// Scratch area for rolling forest.
#define TILE_ROLLING_FOREST1	TILE_MOVING_COLUMN + mr_calculate_prepared_tile_size_horizontal( TILE_COLUMN_WIDTH, TILE_COLUMN_HEIGHT )

// Scratch area for rolling ground.
#define TILE_ROLLING_GROUND		TILE_ROLLING_FOREST1 + mr_calculate_prepared_tile_size_horizontal( TILE_FOREST1_WIDTH, TILE_FOREST1_HEIGHT )

// Scratch area for moving bird (frame 2)
#define TILE_MOVING_BIRD3		TILE_ROLLING_GROUND + mr_calculate_prepared_tile_size_horizontal( TILE_GROUND_WIDTH, TILE_GROUND_HEIGHT )

// Starting of scratch areas to generate animated tiles
#define TILE1_SPARE_AREA		TILE1_COUNT + 1

// Scratch area for moving bird (frame 1) on first screen
#define TILE1_MOVING_BIRD1		TILE1_SPARE_AREA

// Scratch area for moving title (left-right)
#define TILE1_MOVING_MINIBIRD		TILE1_MOVING_BIRD1 + mr_calculate_prepared_tile_size_horizontal( TILE1_BIRD1_WIDTH, TILE1_BIRD1_HEIGHT )

#define TILE1_FINAL				TILE1_MOVING_MINIBIRD + mr_calculate_prepared_tile_size_horizontal( TILE1_MINIBIRD_WIDTH, TILE1_MINIBIRD_HEIGHT )

// Number of frames for bird animation
#define BIRD_FRAMES				2

// Maximum number of column that can appears in a level.
#define COLUMN_COUNT			20

// Space between columns (||        ||) 
//                         <-------> 
#define COLUMN_SPACING			8

// Max hole height, in tiles.
#define HOLE_HEIGHT				7

// Starting speed of the bird.
#define BIRD_INIT_SPEED			1

// Starting acceleration of the bird.
#define BIRD_INIT_ACCELERATION	1

// Starting acceleration of the bird.
#define BIRD_INIT_ACCELERATION	1

// Fixed horizontal position of the bird
#define BIRD_X					( ( MR_SCREEN_WIDTH >> 1 ) * 8 + ( TILE_BIRD1_WIDTH >> 2 ) * 8 )

// Fixed right border for collision detection.
#define BIRD_X_BORDER			( BIRD_X + ( TILE_BIRD1_WIDTH * 8 ) - ( TILE_BIRD1_WIDTH >> 2 ) * 2 )

// Top border for collision detection.
#define BIRD_Y					( birdY + ( TILE_BIRD1_HEIGHT >> 2 ) * 8 )

// Bottom border for collision detection.
#define BIRD_Y_BORDER			( birdY + (TILE_BIRD1_HEIGHT * 8) - ( TILE_BIRD1_HEIGHT >> 2 ) * 8  )

// Maximum vertical speed of the bird (pixel/jiffy).
#define BIRD_MAX_SPEED			2

// Center of the screen for the titles
#define TITLES_CENTER_X			( ( MR_SCREEN_WIDTH - TILE1_TOTTO_WIDTH ) >> 1 )
#define TITLES_CENTER_Y			( ( MR_SCREEN_HEIGHT - TILE1_TOTTO_HEIGHT ) >> 1 )

#define PRESSANYKEY_CENTER_X	( ( MR_SCREEN_WIDTH - TILE1_PRESSANYKEY_WIDTH ) >> 1 )

#ifdef __PLUS4__
#define SKY_COLOR			MR_COLOR_BLACK
#else
#define SKY_COLOR			MR_COLOR_BLUE
#endif

// This is the current frame showed on bird animation.
mr_position birdFrame = 0;

// This is the current position of the bird, in term of pixels.
mr_tile_position birdY = (MR_SCREEN_HEIGHT >> 1) * 8;

// This is the current speed of the bird, in term of pixels/jiffy.
mr_speed birdVY = BIRD_INIT_SPEED;

// This is the current acceleration of the bird, in term of pixels/jiffy^2.
mr_acceleration birdAY = BIRD_INIT_ACCELERATION;

// The player has started to play?
mr_boolean birdPlaying = mr_false;

// The game has over?
mr_boolean gameOver = mr_false;

// The player wins?
mr_boolean playerWin = mr_false;

// Where is the first column, in pixel coordinates? This is the 
// offset for the first column. It starts as negative, since the 
// entire level is out of the screen, and it will be increased 
// with the progress of the time.
mr_tile_position columnX = MR_SCREEN_WIDTH * 8;

// Has the bird "entered" inside an hole?
mr_tile_position enterColumn = COLUMN_COUNT+1;

// Store the score of the player
mr_position score[4] = { 0, 0, 0, 0 };

// Store the hiscore of the player
mr_position hiscore[4] = { 0, 0, 0, 0 };

// Where is the "hole" inside the column? If we represent 
// the column as a column of characters, this value will
// indicate the first row that has no graphical element.
//
//          ||
//          ==
//            <--- h
//          ==
//          ||
//
// For this reason, the minimum value will be 2
// (1 for column, 1 for wayside).
mr_position columnHoleAt[COLUMN_COUNT];

mr_position birdTitleY[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

// Index of the frame ground.
mr_position groundFrame = 0;

// Index of the forest ground.
mr_position forestFrame = 0;

// Store the level
mr_position level[2] = { 1, 0 };

// Index of the forest ground.
mr_position columnCount = 0;

mr_position holeHeight = HOLE_HEIGHT;

/****************************************************************************
 ** RESIDENT FUNCTIONS SECTION
 ****************************************************************************/

// All the functions defined within the resident body of the code are 
// accessible from all modules, both resident and changing ones.

// Draw the score at specific position.
void draw_score(mr_position _x, mr_position _y) {
	mr_position i;
	mr_puttilev(_x, _y, TILE_WAYSIDE1, MR_COLOR_WHITE);
	for (i = 0; i < 4; ++i) {
		mr_puttilev(_x + 1 + (4 - i), _y, TILE_DIGIT0 + score[i], MR_COLOR_WHITE);
	}
}

// Draw the hiscore at specific position.
void draw_hiscore(mr_position _x, mr_position _y) {
	mr_position i;
	mr_puttilev(_x, _y, TILE_HISCORE, MR_COLOR_WHITE);
	for (i = 0; i < 4; ++i) {
		mr_puttilev(_x + 1 + (4 - i), _y, TILE_DIGIT0 + hiscore[i], MR_COLOR_WHITE);
	}
}

// Draw the current level at specific position.
void draw_level(mr_position _y) {
	mr_position i = 0;
	mr_position ofs = (MR_SCREEN_WIDTH - ((TILE_LEVEL_WIDTH + 3))) >> 1;
	mr_puttilesv(ofs, _y, TILE_LEVEL, TILE_LEVEL_WIDTH, MR_COLOR_WHITE);
	for (i = 0; i < 2; ++i) {
		mr_puttilev(ofs + TILE_LEVEL_WIDTH + 3 - i, _y, TILE_DIGIT0 + level[i], MR_COLOR_WHITE);
	}
}

// Increase the given multibyte (max size = 4 bytes) by 1.
void increase(unsigned char* points) {
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

// Prepare the precalculated graphics, starting from the one given on
// storage. This algorithm will change the active tile set, too.
void prepare_graphics() {

	mr_position i;

	// Load the tilesets from disk.
	mr_tileset_load("tttiles.bin", MR_TILESET_0, TILE_START, TILE_COUNT);
	mr_tileset_load("tttiles1.bin", MR_TILESET_1, TILE1_START, TILE1_COUNT);

	// Prepare animation tiles for vertical movement of first frame of the bird.
	mr_tile_prepare_vertical_extended(MR_TILESET_0, TILE_BIRD1, TILE_BIRD1_WIDTH, TILE_BIRD1_HEIGHT, TILE_MOVING_BIRD1);

	// Prepare animation tiles for horizontal movement of the forest.
	mr_tile_prepare_roll_horizontal(MR_TILESET_0, TILE_FOREST1, TILE_ROLLING_FOREST1);

	// Prepare animation tiles for horizontal movement of the columns (made of: column and waysides).
	mr_tile_prepare_horizontal(MR_TILESET_0, TILE_COLUMN, TILE_MOVING_COLUMN);
	mr_tile_prepare_horizontal(MR_TILESET_0, TILE_WAYSIDE1, TILE_MOVING_WAYSIDE1);
	mr_tile_prepare_horizontal(MR_TILESET_0, TILE_WAYSIDE2, TILE_MOVING_WAYSIDE2);

	// Prepare animation tiles for vertical movement of second frame of the bird.
	mr_tile_prepare_vertical_extended(MR_TILESET_0, TILE_BIRD3, TILE_BIRD3_WIDTH, TILE_BIRD3_HEIGHT, TILE_MOVING_BIRD3);

	// Prepare animation tiles for horizontal movement of the ground.
	mr_tile_prepare_roll_horizontal(MR_TILESET_0, TILE_GROUND, TILE_ROLLING_GROUND);

	// Prepare animation tiles for horizontal movement of first frame of the bird.
	mr_tile_prepare_horizontal_extended(MR_TILESET_1, TILE1_BIRD1, TILE1_BIRD1_WIDTH, TILE1_BIRD1_HEIGHT, TILE1_MOVING_BIRD1);

	mr_tile_prepare_horizontal_extended(MR_TILESET_1, TILE1_MINIBIRD, TILE1_MINIBIRD_WIDTH, TILE1_MINIBIRD_HEIGHT, TILE1_MOVING_MINIBIRD);

	// Since we do not need midres, we redefine characters, in order to avoid
	// side effects in using the drawing primitives.
	for (i = 0; i < 16; ++i) {
		RENDERED_MIXELS[0] = TILE_EMPTY;
	}
	RENDERED_MIXELS[15] = TILE_FILLER;

	mr_clear_bitmapv();

	// Enable the custom tileset for viewing.
	mr_tileset_visible(MR_TILESET_0);

}

// Draw the column. The column will be drawn starting from (_x,0)
// up to the upper level of the forest. The column will be
// drawn with an "hole" at given level _h.
//
//          ||
//          ==
//            <--- h
//          ==
//          ||
//
void draw_column(mr_tile_position _x, mr_position _h, mr_position _y) {

	// Used as index.
	mr_position i;

	if (_h > 1) {
		// First of all, we draw the column,
		// up to _h - 2 row.
		for (i = _y; i < (_h - 1) * 8; i += 8) {
			mr_tile_moveto_horizontalv(
				_x, i,
				TILE_MOVING_COLUMN,
				MR_COLOR_YELLOW
			);
		}
	}
	else {
		i = _y;
	}

	// Now we draw the upper wayside.
	mr_tile_moveto_horizontalv(
		_x, i,
		TILE_MOVING_WAYSIDE1,
		MR_COLOR_YELLOW
	);

	i += 8;
	// Move down by 3 tiles, making spaces for 2 empty tiles.
	i += holeHeight*8;
	i += 8;

	// Then, we draw the lower wayside.
	mr_tile_moveto_horizontalv(
		_x, i,
		TILE_MOVING_WAYSIDE2,
		MR_COLOR_YELLOW
	);
	i += 8;

	// We now draw the rest of the column.
	for (; i < ( MR_SCREEN_HEIGHT - 7 ) * 8; i+=8 ) {
		mr_tile_moveto_horizontalv(
			_x, i,
			TILE_MOVING_COLUMN,
			MR_COLOR_YELLOW
		);
	}

}

// This function will prepare the playfied. 
void prepare_playfield() {

	mr_position i;

	// Clear screen bitmap.
	mr_clear_bitmapv();

	// Let's draw the fixed ground terrain.
	for (i = MR_SCREEN_HEIGHT - 3; i < MR_SCREEN_HEIGHT; ++i) {
		mr_htilesv(0, MR_SCREEN_WIDTH - 1, i, 
			TILE_FILLER, 
			MR_COLOR_ORANGE);
	}

	// Let's draw the moving ground terrain.
	mr_htilesv(0, MR_SCREEN_WIDTH - 1, MR_SCREEN_HEIGHT - 4, 
			TILE_GROUND,
			MR_COLOR_LIGHT_GREEN);

	// Let's draw the fixed forest.
	for (i = MR_SCREEN_HEIGHT - 6; i < MR_SCREEN_HEIGHT - 4; ++i) {
		mr_htilesv(0, MR_SCREEN_WIDTH - 1, i, TILE_FILLER, MR_COLOR_LIGHT_GREEN);
	}

	// Finally, let's draw the moving forest.
	mr_htilesv(0, MR_SCREEN_WIDTH - 1, MR_SCREEN_HEIGHT - 7, TILE_FOREST1, MR_COLOR_GREEN);

	columnCount = 1 + ( ( level[0] + 10 * level[1] ) >> 1 );
	if (columnCount > COLUMN_COUNT) {
		columnCount = COLUMN_COUNT;
	}

	// Initialize the holes inside the various columns.
	for (i = 0; i < columnCount; ++i) {
		columnHoleAt[i] = (rand() & 0x03) + (rand() & 0x01) + (rand() & 0x01);
	}

	holeHeight = HOLE_HEIGHT - (level[1] < 3 ? level[1] : 3);

}

void show_game_over() {

	mr_tile_position i;
	mr_position j;
	mr_boolean exitLoop = mr_false;

	// Since we do not need midres, we redefine characters, in order to avoid
	// side effects in using the drawing primitives.
	for (i = 0; i < 16; ++i) {
		RENDERED_MIXELS[0] = TILE1_EMPTY;
	}
	RENDERED_MIXELS[15] = TILE1_FILLER;

	// Set the backgroubnd to the blue sky.
	mr_set_background_color(SKY_COLOR);

	mr_clear_bitmapv();

	mr_putetilesv((MR_SCREEN_WIDTH - TILE_GAMEOVER_WIDTH) >> 1, (MR_SCREEN_HEIGHT - TILE_GAMEOVER_HEIGHT)>>1, TILE_GAMEOVER, TILE_GAMEOVER_WIDTH, TILE_GAMEOVER_HEIGHT, MR_COLOR_YELLOW);

	draw_score(( (MR_SCREEN_WIDTH) >> 1 ) -8, ( (MR_SCREEN_HEIGHT)>>1 ) + 2 );
	draw_hiscore(((MR_SCREEN_WIDTH) >> 1) +2, ((MR_SCREEN_HEIGHT) >> 1) + 2);
	draw_level(( (MR_SCREEN_HEIGHT) >> 1 ) + 4);

	for (j = 0; j < 4; ++j) {
		birdTitleY[j] = j * 8 * TILE1_BIRD1_HEIGHT;
	}
	for (; j < 8; ++j) {
		birdTitleY[j] = MR_SCREEN_HEIGHT * 8 - (j - 5) * 8 * (TILE1_MINIBIRD_HEIGHT + 1);
	}

	mr_wait(2);

}

// This is the main gameloop of the game. We exit from this
// routine only if the player wins or loses.
void gameloop() {

	mr_position j,t;

	prepare_playfield();

	gameOver = mr_false;
	birdPlaying = mr_false;
	playerWin = mr_true;
	columnX = MR_SCREEN_WIDTH * 8;
	birdY = (MR_SCREEN_HEIGHT >> 1) * 8;
	birdVY = BIRD_INIT_SPEED;
	birdAY = BIRD_INIT_ACCELERATION;

	draw_hiscore(0, 0);
	draw_score(MR_SCREEN_WIDTH - 6, 0);

	// Endless loop...
	while (1) {

		mr_start_frame();

		// Draw the animated bird.

		switch (birdFrame) {
			case 0:
				t = TILE_MOVING_BIRD1;
				break;
			case 1:
				t = TILE_MOVING_BIRD3;
				break;
		}

		mr_tile_moveto_vertical_extendedv(
			BIRD_X, birdY,
			t,
			TILE_BIRD1_WIDTH, TILE_BIRD1_HEIGHT,
			MR_COLOR_WHITE);

		// Animate the forest and the ground. We call once the forest's one and
		// three times the ground's one since we have to simulate the "parallax"
		// effect.
		mr_tile_roll_horizontal_on_place(MR_TILESET_0, TILE_ROLLING_FOREST1, mr_direction_left, TILE_FOREST1, &forestFrame);
		mr_tile_roll_horizontal_on_place(MR_TILESET_0, TILE_ROLLING_FOREST1, mr_direction_left, TILE_FOREST1, &forestFrame);
		mr_tile_roll_horizontal_on_place(MR_TILESET_0, TILE_ROLLING_GROUND, mr_direction_left, TILE_GROUND, &groundFrame);
		mr_tile_roll_horizontal_on_place(MR_TILESET_0, TILE_ROLLING_GROUND, mr_direction_left, TILE_GROUND, &groundFrame);
		mr_tile_roll_horizontal_on_place(MR_TILESET_0, TILE_ROLLING_GROUND, mr_direction_left, TILE_GROUND, &groundFrame);

		// If the player is playing...
		if (birdPlaying) {

			// Draw all the (visible) columns of the level.
			for (j = 0; j < columnCount; ++j) {

				// Calculate x position of the column and the
				// position of the hole.
				mr_tile_position x = columnX + j * COLUMN_SPACING * 8;
				mr_position h = columnHoleAt[j];

				// Avoid calculation if the column is not visible.
				if ((x >= -8*TILE_COLUMN_WIDTH) && x <= (MR_SCREEN_WIDTH * 8)) {

					// Collision check. First of all, we need to check if the bird
					// border has touched the column.
					if ( BIRD_X_BORDER >= x && BIRD_X <= ( x + 8 ) ) {

						if (enterColumn != j) {
							enterColumn = j;
							increase(score);
							draw_score(MR_SCREEN_WIDTH - 6, 0);
						}

						// Next, we need to check if the bird has passed
						// inside the hole or if it has touched the upper
						// or lower border of the column.
						if ( ( BIRD_Y_BORDER >= (( h+ holeHeight + 1) * 8) ) || ( birdY <= (h * 8) ) ) {
							mr_clear_bitmap(MR_SCREEN_DEFAULT);
							// TOUCHED! Game over.
							gameOver = mr_true;
							playerWin = mr_false;
							break;
						}

					}

					// Let's draw the column on the screen.
					draw_column(x, h, 8);

					if (x < 0 && j == (columnCount - 1)) {
						playerWin = mr_true;
						gameOver = mr_true;
						break;
					}

				}

			}

			// Stop here if game is over.
			if (gameOver) {
				break;
			}

		}

		// Update the bird animation.
		++birdFrame;
		if (birdFrame >= BIRD_FRAMES) {
			birdFrame = 0;
		}

		// If any key has been pressed...		
		if (mr_key_pressed()) {

			// Start the game, if not already playing.
			birdPlaying = 1;

			// Set vertical acceleration to move 2 px/s^2 to up.
			birdAY = -2;

			// Decrease the actual vertical speed of the bird.
			--birdVY;
		}
		
		// BIRD PHYSICS

		// If the player is playing...
		if (birdPlaying) {

			// If the bird reachs the upper side of the screen
			// we stop its movement and tends to move it down.
			if (birdY <= 10) {
				birdY = 11;
				birdVY = 1;
				birdAY = 0;
			}

			// If the bird reachs the lower side of the screen,
			// then... GAME OVER.
			if ((birdY + (TILE_BIRD1_HEIGHT*8)) > (MR_SCREEN_HEIGHT - 7) * 8) {
				gameOver = mr_true;
				playerWin = mr_false;
				break;
			}

			// Scroll the columns.
			columnX -= ( 1 + ( level[1] < 3 ? level[1] : 3 ) );

			// If the bird has a vertical acceleration,
			// we have to decrease it up to zero.
			if (birdAY <= 0) {
				++birdAY;
			} 
			// Otherwise, if the bird has a vertical velocity
			// to the ground, we must stop the vertical
			// acceleration.
			else if (birdVY > 0) {
				birdAY = 0;
			}
		}
		// If the player is *NOT* playing...
		else {
			if (birdVY >= BIRD_MAX_SPEED ) {
				birdAY = -1;
			} else if (birdVY <= -BIRD_MAX_SPEED) {
				birdAY = 1;
			}
		}

		// If the bird has a too high vertical speed,
		// we need to fix it to the maximum value
		if (birdVY < -BIRD_MAX_SPEED) {
			birdVY = -BIRD_MAX_SPEED;
		}
		if (birdVY > BIRD_MAX_SPEED) {
			birdVY = BIRD_MAX_SPEED;
		}

		// Finally, update the bird physics variables.
		birdY += birdVY;
		birdVY += birdAY;

		mr_end_frame(4);

	}

}

void show_titles() {

	mr_boolean exitLoop = mr_false;
	mr_boolean titleAlternate = mr_false;
	mr_tile_position i;
	mr_position j;
	mr_color c = MR_COLOR_YELLOW + MR_COLOR_BRIGHTNESS;
	mr_position bx;

	mr_tile_position x = 0, y = 0;

	// Since we do not need midres, we redefine characters, in order to avoid
	// side effects in using the drawing primitives.
	for (i = 0; i < 16; ++i) {
		RENDERED_MIXELS[0] = TILE1_EMPTY;
	}
	RENDERED_MIXELS[15] = TILE1_FILLER;

	mr_clear_bitmapv();

	// Enable the custom tileset for viewing.
	mr_tileset_visible(MR_TILESET_1);

	// Set the backgroubnd to the blue sky.
	mr_set_background_color(SKY_COLOR);
	for (j = 0; j < 4; ++j) {
		birdTitleY[j] = j * 8 * ( TILE1_MINIBIRD_HEIGHT + 1 );
	}
	for (; j < 8; ++j) {
		birdTitleY[j] = MR_SCREEN_HEIGHT * 8 - (j-2) * 8 * ( TILE1_MINIBIRD_HEIGHT + 1 );
	}

	mr_putetilesv(PRESSANYKEY_CENTER_X, MR_SCREEN_HEIGHT - TILE1_PRESSANYKEY_HEIGHT - 1, TILE1_PRESSANYKEY, TILE1_PRESSANYKEY_WIDTH, TILE1_PRESSANYKEY_HEIGHT, MR_COLOR_WHITE);

	while (!exitLoop) {

		// Move the bird horizzontally
		for (i = -2 * TILE1_BIRD1_WIDTH; i < (MR_SCREEN_WIDTH * 8 + 4 * 8 * TILE1_BIRD1_WIDTH); i+=2) {
			if (mr_key_pressed()) {
				exitLoop = mr_true;
				break;
			}
			for (j = 0; j < 4; ++j) {
				bx = (i >> 3) - (j * TILE1_BIRD1_WIDTH) - 1;
				if ((bx < MR_SCREEN_WIDTH) && (bx >= 0)) {
					mr_puttilev(bx, birdTitleY[j] >> 3, TILE1_EMPTY, c);
					mr_puttilev(bx, (birdTitleY[j] >> 3) + 1, TILE1_EMPTY, c);
				}
				mr_tile_moveto_horizontal_extendedv(
					i - (j * TILE1_BIRD1_WIDTH * 8), birdTitleY[j],
					TILE1_MOVING_BIRD1,
					TILE1_BIRD1_WIDTH, TILE1_BIRD1_HEIGHT,
					c);
			}
			for (j = 4; j < 8; ++j) {
				bx = (j * TILE1_MINIBIRD_WIDTH) - (i >> 3) + 1;
				if ((bx < MR_SCREEN_WIDTH)&&(bx>=0)) {
					mr_puttilev(bx, birdTitleY[j] >> 3, TILE1_EMPTY, c);
				}
				mr_tile_moveto_horizontalv(
					MR_SCREEN_WIDTH * 8 + (j * TILE1_MINIBIRD_WIDTH * 8) - i, birdTitleY[j],
					TILE1_MOVING_MINIBIRD,
					c);
			}

			if ( i == 160 ) {
				if (titleAlternate) {
					mr_putetilesv(TITLES_CENTER_X, TITLES_CENTER_Y, TILE1_TOTTO, TILE1_TOTTO_WIDTH, TILE1_TOTTO_HEIGHT, MR_COLOR_WHITE);
					titleAlternate = mr_false;
				} else {
					mr_putetilesv(TITLES_CENTER_X, TITLES_CENTER_Y, TILE1_TOTTO2, TILE1_TOTTO2_WIDTH, TILE1_TOTTO2_HEIGHT, MR_COLOR_WHITE);
					titleAlternate = mr_true;
				}
			}

		}

		c = MR_NEXT_COLOR(c);

	}

	// Since we do not need midres, we redefine characters, in order to avoid
	// side effects in using the drawing primitives.
	for (i = 0; i < 16; ++i) {
		RENDERED_MIXELS[0] = TILE_EMPTY;
	}
	RENDERED_MIXELS[15] = TILE_FILLER;

	// Clear the screen.
	mr_clear_bitmapv();

	// Enable the custom tileset for viewing.
	mr_tileset_visible(MR_TILESET_0);

}

void show_level() {

	// Clear the screen.
	mr_clear_bitmapv();

	// Show level
	draw_level(MR_SCREEN_HEIGHT >> 1);

	mr_wait(2);

}

void show_winning_screen() {

	switch (rand() & 0x03) {

		case 0:
			mr_load("ttfinal1.mpic", MR_AUX_DEFAULT);
			break;
		case 1:
			mr_load("ttfinal2.mpic", MR_AUX_DEFAULT);
			break;
		case 2:
			mr_load("ttfinal3.mpic", MR_AUX_DEFAULT);
			break;
		case 3:
			mr_load("ttfinal4.mpic", MR_AUX_DEFAULT);
			break;
	}

	mr_uncompressv(MR_AUX_DEFAULT);
}

void game_totto() {

	// Initialize random number generator
	srand(clock());

	// Initialize graphical subsystem
	mr_init();

	// Clear screen bitmap.
	mr_clear_bitmapv();

	prepare_graphics();

	while (1) {

		show_titles();

		score[0] = 0;
		score[1] = 0;
		score[2] = 0;
		score[3] = 0;

		while (1) {

			show_level();

			gameloop();

			if (!playerWin) {
				break;
			}

			increase(level);

			if ( ( level[0] == 0) && (level[1] == 0) ) {
				show_winning_screen();
				break;
			}
		}

		if (*((long*)&score[0]) > * ((long*)&hiscore[0])) {
			hiscore[0] = score[0];
			hiscore[1] = score[1];
			hiscore[2] = score[2];
			hiscore[3] = score[3];
		}
		
		show_game_over();

	}

}


#endif
