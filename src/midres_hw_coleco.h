/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

#ifndef _MIDRES_HW_COLECO_H_
#define _MIDRES_HW_COLECO_H_

#include <stdint.h>

extern void vdp_out(uint8_t reg, uint8_t val);
extern uint8_t vdp_in();
extern void vdp_put(uint8_t* _src, uint16_t _dst, uint16_t _count);
extern void vdp_fill(uint8_t _value, uint16_t _dst, uint16_t _count);

#define MR_RENDERED_MIXELS MR_RENDERED_MIXELS_COLECO

// The maximum resolution is 80 x 50 pixels, equivalent to 
// 40 x 25 mixels.
#define MR_SCREEN_WIDTH				32
#define MR_SCREEN_HEIGHT			24
#define MR_SCREEN_RAM_SIZE			768

// List of available screens:
#define MR_SCREEN_0					0
#define MR_SCREEN_COUNT				1
#define MR_SCREEN_DEFAULT			MR_SCREEN_0

// The four screens used for double buffering
#define	MR_SCREEN_DB1				MR_SCREEN_0
#define	MR_SCREEN_DB2				MR_SCREEN_0

// List of available tilesets:
#define MR_TILESET_ROM_0				0
#define MR_TILESET_0					0
#define MR_TILESET_COUNT				1
#define MR_TILESET_DEFAULT				MR_TILESET_ROM_0
#define MR_TILESET_TILE_COUNT			256

// List of available areas for bit blits:
#define MR_AUX_0						1
#define MR_AUX_COUNT					1
#define MR_AUX_DEFAULT					MR_AUX_0

// Offset for correct brightness.
#define MR_COLOR_LOWBRIGHTNESS			0
#define MR_COLOR_BRIGHTNESS				0
#define MR_COLOR_HIGHBRIGHTNESS			0

// Default palette.
#define MR_COLOR_BLACK					0x01
#define MR_COLOR_WHITE					0x0f
#define MR_COLOR_RED					0x08
#define MR_COLOR_VIOLET					0x0d
#define MR_COLOR_GREEN					0x02
#define MR_COLOR_BLUE					0x07
#define MR_COLOR_YELLOW					0x0b
#define MR_COLOR_ORANGE					0x09
#define MR_COLOR_BROWN					0x06
#define MR_COLOR_LIGHT_RED				0x09
#define MR_COLOR_DARK_GREY				0x0e
#define MR_COLOR_GREY					0x0e
#define MR_COLOR_LIGHT_GREEN			0x03
#define MR_COLOR_LIGHT_BLUE				0x05
#define MR_COLOR_LIGHT_GREY				0x0e
#define MR_COLOR_MAGENTA				0x0d
#define MR_COLOR_PURPLE					0x0d
#define MR_COLOR_LAVENDER       		0x07
#define MR_COLOR_GOLD       			0x0a
#define MR_COLOR_TURQUOISE       		0x07
#define MR_COLOR_TAN       		        0x07
#define MR_COLOR_YELLOW_GREEN       	0x03
#define MR_COLOR_OLIVE_GREEN       		0x0c
#define MR_COLOR_PINK       			0x09
#define MR_COLOR_PEACH       			0x09
#define MR_COLOR_CYAN					0x07
#define MR_COLOR_DARK_BLUE				0x04

#define MR_NEXT_COLOR( c )				( ( ( ( c & 0x0f ) + 1 ) & 0x0f ) )

#define MR_COLOR_COUNT					16

#define MR_TILE_WIDTH_IN_PIXEL			(8>>(MR_MULTICOLOR))
#define MR_TILE_WIDTH_FACTOR			(3-(MR_MULTICOLOR))

#define MR_TILE_BACKGROUND				0
#define MR_TILE_COLOR0					1
#define MR_TILE_COLOR1					2
#define MR_TILE_COLOR2					3

#define MR_SM(_screen)					0x1400
#define MR_CM(_screen)					0x2000
#define MR_AM(_screen)					0x2200
#define MR_TM(_tileset)					0x0800

#define MR_WRITE_TILE(_screen, _colormap, _offset, _tile, _color) \
        vdp_fill(_tile, _screen + _offset, 1 );

#define MR_READ_TILE(_screen, _offset) ;

#define MIDRES_STANDALONE					1
//#define MIDRES_STANDALONE_BITBLIT			1	
//#define MIDRES_STANDALONE_DRAWING			1
//#define MIDRES_STANDALONE_DRAWING2			1
//#define MIDRES_STANDALONE_SCREEN			1
//#define MIDRES_STANDALONE_SCREEN2			1
//#define MIDRES_STANDALONE_TILE				1
//#define MIDRES_STANDALONE_TILE_PROCESSING	1
//#define MIDRES_STANDALONE_TILE_MULTICOLOR	1
//#define MIDRES_STANDALONE_KEYBOARD			1
// #define MIDRES_STANDALONE_FILE				1
// #define MIDRES_EMBEDDED_FILES				1


#endif