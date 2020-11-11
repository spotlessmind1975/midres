#ifdef __GAME_ALIEN_STORM__

// midresIde.makeCommandLine:game=alienstorm;
// midresIde.executableName:alienstorm.{target}.d64;

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

// Needed for system call.
#include <stdlib.h>
#include <time.h>

// Needed for MIDRES library call.
#include "midres.h"

// Include generated tiles reference.
#include "game_alien_storm_tiles.h"
#include "game_alien_storm_tiles1.h"
#include "game_alien_storm_tiles2.h"

/****************************************************************************
 ** DEFINES AND VARIABLES SECTION
 ****************************************************************************/

////////////////////////////////// BUNKERS //////////////////////////////////

// This is the spacing between one bunker and the next (except the first).
#define BUNKER_SPACING				2

// This is the footprint of a bunker plus the space to the right.
#define BUNKER_WIDTH				(TILE_DEFENSE_WIDTH + BUNKER_SPACING )

// This is the number of defensive bunkers the player will see appear on the 
// playing field. This essentially depends on the bunker's footprint.
#define BUNKER_COUNT				( MR_SCREEN_WIDTH / BUNKER_WIDTH )

// This is the offset from the edge of the first bunker screen.
#define BUNKER_OFFSET				( ( MR_SCREEN_WIDTH - ( BUNKER_COUNT * BUNKER_WIDTH ) ) >> 1 )

// Coordinates of each bunker
#define BUNKER_X(n)			(( n * BUNKER_WIDTH ) + BUNKER_OFFSET)
#define BUNKER_Y			(MR_SCREEN_HEIGHT - TILE_CANNON_HEIGHT - 2 - TILE_DEFENSE_HEIGHT)

////////////////////////////////// ALIENS //////////////////////////////////

// How many rows of aliens on the screen?
#define ALIEN_ROW_COUNT				( MR_SCREEN_HEIGHT >> 2 )

// How many aliens in each row?
#define ALIEN_COLUMN_COUNT			( MR_SCREEN_WIDTH >> 2 )

// This is the spacing between one alien and the next (except the first).
#define ALIEN_SPACE					( TILE_ALIEN1A_WIDTH>>1 )

// What is the footprint of an alien?
#define ALIEN_WIDTH					( TILE_ALIEN1A_WIDTH + ALIEN_SPACE )

// This is the offset from the left edge of the first alien.
#define ALIEN_HORIZONTAL_OFFSET		( ( MR_SCREEN_WIDTH - (ALIEN_COLUMN_COUNT * ALIEN_WIDTH) ) >> 1 )

// Maximum animation elongation.
#define ALIEN_HORIZONTAL_ELONGATION	( ALIEN_HORIZONTAL_OFFSET )

// This is the offset from the top edge of the first row of aliens.
#define ALIEN_VERTICAL_OFFSET		4

// This is the number of frames you have to wait before the game updates 
// the status of the aliens. It also represents the maximum slowness 
// of the game.
#define ALIEN_DELAY_FRAME_COUNT		10

// This is the number of frames you have to wait before the game updates 
// the status of the aliens' fires. It also represents the maximum 
// slowness of the fired weapons.
#define ALIEN_FIRE_DELAY_FRAME_COUNT		2

// This is the number of frames that have passed, and relevant to 
// the management of the aliens.
unsigned char alienFrameCounts = 0;

// This is the updated value of the number of frames that must be 
// waited before the game updates the status of the aliens. This 
// value is decreased with each level passed, in order to make 
// the game faster and more difficult.
unsigned char alienDelayFrameCount = ALIEN_DELAY_FRAME_COUNT;

// This is the updated value of the number of frames that must be 
// waited before the game updates the status of the aliens' fires. 
mr_position alienFireDelayFrameCount = ALIEN_FIRE_DELAY_FRAME_COUNT;

// This is the frame of the alien animation (0, 1).
unsigned char alienFrame = 0;

// This is the matrix with which we represent the state of life of each 
// alien. When an alien placed in position(x, y) is present on the 
// screen, the alien type number (1, 2 or 3) is entered into this matrix.
// When removed because hit, it is assigned a value of zero(0).
// Finally, the intermediate state uses the placeholder value 0xff.
unsigned char alienMatrix[ALIEN_ROW_COUNT][ALIEN_COLUMN_COUNT];

// This vector is filled in with the pre-calculated values of the abscissa 
// of each column of aliens. These abscissa are net of the deviation 
// imposed by the movement.
unsigned char alienColumn[MR_SCREEN_WIDTH];

// This is the bitmap with which we represent if there is at least 
// one alien for a given row. By examining this variable you can 
// quickly know if you can avoid examining a line or if the level
// is over.
unsigned char alienRowBitmap;

// Maximum row in which there is at least one alien. With this 
// information, we can quickly know if the invasion has ended.
unsigned char alienMaxRow = ALIEN_ROW_COUNT;

// Maximum row in which there is at least one alien, divided by 
// each column. With this information we can, quickly, know 
// from which position it is appropriate for the alien to fire 
// its shot.
unsigned char alienMaxRowPerColumn[ALIEN_COLUMN_COUNT];

// Location of the shot fired by the alien. Each column of 
// aliens has one shot at its disposal. So there can be multiple 
// hits in parallel, but coming from aliens on different columns.
unsigned char alienFireY[ALIEN_COLUMN_COUNT];
unsigned char alienFireX[ALIEN_COLUMN_COUNT];

// This is the horizontal offset of the overall movement of 
// all aliens in the array. We also keep the previous position 
// of this information (useful for optimizing animations).
signed char alienX = 0;
signed char alienPX = 0;

// This is the direction in which the aliens will have 
// to move on the next round.
signed char alienDX = 1;

// This is the vertical offset of the overall movement of 
// all aliens in the array. We also keep the previous position 
// of this information (useful for optimizing animations).
mr_position alienY = 1;
mr_position alienPY = 1;

// This is the number of the note of the soundtrack.
unsigned char alienSoundtrack = 0;

// This is the number of the columns freed on the left and right
// set of arriving aliens.
signed char leftFreeColumns = 0;
signed char rightFreeColumns = 0;

////////////////////////////////// MISTERY //////////////////////////////////

// This is the offset from the top edge of the mistery alien.
#define MISTERY_VERTICAL_OFFSET		3

// This is the ordinate of the mistery ship.
#define MISTERY_Y					MISTERY_VERTICAL_OFFSET

// This is the number of frames you have to wait before the game updates 
// the status of the aliens' fires. It also represents the maximum 
// slowness of the mistery ship.
#define MISTERY_DELAY_FRAME_COUNT   8

// This is the updated value of the number of frames that must be 
// waited before the game updates the status of the mistery ship. This 
// value is decreased regurarly.
mr_position misteryDelayFrameCount = MISTERY_DELAY_FRAME_COUNT;

// This is the horizontal offset of the overall movement of 
// the mistery ship. We also keep the previous position 
// of this information (useful for optimizing animations).
mr_position misteryX = 0;
mr_position misteryXP = 0;

////////////////////////////////// PLAYER ///////////////////////////////////

// Max lifes for each player.
#define MAX_LIFES					3

// Max number of players
#define MAX_PLAYERS					2

// This is the number of frames you have to wait before the game updates 
// the status of the player's fires. It also represents the maximum 
// slowness of the fired weapons.
#define FIRE_DELAY_FRAME_COUNT		1

