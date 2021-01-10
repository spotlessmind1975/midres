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

.IF .DEFINED(__ATARI__) || .DEFINED(__ATARILO__)

.export _mr_pokey_init
.export _mr_pokey_set_frequency

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

_mr_pokey_set_frequency:
	sta freq
	jsr popa
    tay
    lda freq
	sta frequencies,y
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
    jmp pokey_set_frequency_done
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
pokey_set_frequency_done:
	rts

; +--------------------------------------------------------------------------+
; | Interrupt Service Routine (for pokey)
; +--------------------------------------------------------------------------+

isr:
    ; Save registers for later restoring.
    ;pha
    ;txa
    ;pha
    ;tya
    ;pha

    lda slot
    bne slot1

slot0:

    lda used
    and $01
    beq slot01

    lda frequencies
    sta $d200

slot01:

    lda used
    and $02
    beq slot02

    lda frequencies+1
    sta $d202

slot02:

    lda used
    and $04
    beq done0

    lda frequencies+2
    sta $d204

slot03:

    lda used
    and $08
    beq done0

    lda frequencies+3
    sta $d206

done0:
    jmp done

slot1:

    sta slott
    dec slott
    bne slot2

    lda used
    and $10
    beq slot11

    lda frequencies+4
    sta $d200

slot11:

    lda used
    and $20
    beq slot12

    lda frequencies+5
    sta $d202

slot12:

    lda used
    and $40
    beq slot13

    lda frequencies+6
    sta $d204

slot13:

    lda used
    and $80
    beq done1

    lda frequencies+7
    sta $d206

done1:
    jmp done

slot2:

    dec slott
    bne slot3

    lda used+1
    and $01
    beq slot21

    lda frequencies+8
    sta $d200

slot21:

    lda used
    and $02
    beq slot22

    lda frequencies+9
    sta $d202

slot22:

    lda used+1
    and $04
    beq slot23

    lda frequencies+10
    sta $d204

slot23:

    lda used+1
    and $08
    beq done2

    lda frequencies+11
    sta $d206

done2:
    jmp done

slot3:

    lda used+1
    and $10
    beq slot31

    lda frequencies+12
    sta $d200

slot31:

    lda used+1
    and $20
    beq slot32

    lda frequencies+13
    sta $d202

slot32:

    lda used+1
    and $40
    beq slot33

    lda frequencies+14
    sta $d204

slot33:

    lda used+1
    and $80
    beq done3

    lda frequencies+15
    sta $d206

done3:
    jmp done

done:

    inc slot
    cmp 3
    bcs donef
    lda 0
    sta slot

donef:

    ; Restore registers.
    ;pla
    ;tay
    ;pla
    ;tax
    ;pla

    ; Jump to original routine.
    jmp $E462

_mr_pokey_init:
	sei

    lda #<isr
    sta $0224
    lda #>isr
    sta $0225

    cli
    rts

.ENDIF