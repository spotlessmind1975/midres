#ifdef __GAME_ALIEN_STORM__

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
#include <stdlib.h>
#include <time.h>
#include "midres.h"
#include "game_alien_storm_tiles.h"
#include "game_alien_storm_tiles1.h"
#include "game_alien_storm_tiles2.h"

unsigned char* mr_translate_file_user(mr_file _file) {

}

/*
La modalità di gioco è piuttosto semplice: il giocatore controlla un cannone mobile che si muove
orizzontalmente sul fondo dello schermo, e deve abbattere uno ad uno gli alieni che piano piano
si avvicinano alla Terra. Le tappe di avvicinamento degli alieni al Mondo seguono uno schema univoco,
un ampio e ordinato zig-zag che li porta lentamente ma inesorabilmente a raggiungere il fondo dello
schermo decretando l'avvenuta invasione e la conseguente fine della partita.

Il giocatore difende la Terra affrontando con il suo cannone orde di alieni che si susseguono
incessantemente (sterminata un'ondata, subito un'altra scende, sempre più veloce e agguerrita).
Nella zona alta dello schermo scorrono, di tanto in tanto, alcune "Navi del Mistero", ovvero
navicelle bonus che, se abbattute, permettono al giocatore di incrementare il suo punteggio
molto più velocemente. Il gioco si conclude quando gli alieni raggiungono il fondo dello
schermo o quando il cannone viene distrutto.

Il cannone può essere distrutto dal fuoco nemico, da bombe o da raggi mortali che periodicamente
vengono lanciati dagli alieni verso il cannone. Il cannone è parzialmente protetto da alcuni bunker
difensivi immobili (il cui numero varia a seconda della versione del gioco) che vengono gradualmente
distrutti dai proiettili degli alieni o dai colpi sparati dallo stesso giocatore.

Il giocatore dispone di tre cannoni mobili (tre vite o più a seconda delle impostazioni), dopo
la distruzione dei quali il gioco termina.

L'utente dispone di un numero illimitato di proiettili ma può sparare solo un colpo per volta.
Man mano che gli alieni vengono distrutti, quelli rimanenti si muovono più velocemente sullo schermo.
La traccia sonora - composta solo da una serie di toni - segue il ritmo sempre in crescendo
della partita. Versioni successive del gioco furono implementate con schemi e scenari
differenti, ma rimasero intatte le regole principali del gioco.
*/

#define BUNKER_COUNT				8
#define BUNKER_OFFSET				1

#define ALIEN_ROW_COUNT				2 /* 5 */
#define ALIEN_COLUMN_COUNT			2 /*10*/

#define ALIEN_HORIZONTAL_OFFSET		( ( MR_SCREEN_WIDTH - (( TILE_ALIEN1A_WIDTH + (TILE_ALIEN1A_WIDTH>>1)) * ALIEN_COLUMN_COUNT) ) >> 1 )
#define ALIEN_VERTICAL_OFFSET		4
#define ALIEN_DELAY_FRAME_COUNT		20

#define MISTERY_VERTICAL_OFFSET		3

#define ALIEN_FIRE_DELAY_FRAME_COUNT		2

#define FIRE_DELAY_FRAME_COUNT		2

#define MISTERY_DELAY_FRAME_COUNT   8

#define	TILE_MOVING_CANNON			TILE_START + TILE_COUNT

mr_tile_position cannonX = 0;
unsigned char cannonLives[2] = { 3, 3 };

unsigned char alienCounts = 0;
unsigned char alienFrame = 0;
unsigned char alienMatrix[ALIEN_ROW_COUNT][ALIEN_COLUMN_COUNT];
unsigned char alienColumn[MR_SCREEN_WIDTH];
unsigned char alienRowBitmap = 0x3f;
unsigned char alienMaxRow = ALIEN_ROW_COUNT;
unsigned char alienMaxRowPerColumn[ALIEN_COLUMN_COUNT];
unsigned char alienFireY[ALIEN_COLUMN_COUNT];
unsigned char alienFireX[ALIEN_COLUMN_COUNT];

signed char alienX = 0;
signed char alienPX = 0;
signed char alienDX = 1;

mr_position alienY = 1;
mr_position alienPY = 1;
mr_boolean gameOver = mr_false;
mr_boolean levelFinished = mr_false;
unsigned char alienDelayFrameCount = ALIEN_DELAY_FRAME_COUNT;
mr_position alienFireDelayFrameCount = ALIEN_FIRE_DELAY_FRAME_COUNT;

