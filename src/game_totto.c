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

#ifdef __TOTTO__

/****************************************************************************
 ** INCLUDE SECTION
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"
#include "midres.h"
#include "main.h"
#include "midres.h"
#include "game_totto_tiles.h"
#include "game_totto_tiles1.h"

#include "rawdata.h"

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
#define HOLE_HEIGHT				6

// Starting speed of the bird.
#define BIRD_INIT_SPEED			1

// Starting acceleration of the bird.
#define BIRD_INIT_ACCELERATION	1

// Starting acceleration of the bird.
#define BIRD_INIT_ACCELERATION	1

// Fixed horizontal position of the bird
#define BIRD_X					( ( ( MR_SCREEN_WIDTH >> 1 ) * 8 ) - ( ( TILE_BIRD1_WIDTH >> 2 ) * 8 ) ) 

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

#define PRESSANYKEY_CENTER_X	( ( MR_SCREEN_WIDTH >> 1 ) - ( TILE1_PRESSANYKEY_WIDTH >> 1 ) )

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
mr_score score[4] = { 0, 0, 0, 0 };

// Store the hiscore of the player
mr_score hiscore[4] = { 0, 0, 0, 0 };

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
mr_level level[2] = { 1, 0 };

// Index of the forest ground.
mr_position columnCount = 0;

// This is the (maximum) height of the gaps.
mr_position holeHeight = HOLE_HEIGHT;

// Sound duration counter (in jiffies)
signed char soundDuration = 0;

// title song
mr_musicplayer_protothread musicPlayerTitles;

// game song
mr_musicplayer_protothread musicPlayerGame;

// game over song
mr_musicplayer_protothread musicPlayerGameOver;

/****************************************************************************
 ** RESIDENT FUNCTIONS SECTION
 ****************************************************************************/

unsigned char* mr_translate_file_user(mr_file _file) {

	switch (_file) {
	case FILE_TTTILES_BIN:
		return "tttiles.bin";
	case FILE_TTTILES1_BIN:
		return "tttiles1.bin";
#ifdef FILE_TOTTO_IMF
	case FILE_TOTTO_IMF:
		return "totto.imf";
#endif
#ifdef FILE_TOTTO2_IMF
	case FILE_TOTTO2_IMF:
		return "totto2.imf";
#endif
#ifdef FILE_TOTTO3_IMF
	case FILE_TOTTO3_IMF:
		return "totto3.imf";
#endif
	}

	return 0;

}
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
	mr_tileset_load_file(FILE_TTTILES_BIN, MR_TILESET_0, TILE_START, TILE_COUNT);
	mr_tileset_load_file(FILE_TTTILES1_BIN, MR_TILESET_1, TILE1_START, TILE1_COUNT);

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
		MR_RENDERED_MIXELS[0] = TILE_EMPTY;
	}
	MR_RENDERED_MIXELS[15] = TILE_FILLER;

	mr_clearv();

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
	mr_clearv();

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

	columnCount = 5 + ( ( level[0] + 10 * level[1] ) >> 1 );
	if (columnCount > COLUMN_COUNT) {
		columnCount = COLUMN_COUNT;
	}

	// Initialize the holes inside the various columns.
	for (i = 0; i < columnCount; ++i) {
		columnHoleAt[i] = 2 + (rand() & 0x02) + (rand() & 0x02);
	}

	// Setup the heiht of each column.
	holeHeight = HOLE_HEIGHT - ((level[1]>>1) < 3 ? (level[1] >> 1) : 3);

}

// Show the "game over" screen.
void show_game_over() {

	mr_start_frame();

	// Set the background to the blue sky.
	mr_set_background_color(SKY_COLOR);

	// Clear the bitmap
	mr_clearv();

	// "GAME OVER"
	mr_putetilesv((MR_SCREEN_WIDTH - TILE_GAMEOVER_WIDTH) >> 1, (MR_SCREEN_HEIGHT - TILE_GAMEOVER_HEIGHT)>>1, TILE_GAMEOVER, TILE_GAMEOVER_WIDTH, TILE_GAMEOVER_HEIGHT, MR_COLOR_YELLOW);

	// Draw score (on left)
	draw_score(( (MR_SCREEN_WIDTH) >> 1 ) -8, ( (MR_SCREEN_HEIGHT)>>1 ) + 2 );

	// Draw hi score (on right)
	draw_hiscore(((MR_SCREEN_WIDTH) >> 1) +2, ((MR_SCREEN_HEIGHT) >> 1) + 2);

	// Draw level reached
	draw_level(( (MR_SCREEN_HEIGHT) >> 1 ) + 4);

	mr_end_frame(0);

	musicPlayerGameOver.done = mr_false;
	musicPlayerGameOver.buffer = musicPlayerGameOver.auto_restart_buffer;
	while (!musicPlayerGameOver.done) {
		mr_musicplayer(&musicPlayerGameOver);
	}

}

