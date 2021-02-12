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

#include "midres.h"

#if defined(MIDRES_STANDALONE_DRAWING2)

int iabs(int _a);

// Draws a line onto the bitmap.
void _mr_line(mr_mixel* _screen, mr_color* _colormap, mr_position _x1, mr_position _y1, mr_position _x2, mr_position _y2, mr_color _color) {
    int x1 = _x1, y1 = _y1;
    int x2 = _x2, y2 = _y2;
    int dx = iabs(x2 - x1), sx = _x1 < _x2 ? 1 : -1;
    int dy = -iabs(y2 - y1), sy = _y1 < _y2 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */
    for (;;) {  /* loop */
        _mr_putpixel(_screen, _colormap, (mr_position)x1, (mr_position)y1, _color);
        if (x1 == _x2 && y1 == _y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; } /* e_xy+e_x > 0 */
        if (e2 <= dx) { err += dx; y1 += sy; } /* e_xy+e_y < 0 */
    }
}

#endif