mr_position misteryX = 0;
mr_position misteryXP = 0;
mr_position misteryDelayFrameCount = MISTERY_DELAY_FRAME_COUNT;

mr_position fireY = 0;
mr_position fireX = 0;
mr_position fireDelayFrameCount = FIRE_DELAY_FRAME_COUNT;

mr_position score[2][4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

// Store the hiscore of the player
mr_position hiscore[4] = { 0, 0, 0, 0 };

mr_position credit[2] = { 0, 0 };

mr_boolean cannonHit = mr_false;

unsigned char players = 1;
unsigned char currentPlayer = 0;

void prepare_graphics() {
	mr_tileset_load("zstiles.bin", MR_TILESET_0, TILE_START, TILE_COUNT);
	mr_tileset_load("zstiles1.bin", MR_TILESET_1, TILE1_START, TILE1_COUNT);
	mr_tileset_load("zstiles2.bin", MR_TILESET_2, TILE2_START, TILE2_COUNT);
	mr_tile_prepare_horizontal_extended(MR_TILESET_0, TILE_CANNON, TILE_CANNON_WIDTH, TILE_CANNON_HEIGHT, TILE_MOVING_CANNON);
	RENDERED_MIXELS[0] = TILE_EMPTY;
}

void initialize_level() {
	unsigned char i, j;
	cannonX = (MR_SCREEN_WIDTH >> 1) * 8;
	alienCounts = 0;
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
		alienColumn[j] = j * (TILE_ALIEN1A_WIDTH + (TILE_ALIEN1A_WIDTH >> 1));
		alienFireY[j] = 0;
		alienFireX[j] = 0;
		alienMaxRowPerColumn[j] = ALIEN_ROW_COUNT;
	}
	alienX = 0;
	alienPX = 0;
	alienDX = 1;
	alienY = 1;
	alienPY = 1;
	--alienDelayFrameCount;
	if (alienDelayFrameCount == 0) {
		alienDelayFrameCount = 1;
	}
	fireY = 0;
	fireX = 0;
	alienFireDelayFrameCount = ALIEN_FIRE_DELAY_FRAME_COUNT;
	fireDelayFrameCount = FIRE_DELAY_FRAME_COUNT;
	alienRowBitmap = 0x3f;
	levelFinished = mr_false;
	gameOver = mr_false;
	alienMaxRow = 0;
}

void draw_score_label(mr_position _x, mr_position _y, mr_position _player) {
	mr_putetilesv(_x, _y, _player == 0 ? TILE_SCORE1 : TILE_SCORE2, TILE_SCORE1_WIDTH, TILE_SCORE1_HEIGHT, MR_COLOR_WHITE);
}

void draw_hiscore_label(mr_position _x, mr_position _y) {
	mr_putetilesv(_x, _y, TILE_HISCORE, TILE_HISCORE_WIDTH, TILE_HISCORE_HEIGHT, MR_COLOR_WHITE);
}

// Draw the score at specific position.
void draw_score(mr_position _x, mr_position _y, mr_position _player) {
	mr_position i;
	for (i = 0; i < 4; ++i) {
		mr_puttilev(_x+i, _y, TILE_DIGIT0 + score[_player][3-i], MR_COLOR_WHITE);
	}
}

// Draw the score at specific position.
void draw_hiscore(mr_position _x, mr_position _y) {
	mr_position i;
	for (i = 0; i < 4; ++i) {
		mr_puttilev(_x+i, _y, TILE_DIGIT0 + hiscore[3-i], MR_COLOR_WHITE);
	}
}

