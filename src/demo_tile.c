#ifdef __DEMO_TILE__

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

#include "main.h"
#include "midres.h"

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

int x = 0, j=0;

mr_mixel data[8] = {
	0xaa,
	0x55,
	0xaa,
	0x55,
	0x00,
	0x0f,
	0xf0,
	0xff
};

void demo_tile() {
	
	mr_tileset i;

	mr_init();

	mr_clear_bitmap(DEFAULT_SCREEN);
	mr_tileset_copy(2, 15);
	mr_tileset_load("heart.tiles", 15, 0, 1);
	mr_tile_redefine(15, 0, data);
	mr_tile_hshift(15, 0, 1);
	mr_tileset_visible(15);

	while (1) {
		for (i = 0; i < SCREEN_WIDTH*8; ++i) {
			mr_puttile_h(DEFAULT_SCREEN, i, i&0x1f, 1, MR_COLOR_GREEN);
			for (j = 0; j < 10000; ++j) { (int)j; }
		}
	}

	getchar();
	mr_tileset_visible(0);

}

#endif
