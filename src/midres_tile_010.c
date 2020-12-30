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

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>

#include "midres.h"

#if defined(MIDRES_STANDALONE_TILE)

// Writes a tile into a bitmap at *precise* vertical position.
void _mr_tile_moveto_vertical_monocolor(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_color _color) {

    int offset = CALCULATE_OFFSET((_x >> 3), (_y >> 3));
    mr_tile t = _tile + (_y & 0x07) + 1;
    mr_mixel* screen = _screen + offset;

    MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(screen, t));
    if (_colormap) {
        _colormap += offset;
        MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(_colormap, _color));
    }
    if ((screen+MR_SCREEN_ROW_WIDTH) < ( _screen + MR_SCREEN_RAM_SIZE)) {
        t += 9;
        screen += MR_SCREEN_ROW_WIDTH;
        MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(screen, t));
        if (_colormap) {
            _colormap += MR_SCREEN_ROW_WIDTH;
            MR_PROTECTED_ACCESS_VRAM(MR_WRITE_VRAM(_colormap, _color));
        }
    }

}
#endif
