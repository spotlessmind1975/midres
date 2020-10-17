#ifdef __DEMO_DRAWING__

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

// Number of buildings to draw in a screen (it depends on width)
#define BUILDINGS_COUNT         ((MR_SCREEN_WIDTH>30)?6:3)

// Offset from the borders (in mixels)
#define BUILDINGS_OFFSET        4

// Height of the root of the buildings (from the top of the screen)
#define BUILDINGS_HEIGHT        5

// Distance (in mixels) between buildings
#define BUILDINGS_DISTANCE      ( ( (MR_SCREEN_WIDTH * 2) ) / BUILDINGS_COUNT )

// Width of a building (in mixels)
#define BUILDINGS_WIDTH         ( ( BUILDINGS_DISTANCE * 7 ) / 12 )

// Width and height of a window (in mixels)
#define BUILDINGS_WINDOW_SIZE   2

// Number of windows on a single building.
#define BUILDINGS_WINDOWS       ( (MR_SCREEN_HEIGHT*2) - BUILDINGS_HEIGHT ) / (BUILDINGS_WINDOW_SIZE*3)

// Half a width and the height of a door (in mixels)
#define BUILDINGS_DOOR_SIZE     2

/****************************************************************************
** RESIDENT FUNCTIONS SECTION
****************************************************************************/

// All the functions defined within the resident body of the code are 
// accessible from all modules, both resident and changing ones.

// Draw the _number-nth building.
void draw_building(mr_position _number) {
    
    // Used as index
    mr_position j = 0;

    // Draw as many vertical lines as the width of the building.
    // Each line is as long as the height of the building.
    for (j = 0; j < (BUILDINGS_WIDTH + 1); ++j) {
        mr_vline( MR_SCREEN_DEFAULT, 
                    BUILDINGS_OFFSET + (_number * BUILDINGS_DISTANCE ) + j,  BUILDINGS_HEIGHT, 
                    MR_SCREEN_HEIGHT * 2 - 1, 
                    MR_COLOR_CYAN);
    }

    // Draw each window.
    for (j = 0; j < BUILDINGS_WINDOWS; ++j) {
        mr_rect(MR_SCREEN_DEFAULT,
            BUILDINGS_OFFSET + (_number * BUILDINGS_DISTANCE ) +
            BUILDINGS_WINDOW_SIZE + (j & 1) * BUILDINGS_WINDOW_SIZE,
            j * (3 * BUILDINGS_WINDOW_SIZE) + BUILDINGS_HEIGHT + BUILDINGS_WINDOW_SIZE + 1,
            BUILDINGS_OFFSET + (_number * BUILDINGS_DISTANCE ) +
            BUILDINGS_WINDOW_SIZE + 1 + (j & 1) * BUILDINGS_WINDOW_SIZE,
            j * (3 * BUILDINGS_WINDOW_SIZE) + BUILDINGS_HEIGHT + BUILDINGS_WINDOW_SIZE + 1,
            MR_COLOR_BLACK);
    }

    // Finally, draw the door.
    mr_rect(MR_SCREEN_DEFAULT,
        BUILDINGS_OFFSET + (_number * BUILDINGS_DISTANCE ) + BUILDINGS_DOOR_SIZE,
        MR_SCREENHEIGHT * 2 - BUILDINGS_DOOR_SIZE,
        BUILDINGS_OFFSET + (_number * BUILDINGS_DISTANCE ) + 2 * BUILDINGS_DOOR_SIZE,
        MR_SCREEN_HEIGHT * 2 - 1,
        MR_COLOR_BLACK);
}

// Destroy the _number-nth building.
void destroy_building(mr_position _number) {

    // Used as index
    mr_position j;

    // We destroy the building starting from the top, by drawing
    // an horizontal line that is as long as its width.
    for (j = BUILDINGS_HEIGHT; j < MR_SCREEN_HEIGHT * 2 - 2 * BUILDINGS_DOOR_SIZE; ++j) {
        mr_hline(MR_SCREEN_DEFAULT,
            BUILDINGS_OFFSET + _number * BUILDINGS_DISTANCE,
            BUILDINGS_OFFSET + _number * BUILDINGS_DISTANCE + BUILDINGS_WIDTH,
            j,
            MR_COLOR_BLACK);
    }

    // The last pixels will be coloured by an index _number based.
    for (; j < MR_SCREEN_HEIGHT * 2 - 1; ++j) {
        mr_hline(MR_SCREEN_DEFAULT,
            BUILDINGS_OFFSET + _number * BUILDINGS_DISTANCE,
            BUILDINGS_OFFSET + _number * BUILDINGS_DISTANCE + BUILDINGS_WIDTH,
            j,
            _number + 2);
    }

    // Draw the door again, finally.
    mr_rect(MR_SCREEN_DEFAULT,
        BUILDINGS_OFFSET + _number * BUILDINGS_DISTANCE + BUILDINGS_DOOR_SIZE,
        MR_SCREEN_HEIGHT * 2 - BUILDINGS_DOOR_SIZE,
        BUILDINGS_OFFSET + _number * BUILDINGS_DISTANCE + 2 * BUILDINGS_DOOR_SIZE,
        MR_SCREEN_HEIGHT * 2 - 1,
        MR_COLOR_BLACK);
}

/****************************************************************************
 ** RESIDENT MAIN FUNCTION
 ****************************************************************************/

// This is the main function body. The purpose is to call the various 
// functions present in the modules, taking care to load the relevant 
// code / data into memory(in the "overlay" area).

void demo_drawing() {

    // Used as index.
    int i = 0, j = 0;

    // Initialize the graphical subsystem.
    mr_init();

    // We are going to use a "drawing2" (drawing v1.1) function.
    // [needed for overlay support]
    mr_use_screen();

    // Clear the current screen and show it.
    mr_show(MR_SCREEN_DEFAULT);

    // We are going to use a "drawing2" (drawing v1.1) function.
    // [needed for overlay support]
    mr_use_drawing2();

    // Endless loop
    while (1) {

        // Clear the bitmap.
        mr_clear_bitmap(MR_SCREEN_DEFAULT);

        // Draw each building.
        for (i = 0; i < BUILDINGS_COUNT; ++i) {
            draw_building(i);
        }

        // Destroy each building.
        for (i = 0; i < BUILDINGS_COUNT; ++i) {
            destroy_building(i);
        }

    }

}

#endif
