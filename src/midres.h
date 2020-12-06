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

	#define mr_mixel_combination(_x,_y) ((_x&1)<<1)|(_y&1)
	#define mr_mixel_bits(_x,_y) MR_RENDERED_MIXEL_BITS[mr_mixel_combination(_x,_y)]

	typedef unsigned char mr_pop;

	// The position of the pixel on the screen is represented by a specific 
	// data type. Since the maximum resolution is 80 x 50, the type used is 
	// the one that represents these two numbers with the lowest numer of bits.

	typedef unsigned char mr_position;

	// The position of the tile on the screen at maximum resolution.

	typedef int mr_tile_position;

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

	// This type of data allows you to choose the blit operation to do.

	#define mr_blitop_set		0x00
	#define mr_blitop_trans		0x01
	#define mr_blitop_and		0x02
	#define mr_blitop_or		0x04
	#define mr_blitop_xor		0x08
	
	#define mr_blitop_mixels	0x0f
	#define mr_blitop_color		0x10

	typedef unsigned char mr_blitop;

	// This type of data allows to represent a "tileset", that is
	// a set of "tiles".

	typedef unsigned char mr_tileset;

	// This type of data allows to represent a "tile", that is
	// a redefined matrix used in place of a matrix of 4 mixel.

	typedef unsigned char mr_tile;

	// This type of data allows to represent the rolling direction (left, right, up, down)

	typedef unsigned char mr_direction;

	#define mr_direction_left	1
	#define mr_direction_right	2
	#define mr_direction_up		1
	#define mr_direction_down	2

	// The speed of an object can be easily represented by this
	// type, that has been added for commodity.

	typedef signed char mr_speed;

	// The acceleration of an object can be easily represented by this
	// type, that has been added for commodity.

	typedef signed char mr_acceleration;

	// The boolean can be easily represented by this type, 
	// that has been added for commodity.

	typedef unsigned char mr_boolean;

	#define mr_false		0
	#define mr_true			1

	// This type can be used to indicate a specific external data file,
	// like tiles, images and so on. 

	typedef unsigned int mr_file;

	// We include some hardware-dependent data type and constants.
	#include "midres_hw.h"

	/************************************************************************
	 * ------ ACCESSIBLE DATA
	 ************************************************************************/

	#ifdef FRAME_BUFFER
		extern unsigned char frameBuffer[MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT];
		extern unsigned char colorBuffer[MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT];
		extern unsigned char auxBuffer[MR_SCREEN_WIDTH * MR_SCREEN_HEIGHT];
	#endif

	 // Set of colors assigned for each screen.
	extern mr_color MR_SCREEN_INK[MR_SCREEN_COUNT];

	// Screen visible to the user.
	extern mr_screen MR_VISIBLE_SCREEN;

	// Screen selected for graphic output (for primitives that do not plan 
	// to give the screen as parameter).
	extern mr_screen MR_ENABLED_SCREEN;

	// Precalculated values for positions.
	extern unsigned char MR_RENDERED_MIXEL_BITS[4];

	// Precalculated values for mixels.
	extern unsigned char MR_RENDERED_MIXELS[16];

	// Screen width
	extern mr_position MR_WIDTH;

	// Screen height
	extern mr_position MR_HEIGHT;

	// Offset brightness for colors
	extern mr_color MR_BRIGHTNESS;

	// First screen for double buffering
	extern mr_screen MR_DB1;

	// Second screen for double buffering
	extern mr_screen MR_DB2;

	// Flag to know if we are in single or multicolor mode
	extern mr_boolean MR_MULTICOLOR;

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

	// To be called before using drawing functions.
	#ifdef __OVERLAY__MIDRES__
		#define mr_use_bitblit() load_overlay("mr5", _OVERLAY5_LOAD__, _OVERLAY5_SIZE__)
	#else
		#define mr_use_bitblit() 
	#endif

	/*-----------------------------------------------------------------------
	 --- LIBRARY MANAGEMENT
	 -----------------------------------------------------------------------*/

	// Initialize the graphics subsystem.
	void mr_init();

	// Initialize the graphics subsystem (with multicolor system).
	void mr_init_multicolor();

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

	// Setup the double buffer (using screens SCREEN_DB1 and SCREEN_DB2)
	void mr_doublebuffer_init();

	// Switch double buffering
	void mr_doublebuffer_switch();

	// Load a (luminance) screen from a file to a specific screen
	unsigned char mr_load(char *_filename, mr_screen _screen);

	// Load a (luminance) screen from a file to a specific screen
	unsigned char mr_load_screen(mr_file _file, mr_screen _screen);

	// Save a specific (luminance) screen to a file
	unsigned char mr_save(char* _filename, mr_screen _screen);

	// Load a (color) screen from a file to a specific screen
	unsigned char mr_load_color(char* _filename, mr_screen _screen);

	// Load a (color) screen from a file to a specific screen
	unsigned char mr_load_screen_color(mr_file _file, mr_screen _screen);

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

	// Set the background color of all screens
	void mr_set_background_color(mr_color _color);

	// Set the border color of all screens
	void mr_set_border_color(mr_color _color);

	/*-----------------------------------------------------------------------
	 --- SCREEN MANAGEMENT (implicit [visible screen])
	 -----------------------------------------------------------------------*/

	// Clear the given screen.
	#define mr_clearv() mr_clear(MR_VISIBLE_SCREEN);

	// Show and enable the given screen for graphical operations.
	// void mr_showv(mr_screen _screen);

	// Enable the screen for graphical operations.
	#define mr_enablev() mr_enable(MR_VISIBLE_SCREEN);

	// Load a (luminance) screen from a file to a specific screen
	#define mr_loadv(_filename) mr_load(_filename, MR_VISIBLE_SCREEN);

	// Load a (luminance) screen from a file to a specific screen
	#define mr_load_screenv(_file) mr_load_screen(_file, MR_VISIBLE_SCREEN);

	// Save a specific (luminance) screen to a file
	#define mr_savev(_filename) mr_save(_filename, MR_VISIBLE_SCREEN);

	// Load a (color) screen from a file to a specific screen
	#define mr_load_colorv(_filename) mr_load_color(_filename, MR_VISIBLE_SCREEN);

	// Load a (color) screen from a file to a specific screen
	#define mr_load_screen_colorv(_file) mr_load_screen_color(_file, MR_VISIBLE_SCREEN);

	// Save a specific (color) screen to a file
	#define mr_save_colorv(_filename) mr_save_color(_filename, MR_VISIBLE_SCREEN);

	// Compress luminance and color in a single mixel screen
	#define mr_compressv(_destination) mr_compress(MR_VISIBLE_SCREEN, _destination);

	// Uncompress luminance and color from a single mixel screen
	#define mr_uncompressv(_source) mr_uncompress(_source, MR_VISIBLE_SCREEN);

	// Pack luminance in an half screen
	#define mr_packv(_destination, _half_screen) mr_pack(MR_VISIBLE_SCREEN, _destination, _half_screen);

	// Unpack luminance in an half screen
	#define mr_unpackv(_source, _half_screen) mr_unpack(_source, MR_VISIBLE_SCREEN, _half_screen);

	/*-----------------------------------------------------------------------
	 --- SCREEN MANAGEMENT (implicit [enabled screen])
	 -----------------------------------------------------------------------*/

	 // Clear the given screen.
	#define mr_cleare() mr_clear(MR_ENABLED_SCREEN);

	// Show and enable the given screen for graphical operations.
	#define mr_showe() mr_show(MR_ENABLED_SCREEN);

	// Enable the screen for graphical operations.
	// #define mr_enablev() mr_enable(MR_ENABLED_SCREEN);

	// Load a (luminance) screen from a file to a specific screen
	#define mr_loade(_filename) mr_load(_filename, MR_ENABLED_SCREEN);

	// Load a (luminance) screen from a file to a specific screen
	#define mr_load_screene(_file) mr_load_screen(_file, MR_ENABLED_SCREEN);

	// Save a specific (luminance) screen to a file
	#define mr_savee(_filename) mr_save(_filename, MR_ENABLED_SCREEN);

	// Load a (luminance) screen from a file to a specific screen
	#define mr_load_screen_colore(_file) mr_load_color(_file, MR_ENABLED_SCREEN);

	// Load a (color) screen from a file to a specific screen
	#define mr_load_colore(_filename) mr_load_color(_filename, MR_ENABLED_SCREEN);

	// Save a specific (color) screen to a file
	#define mr_save_colore(_filename) mr_save_color(_filename, MR_ENABLED_SCREEN);

	// Compress luminance and color in a single mixel screen
	#define mr_compresse(_destination) mr_compress(MR_ENABLED_SCREEN, _destination);

	// Uncompress luminance and color from a single mixel screen
	#define mr_uncompresse(_source) mr_uncompress(_source, MR_ENABLED_SCREEN);

	// Pack luminance in an half screen
	#define mr_packe(_destination, _half_screen) mr_pack(MR_ENABLED_SCREEN, _destination, _half_screen);

	// Unpack luminance in an half screen
	#define mr_unpacke(_source, _half_screen) mr_unpack(_source, MR_ENABLED_SCREEN, _half_screen);

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
	#define mr_pset(_screen, _x, _y) mr_psetop(MR_SM(_screen), _x, _y, mr_pixel_on);

	// Clear a pixel on the given screen
	#define mr_pclear(_screen, _x, _y) mr_psetop(MR_SM(_screen), _x, _y, mr_pixel_off);

	// Invert a pixel on the given screen
	#define mr_pinvert(_screen, _x, _y) mr_psetop(MR_SM(_screen), _x, _y, mr_pixel_invert);

	// Color a pixel on the given screen
	#define mr_pcolor(_screen, _x, _y, _color) mr_pcolorop(MR_CM(_screen), _x, _y, _color);

	// Draw and color a pixel on the given screen
	#define mr_psetc(_screen, _x, _y) mr_pcolorop(MR_CM(_screen), _x, _y, SCREEN_INK[_screen]); mr_psetop(MR_SM(_screen), _x, _y, mr_pixel_on);

	/*-----------------------------------------------------------------------
	 --- DRAWING DERIVATED (IMPLICIT SCREEN [VISIBLE]) [v1.0]
	 -----------------------------------------------------------------------*/

	 // Draw a pixel on the visible screen
	#define mr_psetv(_x, _y) mr_psetop(MR_SM(MR_VISIBLE_SCREEN), _x, _y, mr_pixel_on);

	// Clear a pixel on the visible screen
	#define mr_pclearv(_x, _y) mr_psetop(MR_SM(MR_VISIBLE_SCREEN), _x, _y, mr_pixel_off);

	// Invert a pixel on the visible screen
	#define mr_pinvertv(_x, _y) mr_psetop(MR_SM(MR_VISIBLE_SCREEN), _x, _y, mr_pixel_invert);

	// Color a pixel on the visible screen
	#define mr_pcolorv(_x, _y, _color) mr_pcolorop(MR_CM(MR_VISIBLE_SCREEN), _x, _y, _color);

	// Draw and color a pixel on the visible screen
	#define mr_psetcv(_x, _y) mr_pcolorop(MR_CM(MR_VISIBLE_SCREEN), _x, _y, SCREEN_INK[MR_VISIBLE_SCREEN]); mr_psetop(MR_SM(MR_VISIBLE_SCREEN), _x, _y, mr_pixel_on);

	// Draw a box on the visible screen, with the given logical operation.
	#define mr_boxv(_x0, _y0, _x1, _y1) mr_boxop(MR_SM(MR_ENABLED_SCREEN), _x0, _y0, _x1, _y1);

	/*-----------------------------------------------------------------------
	 --- DRAWING DERIVATED (IMPLICIT SCREEN [ENABLED]) [v1.0]
	 -----------------------------------------------------------------------*/

	 // Draw a pixel on the enabled screen
	#define mr_psete(_x, _y) mr_psetop(MR_SM(MR_ENABLED_SCREEN), _x, _y, mr_pixel_on);

	// Clear a pixel on the enabled screen
	#define mr_pcleare(_x, _y) mr_psetop(MR_SM(MR_ENABLED_SCREEN), _x, _y, mr_pixel_off);

	// Invert a pixel on the enabled screen
	#define mr_pinverte(_x, _y) mr_psetop(MR_SM(MR_ENABLED_SCREEN), _x, _y, mr_pixel_invert);

	// Color a pixel on the enabled screen
	#define mr_pcolore(_x, _y, _color) mr_pcolorop(MR_CM(MR_ENABLED_SCREEN), _x, _y, _color);
	
	// Draw and color a pixel on the enabled screen
	#define mr_psetce(_x, _y) mr_pcolorop(MR_CM(MR_ENABLED_SCREEN), _x, _y, SCREEN_INK[MR_ENABLED_SCREEN]); mr_psetop(MR_SM(MR_ENABLED_SCREEN), _x, _y, mr_pixel_on);

	// Draw a box on the enabled screen, with the given logical operation.
	#define mr_boxe(_x0, _y0, _x1, _y1) mr_boxop(MR_SM(MR_ENABLED_SCREEN), _x0, _y0, _x1, _y1);

	/*-----------------------------------------------------------------------
	 --- DRAWING PRIMITIVES [v1.1]
	 -----------------------------------------------------------------------*/

	// Clears the bitmap to color BLACK.
	void _mr_clear_bitmap(mr_mixel * _screen, mr_color * _colormap);

	// Clears the bitmap to the specified color.
	void _mr_clear_to_color(mr_mixel* _screen, mr_color *_colormap, mr_color _color);

	// Writes a pixel into a bitmap.
	void _mr_putpixel(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_color _color);

	// Clear a pixel into a bitmap.
	void _mr_clearpixel(mr_mixel* _screen, mr_position _x, mr_position _y);

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

	// Draws a filled rectangle.
	void _mr_rectfill(mr_mixel* _screen, mr_color* _colormap, mr_position _x1, mr_position _y1, mr_position _x2, mr_position _y2, mr_color _color);

	// Draws a circle.
	void _mr_circle(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_position _radius, mr_color _color);

	// Color a rectangle.
	void _mr_colorfill(mr_color* _colormap, mr_position _x1, mr_position _y1, mr_position _x2, mr_position _y2, mr_color _color);

	/*-----------------------------------------------------------------------
	 --- DRAWING PRIMITIVES (explicit screen) [v1.1]
	 -----------------------------------------------------------------------*/

	// Clears the bitmap to color BLACK.
	#define mr_clear_bitmap(_screen) _mr_clear_bitmap(MR_SM(_screen), MR_CM(_screen));

	// Clears the bitmap to the specified color.
	#define mr_clear_to_color(_screen, _color) _mr_clear_to_color(MR_SM(_screen), MR_CM(_screen), _color);

	// Writes a pixel into a bitmap.
	#define mr_putpixel(_screen, _x, _y, _color) _mr_putpixel(MR_SM(_screen), MR_CM(_screen), _x, _y, _color );

	// Clear a pixel into a bitmap.
	#define mr_clearpixel(_screen, _x, _y) _mr_clearpixel(MR_SM(_screen), _x, _y );

	// Reads a pixel from a bitmap.
	#define mr_getpixel(_screen, _x, _y) _mr_getpixel(MR_SM(_screen), MR_CM(_screen), _x, _y) ;

	// Draws a vertical line onto the bitmap.
	#define mr_vline(_screen, _x, _y1, _y2, _color) _mr_vline(MR_SM(_screen), MR_CM(_screen), _x, _y1, _y2, _color);

	// Draws a horizontal line onto the bitmap.
	#define mr_hline(_screen, _x1, _x2, _y, _color) _mr_hline(MR_SM(_screen), MR_CM(_screen), _x1, _x2, _y, _color);

	// Draws a line onto the bitmap.
	#define mr_line(_screen, _x1, _y1, _x2, _y2, _color) _mr_line(MR_SM(_screen), MR_CM(_screen), _x1, _y1, _x2, _y2, _color);

	// Draws a triangle.
	#define mr_triangle(_screen, _x1, _y1, _x2, _y2, _x3, _y3, _color) _mr_triangle(MR_SM(_screen), MR_CM(_screen), _x1, _y1, _x2, _y2, _x3, _y3, _color);

	// Draws a polygon.
	#define mr_polygon(_screen, _vertices, _points, _color) _mr_polygon(MR_SM(_screen), MR_CM(_screen), _vertices, _points, _color);

	// Draws an outline rectangle.
	#define mr_rect(_screen, _x1, _y1, _x2, _y2, _color) _mr_rect(MR_SM(_screen), MR_CM(_screen), _x1, _y1, _x2, _y2, _color);

	// Draws a filled rectangle.
	#define mr_rectfill(_screen, _x1, _y1, _x2, _y2, _color) _mr_rectfill(MR_SM(_screen), MR_CM(_screen), _x1, _y1, _x2, _y2, _color);

	// Draws a circle.
	#define mr_circle(_screen, _x, _y, _radius, _color) _mr_circle(MR_SM(_screen), MR_CM(_screen), _x, _y, _radius, _color);

	// Color a filled rectangle.
	#define mr_colorfill(_screen, _x1, _y1, _x2, _y2, _color) _mr_colorfill(MR_CM(_screen), _x1, _y1, _x2, _y2, _color);

	/*-----------------------------------------------------------------------
	 --- DRAWING PRIMITIVES (implicit screen [visible]) [v1.1]
	 -----------------------------------------------------------------------*/

	 // Clears the bitmap to color BLACK.
	#define mr_clear_bitmapv() _mr_clear_bitmap(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN) );

	// Clears the bitmap to the specified color.
	#define mr_clear_to_colorv(_color) _mr_clear_to_color(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _color);

	// Writes a pixel into a bitmap.
	#define mr_putpixelv(_x, _y, _color) _mr_putpixel(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x, _y, _color );

	// Clear a pixel into a bitmap.
	#define mr_clearpixelv(_screen, _x, _y) _mr_clearpixel(MR_SM(MR_VISIBLE_SCREEN), _x, _y );

	// Reads a pixel from a bitmap.
	#define mr_getpixelv(_x, _y) _mr_getpixel(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x, _y) ;

	// Draws a vertical line onto the bitmap.
	#define mr_vlinev(_x, _y1, _y2, _color) _mr_vline(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x, _y1, _y2, _color);

	// Draws a horizontal line onto the bitmap.
	#define mr_hlinev(_x1, _x2, _y, _color) _mr_hline(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x1, _x2, _y, _color);

	// Draws a line onto the bitmap.
	#define mr_linev(_x1, _y1, _x2, _y2, _color) _mr_line(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x1, _y1, _x2, _y2, _color);

	// Draws a triangle.
	#define mr_trianglev(_x1, _y1, _x2, _y2, _x3, _y3, _color) _mr_triangle(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x1, _y1, _x2, _y2, _x3, _y3, _color);

	// Draws a polygon.
	#define mr_polygonv(_vertices, _points, _color) _mr_polygon(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _vertices, _points, _color);

	// Draws an outline rectangle.
	#define mr_rectv(_x1, _y1, _x2, _y2, _color) _mr_rect(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x1, _y1, _x2, _y2, _color);

	// Draws a filled rectangle.
	#define mr_rectfillv(_screen, _x1, _y1, _x2, _y2, _color) _mr_rectfill(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x1, _y1, _x2, _y2, _color);

	// Draws a circle.
	#define mr_circlev(_screen, _x, _y, _radius, _color) _mr_circle(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x, _y, _radius, _color);

	// Color a filled rectangle.
	#define mr_colorfillv(_screen, _x1, _y1, _x2, _y2, _color) _mr_colorfill(MR_CM(_screen), _x1, _y1, _x2, _y2, _color);

	/*-----------------------------------------------------------------------
	 --- DRAWING PRIMITIVES (implicit screen [enabled]) [v1.1]
	 -----------------------------------------------------------------------*/

	// Clears the bitmap to color BLACK.
	#define mr_clear_bitmape() _mr_clear_bitmap(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN) );

	// Clears the bitmap to the specified color.
	#define mr_clear_to_colore(_color) _mr_clear_to_color(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _color);

	// Writes a pixel into a bitmap.
	#define mr_putpixele(_x, _y, _color) _mr_putpixel(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x, _y, _color );

	// Clear a pixel into a bitmap.
	#define mr_clearpixele(_screen, _x, _y) _mr_clearpixel(MR_SM(MR_ENABLED_SCREEN), _x, _y );

	// Reads a pixel from a bitmap.
	#define mr_getpixele(_x, _y) _mr_getpixel(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x, _y) ;

	// Draws a vertical line onto the bitmap.
	#define mr_vlinee(_x, _y1, _y2, _color) _mr_vline(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x, _y1, _y2, _color);

	// Draws a horizontal line onto the bitmap.
	#define mr_hlinee(_x1, _x2, _y, _color) _mr_hline(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x1, _x2, _y, _color);

	// Draws a line onto the bitmap.
	#define mr_linee(_x1, _y1, _x2, _y2, _color) _mr_line(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x1, _y1, _x2, _y2, _color);

	// Draws a triangle.
	#define mr_trianglee(_x1, _y1, _x2, _y2, _x3, _y3, _color) _mr_triangle(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x1, _y1, _x2, _y2, _x3, _y3, _color);

	// Draws a polygon.
	#define mr_polygone(_vertices, _points, _color) _mr_polygon(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _vertices, _points, _color);

	// Draws an outline rectangle.
	#define mr_recte(_x1, _y1, _x2, _y2, _color) _mr_rect(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x1, _y1, _x2, _y2, _color);

	// Draws a filled rectangle.
	#define mr_rectfille(_screen, _x1, _y1, _x2, _y2, _color) _mr_rectfill(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x1, _y1, _x2, _y2, _color);

	// Draws a circle.
	#define mr_circlee(_screen, _x, _y, _radius, _color) _mr_circle(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x, _y, _radius, _color);

	// Color a filled rectangle.
	#define mr_colorfille(_screen, _x1, _y1, _x2, _y2, _color) _mr_colorfill(MR_CM(_screen), _x1, _y1, _x2, _y2, _color);

	/*-----------------------------------------------------------------------
	 --- BIT BLITS
	 -----------------------------------------------------------------------*/

	// Copy a rectangular region from a screen / memory to another screen / memory
	void _mr_copy(mr_mixel* _source, mr_color* _source_color, mr_position _xs, mr_position _ys, mr_position _ws, mr_position _hs, mr_position _displs, mr_mixel* _destination, mr_color* _destination_color, mr_position _xd, mr_position _yd, mr_position _displd, mr_blitop _blitop);

	/*-----------------------------------------------------------------------
	 --- BIT BLITS (explicit screen)
	 -----------------------------------------------------------------------*/

	// Copy a rectangular region from a screen / memory to another screen / memory
	#define mr_copy(_source, _xs, _ys, _ws, _hs, _displs, _destination, _xd, _yd, _displd, _blitop) _mr_copy(MR_SM(_source), MR_CM(_source), _xs, _ys, _ws, _hs, _displs, MR_SM(_destination), MR_CM(_destination), _xd, _yd, _displd, _blitop);

	// Copy a rectangular region from screen to memory
	#define mr_blit_from_screen(_source, _xs, _ys, _ws, _hs, _destination, _xd, _yd, _blitop) _mr_copy(MR_SM(_source), MR_CM(_source), _xs, _ys, _ws, _hs, MR_SCREEN_WIDTH, MR_SM(_destination), MR_SM(_destination+1), _xd, _yd, MR_SCREEN_WIDTH, _blitop);

	// Copy a rectangular region from memory to screen
	#define mr_blit_to_screen(_source, _xs, _ys, _ws, _hs, _destination, _xd, _yd, _blitop) _mr_copy(MR_SM(_source), MR_SM(_source+1), _xs, _ys, _ws, _hs, MR_SCREEN_WIDTH, MR_SM(_destination), MR_CM(_destination), _xd, _yd, MR_SCREEN_WIDTH, _blitop);

	/*-----------------------------------------------------------------------
	 --- BIT BLITS (implicit screen [visible])
	 -----------------------------------------------------------------------*/

	// Copy a rectangular region from screen to memory
	#define mr_blit_from_screenv(_xs, _ys, _ws, _hs, _destination, _xd, _yd, _blitop) _mr_copy(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _xs, _ys, _ws, _hs, MR_SCREEN_WIDTH, MR_SM(_destination), MR_SM(_destination+1), _xd, _yd, MR_SCREEN_WIDTH, _blitop);

	// Copy a rectangular region from memory to screen
	#define mr_blit_to_screenv(_source, _xs, _ys, _ws, _hs, _xd, _yd, _blitop) _mr_copy(MR_SM(_source), MR_SM(_source+1), _xs, _ys, _ws, _hs, MR_SCREEN_WIDTH, MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _xd, _yd, MR_SCREEN_WIDTH, _blitop);

	/*-----------------------------------------------------------------------
	 --- BIT BLITS (implicit screen [visible])
	 -----------------------------------------------------------------------*/

	// Copy a rectangular region from screen to memory
	#define mr_blit_from_screene(_xs, _ys, _ws, _hs, _destination, _xd, _yd, _blitop) _mr_copy(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _xs, _ys, _ws, _hs, MR_SCREEN_WIDTH, MR_SM(_destination), MR_SM(_destination+1), _xd, _yd, MR_SCREEN_WIDTH, _blitop);

	// Copy a rectangular region from memory to screen
	#define mr_blit_to_screene(_source, _xs, _ys, _ws, _hs, _xd, _yd, _blitop) _mr_copy(MR_SM(_source), MR_SM(_source+1), _xs, _ys, _ws, _hs, MR_SCREEN_WIDTH, MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _xd, _yd, MR_SCREEN_WIDTH, _blitop);

	/*-----------------------------------------------------------------------
	 --- TILES
	 -----------------------------------------------------------------------*/

	// Set the visible tilesect.
	void mr_tileset_visible(mr_tileset _tileset);

	// Copy a tileset over another.
	void mr_tileset_copy(mr_tileset _source, mr_tileset _destination);

	// Copy a tileset over another (memory mapped video ram).
	void mr_tileset_copy_memory_mapped(mr_tileset _source, mr_tileset _destination);

	// Downgrade a tileset from multicolor to monocolor.
	void mr_tileset_multicolor_to_monocolor_memory_mapped(mr_tileset _source, mr_position _starting, mr_position _count);

	// Redefine a tile using the given data.
	void mr_tile_redefine(mr_tileset _tileset, mr_tile _tile, mr_mixel* _data);

	// Redefine a tile using the given data.
	void mr_tile_redefine_memory_mapped(mr_tileset _tileset, mr_tile _tile, mr_mixel* _data);

	// Redefine a tile filling the tile with the given data.
	void mr_tile_redefine_fill(mr_tileset _tileset, mr_tile _tile, mr_mixel _data);

	// Redefine a tile filling the tile with the given data.
	void mr_tile_redefine_fill_memory_mapped(mr_tileset _tileset, mr_tile _tile, mr_mixel _data);

	// Set a single color for multicolor tiles
	void mr_tile_setcolor(mr_position _index, mr_color _color);

	// Set a set of colors for multicolor tiles
	void mr_tile_setcolors(mr_color _colors[4]);

	// Redefine a subset of N tiles by "shifting" horizontally a tile
	void mr_tile_prepare_horizontal(mr_tileset _tileset, mr_tile _source, mr_tile _destination);

	// Redefine a subset of N tiles by "shifting" horizontally a tile on memory mapped platforms
	void mr_tile_prepare_horizontal_monocolor_memory_mapped(mr_tileset _tileset, mr_tile _source, mr_tile _destination);
	
	// Redefine a subset of (M+2)xN tiles by "shifting" horizontally a tile
	void mr_tile_prepare_horizontal_extended(mr_tileset _tileset, mr_tile _source, mr_tile _w, mr_tile _h, mr_tile _destination);

	// Redefine a subset of (M+2)xN tiles by "shifting" horizontally a tile on memory mapped platforms
	void mr_tile_prepare_horizontal_extended_monocolor_memory_mapped(mr_tileset _tileset, mr_tile _source, mr_tile _w, mr_tile _h, mr_tile _destination);

	// Calculate che number of frames needed to store the preparation
	#define mr_calculate_prepared_tile_size( _w, _h )	( ( ( _w + 1 ) * _h ) * 9 )
	#define mr_calculate_prepared_tile_size_horizontal( _w, _h )	mr_calculate_prepared_tile_size( _w, _h )
	#define mr_calculate_prepared_tile_size_vertical( _w, _h )	( ( ( _w * ( _h + 1 ) ) ) * 9 )

	// Writes a tile into a bitmap at *precise* horizontal position.
	void _mr_tile_moveto_horizontal(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_color _color);

	// Writes a tile into a bitmap at *precise* horizontal position.
	void _mr_tile_moveto_horizontal_extended(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_position _w, mr_position _h, mr_color _color);

	// Redefine a subset of N tiles by "shifting" vertically a tile
	void mr_tile_prepare_vertical(mr_tileset _tileset, mr_tile _source, mr_tile _destination);

	// Redefine a subset of N tiles by "shifting" vertically a tile on memory mapped platforms
	void mr_tile_prepare_vertical_memory_mapped(mr_tileset _tileset, mr_tile _source, mr_tile _destination);

	// Redefine a subset of N tiles by "shifting" vertically a tile
	void mr_tile_prepare_vertical_extended(mr_tileset _tileset, mr_tile _source, mr_position _w, mr_position _h, mr_tile _destination);

	// Redefine a subset of N tiles by "shifting" vertically a tile
	void mr_tile_prepare_vertical_extended_memory_mapped(mr_tileset _tileset, mr_tile _source, mr_position _w, mr_position _h, mr_tile _destination);

	// Writes a tile into a bitmap at *precise* vertical position.
	void _mr_tile_moveto_vertical(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_color _color);

	// Writes a tile into a bitmap at *precise* vertical position.
	void _mr_tile_moveto_vertical_extended(mr_mixel* _screen, mr_color* _colormap, mr_tile_position _x, mr_tile_position _y, mr_tile _tile, mr_position _w, mr_position _h, mr_color _color);

	// Redefine a subset of N tiles by "rolling" horizontally a tile
	void mr_tile_prepare_roll_horizontal(mr_tileset _tileset, mr_tile _source, mr_tile _destination);
	
	// Redefine a subset of N tiles by "rolling" horizontally a tile using a memory mapped platform
	void mr_tile_prepare_roll_horizontal_monocolor_memory_mapped(mr_tileset _tileset, mr_tile _source, mr_tile _destination);

	// Roll horizontally a tile
	void mr_tile_roll_horizontal(mr_tileset _tileset, mr_tile _tile, mr_direction _direction);

	// Roll horizontally a tile
	void mr_tile_roll_horizontal_memory_mapped(mr_tileset _tileset, mr_tile _destination, mr_direction _direction);

	// Roll horizontally a tile
	void mr_tile_roll_horizontal_on_place(mr_tileset _tileset, mr_tile _tile, mr_direction _direction, mr_tile _place, mr_position *_index);
	
	// Roll horizontally a tile on a memory mapped platform
	void mr_tile_roll_horizontal_on_place_memory_mapped(mr_tileset _tileset, mr_tile _destination, mr_direction _direction, mr_tile _place, mr_position* _index);

	// Redefine a subset of N tiles by "rolling" vertically a tile
	void mr_tile_prepare_roll_vertical(mr_tileset _tileset, mr_tile _source, mr_tile _destination);

	// Redefine a subset of N tiles by "rolling" vertically a tile on memory mapped platform
	void mr_tile_prepare_roll_vertical_memory_mapped(mr_tileset _tileset, mr_tile _source, mr_tile _destination);

	// Roll vertically a tile
	void mr_tile_roll_vertical(mr_tileset _tileset, mr_tile _tile, mr_direction _direction);

	// Roll vertically a tile on memory mapped platforms
	void mr_tile_roll_vertical_memory_mapped(mr_tileset _tileset, mr_tile _destination, mr_direction _direction);

	// Roll vertically a tile
	void mr_tile_roll_vertical_on_place(mr_tileset _tileset, mr_tile _tile, mr_direction _direction, mr_tile _place, mr_position* _index);

	// Writes a tile into a bitmap.
	void _mr_puttile(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile, mr_color _color);

	// Writes a set of tiles into a bitmap.
	void _mr_puttiles(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile_start, mr_tile _tile_count, mr_color _color);

	// Writes a set of tiles into a bitmap.
	void _mr_putetiles(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile_start, mr_position _w, mr_position _h, mr_color _color);

	// Writes a set of tiles into a bitmap.
	void _mr_putftiles(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_tile _tile_start, mr_position _w, mr_position _h, mr_color _color);

	// Clear a tile of a bitmap.
	void _mr_cleartile(mr_mixel* _screen, mr_position _x, mr_position _y);

	// Reads a tile from a bitmap.
	mr_color _mr_gettile(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y);

	// Load a tileset (or part of it)
	void mr_tileset_load(unsigned char *_filename, mr_tileset _tileset, mr_tile _starting, mr_tile _count);

	// Load a tileset (or part of it)
	void mr_tileset_load_file(mr_file _file, mr_tileset _tileset, mr_tile _starting, mr_tile _count);

	// Draws a vertical line onto the bitmap.
	void _mr_vtiles(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y1, mr_position _y2, mr_tile _tile, mr_color _color);

	// Draws a horizontal line onto the bitmap.
	void _mr_htiles(mr_mixel* _screen, mr_color* _colormap, mr_position _x1, mr_position _x2, mr_position _y, mr_tile _tile, mr_color _color);

	/*-----------------------------------------------------------------------
	 --- TILES (explicit screen)
	 -----------------------------------------------------------------------*/

	// Writes a tile into a bitmap.
	#define mr_puttile(_screen, _x, _y, _tile, _color) _mr_puttile(MR_SM(_screen), MR_CM(_screen), _x, _y, _tile, _color );

	// Writes a tile into a bitmap.
	#define mr_puttiles(_screen, _x, _y, _tile, _count, _color) _mr_puttiles(MR_SM(_screen), MR_CM(_screen), _x, _y, _tile, _count, _color );

	// Writes a tile into a bitmap.
	#define mr_putetiles(_screen, _x, _y, _tile, _w, _h, _color) _mr_putetiles(MR_SM(_screen), MR_CM(_screen), _x, _y, _tile, _w, _h, _color );

	// Writes a set of tiles into a bitmap.
	#define mr_putftiles(_screen, _x, _y, _tile_start, _w, _h, _color) _mr_putftiles(MR_SM(_screen), MR_CM(_screen), mr_position _x, mr_position _y, mr_tile _tile_start, mr_position _w, mr_position _h, mr_color _color);

	// Writes a tile into a bitmap at *precise* horizontal position.
	#define mr_tile_moveto_horizontal(_screen, _x, _y, _tile, _color) _mr_tile_moveto_horizontal(MR_SM(_screen), MR_CM(_screen), _x, _y, _tile, _color );

	// Writes a tile into a bitmap at *precise* horizontal position.
	#define mr_tile_moveto_horizontal_extended(_screen, _x, _y, _tile, _w, _h, _color) _mr_tile_moveto_horizontal_extended(MR_SM(_screen), MR_CM(_screen), _x, _y, _tile, _w, _h, _color );

	// Writes a tile into a bitmap at *precise* horizontal position.
	#define mr_tile_moveto_vertical(_screen, _x, _y, _tile, _color) _mr_tile_moveto_vertical(MR_SM(_screen), MR_CM(_screen), _x, _y, _tile, _color );

	// Writes a tile into a bitmap at *precise* horizontal position.
	#define mr_tile_moveto_vertical_extended(_screen, _x, _y, _tile, _w, _h, _color) _mr_tile_moveto_vertical_extended(MR_SM(_screen), MR_CM(_screen), _x, _y, _tile, _w, _h, _color );

	// Clear a tile of a bitmap.
	#define mr_cleartile(_screen, _x, _y) _mr_cleartile(MR_SM(_screen), _x, _y);

	// Reads a tile from a bitmap.
	#define mr_gettile(_screen, _x, _y) _mr_gettile(MR_SM(_screen), MR_CM(_screen), _x, _y)

	// Draws a vertical line of tiles on the bitmap.
	#define mr_vtiles(_screen, _x, _y1, _y2, _tile, _color) _mr_vtiles(MR_SM(_screen), MR_CM(_screen), _x, _y1, _y2, _tile, _color);

	// Draws a horizontal line of tiles onto the bitmap.
	#define mr_htiles(_screen, _x1, _x2, _y, _tile, _color) _mr_htiles(MR_SM(_screen), MR_CM(_screen), _x1, _x2, _y, _tile, _color);

	/*-----------------------------------------------------------------------
	 --- TILES (implicit screen [VISIBLE])
	 -----------------------------------------------------------------------*/

	// Writes a tile into a bitmap.
	#define mr_puttilev(_x, _y, _tile, _color) _mr_puttile(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x, _y, _tile, _color );

	// Writes a set of tiles into a bitmap.
	#define mr_puttilesv(_x, _y, _tile, _count, _color) _mr_puttiles(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x, _y, _tile, _count, _color );

	// Writes a set of tiles into a bitmap.
	#define mr_putetilesv(_x, _y, _tile, _w, _h, _color) _mr_putetiles(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x, _y, _tile, _w, _h, _color );

	// Writes a set of tiles into a bitmap.
	#define mr_putftilesv(_x, _y, _tile_start, _w, _h, _color) _mr_putftiles(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x, _y, _tile_start, _w, _h, _color);

	// Writes a tile into a bitmap at *precise* horizontal position.
	#define mr_tile_moveto_horizontalv(_x, _y, _tile, _color) _mr_tile_moveto_horizontal(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x, _y, _tile, _color );

	// Writes a tile into a bitmap at *precise* horizontal position.
	#define mr_tile_moveto_horizontal_extendedv(_x, _y, _tile, _w, _h, _color) _mr_tile_moveto_horizontal_extended(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x, _y, _tile, _w, _h, _color );

	// Writes a tile into a bitmap at *precise* horizontal position.
	#define mr_tile_moveto_verticalv(_x, _y, _tile, _color) _mr_tile_moveto_vertical(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x, _y, _tile, _color );

	// Writes a tile into a bitmap at *precise* horizontal position.
	#define mr_tile_moveto_vertical_extendedv(_x, _y, _tile, _w, _h, _color) _mr_tile_moveto_vertical_extended(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x, _y, _tile, _w, _h, _color );

	// Clear a tile of a bitmap.
	#define mr_cleartilev(_x, _y) _mr_cleartile(MR_SM(MR_VISIBLE_SCREEN), _x, _y);

	// Writes a tile into a bitmap at *precise* horizontal position.
	#define mr_puttilehv(_x, _y, _tile, _color) _mr_puttile(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x, _y, _tile, _color );

	// Writes a tile into a bitmap at *precise* horizontal position.
	#define mr_puttilevv(_x, _y, _tile, _color) _mr_puttile(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x, _y, _tile, _color );

	// Reads a tile from a bitmap.
	#define mr_gettilev(_x, _y) _mr_gettile(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x, _y)

	// Draws a vertical line of tiles on the bitmap.
	#define mr_vtilesv(_x, _y1, _y2, _tile, _color) _mr_vtiles(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x, _y1, _y2, _tile, _color);

	// Draws a horizontal line of tiles onto the bitmap.
	#define mr_htilesv(_x1, _x2, _y, _tile, _color) _mr_htiles(MR_SM(MR_VISIBLE_SCREEN), MR_CM(MR_VISIBLE_SCREEN), _x1, _x2, _y, _tile, _color);

	/*-----------------------------------------------------------------------
	 --- TILES (implicit screen [ENABLED])
	 -----------------------------------------------------------------------*/

	// Writes a tile into a bitmap.
	#define mr_puttilee(_x, _y, _tile, _color) _mr_puttile(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x, _y, _tile, _color );

	// Writes a set of tiles into a bitmap.
	#define mr_puttilese(_x, _y, _tile, _count, _color) _mr_puttile(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x, _y, _tile, _count, _color );

	// Writes a set of tiles into a bitmap.
	#define mr_putetilese(_x, _y, _tile, _w, _h, _color) _mr_putetiles(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x, _y, _tile, _w, _h, _color );

	// Writes a set of tiles into a bitmap.
	#define mr_putftilese(_x, _y, _tile_start, _w, _h, _color) _mr_putftiles(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), mr_position _x, mr_position _y, mr_tile _tile_start, mr_position _w, mr_position _h, mr_color _color);

	// Writes a tile into a bitmap at *precise* horizontal position.
	#define mr_tile_moveto_horizontale(_x, _y, _tile, _color) _mr_tile_moveto_horizontal(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x, _y, _tile, _color );

	// Writes a tile into a bitmap at *precise* horizontal position.
	#define mr_tile_moveto_horizontal_extendede(_x, _y, _tile, _w, _h, _color) _mr_tile_moveto_horizontal(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x, _y, _tile, _w, _h, _color );

	// Writes a tile into a bitmap at *precise* horizontal position.
	#define mr_tile_moveto_verticale(_x, _y, _tile, _color) _mr_tile_moveto_vertical(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x, _y, _tile, _color );

	// Writes a tile into a bitmap at *precise* horizontal position.
	#define mr_tile_moveto_vertical_extendede(_x, _y, _tile, _w, _h, _color) _mr_tile_moveto_vertical_extended(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x, _y, _tile, _w, _h, _color );

	// Clear a tile of a bitmap.
	#define mr_cleartilee(_x, _y, _tile) _mr_cleartile(MR_SM(MR_ENABLED_SCREEN), _x, _y);

	// Reads a tile from a bitmap.
	#define mr_gettilee(_x, _y, _tile, _color) _mr_gettile(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x, _y, _tile )

	// Draws a vertical line of tiles on the bitmap.
	#define mr_vtilese(_x, _y1, _y2, _tile, _color) _mr_vtiles(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x, _y1, _y2, _tile, _color);

	// Draws a horizontal line of tiles onto the bitmap.
	#define mr_htilese(_x1, _x2, _y, _tile, _color) _mr_htiles(MR_SM(MR_ENABLED_SCREEN), MR_CM(MR_ENABLED_SCREEN), _x1, _x2, _y, _tile, _color);

	/*-----------------------------------------------------------------------
	 --- PROTOTHREADING
	 -----------------------------------------------------------------------*/

	typedef unsigned int mr_lc_t;

	#define MR_LC_INIT(s)		s = 0;
	#define MR_LC_RESUME(s)		switch((unsigned int)(s)) { case 0:
	#define MR_LC_SET(s)		s = (unsigned int)(__LINE__); case (unsigned int)(__LINE__):
	#define MR_LC_END(s)		}

	struct mr_pt {	mr_lc_t lc; };
	typedef struct mr_pt mr_protothread;
	#define MR_PT_CTX(ctx,x)	typedef struct { mr_lc_t lc; x } ctx##_protothread;

	#define MR_PT_WAITING		0
	#define MR_PT_YIELDED		1
	#define MR_PT_EXITED		2
	#define MR_PT_ENDED			3

	#define MR_PT_INIT(pt)		MR_LC_INIT((pt)->lc)
	#define MR_PT_THREAD(name)	unsigned char name(mr_protothread * _mr_pt)
	#define MR_PT_THREAD_EXT(name,ctx) \
								unsigned char name(ctx * _mr_pt)

	#define MR_PT_BEGIN(pt)		{ char MR_PT_YIELD_FLAG = 1; MR_LC_RESUME((pt)->lc)
	#define MR_PT_END(pt)		MR_LC_END((pt)->lc); MR_PT_YIELD_FLAG = 0; MR_PT_INIT(pt); return MR_PT_ENDED; }
	#define MR_PT_SCHEDULE(f)	((f) < MR_PT_EXITED)

	#define MR_PT_WAIT_UNTIL(pt, condition)									\
								do {										\
									MR_LC_SET((pt)->lc);					\
									if(!(condition)) {						\
										return MR_PT_WAITING;				\
									}										\
								} while(0)
	#define MR_PT_WAIT_WHILE(pt, cond) \
								MR_PT_WAIT_UNTIL((pt), !(cond))
	#define MR_PT_WAIT_THREAD(pt, thread) \
								MR_PT_WAIT_WHILE((pt), MR_PT_SCHEDULE(thread))
	#define MR_PT_SPAWN(pt, child, thread)	\
								do {										\
									MR_PT_INIT((child));					\
									MR_PT_WAIT_THREAD((pt), (thread));		\
								} while(0)
	#define MR_PT_RESTART(pt) \
								do {										\
									MR_PT_INIT(pt);							\
									return MR_PT_WAITING;					\
								} while(0)

	#define MR_PT_EXIT(pt) \
								do {										\
									MR_PT_INIT(pt);							\
									return MR_PT_EXITED;					\
								} while(0)

	#define MR_PT_YIELD(pt)	\
								do {										\
									MR_PT_YIELD_FLAG = 0;					\
									MR_LC_SET((pt)->lc);					\
									if(MR_PT_YIELD_FLAG == 0) {				\
										return MR_PT_YIELDED;				\
									}										\
								} while(0)

	#define MR_PT_YIELD_UNTIL(pt, cond) \
								do {										\
									MR_PT_YIELD_FLAG = 0;					\
									MR_LC_SET((pt)->lc);					\
									if((MR_PT_YIELD_FLAG == 0) || !(cond)) {\
									  return MR_PT_YIELDED;					\
									}										\
								} while(0)

	///

	#define MR_PTI_BEGIN()		{ char MR_PT_YIELD_FLAG = 1; MR_LC_RESUME((_mr_pt)->lc)
	#define MR_PTI_CTX()		(_mr_pt)

	#define MR_PTI_END() \
								MR_LC_END((_mr_pt)->lc);					\
								MR_PT_YIELD_FLAG = 0;						\
								MR_PT_INIT(_mr_pt);							\
								return MR_PT_ENDED;							\
								}
	#define MR_PTI_WAIT_UNTIL(condition) \
								do {										\
									MR_LC_SET((_mr_pt)->lc);				\
									if(!(condition)) {						\
										return MR_PT_WAITING;				\
									}										\
								} while(0)
	#define MR_PTI_WAIT_WHILE(cond) \
								MR_PT_WAIT_UNTIL((_mr_pt), !(cond))
	#define MR_PTI_WAIT_THREAD(thread) \
								MR_PT_WAIT_WHILE((_mr_pt), MR_PT_SCHEDULE(thread))
	#define MR_PTI_SPAWN(child, thread) \
								do {										\
									MR_PT_INIT((child));					\
									MR_PTI_WAIT_THREAD((thread));			\
								} while(0)
	#define MR_PTI_RESTART() \
								do {										\
									MR_PT_INIT(_mr_pt);						\
									return MR_PT_WAITING;					\
								} while(0)
	#define MR_PTI_EXIT() \
								do {										\
									MR_PT_INIT(_mr_pt);						\
									return MR_PT_EXITED;					\
								} while(0)
	#define MR_PTI_YIELD() \
								do {										\
									MR_PT_YIELD_FLAG = 0;					\
									MR_LC_SET((_mr_pt)->lc);				\
									if(MR_PT_YIELD_FLAG == 0) {				\
										return MR_PT_YIELDED;				\
									}										\
								} while(0)
	#define MR_PTI_YIELD_UNTIL(cond) \
								do {										\
									MR_PT_YIELD_FLAG = 0;					\
									MR_LC_SET((_mr_pt)->lc);				\
									if((MR_PT_YIELD_FLAG == 0) || !(cond)) {\
										return MR_PT_YIELDED;				\
									}										\
								} while(0)

	struct mr_pt_sem { unsigned char count; };
	typedef struct mr_pt_sem mr_semaphore;

	#define MR_PTI_SEM_INIT(s, c) \
								(s)->count = c;
	#define MR_PTI_SEM_WAIT(s) \
								do {                                        \
									MR_PTI_WAIT_UNTIL((s)->count > 0);      \
									--(s)->count;                           \
								} while(0)
	#define MR_PTI_SEM_SIGNAL(s) \
								++(s)->count;

	struct mr_pt_wait { unsigned char count; };
	typedef struct mr_pt_wait mr_twait;

	#define MR_PTI_WAIT(s, c) \
								(s)->count = c;					\
								while( (s)->count ) {			\
									--(s)->count;				\
									MR_PTI_YIELD( );			\
								}
	#define MR_PTI_RESET(s, c) \
								(s)->count = c;


	/*-----------------------------------------------------------------------
	 --- KEYBOARD & OTHER
	 -----------------------------------------------------------------------*/

	unsigned char mr_key_pressed();
	
	unsigned char mr_get_key_pressed();

	void mr_wait(unsigned char _seconds);

	void mr_wait_jiffies(unsigned char _jiffies);

	mr_boolean mr_wait_or_keypress(unsigned char _seconds);

	unsigned char mr_wait_or_get_keypressed(unsigned char _seconds);

	mr_boolean mr_wait_jiffies_or_keypress(unsigned char _jiffies);

	void mr_sound_start(unsigned char _number);

	void mr_sound_change(int _parameter);

	void mr_sound_stop();

	void mr_sound_start_channel(unsigned char _channel, unsigned char _number);

	void mr_sound_change_channel(unsigned char _channel, int _parameter);

	void mr_sound_stop_channel(unsigned char _channel);

	void mr_start_frame();

	void mr_end_frame(unsigned char _jiffies);

	unsigned char *mr_translate_file(mr_file _file);

	void mr_read_file(mr_file _file, unsigned char* _dest, unsigned int _size);

	void mr_read_file_from_offset(mr_file _file, unsigned int _offset, unsigned char* _dest, unsigned int _size);

	unsigned char mr_joy(unsigned char _number);

	void mr_end_frame(unsigned char _jiffies);

	// We include internal function declaration (needed for overlay support)
	#include "midres_int.h"

#endif