// Add to the given multibyte (max size = 4 bytes) of a value.
void add(unsigned char* _points, unsigned char * _value) {
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

unsigned char addendum[4] = { 0, 0, 0, 0 };

void clear_alien(i, j) {
	mr_putftilesv(alienX + ALIEN_HORIZONTAL_OFFSET + alienColumn[j], alienY + ALIEN_VERTICAL_OFFSET + i * (TILE_ALIEN1A_HEIGHT + TILE_ALIEN1A_HEIGHT), TILE_EMPTY, TILE_EXPLOSION_WIDTH, TILE_EXPLOSION_HEIGHT, MR_COLOR_WHITE);
}

void explode_alien(i, j) {
	mr_putetilesv(alienX + ALIEN_HORIZONTAL_OFFSET + alienColumn[j], alienY + ALIEN_VERTICAL_OFFSET + i * (TILE_ALIEN1A_HEIGHT + TILE_ALIEN1A_HEIGHT), TILE_EXPLOSION, TILE_EXPLOSION_WIDTH, TILE_EXPLOSION_HEIGHT, MR_COLOR_WHITE);
}

void draw_alien(i, j) {
	mr_putetilesv(alienX + ALIEN_HORIZONTAL_OFFSET + alienColumn[j], alienY + ALIEN_VERTICAL_OFFSET + i * (TILE_ALIEN1A_HEIGHT + TILE_ALIEN1A_HEIGHT), alienMatrix[i][j] + (alienFrame * TILE_ALIEN1A_WIDTH * TILE_ALIEN1A_HEIGHT), TILE_ALIEN1A_WIDTH, TILE_ALIEN1A_HEIGHT, MR_COLOR_WHITE);
}

void draw_aliens() {

	unsigned char i, j;
	mr_position alienRowBitmapMask = 0;
	mr_boolean alienPresent = mr_false;

	for (i = 0; i < ALIEN_COLUMN_COUNT; ++i) {
		alienMaxRowPerColumn[i] = 0;
	}

	alienMaxRow = 0;
	for (i = 0; i < ALIEN_ROW_COUNT; ++i) {
		alienRowBitmapMask = (1 << i);
		if (alienRowBitmap & alienRowBitmapMask) {
			if ((alienPX != alienX) || (alienPY != alienY)) {
				mr_htilesv(alienPX + ALIEN_HORIZONTAL_OFFSET, alienPX + ALIEN_HORIZONTAL_OFFSET + (ALIEN_COLUMN_COUNT) * (TILE_ALIEN1A_WIDTH + (TILE_ALIEN1A_WIDTH >> 1)), alienPY + ALIEN_VERTICAL_OFFSET + i * (TILE_ALIEN1A_HEIGHT + TILE_ALIEN1A_HEIGHT), TILE_EMPTY, MR_COLOR_BLACK);
			}
			alienMaxRow = i;
			for (j = 0; j < ALIEN_COLUMN_COUNT; ++j) {
				if (alienMatrix[i][j]) {
					if (alienMatrix[i][j] == 0xff) {
						clear_alien(i, j);
						alienMatrix[i][j] = 0;
					} else {
						alienPresent = mr_true;
						draw_alien(i, j);
						alienMaxRowPerColumn[j] = i;
					}
				}
			}
			if (!alienPresent) {
				alienRowBitmap = alienRowBitmap & ~(1 << i);
			}
		}
	}
	alienPX = alienX;
	alienPY = alienY;

}

void draw_defense() {
	unsigned char i;
	for (i = 0; i < BUNKER_COUNT; ++i) {
		mr_putetilesv(i * (MR_SCREEN_WIDTH >> 3) + BUNKER_OFFSET, MR_SCREEN_HEIGHT - TILE_CANNON_HEIGHT - 2 - TILE_DEFENSE_HEIGHT, TILE_DEFENSE, TILE_DEFENSE_WIDTH, TILE_DEFENSE_HEIGHT, MR_COLOR_GREEN);
	}
}

void draw_mistery() {
	if (misteryX == 0) {
		if (misteryXP != misteryX) {
			mr_putftilesv(misteryXP, MISTERY_VERTICAL_OFFSET, TILE_EMPTY, TILE_MYSTERO_WIDTH, TILE_MYSTERO_HEIGHT, MR_COLOR_BLACK);
		}
	}
	else if (misteryX > 0) {
		mr_putetilesv(misteryX - 1, MISTERY_VERTICAL_OFFSET, TILE_EMPTY, TILE_EMPTY_WIDTH, TILE_EMPTY_HEIGHT, MR_COLOR_BLACK);
		mr_putetilesv(misteryX, MISTERY_VERTICAL_OFFSET, TILE_MYSTERO, TILE_MYSTERO_WIDTH, TILE_MYSTERO_HEIGHT, MR_COLOR_RED);
	}
	misteryXP = misteryX;
}

void draw_cannons() {
	unsigned char i;
	mr_putftilesv(1, MR_SCREEN_HEIGHT - 1, TILE_EMPTY, 7, 1, MR_COLOR_BLACK);
	mr_puttilev(1, MR_SCREEN_HEIGHT-1, TILE_DIGIT0 + cannonLives[currentPlayer], MR_COLOR_WHITE);
	for (i = 0; i < 3; ++i) {
		mr_putetilesv(3+((i+1)*2), MR_SCREEN_HEIGHT-1, TILE_CANNON, TILE_CANNON_WIDTH, TILE_CANNON_HEIGHT, MR_COLOR_GREEN);
	}
}

void draw_credit() {
	unsigned char i;
	mr_putetilesv(MR_SCREEN_WIDTH - 6, MR_SCREEN_HEIGHT - 1, TILE_CREDIT, TILE_CREDIT_WIDTH, TILE_CREDIT_HEIGHT, MR_COLOR_WHITE);
	for (i = 0; i < 2; ++i) {
		mr_puttilev(MR_SCREEN_WIDTH - 2 + i, MR_SCREEN_HEIGHT - 1, TILE_DIGIT0 + credit[1 - i], MR_COLOR_WHITE);
	}
}

mr_boolean keyboard_controller(unsigned char _seconds) {

	unsigned char key = mr_wait_or_get_keypressed(_seconds);

	switch (key) {
		default:
		case MR_KEY_NONE:
			return mr_false;
		case MR_KEY_C:
			++(*(unsigned int*)(&credit[0]));
			draw_credit();
			break;
		case MR_KEY_1:
			if ((*(unsigned int*)(&credit[0])) > 0) {
				--(*(unsigned int*)(&credit[0]));
				players = 1;
				return mr_true;
			}
			break;
		case MR_KEY_2:
			if ((*(unsigned int*)(&credit[0])) > 1) {
				(*(unsigned int*)(&credit[0])) -= 2;
				players = 2;
				return mr_true;
			}
			break;
	}

	return mr_false;

}

void welcome_screen() {

	mr_clearv();

	draw_score_label(0, 0, 0);
	draw_score(0, 1, 0);

	draw_hiscore_label((MR_SCREEN_WIDTH - TILE_HISCORE_WIDTH) >> 1, 0);
	draw_hiscore((MR_SCREEN_WIDTH - TILE_HISCORE_WIDTH) >> 1, 1);

	draw_score_label(MR_SCREEN_WIDTH - TILE_SCORE2_WIDTH, 0, 1);
	draw_score(MR_SCREEN_WIDTH - TILE_SCORE2_WIDTH, 1, 1);

	draw_credit();

	while (1) {

		mr_putftilesv(0, 3, TILE2_EMPTY, MR_SCREEN_WIDTH - 1, MR_SCREEN_HEIGHT - 4, MR_COLOR_BLACK);

		mr_tileset_visible(MR_TILESET_2);

		mr_putetilesv((MR_SCREEN_WIDTH - TILE2_INSERTCOIN_WIDTH) >> 1, (MR_SCREEN_HEIGHT>>1)-1, TILE2_INSERTCOIN, TILE2_INSERTCOIN_WIDTH, TILE2_INSERTCOIN_HEIGHT, MR_COLOR_WHITE);

		mr_putetilesv((MR_SCREEN_WIDTH - TILE2_12PLAYERS_WIDTH) >> 1, (MR_SCREEN_HEIGHT >> 1) + 1, TILE2_12PLAYERS, TILE2_12PLAYERS_WIDTH, TILE2_12PLAYERS_HEIGHT, MR_COLOR_WHITE);

		if (keyboard_controller(1)) return;

		mr_puttilev(((MR_SCREEN_WIDTH - TILE2_12PLAYERS_WIDTH) >> 1) - 1, (MR_SCREEN_HEIGHT >> 1) + 3, TILE2_DIGIT1, MR_COLOR_WHITE);
		mr_putetilesv(((MR_SCREEN_WIDTH - TILE2_12PLAYERS_WIDTH) >> 1), (MR_SCREEN_HEIGHT >> 1) + 3, TILE2_PLAYER, TILE2_PLAYER_WIDTH, TILE2_PLAYER_HEIGHT, MR_COLOR_WHITE);
		mr_puttilev(((MR_SCREEN_WIDTH - TILE2_12PLAYERS_WIDTH) >> 1) + TILE2_PLAYERS_WIDTH, (MR_SCREEN_HEIGHT >> 1) + 3, TILE2_DIGIT1, MR_COLOR_WHITE);
		mr_putetilesv(((MR_SCREEN_WIDTH - TILE2_12PLAYERS_WIDTH) >> 1) + TILE2_PLAYERS_WIDTH + 1, (MR_SCREEN_HEIGHT >> 1) + 3, TILE2_COIN, TILE2_COIN_WIDTH, TILE2_COIN_HEIGHT, MR_COLOR_WHITE);

		if (keyboard_controller(1)) return;

		mr_puttilev(((MR_SCREEN_WIDTH - TILE2_12PLAYERS_WIDTH) >> 1)-1, (MR_SCREEN_HEIGHT >> 1) + 5, TILE2_DIGIT2, MR_COLOR_GREEN);
		mr_putetilesv(((MR_SCREEN_WIDTH - TILE2_12PLAYERS_WIDTH) >> 1) , (MR_SCREEN_HEIGHT >> 1) + 5, TILE2_PLAYERS, TILE2_PLAYERS_WIDTH, TILE2_PLAYER_HEIGHT, MR_COLOR_GREEN);
		mr_puttilev(((MR_SCREEN_WIDTH - TILE2_12PLAYERS_WIDTH) >> 1) + TILE2_PLAYERS_WIDTH, (MR_SCREEN_HEIGHT >> 1) + 5, TILE2_DIGIT2, MR_COLOR_GREEN);
		mr_putetilesv(((MR_SCREEN_WIDTH - TILE2_12PLAYERS_WIDTH) >> 1) + TILE2_PLAYERS_WIDTH + 1, (MR_SCREEN_HEIGHT >> 1) + 5, TILE2_COINS, TILE2_COINS_WIDTH, TILE2_COINS_HEIGHT, MR_COLOR_GREEN);

		if (keyboard_controller(3)) return;

		mr_putftilesv(0, 3, TILE2_EMPTY, MR_SCREEN_WIDTH - 1, MR_SCREEN_HEIGHT - 4, MR_COLOR_BLACK);

		mr_tileset_visible(MR_TILESET_1);

		mr_putetilesv((MR_SCREEN_WIDTH - TILE1_PLAY_WIDTH) >> 1, 4, TILE1_PLAY, TILE1_PLAY_WIDTH, TILE1_PLAY_HEIGHT, MR_COLOR_WHITE);

		if (keyboard_controller(1)) return;

		mr_putetilesv((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1, 6, TILE1_TITLE, TILE1_TITLE_WIDTH, TILE1_TITLE_HEIGHT, MR_COLOR_WHITE);

		if (keyboard_controller(1)) return;

		mr_putetilesv((MR_SCREEN_WIDTH - TILE1_SCORETABLE_WIDTH) >> 1, 9, TILE1_SCORETABLE, TILE1_SCORETABLE_WIDTH, TILE1_SCORETABLE_HEIGHT, MR_COLOR_WHITE);

		if (keyboard_controller(1)) return;

		mr_putetilesv((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1, 11, TILE1_MYSTERO, TILE1_MYSTERO_WIDTH, TILE1_MYSTERO_HEIGHT, MR_COLOR_WHITE);

		if (keyboard_controller(1)) return;

		mr_putetilesv(((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1 ) + TILE1_MYSTERO_WIDTH + 2, 11, TILE1_MISTERY, TILE1_MISTERY_WIDTH, TILE1_MISTERY_HEIGHT, MR_COLOR_WHITE);

		if (keyboard_controller(1)) return;

		mr_putetilesv((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1, 13, TILE1_ALIEN1A, TILE1_ALIEN1A_WIDTH, TILE1_ALIEN1A_HEIGHT, MR_COLOR_WHITE);

		if (keyboard_controller(1)) return;

		mr_puttilev(((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1 ) + TILE1_MYSTERO_WIDTH + 2, 13, TILE1_DIGIT3, MR_COLOR_WHITE);
		mr_puttilev(((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1 ) + TILE1_MYSTERO_WIDTH + 2 + 1, 13, TILE1_DIGIT0, MR_COLOR_WHITE);
		mr_putetilesv(((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1 ) + TILE1_MYSTERO_WIDTH + 2 + 2, 13, TILE1_POINTS, TILE1_POINTS_WIDTH, TILE1_POINTS_HEIGHT, MR_COLOR_WHITE);

		if (keyboard_controller(1)) return;

		mr_putetilesv((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1, 15, TILE1_ALIEN2A, TILE1_ALIEN2A_WIDTH, TILE1_ALIEN2A_HEIGHT, MR_COLOR_WHITE);

		if (keyboard_controller(1)) return;

		mr_puttilev(((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1 ) + TILE1_MYSTERO_WIDTH + 2, 15, TILE1_DIGIT2, MR_COLOR_WHITE);
		mr_puttilev(((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1 ) + TILE1_MYSTERO_WIDTH + 2 + 1, 15, TILE1_DIGIT0, MR_COLOR_WHITE);
		mr_putetilesv(((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1 ) + TILE1_MYSTERO_WIDTH + 2 + 2, 15, TILE1_POINTS, TILE1_POINTS_WIDTH, TILE1_POINTS_HEIGHT, MR_COLOR_WHITE);

		if (keyboard_controller(1)) return;

		mr_putetilesv((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1, 17, TILE1_ALIEN1A, TILE1_ALIEN1A_WIDTH, TILE1_ALIEN1A_HEIGHT, MR_COLOR_GREEN);

		if (keyboard_controller(1)) return;

		mr_puttilev(((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1 ) + TILE1_MYSTERO_WIDTH + 2, 17, TILE1_DIGIT1, MR_COLOR_WHITE);
		mr_puttilev(((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1 ) + TILE1_MYSTERO_WIDTH + 2 + 1, 17, TILE1_DIGIT0, MR_COLOR_WHITE);
		mr_putetilesv(((MR_SCREEN_WIDTH - TILE1_TITLE_WIDTH) >> 1 ) + TILE1_MYSTERO_WIDTH + 2 + 2, 17, TILE1_POINTS, TILE1_POINTS_WIDTH, TILE1_POINTS_HEIGHT, MR_COLOR_WHITE);

		if (keyboard_controller(3)) return;

	}

}


void player_screen() {

	mr_putftilesv(0, 3, TILE2_EMPTY, MR_SCREEN_WIDTH - 1, MR_SCREEN_HEIGHT - 4, MR_COLOR_BLACK);

	mr_tileset_visible(MR_TILESET_2);

	mr_putetilesv(((MR_SCREEN_WIDTH - TILE2_12PLAYERS_WIDTH) >> 1), (MR_SCREEN_HEIGHT >> 1) + 3, TILE2_PLAYER, TILE2_PLAYER_WIDTH, TILE2_PLAYER_HEIGHT, MR_COLOR_WHITE);
	mr_puttilev(((MR_SCREEN_WIDTH - TILE2_12PLAYERS_WIDTH) >> 1) + 2, (MR_SCREEN_HEIGHT >> 1) + 3, currentPlayer?TILE2_DIGIT2:TILE2_DIGIT1, MR_COLOR_WHITE);

	mr_wait_or_keypress(2);

	mr_putftilesv(0, 3, TILE2_EMPTY, MR_SCREEN_WIDTH - 1, MR_SCREEN_HEIGHT - 4, MR_COLOR_BLACK);

	mr_tileset_visible(MR_TILESET_0);

}

void draw_cannon() {
	if (cannonHit) {
		mr_putetilesv((cannonX >> 3), (MR_SCREEN_HEIGHT - TILE_CANNON_HEIGHT - 1) * 8, TILE_EXPLOSION, TILE_EXPLOSION_WIDTH, TILE_EXPLOSION_HEIGHT, MR_COLOR_WHITE);
		cannonHit = mr_false;
	} else {
		mr_tile_moveto_horizontal_extendedv(cannonX, (MR_SCREEN_HEIGHT - TILE_CANNON_HEIGHT - 1) * 8, TILE_MOVING_CANNON, TILE_CANNON_WIDTH, TILE_CANNON_HEIGHT, MR_COLOR_GREEN);
	}
	// Draw the mobile cannon at the actual coordinates.
}

void clear_cannon() {
	mr_putftilesv(0, (MR_SCREEN_HEIGHT - TILE_CANNON_HEIGHT - 1), TILE_EMPTY, MR_SCREEN_WIDTH, TILE_CANNON_HEIGHT, MR_COLOR_BLACK);
	mr_putetilesv((cannonX>>3), (MR_SCREEN_HEIGHT - TILE_CANNON_HEIGHT - 1), TILE_EXPLOSION, TILE_EXPLOSION_WIDTH, TILE_EXPLOSION_HEIGHT, MR_COLOR_WHITE);
}

void gameloop() {

	unsigned char i, j;

	mr_tileset_visible(MR_TILESET_0);

	alienDelayFrameCount = ALIEN_DELAY_FRAME_COUNT;

	cannonLives[0] = 3;
	cannonLives[1] = 3;

	gameOver = mr_false;
	levelFinished = mr_false;
	currentPlayer = 0;

	if (players > 1) {
		player_screen();
	}

	// The player defends the Earth by facing with his cannon hordes of aliens 
	// that follow one another incessantly(one wave exterminated, immediately 
	// another descends, faster and more aggressive).
	while (!gameOver) {

		mr_clearv();

		initialize_level();

		draw_score_label(0,0,0);
		draw_score(0, 1, 0);

		draw_hiscore_label((MR_SCREEN_WIDTH-TILE_HISCORE_WIDTH)>>1, 0);
		draw_hiscore((MR_SCREEN_WIDTH - TILE_HISCORE_WIDTH) >> 1, 1);

		draw_score_label(MR_SCREEN_WIDTH-TILE_SCORE2_WIDTH, 0, 1);
		draw_score(MR_SCREEN_WIDTH - TILE_SCORE2_WIDTH, 1, 1);

		draw_credit();

		// The mobile cannon is partially defended by some immovable bunkers.
		draw_defense();

		draw_aliens();
		
		draw_cannons();

		while (!levelFinished) {

			mr_start_frame();

			// In the upper area of the screen some "MISTERY Ships" scroll from time to time
			draw_mistery();

			if (fireY == 2) {
				mr_puttilev(fireX, fireY+1, TILE_EMPTY, MR_COLOR_BLACK);
			} else if (fireY > 0) {
				mr_tile tile = mr_gettilev(fireX, fireY);
				mr_puttilev(fireX, fireY + 1, TILE_EMPTY, MR_COLOR_BLACK);
				// Bonus ships that, if taken down, allow the player to increase his score much faster
				if ((tile >= TILE_MYSTERO) && (tile < ( TILE_MYSTERO + (TILE_MYSTERO_WIDTH * TILE_MYSTERO_HEIGHT)))) {
					addendum[3] = 0;
					addendum[2] = 1;
					addendum[1] = 0;
					addendum[0] = 0;
					add(score[0], addendum);
					draw_score(0, 1, 0);
					mr_puttilev(fireX, fireY, TILE_EMPTY, MR_COLOR_BLACK);
					fireY = 0;
					misteryX = 0;
				}
				else if ((tile >= TILE_DEFENSE) && (tile < (TILE_DEFENSE + (TILE_DEFENSE_WIDTH * TILE_DEFENSE_HEIGHT)))) {
					mr_puttilev(fireX, fireY, TILE_EMPTY, MR_COLOR_BLACK);
					fireY = 0;
				}
				else if ((tile >= TILE_ALIEN1A) && (tile < ( TILE_ALIEN3B + (TILE_ALIEN3B_WIDTH * TILE_ALIEN3B_HEIGHT)))) {
					for (j = 0; j < ALIEN_COLUMN_COUNT; ++j) {
						if ((fireX - alienX - ALIEN_HORIZONTAL_OFFSET ) <= ( alienColumn[j] + TILE_ALIEN1A_WIDTH) ) {
							break;
						}
					}
					i = ( fireY - alienY - ALIEN_VERTICAL_OFFSET ) >> 1;
					alienMatrix[i][j] = 0xff;
					addendum[3] = 0;
					addendum[2] = 0;
					if (tile >= TILE_ALIEN3A) {
						addendum[1] = 3;
					} else if (tile >= TILE_ALIEN2A) {
						addendum[1] = 2;
					} else if (tile >= TILE_ALIEN1A) {
						addendum[1] = 1;
					} else {
						addendum[1] = 0;
					}
					addendum[0] = 0;
					add(score[0], addendum);
					draw_score(0, 1, 0);
					explode_alien(i, j);
					fireY = 0;
				}
				if (fireY > 2) {
					mr_puttilev(fireX, fireY, TILE_LASER, MR_COLOR_YELLOW);
				}
			}

			if (alienFireY > 0) {
				--alienFireDelayFrameCount;
				if (alienFireDelayFrameCount == 0) {
					for (i = 0; i < ALIEN_COLUMN_COUNT; ++i) {
						if (alienFireY[i] == 0 ) continue;
						if (alienFireY[i] <= (MR_SCREEN_HEIGHT-2)) {
							mr_tile tile = mr_gettilev(alienFireX[i], alienFireY[i]);
							mr_puttilev(alienFireX[i], alienFireY[i] - 1, TILE_EMPTY, MR_COLOR_BLACK);
							if (alienFireY[i] < (MR_SCREEN_HEIGHT - 2)) {
								mr_puttilev(alienFireX[i], alienFireY[i], TILE_LASER2, MR_COLOR_WHITE);
							}
							// Bonus ships that, if taken down, allow the player to increase his score much faster
							if ((tile >= TILE_DEFENSE) && (tile < (TILE_DEFENSE + (TILE_DEFENSE_WIDTH * TILE_DEFENSE_HEIGHT)))) {
								mr_puttilev(alienFireX[i], alienFireY[i], TILE_EMPTY, MR_COLOR_BLACK);
								alienFireY[i] = 0;
							} else if ((tile >= TILE_MOVING_CANNON) && (tile < (TILE_MOVING_CANNON + mr_calculate_prepared_tile_size_horizontal(TILE_CANNON_WIDTH, TILE_CANNON_HEIGHT)))) {
								cannonHit = mr_true;
								clear_cannon();
								--cannonLives[currentPlayer];
								cannonX = (MR_SCREEN_WIDTH >> 1)*8;
								mr_puttilev(alienFireX[i], alienFireY[i], TILE_EMPTY, MR_COLOR_BLACK);
								alienFireY[i] = 0;
							} else if (alienFireY[i] < (MR_SCREEN_HEIGHT - 2)) {
								++alienFireY[i];
							}
							else {
								alienFireY[i] = 0;
								mr_puttilev(alienFireX[i], alienFireY[i], TILE_EMPTY, MR_COLOR_BLACK);
							}
						}
					}
					alienFireDelayFrameCount = ALIEN_FIRE_DELAY_FRAME_COUNT;
				}
			}

			draw_cannon();

			// The player controls the mobile cannon by horizontally moving
			// it on the lower part of the screen. 
			switch (mr_get_key_pressed()) {
			case MR_KEY_A:
				if (cannonX > 0) {
					--cannonX;
				}
				break;
			case MR_KEY_D:
				if (cannonX < (MR_SCREEN_WIDTH * 8 - TILE_CANNON_WIDTH)) {
					++cannonX;
				}
				break;
			case MR_KEY_SPACE:
				// The player has an unlimited number of bullets 
				// but can only fire one shot at a time.
				// if (fireY == 0) {
					fireY = MR_SCREEN_HEIGHT - 4;
					mr_tile_redefine_fill(MR_TILESET_0, TILE_LASER, 1 << (0x07 - (cannonX & 0x07)) );
					fireX = (cannonX>>3) + 1;
				// }
				break;
			}

			// Aliens slowly arrives nearby the Earth!
			++alienCounts;

			if (alienCounts == alienDelayFrameCount) {
				alienFrame ^= 0x01;
				alienCounts = 0;

				for (i = 0; i < ALIEN_COLUMN_COUNT; ++i) {
					if (rand() > (RAND_MAX>>1) ) continue;
					if ((alienFireY[i] == 0) && (alienMaxRowPerColumn[i] > 0)) {
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
						if (alienX > ALIEN_HORIZONTAL_OFFSET) {
							alienDX = -1;
						}
						else {
							alienDX = 1;
						}
					}
					else if (alienX > ALIEN_HORIZONTAL_OFFSET) {
						alienDX = 0;
					}
					else if (alienX <= -ALIEN_HORIZONTAL_OFFSET) {
						alienDX = 0;
					}
				}

				draw_aliens();

			}

			if (misteryX == (MR_SCREEN_WIDTH - TILE_MYSTERO_WIDTH)) {
				misteryX = 0;
			} else if ( misteryX > 0 ) {
				--misteryDelayFrameCount;
				if (misteryDelayFrameCount == 0) {
					misteryDelayFrameCount = MISTERY_DELAY_FRAME_COUNT;
					++misteryX;
				}
			}

			if (misteryX == 0) {
				if (( rand() & 0xff ) > 250) {
					misteryX = 1;
				}
			}

			if (fireY > 0) {
				--fireDelayFrameCount;
				if (fireDelayFrameCount == 0) {
					fireDelayFrameCount = FIRE_DELAY_FRAME_COUNT;
					--fireY;
				}
			}

			if (alienRowBitmap == 0) {
				levelFinished = mr_true;
				gameOver = mr_false;
			}

			if ((alienMaxRow * 2 + alienY + ALIEN_VERTICAL_OFFSET ) > (MR_SCREEN_HEIGHT-2)) {
				levelFinished = mr_true;
				gameOver = mr_true;
			}

			if (cannonLives[currentPlayer] == 0) {
				levelFinished = mr_true;
				if (players == 1 || (players == 2 && currentPlayer == 1)) {
					gameOver = mr_true;
				}
				else {
					++currentPlayer;
					gameOver = mr_false;
				}
			}

			mr_end_frame(2);
		}

		if (!gameOver) {
			player_screen();
		}
	}

	// Update the hi score.
	if (*((long*)&score[0][0]) > * ((long*)&hiscore[0])) {
		hiscore[0] = score[0];
		hiscore[1] = score[1];
		hiscore[2] = score[2];
		hiscore[3] = score[3];
	}
	
}


void game_alien_storm() {

	// Initialize random number generator
	srand(clock());

	mr_init();

	mr_set_border_color(MR_COLOR_BLACK);

	prepare_graphics();

	while (1) {

		welcome_screen();

		gameloop();

	}
		
}

#endif
