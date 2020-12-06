; /****************************************************************************
;  * midres - Portable midres library for retrocomputers                      *
;  *                                                                          *
;  * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
;  *--------------------------------------------------------------------------*
;  * DRAWING LIBRARY                                                          *
;  ****************************************************************************/

PUBLIC _io_put
PUBLIC _io_get

_io_put:
	ld	hl, 2
	add	hl, sp
    ld a,(hl)               ; value
    inc hl
    inc hl
    ld c,(hl)               ; port
    inc hl
    inc hl
    out (c), a
    ret

_io_get:
	ld	hl, 2
	add	hl, sp
    ld c,(hl)               ; port
    in a, (c)
    ld l, a
    ret

