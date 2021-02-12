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

#ifndef _MIDRES_TILE_HD_H_
#define _MIDRES_TILE_HD_H_

#define mr_tileset_copy_memory(_source, _destination) \
        mr_tileset_copy_memory_hd(_source, _destination)
#define mr_tileset_multicolor_to_monocolor(_source, _starting, _count) \
        mr_tileset_multicolor_to_monocolor_hd(_source, _starting, _count)
#define mr_tile_redefine(_tileset, _tile, _data) \
        mr_tile_redefine_hd(_tileset, _tile, _data)
#define mr_tileset_load_file(_index, _tileset, _starting, _count) \
        mr_tileset_load_file_hd(_index, _tileset, _starting, _count)
#define mr_tile_redefine_fill(_tileset, _tile, _data) \
        mr_tile_redefine_fill_hd(_tileset, _tile, _data)

#endif