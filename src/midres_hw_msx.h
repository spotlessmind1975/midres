/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

#ifndef _MIDRES_HW_MSX_H_
#define _MIDRES_HW_MSX_H_

//#define FRAME_BUFFER        1
//#define GRAPHIC_MODE_I      1
//#define GRAPHIC_MODE_II     1

#define MR_RENDERED_MIXELS MR_RENDERED_MIXELS_VDP

// The maximum resolution is 80 x 50 pixels, equivalent to 
// 40 x 25 mixels.
#define MR_SCREEN_WIDTH				32
#define MR_SCREEN_HEIGHT			24
#define MR_SCREEN_RAM_SIZE			768

#ifdef GRAPHIC_MODE_I

// List of available screens:
#define MR_SCREEN_0					5
#define MR_SCREEN_1					6
#define MR_SCREEN_2					7
#define MR_SCREEN_3					8
#define MR_SCREEN_4					9
#define MR_SCREEN_5					10
#define MR_SCREEN_COUNT				6
#define MR_SCREEN_DEFAULT			MR_SCREEN_0

// List of available tilesets:
#define MR_TILESET_ROM_0				1
#define MR_TILESET_0					1
#define MR_TILESET_1					2
#define MR_TILESET_2					6
#define MR_TILESET_3					7
#define MR_TILESET_COUNT				4
#define MR_TILESET_DEFAULT				MR_TILESET_ROM_0
#define MR_TILESET_TILE_COUNT			256

// List of available areas for bit blits:
#define MR_AUX_0						14
#define MR_AUX_1						15
#define MR_AUX_COUNT					1
#define MR_AUX_DEFAULT					MR_AUX_0

#else

#define MR_SCREEN_0					0x0e
#define MR_SCREEN_1					0x0f
#define MR_SCREEN_COUNT				2
#define MR_SCREEN_DEFAULT			MR_SCREEN_0

// List of available tilesets:
#define MR_TILESET_ROM_0				0
#define MR_TILESET_0					0
#define MR_TILESET_COUNT				1
#define MR_TILESET_DEFAULT				MR_TILESET_ROM_0
#define MR_TILESET_TILE_COUNT			256

// List of available areas for bit blits:
#define MR_AUX_0						0x0e
#define MR_AUX_1						0x0f
#define MR_AUX_COUNT					2
#define MR_AUX_DEFAULT					MR_AUX_1

#endif

// The four screens used for double buffering
#define	MR_SCREEN_DB1				MR_SCREEN_0
#define	MR_SCREEN_DB2				MR_SCREEN_1

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
#define MR_KEY_BACKSLASH                128
#define MR_KEY_SQUARE_OPEN              129
#define MR_KEY_SQUARE_CLOSED            130
#define MR_KEY_SEMICOMMA                131
#define MR_KEY_QUOTE                    132
#define MR_KEY_APIX                     133
#define MR_KEY_DEAD                     134
#define MR_KEY_SHIFT                    135
#define MR_KEY_CTRL                     136
#define MR_KEY_GRAPH                    137
#define MR_KEY_CAPS                     138
#define MR_KEY_CODE                     139
#define MR_KEY_ESC                      140
#define MR_KEY_TAB                      141
#define MR_KEY_STOP                     142
#define MR_KEY_BS                       143
#define MR_KEY_SELECT                   144
#define MR_KEY_RET                      145
#define MR_KEY_INS                      146
#define MR_KEY_DEL                      147
#define MR_KEY_LEFT                     148
#define MR_KEY_UP                       149
#define MR_KEY_DOWN                     150
#define MR_KEY_RIGHT                    151
#define MR_KEY_DIVISION                 154

#define MR_JOYSTICK_0					0
#define MR_JOYSTICK_1					1
#define MR_JOYSTICK_COUNT				2
#define MR_JOYSTICK_UP					1
#define MR_JOYSTICK_DOWN				2
#define MR_JOYSTICK_LEFT				4
#define MR_JOYSTICK_RIGHT				8
#define MR_JOYSTICK_FIRE				16
#define MR_JOYSTICK_FIREA				16
#define MR_JOYSTICK_FIREB				32

#define MR_TILE_WIDTH_IN_PIXEL			(8>>(MR_MULTICOLOR))
#define MR_TILE_WIDTH_FACTOR			(3-(MR_MULTICOLOR))

#define MR_TILE_BACKGROUND				0
#define MR_TILE_COLOR0					1
#define MR_TILE_COLOR1					2
#define MR_TILE_COLOR2					3

#ifdef FRAME_BUFFER
    #ifdef GRAPHIC_MODE_I
        #define MR_SM(_screen)					(&frameBuffer[0])
        #define MR_CM(_screen)					(&colorBuffer[0])
        #define MR_AM(_screen)					(&auxBuffer[0])
        #define MR_TM(_tileset)					((unsigned int)(0x800*_tileset))

        #define MR_WRITE_TILE_LUMINANCE(_screen, _offset, _tile) \
		        _screen[(_offset)] = (_tile);

        #define MR_WRITE_TILE(_screen, _colormap, _offset, _tile, _color) \
		        _screen[(_offset)] = (_tile); \
		        _colormap[(_offset)] = (_color);

        #define MR_READ_TILE(_screen, _offset) _screen[(_offset)]
        #define MR_READ_TILE_COLOR(_colormap, _offset) _colormap[(_offset)]

