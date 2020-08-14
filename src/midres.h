/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

#ifndef _MIDRES_H_
#define _MIDRES_H_

	/************************************************************************
	 * ------ DATA TYPES 
	 ************************************************************************/

	// A "mixel" is a group of 1-bit luminance pixels arranged in a "square" 
	// of 2x2 pixels:
	//
	//  +---+---+
	//  | a | b |
	//  +---+---+
	//  | c | d |
	//  +---+---+
	//
	// A mixel needs just 4 bits (one nibble) to be stored, as follows:
	// |abcd|. So, a single byte can store upto 2 consecutive mixels: 
	// |abcd|abcd| = |hi|lo|, and we call it pmixel (packed mixel).

	typedef unsigned char mr_mixelbits;
	typedef unsigned char mr_mixel;
	typedef unsigned char mr_pmixel;

	// You can convert mixel to pmixel and viceversa through two macro:
	// - mr_pack_mixels: to convert two mixels to a pmixel
	// - mr_unpack_pmixel_hi: to convert a pmixel to a mixel (the high one)
	// - mr_unpack_pmixel_lo: to convert a pmixel to a mixel (the low one)

	#define mr_pack_mixels( hi, lo ) ( hi << 4 | lo )
	#define mr_unpack_pmixel_hi( pm ) ( ( pm & 0xf0 ) >> 4 )
	#define mr_unpack_pmixel_lo( pm ) ( ( pm & 0x0f ) )

	// Graphical primitive can operate on pixels of mixels by using specific
	// pixel operations:
	// - mr_pixel_off: to turn off a pixel
	// - mr_pixel_on: to turn on a pixel
	// - mr_pixel_invert: to invert the value of a pixel

	#define mr_pixel_off		0x00
	#define mr_pixel_on			0x01
	#define mr_pixel_invert		0x02

	// Define the bitmask for the given pixel position

	#define mr_mixel_bits(_x,_y) RENDERED_MIXEL_BITS[((_x&1)<<1)|(_y&1)]

	typedef unsigned char mr_pop;

	// The position of the pixel on the screen is represented by a specific 
	// data type. Since the maximum resolution is 80 x 50, the type used is 
	// the one that represents these two numbers with the lowest numer of bits.

	typedef unsigned char mr_position;

	// The library manages a certain number of screens, and it is possible to
	// indicate them with this type of data.

	typedef unsigned char mr_screen;

	// Two screens can be "packed" into one: you can tell which one are you
	// interested, in order to pack/unpack.

	#define mr_half_up			0x00
	#define mr_half_down		0x01

	typedef unsigned char mr_half_screen;

	// This type of data allows you to manage one of the possible colors of 
	// the palette. The palette is normalized between the various systems, 
	// therefore it is necessary to use the constants provided (MR_COL_xxx).

	typedef unsigned char mr_color;

	// We include some hardware-dependent data type and constants.
	#include "midres_hw.h"

	/************************************************************************
	 * ------ ACCESSIBLE DATA
	 ************************************************************************/

	 // Set of colors assigned for each screen.
	extern mr_color SCREEN_INK[MAX_SCREENS];

	// Screen visible to the user.
	extern mr_screen VISIBLE_SCREEN;

	// Screen selected for graphic output (for primitives that do not plan 
	// to give the screen as parameter).
	extern mr_screen ENABLED_SCREEN;

	// Precalculated values for positions.
	extern unsigned char RENDERED_MIXEL_BITS[4];

	// Precalculated values for mixels.
	extern unsigned char RENDERED_MIXELS[16];

	// Screen width
	extern mr_position WIDTH;

	// Screen height
	extern mr_position HEIGHT;

	// Offset brightness for colors
	extern mr_color BRIGHTNESS;

	/************************************************************************
	 * ------ FUNCTIONS
	 ************************************************************************/

	 /*-----------------------------------------------------------------------
	  --- OVERLAY SUPPORT
	  -----------------------------------------------------------------------*/

	// To be called before using screen load/uncompress/unpack functions.
	#ifdef __OVERLAY__MIDRES__
		#define mr_use_screen() load_overlay("mr1", _OVERLAY1_LOAD__, _OVERLAY1_SIZE__)
	#else
		#define mr_use_screen() 
	#endif

	// To be called before using drawing functions.
	#ifdef __OVERLAY__MIDRES__
		#define mr_use_drawing() load_overlay("mr2", _OVERLAY2_LOAD__, _OVERLAY2_SIZE__)
	#else
		#define mr_use_drawing() 
	#endif
	
	// To be called before using screen save/compress/pack functions.
	#ifdef __OVERLAY__MIDRES__
		#define mr_use_screen2() load_overlay("mr3", _OVERLAY3_LOAD__, _OVERLAY3_SIZE__)
	#else
		#define mr_use_screen2() 
	#endif

	// To be called before using drawing functions.
	#ifdef __OVERLAY__MIDRES__
		#define mr_use_drawing2() load_overlay("mr4", _OVERLAY4_LOAD__, _OVERLAY4_SIZE__)
	#else
		#define mr_use_drawing2() 
	#endif

