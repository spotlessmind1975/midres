/*****************************************************************************
 * MIDRES Library - an isomorphic gamelib for retrocomputers                 *
 *****************************************************************************
 * Copyright 2020-2021 Marco Spedaletti (asimov@mclink.it)
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

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>

#include "midres.h"

#if defined(MIDRES_STANDALONE_BITBLIT)

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
                        *_destination = MR_RENDERED_MIXELS[get_mixel_bits(*_destination) | destination];
                        if ((_blitop & mr_blitop_color) == mr_blitop_color) {
                            *_destination_color = *_source_color;
                        }
                        else {
                            *_destination_color = MR_COLOR_WHITE;
                        }
                    }
                    else {
                        *_destination = MR_RENDERED_MIXELS[get_mixel_bits(*_destination) & ~destination];
                    }
                    break;
                case mr_blitop_trans:
                    if (*_source_color != MR_COLOR_BLACK && source != 0) {
                        *_destination = MR_RENDERED_MIXELS[get_mixel_bits(*_destination) | destination];
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
                        *_destination = MR_RENDERED_MIXELS[get_mixel_bits(*_source) | destination];
                    }
                    else {
                        *_destination = MR_RENDERED_MIXELS[get_mixel_bits(*_source) & ~destination];
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
                        *_destination = MR_RENDERED_MIXELS[get_mixel_bits(*_source) | destination];
                    }
                    else {
                        *_destination = MR_RENDERED_MIXELS[get_mixel_bits(*_source) & ~destination];
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
                        *_destination = MR_RENDERED_MIXELS[get_mixel_bits(*_source) | destination];
                    }
                    else {
                        *_destination = MR_RENDERED_MIXELS[get_mixel_bits(*_source) & ~destination];
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