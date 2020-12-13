/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

#ifndef _MIDRES_HW_ATARI_H_
#define _MIDRES_HW_ATARI_H_

#define MR_RENDERED_MIXELS MR_RENDERED_MIXELS_ATARI

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
#define MR_TILESET_0					42
#define MR_TILESET_1					44
#define MR_TILESET_2					46
#define MR_TILESET_COUNT				4
#define MR_TILESET_DEFAULT				MR_TILESET_ROM_0
#define MR_TILESET_TILE_COUNT			128

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

#define MR_KEY_NONE						255
#define MR_KEY_L						0
#define MR_KEY_J						1
#define MR_KEY_SEMICOMMA				2
#define MR_KEY_K						5
#define MR_KEY_PLUS						6
#define MR_KEY_ASTERISK					7
#define MR_KEY_O						8
#define MR_KEY_P						10
#define MR_KEY_U						11
#define MR_KEY_RETURN					12
#define MR_KEY_I						13
#define MR_KEY_MINUS					14
#define MR_KEY_EQUAL					15
#define MR_KEY_V						16
#define MR_KEY_C						18
#define MR_KEY_B						21
#define MR_KEY_X						22
#define MR_KEY_Z						23
#define MR_KEY_4						24
#define MR_KEY_3						26
#define MR_KEY_6						27
#define MR_KEY_ESC						28
#define MR_KEY_5						29
#define MR_KEY_2						30
#define MR_KEY_1						31
#define MR_KEY_COMMA					32
#define MR_KEY_SPACE					33
#define MR_KEY_PERIOD					31
#define MR_KEY_N						35
#define MR_KEY_M						37
#define MR_KEY_SLASH					38
#define MR_KEY_INV						39
#define MR_KEY_R						40
#define MR_KEY_E						42
#define MR_KEY_Y						43
#define MR_KEY_TAB						44
#define MR_KEY_T						45
#define MR_KEY_W						46
#define MR_KEY_Q						47
#define MR_KEY_9						48
#define MR_KEY_0						50
#define MR_KEY_7						51
#define MR_KEY_DEL						52
#define MR_KEY_8						53
#define MR_KEY_LESS_THAN				54
#define MR_KEY_GREATER_THAN				55
#define MR_KEY_F						56
#define MR_KEY_H						57
#define MR_KEY_D						58
#define MR_KEY_CAP						60
#define MR_KEY_G						61
#define MR_KEY_S						62
#define MR_KEY_A						63

#define MR_JOYSTICK_0					0
#define MR_JOYSTICK_1					1
#define MR_JOYSTICK_2					2
#define MR_JOYSTICK_3					3
#define MR_JOYSTICK_COUNT				4
#define MR_JOYSTICK_UP					1
#define MR_JOYSTICK_DOWN				2
#define MR_JOYSTICK_LEFT				4
#define MR_JOYSTICK_RIGHT				8
#define MR_JOYSTICK_FIRE				16

#define MR_TILE_WIDTH_IN_PIXEL			(8>>(MR_MULTICOLOR))
#define MR_TILE_WIDTH_FACTOR			(3-(MR_MULTICOLOR))

#define MR_TILE_BACKGROUND				0
#define MR_TILE_COLOR0					1
#define MR_TILE_COLOR1					2
#define MR_TILE_COLOR2					3

extern mr_mixel* screenRam;

#define MR_SM(_screen)						((mr_mixel*)(screenRam)+(_screen*0))
#define MR_CM(_screen)						((mr_color*)(screenRam+0x400)+(_screen*0))
#define MR_AM(_screen)						((mr_mixel*)((0x0400*(_screen))))
#define MR_TM(_tileset)					((mr_mixel*)((0x0400*(_tileset))))

#define MR_WRITE_TILE_LUMINANCE(_screen, _offset, _tile) \
		_screen[(_offset)] = (_tile);

#define MR_WRITE_TILE(_screen, _colormap, _offset, _tile, _color) \
		_screen[(_offset)] = (_tile); \
		_colormap[(_offset)] = (_color);

#define MR_READ_TILE(_screen, _offset) _screen[(_offset)]
#define MR_READ_TILE_COLOR(_colormap, _offset) _colormap[(_offset)]

#define MIDRES_STANDALONE					1
#define MIDRES_STANDALONE_BITBLIT			1	
#define MIDRES_STANDALONE_DRAWING			1
#define MIDRES_STANDALONE_DRAWING2			1
#define MIDRES_STANDALONE_SCREEN			1
#define MIDRES_STANDALONE_SCREEN2			1
#define MIDRES_STANDALONE_TILE				1
#define MIDRES_STANDALONE_TILE_PROCESSING	1
#define MIDRES_STANDALONE_TILE_MULTICOLOR	1
#define MIDRES_STANDALONE_KEYBOARD			1
// #define MIDRES_STANDALONE_FILE				1
// #define MIDRES_EMBEDDED_FILES				1

#endif