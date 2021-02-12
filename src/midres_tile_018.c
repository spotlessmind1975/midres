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

#if defined(MIDRES_STANDALONE_TILE)

void _mr_putetiles_monocolor(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile_start, mr_position _w, mr_position _h, mr_color _color) {

    int offset = CALCULATE_OFFSET(_x,_y);
    mr_position w = _w;

    _screen += offset;
    if (_colormap) {
        _colormap += offset;
    }

    for (; _h != 0; --_h) {
        mr_position w = _w;
        for (; w != 0; --w, ++_tile_start) {
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(_screen, _tile_start));
            if (_colormap) {
                MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(_colormap, _color));
            }
            ++_screen;
            if (_colormap) {
                ++_colormap;
            }
        }
        _screen += MR_SCREEN_ROW_WIDTH - _w;
        if (_colormap) {
            _colormap += MR_SCREEN_ROW_WIDTH - _w;
        }
    }

}

#endif
