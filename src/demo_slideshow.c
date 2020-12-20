/*****************************************************************************
 * MIDRES Library - an isomorphic gamelib for retrocomputers                 *
 *****************************************************************************
 * Copyright 2020 Marco Spedaletti (asimov@mclink.it)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *----------------------------------------------------------------------------
 * Concesso in licenza secondo i termini della Licenza Apache, versione 2.0
 * (la "Licenza"); è proibito usare questo file se non in conformità alla
 * Licenza. Una copia della Licenza è disponibile all'indirizzo:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Se non richiesto dalla legislazione vigente o concordato per iscritto,
 * il software distribuito nei termini della Licenza è distribuito
 * "COSÌ COM'È", SENZA GARANZIE O CONDIZIONI DI ALCUN TIPO, esplicite o
 * implicite. Consultare la Licenza per il testo specifico che regola le
 * autorizzazioni e le limitazioni previste dalla medesima.
 ****************************************************************************/

#ifdef __DEMO_SLIDESHOW__

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

void demo_slideshow() {

    // Initialize the graphical subsystem.
    mr_init();

    // Clear the current screen and show it.
    mr_clear(MR_SCREEN_DEFAULT);
    mr_show(MR_SCREEN_DEFAULT);

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
                    mr_load(filename, MR_SCREEN_DEFAULT);
                    break;
                // ... load the color bitmap.
                case 2:
                    mr_load_color(filename, MR_SCREEN_DEFAULT);
                    break;
                // ... load the compressed bitmap.
                case 3:
                    mr_load(filename, MR_AUX_DEFAULT);
                    mr_uncompress(MR_AUX_DEFAULT, MR_SCREEN_DEFAULT);
                    break;
                // ... does nothing, unknown mode.
                default:
                    break;
            }

            mr_wait(1);
        }

        // Close the file.
        fclose(slideshow);

    }

}

#endif