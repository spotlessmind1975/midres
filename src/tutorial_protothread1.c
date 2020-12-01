#ifdef __TUTORIAL_PROTOTHREAD1__

/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * RESIDENT MODULE                                                          *
 ****************************************************************************/

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

  // Needed for system call.
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// Needed for MIDRES library call.
#include "midres.h"

mr_protothread firstThread;
mr_protothread secondThread;

MR_PT_THREAD(first) {

	MR_PTI_BEGIN();

	while (1) {

		printf("Hello world 1\n");

		MR_PTI_YIELD();

	}

	MR_PTI_END();
}

MR_PT_THREAD(second) {

	MR_PTI_BEGIN();

	while (1) {

		printf("Hello world 2\n");

		MR_PTI_YIELD();

	}

	MR_PTI_END();
}

void tutorial_protothread1() {

	while (1) {
		first(&firstThread);
		second(&secondThread);
	}

}

#endif
