/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * KEYBOARD LIBRARY                                                         *
 ****************************************************************************/

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "midres.h"

#if ( !defined(__OVERLAY__MIDRES__) && defined(MIDRES_STANDALONE_FILE) ) || defined(__OVERLAY__MIDRES__)

 // The functions defined at this level can only be called up if the current
 // module has been loaded into memory. On the other hand, they can call any 
 // function declared at the resident module level.

unsigned char* mr_translate_file(mr_file _file) {
	return mr_translate_file_hd(_file);
}

void mr_read_file(mr_file _file, unsigned char* _dest, unsigned int _size) {
	mr_read_file_hd(_file, 0, _dest, _size);
}

void mr_read_file_from_offset(mr_file _file, unsigned int _offset, unsigned char* _dest, unsigned int _size) {
	mr_read_file_hd(_file, _offset, _dest, _size);
}

#endif