#else

extern unsigned char frameBuffer[];
extern unsigned char colorBuffer[];
extern unsigned char auxBuffer[];

        #define MR_SM(_screen)					(&frameBuffer[0])
        #define MR_CM(_screen)					(&colorBuffer[0])
        #define MR_AM(_screen)					(&auxBuffer[0])
        #define MR_TM(_tileset)					((unsigned int)0x0000)

        #define MR_WRITE_TILE_LUMINANCE(_screen, _offset, _tile) \
		            _screen[(_offset)] = (_tile);

        #define MR_WRITE_TILE(_screen, _colormap, _offset, _tile, _color) \
		            _screen[(_offset)] = (_tile); \
                    _colormap[(_offset)] = (_color<<4);
            

        #define MR_READ_TILE(_screen, _offset) _screen[(_offset)]
        #define MR_READ_TILE_COLOR(_colormap, _offset) _colormap[(_offset)]
#endif
#else
    #ifdef GRAPHIC_MODE_I
        #define MR_SM(_screen)					((unsigned int)(0x400*_screen))
        #define MR_CM(_screen)					((unsigned int)((0x40*(_screen+0x80))))
        #define MR_AM(_screen)					((unsigned int)(0x400*_screen))
        #define MR_TM(_tileset)					((unsigned int)(0x800*_tileset))

        #define MR_WRITE_TILE_LUMINANCE(_screen, _offset, _tile) \
                mr_vdp_fill8(_tile, _screen + _offset, 1 );

        #define MR_WRITE_TILE(_screen, _colormap, _offset, _tile, _color) \
                mr_vdp_fill8(_tile, _screen + _offset, 1 ); \
                mr_vdp_fill8(_color, _colormap + ( _tile >> 3 ), 1 );

        #define MR_READ_TILE(_screen, _offset) mr_vdp_get(_offset)
        #define MR_READ_TILE_COLOR(_colormap, _offset) mr_vdp_get(_offset)

    #else
        #define MR_SM(_screen)					((unsigned int)(0x400*_screen))
        #define MR_CM(_screen)					((unsigned int)((0x2000*(_screen-0x0e))))
        #define MR_AM(_screen)					((unsigned int)(0x400*_screen))
        #define MR_TM(_tileset)					((unsigned int)((_tileset==0x0e)?0x2000:0x0000))

        #define MR_WRITE_TILE_LUMINANCE(_screen, _offset, _tile) \
                    mr_vdp_fill8(_tile, _screen + _offset, 1 );

        #define MR_WRITE_TILE(_screen, _colormap, _offset, _tile, _color) \
                    mr_vdp_fill8(_tile, _screen + _offset, 1 ); \
                    mr_vdp_fill8((_color & 0x0f), _colormap + (_offset * 8), 1 ); \
                    mr_vdp_fill8((_color & 0x0f), _colormap + (_offset * 8) + 1, 1); \
                    mr_vdp_fill8((_color & 0x0f), _colormap + (_offset * 8) + 2, 1); \
                    mr_vdp_fill8((_color & 0x0f), _colormap + (_offset * 8) + 3, 1); \
                    mr_vdp_fill8((_color & 0x0f), _colormap + (_offset * 8) + 4, 1); \
                    mr_vdp_fill8((_color & 0x0f), _colormap + (_offset * 8) + 5, 1); \
                    mr_vdp_fill8((_color & 0x0f), _colormap + (_offset * 8) + 6, 1); \
                    mr_vdp_fill8((_color & 0x0f), _colormap + (_offset * 8) + 7, 1);

        #define MR_READ_TILE(_screen, _offset) mr_vdp_get(_offset)
        #define MR_READ_TILE_COLOR(_colormap, _offset) mr_vdp_get(_offset)
#endif

#endif


#define MIDRES_STANDALONE					1
//#define MIDRES_STANDALONE_BITBLIT			1	
//#define MIDRES_STANDALONE_DRAWING			1
//#define MIDRES_STANDALONE_DRAWING2			1
#define MIDRES_STANDALONE_SCREEN			1
//#define MIDRES_STANDALONE_SCREEN2			1
#define MIDRES_STANDALONE_TILE				1
#define MIDRES_STANDALONE_TILE_PROCESSING	1
//#define MIDRES_STANDALONE_TILE_MULTICOLOR	1
#define MIDRES_STANDALONE_KEYBOARD			1
// #define MIDRES_STANDALONE_FILE				1
#define MIDRES_EMBEDDED_FILES				1

#include "midres_tile_hd.h"

#endif