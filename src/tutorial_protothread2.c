#define __TUTORIAL__PROTOTHREAD2__ 1
#ifdef __TUTORIAL__PROTOTHREAD2__

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
#include <stdio.h>

// Needed for MIDRES library call.
#include "midres.h"

#include "game_elevator_tiles.h"

// Number of frames for passenger's animation.
#define	WALK_FRAME_COUNT				6

#define FILENAME_TILES_BIN				"zeltiles.bin"

#if(MR_SCREEN_HEIGHT>23)
#define	FLOOR_COUNT						5
#else
#define	FLOOR_COUNT						4
#endif

// Height of a floor, in tiles. We reserve 2 tiles for status lines (up&down).
#define	FLOOR_HEIGHT					( ( MR_SCREEN_HEIGHT - 2 ) / 5 )

// This function will draw a single frame for a passenger that is walking at a given position.
void draw_passenger_walking(unsigned char _floor, mr_position _pos, unsigned char _frame, mr_color _color) {

	// We use a reverse notation for floors: the _floor 0 is, really, the last
	// so we need to invert the meaning of the floor in order to draw them
	// correctly.
	unsigned char realFloor = (FLOOR_COUNT - _floor);

	// We have to choose the correct tile, in base to the current _frame.
	unsigned char tile = TILE_WALK1;
	switch (_frame) {
	case 0:
		tile = TILE_WALK1;
		break;
	case 1:
		tile = TILE_WALK2;
		break;
	case 2:
		tile = TILE_WALK3;
		break;
	case 3:
		tile = TILE_WALK4;
		break;
	case 4:
		tile = TILE_WALK5;
		break;
	case 5:
		tile = TILE_WALK6;
		break;
	}

	// If we are drawing a passenger on a position that is not the
	// leftmost column, we must clear the previous position,
	// to avoid the ghosting.
	if (_pos > 0) {
		// Clear the leftmost column.
		mr_vtilesv(_pos - 1, realFloor * FLOOR_HEIGHT - TILE_WALK1_HEIGHT, realFloor * FLOOR_HEIGHT - 1, TILE_EMPTY, _color);

		// Restore the floor.
		mr_puttilev(_pos - 1, realFloor * FLOOR_HEIGHT, TILE_FLOOR, _color);
	}

	// Finally, we can draw the animation frame.
	mr_putetilesv(_pos, realFloor * FLOOR_HEIGHT - 2, tile, TILE_WALK1_WIDTH, TILE_WALK1_HEIGHT, _color);
}

MR_PT_CTX(walk, unsigned char floor; unsigned char frame; unsigned char pos; unsigned char to; mr_color color;);

walk_protothread walkThread[FLOOR_COUNT];

// This thread takes care of moving a passenger along the plane, from the 
// position where it is located to the one indicated as the input parameter.
// See "WALK" thread diagram.
MR_PT_THREAD_EXT(walk, walk_protothread) {

	MR_PTI_BEGIN();

	// ARRIVED?
	while (MR_PTI_CTX()->pos < MR_PTI_CTX()->to) {

		// INCREASE PASSENGER FRAME
		++MR_PTI_CTX()->frame;

		// EVEN FRAME?
		if ((MR_PTI_CTX()->frame & 1) == 0) {
			// INCREASE PASSENGER POSITION
			++MR_PTI_CTX()->pos;
		}

		// FRAME MAX REACHED?
		if (MR_PTI_CTX()->frame > WALK_FRAME_COUNT) {
			// RESET FRAME
			MR_PTI_CTX()->frame = 0;
		}

		// (yeld)
		MR_PTI_YIELD();

		// DRAW PASSENGER
		draw_passenger_walking(MR_PTI_CTX()->floor, MR_PTI_CTX()->pos, MR_PTI_CTX()->frame, MR_PTI_CTX()->color);
	}

	MR_PTI_END();

}

void tutorial_protothread2() {

	unsigned char i;
	
	mr_init();

	// Load tiles from mass memory.
	mr_tileset_load(FILENAME_TILES_BIN, MR_TILESET_0, TILE_START, TILE_COUNT);

	// This little "trick" allows us to quickly clean the screen 
	// using the prepared empty tile.
	MR_RENDERED_MIXELS[0] = TILE_EMPTY;

	mr_set_background_color(MR_COLOR_LIGHT_GREY);

	mr_clear_bitmapv();

	// Enable the custom charset.
	mr_tileset_visible(MR_TILESET_0);

	for (i = 0; i < FLOOR_COUNT; ++i) {
		walkThread[i].floor = i;
		walkThread[i].pos = rand() & 0xf;
		walkThread[i].frame = 0;
		walkThread[i].to = MR_SCREEN_WIDTH - 2;
		switch (i) {
			case 0:
				walkThread[i].color = MR_COLOR_RED;
				break;
			case 1:
				walkThread[i].color = MR_COLOR_GREEN;
				break;
			case 2:
				walkThread[i].color = MR_COLOR_BLUE;
				break;
			case 3:
				walkThread[i].color = MR_COLOR_BLACK;
				break;
			case 4:
				walkThread[i].color = MR_COLOR_PURPLE;
				break;
		}
	}

	while (1) {
		mr_start_frame();
		for (i = 0; i < 5; ++i) {
			walk(&walkThread[i]);
		}
		mr_end_frame(4);
	}

}

#endif
