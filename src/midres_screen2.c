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

#ifdef __CBM__
#include <cc65.h>
#include <cbm.h>
#include <device.h>
#else

#endif

#if ( !defined(__OVERLAY__MIDRES__) && defined(MIDRES_STANDALONE_SCREEN2) ) || defined(__OVERLAY__MIDRES__)

  // Overlay management is driven by the definition of the appropriate 
  // compilation symbol (__OVERLAY__). In this case, we enable or disable the 
  // compilation of the relevant code.

#ifdef __OVERLAY__MIDRES__

    // In general, the compiler places the executable code in the "CODE" 
    // segment, while the non-modifiable data in the "RODATA" segment.
    // In order to save space, both of these segments are moved to the 
    // overlay segment, so that both the code and the data it uses are 
    // removed from the resident module.
    #pragma code-name ("OVERLAY3");
    #pragma rodata-name ("OVERLAY3");

#endif

/****************************************************************************
 ** RESIDENT VARIABLES SECTION
 ****************************************************************************/

/****************************************************************************
 ** OVERLAYED FUNCTIONS SECTION
 ****************************************************************************/

// The functions defined at this level can only be called up if the current
// module has been loaded into memory. On the other hand, they can call any 
// function declared at the resident module level.

unsigned char mr_save(char* _filename, mr_screen _screen) {

#ifdef __ATARI__

    FILE* f = fopen(_filename, "wb");
    if (f == NULL) {
        return 0;
    }
    fwrite(SM(_screen), (unsigned)MR_SCREEN_RAM_SIZE, 1, f);
    fclose(f);
    return 1;

#elif __CBM__

    return cbm_save(_filename, getcurrentdevice(), SM(_screen), MR_SCREEN_RAM_SIZE);

#endif


}

unsigned char mr_save_color(char* _filename, mr_screen _screen) {

#ifdef __ATARI__

    FILE* f = fopen(_filename, "wb");
    if (f == NULL) {
        return 0;
    }
    fwrite(CM(_screen), (unsigned)MR_SCREEN_RAM_SIZE, 1, f);
    fclose(f);
    return 1;

#elif __CBM__

    return cbm_save(_filename, getcurrentdevice(), CM(_screen), MR_SCREEN_RAM_SIZE);

#endif

}

void mr_compress(mr_screen _source, mr_screen _destination) {

    mr_mixel* source = SM(_source);
    mr_mixel* destination = SM(_destination);
    mr_color* sourceColor = CM(_source);

    int i;
    for (i = 0; i < MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT; ++i) {
        destination[i] = mr_pack_mixels(sourceColor[i], get_mixel_bits(source[i]));
    }

}

void mr_pack(mr_screen _source, mr_screen _destination, mr_half_screen _half_screen) {

    int i;
    mr_mixel* source = SM(_source);
    mr_mixel* destination = SM(_destination);

    if (_half_screen == mr_half_up) {
        destination += MR_SCREEN_RAM_SIZE >> 1;
    }

    for (i = 0; i < MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT; i += 2) {
        destination[i / 2] = mr_pack_mixels(get_mixel_bits(source[i + 1]), get_mixel_bits(source[i]));
    }

}

#endif