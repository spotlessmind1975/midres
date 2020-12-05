#define __COLECO__ 1
#ifdef __COLECO__

/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * DRAWING LIBRARY                                                          *
 ****************************************************************************/

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>
#include <string.h>

#include "midres.h"
#include "rawdata.h"

#define VDP_R0          0x80
#define VDP_R1          0x81
#define VDP_RNAME       0x82
#define VDP_RCOLORTABLE 0x83
#define VDP_RPATTERN    0x84
#define VDP_RSPRITEA    0x85
#define VDP_RSPRITEP    0x86
#define VDP_RCOLOR      0x87

unsigned char MR_RENDERED_MIXELS_COLECO[16] = {
   0x00, 0x01, 0x02, 0x03,
   0x04, 0x05, 0x06, 0x07,
   0x08, 0x09, 0x10, 0x11,
   0x12, 0x13, 0x14, 0x15
};

  /****************************************************************************
   ** RESIDENT VARIABLES SECTION
   ****************************************************************************/

   /****************************************************************************
    ** RESIDENT FUNCTIONS SECTION
    ****************************************************************************/

    // The functions defined at this level can only be called up if the current
    // module has been loaded into memory. On the other hand, they can call any 
    // function declared at the resident module level.

void mr_init_hd() {

    vdp_out(VDP_R0, 0x00);
    vdp_out(VDP_R1, 0xc0);
    vdp_out(VDP_RNAME, 0x05);
    vdp_out(VDP_RCOLORTABLE, 0x80);
    vdp_out(VDP_RPATTERN, 0x01);
    vdp_out(VDP_RSPRITEA, 0x20);
    vdp_out(VDP_RSPRITEP, 0x00);
    vdp_out(VDP_RCOLOR, 0x04);

    vdp_put(&_includedFiles[0][0], 0x0800, 128);

}

void mr_init_multicolor_hd() {

    vdp_out(VDP_R0, 0x00);
    vdp_out(VDP_R1, 0xc0);
    vdp_out(VDP_RNAME, 0x05);
    vdp_out(VDP_RCOLORTABLE, 0x80);
    vdp_out(VDP_RPATTERN, 0x01);
    vdp_out(VDP_RSPRITEA, 0x20);
    vdp_out(VDP_RSPRITEP, 0x00);
    vdp_out(VDP_RCOLOR, 0x04);

    vdp_put(&_includedFiles[0][0], 0x0800, 128);

}

void mr_tile_setcolor_hd(unsigned char _index, unsigned char _color) {

}

void mr_tile_setcolors_hd(unsigned char _colors[4]) {

}

void mr_show_hd(unsigned char _screen) {

}

void mr_cleanup_hd() {

}

void mr_wait_vbl() {

}

void mr_doublebuffer_switch_hd(unsigned char _screen) {

}

void mr_tileset_visible_hd(unsigned char _tileset) {

}

unsigned char mr_key_pressed_hd() {

}

unsigned char mr_get_key_pressed_hd() {

}

void mr_wait_hd(unsigned char _seconds) {

}

void mr_wait_jiffies_hd(unsigned char _jiffies) {

}

// Hardware dependent sound library
void mr_sound_start_hd(unsigned char _channel, unsigned char _number) {

}

// Hardware dependent sound library
void mr_sound_change_hd(unsigned char _channel, int _parameter) {

}

// Hardware dependent sound library
void mr_sound_stop_hd(unsigned char _channel) {

}

void mr_set_background_color_hd(unsigned char _color) {

}

void mr_set_border_color_hd(unsigned char _color) {

}

unsigned char storedJiffy = 0;

void mr_start_frame_hd() {

}

void mr_end_frame_hd(unsigned char _jiffies) {

}

#ifdef MIDRES_STANDALONE_FILE

unsigned char* mr_translate_file_hd(mr_file _file) {

}

void mr_read_file_hd(unsigned int _file, unsigned int _offset, unsigned char* _dest, unsigned int _size) {

}

#endif

unsigned char mr_joy_hd(unsigned char _number) {

}

#endif