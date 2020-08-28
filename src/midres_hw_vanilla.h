/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

#ifndef _MIDRES_HW_VANILLA_H_
#define _MIDRES_HW_VANILLA_H_

/******************************************************************
 * ------ UNCLASSIFIED ("VANILLA") PLATFORM
 ******************************************************************/

#define MR_SCREEN_WIDTH					40
#define MR_SCREEN_HEIGHT				25
#define MR_SCREEN_RAM_SIZE				0x0400

 // List of available screens:
#define MR_SCREEN_0						0
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
#define MR_AUX_COUNT					0

#define MR_COLOR_BRIGHTNESS				0

#define	MR_SCREEN_DB1					MR_SCREEN_0
#define	MR_SCREEN_DB2					MR_SCREEN_0

#define MR_COLOR_BLACK					0
#define MR_COLOR_WHITE					0
#define MR_COLOR_RED					0
#define MR_COLOR_CYAN					0
#define MR_COLOR_VIOLET					0
#define MR_COLOR_GREEN					0
#define MR_COLOR_BLUE					0
#define MR_COLOR_YELLOW					0	
#define MR_COLOR_ORANGE					0
#define MR_COLOR_BROWN					0
#define MR_COLOR_LIGHT_RED				0
#define MR_COLOR_DARK_GREY				0
#define MR_COLOR_GREY					0
#define MR_COLOR_LIGHT_GREEN			0
#define MR_COLOR_LIGHT_BLUE				0
#define MR_COLOR_LIGHT_GREY				0	

#define SM(_screen)						((mr_mixel*)((0x0000*(_screen))))
#define CM(_screen)						((mr_color*)((0x0000*(_screen))))
#define AM(_screen)						((mr_mixel*)((0x0000*(_screen))))
#define TM(_tileset)					((mr_mixel*)((0x0000*(_tileset))))

#define MIDRES_STANDALONE_BITBLIT		1
#define MIDRES_STANDALONE_DRAWING		1
#define MIDRES_STANDALONE_DRAWING2		1
#define MIDRES_STANDALONE_SCREEN		1
#define MIDRES_STANDALONE_SCREEN2		1
#define MIDRES_STANDALONE_TILE			1
#define MIDRES_STANDALONE_KEYBOARD		1

#endif