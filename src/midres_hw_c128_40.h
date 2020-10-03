/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

#ifndef _MIDRES_HW_C128_H_
#define _MIDRES_HW_C128_H_

 /******************************************************************
  * ------ COMMODORE 128 PLATFORM
  ******************************************************************/

  // The maximum resolution is 80 x 50 pixels, equivalent to 
  // 40 x 25 mixels.
#define MR_SCREEN_WIDTH					40
#define MR_SCREEN_HEIGHT				25
#define MR_SCREEN_RAM_SIZE				0x0400

// List of available screens:
#define MR_SCREEN_0					0
#define MR_SCREEN_1					1
#define MR_SCREEN_2					2
#define MR_SCREEN_3					3
#define MR_SCREEN_4					4
#define MR_SCREEN_5					5
#define MR_SCREEN_6					6
#define MR_SCREEN_7					7
#define MR_SCREEN_8	 				8
#define MR_SCREEN_9					9
#define MR_SCREEN_A					10
#define MR_SCREEN_B					11
#define MR_SCREEN_C					12
#define MR_SCREEN_D					13
#define MR_SCREEN_E					14
#define MR_SCREEN_F					15
#define MR_SCREEN_COUNT				16
#define MR_SCREEN_DEFAULT			MR_SCREEN_1

// The four screens used for double buffering
#define	MR_SCREEN_DB1				MR_SCREEN_0
#define	MR_SCREEN_DB2				MR_SCREEN_1

// List of available tilesets:
#define MR_TILESET_ROM_0				2
#define MR_TILESET_ROM_1				3
#define MR_TILESET_0					4
#define MR_TILESET_1					5
#define MR_TILESET_2					6
#define MR_TILESET_3					7
#define MR_TILESET_COUNT				6
#define MR_TILESET_DEFAULT				MR_TILESET_ROM_1
#define MR_TILESET_TILE_COUNT			256

// List of available areas for bit blits:
#define MR_AUX_0						2
#define MR_AUX_1						3
#define MR_AUX_2						4
#define MR_AUX_3						5
#define MR_AUX_4						6
#define MR_AUX_5						7
#define MR_AUX_6						8
#define MR_AUX_7						9
#define MR_AUX_8						10
#define MR_AUX_9						11
#define MR_AUX_A						12
#define MR_AUX_B						13
#define MR_AUX_C						14
#define MR_AUX_D						15
#define MR_AUX_COUNT					14
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
#define MR_COLOR_ORANGE					8
#define MR_COLOR_BROWN					9
#define MR_COLOR_LIGHT_RED				10
#define MR_COLOR_DARK_GREY				11
#define MR_COLOR_GREY					12
#define MR_COLOR_LIGHT_GREEN			13
#define MR_COLOR_LIGHT_BLUE				14
#define MR_COLOR_LIGHT_GREY				15

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

#define MR_NEXT_COLOR( c )				( ( ( ( c & 0x0f ) + 1 ) & 0x0f ) )

#define MR_COLOR_COUNT					16

#define MR_TILE_WIDTH_IN_PIXEL			(8>>(MULTICOLOR))
#define MR_TILE_WIDTH_FACTOR			(3-(MULTICOLOR))
#define MR_TILE_BACKGROUND				3
#define MR_TILE_COLOR0					0
#define MR_TILE_COLOR1					1
#define MR_TILE_COLOR2					2

#define SM(_screen)						((mr_mixel*)(0x8000+(0x0400*(_screen))))
#define CM(_screen)						((mr_color*)(0xd800))
#define AM(_screen)						((mr_mixel*)(0x8000+(0x0400*(_screen))))
#define TM(_tileset)					((mr_mixel*)(0x8000+(0x0800*(_tileset))))

#define MIDRES_STANDALONE_BITBLIT			1
#define MIDRES_STANDALONE_DRAWING			1
#define MIDRES_STANDALONE_DRAWING2			1
#define MIDRES_STANDALONE_SCREEN			1
#define MIDRES_STANDALONE_SCREEN2			1
#define MIDRES_STANDALONE_TILE				1
#define MIDRES_STANDALONE_TILE_MULTICOLOR	1
#define MIDRES_STANDALONE_KEYBOARD			1

#endif