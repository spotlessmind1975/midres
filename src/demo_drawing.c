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

#define BUILDINGS_COUNT         (WIDTH>30?6:3)
#define BUILDINGS_OFFSET        4
#define BUILDINGS_HEIGHT        5
#define BUILDINGS_DISTANCE      ( WIDTH*2 - (BUILDINGS_OFFSET*2) ) / BUILDINGS_COUNT
#define BUILDINGS_WIDTH         BUILDINGS_DISTANCE * 7 / 12
#define BUILDINGS_WINDOW_SIZE   2
#define BUILDINGS_WINDOWS       ( HEIGHT*2 - BUILDINGS_HEIGHT ) / (BUILDINGS_WINDOW_SIZE*3)
#define BUILDINGS_DOOR_SIZE     2

void draw_building(mr_position _number) {
    mr_position j = 0;
    for (j = 0; j < (BUILDINGS_WIDTH+1); ++j) {
        mr_vline(DEFAULT_SCREEN, BUILDINGS_OFFSET + _number * BUILDINGS_DISTANCE + j, BUILDINGS_HEIGHT, HEIGHT * 2 - 1, MR_COLOR_CYAN);
    }
    for (j = 0; j < BUILDINGS_WINDOWS; ++j) {
        mr_rect(DEFAULT_SCREEN, 
                BUILDINGS_OFFSET + _number * BUILDINGS_DISTANCE + 
                BUILDINGS_WINDOW_SIZE + (j & 1) * BUILDINGS_WINDOW_SIZE, 
                j * (3*BUILDINGS_WINDOW_SIZE) + BUILDINGS_HEIGHT + BUILDINGS_WINDOW_SIZE+1,
                BUILDINGS_OFFSET + _number * BUILDINGS_DISTANCE + 
                BUILDINGS_WINDOW_SIZE+1 + (j & 1) * BUILDINGS_WINDOW_SIZE, 
                j * (3 * BUILDINGS_WINDOW_SIZE) + BUILDINGS_HEIGHT + BUILDINGS_WINDOW_SIZE + 1,
                MR_COLOR_BLACK);
    }
    mr_rect(DEFAULT_SCREEN, 
        BUILDINGS_OFFSET + _number * BUILDINGS_DISTANCE + BUILDINGS_DOOR_SIZE,
        HEIGHT * 2 - BUILDINGS_DOOR_SIZE,
        BUILDINGS_OFFSET + _number * BUILDINGS_DISTANCE + 2*BUILDINGS_DOOR_SIZE,
        HEIGHT * 2 - 1, 
        MR_COLOR_BLACK);
}

void destroy_building(mr_position _number) {
    mr_position j;
    for (j = BUILDINGS_HEIGHT; j < HEIGHT * 2 - 2*BUILDINGS_DOOR_SIZE; ++j) {
        mr_hline(DEFAULT_SCREEN, 
            BUILDINGS_OFFSET + _number * BUILDINGS_DISTANCE, 
            BUILDINGS_OFFSET + _number * BUILDINGS_DISTANCE + BUILDINGS_WIDTH, 
            j, 
            MR_COLOR_BLACK);
    }
    for (; j < HEIGHT * 2 - 1; ++j) {
        mr_hline(DEFAULT_SCREEN, 
            BUILDINGS_OFFSET + _number * BUILDINGS_DISTANCE, 
            BUILDINGS_OFFSET + _number * BUILDINGS_DISTANCE + BUILDINGS_WIDTH, 
            j, 
            _number + 2);
    }
    mr_rect(DEFAULT_SCREEN,
        BUILDINGS_OFFSET + _number * BUILDINGS_DISTANCE + BUILDINGS_DOOR_SIZE,
        HEIGHT * 2 - BUILDINGS_DOOR_SIZE,
        BUILDINGS_OFFSET + _number * BUILDINGS_DISTANCE + 2 * BUILDINGS_DOOR_SIZE,
        HEIGHT * 2 - 1,
        MR_COLOR_BLACK);
}

void demo_drawing() {

    int i = 0, j = 0;

    mr_init();

    mr_use_screen();

    mr_show(DEFAULT_SCREEN);

    while (1) {
        mr_clear_bitmap(DEFAULT_SCREEN);
        for (i = 0; i < BUILDINGS_COUNT; ++i) {
            draw_building(i);
        }
        for (i = 0; i < BUILDINGS_COUNT; ++i) {
            destroy_building(i);
        }
    }

    mr_cleanup();

}