// This is the updated value of the number of frames that must be 
// waited before the game updates the status of the player' fire. 
mr_position fireDelayFrameCount = FIRE_DELAY_FRAME_COUNT;

// This is the only dynamic tile in the whole game.
#define	TILE_MOVING_CANNON			TILE_START + TILE_COUNT

// Gun's starting horizontal position
#define CANNON_START_X				( ( MR_SCREEN_WIDTH >> 1 ) * 8 )
#define CANNON_START_Y				( MR_SCREEN_HEIGHT - 2 )

// Current horizontal position of the gun, in pixels.
mr_tile_position cannonX = CANNON_START_X;

// Number of lives for each player.
unsigned char cannonLives[MAX_PLAYERS] = { MAX_LIFES, MAX_LIFES };

// Location of the shot fired by the player.
mr_position fireY = 0;
mr_position fireX = 0;

// This flag keeps the information as to whether the 
// player's gun has been hit or not.
unsigned char cannonHit = 0;

//////////////////////////////// GAMEPLAY /////////////////////////////////

// Let's define, once and for all, the position of the various elements 
// of the game. Starting, of course, with the maximum score obtained by 
// playing previous games.
#define HISCORE_X				( (MR_SCREEN_WIDTH - TILE_HISCORE_WIDTH) >> 1)
#define HISCORE_Y				0

// We continue, therefore, with the position of the score of the first
// and second player.
#define SCORE1_X				0
#define SCORE1_Y				0
#define SCORE2_X				( MR_SCREEN_WIDTH - 4 )
#define SCORE2_Y				0

// This is the position with the cannon count still 
// available to the player.
#define CANNONS_X			1
#define CANNONS_Y			( MR_SCREEN_HEIGHT - 1 )
#define	CANNONS_WIDTH		9
#define	CANNONS_HEIGHT		1

// This is the position for the credit count.
#define CREDIT_X			(MR_SCREEN_WIDTH - 6)
#define CREDIT_Y			(MR_SCREEN_HEIGHT - 1)

// Define a shortcut for players.
#define	PLAYER_1				0
#define PLAYER_2				1

// This flag indicates whether the game (for the given player) is over. 
// The game is over when all of the player's guns are destroyed.
mr_boolean gameOver = mr_false;

// This flag indicates whether the player has finished the level but not 
// the game. It usually happens when all the aliens (except the mystery 
// ship) have been destroyed.
mr_boolean levelFinished = mr_false;

