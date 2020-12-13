#ifdef __JOYCHECK__

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

// Needed for MIDRES library call.
#include "midres.h"

// Include generated tiles reference.
#include "utility_joycheck_tiles.h"

/****************************************************************************
 ** DEFINES AND VARIABLES SECTION
 ****************************************************************************/

// Left and top border of the visual display area (centered on the screen)
#define BORDER_LEFT			( ( MR_SCREEN_WIDTH - (TILE_JOYBIG_WIDTH+2*8+1) >> 1 ) )
#define ELEMENT_HEIGHT		( ( TILE_JOYBIG_HEIGHT ) + 1 )
#define BORDER_TOP			( ( MR_SCREEN_HEIGHT - ( (MR_JOYSTICK_COUNT*ELEMENT_HEIGHT) ) >> 1 ) )

// Left and top title position
#define TITLE_LEFT			( ( MR_SCREEN_WIDTH - TILE_TITLE_WIDTH ) >> 1 )
#define TITLE_TOP			0

// Current value of isomorphic joystick register
unsigned char joyRegister = 0;

// Bit index on isomorphic joystick register to be tested
unsigned char registerBitIndex = 0;

// Joystick to be tested
unsigned char joystickIndex = 0;

// Main program
void utility_joycheck() {

	// Initialize graphical subsystem
	mr_init();

	// Put the border and the background color to black.
	mr_set_border_color(MR_COLOR_BLACK);
	mr_set_background_color(MR_COLOR_BLACK);

	// Load the tiles from the disk on the first tileset space.
	// We load all tiles (TILE_COUNT) from first position (0).
	mr_tileset_load("zdjtiles.bin", MR_TILESET_0, TILE_START, TILE_COUNT);

	// This little "trick" allows us to quickly clean the screen 
	// using the prepared empty tile.
	MR_RENDERED_MIXELS[0] = TILE_EMPTY;

	// Enable custom font.
	mr_tileset_visible(MR_TILESET_0);

	// Clear the screen
	mr_clearv();

	// Let's build the main display area.
	// Word: "JOYCHECK"
	mr_putetilesv(TITLE_LEFT, TITLE_TOP, 
					TILE_TITLE, 
					TILE_TITLE_WIDTH, TILE_TITLE_HEIGHT, 
					MR_COLOR_WHITE);

	// For each joystick...
	for (joystickIndex = 0; joystickIndex < MR_JOYSTICK_COUNT; ++joystickIndex) {

		// Draw the big picture of the joystick.
		mr_putetilesv(BORDER_LEFT, BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT), 
							TILE_JOYBIG, 
							TILE_JOYBIG_WIDTH, TILE_JOYBIG_HEIGHT, 
							MR_COLOR_YELLOW);

		// Draw the first row's upper legend (stressed bit indexes).
		for (registerBitIndex = 7; registerBitIndex != 255; --registerBitIndex) {
			mr_puttilev(BORDER_LEFT + TILE_JOYBIG_WIDTH + 1 + (registerBitIndex * 2),
							BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT),
							TILE_DIGIT0 + (7 - registerBitIndex), MR_COLOR_WHITE);
		}

		// Draw the second row's upper legend (stressed directions).
		mr_puttilev(BORDER_LEFT + TILE_JOYBIG_WIDTH + 1, 
					BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT) + 2, 
					TILE_UP, 
					MR_COLOR_WHITE);
		mr_puttilev(BORDER_LEFT + TILE_JOYBIG_WIDTH + 3, 
					BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT) + 2, 
					TILE_DOWN, 
					MR_COLOR_WHITE);
		mr_puttilev(BORDER_LEFT + TILE_JOYBIG_WIDTH + 5, 
					BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT) + 2, 
					TILE_LEFT, 
					MR_COLOR_WHITE);
		mr_puttilev(BORDER_LEFT + TILE_JOYBIG_WIDTH + 7, 
					BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT) + 2, 
					TILE_RIGHT, 
					MR_COLOR_WHITE);
		mr_puttilev(BORDER_LEFT + TILE_JOYBIG_WIDTH + 9, 
					BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT) + 2, 
					TILE_FIRE, 
					MR_COLOR_WHITE);
	}

	// Endless loop...
	while (1) {

		// Check each available joystick's port.
		for (joystickIndex = 0; joystickIndex < MR_JOYSTICK_COUNT; ++joystickIndex) {

			// Draw the joystick port's number (in green) to give
			// evidence of the joystick port tested.
			mr_puttilev(BORDER_LEFT - 1, 
						BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT) + TILE_JOYBIG_HEIGHT - 1, 
						TILE_DIGIT1 + joystickIndex, 
						MR_COLOR_GREEN);

			// Load the isomorphic register.
			joyRegister = mr_joy(joystickIndex);

			// Check the isomorphic register against the meaning of
			// the current platform, and turn on/off the related led.

			// ---> UP
			if ((joyRegister & MR_JOYSTICK_UP) == MR_JOYSTICK_UP) {
				mr_puttilev(BORDER_LEFT + TILE_JOYBIG_WIDTH + 1, 
							BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT) + 3, 
							TILE_LED1, 
							MR_COLOR_RED);
			} else {
				mr_puttilev(BORDER_LEFT + TILE_JOYBIG_WIDTH + 1, 
							BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT) + 3, 
							TILE_LED0, 
							MR_COLOR_WHITE);
			}

			// ---> DOWN
			if ((joyRegister & MR_JOYSTICK_DOWN) == MR_JOYSTICK_DOWN) {
				mr_puttilev(BORDER_LEFT + TILE_JOYBIG_WIDTH + 1 + 2, 
							BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT) + 3, 
							TILE_LED1, 
							MR_COLOR_RED);
			} else {
				mr_puttilev(BORDER_LEFT + TILE_JOYBIG_WIDTH + 1 + 2, 
							BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT) + 3, 
							TILE_LED0, 
							MR_COLOR_WHITE);
			}

			// ---> LEFT
			if ((joyRegister & MR_JOYSTICK_LEFT) == MR_JOYSTICK_LEFT) {
				mr_puttilev(BORDER_LEFT + TILE_JOYBIG_WIDTH + 1 + 4, 
							BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT) + 3, 
							TILE_LED1, 
							MR_COLOR_RED);
			} else {
				mr_puttilev(BORDER_LEFT + TILE_JOYBIG_WIDTH + 1 + 4, 
							BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT) + 3, 
							TILE_LED0, 
							MR_COLOR_WHITE);
			}

			// ---> RIGHT
			if ((joyRegister & MR_JOYSTICK_RIGHT) == MR_JOYSTICK_RIGHT) {
				mr_puttilev(BORDER_LEFT + TILE_JOYBIG_WIDTH + 1 + 6, 
							BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT) + 3, 
							TILE_LED1, 
							MR_COLOR_RED);
			} else {
				mr_puttilev(BORDER_LEFT + TILE_JOYBIG_WIDTH + 1 + 6, 
							BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT) + 3, 
							TILE_LED0, 
							MR_COLOR_WHITE);
			}

			// ---> FIRE
			if ((joyRegister & MR_JOYSTICK_FIRE) == MR_JOYSTICK_FIRE) {
				mr_puttilev(BORDER_LEFT + TILE_JOYBIG_WIDTH + 1 + 8, 
							BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT) + 3, 
							TILE_LED1, 
							MR_COLOR_RED);
			} else {
				mr_puttilev(BORDER_LEFT + TILE_JOYBIG_WIDTH + 1 + 8, 
							BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT) + 3, 
							TILE_LED0, 
							MR_COLOR_WHITE);
			}

			// Now we check the "raw" isomorphic register.

			// For each bit (7...0).
			for (registerBitIndex = 7; registerBitIndex != 255; --registerBitIndex) {

				// Check the less significant bit.
				if ((joyRegister & 1) == 1) {
					mr_puttilev(BORDER_LEFT + TILE_JOYBIG_WIDTH + 1 + (registerBitIndex * 2), 
								BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT) + 1,
								TILE_LED1, 
								MR_COLOR_RED);
				} else {
					mr_puttilev(BORDER_LEFT + TILE_JOYBIG_WIDTH + 1 + (registerBitIndex *2), 
								BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT)+1,
								TILE_LED0, 
								MR_COLOR_WHITE);
				}

				// Shift the register, in order to check 
				// the following bit.
				joyRegister = joyRegister >> 1;
			}

			// Redraw the joystick port's number (in yellow) to give
			// evidence of the joystick port has been already tested.
			mr_puttilev(BORDER_LEFT - 1, 
						BORDER_TOP + (joystickIndex * ELEMENT_HEIGHT) + TILE_JOYBIG_HEIGHT - 1,
						TILE_DIGIT1 + joystickIndex,
						MR_COLOR_YELLOW);
		}

	}

}

#endif
