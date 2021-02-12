; /***************************************************************************
; *  MIDRES Library - an isomorphic gamelib for retrocomputers               *
; ****************************************************************************
; *  Copyright 2020-2021 Marco Spedaletti (asimov@mclink.it)
; * 
; *  Licensed under the Apache License, Version 2.0 (the "License");
; *  you may not use this file except in compliance with the License.
; *  You may obtain a copy of the License at
; * 
; *  http://www.apache.org/licenses/LICENSE-2.0
; * 
; *  Unless required by applicable law or agreed to in writing, software
; *  distributed under the License is distributed on an "AS IS" BASIS,
; *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; *  See the License for the specific language governing permissions and
; *  limitations under the License.
; *---------------------------------------------------------------------------
; *  Concesso in licenza secondo i termini della Licenza Apache, versione 2.0
; *  (la "Licenza"); � proibito usare questo file se non in conformit� alla
; *  Licenza. Una copia della Licenza � disponibile all'indirizzo:
; * 
; *  http://www.apache.org/licenses/LICENSE-2.0
; * 
; *  Se non richiesto dalla legislazione vigente o concordato per iscritto,
; *  il software distribuito nei termini della Licenza � distribuito
; *  "COS� COM'�", SENZA GARANZIE O CONDIZIONI DI ALCUN TIPO, esplicite o
; *  implicite. Consultare la Licenza per il testo specifico che regola le
; *  autorizzazioni e le limitazioni previste dalla medesima.
; ****************************************************************************/

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

