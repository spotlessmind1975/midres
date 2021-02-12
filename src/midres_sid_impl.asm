;/*****************************************************************************
; * MIDRES Library - an isomorphic gamelib for retrocomputers                 *
; *****************************************************************************
; * Copyright 2020-2021 Marco Spedaletti (asimov@mclink.it)
; *
; * Licensed under the Apache License, Version 2.0 (the "License");
; * you may not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; * http://www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an "AS IS" BASIS,
; * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; *----------------------------------------------------------------------------
; * Concesso in licenza secondo i termini della Licenza Apache, versione 2.0
; * (la "Licenza"); è proibito usare questo file se non in conformità alla
; * Licenza. Una copia della Licenza è disponibile all'indirizzo:
; *
; * http://www.apache.org/licenses/LICENSE-2.0
; *
; * Se non richiesto dalla legislazione vigente o concordato per iscritto,
; * il software distribuito nei termini della Licenza è distribuito
; * "COSÌ COM'È", SENZA GARANZIE O CONDIZIONI DI ALCUN TIPO, esplicite o
; * implicite. Consultare la Licenza per il testo specifico che regola le
; * autorizzazioni e le limitazioni previste dalla medesima.
; ****************************************************************************/

.IF .DEFINED(__C64__) || .DEFINED(__C128__)

.export _mr_sid_init
.export _mr_sid_set_frequency

.import popa, popax
.importzp ptr1, ptr2

src = ptr1 ; borrow runtime zp pointers
dst = ptr2 ; tmp1 and tmp2 are also available

channel = ptr2
freq = ptr1

previous_lo:        .byte   0
previous_hi:        .byte   0

frequencies:        .byte   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
used:               .byte   0, 0
usedt:              .byte   0
slot:               .byte   0
slott:              .byte   0

.code

_mr_sid_set_frequency:
	sta freq
	stx freq + 1;
	jsr popa
    tay
    lda freq
	sta frequencies,y
    inx
    lda freq+1
	sta frequencies+1,y
    cpy #$8
    bcs calculate_used2_init
    lda #$1
    cpy #$0
    beq calculation_used_done
calculate_used:
    dey
    beq calculation_used_done
    asl
    jmp calculate_used
calculation_used_done:
    ora used
    jmp sid_set_frequency_done
    lda 1
calculate_used2_init:
    tya
    sbc 9
    tay
    lda 1
calculate_used2:
    dey
    beq calculation_used2_done
    asl
    jmp calculate_used2
calculation_used2_done:
    ora used+1
sid_set_frequency_done:
	rts

; +--------------------------------------------------------------------------+
; | Interrupt Service Routine (for SID)
; +--------------------------------------------------------------------------+

isr:
    ; Save registers for later restoring.
    pha
    txa
    pha
    tya
    pha

    lda slot
    bne slot1

slot0:

    lda used
    and $01
    beq slot01

    lda frequencies
    sta $d400
    lda frequencies+1
    sta $d401

slot01:

    lda used
    and $02
    beq slot02

    lda frequencies+2
    sta $d408
    lda frequencies+3
    sta $d409

slot02:

    lda used
    and $04
    beq done0

    lda frequencies+4
    sta $d415
    lda frequencies+5
    sta $d416

done0:
    jmp done

slot1:

    sta slott
    dec slott
    bne slot2

    lda used
    and $08
    beq slot11

    lda frequencies+6
    sta $d400
    lda frequencies+7
    sta $d401

slot11:

    lda used
    and $10
    beq slot12

    lda frequencies+8
    sta $d408
    lda frequencies+9
    sta $d409

slot12:

    lda used
    and $20
    beq done1

    lda frequencies+10
    sta $d415
    lda frequencies+11
    sta $d416

done1:
    jmp done

slot2:

    dec slott
    bne slot3

    lda used
    and $40
    beq slot21

    lda frequencies+12
    sta $d400
    lda frequencies+13
    sta $d401

slot21:

    lda used
    and $80
    beq slot22

    lda frequencies+14
    sta $d408
    lda frequencies+15
    sta $d409

slot22:

    lda used+1
    and $01
    beq done2

    lda frequencies+16
    sta $d415
    lda frequencies+17
    sta $d416

done2:
    jmp done

slot3:

    lda used+1
    and $02
    beq slot31

    lda frequencies+18
    sta $d400
    lda frequencies+19
    sta $d401

slot31:

    lda used+1
    and $04
    beq slot32

    lda frequencies+20
    sta $d408
    lda frequencies+21
    sta $d409

slot32:

    lda used+1
    and $08
    beq done3

    lda frequencies+22
    sta $d415
    lda frequencies+23
    sta $d416

done3:
    jmp done

slot4:

    lda used+1
    and $10
    beq slot41

    lda frequencies+24
    sta $d400
    lda frequencies+25
    sta $d401

slot41:

    lda used+1
    and $20
    beq slot42

    lda frequencies+26
    sta $d408
    lda frequencies+27
    sta $d409

slot42:

    lda used+1
    and $08
    beq done4

    lda frequencies+22
    sta $d415
    lda frequencies+23
    sta $d416

done4:
    jmp done

done:

    inc slot
    cmp 4
    bcs donef
    lda 0
    sta slot

donef:

    ; Restore registers.
    pla
    tay
    pla
    tax
    pla

    ; Jump to original routine.
    jmp (previous_lo)

_mr_sid_init:
	sei

.IFDEF __xxC64__

    ldx #32
    lda #$E0
    sta $FC
    ldy #$00
    sty $FB

loop:
    lda ($FB),y
    sta ($FB),y

    iny
    bne loop

    inc $FC
    dex
    bne loop

    lda $fffe
    sta previous_lo
    lda $ffff
    sta previous_hi

    lda #$35
    sta $01

.ENDIF

    lda $0314
    sta previous_lo
    lda $0315
    sta previous_hi

    lda #<isr
    sta $0314
    lda #>isr
    sta $0315

    cli
    rts

.ENDIF