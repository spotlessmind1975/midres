/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

#ifndef _MIDRES_HW_PLUS4_H_
#define _MIDRES_HW_PLUS4_H_

 /******************************************************************
  * ------ COMMODORE PLUS/4 PLATFORM
  ******************************************************************/

  // The maximum resolution is 80 x 50 pixels, equivalent to 
  // 40 x 25 mixels.
#define MR_SCREEN_WIDTH					40
#define MR_SCREEN_HEIGHT				25
#define MR_SCREEN_RAM_SIZE				0x0400

// List of available screens:
#define MR_SCREEN_0						1
#define MR_SCREEN_1						2
#define MR_SCREEN_2						3
#define MR_SCREEN_3						4
#define MR_SCREEN_4						5
#define MR_SCREEN_5						6
#define MR_SCREEN_6						7
#define MR_SCREEN_COUNT					7
#define MR_SCREEN_DEFAULT				MR_SCREEN_0

// The screens used for double buffering
#define	MR_SCREEN_DB1					MR_SCREEN_0
#define	MR_SCREEN_DB2					MR_SCREEN_1

// List of available tilesets:
#define MR_TILESET_ROM_0				52
#define MR_TILESET_0					6
#define MR_TILESET_1					8
#define MR_TILESET_2					10
#define MR_TILESET_3					12
#define MR_TILESET_4					14
#define MR_TILESET_COUNT				6
#define MR_TILESET_DEFAULT				MR_TILESET_ROM_0

// List of available areas for bit blits:
#define MR_AUX_0						2
#define MR_AUX_1						3
#define MR_AUX_2						4
#define MR_AUX_3						5
#define MR_AUX_4						6
#define MR_AUX_5						7
#define MR_AUX_COUNT					6
#define MR_AUX_DEFAULT					MR_AUX_0

// Offset for correct brightness.
#define MR_COLOR_LOWBRIGHTNESS			(3<<4)
#define MR_COLOR_BRIGHTNESS				(5<<4)
#define MR_COLOR_HIGHBRIGHTNESS			(7<<4)

// Default palette.
#define MR_COLOR_BLACK					(0|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_WHITE					(1|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_RED					(2|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_CYAN					(3|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_PURPLE					(4|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_GREEN					(5|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_BLUE					(6|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_YELLOW					(7|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_ORANGE					(8|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_BROWN					(9|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_YELLOW_GREEN			(10|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_PINK					(11|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_BLUE_GREEN				(12|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_LIGHT_BLUE				(13|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_DARK_BLUE				(14|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_LIGHT_GREEN			(15|MR_COLOR_BRIGHTNESS)

#define MR_COLOR_VIOLET					4
#define MR_COLOR_LIGHT_RED				(2|MR_COLOR_HIGHBRIGHTNESS)
#define MR_COLOR_DARK_GREY				1
#define MR_COLOR_GREY					(1|MR_COLOR_BRIGHTNESS)
#define MR_COLOR_LIGHT_GREY				(1|MR_COLOR_HIGHBRIGHTNESS)
#define MR_COLOR_MAGENTA				MR_COLOR_RED
#define MR_COLOR_LAVENDER       		MR_COLOR_VIOLET
#define MR_COLOR_GOLD       			MR_COLOR_YELLOW
#define MR_COLOR_TURQUOISE       		MR_COLOR_LIGHT_BLUE
#define MR_COLOR_TAN       		        MR_COLOR_BROWN
#define MR_COLOR_OLIVE_GREEN       		MR_COLOR_GREEN
#define MR_COLOR_PEACH       			MR_COLOR_PINK

#define MR_NEXT_COLOR( c )				( c & 0xf0 | ( ( ( c & 0x0f ) + 1 ) & 0x0f ) )

#define MR_COLOR_COUNT					16

#define MR_TILE_BACKGROUND				3
#define MR_TILE_COLOR0					0
#define MR_TILE_COLOR1					1
#define MR_TILE_COLOR2					2

#define SM(_screen)						((mr_mixel*)((0x0800*(_screen))+0x0400))
#define CM(_screen)						((mr_color*)((0x0800*(_screen))))
#define AM(_screen)						((mr_mixel*)((0x0800*(_screen))))
#define TM(_tileset)					((mr_mixel*)((0x0400*(_tileset))))

#define MIDRES_STANDALONE_BITBLIT			1
#define MIDRES_STANDALONE_DRAWING			1
#define MIDRES_STANDALONE_DRAWING2			1
#define MIDRES_STANDALONE_SCREEN			1
#define MIDRES_STANDALONE_SCREEN2			1
#define MIDRES_STANDALONE_TILE				1
#define MIDRES_STANDALONE_TILE_MULTICOLOR	1
#define MIDRES_STANDALONE_KEYBOARD			1

#endif