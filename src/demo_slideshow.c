#ifdef __DEMO_SLIDESHOW__

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

// Used to store the display mode for the given image file:
//  - 1 = show the image as a luminance bitmap (mixels only)
//  - 2 = show the image as a color bitmap (colors only)
//  - 3 = show the image as a compressed bitmap (mixels and colors)
unsigned char mode = 0xff;

// Used to store the filename of the image to show
unsigned char filename[16] = { 0 };

/****************************************************************************
** RESIDENT MAIN FUNCTION
****************************************************************************/

// This is the main function body. The purpose is to call the various 
// functions present in the modules, taking care to load the relevant 
// code / data into memory(in the "overlay" area).

void demo_slideshow() {

    // Initialize the graphical subsystem.
    mr_init();

    // We are going to use a "screen" function.
    // [needed for overlay support]
    mr_use_screen();

    // Clear the current screen and show it.
    mr_clear(DEFAULT_SCREEN);
    mr_show(DEFAULT_SCREEN);

    // Endless loop.
    while (1) {

        // Open the file with the slideshow data.
        FILE* slideshow = fopen("slideshow", "rb");
        if (slideshow == NULL) {
            return;
        }

        // It is the current mode requested.
        // (0xff is needed to make at least one loop)
        mode = 0xff;

        // While not end of file...
        while (mode) {

            // Read the mode and filename.
            mode = 0;
            fread(&mode, 1, 1, slideshow);
            fread(filename, 16, 1, slideshow);

            // Based on the mode...
            switch (mode) {
                // ... load the luminance bitmap.
                case 1:
                    mr_load(filename, DEFAULT_SCREEN);
                    break;
                // ... load the color bitmap.
                case 2:
                    mr_load_color(filename, DEFAULT_SCREEN);
                    break;
                // ... load the compressed bitmap.
                case 3:
                    mr_load(filename, AUXILIARY_SCREEN);
                    mr_uncompress(AUXILIARY_SCREEN, DEFAULT_SCREEN);
                    break;
                // ... does nothing, unknown mode.
                default:
                    break;
            }
        }

        // Close the file.
        fclose(slideshow);

    }

}

#endif