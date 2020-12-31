<?php

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

?>
###############################################################################
## FINAL RULES
###############################################################################

$(LIBDIR):
	$(call MKDIR,$@)

$(EXEDIR):
	$(call MKDIR,$@)

$(EXEDIR)/atr:
	$(call MKDIR,$@)

$(TARGETOBJDIR):
	$(call MKDIR,$@)

$(DATADIR):
	$(call MKDIR,$@)

library: $(LIBDIR) $(TARGETOBJDIR) $(LIBS)
	awk '/([mM][rR]_[a-zA-Z0-9]+)/i { for(i=1; i<=NF; i++) { tmp=match($$i,/^([mM][rR]\_[\_a-zA-Z0-9]+)/); if (tmp) { print substr($$i,RSTART, RLENGTH); } } }' src/*.h | sort | uniq >midres_symbols.txt

clean-library: 
	$(foreach LIB,$(LIBS),$(call RMFILES,$(LIB)))

all: $(LIBDIR) $(EXEDIR) $(EXEDIR)/atr $(TARGETOBJDIR) $(LIBS) $(EXES)

probe:
	$(CC) -t $(target) -o probe src/probe.c

probe80:
	$(ASM88) -D__SCCZ80 -m -s -mz80 -omidres_io.o src/midres_io.asm
	$(CC88) +$(target) -D__Z80__ src/probe.c midres_io.o -create-app

clean:
	$(call RMFILES,$(EXEDIR)/midres.c64.d64)
	$(call RMFILES,$(EXEDIR)/midres.vic20.d64)
	$(call RMFILES,$(EXEDIR)/midres.vic2024.d64)
	$(call RMFILES,$(EXEDIR)/midres.c16.d64)
	$(call RMFILES,$(EXEDIR)/midres.plus4.d64)
	$(call RMFILES,$(EXEDIR)/midres.atari.atr)
	$(call RMFILES,$(EXEDIR)/midres.c64)
	$(call RMFILES,$(EXEDIR)/midres.c64ovl*)
	$(call RMFILES,$(EXEDIR)/midres.vic20)
	$(call RMFILES,$(EXEDIR)/midres.vic2024)
	$(call RMFILES,$(EXEDIR)/midres.vic20ovl*)
	$(call RMFILES,$(EXEDIR)/midres.c16)
	$(call RMFILES,$(EXEDIR)/midres.plus4)
	$(call RMFILES,$(EXEDIR)/midres.atari)
	$(call RMFILES,$(EXEDIR)/midres.atmos.tap)
	$(call RMFILES,$(EXEDIR)/airattack.c64)
	$(call RMFILES,$(EXEDIR)/airattack.c64.d64)
	$(call RMFILES,$(EXEDIR)/airattack.vic2024)
	$(call RMFILES,$(EXEDIR)/airattack.vic2024.d64)
	$(call RMFILES,$(EXEDIR)/airattack.plus4)
	$(call RMFILES,$(EXEDIR)/airattack.plus4.d64)
	$(call RMFILES,$(EXEDIR)/airattack.c128)
	$(call RMFILES,$(EXEDIR)/airattack.c128.d64)
	$(call RMFILES,$(EXEDIR)/airattack.atari)
	$(call RMFILES,$(EXEDIR)/airattack.atari.atr)
	$(call RMFILES,$(EXEDIR)/totto.c64)
	$(call RMFILES,$(EXEDIR)/totto.c64.d64)
	$(call RMFILES,$(EXEDIR)/totto.plus4)
	$(call RMFILES,$(EXEDIR)/totto.plus4.d64)
	$(call RMFILES,$(EXEDIR)/totto.vic2024)
	$(call RMFILES,$(EXEDIR)/totto.vic2024.d64)
	$(call RMFILES,$(EXEDIR)/totto.atari.atr)
	$(call RMFILES,$(EXEDIR)/alienstorm.c64)
	$(call RMFILES,$(EXEDIR)/alienstorm.c64.d64)
	$(call RMFILES,$(EXEDIR)/alienstorm.plus4)
	$(call RMFILES,$(EXEDIR)/alienstorm.plus4.d64)
	$(call RMFILES,$(EXEDIR)/alienstorm.vic2024)
	$(call RMFILES,$(EXEDIR)/alienstorm.vic2024.d64)
	$(call RMFILES,$(EXEDIR)/alienstorm.atari.atr)
	$(call RMFILES,$(EXEDIR)/joycheck.c64)
	$(call RMFILES,$(EXEDIR)/joycheck.c64.d64)
	$(call RMFILES,$(EXEDIR)/joycheck.plus4)
	$(call RMFILES,$(EXEDIR)/joycheck.plus4.d64)
	$(call RMFILES,$(EXEDIR)/joycheck.vic2024)
	$(call RMFILES,$(EXEDIR)/joycheck.vic2024.d64)
	$(call RMFILES,$(EXEDIR)/joycheck.c128)
	$(call RMFILES,$(EXEDIR)/joycheck.c128.d64)
	$(call RMFILES,$(EXEDIR)/joycheck.atari.atr)
	$(call RMFILES,$(EXEDIR)/elevator.c64)
	$(call RMFILES,$(EXEDIR)/elevator.c64.d64)
	$(call RMFILES,$(EXEDIR)/elevator.plus4)
	$(call RMFILES,$(EXEDIR)/elevator.plus4.d64)
	$(call RMFILES,$(EXEDIR)/elevator.vic2024)
	$(call RMFILES,$(EXEDIR)/elevator.vic2024.d64)
	$(call RMFILES,$(EXEDIR)/elevator.c128)
	$(call RMFILES,$(EXEDIR)/elevator.c128.d64)
	$(call RMFILES,$(EXEDIR)/elevator.atari.atr)
	$(call RMFILES,$(LIBDIR)/midres.c64.lib)
	$(call RMFILES,$(LIBDIR)/midres.vic2024.lib)
	$(call RMFILES,src/rawdata.h)
	$(call RMFILES,src/rawdata.c)
	$(foreach EXE,$(EXES),$(call RMFILES,$(EXE)))
	$(foreach OBJECT,$(OBJECTS),$(call RMFILES,$(OBJECT)))
	$(foreach OBJECT,$(LIB_OBJECTS),$(call RMFILES,$(OBJECT)))
