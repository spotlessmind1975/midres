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

#ifndef _MIDRES_TILE_PROCESSING_H_
#define _MIDRES_TILE_PROCESSING_H_

	// aliasing
	#define mr_tileset_copy_hd(_source, _dest) \
		mr_tileset_copy_memory_mapped(_source, _dest)
	#define mr_tileset_multicolor_to_monocolor_hd(_source, _starting, _count) \
		mr_tileset_multicolor_to_monocolor_memory_mapped(_source, _starting, _count)
	#define mr_tile_redefine_hd(_tileset, _tile, _data) \
		mr_tile_redefine_memory_mapped(_tileset, _tile, _data)
	#define mr_tile_redefine_fill_hd(_tileset, _tile, _data) \
		mr_tile_redefine_fill_memory_mapped(_tileset, _tile, _data)
	#define mr_tile_prepare_horizontal_monocolor_hd(_tileset, _source, _destination) \
		mr_tile_prepare_horizontal_monocolor_memory_mapped(_tileset, _source, _destination)
	#define mr_tile_prepare_horizontal_extended_monocolor_hd(_tileset, _source, _w, _h, _destination) \
		mr_tile_prepare_horizontal_extended_monocolor_memory_mapped(_tileset, _source, _w, _h, _destination)
	#define mr_tile_prepare_vertical_hd(_tileset, _source, _destination) \
		mr_tile_prepare_vertical_memory_mapped(_tileset, _source, _destination)
	#define mr_tile_prepare_vertical_extended_hd(_tileset, _source, _w, _h, _destination) \
		mr_tile_prepare_vertical_extended_memory_mapped(_tileset, _source, _w, _h, _destination)
	#define mr_tile_prepare_roll_horizontal_monocolor_hd(_tileset, _source, _destination) \
		mr_tile_prepare_roll_horizontal_monocolor_memory_mapped(_tileset, _source, _destination)
	#define mr_tile_roll_horizontal_hd(_tileset, _destination, _direction) \
		mr_tile_roll_horizontal_memory_mapped(_tileset, _destination, _direction)
	#define mr_tile_roll_horizontal_on_place_hd(_tileset, _destination, _direction, _place, _index) \
		mr_tile_roll_horizontal_on_place_memory_mapped(_tileset, _destination, _direction, _place, _index)
	#define mr_tile_prepare_roll_vertical_hd(_tileset, _source, _destination) \
		mr_tile_prepare_roll_vertical_memory_mapped(_tileset, _source, _destination)
	#define mr_tile_roll_vertical_hd(_tileset, _destination, _direction) \
		mr_tile_roll_vertical_memory_mapped(_tileset, _destination, _direction)
	#define mr_tileset_load_file_hd(_index, _tileset, _starting, _count) \
		mr_tileset_load_file_memory_mapped(42, _tileset, _starting, _count)

#endif