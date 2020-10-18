/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

#ifndef _MIDRES_HW_C64_H_
#define _MIDRES_HW_C64_H_

 /******************************************************************
  * ------ COMMODORE 64 PLATFORM
  ******************************************************************/

  // The maximum resolution is 80 x 50 pixels, equivalent to 
  // 40 x 25 mixels.
#define MR_SCREEN_WIDTH				40
#define MR_SCREEN_HEIGHT			25
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
#define MR_COLOR_LOWBRIGHTNESS			0
#define MR_COLOR_BRIGHTNESS				0
#define MR_COLOR_HIGHBRIGHTNESS			0

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

#define MR_KEY_NONE						64
#define MR_KEY_0						35
#define MR_KEY_1						56
#define MR_KEY_2						59
#define MR_KEY_3						8
#define MR_KEY_4						11
#define MR_KEY_5						16
#define MR_KEY_6						19
#define MR_KEY_7						24
#define MR_KEY_8						27
#define MR_KEY_9						32
#define MR_KEY_A						10
#define MR_KEY_ASTERISK					49
#define MR_KEY_AT						46
#define MR_KEY_B						28
#define MR_KEY_C						20
#define MR_KEY_CLEAR					51
#define MR_KEY_COLON					45
#define MR_KEY_COMMA					47
#define MR_KEY_COMMODORE				61
#define MR_KEY_CONTROL					58
#define MR_KEY_CRSR_LEFT_RIGHT			2
#define MR_KEY_CRSR_UP_DOWN				7
#define MR_KEY_D						18
#define MR_KEY_DELETE					0
#define MR_KEY_E						14
#define MR_KEY_EQUAL					53
#define MR_KEY_F						21
#define MR_KEY_F1						4
#define MR_KEY_F2						4
#define MR_KEY_F3						5
#define MR_KEY_F4						5
#define MR_KEY_F5						6
#define MR_KEY_F6						6
#define MR_KEY_F7						3
#define MR_KEY_F8						3
#define MR_KEY_G						26
#define MR_KEY_H						29
#define MR_KEY_HOME						51
#define MR_KEY_I						33
#define MR_KEY_INSERT					0
#define MR_KEY_J						34
#define MR_KEY_K						37
#define MR_KEY_L						42
#define MR_KEY_LEFT_ARROW				57
#define MR_KEY_LEFT_SHIFT				15
#define MR_KEY_M						36
#define MR_KEY_MINUS					43
#define MR_KEY_N						39
#define MR_KEY_O						38
#define MR_KEY_P						41
#define MR_KEY_PERIOD					44
#define MR_KEY_PLUS						40
#define MR_KEY_POUND					48
#define MR_KEY_Q						62
#define MR_KEY_R						17
#define MR_KEY_RETURN					1
#define MR_KEY_RIGHT_SHIFT				52
#define MR_KEY_RUNSTOP					63
#define MR_KEY_S						13
#define MR_KEY_SEMICOLON				50
#define MR_KEY_SLASH					55
#define MR_KEY_SPACE					60
#define MR_KEY_T						22
#define MR_KEY_U						30
#define MR_KEY_UP_ARROW					54
#define MR_KEY_V						31
#define MR_KEY_W						9
#define MR_KEY_X						23
#define MR_KEY_Y						25
#define MR_KEY_Z						12

#define MR_TILE_WIDTH_IN_PIXEL			(8>>(MR_MULTICOLOR))
#define MR_TILE_WIDTH_FACTOR			(3-(MR_MULTICOLOR))
#define MR_TILE_BACKGROUND				3
#define MR_TILE_COLOR0					0
#define MR_TILE_COLOR1					1
#define MR_TILE_COLOR2					2

#define SM(_screen)						((mr_mixel*)(0x8000+(0x0400*(_screen))))
#define MR_CM(_screen)						((mr_color*)(0xd800)+(_screen*0))
#define MR_AM(_screen)						((mr_mixel*)(0x8000+(0x0400*(_screen))))
#define TM(_tileset)					((mr_mixel*)(0x8000+(0x0800*(_tileset))))

#define WRITE_TILE(_screen, _colormap, _offset, _tile, _color) \
		_screen[(_offset)] = (_tile); \
		_colormap[(offset)] = (_color);

#define READ_TILE(_screen, _offset) _screen[(_offset)]

#define MIDRES_STANDALONE					1
#define MIDRES_STANDALONE_BITBLIT			1
#define MIDRES_STANDALONE_DRAWING			1
#define MIDRES_STANDALONE_DRAWING2			1
#define MIDRES_STANDALONE_SCREEN			1
#define MIDRES_STANDALONE_SCREEN2			1
#define MIDRES_STANDALONE_TILE				1
#define MIDRES_STANDALONE_TILE_MULTICOLOR	1
#define MIDRES_STANDALONE_KEYBOARD			1
// #define MIDRES_STANDALONE_FILE				1
// #define MIDRES_EMBEDDED_FILES				1

#endif