// This is each player's score.
mr_position score[2][4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

// Store the hiscore of the best player
mr_position hiscore[4] = { 0, 0, 0, 0 };

// Store the credit available.
mr_position credit = 0;

// Number of players.
unsigned char players = 1;

// Current player number (0 = player 1, 1 = player 2)
unsigned char currentPlayer = 0;

// Temporary space to store an addendum.
unsigned char addendum[4] = { 0, 0, 0, 0 };

// Current level
unsigned char level = 0;

///////////////////////////// WELCOME SCREEN ////////////////////////////////

// FIRST PAGE
#define TITLE_INSERTCOIN_X			( (MR_SCREEN_WIDTH - TILE2_INSERTCOIN_WIDTH) >> 1 )
#define TITLE_INSERTCOIN_Y			( (MR_SCREEN_HEIGHT >> 1) - 1 )
#define TITLE_12PLAYERS_X			( (MR_SCREEN_WIDTH - TILE2_12PLAYERS_WIDTH) >> 1 )
#define TITLE_12PLAYERS_Y			( (MR_SCREEN_HEIGHT >> 1) + 1 )
#define TITLE_1PLAYER_1COIN_X		(((MR_SCREEN_WIDTH - TILE2_12PLAYERS_WIDTH) >> 1) - 1)
#define TITLE_1PLAYER_1COIN_Y		((MR_SCREEN_HEIGHT >> 1) + 3)
#define TITLE_2PLAYERS_2COINS_X		(((MR_SCREEN_WIDTH - TILE2_12PLAYERS_WIDTH) >> 1) - 1)
#define TITLE_2PLAYERS_2COINS_Y		((MR_SCREEN_HEIGHT >> 1) + 5)

// SECOND PAGE
#define TITLE_PLAY_X				((MR_SCREEN_WIDTH - TILE1_PLAY_WIDTH) >> 1)
#define TITLE_PLAY_Y				4
#define TITLE_ALIEN_STORM_X			((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1)
#define TITLE_ALIEN_STORM_Y			6
#define TITLE_SCORETABLE_X			((MR_SCREEN_WIDTH - TILE1_SCORETABLE_WIDTH) >> 1)
#define TITLE_SCORETABLE_Y			9
#define TITLE_MISTERY_SCORE_X		((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1)
#define TITLE_MISTERY_SCORE_Y		11
#define TITLE_ALIEN1_SCORE_X		((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1)
#define TITLE_ALIEN1_SCORE_Y		13
#define	TITLE_ALIEN2_SCORE_X		((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1)
#define	TITLE_ALIEN2_SCORE_Y		15
#define	TITLE_ALIEN3_SCORE_X		((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1)
#define	TITLE_ALIEN3_SCORE_Y		17

// PLAYER PAGE

#define TITLE_PLAYER_X		(((MR_SCREEN_WIDTH - TILE2_PLAYER_WIDTH - 2) >> 1))
#define TITLE_PLAYER_Y		((MR_SCREEN_HEIGHT >> 1) + 3)

/****************************************************************************
 ** RESIDENT FUNCTIONS SECTION
 ****************************************************************************/

unsigned char* mr_translate_file_user(mr_file _file) {

    _file = 0;

    return NULL;

}

// Add to the given multibyte (max size = 4 bytes) of a value.
void add(unsigned char* _points, unsigned char* _value) {
	unsigned char i;
	for (i = 0; i < 4; ++i) {
		unsigned char s = _points[i] + _value[i];
		if (s > 9) {
			s -= 10;
			_points[i] = s;
			if (i < 3) {
				++_points[i + 1];
			}
		}
		else {
			_points[i] = s;
		}
	}
}

// Draw the score in the right position, depending on which player 
// we are considering.
void draw_score(mr_position _player) {
	mr_position x, y,i;
	x = _player ? SCORE2_X : SCORE1_X;
	y = _player ? SCORE2_Y : SCORE1_Y;
	mr_putetilesv(x, y, _player ? TILE_SCORE2 : TILE_SCORE1, TILE_SCORE1_WIDTH, TILE_SCORE1_HEIGHT, MR_COLOR_WHITE);
	for (i = 0; i < 4; ++i) {
		mr_puttilev(x + i, y + 1, TILE_DIGIT0 + score[_player][3 - i], MR_COLOR_WHITE);
	}
}

// Draw the hiscore in the right position.
void draw_hiscore() {
	mr_position i;
	mr_putetilesv(HISCORE_X, HISCORE_Y, TILE_HISCORE, TILE_HISCORE_WIDTH, TILE_HISCORE_HEIGHT, MR_COLOR_WHITE);
	for (i = 0; i < 4; ++i) {
		mr_puttilev(HISCORE_X + i, HISCORE_Y + 1, TILE_DIGIT0 + hiscore[3 - i], MR_COLOR_WHITE);
	}
}

// Draw available credit
void draw_credit() {

	mr_putetilesv(CREDIT_X, CREDIT_Y,
		TILE_CREDIT,
		TILE_CREDIT_WIDTH,
		TILE_CREDIT_HEIGHT,
		MR_COLOR_WHITE);

	mr_puttilev(MR_SCREEN_WIDTH - 1, MR_SCREEN_HEIGHT - 1, TILE_DIGIT0 + credit, MR_COLOR_WHITE);
}

// This function contains the functional logic for managing the 
// number of players and the available credit. The routine serves, 
// fundamentally, to wait a certain number of seconds. 
// If, during this wait, the player presses some keys then the 
// wait will be interrupted and certain actions will be performed.
mr_boolean keyboard_controller(unsigned char _seconds) {

	// We wait for a key to be pressed or for a 
	// certain number of seconds to pass.
	unsigned char key = mr_wait_or_get_keypressed(_seconds);

	// Key based check.
	switch (key) {

		// Timeout (no key or unknown key pressed)
	default:
	case MR_KEY_NONE:
		return mr_false;

		// (C) coin
	case MR_KEY_C:
		if (credit < 9) {
			// increase the credit by 1 coin
			++credit;
			// update credit on the screen
			draw_credit();
		}
		break;

		// (1) 1 PLAYER GAME
	case MR_KEY_1:
		// if enough credit...
		if (credit > 0) {
			// decrease the credit by 1 coin
			--credit;
			// update credit on the screen
			draw_credit();
			// set the game for 1 player
			players = 1;
			// let's start!
			return mr_true;
		}
		break;

		// (1) 2 PLAYER GAME
	case MR_KEY_2:
		// if enough credit...
		if (credit > 1) {
			// decrease the credit by 2 coins
			credit -= 2;
			// update credit on the screen
			draw_credit();
			// set the game for 2 players
			players = 2;
			// let's start!
			return mr_true;
		}
		break;
	}

	// continue to wait...
	return mr_false;

}

// Clear the variable part of the welcome screen.
void clear_welcome_screen() {
	mr_putftilesv(0, 3, TILE2_EMPTY, MR_SCREEN_WIDTH - 1, MR_SCREEN_HEIGHT - 4, MR_COLOR_BLACK);
}

// This routine prepares the graphics needed for the game. It loads the 
// resources and it copy them from the mass memory (or from the embedded 
// memory) to the designated zones.
void prepare_graphics() {

	// We set the border and background color to black, to simulate the 
	// absence of a "border" of the game, as with the original one.
	mr_set_border_color(MR_COLOR_BLACK);
	mr_set_background_color(MR_COLOR_BLACK);

	// Load tiles from mass memory.
	mr_tileset_load("zstiles.bin", MR_TILESET_0, TILE_START, TILE_COUNT);
	mr_tileset_load("zstiles1.bin", MR_TILESET_1, TILE1_START, TILE1_COUNT);
#ifdef MR_TILESET_2
	mr_tileset_load("zstiles2.bin", MR_TILESET_2, TILE2_START, TILE2_COUNT);
#endif

	// Let's prepare the cannon animation.
	mr_tile_prepare_horizontal_extended( MR_TILESET_0, TILE_CANNON, 
											TILE_CANNON_WIDTH, TILE_CANNON_HEIGHT, 
											TILE_MOVING_CANNON);

	// This little "trick" allows us to quickly clean the screen 
	// using the prepared empty tile.
	MR_RENDERED_MIXELS[0] = TILE_EMPTY;
}

// This routine draws the presentation (or welcome) screen. This screen suggests 
// inserting a coin to play and, if inserted, allows you to choose whether to 
// play alone or in pairs. It also shows the score that can be obtained by 
// hitting the various elements of the game.
void welcome_screen() {

	// Let's start by clearing the screen.
	mr_clearv();

	// Let's draw the upper and lower part of the screen, the one that 
	// contains the total score and the maximum. At the bottom we draw 
	// the available credit. This part will remain fixed.
	draw_score(PLAYER_1);
	draw_hiscore();
	draw_score(PLAYER_2);
	draw_credit();

	// Endless loop (until a valid key is pressed).
	while (1) {

#ifdef MR_TILESET_2
		//---------------------------------------------- FIRST SCREEN:
		// We suggest to insert the coin to play:
		//		1 player = 1 coin
		//		2 players = 2 coins.

		clear_welcome_screen();

		// Let's change the tile set on the fly, in order 
		// to have the right tiles for writing on the screen.
		mr_tileset_visible(MR_TILESET_2);

		// "INSERT COIN"
		mr_putetilesv(TITLE_INSERTCOIN_X, TITLE_INSERTCOIN_Y, 
							TILE2_INSERTCOIN, 
							TILE2_INSERTCOIN_WIDTH, 
							TILE2_INSERTCOIN_HEIGHT, 
							MR_COLOR_WHITE);

		// "1 OR 2 PLAYERS"
		mr_putetilesv(TITLE_12PLAYERS_X, TITLE_12PLAYERS_Y, 
							TILE2_12PLAYERS, 
							TILE2_12PLAYERS_WIDTH, 
							TILE2_12PLAYERS_HEIGHT, 
							MR_COLOR_WHITE);

		// Wait 1 second or press certain keys, 
		// some of which can take you out of 
		// the welcome screen and start playing.
		if (keyboard_controller(1)) return;

		// "1 PLAYER = 1 COIN"
		mr_puttilev(TITLE_1PLAYER_1COIN_X, TITLE_1PLAYER_1COIN_Y, TILE2_DIGIT1, MR_COLOR_WHITE);
		mr_putetilesv(TITLE_1PLAYER_1COIN_X + 1, TITLE_1PLAYER_1COIN_Y, 
						TILE2_PLAYER, 
						TILE2_PLAYER_WIDTH, 
						TILE2_PLAYER_HEIGHT, 
						MR_COLOR_WHITE);
		mr_puttilev(TITLE_1PLAYER_1COIN_X + 1 + TILE2_PLAYERS_WIDTH, TITLE_1PLAYER_1COIN_Y, TILE2_DIGIT1, MR_COLOR_WHITE);
		mr_putetilesv(TITLE_1PLAYER_1COIN_X + 1 + TILE2_PLAYERS_WIDTH + 1, TITLE_1PLAYER_1COIN_Y, 
						TILE2_COIN, 
						TILE2_COIN_WIDTH, 
						TILE2_COIN_HEIGHT, 
						MR_COLOR_WHITE);

		// Wait 1 second or press certain keys, 
		// some of which can take you out of 
		// the welcome screen and start playing.
		if (keyboard_controller(1)) return;

		// "2 COINS = 2 PLAYERS"
		mr_puttilev(TITLE_2PLAYERS_2COINS_X, TITLE_2PLAYERS_2COINS_Y, TILE2_DIGIT2, MR_COLOR_GREEN);
		mr_putetilesv(TITLE_2PLAYERS_2COINS_X+1, TITLE_2PLAYERS_2COINS_Y, 
						TILE2_PLAYERS, 
						TILE2_PLAYERS_WIDTH, 
						TILE2_PLAYER_HEIGHT, 
						MR_COLOR_GREEN);
		mr_puttilev(TITLE_2PLAYERS_2COINS_X + 1 + TILE2_PLAYERS_WIDTH, TITLE_2PLAYERS_2COINS_Y, TILE2_DIGIT2, MR_COLOR_GREEN);
		mr_putetilesv(TITLE_2PLAYERS_2COINS_X + 1 + TILE2_PLAYERS_WIDTH + 1, TITLE_2PLAYERS_2COINS_Y, 
						TILE2_COINS, 
						TILE2_COINS_WIDTH, 
						TILE2_COINS_HEIGHT, 
						MR_COLOR_GREEN);

		// Wait 3 seconds or press certain keys, 
		// some of which can take you out of 
		// the welcome screen and start playing.
		if (keyboard_controller(3)) return;

#endif

		//------------------------------------------- SECOND SCREEN:
		// We suggest the scores that can be obtained 
		// by hitting the various aliens.

		clear_welcome_screen();

		// Let's change the tile set on the fly, in order 
		// to have the right tiles for writing on the screen.
		mr_tileset_visible(MR_TILESET_1);

		// "PLAY"
		mr_putetilesv(TITLE_PLAY_X, TITLE_PLAY_Y, 
						TILE1_PLAY, 
						TILE1_PLAY_WIDTH, 
						TILE1_PLAY_HEIGHT, 
						MR_COLOR_WHITE);

		// Wait 1 second or press certain keys, 
		// some of which can take you out of 
		// the welcome screen and start playing.
		if (keyboard_controller(1)) return;

		// "ALIEN STORM"
		mr_putetilesv(TITLE_ALIEN_STORM_X, TITLE_ALIEN_STORM_Y, 
						TILE1_TITLE, 
						TILE1_TITLE_WIDTH, 
						TILE1_TITLE_HEIGHT, 
						MR_COLOR_WHITE);

		// Wait 1 second or press certain keys, 
		// some of which can take you out of 
		// the welcome screen and start playing.
		if (keyboard_controller(1)) return;

		// "SCORETABLE"
		mr_putetilesv(TITLE_SCORETABLE_X, TITLE_SCORETABLE_Y, 
						TILE1_SCORETABLE, 
						TILE1_SCORETABLE_WIDTH, 
						TILE1_SCORETABLE_HEIGHT, 
						MR_COLOR_WHITE);

		// Wait 1 second or press certain keys, 
		// some of which can take you out of 
		// the welcome screen and start playing.
		if (keyboard_controller(1)) return;

		// (mistery ship icon)
		mr_putetilesv(TITLE_MISTERY_SCORE_X, TITLE_MISTERY_SCORE_Y, 
						TILE1_MYSTERO, 
						TILE1_MYSTERO_WIDTH, 
						TILE1_MYSTERO_HEIGHT, 
						MR_COLOR_WHITE);

		// Wait 1 second or press certain keys, 
		// some of which can take you out of 
		// the welcome screen and start playing.
		if (keyboard_controller(1)) return;

		// "? MISTERY"
		mr_putetilesv(TITLE_MISTERY_SCORE_X + TILE1_MYSTERO_WIDTH + 2, TITLE_MISTERY_SCORE_Y, 
						TILE1_MISTERY, 
						TILE1_MISTERY_WIDTH, 
						TILE1_MISTERY_HEIGHT, 
						MR_COLOR_WHITE);

		// Wait 1 second or press certain keys, 
		// some of which can take you out of 
		// the welcome screen and start playing.
		if (keyboard_controller(1)) return;

		// (alien 1 icon)
		mr_putetilesv(TITLE_ALIEN1_SCORE_X, TITLE_ALIEN1_SCORE_Y, 
						TILE1_ALIEN1A, 
						TILE1_ALIEN1A_WIDTH, 
						TILE1_ALIEN1A_HEIGHT, 
						MR_COLOR_WHITE);

		// Wait 1 second or press certain keys, 
		// some of which can take you out of 
		// the welcome screen and start playing.
		if (keyboard_controller(1)) return;

		// "= 30 POINTS"
		mr_puttilev(TITLE_ALIEN1_SCORE_X + TILE1_MYSTERO_WIDTH + 2, TITLE_ALIEN1_SCORE_Y, TILE1_DIGIT3, MR_COLOR_WHITE);
		mr_puttilev(TITLE_ALIEN1_SCORE_X + TILE1_MYSTERO_WIDTH + 2 + 1, TITLE_ALIEN1_SCORE_Y, TILE1_DIGIT0, MR_COLOR_WHITE);
		mr_putetilesv(TITLE_ALIEN1_SCORE_X + TILE1_MYSTERO_WIDTH + 2 + 2, TITLE_ALIEN1_SCORE_Y, 
						TILE1_POINTS, 
						TILE1_POINTS_WIDTH, 
						TILE1_POINTS_HEIGHT, 
						MR_COLOR_WHITE);

		// Wait 1 second or press certain keys, 
		// some of which can take you out of 
		// the welcome screen and start playing.
		if (keyboard_controller(1)) return;

		// (alien 2 icon)
		mr_putetilesv(TITLE_ALIEN2_SCORE_X, TITLE_ALIEN2_SCORE_Y, 
						TILE1_ALIEN2A, 
						TILE1_ALIEN2A_WIDTH, 
						TILE1_ALIEN2A_HEIGHT, 
						MR_COLOR_WHITE);

		// Wait 1 second or press certain keys, 
		// some of which can take you out of 
		// the welcome screen and start playing.
		if (keyboard_controller(1)) return;

		// "= 20 POINTS"
		mr_puttilev(TITLE_ALIEN2_SCORE_X + TILE1_MYSTERO_WIDTH + 2, 15, TILE1_DIGIT2, MR_COLOR_WHITE);
		mr_puttilev(TITLE_ALIEN2_SCORE_X + TILE1_MYSTERO_WIDTH + 2 + 1, 15, TILE1_DIGIT0, MR_COLOR_WHITE);
		mr_putetilesv(TITLE_ALIEN2_SCORE_X + TILE1_MYSTERO_WIDTH + 2 + 2, 15, 
						TILE1_POINTS, 
						TILE1_POINTS_WIDTH, 
						TILE1_POINTS_HEIGHT, 
						MR_COLOR_WHITE);

		// Wait 1 second or press certain keys, 
		// some of which can take you out of 
		// the welcome screen and start playing.
		if (keyboard_controller(1)) return;

		// (alien 3 icon)
		mr_putetilesv(TITLE_ALIEN3_SCORE_X, TITLE_ALIEN3_SCORE_Y, 
						TILE1_ALIEN1A, 
						TILE1_ALIEN1A_WIDTH, 
						TILE1_ALIEN1A_HEIGHT, 
						MR_COLOR_GREEN);

		// Wait 1 second or press certain keys, 
		// some of which can take you out of 
		// the welcome screen and start playing.
		if (keyboard_controller(1)) return;

		// "= 10 POINTS"
		mr_puttilev(TITLE_ALIEN3_SCORE_X + TILE1_MYSTERO_WIDTH + 2, TITLE_ALIEN3_SCORE_Y, TILE1_DIGIT1, MR_COLOR_GREEN);
		mr_puttilev(TITLE_ALIEN3_SCORE_X + TILE1_MYSTERO_WIDTH + 2 + 1, TITLE_ALIEN3_SCORE_Y, TILE1_DIGIT0, MR_COLOR_GREEN);
		mr_putetilesv(TITLE_ALIEN3_SCORE_X + TILE1_MYSTERO_WIDTH + 2 + 2, TITLE_ALIEN3_SCORE_Y, 
						TILE1_POINTS, 
						TILE1_POINTS_WIDTH, 
						TILE1_POINTS_HEIGHT, 
						MR_COLOR_GREEN);

		// Wait 3 seconds or press certain keys, 
		// some of which can take you out of 
		// the welcome screen and start playing.
		if (keyboard_controller(3)) return;

	}

}

// This routine takes care of initializing all the various 
// variables that may have been changed from a previous run.
void initialize_game() {
	unsigned char i, j;

	level = 0;

	////////////////////////////////////// PLAYER
	cannonX = CANNON_START_X;
	fireY = 0;
	fireX = 0;
	fireDelayFrameCount = FIRE_DELAY_FRAME_COUNT;

	////////////////////////////////////// ALIENS
	alienFrameCounts = 0;
	alienFrame = 0;
	for (i = 0; i < ALIEN_ROW_COUNT; ++i) {
		for (j = 0; j < ALIEN_COLUMN_COUNT; ++j) {
			switch (i) {
			case 0:
				alienMatrix[i][j] = TILE_ALIEN3A;
				break;
			case 1:
			case 2:
				alienMatrix[i][j] = TILE_ALIEN2A;
				break;
			case 3:
			case 4:
				alienMatrix[i][j] = TILE_ALIEN1A;
				break;
			}
		}
	}
	for (j = 0; j < ALIEN_COLUMN_COUNT; ++j) {
		alienColumn[j] = j * ALIEN_WIDTH;
		alienFireY[j] = 0;
		alienFireX[j] = 0;
		alienMaxRowPerColumn[j] = ALIEN_ROW_COUNT;
	}
	alienX = 0;
	alienPX = 0;
	alienDX = 1;
	alienY = 1;
	alienPY = 1;
	alienDelayFrameCount = ALIEN_DELAY_FRAME_COUNT;
	alienFireDelayFrameCount = ALIEN_FIRE_DELAY_FRAME_COUNT;
	alienRowBitmap = 0;
	for (i = 0; i < ALIEN_ROW_COUNT; ++i) {
		alienRowBitmap = alienRowBitmap | (1 << i);
	}
	alienMaxRow = 0;
	leftFreeColumns = 0;
	rightFreeColumns = 0;

	////////////////////////////////////// GAMEPLAY

	levelFinished = mr_false;
	gameOver = mr_false;

	// Let's adjust the speed of the game 
	// to be the default to start with.
	alienDelayFrameCount = ALIEN_DELAY_FRAME_COUNT;

	// We set the overall lives for each player 
	// (this applies even if there is only one player!)
	cannonLives[0] = MAX_LIFES;
	cannonLives[1] = MAX_LIFES;

	// Reset scores.
	score[0][0] = 0;
	score[0][1] = 0;
	score[0][2] = 0;
	score[0][3] = 0;
	score[1][0] = 0;
	score[1][1] = 0;
	score[1][2] = 0;
	score[1][3] = 0;

	// By default, the game has not over 
	// and the level is not finished, yet.
	gameOver = mr_false;
	levelFinished = mr_false;

}

// This routine takes care of initializing all the various 
// variables that may have been changed from a previous run.
void initialize_level() {
	unsigned char i, j;

	////////////////////////////////////// PLAYER
	cannonX = CANNON_START_X;
	fireY = 0;
	fireX = 0;
	fireDelayFrameCount = FIRE_DELAY_FRAME_COUNT;

	////////////////////////////////////// ALIENS
	alienFrameCounts = 0;
	alienFrame = 0;
	for (i = 0; i < ALIEN_ROW_COUNT; ++i) {
		for (j = 0; j < ALIEN_COLUMN_COUNT; ++j) {
			switch (i) {
			case 0:
				alienMatrix[i][j] = TILE_ALIEN3A;
				break;
			case 1:
			case 2:
				alienMatrix[i][j] = TILE_ALIEN2A;
				break;
			case 3:
			case 4:
				alienMatrix[i][j] = TILE_ALIEN1A;
				break;
			}
		}
	}
	for (j = 0; j < ALIEN_COLUMN_COUNT; ++j) {
		alienColumn[j] = j * ALIEN_WIDTH;
		alienFireY[j] = 0;
		alienFireX[j] = 0;
		alienMaxRowPerColumn[j] = ALIEN_ROW_COUNT;
	}
	alienX = 0;
	alienPX = 0;
	alienDX = 1;
	alienY = 1;
	alienPY = 1;
	if (level < 10) {
		alienDelayFrameCount = ((ALIEN_DELAY_FRAME_COUNT)+(10-level));
	}
	else {
		alienDelayFrameCount = ALIEN_DELAY_FRAME_COUNT;
	}
	alienFireDelayFrameCount = (ALIEN_FIRE_DELAY_FRAME_COUNT);
	alienRowBitmap = 0;
	for (i = 0; i < ALIEN_ROW_COUNT; ++i) {
		alienRowBitmap = alienRowBitmap | (1 << i);
	}
	alienMaxRow = 0;
	leftFreeColumns = 0;
	rightFreeColumns = 0;

	////////////////////////////////////// GAMEPLAY

	levelFinished = mr_false;
	gameOver = mr_false;

}

void clear_cannon_row() {
	mr_putftilesv(0, CANNON_START_Y, 
					TILE_EMPTY, 
					MR_SCREEN_WIDTH, 
					TILE_CANNON_HEIGHT, 
					MR_COLOR_BLACK);
}

// Draw the player's cannon
void draw_cannon() {
	// If cannon has been hit, we clear the previous explosion first.
	if (cannonHit > 1) {
		--cannonHit;
		mr_sound_start_channel(0, 0);
		mr_sound_change_channel(0, 500+100*(cannonHit&0x03));
	} else if (cannonHit==1) {
		clear_cannon_row();
		mr_sound_stop_channel(0);
		--cannonHit;
		cannonX = CANNON_START_X;
	// Otherwise, we draw it
	} else {
		mr_tile_moveto_horizontal_extendedv( cannonX, CANNON_START_Y * 8,
											TILE_MOVING_CANNON, 
											TILE_CANNON_WIDTH, TILE_CANNON_HEIGHT, 
											MR_COLOR_GREEN);
	}
}

// Clear the player's cannon (with an explosion)
void clear_cannon() {
	clear_cannon_row();
	mr_putetilesv( (cannonX >> 3), CANNON_START_Y,
						TILE_EXPLOSION, 
						TILE_EXPLOSION_WIDTH, 
						TILE_EXPLOSION_HEIGHT, 
						MR_COLOR_WHITE);
}


// Draw a set of bunkers
void draw_defense() {
	unsigned char i;
	for (i = 0; i < BUNKER_COUNT; ++i) {
		mr_putetilesv(	BUNKER_X(i), 
						BUNKER_Y, 
						TILE_DEFENSE, 
						TILE_DEFENSE_WIDTH, 
						TILE_DEFENSE_HEIGHT, MR_COLOR_GREEN);
	}
}

// Draw a moving mistery ship.
void draw_mistery() {
	if (misteryX == 0) {
		if (misteryXP != misteryX) {
			mr_putftilesv(	misteryXP, 
							MISTERY_Y, 
							TILE_EMPTY, 
							TILE_MYSTERO_WIDTH, 
							TILE_MYSTERO_HEIGHT, 
							MR_COLOR_BLACK);
		}
	}
	else if (misteryX > 0) {
		mr_putetilesv(	misteryX - 1, 
						MISTERY_Y,
						TILE_EMPTY, 
						TILE_EMPTY_WIDTH, 
						TILE_EMPTY_HEIGHT, 
						MR_COLOR_BLACK);

		mr_putetilesv(	misteryX, 
						MISTERY_Y,
						TILE_MYSTERO, 
						TILE_MYSTERO_WIDTH, 
						TILE_MYSTERO_HEIGHT, 
						MR_COLOR_RED);
	}
	misteryXP = misteryX;
}

void clear_cannons() {
	mr_putftilesv(CANNONS_X, CANNONS_Y,
		TILE_EMPTY,
		CANNONS_WIDTH, CANNONS_HEIGHT, MR_COLOR_BLACK);
}

// Draw available cannons
void draw_cannons() {
	unsigned char i;
	clear_cannons();
	mr_puttilev(CANNONS_X, CANNONS_Y, TILE_DIGIT0 + cannonLives[currentPlayer], MR_COLOR_WHITE);
	for (i = 0; i < cannonLives[currentPlayer]; ++i) {
		mr_putetilesv(	CANNONS_X + 1 + ((i + 1) * TILE_CANNON_WIDTH), 
						CANNONS_Y, 
						TILE_CANNON, 
						TILE_CANNON_WIDTH, 
						TILE_CANNON_HEIGHT, 
						MR_COLOR_GREEN);
	}
}

// Draw an explosion where the alien is.
void explode_alien(x, y) {
	mr_putetilesv(	alienX + ALIEN_HORIZONTAL_OFFSET + alienColumn[x], 
					alienY + ALIEN_VERTICAL_OFFSET + y * (TILE_ALIEN1A_HEIGHT + TILE_ALIEN1A_HEIGHT), 
					TILE_EXPLOSION, 
					TILE_EXPLOSION_WIDTH, 
					TILE_EXPLOSION_HEIGHT, 
					MR_COLOR_WHITE);
}

// Erase the alien at the given location
void clear_alien(x, y) {
	mr_putftilesv(	alienX + ALIEN_HORIZONTAL_OFFSET + alienColumn[x], 
					alienY + ALIEN_VERTICAL_OFFSET + y * (TILE_ALIEN1A_HEIGHT + TILE_ALIEN1A_HEIGHT), 
					TILE_EMPTY, 
					TILE_EXPLOSION_WIDTH, 
					TILE_EXPLOSION_HEIGHT, 
					MR_COLOR_WHITE);
}

void erase_aliens(row) {
	mr_htilesv(		alienPX + ALIEN_HORIZONTAL_OFFSET, 
					alienPX + ALIEN_HORIZONTAL_OFFSET + (ALIEN_COLUMN_COUNT) * (TILE_ALIEN1A_WIDTH + (TILE_ALIEN1A_WIDTH >> 1)), 
					alienPY + ALIEN_VERTICAL_OFFSET + row * (TILE_ALIEN1A_HEIGHT + TILE_ALIEN1A_HEIGHT), 
					TILE_EMPTY, 
					MR_COLOR_BLACK);
}

// Draw an alien
void draw_alien(x, y) {
	mr_putetilesv(	alienX + ALIEN_HORIZONTAL_OFFSET + alienColumn[x], 
					alienY + ALIEN_VERTICAL_OFFSET + y * (TILE_ALIEN1A_HEIGHT + TILE_ALIEN1A_HEIGHT), 
					alienMatrix[y][x] + (alienFrame * TILE_ALIEN1A_WIDTH * TILE_ALIEN1A_HEIGHT), 
					TILE_ALIEN1A_WIDTH, 
					TILE_ALIEN1A_HEIGHT, 
					MR_COLOR_WHITE);
}

// This routine deals with drawing the array of aliens that are descending 
// towards the player. It also detects any impacts with existing aliens.
void draw_aliens() {

	unsigned char i, j;
	mr_position alienRowBitmapMask = 0;
	mr_boolean alienPresent = mr_false;

	// We initialize the control structures.
	for (i = 0; i < ALIEN_COLUMN_COUNT; ++i) {
		alienMaxRowPerColumn[i] = 0xff;
	}

	alienMaxRow = 0;

	// We explore the alien matrix, to update the structures
	// and to draw them(if necessary).
	for (i = 0; i < ALIEN_ROW_COUNT; ++i) {

		// We calculate the bit mask corresponding 
		// to the examined row.
		alienRowBitmapMask = (1 << i);

		// If the corresponding bit is 1, it means that 
		// there is at least one alien to draw.
		if (alienRowBitmap & alienRowBitmapMask) {

			// If the vertical or horizontal position of the array 
			// of aliens has changed, then it's time to delete 
			// the entire line immediately.
			if ((alienPX != alienX) || (alienPY != alienY)) {
				erase_aliens(i);
			}

			// For the moment this is the highest line we have explored.
			alienMaxRow = i;

			// We check the status of every single alien on the row.
			for (j = 0; j < ALIEN_COLUMN_COUNT; ++j) {

				// Is there an alien?
				if (alienMatrix[i][j]) {

					// Is it an exploding alien?
					if (alienMatrix[i][j] == 0xff) {
						clear_alien(j, i);
						alienMatrix[i][j] = 0;

					// Is it present?
					} else {
						alienPresent = mr_true;
						draw_alien(j, i);
						alienMaxRowPerColumn[j] = i;
					}
				}
			}
			// If from the precise exploration of the whole row we do 
			// not find aliens, mark it to avoid repeating the loop.
			if (!alienPresent) {
				alienRowBitmap = alienRowBitmap & ~(1 << i);
			}
		}
	}

	leftFreeColumns = 0;
	for (i = 0; i < (ALIEN_COLUMN_COUNT>>1); ++i) {
		if (alienMaxRowPerColumn[i] == 0xff) {
			leftFreeColumns = (i+1);
		}
		else {
			break;
		}
	}

	rightFreeColumns = 0;
	for (i = (ALIEN_COLUMN_COUNT-1); i > (ALIEN_COLUMN_COUNT >> 1); --i) {
		if (alienMaxRowPerColumn[i] == 0xff) {
			rightFreeColumns = (ALIEN_COLUMN_COUNT-i);
		}
		else {
			break;
		}
	}

	// Finally, we update the previous values.
	alienPX = alienX;
	alienPY = alienY;

}

// This function allows you to draw an interstitial screen, 
// to inform you that the next game must be played by 
// player 1 or by player 2.
void player_screen() {

#ifdef MR_TILESET_2

	clear_welcome_screen();

	clear_cannons();

	// We set the proper tileset to print the writings.
	mr_tileset_visible(MR_TILESET_2);

	// "PLAYER 1" or "PLAYER 2"
	mr_putetilesv(TITLE_PLAYER_X, TITLE_PLAYER_Y, 
					TILE2_PLAYER, 
					TILE2_PLAYER_WIDTH, 
					TILE2_PLAYER_HEIGHT, 
					MR_COLOR_WHITE);
	mr_puttilev(TITLE_PLAYER_X + TILE2_PLAYER_WIDTH + 1, TITLE_PLAYER_Y, currentPlayer?TILE2_DIGIT2:TILE2_DIGIT1, MR_COLOR_WHITE);

	// Wait just 2 seconds (or key press)
	mr_wait_or_keypress(2);

	clear_welcome_screen();

	// We set the proper tileset to play.
	mr_tileset_visible(MR_TILESET_0);

#endif

}

// This routine deals with drawing the shot fired 
// by the player and checking if it hit one of 
// the aliens, or a bunker.
void check_player_fire() {
	unsigned char i, j;

	// If the fire reach the top of the screen, 
	// we must clear the screen occupied by the fire 
	// and stop it.
	if (fireY == 2) {
		mr_puttilev(fireX, fireY + 1, TILE_EMPTY, MR_COLOR_BLACK);
		fireY = 0;
		mr_sound_stop_channel(0);
	// Otherwise, if it's still moving ...
	} else if (fireY > 0) {

		// We recover the tile it is about to overwrite.
		mr_tile tile = mr_gettilev(fireX, fireY);

		// We clear the previous space occupied.
		mr_puttilev(fireX, fireY + 1, TILE_EMPTY, MR_COLOR_BLACK);

		// Let's see if the player hit the mystery ship.
		if ((tile >= TILE_MYSTERO) && (tile < (TILE_MYSTERO + (TILE_MYSTERO_WIDTH * TILE_MYSTERO_HEIGHT)))) {

			// The mystery ship has been hit: add 100 points to the score 
			// of the player who is playing, and update the score on 
			// the screen.
			addendum[3] = 0;
			addendum[2] = 1;
			addendum[1] = 0;
			addendum[0] = 0;
			add(score[currentPlayer], addendum);
			draw_score(currentPlayer);

			// Clear the space occupied and stop fire & mistery ship.
			mr_puttilev(fireX, fireY, TILE_EMPTY, MR_COLOR_BLACK);
			fireY = 0;
			misteryX = 0;

			--alienDelayFrameCount;
			if (alienDelayFrameCount == 0) {
				alienDelayFrameCount = 1;
			}

			mr_sound_stop_channel(0);
		}
		// Let's see if it hit a bunker.
		else if ((tile >= TILE_DEFENSE) && (tile < (TILE_DEFENSE + (TILE_DEFENSE_WIDTH * TILE_DEFENSE_HEIGHT)))) {
			// Clear the space occupied and stop fire.
			mr_puttilev(fireX, fireY, TILE_EMPTY, MR_COLOR_BLACK);
			fireY = 0;
			mr_sound_stop_channel(0);
		}
		// Let's see if it hit an alien.
		else if ((tile >= TILE_ALIEN1A) && (tile < (TILE_ALIEN3B + (TILE_ALIEN3B_WIDTH * TILE_ALIEN3B_HEIGHT)))) {

			// The alien ship has been hit: add points to the score 
			// of the player who is playing, and update the score on 
			// the screen.
			addendum[3] = 0;
			addendum[2] = 0;
			if (tile >= TILE_ALIEN3A) {
				addendum[1] = 3;
			}
			else if (tile >= TILE_ALIEN2A) {
				addendum[1] = 2;
			}
			else if (tile >= TILE_ALIEN1A) {
				addendum[1] = 1;
			}
			else {
				addendum[1] = 0;
			}
			addendum[0] = 0;
			add(score[0], addendum);
			draw_score(currentPlayer);

			// We need to figure out which column 
			// the affected alien belongs to.
			for (j = 0; j < ALIEN_COLUMN_COUNT; ++j) {
				if ((fireX - alienX - ALIEN_HORIZONTAL_OFFSET) <= (alienColumn[j] + TILE_ALIEN1A_WIDTH)) {
					break;
				}
			}
			// And which row belongs to.
			i = (fireY - alienY - ALIEN_VERTICAL_OFFSET) >> 1;

			// We update the status matrix, representing that 
			// specific position as an "explosion".
			alienMatrix[i][j] = 0xff;
			
			// Explode the alien and stop fire.
			explode_alien(j, i);
			fireY = 0;
			mr_sound_stop_channel(0);
		}

		// Let's animate the shot.
		if (fireY > 2) {
			mr_puttilev(fireX, fireY, TILE_LASER, MR_COLOR_YELLOW);
		}
	}
}

// This routine deals with drawing the shot fired 
// by the alien and checking if it hit the player
// or a bunker.
void check_alien_fire() {
	
	unsigned char i;

	// Move the alien's fire on correct frame.
	if (alienFireY > 0) {
		--alienFireDelayFrameCount;
		if (alienFireDelayFrameCount == 0) {

			// We have to look for the only active shots.
			for (i = 0; i < ALIEN_COLUMN_COUNT; ++i) {

				// Skip if not active.
				if (alienFireY[i] == 0) continue;

				// If the shot can still move.
				if (alienFireY[i] <= (MR_SCREEN_HEIGHT - 2)) {

					// We recover the tile it is about to overwrite.
					mr_tile tile = mr_gettilev(alienFireX[i], alienFireY[i]);
					// We clear the previous.
					mr_puttilev(alienFireX[i], alienFireY[i] - 1, TILE_EMPTY, MR_COLOR_BLACK);

					// If the shot can still move.
					if (alienFireY[i] < (MR_SCREEN_HEIGHT - 2)) {
						mr_puttilev(alienFireX[i], alienFireY[i], TILE_LASER2, MR_COLOR_WHITE);
					}

					// Check if the alien hit a bunker.
					if ((tile >= TILE_DEFENSE) && (tile < (TILE_DEFENSE + (TILE_DEFENSE_WIDTH * TILE_DEFENSE_HEIGHT)))) {
						// Destroy the bunker and stop it.
						mr_puttilev(alienFireX[i], alienFireY[i], TILE_EMPTY, MR_COLOR_BLACK);
						alienFireY[i] = 0;
					}

					// Check if the alien hit the player.
					else if (( alienFireX[i] == (cannonX>>3) || alienFireX[i] == ((cannonX >> 3)+1) ) && alienFireY[i] == CANNON_START_Y) {

						// Reset and stop the shot.
						mr_puttilev(alienFireX[i], alienFireY[i], TILE_EMPTY, MR_COLOR_BLACK);
						alienFireY[i] = 0;

						// Player has been hit: explode the cannon and decrease the cannons.
						cannonHit = 40;
						clear_cannon();
						if (cannonLives[currentPlayer] > 0) {
							--cannonLives[currentPlayer];
						}
						draw_cannons();

					}

					// If the shot can still move.
					else if (alienFireY[i] < (MR_SCREEN_HEIGHT - 2)) {
						++alienFireY[i];
					}
					// Otherwise, stop & clear.
					else {
						mr_puttilev(alienFireX[i], alienFireY[i], TILE_EMPTY, MR_COLOR_BLACK);
						alienFireY[i] = 0;
					}
				}
			}
			// Reset the delay timing.
			alienFireDelayFrameCount = ALIEN_FIRE_DELAY_FRAME_COUNT;
		}
	}
}

void control_cannon() {
	if (cannonHit == 0) {
		unsigned char joy = mr_joy(MR_JOYSTICK_0);
		if ((joy & MR_JOYSTICK_RIGHT) == MR_JOYSTICK_RIGHT) {
			if (cannonX < (MR_SCREEN_WIDTH - TILE_CANNON_WIDTH) * 8) {
				++cannonX;
			}
		}
		else if ((joy & MR_JOYSTICK_LEFT) == MR_JOYSTICK_LEFT) {
			if (cannonX > 0) {
				--cannonX;
			}
		}
		if ((joy & MR_JOYSTICK_FIRE) == MR_JOYSTICK_FIRE) {
			// The player has an unlimited number of bullets 
			// but can only fire one shot at a time.
			if (fireY == 0) {
				fireY = MR_SCREEN_HEIGHT - 4;
				mr_tile_redefine_fill(MR_TILESET_0, TILE_LASER, 1 << (0x07 - (cannonX & 0x07)));
				fireX = (cannonX >> 3) + 1;
			}
		}
		if (!joy) {
			switch (mr_get_key_pressed()) {
			case MR_KEY_A:
				if (cannonX > 0) {
					--cannonX;
				}
				break;
			case MR_KEY_D:
				if (cannonX < (MR_SCREEN_WIDTH - TILE_CANNON_WIDTH) * 8) {
					++cannonX;
				}
				break;
			case MR_KEY_SPACE:
				// The player has an unlimited number of bullets 
				// but can only fire one shot at a time.
				if (fireY == 0) {
					fireY = MR_SCREEN_HEIGHT - 4;
					mr_tile_redefine_fill(MR_TILESET_0, TILE_LASER, 1 << (0x07 - (cannonX & 0x07)));
					fireX = (cannonX >> 3) + 1;
				}
				break;
			}
		}
	}
}

void play_alien_note() {
	mr_sound_start_channel(1, 0);
	mr_sound_change_channel(1, 1000 + (3 - alienSoundtrack) * 100);
	++alienSoundtrack;
	if (alienSoundtrack > 3) alienSoundtrack = 0;
}

void stop_alien_note() {
	mr_sound_stop_channel(1);
}

// This routine takes care of moving the aliens, if necessary.
void move_aliens() {

	unsigned char i;

	++alienFrameCounts;

	if (alienFrameCounts == (alienDelayFrameCount >> 1)) {
		stop_alien_note();
	}

	if (alienFrameCounts == alienDelayFrameCount) {

		// Invert alien's animation frame.
		alienFrame ^= 0x01;
		alienFrameCounts = 0;

		// We look to see if there are spaces to shoot.
		for (i = 0; i < ALIEN_COLUMN_COUNT; ++i) {
			if (rand() > (RAND_MAX >> 1)) continue;
			if ((alienFireY[i] == 0) && (alienMaxRowPerColumn[i] > 0) && (alienMaxRowPerColumn[i] != 0xff)) {
				alienFireY[i] = alienY + ALIEN_VERTICAL_OFFSET + (alienMaxRowPerColumn[i] + 1) * (TILE_ALIEN1A_HEIGHT + TILE_ALIEN1A_HEIGHT) + 1;
				alienFireX[i] = alienX + ALIEN_HORIZONTAL_OFFSET + alienColumn[i];
				break;
			}
		}

		// The stages of approach of the aliens to the world follow a 
		// unique pattern, a wideand orderly zig - zag that leads them 
		// slowly but surely to reach the bottom of the screen decreeing 
		// the invasion and the consequent end of the game.

		if (alienFrame == 1) {
			alienX += alienDX;
			if (alienDX == 0) {
				++alienY;
				if (alienX > ( ( ALIEN_HORIZONTAL_ELONGATION ) + (rightFreeColumns * ALIEN_WIDTH) ) ) {
					alienDX = -1;
				}
				else {
					alienDX = 1;
				}
			}
			else if (alienX > ((ALIEN_HORIZONTAL_ELONGATION)+ (rightFreeColumns * ALIEN_WIDTH))) {
				alienDX = 0;
			}
			else if (alienX <= -((ALIEN_HORIZONTAL_ELONGATION)+ (leftFreeColumns * ALIEN_WIDTH))) {
				alienDX = 0;
			}
		}

		draw_aliens();

		play_alien_note();
	}

}

void move_mistery() {
	if (misteryX == (MR_SCREEN_WIDTH - TILE_MYSTERO_WIDTH)) {
		misteryX = 0;
	}
	else if (misteryX > 0) {
		--misteryDelayFrameCount;
		if (misteryDelayFrameCount == 0) {
			misteryDelayFrameCount = MISTERY_DELAY_FRAME_COUNT;
			++misteryX;
		}
	}

	if (misteryX == 0) {
		if ((rand() & 0xff) > 254) {
			misteryX = 1;
		}
	}
}

void move_fire() {
	if (fireY > 2) {
		--fireDelayFrameCount;
		if (fireDelayFrameCount == 0) {
			fireDelayFrameCount = FIRE_DELAY_FRAME_COUNT;
			--fireY;
			mr_sound_start_channel(0, 0);
			mr_sound_change_channel(0, 2000 + (MR_SCREEN_HEIGHT - fireY) * 300);
		}
	}
}

void check_end_conditions() {
	if (alienRowBitmap == 0) {
		levelFinished = mr_true;
		gameOver = mr_false;
	}

	if ((alienMaxRow * 2 + alienY + ALIEN_VERTICAL_OFFSET) > (MR_SCREEN_HEIGHT - 2)) {
		levelFinished = mr_true;
		gameOver = mr_true;
	}

	if (cannonLives[currentPlayer] == 0 && cannonHit==0) {
		levelFinished = mr_true;
		if (players == 1 || (players == 2 && currentPlayer == 1)) {
			gameOver = mr_true;
		}
		else {
			++currentPlayer;
			gameOver = mr_false;
		}
	}
}

// This is the main loop of the game.
void gameloop() {

	mr_clearv();

	// First of all, let's activate the 
	// correct tileset for animations.
	mr_tileset_visible(MR_TILESET_0);

	initialize_game();

	// The first player starts playing. If there is a second player, 
	// then we show the player's interstitial page.
	currentPlayer = 0;
	if (players > 1) {
		player_screen();
	}

	// We prepare the top and bottom of the screen.
	draw_score(PLAYER_1);
	draw_hiscore();
	draw_score(PLAYER_2);
	draw_credit();

	//This loop runs until the end of the game.
	while (!gameOver) {

		// Clear the middle part.
		clear_welcome_screen();

		draw_cannons();

		// Initialize level's variables.
		initialize_level();

		// Draw fresh new bunkers.
		draw_defense();

		// Draw aliens array.
		draw_aliens();

		//This loop runs until the end of the level.
		while (!levelFinished) {

			// BEGIN FRAME
			mr_start_frame();

			// In the upper area of the screen some "MISTERY Ships" scroll from time to time
			draw_mistery();

			check_player_fire();

			check_alien_fire();

			draw_cannon();

			control_cannon();

			move_aliens();

			move_mistery();

			move_fire();

			check_end_conditions();

			mr_end_frame(2);
		}

		mr_sound_stop_channel(0);
		mr_sound_stop_channel(1);

		if (!gameOver) {
			player_screen();
		}
	}

	// Update the hi score.
	if (*((long*)&score[0][0]) > * ((long*)&hiscore[0])) {
		hiscore[0] = score[0][0];
		hiscore[1] = score[0][1];
		hiscore[2] = score[0][2];
		hiscore[3] = score[0][3];
	}
	
	if (*((long*)&score[1][0]) > * ((long*)&hiscore[0])) {
		hiscore[0] = score[1][0];
		hiscore[1] = score[1][1];
		hiscore[2] = score[1][2];
		hiscore[3] = score[1][3];
	}

}

#include <stdio.h>

void game_alien_storm() {

	// Initialize random number generator
	srand(clock());

	// Let's initialize the graphics library.
	mr_init();

	// We prepare the graphics.
	prepare_graphics();

	// Endless loop.
	while (1) {

		welcome_screen();

		gameloop();

	}
		
}

#endif
