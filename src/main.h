/****************************************************************************
 * midres - Portable midres library for retrocomputers                      * 
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

// Overlay management is driven by the definition of the appropriate 
// compilation symbol (__OVERLAY__). In this case, we enable or disable the 
// compilation of the relevant code.

#ifdef __OVERLAY__

    // These variables are defined by the linker, and allow to identify the 
    // address where to store the pieces of codeand data loaded from the 
    // mass memories.
    extern void _OVERLAY1_LOAD__[], _OVERLAY1_SIZE__[];
    extern void _OVERLAY2_LOAD__[], _OVERLAY2_SIZE__[];
    extern void _OVERLAY3_LOAD__[], _OVERLAY3_SIZE__[];
    extern void _OVERLAY4_LOAD__[], _OVERLAY4_SIZE__[];

    unsigned char load_overlay(char* module_name, void* overlay_address, 
                                                        void* overlay_size);

#endif

// RESIDENT FUNCTIONS
void demo_slideshow();
void demo_drawing();
void demo_bitblit();
void demo_tile();

void game_air_attack();
void game_totto();

void tutorial_mctile();

// OVERLAYED FUNCTIONS (MODULE 1)
#include "midres.h"

// OVERLAYED FUNCTIONS (MODULE 2)

// OVERLAYED FUNCTIONS (MODULE 3)

// OVERLAYED FUNCTIONS (MODULE 4)