// This is the main gameloop of the game. We exit from this
// routine only if the player wins or loses.
void gameloop() {

	mr_position j,t;

	// Prepare a fresh new playfield.
	prepare_playfield();

	// Game is not over (yet!).
	gameOver = mr_false;

	// Bird is not playing (yet!).
	birdPlaying = mr_false;

	// The player will win, normally.
	playerWin = mr_true;

	// Offset of the first column (outside the screen)
	columnX = MR_SCREEN_WIDTH * 8;

	// Starting vertical position of the bird is
	// the center of the screen
	birdY = (MR_SCREEN_HEIGHT >> 1) * 8;

	// Bird has an initial speed and acceleration.
	birdVY = BIRD_INIT_SPEED;
	birdAY = BIRD_INIT_ACCELERATION;

	// Draw hi score (on left) and score (on right)
	draw_hiscore(0, 0);
	draw_score(MR_SCREEN_WIDTH - 6, 0);
	
	// "PRESS ANY KEY"
	mr_putetilesv(PRESSANYKEY_CENTER_X, MR_SCREEN_HEIGHT - TILE_PRESSANYKEY_HEIGHT - 8, TILE_PRESSANYKEY, TILE_PRESSANYKEY_WIDTH, TILE_PRESSANYKEY_HEIGHT, MR_COLOR_WHITE);

	// Endless loop...
	while (1) {

		// Track the start time of frame drawing. This is needed
		// since the drawing time is variable.
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

		// Animate the forest and the ground. We call twice the forest's one and
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
				// (starting) position of the hole.
				mr_tile_position x = columnX + j * COLUMN_SPACING * 8;
				mr_position h = columnHoleAt[j];

				// Avoid calculation if the column is not visible.
				if ((x >= -8*TILE_COLUMN_WIDTH) && x <= (MR_SCREEN_WIDTH * 8)) {

					// Collision check. First of all, we need to check if the bird
					// border has touched the column.
					if ( BIRD_X_BORDER >= x && BIRD_X <= ( x + 8 ) ) {

						// Track if the bird changed column.
						if (enterColumn != j) {

							// Bird passed the pipe. Now, we can start a "beep"
							// that will last for 24 jiffies.
							mr_sound_start(0);
							mr_sound_change(2000);
							soundDuration = 24;

							// Increase (and draw) the score.
							increase(score);
							draw_score(MR_SCREEN_WIDTH - 6, 0);

							// Update the last passed column.
							enterColumn = j;

						}

						// Next, we need to check if the bird has passed
						// inside the hole or if it has touched the upper
						// or lower border of the column.
						if ( ( BIRD_Y_BORDER >= (( h+ holeHeight + 1) * 8) ) || ( birdY <= (h * 8) ) ) {

							mr_clearv();

							// OUCH! Game over and player loses.
							gameOver = mr_true;
							playerWin = mr_false;
							break;
						}

					}

					// Let's draw the column on the screen.
					draw_column(x, h, 8);

					// Is last column outside the screen?
					if (x < 0 && j == (columnCount - 1)) {
						// Game over and go to next level.
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

			// remove "PRESS ANY KEY".
			if (!birdPlaying) {
				mr_htilesv(PRESSANYKEY_CENTER_X, PRESSANYKEY_CENTER_X + TILE_PRESSANYKEY_WIDTH, MR_SCREEN_HEIGHT - TILE_PRESSANYKEY_HEIGHT - 8, TILE_EMPTY, MR_COLOR_WHITE);
			}

			// Start the game, if not already playing.
			birdPlaying = mr_true;

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
			// then... "GAME OVER".
			if ((birdY + (TILE_BIRD1_HEIGHT*8)) > (MR_SCREEN_HEIGHT - 7) * 8) {
				gameOver = mr_true;
				playerWin = mr_false;
				break;
			}

			// Scroll the columns.
			columnX -= ( 3 + ( level[1] < 3 ? level[1] : 3 ) );

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
			// Oscillation here.
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

		// Stop sound if duration passed.
		if (soundDuration > 0) {
			soundDuration -= 4;
			if (soundDuration <= 0) {
				mr_sound_stop();
				soundDuration = 0;
			}
		}

		if (birdPlaying) {
			MR_PTI_END_FRAME_RUNNING(3, mr_musicplayer, musicPlayerGame);
		}
		else {
			MR_PTI_END_FRAME_RUNNING(6, mr_musicplayer, musicPlayerTitles);
		}

		// Wait for up to 4 jiffies before continuing.
		//mr_end_frame(4);

	}

	// Stop sound if playing.
	mr_sound_stop();

}

// Show the "titles" screen.
void show_titles() {

	// Used to exit the loop.
	mr_boolean exitLoop = mr_false;

	// Used to alternate the titles (western/eastern)
	mr_boolean titleAlternate = mr_false;

	// Used as indexes.
	mr_tile_position i;
	mr_position j;

	// Last bird color.
	mr_color c = MR_COLOR_YELLOW + MR_COLOR_BRIGHTNESS;

	// Used to maintain the horizontal position of animated bird.
	mr_position bx;

	mr_start_frame();

	// Since we do not need midres, we redefine characters, in order to avoid
	// side effects in using the drawing primitives.
	for (i = 0; i < 16; ++i) {
		MR_RENDERED_MIXELS[0] = TILE1_EMPTY;
	}
	MR_RENDERED_MIXELS[15] = TILE1_FILLER;
	mr_clearv();

	// Enable the custom tileset for viewing.
	mr_tileset_visible(MR_TILESET_1);

	// Set the background to the blue sky.
	mr_set_background_color(SKY_COLOR);

	// Initialize birds' positions.
	for (j = 0; j < 4; ++j) {
		birdTitleY[j] = j * 8 * ( TILE1_MINIBIRD_HEIGHT + 1 );
	}
	for (; j < 8; ++j) {
		birdTitleY[j] = MR_SCREEN_HEIGHT * 8 - (j-2) * 8 * ( TILE1_MINIBIRD_HEIGHT + 1 );
	}

	// "PRESS ANY KEY"
	mr_putetilesv(PRESSANYKEY_CENTER_X, MR_SCREEN_HEIGHT - TILE1_PRESSANYKEY_HEIGHT - 1, TILE1_PRESSANYKEY, TILE1_PRESSANYKEY_WIDTH, TILE1_PRESSANYKEY_HEIGHT, MR_COLOR_WHITE);

	mr_end_frame(0);

	while (!exitLoop) {

		mr_musicplayer(&musicPlayerTitles);

		// Move the birds horizzontally
		for (i = -2 * TILE1_BIRD1_WIDTH; i < (MR_SCREEN_WIDTH * 8 + 4 * 8 * TILE1_BIRD1_WIDTH); i+=2) {

			mr_start_frame();

			// Exit if a key is pressed.
			if (mr_key_pressed()) {
				exitLoop = mr_true;
				break;
			}

			// Move big birds.
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
				mr_musicplayer(&musicPlayerTitles);
			}

			// Move small birds.
			for (j = 4; j < 8; ++j) {
				bx = (j * TILE1_MINIBIRD_WIDTH) - (i >> 3) + 1;
				if ((bx < MR_SCREEN_WIDTH)&&(bx>=0)) {
					mr_puttilev(bx, birdTitleY[j] >> 3, TILE1_EMPTY, c);
				}
				mr_tile_moveto_horizontalv(
					MR_SCREEN_WIDTH * 8 + (j * TILE1_MINIBIRD_WIDTH * 8) - i, birdTitleY[j],
					TILE1_MOVING_MINIBIRD,
					c);
				mr_musicplayer(&musicPlayerTitles);
			}

			// Alternate titles on timing condition.
			if ( i == 160 ) {
				if (titleAlternate) {
					mr_putetilesv(TITLES_CENTER_X, TITLES_CENTER_Y, TILE1_TOTTO, TILE1_TOTTO_WIDTH, TILE1_TOTTO_HEIGHT, MR_COLOR_WHITE);
					titleAlternate = mr_false;
				} else {
					mr_putetilesv(TITLES_CENTER_X, TITLES_CENTER_Y, TILE1_TOTTO2, TILE1_TOTTO2_WIDTH, TILE1_TOTTO2_HEIGHT, MR_COLOR_WHITE);
					titleAlternate = mr_true;
				}
			}

			mr_end_frame(0);
			mr_musicplayer(&musicPlayerTitles);

		}

		// Pass to next color.
		c = MR_NEXT_COLOR(c);


	}

	mr_start_frame();

	// Since we do not need midres, we redefine characters, in order to avoid
	// side effects in using the drawing primitives.
	for (i = 0; i < 16; ++i) {
		MR_RENDERED_MIXELS[0] = TILE_EMPTY;
	}
	MR_RENDERED_MIXELS[15] = TILE_FILLER;
	mr_clearv();

	// Enable the custom tileset for viewing.
	mr_tileset_visible(MR_TILESET_0);

	mr_musicplayer(&musicPlayerTitles);

	mr_end_frame(0);
}

// Show the "level" screen.
void show_level() {

	mr_start_frame();

	// Clear the screen.
	mr_clearv();

	// Show level
	draw_level(MR_SCREEN_HEIGHT >> 1);

	mr_end_frame(0);

	MR_PTI_WAIT_RUNNING(120, mr_musicplayer, musicPlayerTitles);

}

// Show the "winning" screen.
void show_winning_screen() {

	mr_start_frame();

	// Choose a random screen.
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
	mr_musicplayer(&musicPlayerTitles);
	mr_uncompressv(MR_AUX_DEFAULT);
	mr_musicplayer(&musicPlayerTitles);

	mr_end_frame(0);

}

// 
void game_totto() {

	// Initialize random number generator
	srand(clock());

	// Initialize graphical subsystem
	mr_init();

	mr_start_frame();

	mr_set_background_color(SKY_COLOR);
	mr_set_border_color(SKY_COLOR);

	// Clear screen bitmap.
	mr_clearv();

	mr_end_frame(0);

	// Prepare the graphics.
	prepare_graphics();

#ifdef FILE_TOTTO_IMF
	musicPlayerTitles.done = mr_false;
	musicPlayerTitles.buffer = mr_map_file(FILE_TOTTO_IMF, FILE_TOTTO_IMF_SIZE);
	musicPlayerTitles.auto_restart_buffer = musicPlayerTitles.buffer;
	musicPlayerTitles.eof = musicPlayerTitles.buffer + FILE_TOTTO_IMF_SIZE;
	musicPlayerTitles.auto_restart = mr_true;
#else
	musicPlayerTitles.done = mr_false;
	musicPlayerTitles.buffer = NULL;
	musicPlayerTitles.auto_restart_buffer = musicPlayerTitles.buffer;
	musicPlayerTitles.eof = NULL;
	musicPlayerTitles.auto_restart = mr_false;
#endif

#ifdef FILE_TOTTO2_IMF
	musicPlayerGame.done = mr_false;
	musicPlayerGame.buffer = mr_map_file(FILE_TOTTO2_IMF, FILE_TOTTO2_IMF_SIZE);
	musicPlayerGame.auto_restart_buffer = musicPlayerGame.buffer;
	musicPlayerGame.eof = musicPlayerGame.buffer + FILE_TOTTO2_IMF_SIZE;
	musicPlayerGame.auto_restart = mr_true;
#else
	musicPlayerGame.done = mr_false;
	musicPlayerGame.buffer = NULL;
	musicPlayerGame.auto_restart_buffer = NULL;
	musicPlayerGame.eof = NULL;
	musicPlayerGame.auto_restart = mr_false;
#endif

#ifdef FILE_TOTTO3_IMF
	musicPlayerGameOver.done = mr_false;
	musicPlayerGameOver.buffer = mr_map_file(FILE_TOTTO3_IMF, FILE_TOTTO3_IMF_SIZE);
	musicPlayerGameOver.auto_restart_buffer = musicPlayerGameOver.buffer;
	musicPlayerGameOver.eof = musicPlayerGameOver.buffer + FILE_TOTTO3_IMF_SIZE;
	musicPlayerGameOver.auto_restart = mr_false;
#else
	musicPlayerGameOver.done = mr_false;
	musicPlayerGameOver.buffer = NULL;
	musicPlayerGameOver.auto_restart_buffer = musicPlayerGameOver.buffer;
	musicPlayerGameOver.eof = NULL;
	musicPlayerGameOver.auto_restart = mr_false;
#endif

	// Endless loop...
	while (1) {

		musicPlayerTitles.buffer = musicPlayerTitles.auto_restart_buffer;

		mr_musicplayer(&musicPlayerTitles);

		// Show titles.
		show_titles();

		// Reset score.
		score[0] = 0;
		score[1] = 0;
		score[2] = 0;
		score[3] = 0;

		// Reset level.
		level[0] = 1;
		level[1] = 0;

		// Endless loop...
		while (1) {

			mr_musicplayer(&musicPlayerTitles);

			// Show the level screen.
			show_level();

			mr_musicplayer(&musicPlayerGame);

			// Play the game.
			gameloop();

			mr_musicplayer(&musicPlayerGame);

			// Game finished: exit if player lose it.
			if (!playerWin) {
				break;
			}

			// Increase the level.
			increase(level);

			// Level 100 (= 00) is "END OF GAME", and we can
			// show to the player the winning screen.
			if ( ( level[0] == 0) && (level[1] == 0) ) {
				show_winning_screen();
				break;
			}
		}

		// Update the hi score.
		if (*((long*)&score[0]) > * ((long*)&hiscore[0])) {
			hiscore[0] = score[0];
			hiscore[1] = score[1];
			hiscore[2] = score[2];
			hiscore[3] = score[3];
		}
		
		// Show "game over" screen.
		show_game_over();

	}

}


#endif
