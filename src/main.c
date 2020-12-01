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

void main(void) {

#ifdef __DEMO_SLIDESHOW__

    demo_slideshow();

#endif

#ifdef __DEMO_DRAWING__

    demo_drawing();

#endif

#ifdef __DEMO_BITBLIT__

#ifndef __OVERLAY__MIDRES__

    demo_bitblit();

#endif

#endif

#ifdef __DEMO_TILE__

#ifndef __OVERLAY__MIDRES__

    demo_tile();

#endif

#endif

#ifdef __DEMO_TEST_CARD__

    demo_test_card();

#endif

#ifdef __GAME_AIR_ATTACK__

    game_air_attack();

#endif

#ifdef __GAME_TOTTO__

    game_totto();

#endif

#ifdef __TUTORIAL_MCTILE__

    tutorial_mctile();

#endif

#ifdef __TUTORIAL_PROTOTHREAD1__

    tutorial_protothread1();

#endif

#ifdef __TUTORIAL_PROTOTHREAD2__

    tutorial_protothread2();

#endif

#ifdef __GAME_ALIEN_STORM__

    game_alien_storm();

#endif

#ifdef __GAME_ELEVATOR__

    game_elevator();

#endif

#ifdef __UTILITY_JOYCHECK__

    utility_joycheck();

#endif

}