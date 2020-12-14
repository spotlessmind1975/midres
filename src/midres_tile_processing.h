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