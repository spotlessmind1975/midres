;/*****************************************************************************
; * MIDRES Library - an isomorphic gamelib for retrocomputers                 *
; *****************************************************************************
; * Copyright 2020 Marco Spedaletti (asimov@mclink.it)
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

.IF .DEFINED(__C16__) || .DEFINED(__PLUS4__)

.export isr
.export _mr_ted_init
.export _mr_ted_set_frequency

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

_mr_ted_set_frequency:
	sta freq
	stx freq + 1;
	jsr popa
    tay
    lda freq
	sta frequencies,y
    inx
    lda freq+1
    and #$3
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
    sta $ff0e
    lda $ff12
    and #$fc
    ora frequencies+1
    sta $ff12

slot01:

    lda used
    and $02
    beq done0

    lda frequencies+2
    sta $ff0f
    lda $ff10
    and #$fc
    ora frequencies+3
    sta $ff10

done0:
    jmp done

slot1:

    sta slott
    dec slott
    bne slot2

    lda used
    and $08
    beq slot11

    lda frequencies+4
    sta $ff0e
    lda $ff12
    and #$fc
    ora frequencies+5
    sta $ff12

slot11:

    lda used
    and $10
    beq done1

    lda frequencies+6
    sta $ff0f
    lda $ff10
    and #$fc
    ora frequencies+7
    sta $ff10

done1:
    jmp done

slot2:

    dec slott
    bne slot3

    lda used
    and $40
    beq slot21

    lda frequencies+8
    sta $ff0e
    lda $ff12
    and #$fc
    ora frequencies+9
    sta $ff12

slot21:

    lda used
    and $80
    beq done2

    lda frequencies+10
    sta $ff0f
    lda $ff10
    and #$fc
    ora frequencies+11
    sta $ff10

done2:
    jmp done

slot3:

    lda used+1
    and $02
    beq slot31

    lda frequencies+12
    sta $ff0e
    lda $ff12
    and #$fc
    ora frequencies+13
    sta $ff12

slot31:

    lda used+1
    and $04
    beq done3

    lda frequencies+14
    sta $ff0f
    lda $ff10
    and #$fc
    ora frequencies+15
    sta $ff10

done3:
    jmp done

slot4:

    lda used+1
    and $10
    beq slot41

    lda frequencies+16
    sta $ff0e
    lda $ff12
    and #$fc
    ora frequencies+17
    sta $ff12

slot41:

    lda used+1
    and $20
    beq done4

    lda frequencies+18
    sta $ff0f
    lda $ff10
    and #$fc
    ora frequencies+19
    sta $ff10

done4:
    jmp done

done:

    inc slot
    lda slot
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

_mr_ted_init:
	sei

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