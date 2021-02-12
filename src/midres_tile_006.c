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

// Writes a tile into a bitmap at *precise* horizontal position.
void _mr_tile_moveto_horizontal_monocolor(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_color _color) {

    int offset = CALCULATE_OFFSET((_x >> 3),(_y >> 3));
    mr_tile t = _tile + (_x & 0x07);
    _screen += offset;
    if (_colormap) {
        _colormap += offset;
    }

    if (_x >= 0 && ((_x >> 3)) < MR_SCREEN_WIDTH) {
        MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(_screen, t));
        if (_colormap) {
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(_colormap, _color));
        }
    }

    if ((_x + 8) >= 0 && ((_x >> 3) + 1) < MR_SCREEN_WIDTH) {
        ++_screen;
        t += 8;
        MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(_screen, t));
        if (_colormap) {
            ++_colormap;
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(_colormap, _color));
        }
    }

    if ((_x + 16) >= 0 && ((_x >> 3) + 2) < MR_SCREEN_WIDTH) {
        ++_screen;
        t = _tile + 0x08;
        MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(_screen, t));
        if (_colormap) {
            ++_colormap;
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(_colormap, _color));
        }
    }
    
}
#endif
