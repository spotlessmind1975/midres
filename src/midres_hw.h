/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

#ifndef _MIDRES_HW_H_
#define _MIDRES_HW_H_

#ifdef __C64__
#include "midres_hw_cbm.h"
#include "midres_hw_c64.h"
#elif __VIC20__
#ifdef __24K__
#include "midres_hw_cbm.h"
#include "midres_hw_vic20_32k.h"
#else
#include "midres_hw_cbm.h"
#include "midres_hw_vic20.h"
#endif
#elif __PLUS4__
#include "midres_hw_cbm.h"
#include "midres_hw_plus4.h"
#elif __C16__
#include "midres_hw_cbm.h"
#include "midres_hw_c16.h"
#elif __C128__
#include "midres_hw_cbm.h"
#include "midres_hw_c128_40.h"
#elif __ATARI__
#ifdef __LORES__
#include "midres_hw_atari_lores.h"
#else
#include "midres_hw_atari.h"
#endif
#elif __ATMOS__
#include "midres_hw_atmos.h"
#elif __COLECO__
#include "midres_hw_io.h"
#include "midres_hw_vdp.h"
#include "midres_hw_coleco.h"
#elif __MSX__
#include "midres_hw_io.h"
#include "midres_hw_vdp.h"
#include "midres_hw_msx.h"
#else
#include "midres_hw_vanilla.h"
#endif

/************************************************************************
 * ------ FUNCTIONS
 ************************************************************************/

// Hardware dependent initialization.
void mr_init_hd();

// Hardware dependent initialization (for multicolor).
void mr_init_multicolor_hd();

// Hardware dependent set screen background color
void mr_set_background_color_hd(unsigned char _color);

// Hardware dependent set screen border color
void mr_set_border_color_hd(unsigned char _color);

// Hardware dependent set a single color for multicolor tiles
void mr_tile_setcolor_hd(unsigned char _index, unsigned char _color);

// Hardware dependent set a set of colors for multicolor tiles
void mr_tile_setcolors_hd(unsigned char _colors[4]);

// Hardware dependent screen management.
void mr_show_hd(unsigned char _screen);

// Hardware dependent cleanup.
void mr_cleanup_hd();

// Hardware dependend waiting for VBL
void mr_wait_vbl();

// Hardware dependent double buffer switch
void mr_doublebuffer_switch_hd(unsigned char _screen);

// Hardware dependent tile set
void mr_tileset_visible_hd(unsigned char _tileset);

// Hardware dependent key press listener
unsigned char mr_key_pressed_hd();

// Hardware dependent get key pressed
unsigned char mr_get_key_pressed_hd();

// Hardware dependent time wait
void mr_wait_hd(unsigned char _seconds);

// Hardware dependent jiffies
void mr_wait_jiffies_hd(unsigned char _jiffies);

// Hardware dependent sound library
void mr_sound_start_hd(unsigned char _channel, unsigned char _number);

// Hardware dependent sound library
void mr_sound_change_hd(unsigned char _channel, int _parameter);

// Hardware dependent sound library
void mr_sound_stop_hd(unsigned char _channel);

void mr_start_frame_hd();

void mr_end_frame_hd(unsigned char _jiffies);

// Hardware dependent translate filename (if needed)
unsigned char* mr_translate_file_hd(unsigned int _file);

unsigned char* mr_translate_file_user(unsigned int _file);

// Hardware dependent read external data
void mr_read_file_hd(unsigned int _file, unsigned int _offset, unsigned char* _dest, unsigned int _size);

unsigned char mr_joy_hd(unsigned char _number);

// Hardware dependent tileset copy
void mr_tileset_copy_hd(unsigned char _source, unsigned char _dest);

// Hardware dependent tileset converted multicolor > monocolor
void mr_tileset_multicolor_to_monocolor_hd(unsigned char _source, unsigned char _starting, unsigned char _count);

// Hardware dependent tile redefinition
void mr_tile_redefine_hd(unsigned char _tileset, unsigned char _tile, unsigned char* _data);

// Hardware dependent tile redefinition with filling
void mr_tile_redefine_fill_hd(unsigned char _tileset, unsigned char _tile, unsigned char _data);

// Hardware dependent tile preparation
void mr_tile_prepare_horizontal_monocolor_hd(unsigned char _tileset, unsigned char _source, unsigned char _destination);

// Hardware dependent tile preparation
void mr_tile_prepare_horizontal_extended_monocolor_hd(unsigned char _tileset, unsigned char _source, unsigned char _w, unsigned char _h, unsigned char _destination);

// Hardware dependent tile preparation
void mr_tile_prepare_vertical_hd(unsigned char _tileset, unsigned char _source, unsigned char _destination);

// Hardware dependent tile preparation
void mr_tile_prepare_vertical_extended_hd(unsigned char _tileset, unsigned char _source, unsigned char _w, unsigned char _h, unsigned char _destination);

// Hardware dependent tile preparation
void mr_tile_prepare_roll_horizontal_monocolor_hd(unsigned char _tileset, unsigned char _source, unsigned char _destination);

// Hardware dependent tile preparation
void mr_tile_roll_horizontal_hd(unsigned char _tileset, unsigned char _destination, unsigned char _direction);

// Hardware dependent tile preparation
void mr_tile_roll_horizontal_on_place_hd(unsigned char _tileset, unsigned char _destination, unsigned char _direction, unsigned char _place, unsigned char* _index);

// Hardware dependent tile preparation
void mr_tile_prepare_roll_vertical_hd(unsigned char _tileset, unsigned char _source, unsigned char _destination);

// Hardware dependent tile preparation
void mr_tile_roll_vertical_hd(unsigned char _tileset, unsigned char _destination, mr_direction _direction);

// Hardware dependent loading tileset
void mr_tileset_load_file_hd(unsigned char _index, unsigned char _tileset, unsigned char _starting, unsigned char _count);

#define mr_assert_max_tiles( x ) if ( x > MR_TILESET_TILE_COUNT ) { puts("Maximum number of tiles for tileset exceeded."); return; }

#endif