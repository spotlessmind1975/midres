/*****************************************************************************
 * MIDRES Library - an isomorphic gamelib for retrocomputers                 *
 *****************************************************************************
 * Copyright 2020 Marco Spedaletti (asimov@mclink.it)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *----------------------------------------------------------------------------
 * Concesso in licenza secondo i termini della Licenza Apache, versione 2.0
 * (la "Licenza"); è proibito usare questo file se non in conformità alla
 * Licenza. Una copia della Licenza è disponibile all'indirizzo:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Se non richiesto dalla legislazione vigente o concordato per iscritto,
 * il software distribuito nei termini della Licenza è distribuito
 * "COSÌ COM'È", SENZA GARANZIE O CONDIZIONI DI ALCUN TIPO, esplicite o
 * implicite. Consultare la Licenza per il testo specifico che regola le
 * autorizzazioni e le limitazioni previste dalla medesima.
 ****************************************************************************/

#ifndef _MIDRES_HW_VDP_H_
#define _MIDRES_HW_VDP_H_

#include <stdint.h>

#define __VDP__			1

#define VDP_R0          0x80
#define VDP_R1          0x81
#define VDP_RNAME       0x82
#define VDP_RCOLORTABLE 0x83
#define VDP_RPATTERN    0x84
#define VDP_RSPRITEA    0x85
#define VDP_RSPRITEP    0x86
#define VDP_RCOLOR      0x87

extern void mr_vdp_port(uint8_t port);
extern void mr_vdp_port_read(uint8_t port);
extern void mr_vdp_port_write(uint8_t port);
extern void mr_vdp_data_port(uint8_t port);
extern void mr_vdp_data_port_read(uint8_t port);
extern void mr_vdp_data_port_write(uint8_t port);
extern void mr_vdp_control_port(uint8_t port);
extern void mr_vdp_control_port_read(uint8_t port);
extern void mr_vdp_control_port_write(uint8_t port);
extern void mr_vdp_out(uint8_t reg, uint8_t val);
extern uint8_t mr_vdp_in();
extern void mr_vdp_put(uint8_t* _src, uint16_t _dst, uint16_t _count);
extern void mr_vdp_put8(uint8_t* _src, uint16_t _dst, uint16_t _count);
extern void mr_vdp_fill8(uint8_t _value, uint16_t _dst, uint8_t _count);
extern void mr_vdp_fill(uint8_t _value, uint16_t _dst, uint16_t _count);
extern uint8_t mr_vdp_get(uint16_t _src);

void mr_init_vdp_hd();
void mr_init_multicolor_vdp_hd();

#define MR_RENDERED_MIXELS MR_RENDERED_MIXELS_VDP

#define MR_SCREEN_WIDTH				32
#define MR_SCREEN_ROW_WIDTH			32
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
		        _colormap[(_tile>>3)] = _colormap[(_tile>>3)]|((MR_COLOR_WHITE<<4));

#define MR_PROTECTED_ACCESS_VRAM( f ) \
		{ \
			f; \
		}

#define MR_WRITE_VRAM(x,v)          MR_WRITE_VRAM_MM(x, v)
#define MR_READ_VRAM(x)				MR_READ_VRAM_MM(x)

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

#define MR_PROTECTED_ACCESS_VRAM( f ) \
		{ \
			f; \
		}

#define MR_WRITE_VRAM(x,v)          MR_WRITE_VRAM_MM(x, v)
#define MR_READ_VRAM(x)				MR_READ_VRAM_MM(x)

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

#define MR_READ_TILE(_screen, _offset) mr_vdp_get(_screen + _offset)
#define MR_READ_TILE_COLOR(_colormap, _offset) mr_vdp_get(_colormap + _offset)

#define MR_PROTECTED_ACCESS_VRAM( f ) \
		{ \
			f; \
		}

#define MR_WRITE_VRAM(x,v)          mr_vdp_fill8(v, x, 1)
#define MR_READ_VRAM(x)				mr_vdp_get(x)

#else
#define MR_SM(_screen)					((unsigned int)(0x400*_screen))
#define MR_CM(_screen)					((unsigned int)((_screen==0x0e)?0x2000:0x0000))
#define MR_AM(_screen)					((unsigned int)(0x400*_screen))
#define MR_TM(_tileset)					((unsigned int)((_tileset==0x00)?0x0000:0x2000))

#define MR_WRITE_TILE_LUMINANCE(_screen, _offset, _tile) \
                    mr_vdp_fill8(_tile, _screen + _offset, 1 );

#define MR_WRITE_TILE_AUX(_colormap, _color) \
                    { \
                        unsigned char iMWT, oMWT = (_color & 0x0f); \
                        for(iMWT=0; iMWT<8;++iMWT) { \
                            mr_vdp_fill8(oMWT, _colormap+iMWT, 1 ); \
                        } \
                    }

#define MR_WRITE_TILE(_screen, _colormap, _offset, _tile, _color) \
                    { \
                        int cMWT = _colormap + (_tile * 8); \
                        int sMWT = _screen + _offset; \
                        unsigned char oMWT = (_color & 0x0f); \
                        MR_WRITE_TILE_AUX(cMWT, (_color)); \
                        MR_WRITE_TILE_AUX(0x800 + cMWT, (_color)); \
                        MR_WRITE_TILE_AUX(0x1000 + cMWT, (_color)); \
                    }

#define MR_READ_TILE(_screen, _offset) mr_vdp_get(_screen+_offset)
#define MR_READ_TILE_COLOR(_colormap, _offset) mr_vdp_get(_colormap+_offset)

#define MR_PROTECTED_ACCESS_VRAM( f ) \
		{ \
			f; \
		}

#define MR_WRITE_VRAM(x,v)          mr_vdp_fill8(v, x, 1)
#define MR_READ_VRAM(x)				mr_vdp_get(x)

#endif

#endif

#endif