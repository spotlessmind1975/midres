#ifdef __DEMO_BITBLIT__

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

// Width and height of the sprite, in mixels.
#define		SPRITE_WIDTH		10
#define		SPRITE_HEIGHT		10

// Speed of the sprite, in mixels/frame
#define		SPRITE_SPEED		2

// Or with "mr_blitop_color" if you want to have a colored sprite:
// in this case, however, you will have color clashes.
#define		SPRITE_COLOR		mr_blitop_set

// Position of the sprite on the screen.
int x = 0, y = 0;

// Speed along abscissa and ordinate, in mixels/frame
int dx = SPRITE_SPEED, dy = SPRITE_SPEED;

// This flag is used to know if the background has been saved.
unsigned char p = 0;

// Position of last saved background.
int px = 0, py = 0;

/****************************************************************************
 ** RESIDENT FUNCTIONS SECTION
 ****************************************************************************/

// All the functions defined within the resident body of the code are 
// accessible from all modules, both resident and changing ones.

// Generate a color sprite and save it on the auxiliary screen 
// at (0,0)-(SPRITE_WIDTH, SPRITE_HEIGHT)
void create_sprite() {
	mr_recte(0, 0, SPRITE_WIDTH-1, SPRITE_HEIGHT-1, MR_COLOR_YELLOW);
	mr_linee(0, 0, SPRITE_WIDTH - 1, SPRITE_HEIGHT - 1, MR_COLOR_RED);
	mr_linee(SPRITE_WIDTH - 1, 0, 0, SPRITE_HEIGHT - 1, MR_COLOR_RED);
	mr_blit_from_screene(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT, MR_AUX_DEFAULT, 0, 0, SPRITE_COLOR);
	mr_clear_bitmape();
}

// Draw the color sprite on the screen.
void draw_sprite() {

	// If needed, restore the screen. The saved background is
	// at position (SPRITE_WIDTH+1,0).
	if (p > 0) {
		mr_blit_to_screene(MR_AUX_DEFAULT,
				SPRITE_WIDTH+1, 0, SPRITE_WIDTH, SPRITE_HEIGHT, 
				px, py, mr_blitop_set | mr_blitop_color);
	}

	// Save the background.
	mr_blit_from_screene( x, y, SPRITE_WIDTH, SPRITE_HEIGHT, 
		MR_AUX_DEFAULT, SPRITE_WIDTH+1, 0,
				mr_blitop_set | mr_blitop_color);
	
	// Draw the sprite.
	mr_blit_to_screene(MR_AUX_DEFAULT, 0, 0, SPRITE_WIDTH, SPRITE_HEIGHT,
			x, y, 
			SPRITE_COLOR);

	// Save the fact we draw the sprite at position given.
	p = 1;
	px = x;
	py = y; 

}

/****************************************************************************
 ** RESIDENT MAIN FUNCTION
 ****************************************************************************/

// This is the main function body. The purpose is to call the various 
// functions present in the modules, taking care to load the relevant 
// code / data into memory(in the "overlay" area).

void demo_bitblit() {

	// Initialize the graphical subsystem.
	mr_init();

	// Enable the double buffering.
	mr_doublebuffer_init();

	// Clear all involved screens.
	mr_clear_bitmap(MR_AUX_DEFAULT);
	mr_clear_bitmapv();
	mr_clear_bitmape();

	// Update the frame.
	mr_doublebuffer_switch();

	// Create a sprite to move.
	create_sprite();

	// Endless loop.
	while (1) {
		
		// Draw the sprite on the screen
		draw_sprite();
		
		// Update the screen
		mr_doublebuffer_switch();

		// Move the sprite along the direction.
		x = x + dx;
		y = y + dy;

		// Change direction if we are going to hit the borders.
		if ((x + SPRITE_WIDTH+1) >= WIDTH * 2) {
			dx = -dx;
		}
		if ((y + SPRITE_HEIGHT+1) >= (MR_SCREEN_HEIGHT) * 2) {
			dy = -dy;
		}
		if (x == 0) {
			dx = -dx;
		}
		if (y == 0) {
			dy = -dy;
		}


	}

}

#endif
