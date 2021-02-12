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

  // Writes a pixel into a bitmap.
void _mr_putpixel(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_color _color) {

    mr_position mx, my;
    mr_mixelbits abcd;
    int offset;

    mx = _x >> 1;
    my = _y >> 1;
    offset = my * MR_SCREEN_ROW_WIDTH + mx;

    // x1 * y1 + ( (x1 - 1) * y1) * 2+ ( x1 * (y1-1)) * 4 + ( (x1 - 1 ) * (y1-1)) * 8;
    // x1 * y1 + ((x1 - 1) * y1) * 2 + (x1 * (y1 - 1)) * 4 + ((x1 - 1) * (y1 - 1)) * 8;
    // x1y1 + (x1y1 - y1) * 2 + (x1y1 - x1) * 4 + (x1y1 - y1 + 1 - x1) * 8;
    // x1y1 + 2*x1y1 - 2*y1 + 4*x1y1 - 4*x1 + 8*x1y1 - 8*y1 + 8 - 8*x1;

    abcd = mr_mixel_bits(_x, _y);

    MR_WRITE_TILE(_screen, _colormap, offset, MR_RENDERED_MIXELS[get_mixel_bits(MR_READ_TILE(_screen, offset)) | abcd], _color);

}

#endif