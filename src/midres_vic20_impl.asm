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

.IF .DEFINED(__VIC20__) || .DEFINED(__VIC2024__)

.export _mr_memcpy_filtered
.export _mr_memfill4
.import         popax, popptr1
.importzp       sp, ptr1, ptr2, ptr3

.code

_mr_memcpy_filtered:
        sei
        jsr     mr_memcpy_filtered_getparams

memcpy_upwards:                 ; assert Y = 0
        ldx     ptr3+1          ; Get high byte of n
        beq     L2              ; Jump if zero

L1:     
        lda     (ptr1),Y        ; copy a byte
        cmp     #$ff
        beq     L1b
        sta     (ptr2),Y
L1b:
        iny
        beq     L1e

L1c:     
        lda     (ptr1),Y        ; copy a byte
        cmp     #$ff
        beq     L1d
        sta     (ptr2),Y
L1d:
        iny        
        bne     L1
L1e:
        inc     ptr1+1
        inc     ptr2+1
        dex                     ; Next 256 byte block
        bne     L1              ; Repeat if any

        ; the following section could be 10% faster if we were able to copy
        ; back to front - unfortunately we are forced to copy strict from
        ; low to high since this function is also used for
        ; memmove and blocks could be overlapping!
        ; {
L2:                             ; assert Y = 0
        ldx     ptr3            ; Get the low byte of n
        beq     done            ; something to copy

L3:     lda     (ptr1),Y        ; copy a byte
        cmp     #$ff
        beq     L3b
        sta     (ptr2),Y
L3b:
        iny
        dex
        bne     L3

        ; }

done:   
        inc     sp
        inc     sp
        cli
        rts

; ----------------------------------------------------------------------
; Get the parameters from stack as follows:
;
;       size            --> ptr3
;       src             --> ptr1
;       dest            --> ptr2
;       First argument (dest) will remain on stack and is returned in a/x!

mr_memcpy_filtered_getparams:               ; IMPORTANT! Function has to leave with Y=0!
        sta     ptr3
        stx     ptr3+1          ; save n to ptr3

        jsr     popptr1         ; save src to ptr1

                                ; save dest to ptr2
        iny                     ; Y=0 guaranteed by popptr1, we need '1' here...                        
                                ; (direct stack access is three cycles faster
                                ; (total cycle count with return))
        lda     (sp),y
        tax
        stx     ptr2+1          ; save high byte of ptr2
        dey                     ; Y = 0
        lda     (sp),y          ; Get ptr2 low
        sta     ptr2
        rts

_mr_memfill4:
        sei
        jsr     mr_memfill4_getparams

memfill_upwards:                 ; assert Y = 0
        ldx     ptr3+1          ; Get high byte of n
        beq     F2              ; Jump if zero

F1:     
        lda     (ptr1),Y        ; copy a byte
        sta     (ptr2),Y
        iny
        lda     (ptr1),Y        ; copy a byte
        sta     (ptr2),Y
        iny
        lda     (ptr1),Y        ; copy a byte
        sta     (ptr2),Y
        iny
        lda     (ptr1),Y        ; copy a byte
        sta     (ptr2),Y
        iny
        ldy     #0
        inc     ptr2
        bne     F1b
        inc     ptr2+1
F1b:
        inc     ptr2
        bne     F1c
        inc     ptr2+1
F1c:
        inc     ptr2
        bne     F1d
        inc     ptr2+1
F1d:
        inc     ptr2
        bne     F1e
        inc     ptr2+1
F1e:
        dex                     ; Next 256 byte block
        bne     F1              ; Repeat if any

        ; the following section could be 10% faster if we were able to copy
        ; back to front - unfortunately we are forced to copy strict from
        ; low to high since this function is also used for
        ; memmove and blocks could be overlapping!
        ; {
F2:                             ; assert Y = 0
        ldx     ptr3            ; Get the low byte of n
        beq     done2            ; something to copy

F2a:
        lda     (ptr1),Y        ; copy a byte
        sta     (ptr2),Y
        iny
        lda     (ptr1),Y        ; copy a byte
        sta     (ptr2),Y
        iny
        lda     (ptr1),Y        ; copy a byte
        sta     (ptr2),Y
        iny
        lda     (ptr1),Y        ; copy a byte
        sta     (ptr2),Y
        ldy     #0
        inc     ptr2
        bne     F2b
        inc     ptr2+1
F2b:
        inc     ptr2
        bne     F2c
        inc     ptr2+1
F2c:
        inc     ptr2
        bne     F2d
        inc     ptr2+1
F2d:
        inc     ptr2
        bne     F2e
        inc     ptr2+1
F2e:
        dex
        bne     F2a

        ; }

done2:  
        inc     sp
        inc     sp
        cli
        rts

; ----------------------------------------------------------------------
; Get the parameters from stack as follows:
;
;       size            --> ptr3
;       src             --> ptr1
;       dest            --> ptr2
;       First argument (dest) will remain on stack and is returned in a/x!

mr_memfill4_getparams:               ; IMPORTANT! Function has to leave with Y=0!
        sta     ptr3
        stx     ptr3+1          ; save n to ptr3

        jsr     popptr1         ; save src to ptr1

                                ; save dest to ptr2
        iny                     ; Y=0 guaranteed by popptr1, we need '1' here...                        
                                ; (direct stack access is three cycles faster
                                ; (total cycle count with return))
        lda     (sp),y
        tax
        stx     ptr2+1          ; save high byte of ptr2
        dey                     ; Y = 0
        lda     (sp),y          ; Get ptr2 low
        sta     ptr2
        rts

.ENDIF