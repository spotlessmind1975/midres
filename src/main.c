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

unsigned char mode = 0xff;
unsigned char filename[16] = { 0 };

void main(void) {

#ifdef __DEMO_SLIDESHOW__

    FILE* slideshow = fopen("slideshow", "rb");

    if (slideshow == NULL) {
        return;
    }

    mr_init();

    mr_use_screen();

    mr_show(DEFAULT_SCREEN);

    while (mode) {
        // In this way, we are reading directly into mode and filename.
        // This save space in MAIN segment!
        mode = 0;
        fread(&mode, 1, 1, slideshow);
        fread(filename, 16, 1, slideshow);
        switch (mode) {
            case 1:
                mr_load(filename, DEFAULT_SCREEN);
                break;
            case 2:
                mr_load_color(filename, DEFAULT_SCREEN);
                break;
            case 3:
                mr_load(filename, 5);
                mr_uncompress(5, DEFAULT_SCREEN);
                break;
            default:
                break;
        }
    }

    fclose(slideshow);

    getchar();

    mr_cleanup();

#endif

#ifdef __DEMO_DRAWING__

    demo_drawing();

#endif

}