/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

#ifndef _MIDRES_HW_H_
#define _MIDRES_HW_H_

	#ifdef __C64__

		/******************************************************************
		 * ------ COMMODORE 64 PLATFORM
		 ******************************************************************/

		// The maximum resolution is 80 x 50 pixels, equivalent to 
		// 40 x 25 mixels.
		#define SCREEN_WIDTH				40
		#define SCREEN_HEIGHT				25

		// Luminance data (on/off pixel) is stored starting from 0x8000 and
		// each screen occupy 1.024 bytes. The colour data, instead, is
		// stored in one single space ad 0xd800.
		#define SCREEN_RAM_START_ADDRESS	0x8000
		#define SCREEN_RAM_SIZE				0x0400
		#define COLOUR_RAM_START_ADDRESS	0xd800
		#define COLOUR_RAM_SIZE_MASK		0x0000

		// The startup (BASIC) screen is 1.
		#define		DEFAULT_SCREEN			 1

		// The memory area useful as an auxiliary screen
		#define		AUXILIARY_SCREEN		5

		// The four screens used for double buffering
		#define		SCREEN_DB1				0
		#define		SCREEN_DB2				1

		// There is space for 16 screens.
		#define		MAX_SCREENS				16

		// Offset for correct brightness.
		#define COLOR_BRIGHTNESS			 0

		// Default palette.
		#define MR_COLOR_BLACK				 0
		#define MR_COLOR_WHITE				 1
		#define MR_COLOR_RED				 2
		#define MR_COLOR_CYAN				 3
		#define MR_COLOR_VIOLET				 4
		#define MR_COLOR_GREEN				 5
		#define MR_COLOR_BLUE				 6
		#define MR_COLOR_YELLOW				 7
		#define MR_COLOR_ORANGE				 8
		#define MR_COLOR_BROWN				 9
		#define MR_COLOR_LIGHT_RED			10
		#define MR_COLOR_DARK_GREY			11
		#define MR_COLOR_GREY				12
		#define MR_COLOR_LIGHT_GREEN		13
		#define MR_COLOR_LIGHT_BLUE			14
		#define MR_COLOR_LIGHT_GREY			15

	#elif __VIC20__

		/******************************************************************
		 * ------ VIC 20 PLATFORM
		 ******************************************************************/

		 // The maximum resolution is 44 x 46 pixels, equivalent to 
		 // 22 x 23 mixels.
		#define SCREEN_WIDTH				22
		#define SCREEN_HEIGHT				23

		#ifdef __24K__

			// Luminance data (on/off pixel) is stored starting from 0x1000 and
			// each screen occupy about 512 bytes. The colour data, instead, is
			// stored in one single space ad 0x9400.
			#define SCREEN_RAM_START_ADDRESS	0x1000
			#define SCREEN_RAM_SIZE				0x0200
			#define COLOUR_RAM_START_ADDRESS	0x9400
			#define COLOUR_RAM_SIZE_MASK		0x0000

			// The memory area useful as an auxiliary screen
			#define		AUXILIARY_SCREEN			10

		#else

			// Luminance data (on/off pixel) is stored starting from 0x1e00 and
			// each screen occupy about 512 bytes. The colour data, instead, is
			// stored in one single space ad 0x9600.
			#define SCREEN_RAM_START_ADDRESS	0x1e00
			#define SCREEN_RAM_SIZE				0x0200
			#define COLOUR_RAM_START_ADDRESS	0x9600
			#define COLOUR_RAM_SIZE_MASK		0x0000

			// The memory area useful as an auxiliary screen
			#define		AUXILIARY_SCREEN		16

		#endif

		// The startup (BASIC) screen is 0.
		#define		DEFAULT_SCREEN			 0

		// There is space for 1 screens.
		#define		MAX_SCREENS				 1

		// The four screens used for double buffering
		#define		SCREEN_DB1				0
		#define		SCREEN_DB2				0

		// Offset for correct brightness.
		#define COLOR_BRIGHTNESS			 0

		// Default palette.
		#define MR_COLOR_BLACK				 0
		#define MR_COLOR_WHITE				 1
		#define MR_COLOR_RED				 2
		#define MR_COLOR_CYAN				 3
		#define MR_COLOR_VIOLET				 4
		#define MR_COLOR_GREEN				 5
		#define MR_COLOR_BLUE				 6
		#define MR_COLOR_YELLOW				 7

		// Fallback palette.
		#define MR_COLOR_ORANGE				 MR_COLOR_YELLOW
		#define MR_COLOR_BROWN				 MR_COLOR_RED
		#define MR_COLOR_LIGHT_RED			 MR_COLOR_RED
		#define MR_COLOR_DARK_GREY			 MR_COLOR_BLACK
		#define MR_COLOR_GREY				 MR_COLOR_WHITE
		#define MR_COLOR_LIGHT_GREEN		 MR_COLOR_GREEN
		#define MR_COLOR_LIGHT_BLUE		 	 MR_COLOR_BLUE
		#define MR_COLOR_LIGHT_GREY			 MR_COLOR_WHITE

	#elif __C16__

		/******************************************************************
		 * ------ COMMODORE 16 PLATFORM
		 ******************************************************************/

		// The maximum resolution is 80 x 50 pixels, equivalent to 
		// 40 x 25 mixels.
		#define SCREEN_WIDTH				40
		#define SCREEN_HEIGHT				25

		// Luminance data (on/off pixel) is stored starting from 0x0000 and
		// each screen occupy 1.024 bytes. The colour data, instead, is
		// stored in one single space ad 0xd800.
		#define SCREEN_RAM_START_ADDRESS	0x0400
		#define SCREEN_RAM_SIZE				0x0800
		#define COLOUR_RAM_START_ADDRESS	0x0000
		#define COLOUR_RAM_SIZE_MASK		0xff00

		// The startup (BASIC) screen is 1.
		#define		DEFAULT_SCREEN			 1

		// The memory area useful as an auxiliary screen
		#define		AUXILIARY_SCREEN		 10

		// There is space for 16 screens.
		#define		MAX_SCREENS				 4

		// The four screens used for double buffering
		#define		SCREEN_DB1				1
		#define		SCREEN_DB2				2

		// Offset for correct brightness.
		#define COLOR_BRIGHTNESS			6<<4

		// Default palette.
		#define MR_COLOR_BLACK				 0
		#define MR_COLOR_WHITE				 1
		#define MR_COLOR_RED				 2
		#define MR_COLOR_CYAN				 3
		#define MR_COLOR_PURPLE				 4
		#define MR_COLOR_GREEN				 5
		#define MR_COLOR_BLUE				 6
		#define MR_COLOR_YELLOW				 7
		#define MR_COLOR_ORANGE				 8
		#define MR_COLOR_BROWN				 9
		#define MR_COLOR_YELLOW_GREEN		10
		#define MR_COLOR_PINK				11
		#define MR_COLOR_BLUE_GREEN			12
		#define MR_COLOR_LIGHT_BLUE			13
		#define MR_COLOR_DARK_BLUE			14
		#define MR_COLOR_LIGHT_GREEN		15

	#else

		/******************************************************************
	     * ------ UNSUPPORTED PLATFORM
	     ******************************************************************/

		#define SCREEN_WIDTH				40
		#define SCREEN_HEIGHT				25
		#define SCREEN_RAM_START_ADDRESS	0x0000
		#define SCREEN_RAM_SIZE				0x0000
		#define COLOUR_RAM_START_ADDRESS	0x0000
		#define COLOUR_RAM_SIZE_MASK		0x0000

		#define	DEFAULT_SCREEN				0
		#define AUXILIARY_SCREEN			0
		#define	MAX_SCREENS					1
		#define COLOR_BRIGHTNESS			0
		#define	SCREEN_DB1					0
		#define	SCREEN_DB2					1

		#define MR_COLOR_BLACK				0
		#define MR_COLOR_WHITE				0
		#define MR_COLOR_RED				0
		#define MR_COLOR_CYAN				0
		#define MR_COLOR_VIOLET				0
		#define MR_COLOR_GREEN				0
		#define MR_COLOR_BLUE				0
		#define MR_COLOR_YELLOW				0

		#define MR_COLOR_ORANGE				0
		#define MR_COLOR_BROWN				0
		#define MR_COLOR_LIGHT_RED			0
		#define MR_COLOR_DARK_GREY			0
		#define MR_COLOR_GREY				0
		#define MR_COLOR_LIGHT_GREEN		0
		#define MR_COLOR_LIGHT_BLUE			0
		#define MR_COLOR_LIGHT_GREY			0

	#endif

	/************************************************************************
	 * ------ FUNCTIONS
	 ************************************************************************/

	// Hardware dependent initialization.
	void mr_init_hd();

	// Hardware dependent screen management.
	void mr_show_hd(unsigned char _screen);

	// Hardware dependent cleanup.
	void mr_cleanup_hd();

	// Hardware dependend waiting for VBL
	void mr_wait_vbl();

	// Hardware dependent double buffer switch
	void mr_doublebuffer_switch_hd(unsigned char _screen);


#endif