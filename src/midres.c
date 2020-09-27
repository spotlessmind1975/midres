/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * DRAWING LIBRARY                                                          *
 ****************************************************************************/

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>

#include "midres.h"

/****************************************************************************
 ** RESIDENT FUNCTIONS SECTION
 ****************************************************************************/

 // Overlay management is driven by the definition of the appropriate 
 // compilation symbol (__OVERLAY__). In this case, we enable or disable the 
 // compilation of the relevant code.

#ifdef __OVERLAY__MIDRES__

    /************************************************************************
     ** OVERLAY MANAGEMENT SECTION
     ************************************************************************/

     // We differentiate management if we are with a Commodore system or not. 
     // The difference lies in the fact that, in the case of "commodore" 
     // (__CBM__) targets, we take advantage of the fact that the binaries 
     // produced contain, at the beginning of the file, the starting position 
     // where to load the code.

    #ifndef __CBM__

            //-------------------------------------------------------------------
            // GENERAL OVERLAY MANAGEMENT
            //-------------------------------------------------------------------

        #include <fcntl.h>
        #include <unistd.h>

        /**
         * This function loads a module (code / data) named "module_name"
         * from the mass storage into memory. The module will be loaded
         * starting from address "overlay_address" for a length of
         * overlay_size bytes.
         * It returns 0 if any error occours, and print out an error message.
         */
        unsigned char load_overlay(char* module_name, void* overlay_address, void* overlay_size)
        {
            int f = open(module_name, O_RDONLY);
            if (f == -1) {
                return 0;
            }
            read(f, overlay_address, (unsigned)overlay_size);
            close(f);
            return 1;
        }

    #else

            //-------------------------------------------------------------------
            // COMMODORE OVERLAY MANAGEMENT
            //-------------------------------------------------------------------

        #include <cbm.h>
        #include <device.h>

        /**
         * This function loads a module (code / data) named "module_name"
         * from the mass storage into the address present in the header of
         * the binary file.
         * It returns 0 if any error occours, and print out an error message.
         */
        unsigned char load_overlay(char* module_name, void* overlay_address, void* overlay_size)
        {
            // Ignore overlay_address and overlay_size parameters
            (void)overlay_address; (void)overlay_size;
            if (cbm_load(module_name, getcurrentdevice(), NULL) == 0) {
                return 0;
            }
            return 1;
        }

    #endif

#endif

void mr_init() {

    WIDTH = MR_SCREEN_WIDTH;
    HEIGHT = MR_SCREEN_HEIGHT;
    BRIGHTNESS = MR_COLOR_BRIGHTNESS;

    mr_init_hd();

}

void mr_init_multicolor() {

    WIDTH = MR_SCREEN_WIDTH;
    HEIGHT = MR_SCREEN_HEIGHT;
    BRIGHTNESS = MR_COLOR_BRIGHTNESS;

    mr_init_multicolor_hd();

}

void mr_cleanup() {

    mr_cleanup_hd();

}
