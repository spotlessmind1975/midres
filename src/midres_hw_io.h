/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

#ifndef _MIDRES_HW_IO_H_
#define _MIDRES_HW_IO_H_

#include <stdint.h>

extern void io_put(uint8_t _port, uint8_t _val);
extern uint8_t io_get(uint16_t _port);

#endif