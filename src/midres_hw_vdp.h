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