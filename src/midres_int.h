/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

#ifndef _MIDRES_INT_H_
#define _MIDRES_INT_H_

    #ifdef __OVERLAY__MIDRES__

         // These variables are defined by the linker, and allow to identify the 
         // address where to store the pieces of codeand data loaded from the 
         // mass memories.
        extern void _OVERLAY1_LOAD__[], _OVERLAY1_SIZE__[];
        extern void _OVERLAY2_LOAD__[], _OVERLAY2_SIZE__[];
        extern void _OVERLAY3_LOAD__[], _OVERLAY3_SIZE__[];
        extern void _OVERLAY4_LOAD__[], _OVERLAY4_SIZE__[];
        extern void _OVERLAY4_LOAD__[], _OVERLAY4_SIZE__[];

        unsigned char load_overlay(char* module_name, void* overlay_address,
            void* overlay_size);

    #endif

    /***
     * This value is used to check if the screen memory is corrupted.
     */

    #define MIXEL_THRESHOLD	0xff

    /************************************************************************
     * ------ INTERNAL FUNCTIONS
     ************************************************************************/

    mr_mixel get_mixel_bits(mr_mixel _current);
    mr_mixel calculate_mixel(mr_mixel _current, mr_mixelbits _abcd, mr_pop _pop);

#endif