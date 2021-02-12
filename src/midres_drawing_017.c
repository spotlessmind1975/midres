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

// Draws a circle.
void _mr_circle(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_position _radius, mr_color _color) {
    int x = -_radius, y = 0, err = 2 - 2 * _radius; /* II. Quadrant */
    do {
        _mr_putpixel(_screen, _colormap, _x - x, _y + y, _color); /*   I. Quadrant */
        _mr_putpixel(_screen, _colormap, _x - y, _y - x, _color); /*  II. Quadrant */
        _mr_putpixel(_screen, _colormap, _x + x, _y - y, _color); /* III. Quadrant */
        _mr_putpixel(_screen, _colormap, _x + y, _y + x, _color); /*  IV. Quadrant */
        _radius = err;
        if (_radius <= y) err += ++y * 2 + 1;           /* e_xy+e_y < 0 */
        if (_radius > x || err > y) err += ++x * 2 + 1; /* e_xy+e_x > 0 or no 2nd y-step */
    } while (x < 0);
}

#endif