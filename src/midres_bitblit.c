/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * BITBLIT LIBRARY                                                          *
 ****************************************************************************/

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>

#include "midres.h"

#if ( !defined(__OVERLAY__MIDRES__) && defined(MIDRES_STANDALONE_BITBLIT) ) || defined(__OVERLAY__MIDRES__)

  // Overlay management is driven by the definition of the appropriate 
// compilation symbol (__OVERLAY__). In this case, we enable or disable the 
// compilation of the relevant code.

#ifdef __OVERLAY__MIDRES__

    // In general, the compiler places the executable code in the "CODE" 
    // segment, while the non-modifiable data in the "RODATA" segment.
    // In order to save space, both of these segments are moved to the 
    // overlay segment, so that both the code and the data it uses are 
    // removed from the resident module.
    #pragma code-name ("OVERLAY5");
    #pragma rodata-name ("OVERLAY5");

#endif

/****************************************************************************
 ** OVERLAYED FUNCTIONS SECTION
 ****************************************************************************/

 // The functions defined at this level can only be called up if the current
 // module has been loaded into memory. On the other hand, they can call any 
 // function declared at the resident module level.

void _mr_copy(mr_mixel* _source, mr_color* _source_color, mr_position _xs, mr_position _ys, mr_position _ws, mr_position _hs, mr_position _displs, mr_mixel* _destination, mr_color* _destination_color, mr_position _xd, mr_position _yd, mr_position _displd, mr_blitop _blitop) {

    mr_position ws = _ws;
    mr_position xs = _xs;
    mr_position xd = _xd;

    int os = (_ys>>1) * _displs + (_xs>>1);
    int od = (_yd>>1) * _displd + (_xd>>1);

    _source += os;
    _destination += od;
    _source_color += os;
    _destination_color += od;

    for (; _hs > 0; ++_ys, ++_yd, --_hs) {
        ws = _ws;
        xs = _xs;
        xd = _xd;
        for (; ws > 0; ++xs, ++xd, --ws) {
            mr_mixelbits source = get_mixel_bits(*_source) & mr_mixel_bits(xs, _ys);
            mr_mixelbits destination = mr_mixel_bits(xd, _yd);

            switch (_blitop & mr_blitop_mixels) {
                case mr_blitop_set:
                    if (source) {
                        *_destination = RENDERED_MIXELS[get_mixel_bits(*_destination) | destination];
                        if ((_blitop & mr_blitop_color) == mr_blitop_color) {
                            *_destination_color = *_source_color;
                        }
                        else {
                            *_destination_color = MR_COLOR_WHITE;
                        }
                    }
                    else {
                        *_destination = RENDERED_MIXELS[get_mixel_bits(*_destination) & ~destination];
                    }
                    break;
                case mr_blitop_trans:
                    if (*_source_color != MR_COLOR_BLACK && source != 0) {
                        *_destination = RENDERED_MIXELS[get_mixel_bits(*_destination) | destination];
                        if ((_blitop & mr_blitop_color) == mr_blitop_color) {
                            *_destination_color = *_source_color;
                        } else {
                            *_destination_color = MR_COLOR_WHITE;
                        }
                    }
                    break;
                case mr_blitop_and:
                    if ((_blitop & mr_blitop_color) == mr_blitop_color) {
                        *_destination_color = *_destination_color & *_source_color;
                    }
                    else {
                        *_destination_color = MR_COLOR_WHITE;
                    }
                    if ((source & destination) != 0) {
                        *_destination = RENDERED_MIXELS[get_mixel_bits(*_source) | destination];
                    }
                    else {
                        *_destination = RENDERED_MIXELS[get_mixel_bits(*_source) & ~destination];
                    }
                    break;
                case mr_blitop_or:
                    if ((_blitop & mr_blitop_color) == mr_blitop_color) {
                        *_destination_color = *_destination_color | *_source_color;
                    }
                    else {
                        *_destination_color = MR_COLOR_WHITE;
                    }
                    if ((source | destination)!=0) {
                        *_destination = RENDERED_MIXELS[get_mixel_bits(*_source) | destination];
                    }
                    else {
                        *_destination = RENDERED_MIXELS[get_mixel_bits(*_source) & ~destination];
                    }
                    break;
                case mr_blitop_xor:
                    if ((_blitop & mr_blitop_color) == mr_blitop_color) {
                        *_destination_color = *_destination_color ^ *_source_color;
                    }
                    else {
                        *_destination_color = MR_COLOR_WHITE;
                    }
                    if (( source ^ destination) != 0) {
                        *_destination = RENDERED_MIXELS[get_mixel_bits(*_source) | destination];
                    }
                    else {
                        *_destination = RENDERED_MIXELS[get_mixel_bits(*_source) & ~destination];
                    }
                    break;
            }
            if (xs & 1) {
                ++_source;
                ++_source_color;
            }
            if (xd & 1) {
                ++_destination;
                ++_destination_color;
            }
        }
        if (_ys & 1) {
            _source += _displs - (_ws >> 1);
            _source_color += _displs - (_ws >> 1);
        }
        else {
            _source -= (_ws>>1);
            _source_color -= (_ws >> 1);
        }
        if (_yd & 1) {
            _destination += _displd - (_ws >> 1);
            _destination_color += _displd - (_ws >> 1);
        } else {
            _destination -= (_ws >> 1);
            _destination_color -= (_ws >> 1);
        }
    }

}

#endif