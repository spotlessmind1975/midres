; /****************************************************************************
;  * midres - Portable midres library for retrocomputers                      *
;  *                                                                          *
;  * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
;  *--------------------------------------------------------------------------*
;  * DRAWING LIBRARY                                                          *
;  ****************************************************************************/

PUBLIC _mr_vdp_port_read
PUBLIC _mr_vdp_port_write
PUBLIC _mr_vdp_port
PUBLIC _mr_vdp_out
PUBLIC _mr_vdp_put
PUBLIC _mr_vdp_put8
PUBLIC _mr_vdp_fill8
PUBLIC _mr_vdp_fill
PUBLIC _mr_vdp_get

VdpWriteBit: equ     40h

VdpSetReg:
        call    VdpRegOut
        ld      a, e
VdpRegOut:
        push    bc
        ld      bc, (VdpPortWrite)
        inc     c
        out     (c), a
        pop     bc
        ret

VdpRegIn:
        push    bc
        ld      bc, (VdpPortRead)
        inc     c
        in      a, (c)
        pop     bc
        ret

VdpRamOut:
        di
        push    bc
        ld      bc, (VdpPortWrite)
        out     (c), a
        pop     bc
        ei
        ret

VdpRamOut8:
        di
        push    bc
        ld      bc, (VdpPortWrite)
        out     (c), a
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        out     (c), a
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        out     (c), a
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        out     (c), a
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        out     (c), a
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        out     (c), a
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        out     (c), a
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        out     (c), a
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        pop     bc
        ei
        ret

VdpRamIn:
        push    bc
        ld      bc, (VdpPortRead)
        in      a, (c)
        pop     bc
        ret

VdpWriteAddr:
        ld      a, e                    ; send lsb
        call    VdpRegOut
        ld      a, d                    ; mask off msb to max of 16KB
        and     3fh
        or      VdpWriteBit             ; indicate that this is a write
        call    VdpRegOut
        ret

VdpReadAddr:
        ld      a, e                    ; send lsb
        call    VdpRegOut
        ld      a, d                    ; mask off msb to max of 16KB
        and     3Fh
        call    VdpRegOut
        ret

VdpWrite:
        di
        call    VdpWriteAddr
VdpWriteLoop:
        ld      a, (hl)
        call    VdpRamOut
        inc     hl
        dec     bc
        ld      a, b
        or      c
        jp      nz, VdpWriteLoop
        ei
        ret

VdpWrite8:
        di
        call    VdpWriteAddr
VdpWrite8Loop:
        ld      a, (hl)
        call    VdpRamOut8
        inc     hl
        dec     bc
        ld      a, b
        or      c
        jp      nz, VdpWrite8Loop
        ei
        ret

VdpRead:
        di
        call    VdpReadAddr
        call    VdpRamIn
        ld      h, a
        ld      l, a
        ei
        ret

VdpFill:
        di
        push    af
        call    VdpWriteAddr
        pop     af
VdpFillLoop:
        call    VdpRamOut
        dec     c
        jp      nz, VdpFillLoop
        djnz    VdpFillLoop
        ei
        ret

VdpFill8:
        di
        push    af
        call    VdpWriteAddr
        pop     af
VdpFillLoop8:
        call    VdpRamOut
        dec     c
        jp      nz, VdpFillLoop8
        ei
        ret

_mr_vdp_out:
	ld	hl, 2
	add	hl, sp
	ld	a, (hl)			; val
	inc 	hl
	inc 	hl
	ld	e, (hl)			; reg
	jp 	VdpSetReg

_mr_vdp_in:
	call	VdpRegIn
	ret

_mr_vdp_put:
	ld	hl, 2
	add	hl, sp
    ld c,(hl)
    inc hl
    ld b,(hl)              ; byte count
    inc hl
    ld e,(hl)
    inc hl
    ld d,(hl)              ; vram destination address
    inc hl
    ld a, (hl)
    inc hl
    ex af,af'
    ld a, (hl)
    inc hl
    ld h, a
    ex af,af'
    ld l, a
    jp VdpWrite

_mr_vdp_put8:
	ld	hl, 2
	add	hl, sp
    ld c,(hl)
    inc hl
    ld b,(hl)              ; byte count
    inc hl
    ld e,(hl)
    inc hl
    ld d,(hl)              ; vram destination address
    inc hl
    ld a, (hl)
    inc hl
    ex af,af'
    ld a, (hl)
    inc hl
    ld h, a
    ex af,af'
    ld l, a
    jp VdpWrite8

_mr_vdp_fill8:
	ld	hl, 2
	add	hl, sp
    ld c,(hl)
    inc hl
    ld b,(hl)              ; byte count
    inc hl
    ld e,(hl)
    inc hl
    ld d,(hl)              ; vram destination address
    inc hl
	ld	a, (hl)			    ; value to fill
    jp VdpFill8

_mr_vdp_fill:
	ld	hl, 2
	add	hl, sp
    ld c,(hl)
    inc hl
    ld b,(hl)              ; byte count
    inc hl
    ld e,(hl)
    inc hl
    ld d,(hl)              ; vram destination address
    inc hl
	ld	a, (hl)			    ; value to fill
    jp VdpFill

_mr_vdp_get:
	ld	hl, 2
	add	hl, sp
    ld e,(hl)
    inc hl
    ld d,(hl)              ; vram source address
    jp VdpRead

_mr_vdp_port:
	ld	hl, 2
	add	hl, sp
	ld	a, (hl)			; port
    ld (VdpPortRead), a
    ld (VdpPortWrite), a
    ret

_mr_vdp_port_read:
	ld	hl, 2
	add	hl, sp
	ld	a, (hl)			; port
    ld (VdpPortRead), a
    ret

_mr_vdp_port_write:
	ld	hl, 2
	add	hl, sp
	ld	a, (hl)			; port
    ld (VdpPortWrite), a
    ret

SECTION data
VdpPortRead:
    defb    0beh
VdpPortWrite:
    defb    0beh

