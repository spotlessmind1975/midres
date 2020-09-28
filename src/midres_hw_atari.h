/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

#ifndef _MIDRES_HW_ATARI_H_
#define _MIDRES_HW_ATARI_H_

#define RENDERED_MIXELS RENDERED_MIXELS_ATARI

 /******************************************************************
  * ------ COMMODORE 64 PLATFORM
  ******************************************************************/

  // The maximum resolution is 80 x 50 pixels, equivalent to 
  // 40 x 25 mixels.
#define MR_SCREEN_WIDTH				40
#define MR_SCREEN_HEIGHT			24
#define MR_SCREEN_RAM_SIZE				0x0400

// List of available screens:
#define MR_SCREEN_0					0
#define MR_SCREEN_1					1
#define MR_SCREEN_2					2
#define MR_SCREEN_3					3
#define MR_SCREEN_COUNT				4
#define MR_SCREEN_DEFAULT			MR_SCREEN_0

// The four screens used for double buffering
#define	MR_SCREEN_DB1				MR_SCREEN_0
#define	MR_SCREEN_DB2				MR_SCREEN_1

// List of available tilesets:
#define MR_TILESET_ROM_0				56
#define MR_TILESET_0					46
#define MR_TILESET_1					48
#define MR_TILESET_2					50
#define MR_TILESET_COUNT				4
#define MR_TILESET_DEFAULT				MR_TILESET_ROM_0

// List of available areas for bit blits:
#define MR_AUX_0						1
#define MR_AUX_1						2
#define MR_AUX_COUNT					2
#define MR_AUX_DEFAULT					MR_AUX_0

// Offset for correct brightness.
#define MR_COLOR_LOWBRIGHTNESS			0
#define MR_COLOR_BRIGHTNESS				0
#define MR_COLOR_HIGHBRIGHTNESS			0

// Default palette.
#define MR_COLOR_BLACK					0
#define MR_COLOR_WHITE					14
#define MR_COLOR_RED					66
#define MR_COLOR_VIOLET					96
#define MR_COLOR_GREEN					196
#define MR_COLOR_BLUE					116
#define MR_COLOR_YELLOW					24
#define MR_COLOR_ORANGE					54
#define MR_COLOR_BROWN					34
#define MR_COLOR_LIGHT_RED				10
#define MR_COLOR_DARK_GREY				4
#define MR_COLOR_GREY					6
#define MR_COLOR_LIGHT_GREEN			200
#define MR_COLOR_LIGHT_BLUE				120
#define MR_COLOR_LIGHT_GREY				15
#define MR_COLOR_MAGENTA				82
#define MR_COLOR_PURPLE					MR_COLOR_VIOLET
#define MR_COLOR_LAVENDER       		102
#define MR_COLOR_GOLD       			20
#define MR_COLOR_TURQUOISE       		164
#define MR_COLOR_TAN       		        36
#define MR_COLOR_YELLOW_GREEN       	214
#define MR_COLOR_OLIVE_GREEN       		228
#define MR_COLOR_PINK       			72
#define MR_COLOR_PEACH       			246
#define MR_COLOR_CYAN					MR_COLOR_TURQUOISE

#define MR_COLOR_DARK_BLUE				6

#define MR_NEXT_COLOR( c )				( ( ( ( c & 0x0f ) + 1 ) & 0x0f ) )

#define MR_COLOR_COUNT					16

#define MR_KEY_UP						9
#define MR_KEY_DOWN						12

#ifdef __ATARICOL__
#define MR_TILE_WIDTH_IN_PIXEL			4
#define MR_TILE_WIDTH_FACTOR			2
#else
#define MR_TILE_WIDTH_IN_PIXEL			8
#define MR_TILE_WIDTH_FACTOR			3
#endif
#define MR_TILE_BACKGROUND				0
#define MR_TILE_COLOR0					1
#define MR_TILE_COLOR1					2
#define MR_TILE_COLOR2					3

extern mr_mixel* screenRam;

#define SM(_screen)						((mr_mixel*)(screenRam))
#define CM(_screen)						((mr_color*)(screenRam+0x400))
#define AM(_screen)						((mr_mixel*)((0x0400*(_screen))))
#define TM(_tileset)					((mr_mixel*)((0x0400*(_tileset))))

#define MIDRES_STANDALONE_BITBLIT			1	
#define MIDRES_STANDALONE_DRAWING			1
#define MIDRES_STANDALONE_DRAWING2			1
#define MIDRES_STANDALONE_SCREEN			1
#define MIDRES_STANDALONE_SCREEN2			1
#define MIDRES_STANDALONE_TILE				1
#ifdef __ATARICOL__
#define MIDRES_STANDALONE_TILE_MULTICOLOR	1
#endif
#define MIDRES_STANDALONE_KEYBOARD			1

#endif