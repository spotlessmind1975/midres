/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

#ifndef _MIDRES_HW_VDP_H_
#define _MIDRES_HW_VDP_H_

#include <stdint.h>

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
extern void mr_vdp_out(uint8_t reg, uint8_t val);
extern uint8_t mr_vdp_in();
extern void mr_vdp_put(uint8_t* _src, uint16_t _dst, uint16_t _count);
extern void mr_vdp_put8(uint8_t* _src, uint16_t _dst, uint16_t _count);
extern void mr_vdp_fill8(uint8_t _value, uint16_t _dst, uint8_t _count);
extern void mr_vdp_fill(uint8_t _value, uint16_t _dst, uint16_t _count);
extern uint8_t mr_vdp_get(uint16_t _src);

#endif