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