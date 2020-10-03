/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

#ifndef _MIDRES_HW_VIC20_32K_H_
#define _MIDRES_HW_VIC20_32K_H_

/******************************************************************
 * ------ VIC 20 PLATFORM (with 32 KB of RAM)
 ******************************************************************/

// The maximum resolution is 44 x 46 pixels, equivalent to 
// 22 x 23 mixels.
#define MR_SCREEN_WIDTH					22
#define MR_SCREEN_HEIGHT				23
#define MR_SCREEN_RAM_SIZE				0x0200

// List of available screens:
#define MR_SCREEN_0						8
#define MR_SCREEN_1						9
#define MR_SCREEN_2						10
#define MR_SCREEN_3						11
#define MR_SCREEN_4						12
#define MR_SCREEN_5						13
#define MR_SCREEN_6						14
#define MR_SCREEN_7						15
#define MR_SCREEN_COUNT					8
#define MR_SCREEN_DEFAULT				MR_SCREEN_0

// The four screens used for double buffering
#define	MR_SCREEN_DB1					MR_SCREEN_0
#define	MR_SCREEN_DB2					MR_SCREEN_1

// List of available tilesets:
#define MR_TILESET_0					5
#define MR_TILESET_1					7
#define MR_TILESET_ROM_0				32
#define MR_TILESET_ROM_1				33
#define MR_TILESET_ROM_2				34
#define MR_TILESET_ROM_3				35
#define MR_TILESET_COUNT				6
#define MR_TILESET_DEFAULT				MR_TILESET_ROM_2
#define MR_TILESET_TILE_COUNT			256

// List of available bit blits:
#define MR_AUX_0						9
#define MR_AUX_1						10
#define MR_AUX_2						11
#define MR_AUX_3						12
#define MR_AUX_4						13
#define MR_AUX_5						14
#define MR_AUX_6						15
#define MR_AUX_COUNT					7
#define MR_AUX_DEFAULT					MR_AUX_0

// Offset for correct brightness.
#define MR_COLOR_BRIGHTNESS				0

// Default palette.
#define MR_COLOR_BLACK					0
#define MR_COLOR_WHITE					1
#define MR_COLOR_RED					2
#define MR_COLOR_CYAN					3
#define MR_COLOR_VIOLET					4
#define MR_COLOR_GREEN					5
#define MR_COLOR_BLUE					6
#define MR_COLOR_YELLOW					7	
#define MR_COLOR_ORANGE					MR_COLOR_YELLOW
#define MR_COLOR_BROWN					MR_COLOR_RED
#define MR_COLOR_LIGHT_RED				MR_COLOR_RED
#define MR_COLOR_DARK_GREY				MR_COLOR_BLUE
#define MR_COLOR_GREY					MR_COLOR_CYAN
#define MR_COLOR_LIGHT_GREEN			MR_COLOR_GREEN
#define MR_COLOR_LIGHT_BLUE		 		MR_COLOR_BLUE
#define MR_COLOR_LIGHT_GREY				MR_COLOR_WHITE

#define MR_COLOR_DARK_BLUE				6
#define MR_COLOR_MAGENTA				MR_COLOR_RED
#define MR_COLOR_PURPLE					MR_COLOR_VIOLET
#define MR_COLOR_LAVENDER       		MR_COLOR_VIOLET
#define MR_COLOR_GOLD       			MR_COLOR_YELLOW
#define MR_COLOR_TURQUOISE       		MR_COLOR_LIGHT_BLUE
#define MR_COLOR_TAN       		        MR_COLOR_BROWN
#define MR_COLOR_YELLOW_GREEN       	MR_COLOR_YELLOW
#define MR_COLOR_OLIVE_GREEN       		MR_COLOR_GREEN
#define MR_COLOR_PINK       			MR_COLOR_LIGHT_RED
#define MR_COLOR_PEACH       			MR_COLOR_PINK

#define MR_NEXT_COLOR( c )				( ( ( ( c & 0x07 ) + 1 ) & 0x07 ) )

#define MR_COLOR_COUNT					16

#define MR_TILE_WIDTH_IN_PIXEL			(8>>(MULTICOLOR))
#define MR_TILE_WIDTH_FACTOR			(3-(MULTICOLOR))
#define MR_TILE_BACKGROUND				3
#define MR_TILE_COLOR0					0
#define MR_TILE_COLOR1					1
#define MR_TILE_COLOR2					2

#define SM(_screen)						((mr_mixel*)(0x0200*(_screen)))
#define CM(_screen)						((mr_mixel*)(0x9400))
#define TM(_tileset)					((mr_mixel*)( _tileset > 16 ? (0x8000 + ((_tileset)-16) * 0x0400) : ((_tileset) * 0x400) ) )
#define AM(_bitblit)					((mr_mixel*)(0x0200*(_bitblit)))

// #define MIDRES_STANDALONE_BITBLIT		1
#define MIDRES_STANDALONE_DRAWING			1
#define MIDRES_STANDALONE_DRAWING2			1
#define MIDRES_STANDALONE_SCREEN			1
// #define MIDRES_STANDALONE_SCREEN2		1
#define MIDRES_STANDALONE_TILE				1
#define MIDRES_STANDALONE_TILE_MULTICOLOR	1
#define MIDRES_STANDALONE_KEYBOARD			1

#endif