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