/*-----------------------------------------------------------------------
	 --- LIBRARY MANAGEMENT
	 -----------------------------------------------------------------------*/

	// Initialize the graphics subsystem.
	void mr_init();

	// Release the graphics subsystem.
	void mr_cleanup();

	/*-----------------------------------------------------------------------
	 --- SCREEN MANAGEMENT
	 -----------------------------------------------------------------------*/

	// Clear the given screen.
	void mr_clear(mr_screen _screen);

	// Show and enable the given screen for graphical operations.
	void mr_show(mr_screen _screen);

	// Enable the screen for graphical operations.
	void mr_enable(mr_screen _screen);

	// Load a (luminance) screen from a file to a specific screen
	unsigned char mr_load(char *_filename, mr_screen _screen);

	// Save a specific (luminance) screen to a file
	unsigned char mr_save(char* _filename, mr_screen _screen);

	// Load a (color) screen from a file to a specific screen
	unsigned char mr_load_color(char* _filename, mr_screen _screen);

	// Save a specific (color) screen to a file
	unsigned char mr_save_color(char* _filename, mr_screen _screen);

	// Compress luminance and color in a single mixel screen
	void mr_compress(mr_screen _source, mr_screen _destination);

	// Uncompress luminance and color from a single mixel screen
	void mr_uncompress(mr_screen _source, mr_screen _destination);

	// Pack luminance in an half screen
	void mr_pack(mr_screen _source, mr_screen _destination, mr_half_screen _half_screen);

	// Unpack luminance in an half screen
	void mr_unpack(mr_screen _source, mr_screen _destination, mr_half_screen _half_screen);

	/*-----------------------------------------------------------------------
	 --- DRAWING PRIMITIVES [v1.0]
	 -----------------------------------------------------------------------*/

	 // Draw a pixel on the given screen, with the given logical operation.
	void mr_psetop(mr_mixel* _screen, mr_position _x, mr_position _y, mr_pop _pop);

	// Color a pixel on the given screen.
	void mr_pcolorop(mr_color* _screen, mr_position _x, mr_position _y, mr_color _color);

	// Set the default color for subsequent operations.
	void mr_ink(mr_screen _screen, mr_color _color);

	/*-----------------------------------------------------------------------
	 --- DRAWING DERIVATED (EXPLICIT SCREEN) [v1.0]
	 -----------------------------------------------------------------------*/

	 // Draw a pixel on the given screen
	#define mr_pset(_screen, _x, _y) mr_psetop((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * _screen), _x, _y, mr_pixel_on);

	// Clear a pixel on the given screen
	#define mr_pclear(_screen, _x, _y) mr_psetop((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * _screen), _x, _y, mr_pixel_off);

	// Invert a pixel on the given screen
	#define mr_pinvert(_screen, _x, _y) mr_psetop((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * _screen), _x, _y, mr_pixel_invert);

	// Color a pixel on the given screen
	#define mr_pcolor(_screen, _x, _y, _color) (mr_screen)_screen; mr_pcolorop((mr_color*)COLOUR_RAM_START_ADDRESS, _x, _y, _color);

	// Draw and color a pixel on the given screen
	#define mr_psetc(_screen, _x, _y) mr_pcolorop((mr_color*)COLOUR_RAM_START_ADDRESS, _x, _y, SCREEN_INK[_screen]); mr_psetop((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * _screen), _x, _y, mr_pixel_on);

	/*-----------------------------------------------------------------------
	 --- DRAWING DERIVATED (IMPLICIT SCREEN [VISIBLE]) [v1.0]
	 -----------------------------------------------------------------------*/

	 // Draw a pixel on the visible screen
	#define mr_psetv(_x, _y) mr_psetop((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * VISIBLE_SCREEN), _x, _y, mr_pixel_on);

	// Clear a pixel on the visible screen
	#define mr_pclearv(_x, _y) mr_psetop((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * VISIBLE_SCREEN), _x, _y, mr_pixel_off);

	// Invert a pixel on the visible screen
	#define mr_pinvertv(_x, _y) mr_psetop((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * VISIBLE_SCREEN), _x, _y, mr_pixel_invert);

	// Color a pixel on the visible screen
	#define mr_pcolorv(_x, _y, _color) mr_pcolorop((mr_color*)COLOUR_RAM_START_ADDRESS, _x, _y, _color);

	// Draw and color a pixel on the visible screen
	#define mr_psetcv(_x, _y) mr_pcolorop((mr_color*)COLOUR_RAM_START_ADDRESS, _x, _y, SCREEN_INK[VISIBLE_SCREEN]); mr_psetop((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * VISIBLE_SCREEN), _x, _y, mr_pixel_on);

	/*-----------------------------------------------------------------------
	 --- DRAWING DERIVATED (IMPLICIT SCREEN [ENABLED]) [v1.0]
	 -----------------------------------------------------------------------*/

	 // Draw a pixel on the enabled screen
	#define mr_psete(_x, _y) mr_psetop((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * ENABLED_SCREEN), _x, _y, mr_pixel_on);

	// Clear a pixel on the enabled screen
	#define mr_pcleare(_x, _y) { mr_psetop((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * ENABLED_SCREEN), _x, _y, mr_pixel_off);

	// Invert a pixel on the enabled screen
	#define mr_pinverte(_x, _y) { mr_psetop((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * ENABLED_SCREEN), _x, _y, mr_pixel_invert);

	// Color a pixel on the enabled screen
	#define mr_pcolore(_x, _y, _color) { mr_pcolorop((mr_color*)COLOUR_RAM_START_ADDRESS, _x, _y, _color);
	
	// Draw and color a pixel on the enabled screen
	#define mr_psetce(_x, _y) { mr_pcolorop((mr_color*)COLOUR_RAM_START_ADDRESS, _x, _y, SCREEN_INK[ENABLED_SCREEN]); mr_psetop((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * ENABLED_SCREEN), _x, _y, mr_pixel_on);

	// Draw a box on the enabled screen, with the given logical operation.
	#define mr_boxe(_screen, _x0, _y0, _x1, _y1) mr_boxop((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * ENABLED_SCREEN), _x0, _y0, _x1, _y1);

	/*-----------------------------------------------------------------------
	 --- DRAWING PRIMITIVES [v1.1]
	 -----------------------------------------------------------------------*/

	// Clears the bitmap to color BLACK.
	void _mr_clear_bitmap(mr_mixel * _screen, mr_color * _colormap);

	// Clears the bitmap to the specified color.
	void _mr_clear_to_color(mr_mixel* _screen, mr_color *_colormap, mr_color _color);

	// Writes a pixel into a bitmap.
	void _mr_putpixel(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_color _color);

	// Reads a pixel from a bitmap.
	mr_color _mr_getpixel(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y);

	// Draws a vertical line onto the bitmap.
	void _mr_vline(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y1, mr_position _y2, mr_color _color );

	// Draws a horizontal line onto the bitmap.
	void _mr_hline(mr_mixel* _screen, mr_color* _colormap, mr_position _x1, mr_position _x2, mr_position _y, mr_color _color);

	// Draws a line onto the bitmap.
	void _mr_line(mr_mixel* _screen, mr_color* _colormap, mr_position _x1, mr_position _y1, mr_position _x2, mr_position _y2, mr_color _color);

	// Draws a triangle.
	void _mr_triangle(mr_mixel* _screen, mr_color* _colormap, mr_position _x1, mr_position _y1, mr_position _x2, mr_position _y2, mr_position _x3, mr_position _y3, mr_color _color);

	// Draws a polygon.
	void _mr_polygon(mr_mixel* _screen, mr_color* _colormap, mr_position _vertices, mr_position _points[], mr_color _color);

	// Draws an outline rectangle.
	void _mr_rect(mr_mixel* _screen, mr_color* _colormap, mr_position _x1, mr_position _y1, mr_position _x2, mr_position _y2, mr_color _color);

	// Draws a circle.
	void _mr_circle(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_position _radius, mr_color _color);

	/*-----------------------------------------------------------------------
	 --- DRAWING PRIMITIVES (explicit screen) [v1.1]
	 -----------------------------------------------------------------------*/

	// Clears the bitmap to color BLACK.
	#define mr_clear_bitmap(_screen) _mr_clear_bitmap((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * _screen), (mr_color*)COLOUR_RAM_START_ADDRESS );

	// Clears the bitmap to the specified color.
	#define mr_clear_to_color(_screen, _color) _mr_clear_to_color((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * _screen), (mr_color*)COLOUR_RAM_START_ADDRESS, _color);

	// Writes a pixel into a bitmap.
	#define mr_putpixel(_screen, _x, _y, _color) _mr_putpixel((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * _screen), (mr_color*)COLOUR_RAM_START_ADDRESS, _x, _y, _color );

	// Reads a pixel from a bitmap.
	#define mr_getpixel(_screen, _x, _y) _mr_getpixel((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * _screen), (mr_color*)COLOUR_RAM_START_ADDRESS, _x, _y) ;

	// Draws a vertical line onto the bitmap.
	#define mr_vline(_screen, _x, _y1, _y2, _color) _mr_vline((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * _screen), (mr_color*)COLOUR_RAM_START_ADDRESS, _x, _y1, _y2, _color);

	// Draws a horizontal line onto the bitmap.
	#define mr_hline(_screen, _x1, _x2, _y, _color) _mr_hline((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * _screen), (mr_color*)COLOUR_RAM_START_ADDRESS, _x1, _x2, _y, _color);

	// Draws a line onto the bitmap.
	#define mr_line(_screen, _x1, _y1, _x2, _y2, _color) _mr_line((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * _screen), (mr_color*)COLOUR_RAM_START_ADDRESS, _x1, _y1, _x2, _y2, _color);

	// Draws a triangle.
	#define mr_triangle(_screen, _x1, _y1, _x2, _y2, _x3, _y3, _color) _mr_triangle((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * _screen), (mr_color*)COLOUR_RAM_START_ADDRESS, _x1, _y1, _x2, _y2, _x3, _y3, _color);

	// Draws a polygon.
	#define mr_polygon(_screen, _vertices, _points, _color) _mr_polygon((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * _screen), (mr_color*)COLOUR_RAM_START_ADDRESS, _vertices, _points, _color);

	// Draws an outline rectangle.
	#define mr_rect(_screen, _x1, _y1, _x2, _y2, _color) _mr_rect((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * _screen), (mr_color*)COLOUR_RAM_START_ADDRESS, _x1, _y1, _x2, _y2, _color);

	// Draws a circle.
	#define mr_circle(_screen, _x, _y, _radius, _color) _mr_circle((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * _screen), (mr_color*)COLOUR_RAM_START_ADDRESS, _x, _y, _radius, _color);

	/*-----------------------------------------------------------------------
	 --- DRAWING PRIMITIVES (implicit screen [visible]) [v1.1]
	 -----------------------------------------------------------------------*/

	 // Clears the bitmap to color BLACK.
	#define mr_clear_bitmapv() _mr_clear_bitmap((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * VISIBLE_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS );

	// Clears the bitmap to the specified color.
	#define mr_clear_to_colorv(_color) _mr_clear_to_color((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * VISIBLE_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _color);

	// Writes a pixel into a bitmap.
	#define mr_putpixelv(_x, _y, _color) _mr_putpixel((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * VISIBLE_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _x, _y, _color );

	// Reads a pixel from a bitmap.
	#define mr_getpixelv(_x, _y) _mr_getpixel((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * VISIBLE_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _x, _y) ;

	// Draws a vertical line onto the bitmap.
	#define mr_vlinev(_x, _y1, _y2, _color) _mr_vline((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * VISIBLE_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _x, _y1, _y2, _color);

	// Draws a horizontal line onto the bitmap.
	#define mr_hlinev(_x1, _x2, _y, _color) _mr_hline((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * VISIBLE_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _x1, _x2, _y, _color);

	// Draws a line onto the bitmap.
	#define mr_linev(_x1, _y1, _x2, _y2, _color) _mr_line((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * VISIBLE_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _x1, _y1, _x2, _y2, _color);

	// Draws a triangle.
	#define mr_trianglev(_x1, _y1, _x2, _y2, _x3, _y3, _color) _mr_triangle((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * VISIBLE_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _x1, _y1, _x2, _y2, _x3, _y3, _color);

	// Draws a polygon.
	#define mr_polygonv(_vertices, _points, _color) _mr_polygon((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * VISIBLE_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _vertices, _points, _color);

	// Draws an outline rectangle.
	#define mr_rectv(_x1, _y1, _x2, _y2, _color) _mr_rect((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * VISIBLE_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _x1, _y1, _x2, _y2, _color);

	// Draws a circle.
	#define mr_circlev(_screen, _x, _y, _radius, _color) _mr_circle((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * VISIBLE_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _x, _y, _radius, _color);

	/*-----------------------------------------------------------------------
	 --- DRAWING PRIMITIVES (implicit screen [enabled]) [v1.1]
	 -----------------------------------------------------------------------*/

	 // Clears the bitmap to color BLACK.
	#define mr_clear_bitmape() _mr_clear_bitmap((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * ENABLED_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS );

	// Clears the bitmap to the specified color.
	#define mr_clear_to_colore(_color) _mr_clear_to_color((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * ENABLED_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _color);

	// Writes a pixel into a bitmap.
	#define mr_putpixele(_x, _y, _color) _mr_putpixel((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * ENABLED_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _x, _y, _color );

	// Reads a pixel from a bitmap.
	#define mr_getpixele(_x, _y) _mr_getpixel((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * ENABLED_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _x, _y) ;

	// Draws a vertical line onto the bitmap.
	#define mr_vlinee(_x, _y1, _y2, _color) _mr_vline((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * ENABLED_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _x, _y1, _y2, _color);

	// Draws a horizontal line onto the bitmap.
	#define mr_hlinee(_x1, _x2, _y, _color) _mr_hline((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * ENABLED_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _x1, _x2, _y, _color);

	// Draws a line onto the bitmap.
	#define mr_linee(_x1, _y1, _x2, _y2, _color) _mr_line((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * ENABLED_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _x1, _y1, _x2, _y2, _color);

	// Draws a triangle.
	#define mr_trianglee(_x1, _y1, _x2, _y2, _x3, _y3, _color) _mr_triangle((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * ENABLED_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _x1, _y1, _x2, _y2, _x3, _y3, _color);

	// Draws a polygon.
	#define mr_polygone(_vertices, _points, _color) _mr_polygon((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * ENABLED_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _vertices, _points, _color);

	// Draws an outline rectangle.
	#define mr_recte(_x1, _y1, _x2, _y2, _color) _mr_rect((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * ENABLED_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _x1, _y1, _x2, _y2, _color);

	// Draws a circle.
	#define mr_circlee(_screen, _x, _y, _radius, _color) _mr_circle((mr_mixel*)(SCREEN_RAM_START_ADDRESS+(SCREEN_RAM_SIZE) * ENABLED_SCREEN), (mr_color*)COLOUR_RAM_START_ADDRESS, _x, _y, _radius, _color);

	// We include internal function declaration (needed for overlay support)
	#include "midres_int.h"

#endif