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
#define MR_TILESET_ROM_0				34
#define MR_TILESET_ROM_1				35
#define MR_TILESET_ROM_2				36
#define MR_TILESET_ROM_3				37
#define MR_TILESET_0					2
#define MR_TILESET_1					3
#define MR_TILESET_2					4
#define MR_TILESET_3					5
#define MR_TILESET_4					6
#define MR_TILESET_7					7
#define MR_TILESET_8					8
#define MR_TILESET_9					9
#define MR_TILESET_A					10
#define MR_TILESET_B					11
#define MR_TILESET_C					12
#define MR_TILESET_D					13
#define MR_TILESET_E					14
#define MR_TILESET_F					15
#define MR_TILESET_COUNT				20
#define MR_TILESET_DEFAULT				MR_TILESET_ROM_1

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
#define MR_COLOR_BRIGHTNESS				6<<4

// Default palette.
#define MR_COLOR_BLACK					0
#define MR_COLOR_WHITE					1
#define MR_COLOR_RED					2
#define MR_COLOR_CYAN					3
#define MR_COLOR_PURPLE					4
#define MR_COLOR_GREEN					5
#define MR_COLOR_BLUE					6
#define MR_COLOR_YELLOW					7
#define MR_COLOR_ORANGE					8
#define MR_COLOR_BROWN					9
#define MR_COLOR_YELLOW_GREEN			10
#define MR_COLOR_PINK					11
#define MR_COLOR_BLUE_GREEN				12
#define MR_COLOR_LIGHT_BLUE				13
#define MR_COLOR_DARK_BLUE				14
#define MR_COLOR_LIGHT_GREEN			15

#define SM(_screen)						((mr_mixel*)((0x0800*(_screen))+0x0400))
#define CM(_screen)						((mr_color*)((0x0800*(_screen))))
#define AM(_screen)						((mr_mixel*)((0x0800*(_screen))))
#define TM(_tileset)					((mr_mixel*)((0x0400*(_tileset))))

#define MIDRES_STANDALONE_BITBLIT		1
#define MIDRES_STANDALONE_DRAWING		1
#define MIDRES_STANDALONE_DRAWING2		1
#define MIDRES_STANDALONE_SCREEN		1
#define MIDRES_STANDALONE_SCREEN2		1
#define MIDRES_STANDALONE_TILE			1

#endif