#ifdef __DEMO_TEST_CARD__

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
#include "demo_test_card_tiles.h"

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

/****************************************************************************
 ** RESIDENT MAIN FUNCTION
 ****************************************************************************/

// This is the main function body. The purpose is to call the various 
// functions present in the modules, taking care to load the relevant 
// code / data into memory(in the "overlay" area).

void demo_test_card() {

    mr_init_multicolor();

	mr_set_border_color(MR_COLOR_GREY);
	mr_set_background_color(MR_COLOR_YELLOW);

    mr_clearv();

	// Load tiles from mass memory.
	mr_tileset_load("testcard.bin", MR_TILESET_0, TILE_START, TILE_COUNT);
    mr_tileset_visible(MR_TILESET_0);
    mr_tile_setcolor(MR_TILE_BACKGROUND, TILE_COLOR0);
    mr_tile_setcolor(MR_TILE_COLOR0, TILE_COLOR1);
    mr_tile_setcolor(MR_TILE_COLOR1, TILE_COLOR2);
    mr_tile_setcolor(MR_TILE_COLOR2, TILE_COLOR3);

    mr_putetilesv((MR_SCREEN_WIDTH-TILE_TESTCARD_WIDTH)>>1, MR_SCREEN_HEIGHT>>1, TILE_TESTCARD, TILE_TESTCARD_WIDTH, TILE_TESTCARD_HEIGHT, TILE_COLOR3);

    while(1) { }

}

#endif
