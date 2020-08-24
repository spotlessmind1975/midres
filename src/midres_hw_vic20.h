/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

#ifndef _MIDRES_HW_VIC20_H_
#define _MIDRES_HW_VIC20_H_

/******************************************************************
 * ------ VIC 20 PLATFORM
 ******************************************************************/

// The maximum resolution is 44 x 46 pixels, equivalent to 
// 22 x 23 mixels.
#define MR_SCREEN_WIDTH					22
#define MR_SCREEN_HEIGHT				23
#define MR_SCREEN_RAM_SIZE				0x0200

// List of available screens:
#define MR_SCREEN_0						15
#define MR_SCREEN_COUNT					1
#define MR_SCREEN_DEFAULT				MR_SCREEN_0

// The two screens used for double buffering
#define	MR_SCREEN_DB1					MR_SCREEN_0
#define	MR_SCREEN_DB2					MR_SCREEN_0

// List of available tilesets:
#define MR_TILESET_ROM_0				0
#define MR_TILESET_ROM_1				1
#define MR_TILESET_ROM_2				2
#define MR_TILESET_ROM_3				3
#define MR_TILESET_COUNT				4
#define MR_TILESET_DEFAULT				MR_TILESET_ROM_2

// List of available bitblits:
#define MR_AUX_0						15
#define MR_AUX_COUNT					0
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
#define MR_COLOR_DARK_GREY				MR_COLOR_BLACK
#define MR_COLOR_GREY					MR_COLOR_WHITE
#define MR_COLOR_LIGHT_GREEN			MR_COLOR_GREEN
#define MR_COLOR_LIGHT_BLUE		 		MR_COLOR_BLUE
#define MR_COLOR_LIGHT_GREY				MR_COLOR_WHITE

#define SM(_screen)						((mr_mixel*)(0x1e00))
#define CM(_screen)						((mr_color*)(0x9600))
#define TM(_tileset)					((mr_mixel*)(0x8000 + (_tileset) * 0x0400))
#define AM(_bitblit)					((mr_mixel*)(0x0000))

// #define MIDRES_STANDALONE_BITBLIT		1
// #define MIDRES_STANDALONE_DRAWING		1
// #define MIDRES_STANDALONE_DRAWING2		1
#define MIDRES_STANDALONE_SCREEN		1
// #define MIDRES_STANDALONE_SCREEN2		1

#endif