#/****************************************************************************
#* MIDRES Library - an isomorphic gamelib for retrocomputers                 *
#*****************************************************************************
#* Copyright 2020 Marco Spedaletti (asimov@mclink.it)
#* 
#* Licensed under the Apache License, Version 2.0 (the "License");
#* you may not use this file except in compliance with the License.
#* You may obtain a copy of the License at
#* 
#* http://www.apache.org/licenses/LICENSE-2.0
#* 
#* Unless required by applicable law or agreed to in writing, software
#* distributed under the License is distributed on an "AS IS" BASIS,
#* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#* See the License for the specific language governing permissions and
#* limitations under the License.
#*----------------------------------------------------------------------------
#* Concesso in licenza secondo i termini della Licenza Apache, versione 2.0 
#* (la "Licenza"); è proibito usare questo file se non in conformità alla 
#* Licenza. Una copia della Licenza è disponibile all'indirizzo:
#* 
#* http://www.apache.org/licenses/LICENSE-2.0
#* 
#* Se non richiesto dalla legislazione vigente o concordato per iscritto, 
#* il software distribuito nei termini della Licenza è distribuito 
#* "COSÌ COM'È", SENZA GARANZIE O CONDIZIONI DI ALCUN TIPO, esplicite o 
#* implicite. Consultare la Licenza per il testo specifico che regola le 
#* autorizzazioni e le limitazioni previste dalla medesima.
#****************************************************************************/
###############################################################################
## This makefile was created with the aim of generalizing the compilation of 
## a source for different 8-bit environments. It follows that, before you can 
## use it, you must indicate which environments you are interested in obtaining
## an executable for.
###############################################################################

# Allowed targets:

#  MIDRES LIBRARY (with DEMO, see below)
#  - c64: single executable for Commodore 64 (named on disk: "midres-single")
#  - c64ovl: overlayed executable for Commodore 64 (named on disk: "midres")
#  - vic20: single executable for unexpanded VIC 20 (named on disk: "midres-single")
#  - vic20ovl: overlayed executable for Commodore VIC20 (named on disk: "midres")
#  - vic2024: single executable for 24K VIC 20 (named on disk: "midres-single")
#  - c16: single executable for Commodore 16 (named on disk: "midres-single")
#  - plus4: single executable for Plus 4 (named on disk: "midres-single")
#  - c128: single executable for Commodore 128 (named on disk: "midres-single")
#  - atari: single executable for ATARI
#  - atarilo: single executable for ATARI (low res)

ifdef target
LTARGETS ?= $(target)
endif

# Given demonstrations:
#  - SLIDESHOW - a slideshow with some images converted using img2midres
#  - DRAWING - an animation using drawing primitives (v1.1)
#  - BITBLIT - an animation using bit blits primivites (v1.2)
#  - TILE - an animation using tiles primivites (v1.3)
#  - JOY - joystick feedback (v1.61)

ifdef demo
DEMO := $(demo)
ATARGETS ?= midres.$(demo).$(target)
ifdef target
TARGETS ?= $(target)
endif
endif

# Given tutorials:
#  - MCTILE
ifdef tutorial
TUTORIAL := $(tutorial)
ifdef target
TARGETS ?= $(target)
endif
endif

# Given PROGRAMS:
#  - totto
#  - airattack
#  - alienstorm
#  - elevator
#  - joycheck

ifdef program
ifdef target
ATARGETS ?= $(program).$(target)
LTARGETS ?= $(target)
endif
endif

###############################################################################
###############################################################################
###############################################################################

###############################################################################
## UNIX / WINDOWS ENVIROMENT
###############################################################################
# The "Native Win32" GNU Make contains quite some workarounds to get along with
# cmd.exe as shell. However it does not provide means to determine that it does
# actually activate those workarounds. Especially $(SHELL) does NOT contain the
# value 'cmd.exe'. So the usual way to determine if cmd.exe is being used is to
# execute the command 'echo' without any parameters. Only cmd.exe will return a
# non-empty string - saying 'ECHO is on/off'.
#
# Many "Native Win32" programs accept '/' as directory delimiter just fine. How-
# ever the internal commands of cmd.exe generally require '\' to be used.
#
# cmd.exe has an internal command 'mkdir' that doesn't understand nor require a
# '-p' to create parent directories as needed.
#
# cmd.exe has an internal command 'del' that reports a syntax error if executed
# without any file so make sure to call it only if there's an actual argument.
ifeq ($(shell echo),)
  MKDIR = mkdir -p $1
  RMDIR = rmdir $1
  RMFILES = $(RM) $1
  COPYFILES = cp $1 $2
else
  MKDIR = mkdir $(subst /,\,$1)
  RMDIR = $(if exist $(subst /,\,$1),rmdir $(subst /,\,$1))
  RMFILES = $(if exist $(subst /,\,$1), del /f /q $(subst /,\,$1))
  COPYFILES = copy $(subst /,\,$1) $(subst /,\,$2)
endif
COMMA := ,
SPACE := $(N/A) $(N/A)
define NEWLINE


endef
# Note: Do not remove any of the two empty lines above !

# On Windows it is mandatory to have CC65_HOME set. So do not unnecessarily
# rely on cl65 being added to the PATH in this scenario.
ifdef CC65_HOME
  CC := $(CC65_HOME)/bin/cl65
  AR := $(CC65_HOME)/bin/ar65
  ASM := $(CC65_HOME)/bin/ca65
else
  CC := cl65
  AR := ar65
  ASM := ca65
endif

# On Windows it is mandatory to have CC1541_HOME set. So do not unnecessarily
# rely on being added to the PATH in this scenario.
ifdef CC1541_HOME
  CC1541 := $(CC1541_HOME)/cc1541
else
  CC1541 := cc1541
endif

# On Windows it is mandatory to have DIR2ATR_HOME set. So do not unnecessarily
# rely on being added to the PATH in this scenario.
ifdef DIR2ATR_HOME
  DIR2ATR := $(DIR2ATR_HOME)/dir2atr
else
  DIR2ATR := dir2atr
endif

# On Windows it is mandatory to have ATRAUTORUN_HOME set. So do not unnecessarily
# rely on being added to the PATH in this scenario.
ifdef ATRAUTORUN_HOME
  ATRAUTORUN := $(ATRAUTORUN_HOME)/atrautorun
else
  ATRAUTORUN := atrautorun
endif

# On Windows it is mandatory to have FILE2INCLUDE_HOME set. So do not unnecessarily
# rely on being added to the PATH in this scenario.
ifdef FILE2INCLUDE_HOME
  FILE2INCLUDE := $(FILE2INCLUDE_HOME)/file2include64
else
  FILE2INCLUDE := file2include64
endif

# On Windows it is mandatory to have ZCCCFG set. So do not unnecessarily
# rely on z88dk being added to the PATH in this scenario.
ifdef ZCCCFG
  CC88 := $(ZCCCFG)/../../bin/zcc
  ASM88 := $(ZCCCFG)/../../bin/z80asm
else
  CC88 := zcc
  ASM88 := z80asm
endif

###############################################################################
## COMPILATION / LINKING OPTIONS
###############################################################################

CFLAGS := -D__DEMO_$(DEMO)__ -D__GAME_$(GAME)__ -D__TUTORIAL_$(TUTORIAL)__ -W -const-comparison
CFLAGS88 := -D__Z80__ -O3
LDFLAGS := 
LDFLAGS88 :=
CRT :=
REMOVES :=

# Compiler flags used to tell the compiler to optimise for SPEED
define _optspeed_
  CFLAGS += -Oris
endef

# Compiler flags used to tell the compiler to optimise for SIZE
define _optsize_
  CFLAGS += -Or
endef

# Linker flags for generating map file
define _mapfile_
  LDFLAGS += --mapfile $$@.map
  REMOVES += $(PROGRAM).map
endef

# Linker flags for generating VICE label file
define _labelfile_
  LDFLAGS += -Ln $$@.lbl
  REMOVES += $(PROGRAM).lbl
endef

# Linker flags for generating a debug file
define _debugfile_
  LDFLAGS += -Wl --dbgfile,$$@.dbg
  REMOVES += $(PROGRAM).dbg
endef

###############################################################################
## MAKEFILE'S "CORE"
###############################################################################

# This is the set of files that will have to be analyzed. Let's consider, at 
# the moment, only the C sources are since he ASM ones are specific for each 
# environment.
SOURCES := $(wildcard src/main.c)
SOURCES += $(wildcard src/demo_*.c)
SOURCES += $(wildcard src/game_*.c)
SOURCES += $(wildcard src/tutorial_*.c)
SOURCES += $(wildcard src/utility_*.c)

LIB_SOURCES := $(wildcard src/midres.c)
LIB_SOURCES += $(wildcard src/midres_*.c)
LIB_INCLUDES:= $(wildcard src/midres.h)
LIB_INCLUDES+= $(wildcard src/midres_*.h)

# Let's calculate what the names of the object files could be. Usually, there 
# will be one for each source. Object files are stored in a separate location 
# for each target environment. 
OBJS := $(addsuffix .o,$(basename $(addprefix obj/PLATFORM/,$(SOURCES:src/%=%))))
LIB_OBJS := $(addsuffix .o,$(basename $(addprefix obj/PLATFORM/,$(LIB_SOURCES:src/%=%))))

# Here we expand every single object produced, according to each expected 
# environment. In this way you get the complete list of all object files to be 
# compiled separately, each according to the compiler suitable for that 
# environment.
OBJECTS := $(foreach TARGET,$(TARGETS),$(subst PLATFORM,$(TARGET),$(OBJS)))
OBJECTS += $(foreach ATARGET,$(ATARGETS),$(subst PLATFORM,$(ATARGET),$(OBJS)))
LIB_OBJECTS := $(foreach TARGET,$(TARGETS),$(subst PLATFORM,$(TARGET),$(LIB_OBJS)))

# We generate the list of paths where the object files for each target will 
# end, so that we can generate them in advance (as paths).
TARGETOBJDIR := $(foreach TARGET,$(TARGETS),obj/$(TARGET))
TARGETOBJDIR += $(foreach ATARGET,$(ATARGETS),obj/$(ATARGET))
TARGETOBJDIR += $(foreach TARGET,$(LTARGETS),obj/$(TARGET))

# This is the path where all executables will be put.
EXEDIR := exe

# This is the path where all libraries will be put.
LIBDIR := lib

# Similarly, we expand the set of executables that are required.
#EXES := $(foreach TARGET,$(TARGETS),$(EXEDIR)/midres.$(TARGET))
EXES += $(foreach ATARGET,$(ATARGETS),$(EXEDIR)/$(ATARGET))

LIBS := $(foreach TARGET,$(TARGETS),$(LIBDIR)/midres.$(TARGET).lib)
LIBS += $(foreach TARGET,$(LTARGETS),$(LIBDIR)/midres.$(TARGET).lib)

# This is the path where all data will be put.
DATADIR := data

###############################################################################
## TARGETS' RULES
###############################################################################

# Makefile generated for following targets: atari, c128, c16, c64, coleco, gb, lm80c, msx, mtx500, plus4, svi, vic20, vic2024
# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR ATARI 
# -------------------------------------------------------------------

obj/atari/midres_sid_impl.o:	src/midres_sid_impl.asm
	$(ASM) -t atari -oobj/atari/midres_sid_impl.o src/midres_sid_impl.asm

obj/atari/midres_ted_impl.o:	src/midres_ted_impl.asm
	$(ASM) -t atari -oobj/atari/midres_ted_impl.o src/midres_ted_impl.asm

obj/atari/midres_pokey_impl.o:	src/midres_pokey_impl.asm
	$(ASM) -t atari -oobj/atari/midres_pokey_impl.o src/midres_pokey_impl.asm

obj/atari/%.o:	$(LIB_INCLUDES) $(LIB_SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t atari -c $(CFLAGS)  -Osir -Cl   -o $@ $(subst obj/atari/,src/,$(@:.o=.c))

$(LIBDIR)/midres.atari.lib:	$(LIB_INCLUDES) $(subst PLATFORM,atari,$(LIB_OBJS)) obj/atari/midres_sid_impl.o obj/atari/midres_ted_impl.o obj/atari/midres_pokey_impl.o
	$(AR) r $(LIBDIR)/midres.atari.lib $(subst PLATFORM,atari,$(LIB_OBJS)) obj/atari/midres_sid_impl.o obj/atari/midres_ted_impl.o obj/atari/midres_pokey_impl.o

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR ATARI 
# -------------------------------------------------------------------

.PHONY: midres.MUSIC.embedded.atari midres.MUSIC.atari
midres.MUSIC.embedded.atari:
	$(FILE2INCLUDE) -i $(DATADIR)/slideshowa.dat -n slideshow -i $(DATADIR)/imagea01.pic -n imagea01.mpic -i $(DATADIR)/imagea02.pic -n imagea02.mpic -i $(DATADIR)/imagea03.pic -n imagea03.mpic -i $(DATADIR)/imagea04.pic -n imagea04.mpic -i $(DATADIR)/ztiles.bin -n ztiles.bin -i $(DATADIR)/tiles.bin -n tiles.bin -i $(DATADIR)/tutorial_mctile.bin -n mctile.bin -i $(DATADIR)/testcard.bin -n testcard.bin -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/toccatina.imf -n toccatina.imf -i $(DATADIR)/island.imf -n island.imf -i $(DATADIR)/tetris.imf -n tetris.imf -c src/rawdata.c -h src/rawdata.h

obj/atari/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t atari -c $(CFLAGS)  -Osir -Cl  -o $@ $(subst obj/atari/,src/,$(@:.o=.c))

$(EXEDIR)/midres.MUSIC.atari: midres.MUSIC.embedded.atari $(subst PLATFORM,atari,$(OBJS))
	$(CC) -Ln demoatari.lbl -t atari -C cfg/atari.cfg  $(LDFLAGS) -m $(EXEDIR)/midres.atari.map -o $(EXEDIR)/midres.atari $(subst PLATFORM,atari,$(OBJS)) $(LIBDIR)/midres.atari.lib
	$(call RMFILES,$(EXEDIR)/atr/*.*)
	$(call COPYFILES,$(DIR2ATR_HOME)/dos25/dos.sys,$(EXEDIR)/atr/dos.sys)
	$(call COPYFILES,$(EXEDIR)/midres.atari,$(EXEDIR)/atr/midres.exe)
	$(call COPYFILES,$(DATADIR)/slideshowa.dat,$(EXEDIR)/atr/slideshow)
	$(call COPYFILES,$(DATADIR)/imagea01.pic,$(EXEDIR)/atr/imagea01.mpic)
	$(call COPYFILES,$(DATADIR)/imagea02.pic,$(EXEDIR)/atr/imagea02.mpic)
	$(call COPYFILES,$(DATADIR)/imagea03.pic,$(EXEDIR)/atr/imagea03.mpic)
	$(call COPYFILES,$(DATADIR)/imagea04.pic,$(EXEDIR)/atr/imagea04.mpic)
	$(call COPYFILES,$(DATADIR)/ztiles.bin,$(EXEDIR)/atr/ztiles.bin)
	$(call COPYFILES,$(DATADIR)/tiles.bin,$(EXEDIR)/atr/tiles.bin)
	$(call COPYFILES,$(DATADIR)/tutorial_mctile.bin,$(EXEDIR)/atr/mctile.bin)
	$(call COPYFILES,$(DATADIR)/testcard.bin,$(EXEDIR)/atr/testcard.bin)
	$(call COPYFILES,$(DATADIR)/zdjtiles.bin,$(EXEDIR)/atr/zdjtiles.bin)
	$(call COPYFILES,$(DATADIR)/zeltiles.bin,$(EXEDIR)/atr/zeltiles.bin)
	$(call COPYFILES,$(DATADIR)/toccatina.imf,$(EXEDIR)/atr/toccatina.imf)
	$(call COPYFILES,$(DATADIR)/island.imf,$(EXEDIR)/atr/island.imf)
	$(call COPYFILES,$(DATADIR)/tetris.imf,$(EXEDIR)/atr/tetris.imf)
	$(DIR2ATR) -S -p -B $(DIR2ATR_HOME)/dos25/bootcode $(EXEDIR)/midres.atari.atr $(EXEDIR)/atr
	$(ATRAUTORUN) -i $(EXEDIR)/midres.atari.atr -o $(EXEDIR)/midres.atari.atr -f midres.exe

# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR C128 
# -------------------------------------------------------------------

obj/c128/midres_sid_impl.o:	src/midres_sid_impl.asm
	$(ASM) -t c128 -oobj/c128/midres_sid_impl.o src/midres_sid_impl.asm

obj/c128/midres_ted_impl.o:	src/midres_ted_impl.asm
	$(ASM) -t c128 -oobj/c128/midres_ted_impl.o src/midres_ted_impl.asm

obj/c128/midres_pokey_impl.o:	src/midres_pokey_impl.asm
	$(ASM) -t c128 -oobj/c128/midres_pokey_impl.o src/midres_pokey_impl.asm

obj/c128/%.o:	$(LIB_INCLUDES) $(LIB_SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c128 -c $(CFLAGS)  -Osir -Cl -D__CBM__  -o $@ $(subst obj/c128/,src/,$(@:.o=.c))

$(LIBDIR)/midres.c128.lib:	$(LIB_INCLUDES) $(subst PLATFORM,c128,$(LIB_OBJS)) obj/c128/midres_sid_impl.o obj/c128/midres_ted_impl.o obj/c128/midres_pokey_impl.o
	$(AR) r $(LIBDIR)/midres.c128.lib $(subst PLATFORM,c128,$(LIB_OBJS)) obj/c128/midres_sid_impl.o obj/c128/midres_ted_impl.o obj/c128/midres_pokey_impl.o

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR C128 
# -------------------------------------------------------------------

.PHONY: midres.MUSIC.embedded.c128 midres.MUSIC.c128
midres.MUSIC.embedded.c128:
	$(FILE2INCLUDE) -i $(DATADIR)/slideshow64.dat -n slideshow -i $(DATADIR)/image6401.mpic -n image6401.mpic -i $(DATADIR)/image6402.mpic -n image6402.mpic -i $(DATADIR)/image6403.mpic -n image6403.mpic -i $(DATADIR)/image6404.mpic -n image6404.mpic -i $(DATADIR)/tiles.bin -n tiles.bin -i $(DATADIR)/aatiles.bin -n aatiles.bin -i $(DATADIR)/aaintro64.mpic -n aaintro.mpic -i $(DATADIR)/tutorial_mctile.bin -n mctile.bin -i $(DATADIR)/testcard.bin -n testcard.bin -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/toccatina.imf -n toccatina.imf -i $(DATADIR)/alice.imf -n alice.imf -i $(DATADIR)/island.imf -n island.imf -i $(DATADIR)/tetris.imf -n tetris.imf -c src/rawdata.c -h src/rawdata.h

obj/c128/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c128 -c $(CFLAGS)  -Osir -Cl -D__CBM__ -o $@ $(subst obj/c128/,src/,$(@:.o=.c))

$(EXEDIR)/midres.MUSIC.c128: midres.MUSIC.embedded.c128 $(subst PLATFORM,c128,$(OBJS))
	$(CC) -Ln democ128.lbl -t c128 -C cfg/c128.cfg  $(LDFLAGS) -m $(EXEDIR)/midres.c128.map -o $(EXEDIR)/midres.c128 $(subst PLATFORM,c128,$(OBJS)) $(LIBDIR)/midres.c128.lib
	$(call RMFILES,$(EXEDIR)/midres.c128.d64)
	$(CC1541) -f midres-single -w $(EXEDIR)/midres.c128 $(EXEDIR)/midres.c128.d64 
	$(CC1541) -f slideshow -w $(DATADIR)/slideshow64.dat $(EXEDIR)/midres.c128.d64 
	$(CC1541) -f image6401.mpic -w $(DATADIR)/image6401.mpic $(EXEDIR)/midres.c128.d64 
	$(CC1541) -f image6402.mpic -w $(DATADIR)/image6402.mpic $(EXEDIR)/midres.c128.d64 
	$(CC1541) -f image6403.mpic -w $(DATADIR)/image6403.mpic $(EXEDIR)/midres.c128.d64 
	$(CC1541) -f image6404.mpic -w $(DATADIR)/image6404.mpic $(EXEDIR)/midres.c128.d64 
	$(CC1541) -f tiles.bin -w $(DATADIR)/tiles.bin $(EXEDIR)/midres.c128.d64 
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles.bin $(EXEDIR)/midres.c128.d64 
	$(CC1541) -f aaintro.mpic -w $(DATADIR)/aaintro64.mpic $(EXEDIR)/midres.c128.d64 
	$(CC1541) -f mctile.bin -w $(DATADIR)/tutorial_mctile.bin $(EXEDIR)/midres.c128.d64 
	$(CC1541) -f testcard.bin -w $(DATADIR)/testcard.bin $(EXEDIR)/midres.c128.d64 
	$(CC1541) -f zdjtiles.bin -w $(DATADIR)/zdjtiles.bin $(EXEDIR)/midres.c128.d64 
	$(CC1541) -f zeltiles.bin -w $(DATADIR)/zeltiles.bin $(EXEDIR)/midres.c128.d64 
	$(CC1541) -f toccatina.imf -w $(DATADIR)/toccatina.imf $(EXEDIR)/midres.c128.d64 
	$(CC1541) -f alice.imf -w $(DATADIR)/alice.imf $(EXEDIR)/midres.c128.d64 
	$(CC1541) -f island.imf -w $(DATADIR)/island.imf $(EXEDIR)/midres.c128.d64 
	$(CC1541) -f tetris.imf -w $(DATADIR)/tetris.imf $(EXEDIR)/midres.c128.d64 


# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR C16 
# -------------------------------------------------------------------

obj/c16/midres_sid_impl.o:	src/midres_sid_impl.asm
	$(ASM) -t c16 -oobj/c16/midres_sid_impl.o src/midres_sid_impl.asm

obj/c16/midres_ted_impl.o:	src/midres_ted_impl.asm
	$(ASM) -t c16 -oobj/c16/midres_ted_impl.o src/midres_ted_impl.asm

obj/c16/midres_pokey_impl.o:	src/midres_pokey_impl.asm
	$(ASM) -t c16 -oobj/c16/midres_pokey_impl.o src/midres_pokey_impl.asm

obj/c16/%.o:	$(LIB_INCLUDES) $(LIB_SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c16 -c $(CFLAGS)  -Osir -Cl -D__CBM__  -o $@ $(subst obj/c16/,src/,$(@:.o=.c))

$(LIBDIR)/midres.c16.lib:	$(LIB_INCLUDES) $(subst PLATFORM,c16,$(LIB_OBJS)) obj/c16/midres_sid_impl.o obj/c16/midres_ted_impl.o obj/c16/midres_pokey_impl.o
	$(AR) r $(LIBDIR)/midres.c16.lib $(subst PLATFORM,c16,$(LIB_OBJS)) obj/c16/midres_sid_impl.o obj/c16/midres_ted_impl.o obj/c16/midres_pokey_impl.o

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR C16 
# -------------------------------------------------------------------

.PHONY: midres.MUSIC.embedded.c16 midres.MUSIC.c16
midres.MUSIC.embedded.c16:
	$(FILE2INCLUDE) -i $(DATADIR)/slideshow16.dat -n slideshow -i $(DATADIR)/image1601.mpic -n image1601.mpic -i $(DATADIR)/image1602.mpic -n image1602.mpic -i $(DATADIR)/image1603.mpic -n image1603.mpic -i $(DATADIR)/image1604.mpic -n image1604.mpic -i $(DATADIR)/tiles.bin -n tiles.bin -i $(DATADIR)/aatiles4.bin -n aatiles.bin -i $(DATADIR)/aaintro16.mpic -n aaintro.mpic -i $(DATADIR)/tutorial_mctile.bin -n mctile.bin -i $(DATADIR)/testcard.bin -n testcard.bin -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/toccatina.imf -n toccatina.imf -i $(DATADIR)/alice.imf -n alice.imf -i $(DATADIR)/island.imf -n island.imf -i $(DATADIR)/tetris.imf -n tetris.imf -c src/rawdata.c -h src/rawdata.h

obj/c16/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c16 -c $(CFLAGS)  -Osir -Cl -D__CBM__ -o $@ $(subst obj/c16/,src/,$(@:.o=.c))

$(EXEDIR)/midres.MUSIC.c16: midres.MUSIC.embedded.c16 $(subst PLATFORM,c16,$(OBJS))
	$(CC) -Ln democ16.lbl -t c16 -C cfg/c16.cfg  $(LDFLAGS) -m $(EXEDIR)/midres.c16.map -o $(EXEDIR)/midres.c16 $(subst PLATFORM,c16,$(OBJS)) $(LIBDIR)/midres.c16.lib
	$(call RMFILES,$(EXEDIR)/midres.c16.d64)
	$(CC1541) -f midres-single -w $(EXEDIR)/midres.c16 $(EXEDIR)/midres.c16.d64 
	$(CC1541) -f  -w  $(EXEDIR)/midres.c16.d64 
	$(CC1541) -f slideshow -w $(DATADIR)/slideshow16.dat $(EXEDIR)/midres.c16.d64 
	$(CC1541) -f image1601.mpic -w $(DATADIR)/image1601.mpic $(EXEDIR)/midres.c16.d64 
	$(CC1541) -f image1602.mpic -w $(DATADIR)/image1602.mpic $(EXEDIR)/midres.c16.d64 
	$(CC1541) -f image1603.mpic -w $(DATADIR)/image1603.mpic $(EXEDIR)/midres.c16.d64 
	$(CC1541) -f image1604.mpic -w $(DATADIR)/image1604.mpic $(EXEDIR)/midres.c16.d64 
	$(CC1541) -f tiles.bin -w $(DATADIR)/tiles.bin $(EXEDIR)/midres.c16.d64 
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles4.bin $(EXEDIR)/midres.c16.d64 
	$(CC1541) -f aaintro.mpic -w $(DATADIR)/aaintro16.mpic $(EXEDIR)/midres.c16.d64 
	$(CC1541) -f mctile.bin -w $(DATADIR)/tutorial_mctile.bin $(EXEDIR)/midres.c16.d64 
	$(CC1541) -f testcard.bin -w $(DATADIR)/testcard.bin $(EXEDIR)/midres.c16.d64 
	$(CC1541) -f zdjtiles.bin -w $(DATADIR)/zdjtiles.bin $(EXEDIR)/midres.c16.d64 
	$(CC1541) -f zeltiles.bin -w $(DATADIR)/zeltiles.bin $(EXEDIR)/midres.c16.d64 
	$(CC1541) -f toccatina.imf -w $(DATADIR)/toccatina.imf $(EXEDIR)/midres.c16.d64 
	$(CC1541) -f alice.imf -w $(DATADIR)/alice.imf $(EXEDIR)/midres.c16.d64 
	$(CC1541) -f island.imf -w $(DATADIR)/island.imf $(EXEDIR)/midres.c16.d64 
	$(CC1541) -f tetris.imf -w $(DATADIR)/tetris.imf $(EXEDIR)/midres.c16.d64 


# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR C64 
# -------------------------------------------------------------------

obj/c64/midres_sid_impl.o:	src/midres_sid_impl.asm
	$(ASM) -t c64 -oobj/c64/midres_sid_impl.o src/midres_sid_impl.asm

obj/c64/midres_ted_impl.o:	src/midres_ted_impl.asm
	$(ASM) -t c64 -oobj/c64/midres_ted_impl.o src/midres_ted_impl.asm

obj/c64/midres_pokey_impl.o:	src/midres_pokey_impl.asm
	$(ASM) -t c64 -oobj/c64/midres_pokey_impl.o src/midres_pokey_impl.asm

obj/c64/%.o:	$(LIB_INCLUDES) $(LIB_SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c64 -c $(CFLAGS)  -Osir -Cl -D__CBM__  -o $@ $(subst obj/c64/,src/,$(@:.o=.c))

$(LIBDIR)/midres.c64.lib:	$(LIB_INCLUDES) $(subst PLATFORM,c64,$(LIB_OBJS)) obj/c64/midres_sid_impl.o obj/c64/midres_ted_impl.o obj/c64/midres_pokey_impl.o
	$(AR) r $(LIBDIR)/midres.c64.lib $(subst PLATFORM,c64,$(LIB_OBJS)) obj/c64/midres_sid_impl.o obj/c64/midres_ted_impl.o obj/c64/midres_pokey_impl.o

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR C64 
# -------------------------------------------------------------------

.PHONY: midres.MUSIC.embedded.c64 midres.MUSIC.c64
midres.MUSIC.embedded.c64:
	$(FILE2INCLUDE) -i $(DATADIR)/slideshow64.dat -n slideshow -i $(DATADIR)/image6401.mpic -n image6401.mpic -i $(DATADIR)/image6402.mpic -n image6402.mpic -i $(DATADIR)/image6403.mpic -n image6403.mpic -i $(DATADIR)/image6404.mpic -n image6404.mpic -i $(DATADIR)/tiles.bin -n tiles.bin -i $(DATADIR)/aatiles.bin -n aatiles.bin -i $(DATADIR)/aaintro64.mpic -n aaintro.mpic -i $(DATADIR)/tutorial_mctile.bin -n mctile.bin -i $(DATADIR)/testcard.bin -n testcard.bin -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/toccatina.imf -n toccatina.imf -i $(DATADIR)/alice.imf -n alice.imf -i $(DATADIR)/island.imf -n island.imf -i $(DATADIR)/tetris.imf -n tetris.imf -c src/rawdata.c -h src/rawdata.h

obj/c64/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c64 -c $(CFLAGS)  -Osir -Cl -D__CBM__ -o $@ $(subst obj/c64/,src/,$(@:.o=.c))

$(EXEDIR)/midres.MUSIC.c64: midres.MUSIC.embedded.c64 $(subst PLATFORM,c64,$(OBJS))
	$(CC) -Ln democ64.lbl -t c64 -C cfg/c64.cfg  $(LDFLAGS) -m $(EXEDIR)/midres.c64.map -o $(EXEDIR)/midres.c64 $(subst PLATFORM,c64,$(OBJS)) $(LIBDIR)/midres.c64.lib
	$(call RMFILES,$(EXEDIR)/midres.c64.d64)
	$(CC1541) -f midres-single -w $(EXEDIR)/midres.c64 $(EXEDIR)/midres.c64.d64 
	$(CC1541) -f slideshow -w $(DATADIR)/slideshow64.dat $(EXEDIR)/midres.c64.d64 
	$(CC1541) -f image6401.mpic -w $(DATADIR)/image6401.mpic $(EXEDIR)/midres.c64.d64 
	$(CC1541) -f image6402.mpic -w $(DATADIR)/image6402.mpic $(EXEDIR)/midres.c64.d64 
	$(CC1541) -f image6403.mpic -w $(DATADIR)/image6403.mpic $(EXEDIR)/midres.c64.d64 
	$(CC1541) -f image6404.mpic -w $(DATADIR)/image6404.mpic $(EXEDIR)/midres.c64.d64 
	$(CC1541) -f tiles.bin -w $(DATADIR)/tiles.bin $(EXEDIR)/midres.c64.d64 
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles.bin $(EXEDIR)/midres.c64.d64 
	$(CC1541) -f aaintro.mpic -w $(DATADIR)/aaintro64.mpic $(EXEDIR)/midres.c64.d64 
	$(CC1541) -f mctile.bin -w $(DATADIR)/tutorial_mctile.bin $(EXEDIR)/midres.c64.d64 
	$(CC1541) -f testcard.bin -w $(DATADIR)/testcard.bin $(EXEDIR)/midres.c64.d64 
	$(CC1541) -f zdjtiles.bin -w $(DATADIR)/zdjtiles.bin $(EXEDIR)/midres.c64.d64 
	$(CC1541) -f zeltiles.bin -w $(DATADIR)/zeltiles.bin $(EXEDIR)/midres.c64.d64 
	$(CC1541) -f toccatina.imf -w $(DATADIR)/toccatina.imf $(EXEDIR)/midres.c64.d64 
	$(CC1541) -f alice.imf -w $(DATADIR)/alice.imf $(EXEDIR)/midres.c64.d64 
	$(CC1541) -f island.imf -w $(DATADIR)/island.imf $(EXEDIR)/midres.c64.d64 
	$(CC1541) -f tetris.imf -w $(DATADIR)/tetris.imf $(EXEDIR)/midres.c64.d64 


# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR COLECO 
# -------------------------------------------------------------------

lib/midres.coleco.lib:
# find_main_module(DEMO_MUSIC)
# -> src/demo_music.c

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR COLECO 
# -------------------------------------------------------------------

.PHONY: midres.MUSIC.embedded.coleco midres.MUSIC.coleco
midres.MUSIC.embedded.coleco:
	$(FILE2INCLUDE) -i $(DATADIR)/mtiles.bin -n mtiles.bin -i $(DATADIR)/tiles.bin -n tiles.bin -i $(DATADIR)/tutorial_mctile.bin -n tutorial_mctile.bin -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/toccatina.imf -n toccatina.imf -i $(DATADIR)/alice.imf -n alice.imf -i $(DATADIR)/island.imf -n island.imf -i $(DATADIR)/tetris.imf -n tetris.imf -c src/rawdata.c -h src/rawdata.h
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/coleco/rawdata.o src/rawdata.c

obj/coleco/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/coleco/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/coleco/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/coleco/midres_io.o src/midres_io.asm

obj/coleco/%.o:	$(SOURCES)
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o $@ $(subst obj/coleco/,src/,$(@:.o=.c))

$(EXEDIR)/midres.MUSIC.coleco:	midres.MUSIC.embedded.coleco $(subst PLATFORM,coleco,$(OBJS)) $(subst PLATFORM,coleco,$(LIB_OBJS)) obj/coleco/rawdata.o obj/coleco/midres_vdp_impl.o obj/coleco/midres_io.o
	$(CC88) +coleco  -m $(LDFLAGS88) obj/coleco/rawdata.o obj/coleco/midres_io.o obj/coleco/midres_vdp_impl.o obj/coleco/demo_music.o obj/coleco/demo_tile.o obj/coleco/game_air_attack.o obj/coleco/game_alien_storm.o obj/coleco/game_elevator.o obj/coleco/game_totto.o obj/coleco/midres.o obj/coleco/midres_atari.o obj/coleco/midres_atmos.o obj/coleco/midres_ay8910.o obj/coleco/midres_c128.o obj/coleco/midres_c16.o obj/coleco/midres_c64.o obj/coleco/midres_coleco.o obj/coleco/midres_control_000.o obj/coleco/midres_control_001.o obj/coleco/midres_control_002.o obj/coleco/midres_control_003.o obj/coleco/midres_control_004.o obj/coleco/midres_control_005.o obj/coleco/midres_control_007.o obj/coleco/midres_control_010.o obj/coleco/midres_control_011.o obj/coleco/midres_file_001.o obj/coleco/midres_file_003.o obj/coleco/midres_frame_000.o obj/coleco/midres_frame_001.o obj/coleco/midres_gb.o obj/coleco/midres_lm80c.o obj/coleco/midres_msx.o obj/coleco/midres_mtx500.o obj/coleco/midres_music_001.o obj/coleco/midres_music_002.o obj/coleco/midres_music_004.o obj/coleco/midres_music_005.o obj/coleco/midres_music_006.o obj/coleco/midres_plus4.o obj/coleco/midres_pokey.o obj/coleco/midres_screen_000.o obj/coleco/midres_screen_006.o obj/coleco/midres_screen_007.o obj/coleco/midres_screen_010.o obj/coleco/midres_screen_012.o obj/coleco/midres_screen_013.o obj/coleco/midres_sid.o obj/coleco/midres_sound_000.o obj/coleco/midres_sound_001.o obj/coleco/midres_sound_002.o obj/coleco/midres_sound_005.o obj/coleco/midres_sound_006.o obj/coleco/midres_sound_007.o obj/coleco/midres_svi.o obj/coleco/midres_ted.o obj/coleco/midres_tile_000.o obj/coleco/midres_tile_004.o obj/coleco/midres_tile_005.o obj/coleco/midres_tile_006.o obj/coleco/midres_tile_007.o obj/coleco/midres_tile_008.o obj/coleco/midres_tile_009.o obj/coleco/midres_tile_010.o obj/coleco/midres_tile_011.o obj/coleco/midres_tile_012.o obj/coleco/midres_tile_013.o obj/coleco/midres_tile_014.o obj/coleco/midres_tile_015.o obj/coleco/midres_tile_016.o obj/coleco/midres_tile_018.o obj/coleco/midres_tile_019.o obj/coleco/midres_tile_020.o obj/coleco/midres_tile_021.o obj/coleco/midres_tile_022.o obj/coleco/midres_tile_023.o obj/coleco/midres_tile_025.o obj/coleco/midres_tile_026.o obj/coleco/midres_tile_027.o obj/coleco/midres_tile_028.o obj/coleco/midres_tile_multicolor_000.o obj/coleco/midres_tile_multicolor_001.o obj/coleco/midres_tile_multicolor_002.o obj/coleco/midres_tile_multicolor_003.o obj/coleco/midres_tile_multicolor_004.o obj/coleco/midres_tile_multicolor_005.o obj/coleco/midres_tile_multicolor_006.o obj/coleco/midres_tile_multicolor_007.o obj/coleco/midres_tile_multicolor_008.o obj/coleco/midres_tile_multicolor_009.o obj/coleco/midres_tile_multicolor_010.o obj/coleco/midres_tile_multicolor_011.o obj/coleco/midres_tile_multicolor_012.o obj/coleco/midres_tile_processing_001.o obj/coleco/midres_tile_processing_002.o obj/coleco/midres_tile_processing_004.o obj/coleco/midres_tile_processing_005.o obj/coleco/midres_tile_processing_007.o obj/coleco/midres_tile_processing_009.o obj/coleco/midres_tile_processing_011.o obj/coleco/midres_tile_processing_012.o obj/coleco/midres_tile_processing_016.o obj/coleco/midres_vanilla.o obj/coleco/midres_vdp.o obj/coleco/midres_vic.o obj/coleco/midres_vic20.o obj/coleco/utility_joycheck.o obj/coleco/midres_data.o obj/coleco/main.o -create-app 
	$(call COPYFILES,a.rom,$(EXEDIR)/midres.coleco.rom)
	$(call RMFILES,a.*)

# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR GB 
# -------------------------------------------------------------------

lib/midres.gb.lib:
# find_main_module(DEMO_MUSIC)
# -> src/demo_music.c

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR GB 
# -------------------------------------------------------------------

.PHONY: midres.MUSIC.embedded.gb midres.MUSIC.gb
midres.MUSIC.embedded.gb:
	$(FILE2INCLUDE) -i $(DATADIR)/mtiles.bin -n mtiles.bin -i $(DATADIR)/tiles.bin -n tiles.bin -i $(DATADIR)/tutorial_mctile.bin -n tutorial_mctile.bin -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/toccatina.imf -n toccatina.imf -i $(DATADIR)/alice.imf -n alice.imf -i $(DATADIR)/island.imf -n island.imf -i $(DATADIR)/tetris.imf -n tetris.imf -c src/rawdata.c -h src/rawdata.h
	$(CC88) +gb $(CFLAGS) -c $(CFLAGS88)  -o obj/gb/rawdata.o src/rawdata.c

obj/gb/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/gb/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/gb/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/gb/midres_io.o src/midres_io.asm

obj/gb/%.o:	$(SOURCES)
	$(CC88) +gb $(CFLAGS) -c $(CFLAGS88)  -o $@ $(subst obj/gb/,src/,$(@:.o=.c))

$(EXEDIR)/midres.MUSIC.gb:	midres.MUSIC.embedded.gb $(subst PLATFORM,gb,$(OBJS)) $(subst PLATFORM,gb,$(LIB_OBJS)) obj/gb/rawdata.o obj/gb/midres_vdp_impl.o obj/gb/midres_io.o
	$(CC88) +gb  -m $(LDFLAGS88) obj/gb/rawdata.o obj/gb/midres_io.o obj/gb/midres_vdp_impl.o obj/gb/demo_music.o obj/gb/demo_tile.o obj/gb/game_air_attack.o obj/gb/game_alien_storm.o obj/gb/game_elevator.o obj/gb/game_totto.o obj/gb/midres.o obj/gb/midres_atari.o obj/gb/midres_atmos.o obj/gb/midres_ay8910.o obj/gb/midres_c128.o obj/gb/midres_c16.o obj/gb/midres_c64.o obj/gb/midres_coleco.o obj/gb/midres_control_000.o obj/gb/midres_control_001.o obj/gb/midres_control_002.o obj/gb/midres_control_003.o obj/gb/midres_control_004.o obj/gb/midres_control_005.o obj/gb/midres_control_007.o obj/gb/midres_control_010.o obj/gb/midres_control_011.o obj/gb/midres_file_001.o obj/gb/midres_file_003.o obj/gb/midres_frame_000.o obj/gb/midres_frame_001.o obj/gb/midres_gb.o obj/gb/midres_lm80c.o obj/gb/midres_msx.o obj/gb/midres_mtx500.o obj/gb/midres_music_001.o obj/gb/midres_music_002.o obj/gb/midres_music_004.o obj/gb/midres_music_005.o obj/gb/midres_music_006.o obj/gb/midres_plus4.o obj/gb/midres_pokey.o obj/gb/midres_screen_000.o obj/gb/midres_screen_006.o obj/gb/midres_screen_007.o obj/gb/midres_screen_010.o obj/gb/midres_screen_012.o obj/gb/midres_screen_013.o obj/gb/midres_sid.o obj/gb/midres_sound_000.o obj/gb/midres_sound_001.o obj/gb/midres_sound_002.o obj/gb/midres_sound_005.o obj/gb/midres_sound_006.o obj/gb/midres_sound_007.o obj/gb/midres_svi.o obj/gb/midres_ted.o obj/gb/midres_tile_000.o obj/gb/midres_tile_004.o obj/gb/midres_tile_005.o obj/gb/midres_tile_006.o obj/gb/midres_tile_007.o obj/gb/midres_tile_008.o obj/gb/midres_tile_009.o obj/gb/midres_tile_010.o obj/gb/midres_tile_011.o obj/gb/midres_tile_012.o obj/gb/midres_tile_013.o obj/gb/midres_tile_014.o obj/gb/midres_tile_015.o obj/gb/midres_tile_016.o obj/gb/midres_tile_018.o obj/gb/midres_tile_019.o obj/gb/midres_tile_020.o obj/gb/midres_tile_021.o obj/gb/midres_tile_022.o obj/gb/midres_tile_023.o obj/gb/midres_tile_025.o obj/gb/midres_tile_026.o obj/gb/midres_tile_027.o obj/gb/midres_tile_028.o obj/gb/midres_tile_multicolor_000.o obj/gb/midres_tile_multicolor_001.o obj/gb/midres_tile_multicolor_002.o obj/gb/midres_tile_multicolor_003.o obj/gb/midres_tile_multicolor_004.o obj/gb/midres_tile_multicolor_005.o obj/gb/midres_tile_multicolor_006.o obj/gb/midres_tile_multicolor_007.o obj/gb/midres_tile_multicolor_008.o obj/gb/midres_tile_multicolor_009.o obj/gb/midres_tile_multicolor_010.o obj/gb/midres_tile_multicolor_011.o obj/gb/midres_tile_multicolor_012.o obj/gb/midres_tile_processing_001.o obj/gb/midres_tile_processing_002.o obj/gb/midres_tile_processing_004.o obj/gb/midres_tile_processing_005.o obj/gb/midres_tile_processing_007.o obj/gb/midres_tile_processing_009.o obj/gb/midres_tile_processing_011.o obj/gb/midres_tile_processing_012.o obj/gb/midres_tile_processing_016.o obj/gb/midres_vanilla.o obj/gb/midres_vdp.o obj/gb/midres_vic.o obj/gb/midres_vic20.o obj/gb/utility_joycheck.o obj/gb/midres_data.o obj/gb/main.o -create-app 
	$(call COPYFILES,a.gb,$(EXEDIR)/midres.gb.gb)
	$(call RMFILES,a.*)

# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR LM80C 
# -------------------------------------------------------------------

lib/midres.lm80c.lib:
# find_main_module(DEMO_MUSIC)
# -> src/demo_music.c

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR LM80C 
# -------------------------------------------------------------------

.PHONY: midres.MUSIC.embedded.lm80c midres.MUSIC.lm80c
midres.MUSIC.embedded.lm80c:
	$(FILE2INCLUDE) -i $(DATADIR)/tetris.imf -n tetris.imf -c src/rawdata.c -h src/rawdata.h
	$(CC88) +lm80c $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -o obj/lm80c/rawdata.o src/rawdata.c

obj/lm80c/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/lm80c/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/lm80c/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/lm80c/midres_io.o src/midres_io.asm

obj/lm80c/%.o:	$(SOURCES)
	$(CC88) +lm80c $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -o $@ $(subst obj/lm80c/,src/,$(@:.o=.c))

$(EXEDIR)/midres.MUSIC.lm80c:	midres.MUSIC.embedded.lm80c $(subst PLATFORM,lm80c,$(OBJS)) $(subst PLATFORM,lm80c,$(LIB_OBJS)) obj/lm80c/rawdata.o obj/lm80c/midres_vdp_impl.o obj/lm80c/midres_io.o
	$(CC88) +lm80c  -m $(LDFLAGS88) obj/lm80c/rawdata.o obj/lm80c/midres_io.o obj/lm80c/midres_vdp_impl.o obj/lm80c/demo_music.o obj/lm80c/demo_tile.o obj/lm80c/game_air_attack.o obj/lm80c/game_alien_storm.o obj/lm80c/game_elevator.o obj/lm80c/game_totto.o obj/lm80c/midres.o obj/lm80c/midres_atari.o obj/lm80c/midres_atmos.o obj/lm80c/midres_ay8910.o obj/lm80c/midres_c128.o obj/lm80c/midres_c16.o obj/lm80c/midres_c64.o obj/lm80c/midres_coleco.o obj/lm80c/midres_control_000.o obj/lm80c/midres_control_001.o obj/lm80c/midres_control_002.o obj/lm80c/midres_control_003.o obj/lm80c/midres_control_004.o obj/lm80c/midres_control_005.o obj/lm80c/midres_control_007.o obj/lm80c/midres_control_010.o obj/lm80c/midres_control_011.o obj/lm80c/midres_file_001.o obj/lm80c/midres_file_003.o obj/lm80c/midres_frame_000.o obj/lm80c/midres_frame_001.o obj/lm80c/midres_gb.o obj/lm80c/midres_lm80c.o obj/lm80c/midres_msx.o obj/lm80c/midres_mtx500.o obj/lm80c/midres_music_001.o obj/lm80c/midres_music_002.o obj/lm80c/midres_music_004.o obj/lm80c/midres_music_005.o obj/lm80c/midres_music_006.o obj/lm80c/midres_plus4.o obj/lm80c/midres_pokey.o obj/lm80c/midres_screen_000.o obj/lm80c/midres_screen_006.o obj/lm80c/midres_screen_007.o obj/lm80c/midres_screen_010.o obj/lm80c/midres_screen_012.o obj/lm80c/midres_screen_013.o obj/lm80c/midres_sid.o obj/lm80c/midres_sound_000.o obj/lm80c/midres_sound_001.o obj/lm80c/midres_sound_002.o obj/lm80c/midres_sound_005.o obj/lm80c/midres_sound_006.o obj/lm80c/midres_sound_007.o obj/lm80c/midres_svi.o obj/lm80c/midres_ted.o obj/lm80c/midres_tile_000.o obj/lm80c/midres_tile_004.o obj/lm80c/midres_tile_005.o obj/lm80c/midres_tile_006.o obj/lm80c/midres_tile_007.o obj/lm80c/midres_tile_008.o obj/lm80c/midres_tile_009.o obj/lm80c/midres_tile_010.o obj/lm80c/midres_tile_011.o obj/lm80c/midres_tile_012.o obj/lm80c/midres_tile_013.o obj/lm80c/midres_tile_014.o obj/lm80c/midres_tile_015.o obj/lm80c/midres_tile_016.o obj/lm80c/midres_tile_018.o obj/lm80c/midres_tile_019.o obj/lm80c/midres_tile_020.o obj/lm80c/midres_tile_021.o obj/lm80c/midres_tile_022.o obj/lm80c/midres_tile_023.o obj/lm80c/midres_tile_025.o obj/lm80c/midres_tile_026.o obj/lm80c/midres_tile_027.o obj/lm80c/midres_tile_028.o obj/lm80c/midres_tile_multicolor_000.o obj/lm80c/midres_tile_multicolor_001.o obj/lm80c/midres_tile_multicolor_002.o obj/lm80c/midres_tile_multicolor_003.o obj/lm80c/midres_tile_multicolor_004.o obj/lm80c/midres_tile_multicolor_005.o obj/lm80c/midres_tile_multicolor_006.o obj/lm80c/midres_tile_multicolor_007.o obj/lm80c/midres_tile_multicolor_008.o obj/lm80c/midres_tile_multicolor_009.o obj/lm80c/midres_tile_multicolor_010.o obj/lm80c/midres_tile_multicolor_011.o obj/lm80c/midres_tile_multicolor_012.o obj/lm80c/midres_tile_processing_001.o obj/lm80c/midres_tile_processing_002.o obj/lm80c/midres_tile_processing_004.o obj/lm80c/midres_tile_processing_005.o obj/lm80c/midres_tile_processing_007.o obj/lm80c/midres_tile_processing_009.o obj/lm80c/midres_tile_processing_011.o obj/lm80c/midres_tile_processing_012.o obj/lm80c/midres_tile_processing_016.o obj/lm80c/midres_vanilla.o obj/lm80c/midres_vdp.o obj/lm80c/midres_vic.o obj/lm80c/midres_vic20.o obj/lm80c/utility_joycheck.o obj/lm80c/midres_data.o obj/lm80c/main.o -create-app 
	$(call COPYFILES,a.PRG,$(EXEDIR)/midres.lm80c.prg)
	$(call RMFILES,a.*)

# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR MSX 
# -------------------------------------------------------------------

lib/midres.msx.lib:
# find_main_module(DEMO_MUSIC)
# -> src/demo_music.c

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR MSX 
# -------------------------------------------------------------------

.PHONY: midres.MUSIC.embedded.msx midres.MUSIC.msx
midres.MUSIC.embedded.msx:
	$(FILE2INCLUDE) -i $(DATADIR)/tetris.imf -n tetris.imf -c src/rawdata.c -h src/rawdata.h
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -o obj/msx/rawdata.o src/rawdata.c

obj/msx/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/msx/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/msx/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/msx/midres_io.o src/midres_io.asm

obj/msx/%.o:	$(SOURCES)
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -o $@ $(subst obj/msx/,src/,$(@:.o=.c))

$(EXEDIR)/midres.MUSIC.msx:	midres.MUSIC.embedded.msx $(subst PLATFORM,msx,$(OBJS)) $(subst PLATFORM,msx,$(LIB_OBJS)) obj/msx/rawdata.o obj/msx/midres_vdp_impl.o obj/msx/midres_io.o
	$(CC88) +msx -subtype=rom -m $(LDFLAGS88) obj/msx/rawdata.o obj/msx/midres_io.o obj/msx/midres_vdp_impl.o obj/msx/demo_music.o obj/msx/demo_tile.o obj/msx/game_air_attack.o obj/msx/game_alien_storm.o obj/msx/game_elevator.o obj/msx/game_totto.o obj/msx/midres.o obj/msx/midres_atari.o obj/msx/midres_atmos.o obj/msx/midres_ay8910.o obj/msx/midres_c128.o obj/msx/midres_c16.o obj/msx/midres_c64.o obj/msx/midres_coleco.o obj/msx/midres_control_000.o obj/msx/midres_control_001.o obj/msx/midres_control_002.o obj/msx/midres_control_003.o obj/msx/midres_control_004.o obj/msx/midres_control_005.o obj/msx/midres_control_007.o obj/msx/midres_control_010.o obj/msx/midres_control_011.o obj/msx/midres_file_001.o obj/msx/midres_file_003.o obj/msx/midres_frame_000.o obj/msx/midres_frame_001.o obj/msx/midres_gb.o obj/msx/midres_lm80c.o obj/msx/midres_msx.o obj/msx/midres_mtx500.o obj/msx/midres_music_001.o obj/msx/midres_music_002.o obj/msx/midres_music_004.o obj/msx/midres_music_005.o obj/msx/midres_music_006.o obj/msx/midres_plus4.o obj/msx/midres_pokey.o obj/msx/midres_screen_000.o obj/msx/midres_screen_006.o obj/msx/midres_screen_007.o obj/msx/midres_screen_010.o obj/msx/midres_screen_012.o obj/msx/midres_screen_013.o obj/msx/midres_sid.o obj/msx/midres_sound_000.o obj/msx/midres_sound_001.o obj/msx/midres_sound_002.o obj/msx/midres_sound_005.o obj/msx/midres_sound_006.o obj/msx/midres_sound_007.o obj/msx/midres_svi.o obj/msx/midres_ted.o obj/msx/midres_tile_000.o obj/msx/midres_tile_004.o obj/msx/midres_tile_005.o obj/msx/midres_tile_006.o obj/msx/midres_tile_007.o obj/msx/midres_tile_008.o obj/msx/midres_tile_009.o obj/msx/midres_tile_010.o obj/msx/midres_tile_011.o obj/msx/midres_tile_012.o obj/msx/midres_tile_013.o obj/msx/midres_tile_014.o obj/msx/midres_tile_015.o obj/msx/midres_tile_016.o obj/msx/midres_tile_018.o obj/msx/midres_tile_019.o obj/msx/midres_tile_020.o obj/msx/midres_tile_021.o obj/msx/midres_tile_022.o obj/msx/midres_tile_023.o obj/msx/midres_tile_025.o obj/msx/midres_tile_026.o obj/msx/midres_tile_027.o obj/msx/midres_tile_028.o obj/msx/midres_tile_multicolor_000.o obj/msx/midres_tile_multicolor_001.o obj/msx/midres_tile_multicolor_002.o obj/msx/midres_tile_multicolor_003.o obj/msx/midres_tile_multicolor_004.o obj/msx/midres_tile_multicolor_005.o obj/msx/midres_tile_multicolor_006.o obj/msx/midres_tile_multicolor_007.o obj/msx/midres_tile_multicolor_008.o obj/msx/midres_tile_multicolor_009.o obj/msx/midres_tile_multicolor_010.o obj/msx/midres_tile_multicolor_011.o obj/msx/midres_tile_multicolor_012.o obj/msx/midres_tile_processing_001.o obj/msx/midres_tile_processing_002.o obj/msx/midres_tile_processing_004.o obj/msx/midres_tile_processing_005.o obj/msx/midres_tile_processing_007.o obj/msx/midres_tile_processing_009.o obj/msx/midres_tile_processing_011.o obj/msx/midres_tile_processing_012.o obj/msx/midres_tile_processing_016.o obj/msx/midres_vanilla.o obj/msx/midres_vdp.o obj/msx/midres_vic.o obj/msx/midres_vic20.o obj/msx/utility_joycheck.o obj/msx/midres_data.o obj/msx/main.o -create-app 
	$(call COPYFILES,a.rom,$(EXEDIR)/midres.msx.rom)
	$(call RMFILES,a.*)

# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR MTX500 
# -------------------------------------------------------------------

lib/midres.mtx500.lib:
# find_main_module(DEMO_MUSIC)
# -> src/demo_music.c

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR MTX500 
# -------------------------------------------------------------------

.PHONY: midres.MUSIC.embedded.mtx500 midres.MUSIC.mtx500
midres.MUSIC.embedded.mtx500:
	$(FILE2INCLUDE) -i $(DATADIR)/mtiles.bin -n mtiles.bin -i $(DATADIR)/tiles.bin -n tiles.bin -i $(DATADIR)/tutorial_mctile.bin -n tutorial_mctile.bin -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/toccatina.imf -n toccatina.imf -i $(DATADIR)/alice.imf -n alice.imf -i $(DATADIR)/island.imf -n island.imf -i $(DATADIR)/tetris.imf -n tetris.imf -c src/rawdata.c -h src/rawdata.h
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -o obj/mtx500/rawdata.o src/rawdata.c

obj/mtx500/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/mtx500/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/mtx500/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/mtx500/midres_io.o src/midres_io.asm

obj/mtx500/%.o:	$(SOURCES)
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -o $@ $(subst obj/mtx500/,src/,$(@:.o=.c))

$(EXEDIR)/midres.MUSIC.mtx500:	midres.MUSIC.embedded.mtx500 $(subst PLATFORM,mtx500,$(OBJS)) $(subst PLATFORM,mtx500,$(LIB_OBJS)) obj/mtx500/rawdata.o obj/mtx500/midres_vdp_impl.o obj/mtx500/midres_io.o
	$(CC88) +mtx  -m $(LDFLAGS88) obj/mtx500/rawdata.o obj/mtx500/midres_io.o obj/mtx500/midres_vdp_impl.o obj/mtx500/demo_music.o obj/mtx500/demo_tile.o obj/mtx500/game_air_attack.o obj/mtx500/game_alien_storm.o obj/mtx500/game_elevator.o obj/mtx500/game_totto.o obj/mtx500/midres.o obj/mtx500/midres_atari.o obj/mtx500/midres_atmos.o obj/mtx500/midres_ay8910.o obj/mtx500/midres_c128.o obj/mtx500/midres_c16.o obj/mtx500/midres_c64.o obj/mtx500/midres_coleco.o obj/mtx500/midres_control_000.o obj/mtx500/midres_control_001.o obj/mtx500/midres_control_002.o obj/mtx500/midres_control_003.o obj/mtx500/midres_control_004.o obj/mtx500/midres_control_005.o obj/mtx500/midres_control_007.o obj/mtx500/midres_control_010.o obj/mtx500/midres_control_011.o obj/mtx500/midres_file_001.o obj/mtx500/midres_file_003.o obj/mtx500/midres_frame_000.o obj/mtx500/midres_frame_001.o obj/mtx500/midres_gb.o obj/mtx500/midres_lm80c.o obj/mtx500/midres_msx.o obj/mtx500/midres_mtx500.o obj/mtx500/midres_music_001.o obj/mtx500/midres_music_002.o obj/mtx500/midres_music_004.o obj/mtx500/midres_music_005.o obj/mtx500/midres_music_006.o obj/mtx500/midres_plus4.o obj/mtx500/midres_pokey.o obj/mtx500/midres_screen_000.o obj/mtx500/midres_screen_006.o obj/mtx500/midres_screen_007.o obj/mtx500/midres_screen_010.o obj/mtx500/midres_screen_012.o obj/mtx500/midres_screen_013.o obj/mtx500/midres_sid.o obj/mtx500/midres_sound_000.o obj/mtx500/midres_sound_001.o obj/mtx500/midres_sound_002.o obj/mtx500/midres_sound_005.o obj/mtx500/midres_sound_006.o obj/mtx500/midres_sound_007.o obj/mtx500/midres_svi.o obj/mtx500/midres_ted.o obj/mtx500/midres_tile_000.o obj/mtx500/midres_tile_004.o obj/mtx500/midres_tile_005.o obj/mtx500/midres_tile_006.o obj/mtx500/midres_tile_007.o obj/mtx500/midres_tile_008.o obj/mtx500/midres_tile_009.o obj/mtx500/midres_tile_010.o obj/mtx500/midres_tile_011.o obj/mtx500/midres_tile_012.o obj/mtx500/midres_tile_013.o obj/mtx500/midres_tile_014.o obj/mtx500/midres_tile_015.o obj/mtx500/midres_tile_016.o obj/mtx500/midres_tile_018.o obj/mtx500/midres_tile_019.o obj/mtx500/midres_tile_020.o obj/mtx500/midres_tile_021.o obj/mtx500/midres_tile_022.o obj/mtx500/midres_tile_023.o obj/mtx500/midres_tile_025.o obj/mtx500/midres_tile_026.o obj/mtx500/midres_tile_027.o obj/mtx500/midres_tile_028.o obj/mtx500/midres_tile_multicolor_000.o obj/mtx500/midres_tile_multicolor_001.o obj/mtx500/midres_tile_multicolor_002.o obj/mtx500/midres_tile_multicolor_003.o obj/mtx500/midres_tile_multicolor_004.o obj/mtx500/midres_tile_multicolor_005.o obj/mtx500/midres_tile_multicolor_006.o obj/mtx500/midres_tile_multicolor_007.o obj/mtx500/midres_tile_multicolor_008.o obj/mtx500/midres_tile_multicolor_009.o obj/mtx500/midres_tile_multicolor_010.o obj/mtx500/midres_tile_multicolor_011.o obj/mtx500/midres_tile_multicolor_012.o obj/mtx500/midres_tile_processing_001.o obj/mtx500/midres_tile_processing_002.o obj/mtx500/midres_tile_processing_004.o obj/mtx500/midres_tile_processing_005.o obj/mtx500/midres_tile_processing_007.o obj/mtx500/midres_tile_processing_009.o obj/mtx500/midres_tile_processing_011.o obj/mtx500/midres_tile_processing_012.o obj/mtx500/midres_tile_processing_016.o obj/mtx500/midres_vanilla.o obj/mtx500/midres_vdp.o obj/mtx500/midres_vic.o obj/mtx500/midres_vic20.o obj/mtx500/utility_joycheck.o obj/mtx500/midres_data.o obj/mtx500/main.o -create-app 
	$(call COPYFILES,a.Array,$(EXEDIR)/midres.mtx500.Array)
	$(call RMFILES,a.*)

# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR PLUS4 
# -------------------------------------------------------------------

obj/plus4/midres_sid_impl.o:	src/midres_sid_impl.asm
	$(ASM) -t plus4 -oobj/plus4/midres_sid_impl.o src/midres_sid_impl.asm

obj/plus4/midres_ted_impl.o:	src/midres_ted_impl.asm
	$(ASM) -t plus4 -oobj/plus4/midres_ted_impl.o src/midres_ted_impl.asm

obj/plus4/midres_pokey_impl.o:	src/midres_pokey_impl.asm
	$(ASM) -t plus4 -oobj/plus4/midres_pokey_impl.o src/midres_pokey_impl.asm

obj/plus4/%.o:	$(LIB_INCLUDES) $(LIB_SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t plus4 -c $(CFLAGS)  -Osir -Cl -D__CBM__  -o $@ $(subst obj/plus4/,src/,$(@:.o=.c))

$(LIBDIR)/midres.plus4.lib:	$(LIB_INCLUDES) $(subst PLATFORM,plus4,$(LIB_OBJS)) obj/plus4/midres_sid_impl.o obj/plus4/midres_ted_impl.o obj/plus4/midres_pokey_impl.o
	$(AR) r $(LIBDIR)/midres.plus4.lib $(subst PLATFORM,plus4,$(LIB_OBJS)) obj/plus4/midres_sid_impl.o obj/plus4/midres_ted_impl.o obj/plus4/midres_pokey_impl.o

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR PLUS4 
# -------------------------------------------------------------------

.PHONY: midres.MUSIC.embedded.plus4 midres.MUSIC.plus4
midres.MUSIC.embedded.plus4:
	$(FILE2INCLUDE) -i $(DATADIR)/slideshow16.dat -n slideshow -i $(DATADIR)/image1601.mpic -n image1601.mpic -i $(DATADIR)/image1602.mpic -n image1602.mpic -i $(DATADIR)/image1603.mpic -n image1603.mpic -i $(DATADIR)/image1604.mpic -n image1604.mpic -i $(DATADIR)/tiles.bin -n tiles.bin -i $(DATADIR)/aatiles4.bin -n aatiles.bin -i $(DATADIR)/aaintro16.mpic -n aaintro.mpic -i $(DATADIR)/tutorial_mctile.bin -n mctile.bin -i $(DATADIR)/testcard.bin -n testcard.bin -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/toccatina.imf -n toccatina.imf -i $(DATADIR)/alice.imf -n alice.imf -i $(DATADIR)/island.imf -n island.imf -i $(DATADIR)/tetris.imf -n tetris.imf -c src/rawdata.c -h src/rawdata.h

obj/plus4/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t plus4 -c $(CFLAGS)  -Osir -Cl -D__CBM__ -o $@ $(subst obj/plus4/,src/,$(@:.o=.c))

$(EXEDIR)/midres.MUSIC.plus4: midres.MUSIC.embedded.plus4 $(subst PLATFORM,plus4,$(OBJS))
	$(CC) -Ln demoplus4.lbl -t plus4 -C cfg/plus4.cfg  $(LDFLAGS) -m $(EXEDIR)/midres.plus4.map -o $(EXEDIR)/midres.plus4 $(subst PLATFORM,plus4,$(OBJS)) $(LIBDIR)/midres.plus4.lib
	$(call RMFILES,$(EXEDIR)/midres.plus4.d64)
	$(CC1541) -f loader -w $(DATADIR)/loader4.prg $(EXEDIR)/midres.plus4.d64 
	$(CC1541) -f midres-single -w $(EXEDIR)/midres.plus4 $(EXEDIR)/midres.plus4.d64 
	$(CC1541) -f slideshow -w $(DATADIR)/slideshow16.dat $(EXEDIR)/midres.plus4.d64 
	$(CC1541) -f image1601.mpic -w $(DATADIR)/image1601.mpic $(EXEDIR)/midres.plus4.d64 
	$(CC1541) -f image1602.mpic -w $(DATADIR)/image1602.mpic $(EXEDIR)/midres.plus4.d64 
	$(CC1541) -f image1603.mpic -w $(DATADIR)/image1603.mpic $(EXEDIR)/midres.plus4.d64 
	$(CC1541) -f image1604.mpic -w $(DATADIR)/image1604.mpic $(EXEDIR)/midres.plus4.d64 
	$(CC1541) -f tiles.bin -w $(DATADIR)/tiles.bin $(EXEDIR)/midres.plus4.d64 
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles4.bin $(EXEDIR)/midres.plus4.d64 
	$(CC1541) -f aaintro.mpic -w $(DATADIR)/aaintro16.mpic $(EXEDIR)/midres.plus4.d64 
	$(CC1541) -f mctile.bin -w $(DATADIR)/tutorial_mctile.bin $(EXEDIR)/midres.plus4.d64 
	$(CC1541) -f testcard.bin -w $(DATADIR)/testcard.bin $(EXEDIR)/midres.plus4.d64 
	$(CC1541) -f zdjtiles.bin -w $(DATADIR)/zdjtiles.bin $(EXEDIR)/midres.plus4.d64 
	$(CC1541) -f zeltiles.bin -w $(DATADIR)/zeltiles.bin $(EXEDIR)/midres.plus4.d64 
	$(CC1541) -f toccatina.imf -w $(DATADIR)/toccatina.imf $(EXEDIR)/midres.plus4.d64 
	$(CC1541) -f alice.imf -w $(DATADIR)/alice.imf $(EXEDIR)/midres.plus4.d64 
	$(CC1541) -f island.imf -w $(DATADIR)/island.imf $(EXEDIR)/midres.plus4.d64 
	$(CC1541) -f tetris.imf -w $(DATADIR)/tetris.imf $(EXEDIR)/midres.plus4.d64 


# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR SVI 
# -------------------------------------------------------------------

lib/midres.svi.lib:
# find_main_module(DEMO_MUSIC)
# -> src/demo_music.c

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR SVI 
# -------------------------------------------------------------------

.PHONY: midres.MUSIC.embedded.svi midres.MUSIC.svi
midres.MUSIC.embedded.svi:
	$(FILE2INCLUDE) -i $(DATADIR)/mtiles.bin -n mtiles.bin -i $(DATADIR)/tiles.bin -n tiles.bin -i $(DATADIR)/tutorial_mctile.bin -n tutorial_mctile.bin -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/toccatina.imf -n toccatina.imf -i $(DATADIR)/alice.imf -n alice.imf -i $(DATADIR)/island.imf -n island.imf -i $(DATADIR)/tetris.imf -n tetris.imf -c src/rawdata.c -h src/rawdata.h
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/svi/rawdata.o src/rawdata.c

obj/svi/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/svi/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/svi/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/svi/midres_io.o src/midres_io.asm

obj/svi/%.o:	$(SOURCES)
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o $@ $(subst obj/svi/,src/,$(@:.o=.c))

$(EXEDIR)/midres.MUSIC.svi:	midres.MUSIC.embedded.svi $(subst PLATFORM,svi,$(OBJS)) $(subst PLATFORM,svi,$(LIB_OBJS)) obj/svi/rawdata.o obj/svi/midres_vdp_impl.o obj/svi/midres_io.o
	$(CC88) +svi  -m $(LDFLAGS88) obj/svi/rawdata.o obj/svi/midres_io.o obj/svi/midres_vdp_impl.o obj/svi/demo_music.o obj/svi/demo_tile.o obj/svi/game_air_attack.o obj/svi/game_alien_storm.o obj/svi/game_elevator.o obj/svi/game_totto.o obj/svi/midres.o obj/svi/midres_atari.o obj/svi/midres_atmos.o obj/svi/midres_ay8910.o obj/svi/midres_c128.o obj/svi/midres_c16.o obj/svi/midres_c64.o obj/svi/midres_coleco.o obj/svi/midres_control_000.o obj/svi/midres_control_001.o obj/svi/midres_control_002.o obj/svi/midres_control_003.o obj/svi/midres_control_004.o obj/svi/midres_control_005.o obj/svi/midres_control_007.o obj/svi/midres_control_010.o obj/svi/midres_control_011.o obj/svi/midres_file_001.o obj/svi/midres_file_003.o obj/svi/midres_frame_000.o obj/svi/midres_frame_001.o obj/svi/midres_gb.o obj/svi/midres_lm80c.o obj/svi/midres_msx.o obj/svi/midres_mtx500.o obj/svi/midres_music_001.o obj/svi/midres_music_002.o obj/svi/midres_music_004.o obj/svi/midres_music_005.o obj/svi/midres_music_006.o obj/svi/midres_plus4.o obj/svi/midres_pokey.o obj/svi/midres_screen_000.o obj/svi/midres_screen_006.o obj/svi/midres_screen_007.o obj/svi/midres_screen_010.o obj/svi/midres_screen_012.o obj/svi/midres_screen_013.o obj/svi/midres_sid.o obj/svi/midres_sound_000.o obj/svi/midres_sound_001.o obj/svi/midres_sound_002.o obj/svi/midres_sound_005.o obj/svi/midres_sound_006.o obj/svi/midres_sound_007.o obj/svi/midres_svi.o obj/svi/midres_ted.o obj/svi/midres_tile_000.o obj/svi/midres_tile_004.o obj/svi/midres_tile_005.o obj/svi/midres_tile_006.o obj/svi/midres_tile_007.o obj/svi/midres_tile_008.o obj/svi/midres_tile_009.o obj/svi/midres_tile_010.o obj/svi/midres_tile_011.o obj/svi/midres_tile_012.o obj/svi/midres_tile_013.o obj/svi/midres_tile_014.o obj/svi/midres_tile_015.o obj/svi/midres_tile_016.o obj/svi/midres_tile_018.o obj/svi/midres_tile_019.o obj/svi/midres_tile_020.o obj/svi/midres_tile_021.o obj/svi/midres_tile_022.o obj/svi/midres_tile_023.o obj/svi/midres_tile_025.o obj/svi/midres_tile_026.o obj/svi/midres_tile_027.o obj/svi/midres_tile_028.o obj/svi/midres_tile_multicolor_000.o obj/svi/midres_tile_multicolor_001.o obj/svi/midres_tile_multicolor_002.o obj/svi/midres_tile_multicolor_003.o obj/svi/midres_tile_multicolor_004.o obj/svi/midres_tile_multicolor_005.o obj/svi/midres_tile_multicolor_006.o obj/svi/midres_tile_multicolor_007.o obj/svi/midres_tile_multicolor_008.o obj/svi/midres_tile_multicolor_009.o obj/svi/midres_tile_multicolor_010.o obj/svi/midres_tile_multicolor_011.o obj/svi/midres_tile_multicolor_012.o obj/svi/midres_tile_processing_001.o obj/svi/midres_tile_processing_002.o obj/svi/midres_tile_processing_004.o obj/svi/midres_tile_processing_005.o obj/svi/midres_tile_processing_007.o obj/svi/midres_tile_processing_009.o obj/svi/midres_tile_processing_011.o obj/svi/midres_tile_processing_012.o obj/svi/midres_tile_processing_016.o obj/svi/midres_vanilla.o obj/svi/midres_vdp.o obj/svi/midres_vic.o obj/svi/midres_vic20.o obj/svi/utility_joycheck.o obj/svi/midres_data.o obj/svi/main.o -create-app 
	$(call COPYFILES,a.cas,$(EXEDIR)/midres.svi.cas)
	$(call RMFILES,a.*)

# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR VIC20 
# -------------------------------------------------------------------

obj/vic20/midres_sid_impl.o:	src/midres_sid_impl.asm
	$(ASM) -t vic20 -oobj/vic20/midres_sid_impl.o src/midres_sid_impl.asm

obj/vic20/midres_ted_impl.o:	src/midres_ted_impl.asm
	$(ASM) -t vic20 -oobj/vic20/midres_ted_impl.o src/midres_ted_impl.asm

obj/vic20/midres_pokey_impl.o:	src/midres_pokey_impl.asm
	$(ASM) -t vic20 -oobj/vic20/midres_pokey_impl.o src/midres_pokey_impl.asm

obj/vic20/%.o:	$(LIB_INCLUDES) $(LIB_SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS)  -Osir -Cl -D__CBM__  -o $@ $(subst obj/vic20/,src/,$(@:.o=.c))

$(LIBDIR)/midres.vic20.lib:	$(LIB_INCLUDES) $(subst PLATFORM,vic20,$(LIB_OBJS)) obj/vic20/midres_sid_impl.o obj/vic20/midres_ted_impl.o obj/vic20/midres_pokey_impl.o
	$(AR) r $(LIBDIR)/midres.vic20.lib $(subst PLATFORM,vic20,$(LIB_OBJS)) obj/vic20/midres_sid_impl.o obj/vic20/midres_ted_impl.o obj/vic20/midres_pokey_impl.o

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR VIC20 
# -------------------------------------------------------------------

.PHONY: midres.MUSIC.embedded.vic20 midres.MUSIC.vic20
midres.MUSIC.embedded.vic20:
	$(FILE2INCLUDE) -i $(DATADIR)/slideshow20.dat -n slideshow -i $(DATADIR)/image2001.mpic -n image2001.mpic -i $(DATADIR)/image2002.mpic -n image2002.mpic -i $(DATADIR)/image2003.mpic -n image2003.mpic -i $(DATADIR)/image2004.mpic -n image2004.mpic -i $(DATADIR)/tiles.bin -n tiles.bin -i $(DATADIR)/aatiles20.bin -n aatiles.bin -i $(DATADIR)/aaintro20.mpic -n aaintro.mpic -i $(DATADIR)/tutorial_mctile.bin -n mctile.bin -i $(DATADIR)/testcard.bin -n testcard.bin -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/toccatina.imf -n toccatina.imf -i $(DATADIR)/alice.imf -n alice.imf -i $(DATADIR)/island.imf -n island.imf -i $(DATADIR)/tetris.imf -n tetris.imf -c src/rawdata.c -h src/rawdata.h

obj/vic20/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS)  -Osir -Cl -D__CBM__ -o $@ $(subst obj/vic20/,src/,$(@:.o=.c))

$(EXEDIR)/midres.MUSIC.vic20: midres.MUSIC.embedded.vic20 $(subst PLATFORM,vic20,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic20.cfg  $(LDFLAGS) -m $(EXEDIR)/midres.vic20.map -o $(EXEDIR)/midres.vic20 $(subst PLATFORM,vic20,$(OBJS)) $(LIBDIR)/midres.vic20.lib
	$(call RMFILES,$(EXEDIR)/midres.vic20.d64)
	$(CC1541) -f midres-single -w $(EXEDIR)/midres.vic20 $(EXEDIR)/midres.vic20.d64 
	$(CC1541) -f slideshow -w $(DATADIR)/slideshow20.dat $(EXEDIR)/midres.vic20.d64 
	$(CC1541) -f image2001.mpic -w $(DATADIR)/image2001.mpic $(EXEDIR)/midres.vic20.d64 
	$(CC1541) -f image2002.mpic -w $(DATADIR)/image2002.mpic $(EXEDIR)/midres.vic20.d64 
	$(CC1541) -f image2003.mpic -w $(DATADIR)/image2003.mpic $(EXEDIR)/midres.vic20.d64 
	$(CC1541) -f image2004.mpic -w $(DATADIR)/image2004.mpic $(EXEDIR)/midres.vic20.d64 
	$(CC1541) -f tiles.bin -w $(DATADIR)/tiles.bin $(EXEDIR)/midres.vic20.d64 
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles20.bin $(EXEDIR)/midres.vic20.d64 
	$(CC1541) -f aaintro.mpic -w $(DATADIR)/aaintro20.mpic $(EXEDIR)/midres.vic20.d64 
	$(CC1541) -f mctile.bin -w $(DATADIR)/tutorial_mctile.bin $(EXEDIR)/midres.vic20.d64 
	$(CC1541) -f testcard.bin -w $(DATADIR)/testcard.bin $(EXEDIR)/midres.vic20.d64 
	$(CC1541) -f zdjtiles.bin -w $(DATADIR)/zdjtiles.bin $(EXEDIR)/midres.vic20.d64 
	$(CC1541) -f zeltiles.bin -w $(DATADIR)/zeltiles.bin $(EXEDIR)/midres.vic20.d64 
	$(CC1541) -f toccatina.imf -w $(DATADIR)/toccatina.imf $(EXEDIR)/midres.vic20.d64 
	$(CC1541) -f alice.imf -w $(DATADIR)/alice.imf $(EXEDIR)/midres.vic20.d64 
	$(CC1541) -f island.imf -w $(DATADIR)/island.imf $(EXEDIR)/midres.vic20.d64 
	$(CC1541) -f tetris.imf -w $(DATADIR)/tetris.imf $(EXEDIR)/midres.vic20.d64 


# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR VIC2024 
# -------------------------------------------------------------------

obj/vic2024/midres_sid_impl.o:	src/midres_sid_impl.asm
	$(ASM) -t vic20 -oobj/vic2024/midres_sid_impl.o src/midres_sid_impl.asm

obj/vic2024/midres_ted_impl.o:	src/midres_ted_impl.asm
	$(ASM) -t vic20 -oobj/vic2024/midres_ted_impl.o src/midres_ted_impl.asm

obj/vic2024/midres_pokey_impl.o:	src/midres_pokey_impl.asm
	$(ASM) -t vic20 -oobj/vic2024/midres_pokey_impl.o src/midres_pokey_impl.asm

obj/vic2024/%.o:	$(LIB_INCLUDES) $(LIB_SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS) -D__24K__ -Osir -Cl -D__CBM__  -o $@ $(subst obj/vic2024/,src/,$(@:.o=.c))

$(LIBDIR)/midres.vic2024.lib:	$(LIB_INCLUDES) $(subst PLATFORM,vic2024,$(LIB_OBJS)) obj/vic2024/midres_sid_impl.o obj/vic2024/midres_ted_impl.o obj/vic2024/midres_pokey_impl.o
	$(AR) r $(LIBDIR)/midres.vic2024.lib $(subst PLATFORM,vic2024,$(LIB_OBJS)) obj/vic2024/midres_sid_impl.o obj/vic2024/midres_ted_impl.o obj/vic2024/midres_pokey_impl.o

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR VIC2024 
# -------------------------------------------------------------------

.PHONY: midres.MUSIC.embedded.vic2024 midres.MUSIC.vic2024
midres.MUSIC.embedded.vic2024:
	$(FILE2INCLUDE) -i $(DATADIR)/image2001.mpic -n image2001.mpic -i $(DATADIR)/image2002.mpic -n image2002.mpic -i $(DATADIR)/image2003.mpic -n image2003.mpic -i $(DATADIR)/image2004.mpic -n image2004.mpic -i $(DATADIR)/tiles.bin -n tiles.bin -i $(DATADIR)/aatiles20.bin -n aatiles.bin -i $(DATADIR)/aaintro20.mpic -n aaintro.mpic -i $(DATADIR)/tutorial_mctile.bin -n mctile.bin -i $(DATADIR)/testcard.bin -n testcard.bin -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/toccatina.imf -n toccatina.imf -i $(DATADIR)/alice.imf -n alice.imf -i $(DATADIR)/island.imf -n island.imf -i $(DATADIR)/tetris.imf -n tetris.imf -c src/rawdata.c -h src/rawdata.h

obj/vic2024/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS) -D__24K__ -Osir -Cl -D__CBM__ -o $@ $(subst obj/vic2024/,src/,$(@:.o=.c))

$(EXEDIR)/midres.MUSIC.vic2024: midres.MUSIC.embedded.vic2024 $(subst PLATFORM,vic2024,$(OBJS))
	$(CC) -Ln demovic2024.lbl -t vic20 -C cfg/vic2024.cfg  $(LDFLAGS) -m $(EXEDIR)/midres.vic2024.map -o $(EXEDIR)/midres.vic2024 $(subst PLATFORM,vic2024,$(OBJS)) $(LIBDIR)/midres.vic2024.lib
	$(call RMFILES,$(EXEDIR)/midres.vic2024.d64)
	$(CC1541) -f loader -w $(DATADIR)/loader2024.prg $(EXEDIR)/midres.vic2024.d64 
	$(CC1541) -f midres-single -w $(EXEDIR)/midres.vic2024 $(EXEDIR)/midres.vic2024.d64 
	$(CC1541) -f image2001.mpic -w $(DATADIR)/image2001.mpic $(EXEDIR)/midres.vic2024.d64 
	$(CC1541) -f image2002.mpic -w $(DATADIR)/image2002.mpic $(EXEDIR)/midres.vic2024.d64 
	$(CC1541) -f image2003.mpic -w $(DATADIR)/image2003.mpic $(EXEDIR)/midres.vic2024.d64 
	$(CC1541) -f image2004.mpic -w $(DATADIR)/image2004.mpic $(EXEDIR)/midres.vic2024.d64 
	$(CC1541) -f tiles.bin -w $(DATADIR)/tiles.bin $(EXEDIR)/midres.vic2024.d64 
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles20.bin $(EXEDIR)/midres.vic2024.d64 
	$(CC1541) -f aaintro.mpic -w $(DATADIR)/aaintro20.mpic $(EXEDIR)/midres.vic2024.d64 
	$(CC1541) -f mctile.bin -w $(DATADIR)/tutorial_mctile.bin $(EXEDIR)/midres.vic2024.d64 
	$(CC1541) -f testcard.bin -w $(DATADIR)/testcard.bin $(EXEDIR)/midres.vic2024.d64 
	$(CC1541) -f zdjtiles.bin -w $(DATADIR)/zdjtiles.bin $(EXEDIR)/midres.vic2024.d64 
	$(CC1541) -f zeltiles.bin -w $(DATADIR)/zeltiles.bin $(EXEDIR)/midres.vic2024.d64 
	$(CC1541) -f toccatina.imf -w $(DATADIR)/toccatina.imf $(EXEDIR)/midres.vic2024.d64 
	$(CC1541) -f alice.imf -w $(DATADIR)/alice.imf $(EXEDIR)/midres.vic2024.d64 
	$(CC1541) -f island.imf -w $(DATADIR)/island.imf $(EXEDIR)/midres.vic2024.d64 
	$(CC1541) -f tetris.imf -w $(DATADIR)/tetris.imf $(EXEDIR)/midres.vic2024.d64 


# -------------------------------------------------------------------
# --- AIRATTACK FOR ATARI 
# -------------------------------------------------------------------

.PHONY: airattack.embedded.atari
airattack.embedded.atari:
	$(FILE2INCLUDE) -i $(DATADIR)/aatiles4.bin -n zztiles.bin -i $(DATADIR)/aaintroa.pic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h

obj/airattack.atari/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t atari -c $(CFLAGS)  -Osir -Cl -D__AIRATTACK__  -o $@ $(subst obj/airattack.atari/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.atari: airattack.embedded.atari $(subst PLATFORM,airattack.atari,$(OBJS))
	$(CC) -Ln demoatari.lbl -t atari -C cfg/atari.cfg $(LDFLAGS) -m $(EXEDIR)/airattack.atari.map -o $(EXEDIR)/airattack.atari  $(subst PLATFORM,airattack.atari,$(OBJS)) $(LIBDIR)/midres.atari.lib
	$(call RMFILES,$(EXEDIR)/atr/*.*)
	$(call COPYFILES,$(DIR2ATR_HOME)/dos25/dos.sys,$(EXEDIR)/atr/dos.sys)
	$(call COPYFILES,$(EXEDIR)/airattack.atari,$(EXEDIR)/atr/game.exe)
	$(call COPYFILES,$(DATADIR)/aatiles4.bin,$(EXEDIR)/atr/zztiles.bin)
	$(call COPYFILES,$(DATADIR)/aaintroa.pic,$(EXEDIR)/atr/zzintro.pic)
	$(DIR2ATR) -S -p -B $(DIR2ATR_HOME)/dos25/bootcode $(EXEDIR)/airattack.atari.atr $(EXEDIR)/atr
	$(ATRAUTORUN) -i $(EXEDIR)/airattack.atari.atr -o $(EXEDIR)/airattack.atari.atr -f game.exe

# -------------------------------------------------------------------
# --- ALIENSTORM FOR ATARI 
# -------------------------------------------------------------------

.PHONY: alienstorm.embedded.atari
alienstorm.embedded.atari:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h

obj/alienstorm.atari/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t atari -c $(CFLAGS)  -Osir -Cl -D__ALIENSTORM__  -o $@ $(subst obj/alienstorm.atari/,src/,$(@:.o=.c))

$(EXEDIR)/alienstorm.atari: alienstorm.embedded.atari $(subst PLATFORM,alienstorm.atari,$(OBJS))
	$(CC) -Ln demoatari.lbl -t atari -C cfg/atari.cfg $(LDFLAGS) -m $(EXEDIR)/alienstorm.atari.map -o $(EXEDIR)/alienstorm.atari  $(subst PLATFORM,alienstorm.atari,$(OBJS)) $(LIBDIR)/midres.atari.lib
	$(call RMFILES,$(EXEDIR)/atr/*.*)
	$(call COPYFILES,$(DIR2ATR_HOME)/dos25/dos.sys,$(EXEDIR)/atr/dos.sys)
	$(call COPYFILES,$(EXEDIR)/alienstorm.atari,$(EXEDIR)/atr/game.exe)
	$(call COPYFILES,$(DATADIR)/astiles.bin,$(EXEDIR)/atr/zstiles.bin)
	$(call COPYFILES,$(DATADIR)/astiles1.bin,$(EXEDIR)/atr/zstiles1.bin)
	$(call COPYFILES,$(DATADIR)/astiles2.bin,$(EXEDIR)/atr/zstiles2.bin)
	$(DIR2ATR) -S -p -B $(DIR2ATR_HOME)/dos25/bootcode $(EXEDIR)/alienstorm.atari.atr $(EXEDIR)/atr
	$(ATRAUTORUN) -i $(EXEDIR)/alienstorm.atari.atr -o $(EXEDIR)/alienstorm.atari.atr -f game.exe

# -------------------------------------------------------------------
# --- ELEVATOR FOR ATARI 
# -------------------------------------------------------------------

.PHONY: elevator.embedded.atari
elevator.embedded.atari:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator64.mpic -n zelintro.bin -i $(DATADIR)/elevator1.imf -n elevator1.imf -c src/rawdata.c -h src/rawdata.h

obj/elevator.atari/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t atari -c $(CFLAGS)  -Osir -Cl -D__ELEVATOR__  -o $@ $(subst obj/elevator.atari/,src/,$(@:.o=.c))

$(EXEDIR)/elevator.atari: elevator.embedded.atari $(subst PLATFORM,elevator.atari,$(OBJS))
	$(CC) -Ln demoatari.lbl -t atari -C cfg/atari.cfg $(LDFLAGS) -m $(EXEDIR)/elevator.atari.map -o $(EXEDIR)/elevator.atari  $(subst PLATFORM,elevator.atari,$(OBJS)) $(LIBDIR)/midres.atari.lib
	$(call RMFILES,$(EXEDIR)/atr/*.*)
	$(call COPYFILES,$(DIR2ATR_HOME)/dos25/dos.sys,$(EXEDIR)/atr/dos.sys)
	$(call COPYFILES,$(EXEDIR)/elevator.atari,$(EXEDIR)/atr/game.exe)
	$(call COPYFILES,$(DATADIR)/zeltiles.bin,$(EXEDIR)/atr/zeltiles.bin)
	$(call COPYFILES,$(DATADIR)/elevator64.mpic,$(EXEDIR)/atr/zelintro.bin)
	$(call COPYFILES,$(DATADIR)/elevator1.imf,$(EXEDIR)/atr/elevator1.imf)
	$(DIR2ATR) -S -p -B $(DIR2ATR_HOME)/dos25/bootcode $(EXEDIR)/elevator.atari.atr $(EXEDIR)/atr
	$(ATRAUTORUN) -i $(EXEDIR)/elevator.atari.atr -o $(EXEDIR)/elevator.atari.atr -f game.exe

# -------------------------------------------------------------------
# --- JOYCHECK FOR ATARI 
# -------------------------------------------------------------------

.PHONY: joycheck.embedded.atari
joycheck.embedded.atari:
	$(FILE2INCLUDE) -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -c src/rawdata.c -h src/rawdata.h

obj/joycheck.atari/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t atari -c $(CFLAGS)  -Osir -Cl -D__JOYCHECK__  -o $@ $(subst obj/joycheck.atari/,src/,$(@:.o=.c))

$(EXEDIR)/joycheck.atari: joycheck.embedded.atari $(subst PLATFORM,joycheck.atari,$(OBJS))
	$(CC) -Ln demoatari.lbl -t atari -C cfg/atari.cfg $(LDFLAGS) -m $(EXEDIR)/joycheck.atari.map -o $(EXEDIR)/joycheck.atari  $(subst PLATFORM,joycheck.atari,$(OBJS)) $(LIBDIR)/midres.atari.lib
	$(call RMFILES,$(EXEDIR)/atr/*.*)
	$(call COPYFILES,$(DIR2ATR_HOME)/dos25/dos.sys,$(EXEDIR)/atr/dos.sys)
	$(call COPYFILES,$(EXEDIR)/joycheck.atari,$(EXEDIR)/atr/game.exe)
	$(call COPYFILES,$(DATADIR)/zdjtiles.bin,$(EXEDIR)/atr/zdjtiles.bin)
	$(DIR2ATR) -S -p -B $(DIR2ATR_HOME)/dos25/bootcode $(EXEDIR)/joycheck.atari.atr $(EXEDIR)/atr
	$(ATRAUTORUN) -i $(EXEDIR)/joycheck.atari.atr -o $(EXEDIR)/joycheck.atari.atr -f game.exe
                    
#!!! missing resources for totto (atari)


# -------------------------------------------------------------------
# --- AIRATTACK FOR C128 
# -------------------------------------------------------------------

.PHONY: airattack.embedded.c128
airattack.embedded.c128:
	$(FILE2INCLUDE) -i $(DATADIR)/aatiles.bin -n zztiles.bin -i $(DATADIR)/aaintro64.mpic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h

obj/airattack.c128/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c128 -c $(CFLAGS)  -Osir -Cl -D__AIRATTACK__ -D__CBM__ -o $@ $(subst obj/airattack.c128/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.c128: airattack.embedded.c128 $(subst PLATFORM,airattack.c128,$(OBJS))
	$(CC) -Ln democ128.lbl -t c128 -C cfg/c128.cfg $(LDFLAGS) -m $(EXEDIR)/airattack.c128.map -o $(EXEDIR)/airattack.c128  $(subst PLATFORM,airattack.c128,$(OBJS)) $(LIBDIR)/midres.c128.lib
	$(call RMFILES,$(EXEDIR)/airattack.c128.d64)
	$(CC1541) -f airattack -w $(EXEDIR)/airattack.c128 $(EXEDIR)/airattack.c128.d64 
	$(CC1541) -f zztiles.bin -w $(DATADIR)/aatiles.bin $(EXEDIR)/airattack.c128.d64 
	$(CC1541) -f zzintro.pic -w $(DATADIR)/aaintro64.mpic $(EXEDIR)/airattack.c128.d64 


# -------------------------------------------------------------------
# --- ALIENSTORM FOR C128 
# -------------------------------------------------------------------

.PHONY: alienstorm.embedded.c128
alienstorm.embedded.c128:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h

obj/alienstorm.c128/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c128 -c $(CFLAGS)  -Osir -Cl -D__ALIENSTORM__ -D__CBM__ -o $@ $(subst obj/alienstorm.c128/,src/,$(@:.o=.c))

$(EXEDIR)/alienstorm.c128: alienstorm.embedded.c128 $(subst PLATFORM,alienstorm.c128,$(OBJS))
	$(CC) -Ln democ128.lbl -t c128 -C cfg/c128.cfg $(LDFLAGS) -m $(EXEDIR)/alienstorm.c128.map -o $(EXEDIR)/alienstorm.c128  $(subst PLATFORM,alienstorm.c128,$(OBJS)) $(LIBDIR)/midres.c128.lib
	$(call RMFILES,$(EXEDIR)/alienstorm.c128.d64)
	$(CC1541) -f alienstorm -w $(EXEDIR)/alienstorm.c128 $(EXEDIR)/alienstorm.c128.d64 
	$(CC1541) -f zstiles.bin -w $(DATADIR)/astiles.bin $(EXEDIR)/alienstorm.c128.d64 
	$(CC1541) -f zstiles1.bin -w $(DATADIR)/astiles1.bin $(EXEDIR)/alienstorm.c128.d64 
	$(CC1541) -f zstiles2.bin -w $(DATADIR)/astiles2.bin $(EXEDIR)/alienstorm.c128.d64 


# -------------------------------------------------------------------
# --- ELEVATOR FOR C128 
# -------------------------------------------------------------------

.PHONY: elevator.embedded.c128
elevator.embedded.c128:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator64.mpic -n zelintro.bin -i $(DATADIR)/elevator1.imf -n elevator1.imf -c src/rawdata.c -h src/rawdata.h

obj/elevator.c128/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c128 -c $(CFLAGS)  -Osir -Cl -D__ELEVATOR__ -D__CBM__ -o $@ $(subst obj/elevator.c128/,src/,$(@:.o=.c))

$(EXEDIR)/elevator.c128: elevator.embedded.c128 $(subst PLATFORM,elevator.c128,$(OBJS))
	$(CC) -Ln democ128.lbl -t c128 -C cfg/c128.cfg $(LDFLAGS) -m $(EXEDIR)/elevator.c128.map -o $(EXEDIR)/elevator.c128  $(subst PLATFORM,elevator.c128,$(OBJS)) $(LIBDIR)/midres.c128.lib
	$(call RMFILES,$(EXEDIR)/elevator.c128.d64)
	$(CC1541) -f elevator -w $(EXEDIR)/elevator.c128 $(EXEDIR)/elevator.c128.d64 
	$(CC1541) -f zeltiles.bin -w $(DATADIR)/zeltiles.bin $(EXEDIR)/elevator.c128.d64 
	$(CC1541) -f zelintro.bin -w $(DATADIR)/elevator64.mpic $(EXEDIR)/elevator.c128.d64 
	$(CC1541) -f elevator1.imf -w $(DATADIR)/elevator1.imf $(EXEDIR)/elevator.c128.d64 


# -------------------------------------------------------------------
# --- JOYCHECK FOR C128 
# -------------------------------------------------------------------

.PHONY: joycheck.embedded.c128
joycheck.embedded.c128:
	$(FILE2INCLUDE) -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -c src/rawdata.c -h src/rawdata.h

obj/joycheck.c128/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c128 -c $(CFLAGS)  -Osir -Cl -D__JOYCHECK__ -D__CBM__ -o $@ $(subst obj/joycheck.c128/,src/,$(@:.o=.c))

$(EXEDIR)/joycheck.c128: joycheck.embedded.c128 $(subst PLATFORM,joycheck.c128,$(OBJS))
	$(CC) -Ln democ128.lbl -t c128 -C cfg/c128.cfg $(LDFLAGS) -m $(EXEDIR)/joycheck.c128.map -o $(EXEDIR)/joycheck.c128  $(subst PLATFORM,joycheck.c128,$(OBJS)) $(LIBDIR)/midres.c128.lib
	$(call RMFILES,$(EXEDIR)/joycheck.c128.d64)
	$(CC1541) -f joycheck -w $(EXEDIR)/joycheck.c128 $(EXEDIR)/joycheck.c128.d64 
	$(CC1541) -f zdjtiles.bin -w $(DATADIR)/zdjtiles.bin $(EXEDIR)/joycheck.c128.d64 


# -------------------------------------------------------------------
# --- TOTTO FOR C128 
# -------------------------------------------------------------------

.PHONY: totto.embedded.c128
totto.embedded.c128:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -i $(DATADIR)/ttfinal164.mpic -n ttfinal1.mpic -i $(DATADIR)/ttfinal264.mpic -n ttfinal2.mpic -i $(DATADIR)/ttfinal364.mpic -n ttfinal3.mpic -i $(DATADIR)/ttfinal464.mpic -n ttfinal4.mpic -i $(DATADIR)/totto.imf -n totto.imf -i $(DATADIR)/totto2.imf -n totto2.imf -i $(DATADIR)/totto3.imf -n totto3.imf -c src/rawdata.c -h src/rawdata.h

obj/totto.c128/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c128 -c $(CFLAGS)  -Osir -Cl -D__TOTTO__ -D__CBM__ -o $@ $(subst obj/totto.c128/,src/,$(@:.o=.c))

$(EXEDIR)/totto.c128: totto.embedded.c128 $(subst PLATFORM,totto.c128,$(OBJS))
	$(CC) -Ln democ128.lbl -t c128 -C cfg/c128.cfg $(LDFLAGS) -m $(EXEDIR)/totto.c128.map -o $(EXEDIR)/totto.c128  $(subst PLATFORM,totto.c128,$(OBJS)) $(LIBDIR)/midres.c128.lib
	$(call RMFILES,$(EXEDIR)/totto.c128.d64)
	$(CC1541) -f totto -w $(EXEDIR)/totto.c128 $(EXEDIR)/totto.c128.d64 
	$(CC1541) -f tttiles.bin -w $(DATADIR)/tttiles.bin $(EXEDIR)/totto.c128.d64 
	$(CC1541) -f tttiles1.bin -w $(DATADIR)/tttiles1.bin $(EXEDIR)/totto.c128.d64 
	$(CC1541) -f ttfinal1.mpic -w $(DATADIR)/ttfinal164.mpic $(EXEDIR)/totto.c128.d64 
	$(CC1541) -f ttfinal2.mpic -w $(DATADIR)/ttfinal264.mpic $(EXEDIR)/totto.c128.d64 
	$(CC1541) -f ttfinal3.mpic -w $(DATADIR)/ttfinal364.mpic $(EXEDIR)/totto.c128.d64 
	$(CC1541) -f ttfinal4.mpic -w $(DATADIR)/ttfinal464.mpic $(EXEDIR)/totto.c128.d64 
	$(CC1541) -f totto.imf -w $(DATADIR)/totto.imf $(EXEDIR)/totto.c128.d64 
	$(CC1541) -f totto2.imf -w $(DATADIR)/totto2.imf $(EXEDIR)/totto.c128.d64 
	$(CC1541) -f totto3.imf -w $(DATADIR)/totto3.imf $(EXEDIR)/totto.c128.d64 


# -------------------------------------------------------------------
# --- AIRATTACK FOR C16 
# -------------------------------------------------------------------

.PHONY: airattack.embedded.c16
airattack.embedded.c16:
	$(FILE2INCLUDE) -i $(DATADIR)/aatiles4.bin -n zztiles.bin -i $(DATADIR)/aaintro16.mpic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h

obj/airattack.c16/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c16 -c $(CFLAGS)  -Osir -Cl -D__AIRATTACK__ -D__CBM__ -o $@ $(subst obj/airattack.c16/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.c16: airattack.embedded.c16 $(subst PLATFORM,airattack.c16,$(OBJS))
	$(CC) -Ln democ16.lbl -t c16 -C cfg/c16.cfg $(LDFLAGS) -m $(EXEDIR)/airattack.c16.map -o $(EXEDIR)/airattack.c16  $(subst PLATFORM,airattack.c16,$(OBJS)) $(LIBDIR)/midres.c16.lib
	$(call RMFILES,$(EXEDIR)/airattack.c16.d64)
	$(CC1541) -f airattack -w $(EXEDIR)/airattack.c16 $(EXEDIR)/airattack.c16.d64 
	$(CC1541) -f  -w  $(EXEDIR)/airattack.c16.d64 
	$(CC1541) -f zztiles.bin -w $(DATADIR)/aatiles4.bin $(EXEDIR)/airattack.c16.d64 
	$(CC1541) -f zzintro.pic -w $(DATADIR)/aaintro16.mpic $(EXEDIR)/airattack.c16.d64 


# -------------------------------------------------------------------
# --- ALIENSTORM FOR C16 
# -------------------------------------------------------------------

.PHONY: alienstorm.embedded.c16
alienstorm.embedded.c16:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h

obj/alienstorm.c16/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c16 -c $(CFLAGS)  -Osir -Cl -D__ALIENSTORM__ -D__CBM__ -o $@ $(subst obj/alienstorm.c16/,src/,$(@:.o=.c))

$(EXEDIR)/alienstorm.c16: alienstorm.embedded.c16 $(subst PLATFORM,alienstorm.c16,$(OBJS))
	$(CC) -Ln democ16.lbl -t c16 -C cfg/c16.cfg $(LDFLAGS) -m $(EXEDIR)/alienstorm.c16.map -o $(EXEDIR)/alienstorm.c16  $(subst PLATFORM,alienstorm.c16,$(OBJS)) $(LIBDIR)/midres.c16.lib
	$(call RMFILES,$(EXEDIR)/alienstorm.c16.d64)
	$(CC1541) -f alienstorm -w $(EXEDIR)/alienstorm.c16 $(EXEDIR)/alienstorm.c16.d64 
	$(CC1541) -f zstiles.bin -w $(DATADIR)/astiles.bin $(EXEDIR)/alienstorm.c16.d64 
	$(CC1541) -f zstiles1.bin -w $(DATADIR)/astiles1.bin $(EXEDIR)/alienstorm.c16.d64 
	$(CC1541) -f zstiles2.bin -w $(DATADIR)/astiles2.bin $(EXEDIR)/alienstorm.c16.d64 


# -------------------------------------------------------------------
# --- ELEVATOR FOR C16 
# -------------------------------------------------------------------

.PHONY: elevator.embedded.c16
elevator.embedded.c16:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator16.mpic -n zelintro.bin -i $(DATADIR)/elevator1.imf -n elevator1.imf -c src/rawdata.c -h src/rawdata.h

obj/elevator.c16/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c16 -c $(CFLAGS)  -Osir -Cl -D__ELEVATOR__ -D__CBM__ -o $@ $(subst obj/elevator.c16/,src/,$(@:.o=.c))

$(EXEDIR)/elevator.c16: elevator.embedded.c16 $(subst PLATFORM,elevator.c16,$(OBJS))
	$(CC) -Ln democ16.lbl -t c16 -C cfg/c16.cfg $(LDFLAGS) -m $(EXEDIR)/elevator.c16.map -o $(EXEDIR)/elevator.c16  $(subst PLATFORM,elevator.c16,$(OBJS)) $(LIBDIR)/midres.c16.lib
	$(call RMFILES,$(EXEDIR)/elevator.c16.d64)
	$(CC1541) -f elevator -w $(EXEDIR)/elevator.c16 $(EXEDIR)/elevator.c16.d64 
	$(CC1541) -f  -w  $(EXEDIR)/elevator.c16.d64 
	$(CC1541) -f zeltiles.bin -w $(DATADIR)/zeltiles.bin $(EXEDIR)/elevator.c16.d64 
	$(CC1541) -f zelintro.bin -w $(DATADIR)/elevator16.mpic $(EXEDIR)/elevator.c16.d64 
	$(CC1541) -f elevator1.imf -w $(DATADIR)/elevator1.imf $(EXEDIR)/elevator.c16.d64 


# -------------------------------------------------------------------
# --- JOYCHECK FOR C16 
# -------------------------------------------------------------------

.PHONY: joycheck.embedded.c16
joycheck.embedded.c16:
	$(FILE2INCLUDE) -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -c src/rawdata.c -h src/rawdata.h

obj/joycheck.c16/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c16 -c $(CFLAGS)  -Osir -Cl -D__JOYCHECK__ -D__CBM__ -o $@ $(subst obj/joycheck.c16/,src/,$(@:.o=.c))

$(EXEDIR)/joycheck.c16: joycheck.embedded.c16 $(subst PLATFORM,joycheck.c16,$(OBJS))
	$(CC) -Ln democ16.lbl -t c16 -C cfg/c16.cfg $(LDFLAGS) -m $(EXEDIR)/joycheck.c16.map -o $(EXEDIR)/joycheck.c16  $(subst PLATFORM,joycheck.c16,$(OBJS)) $(LIBDIR)/midres.c16.lib
	$(call RMFILES,$(EXEDIR)/joycheck.c16.d64)
	$(CC1541) -f joycheck -w $(EXEDIR)/joycheck.c16 $(EXEDIR)/joycheck.c16.d64 
	$(CC1541) -f  -w  $(EXEDIR)/joycheck.c16.d64 
	$(CC1541) -f zdjtiles.bin -w $(DATADIR)/zdjtiles.bin $(EXEDIR)/joycheck.c16.d64 


# -------------------------------------------------------------------
# --- TOTTO FOR C16 
# -------------------------------------------------------------------

.PHONY: totto.embedded.c16
totto.embedded.c16:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -i $(DATADIR)/ttfinal116.mpic -n ttfinal1.mpic -i $(DATADIR)/ttfinal216.mpic -n ttfinal2.mpic -i $(DATADIR)/ttfinal316.mpic -n ttfinal3.mpic -i $(DATADIR)/ttfinal416.mpic -n ttfinal4.mpic -i $(DATADIR)/totto.imf -n totto.imf -i $(DATADIR)/totto3.imf -n totto3.imf -c src/rawdata.c -h src/rawdata.h

obj/totto.c16/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c16 -c $(CFLAGS)  -Osir -Cl -D__TOTTO__ -D__CBM__ -o $@ $(subst obj/totto.c16/,src/,$(@:.o=.c))

$(EXEDIR)/totto.c16: totto.embedded.c16 $(subst PLATFORM,totto.c16,$(OBJS))
	$(CC) -Ln democ16.lbl -t c16 -C cfg/c16.cfg $(LDFLAGS) -m $(EXEDIR)/totto.c16.map -o $(EXEDIR)/totto.c16  $(subst PLATFORM,totto.c16,$(OBJS)) $(LIBDIR)/midres.c16.lib
	$(call RMFILES,$(EXEDIR)/totto.c16.d64)
	$(CC1541) -f totto -w $(EXEDIR)/totto.c16 $(EXEDIR)/totto.c16.d64 
	$(CC1541) -f  -w  $(EXEDIR)/totto.c16.d64 
	$(CC1541) -f tttiles.bin -w $(DATADIR)/tttiles.bin $(EXEDIR)/totto.c16.d64 
	$(CC1541) -f tttiles1.bin -w $(DATADIR)/tttiles1.bin $(EXEDIR)/totto.c16.d64 
	$(CC1541) -f ttfinal1.mpic -w $(DATADIR)/ttfinal116.mpic $(EXEDIR)/totto.c16.d64 
	$(CC1541) -f ttfinal2.mpic -w $(DATADIR)/ttfinal216.mpic $(EXEDIR)/totto.c16.d64 
	$(CC1541) -f ttfinal3.mpic -w $(DATADIR)/ttfinal316.mpic $(EXEDIR)/totto.c16.d64 
	$(CC1541) -f ttfinal4.mpic -w $(DATADIR)/ttfinal416.mpic $(EXEDIR)/totto.c16.d64 
	$(CC1541) -f totto.imf -w $(DATADIR)/totto.imf $(EXEDIR)/totto.c16.d64 
	$(CC1541) -f totto3.imf -w $(DATADIR)/totto3.imf $(EXEDIR)/totto.c16.d64 


# -------------------------------------------------------------------
# --- AIRATTACK FOR C64 
# -------------------------------------------------------------------

.PHONY: airattack.embedded.c64
airattack.embedded.c64:
	$(FILE2INCLUDE) -i $(DATADIR)/aatiles.bin -n zztiles.bin -i $(DATADIR)/aaintro64.mpic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h

obj/airattack.c64/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c64 -c $(CFLAGS)  -Osir -Cl -D__AIRATTACK__ -D__CBM__ -o $@ $(subst obj/airattack.c64/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.c64: airattack.embedded.c64 $(subst PLATFORM,airattack.c64,$(OBJS))
	$(CC) -Ln democ64.lbl -t c64 -C cfg/c64.cfg $(LDFLAGS) -m $(EXEDIR)/airattack.c64.map -o $(EXEDIR)/airattack.c64  $(subst PLATFORM,airattack.c64,$(OBJS)) $(LIBDIR)/midres.c64.lib
	$(call RMFILES,$(EXEDIR)/airattack.c64.d64)
	$(CC1541) -f airattack -w $(EXEDIR)/airattack.c64 $(EXEDIR)/airattack.c64.d64 
	$(CC1541) -f zztiles.bin -w $(DATADIR)/aatiles.bin $(EXEDIR)/airattack.c64.d64 
	$(CC1541) -f zzintro.pic -w $(DATADIR)/aaintro64.mpic $(EXEDIR)/airattack.c64.d64 


# -------------------------------------------------------------------
# --- ALIENSTORM FOR C64 
# -------------------------------------------------------------------

.PHONY: alienstorm.embedded.c64
alienstorm.embedded.c64:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h

obj/alienstorm.c64/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c64 -c $(CFLAGS)  -Osir -Cl -D__ALIENSTORM__ -D__CBM__ -o $@ $(subst obj/alienstorm.c64/,src/,$(@:.o=.c))

$(EXEDIR)/alienstorm.c64: alienstorm.embedded.c64 $(subst PLATFORM,alienstorm.c64,$(OBJS))
	$(CC) -Ln democ64.lbl -t c64 -C cfg/c64.cfg $(LDFLAGS) -m $(EXEDIR)/alienstorm.c64.map -o $(EXEDIR)/alienstorm.c64  $(subst PLATFORM,alienstorm.c64,$(OBJS)) $(LIBDIR)/midres.c64.lib
	$(call RMFILES,$(EXEDIR)/alienstorm.c64.d64)
	$(CC1541) -f alienstorm -w $(EXEDIR)/alienstorm.c64 $(EXEDIR)/alienstorm.c64.d64 
	$(CC1541) -f zstiles.bin -w $(DATADIR)/astiles.bin $(EXEDIR)/alienstorm.c64.d64 
	$(CC1541) -f zstiles1.bin -w $(DATADIR)/astiles1.bin $(EXEDIR)/alienstorm.c64.d64 
	$(CC1541) -f zstiles2.bin -w $(DATADIR)/astiles2.bin $(EXEDIR)/alienstorm.c64.d64 


# -------------------------------------------------------------------
# --- ELEVATOR FOR C64 
# -------------------------------------------------------------------

.PHONY: elevator.embedded.c64
elevator.embedded.c64:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator64.mpic -n zelintro.bin -i $(DATADIR)/elevator1.imf -n elevator1.imf -c src/rawdata.c -h src/rawdata.h

obj/elevator.c64/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c64 -c $(CFLAGS)  -Osir -Cl -D__ELEVATOR__ -D__CBM__ -o $@ $(subst obj/elevator.c64/,src/,$(@:.o=.c))

$(EXEDIR)/elevator.c64: elevator.embedded.c64 $(subst PLATFORM,elevator.c64,$(OBJS))
	$(CC) -Ln democ64.lbl -t c64 -C cfg/c64.cfg $(LDFLAGS) -m $(EXEDIR)/elevator.c64.map -o $(EXEDIR)/elevator.c64  $(subst PLATFORM,elevator.c64,$(OBJS)) $(LIBDIR)/midres.c64.lib
	$(call RMFILES,$(EXEDIR)/elevator.c64.d64)
	$(CC1541) -f elevator -w $(EXEDIR)/elevator.c64 $(EXEDIR)/elevator.c64.d64 
	$(CC1541) -f zeltiles.bin -w $(DATADIR)/zeltiles.bin $(EXEDIR)/elevator.c64.d64 
	$(CC1541) -f zelintro.bin -w $(DATADIR)/elevator64.mpic $(EXEDIR)/elevator.c64.d64 
	$(CC1541) -f elevator1.imf -w $(DATADIR)/elevator1.imf $(EXEDIR)/elevator.c64.d64 


# -------------------------------------------------------------------
# --- JOYCHECK FOR C64 
# -------------------------------------------------------------------

.PHONY: joycheck.embedded.c64
joycheck.embedded.c64:
	$(FILE2INCLUDE) -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -c src/rawdata.c -h src/rawdata.h

obj/joycheck.c64/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c64 -c $(CFLAGS)  -Osir -Cl -D__JOYCHECK__ -D__CBM__ -o $@ $(subst obj/joycheck.c64/,src/,$(@:.o=.c))

$(EXEDIR)/joycheck.c64: joycheck.embedded.c64 $(subst PLATFORM,joycheck.c64,$(OBJS))
	$(CC) -Ln democ64.lbl -t c64 -C cfg/c64.cfg $(LDFLAGS) -m $(EXEDIR)/joycheck.c64.map -o $(EXEDIR)/joycheck.c64  $(subst PLATFORM,joycheck.c64,$(OBJS)) $(LIBDIR)/midres.c64.lib
	$(call RMFILES,$(EXEDIR)/joycheck.c64.d64)
	$(CC1541) -f joycheck -w $(EXEDIR)/joycheck.c64 $(EXEDIR)/joycheck.c64.d64 
	$(CC1541) -f zdjtiles.bin -w $(DATADIR)/zdjtiles.bin $(EXEDIR)/joycheck.c64.d64 


# -------------------------------------------------------------------
# --- TOTTO FOR C64 
# -------------------------------------------------------------------

.PHONY: totto.embedded.c64
totto.embedded.c64:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -i $(DATADIR)/ttfinal164.mpic -n ttfinal1.mpic -i $(DATADIR)/ttfinal264.mpic -n ttfinal2.mpic -i $(DATADIR)/ttfinal364.mpic -n ttfinal3.mpic -i $(DATADIR)/ttfinal464.mpic -n ttfinal4.mpic -i $(DATADIR)/totto.imf -n totto.imf -i $(DATADIR)/totto2.imf -n totto2.imf -i $(DATADIR)/totto3.imf -n totto3.imf -c src/rawdata.c -h src/rawdata.h

obj/totto.c64/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c64 -c $(CFLAGS)  -Osir -Cl -D__TOTTO__ -D__CBM__ -o $@ $(subst obj/totto.c64/,src/,$(@:.o=.c))

$(EXEDIR)/totto.c64: totto.embedded.c64 $(subst PLATFORM,totto.c64,$(OBJS))
	$(CC) -Ln democ64.lbl -t c64 -C cfg/c64.cfg $(LDFLAGS) -m $(EXEDIR)/totto.c64.map -o $(EXEDIR)/totto.c64  $(subst PLATFORM,totto.c64,$(OBJS)) $(LIBDIR)/midres.c64.lib
	$(call RMFILES,$(EXEDIR)/totto.c64.d64)
	$(CC1541) -f totto -w $(EXEDIR)/totto.c64 $(EXEDIR)/totto.c64.d64 
	$(CC1541) -f tttiles.bin -w $(DATADIR)/tttiles.bin $(EXEDIR)/totto.c64.d64 
	$(CC1541) -f tttiles1.bin -w $(DATADIR)/tttiles1.bin $(EXEDIR)/totto.c64.d64 
	$(CC1541) -f ttfinal1.mpic -w $(DATADIR)/ttfinal164.mpic $(EXEDIR)/totto.c64.d64 
	$(CC1541) -f ttfinal2.mpic -w $(DATADIR)/ttfinal264.mpic $(EXEDIR)/totto.c64.d64 
	$(CC1541) -f ttfinal3.mpic -w $(DATADIR)/ttfinal364.mpic $(EXEDIR)/totto.c64.d64 
	$(CC1541) -f ttfinal4.mpic -w $(DATADIR)/ttfinal464.mpic $(EXEDIR)/totto.c64.d64 
	$(CC1541) -f totto.imf -w $(DATADIR)/totto.imf $(EXEDIR)/totto.c64.d64 
	$(CC1541) -f totto2.imf -w $(DATADIR)/totto2.imf $(EXEDIR)/totto.c64.d64 
	$(CC1541) -f totto3.imf -w $(DATADIR)/totto3.imf $(EXEDIR)/totto.c64.d64 

# find_main_module(airattack)
# -> src/game_air_attack.c

# -------------------------------------------------------------------
# --- AIRATTACK FOR COLECO 
# -------------------------------------------------------------------
# 

.PHONY: airattack.embedded.coleco
airattack.embedded.coleco:
	$(FILE2INCLUDE) -i $(DATADIR)/ztiles.bin -n ztiles.bin -i $(DATADIR)/aatiles4.bin -n zztiles.bin -i $(DATADIR)/aaintrox.pic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/airattack.coleco/rawdata.o src/rawdata.c

obj/airattack.coleco/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.coleco/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/airattack.coleco/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.coleco/midres_io.o src/midres_io.asm

obj/airattack.coleco/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__AIRATTACK__ -o $@ $(subst obj/airattack.coleco/,src/,$(@:.o=.c)) 

$(EXEDIR)/airattack.coleco:	airattack.embedded.coleco obj/airattack.coleco/demo_music.o obj/airattack.coleco/demo_tile.o obj/airattack.coleco/game_air_attack.o obj/airattack.coleco/game_alien_storm.o obj/airattack.coleco/game_elevator.o obj/airattack.coleco/game_totto.o obj/airattack.coleco/midres.o obj/airattack.coleco/midres_atari.o obj/airattack.coleco/midres_atmos.o obj/airattack.coleco/midres_ay8910.o obj/airattack.coleco/midres_c128.o obj/airattack.coleco/midres_c16.o obj/airattack.coleco/midres_c64.o obj/airattack.coleco/midres_coleco.o obj/airattack.coleco/midres_control_000.o obj/airattack.coleco/midres_control_001.o obj/airattack.coleco/midres_control_002.o obj/airattack.coleco/midres_control_003.o obj/airattack.coleco/midres_control_004.o obj/airattack.coleco/midres_control_005.o obj/airattack.coleco/midres_control_007.o obj/airattack.coleco/midres_control_010.o obj/airattack.coleco/midres_file_001.o obj/airattack.coleco/midres_file_003.o obj/airattack.coleco/midres_frame_000.o obj/airattack.coleco/midres_frame_001.o obj/airattack.coleco/midres_gb.o obj/airattack.coleco/midres_lm80c.o obj/airattack.coleco/midres_msx.o obj/airattack.coleco/midres_mtx500.o obj/airattack.coleco/midres_music_001.o obj/airattack.coleco/midres_music_002.o obj/airattack.coleco/midres_music_004.o obj/airattack.coleco/midres_music_005.o obj/airattack.coleco/midres_music_006.o obj/airattack.coleco/midres_plus4.o obj/airattack.coleco/midres_pokey.o obj/airattack.coleco/midres_screen_000.o obj/airattack.coleco/midres_screen_006.o obj/airattack.coleco/midres_screen_007.o obj/airattack.coleco/midres_screen_010.o obj/airattack.coleco/midres_screen_012.o obj/airattack.coleco/midres_screen_013.o obj/airattack.coleco/midres_sid.o obj/airattack.coleco/midres_sound_000.o obj/airattack.coleco/midres_sound_001.o obj/airattack.coleco/midres_sound_002.o obj/airattack.coleco/midres_sound_005.o obj/airattack.coleco/midres_sound_006.o obj/airattack.coleco/midres_sound_007.o obj/airattack.coleco/midres_svi.o obj/airattack.coleco/midres_ted.o obj/airattack.coleco/midres_tile_000.o obj/airattack.coleco/midres_tile_004.o obj/airattack.coleco/midres_tile_005.o obj/airattack.coleco/midres_tile_006.o obj/airattack.coleco/midres_tile_007.o obj/airattack.coleco/midres_tile_008.o obj/airattack.coleco/midres_tile_009.o obj/airattack.coleco/midres_tile_010.o obj/airattack.coleco/midres_tile_011.o obj/airattack.coleco/midres_tile_012.o obj/airattack.coleco/midres_tile_013.o obj/airattack.coleco/midres_tile_014.o obj/airattack.coleco/midres_tile_015.o obj/airattack.coleco/midres_tile_016.o obj/airattack.coleco/midres_tile_018.o obj/airattack.coleco/midres_tile_019.o obj/airattack.coleco/midres_tile_020.o obj/airattack.coleco/midres_tile_021.o obj/airattack.coleco/midres_tile_022.o obj/airattack.coleco/midres_tile_023.o obj/airattack.coleco/midres_tile_025.o obj/airattack.coleco/midres_tile_026.o obj/airattack.coleco/midres_tile_027.o obj/airattack.coleco/midres_tile_028.o obj/airattack.coleco/midres_tile_multicolor_000.o obj/airattack.coleco/midres_tile_multicolor_001.o obj/airattack.coleco/midres_tile_multicolor_002.o obj/airattack.coleco/midres_tile_multicolor_003.o obj/airattack.coleco/midres_tile_multicolor_004.o obj/airattack.coleco/midres_tile_multicolor_005.o obj/airattack.coleco/midres_tile_multicolor_006.o obj/airattack.coleco/midres_tile_multicolor_007.o obj/airattack.coleco/midres_tile_multicolor_008.o obj/airattack.coleco/midres_tile_multicolor_009.o obj/airattack.coleco/midres_tile_multicolor_010.o obj/airattack.coleco/midres_tile_multicolor_011.o obj/airattack.coleco/midres_tile_multicolor_012.o obj/airattack.coleco/midres_tile_processing_001.o obj/airattack.coleco/midres_tile_processing_002.o obj/airattack.coleco/midres_tile_processing_004.o obj/airattack.coleco/midres_tile_processing_005.o obj/airattack.coleco/midres_tile_processing_007.o obj/airattack.coleco/midres_tile_processing_009.o obj/airattack.coleco/midres_tile_processing_011.o obj/airattack.coleco/midres_tile_processing_012.o obj/airattack.coleco/midres_tile_processing_016.o obj/airattack.coleco/midres_vanilla.o obj/airattack.coleco/midres_vdp.o obj/airattack.coleco/midres_vic.o obj/airattack.coleco/midres_vic20.o obj/airattack.coleco/utility_joycheck.o obj/airattack.coleco/midres_control_011.o obj/airattack.coleco/midres_data.o obj/airattack.coleco/main.o obj/airattack.coleco/rawdata.o obj/airattack.coleco/midres_vdp_impl.o obj/airattack.coleco/midres_io.o
	$(CC88) +coleco  -m $(LDFLAGS88) obj/airattack.coleco/rawdata.o obj/airattack.coleco/midres_io.o obj/airattack.coleco/midres_vdp_impl.o obj/airattack.coleco/demo_music.o obj/airattack.coleco/demo_tile.o obj/airattack.coleco/game_air_attack.o obj/airattack.coleco/game_alien_storm.o obj/airattack.coleco/game_elevator.o obj/airattack.coleco/game_totto.o obj/airattack.coleco/midres.o obj/airattack.coleco/midres_atari.o obj/airattack.coleco/midres_atmos.o obj/airattack.coleco/midres_ay8910.o obj/airattack.coleco/midres_c128.o obj/airattack.coleco/midres_c16.o obj/airattack.coleco/midres_c64.o obj/airattack.coleco/midres_coleco.o obj/airattack.coleco/midres_control_000.o obj/airattack.coleco/midres_control_001.o obj/airattack.coleco/midres_control_002.o obj/airattack.coleco/midres_control_003.o obj/airattack.coleco/midres_control_004.o obj/airattack.coleco/midres_control_005.o obj/airattack.coleco/midres_control_007.o obj/airattack.coleco/midres_control_010.o obj/airattack.coleco/midres_file_001.o obj/airattack.coleco/midres_file_003.o obj/airattack.coleco/midres_frame_000.o obj/airattack.coleco/midres_frame_001.o obj/airattack.coleco/midres_gb.o obj/airattack.coleco/midres_lm80c.o obj/airattack.coleco/midres_msx.o obj/airattack.coleco/midres_mtx500.o obj/airattack.coleco/midres_music_001.o obj/airattack.coleco/midres_music_002.o obj/airattack.coleco/midres_music_004.o obj/airattack.coleco/midres_music_005.o obj/airattack.coleco/midres_music_006.o obj/airattack.coleco/midres_plus4.o obj/airattack.coleco/midres_pokey.o obj/airattack.coleco/midres_screen_000.o obj/airattack.coleco/midres_screen_006.o obj/airattack.coleco/midres_screen_007.o obj/airattack.coleco/midres_screen_010.o obj/airattack.coleco/midres_screen_012.o obj/airattack.coleco/midres_screen_013.o obj/airattack.coleco/midres_sid.o obj/airattack.coleco/midres_sound_000.o obj/airattack.coleco/midres_sound_001.o obj/airattack.coleco/midres_sound_002.o obj/airattack.coleco/midres_sound_005.o obj/airattack.coleco/midres_sound_006.o obj/airattack.coleco/midres_sound_007.o obj/airattack.coleco/midres_svi.o obj/airattack.coleco/midres_ted.o obj/airattack.coleco/midres_tile_000.o obj/airattack.coleco/midres_tile_004.o obj/airattack.coleco/midres_tile_005.o obj/airattack.coleco/midres_tile_006.o obj/airattack.coleco/midres_tile_007.o obj/airattack.coleco/midres_tile_008.o obj/airattack.coleco/midres_tile_009.o obj/airattack.coleco/midres_tile_010.o obj/airattack.coleco/midres_tile_011.o obj/airattack.coleco/midres_tile_012.o obj/airattack.coleco/midres_tile_013.o obj/airattack.coleco/midres_tile_014.o obj/airattack.coleco/midres_tile_015.o obj/airattack.coleco/midres_tile_016.o obj/airattack.coleco/midres_tile_018.o obj/airattack.coleco/midres_tile_019.o obj/airattack.coleco/midres_tile_020.o obj/airattack.coleco/midres_tile_021.o obj/airattack.coleco/midres_tile_022.o obj/airattack.coleco/midres_tile_023.o obj/airattack.coleco/midres_tile_025.o obj/airattack.coleco/midres_tile_026.o obj/airattack.coleco/midres_tile_027.o obj/airattack.coleco/midres_tile_028.o obj/airattack.coleco/midres_tile_multicolor_000.o obj/airattack.coleco/midres_tile_multicolor_001.o obj/airattack.coleco/midres_tile_multicolor_002.o obj/airattack.coleco/midres_tile_multicolor_003.o obj/airattack.coleco/midres_tile_multicolor_004.o obj/airattack.coleco/midres_tile_multicolor_005.o obj/airattack.coleco/midres_tile_multicolor_006.o obj/airattack.coleco/midres_tile_multicolor_007.o obj/airattack.coleco/midres_tile_multicolor_008.o obj/airattack.coleco/midres_tile_multicolor_009.o obj/airattack.coleco/midres_tile_multicolor_010.o obj/airattack.coleco/midres_tile_multicolor_011.o obj/airattack.coleco/midres_tile_multicolor_012.o obj/airattack.coleco/midres_tile_processing_001.o obj/airattack.coleco/midres_tile_processing_002.o obj/airattack.coleco/midres_tile_processing_004.o obj/airattack.coleco/midres_tile_processing_005.o obj/airattack.coleco/midres_tile_processing_007.o obj/airattack.coleco/midres_tile_processing_009.o obj/airattack.coleco/midres_tile_processing_011.o obj/airattack.coleco/midres_tile_processing_012.o obj/airattack.coleco/midres_tile_processing_016.o obj/airattack.coleco/midres_vanilla.o obj/airattack.coleco/midres_vdp.o obj/airattack.coleco/midres_vic.o obj/airattack.coleco/midres_vic20.o obj/airattack.coleco/utility_joycheck.o obj/airattack.coleco/midres_control_011.o obj/airattack.coleco/midres_data.o obj/airattack.coleco/main.o -create-app 
	$(call COPYFILES,a.rom,$(EXEDIR)/airattack.coleco.rom)
	$(call RMFILES,a.*)

# find_main_module(alienstorm)
# -> src/game_alien_storm.c

# -------------------------------------------------------------------
# --- ALIENSTORM FOR COLECO 
# -------------------------------------------------------------------
# 

.PHONY: alienstorm.embedded.coleco
alienstorm.embedded.coleco:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/alienstorm.coleco/rawdata.o src/rawdata.c

obj/alienstorm.coleco/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.coleco/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/alienstorm.coleco/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.coleco/midres_io.o src/midres_io.asm

obj/alienstorm.coleco/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__ALIENSTORM__ -o $@ $(subst obj/alienstorm.coleco/,src/,$(@:.o=.c)) 

$(EXEDIR)/alienstorm.coleco:	alienstorm.embedded.coleco obj/alienstorm.coleco/demo_music.o obj/alienstorm.coleco/demo_tile.o obj/alienstorm.coleco/game_air_attack.o obj/alienstorm.coleco/game_alien_storm.o obj/alienstorm.coleco/game_elevator.o obj/alienstorm.coleco/game_totto.o obj/alienstorm.coleco/midres.o obj/alienstorm.coleco/midres_atari.o obj/alienstorm.coleco/midres_atmos.o obj/alienstorm.coleco/midres_ay8910.o obj/alienstorm.coleco/midres_c128.o obj/alienstorm.coleco/midres_c16.o obj/alienstorm.coleco/midres_c64.o obj/alienstorm.coleco/midres_coleco.o obj/alienstorm.coleco/midres_control_000.o obj/alienstorm.coleco/midres_control_001.o obj/alienstorm.coleco/midres_control_002.o obj/alienstorm.coleco/midres_control_003.o obj/alienstorm.coleco/midres_control_004.o obj/alienstorm.coleco/midres_control_005.o obj/alienstorm.coleco/midres_control_007.o obj/alienstorm.coleco/midres_control_010.o obj/alienstorm.coleco/midres_file_001.o obj/alienstorm.coleco/midres_file_003.o obj/alienstorm.coleco/midres_frame_000.o obj/alienstorm.coleco/midres_frame_001.o obj/alienstorm.coleco/midres_gb.o obj/alienstorm.coleco/midres_lm80c.o obj/alienstorm.coleco/midres_msx.o obj/alienstorm.coleco/midres_mtx500.o obj/alienstorm.coleco/midres_music_001.o obj/alienstorm.coleco/midres_music_002.o obj/alienstorm.coleco/midres_music_004.o obj/alienstorm.coleco/midres_music_005.o obj/alienstorm.coleco/midres_music_006.o obj/alienstorm.coleco/midres_plus4.o obj/alienstorm.coleco/midres_pokey.o obj/alienstorm.coleco/midres_screen_000.o obj/alienstorm.coleco/midres_screen_006.o obj/alienstorm.coleco/midres_screen_007.o obj/alienstorm.coleco/midres_screen_010.o obj/alienstorm.coleco/midres_screen_012.o obj/alienstorm.coleco/midres_screen_013.o obj/alienstorm.coleco/midres_sid.o obj/alienstorm.coleco/midres_sound_000.o obj/alienstorm.coleco/midres_sound_001.o obj/alienstorm.coleco/midres_sound_002.o obj/alienstorm.coleco/midres_sound_005.o obj/alienstorm.coleco/midres_sound_006.o obj/alienstorm.coleco/midres_sound_007.o obj/alienstorm.coleco/midres_svi.o obj/alienstorm.coleco/midres_ted.o obj/alienstorm.coleco/midres_tile_000.o obj/alienstorm.coleco/midres_tile_004.o obj/alienstorm.coleco/midres_tile_005.o obj/alienstorm.coleco/midres_tile_006.o obj/alienstorm.coleco/midres_tile_007.o obj/alienstorm.coleco/midres_tile_008.o obj/alienstorm.coleco/midres_tile_009.o obj/alienstorm.coleco/midres_tile_010.o obj/alienstorm.coleco/midres_tile_011.o obj/alienstorm.coleco/midres_tile_012.o obj/alienstorm.coleco/midres_tile_013.o obj/alienstorm.coleco/midres_tile_014.o obj/alienstorm.coleco/midres_tile_015.o obj/alienstorm.coleco/midres_tile_016.o obj/alienstorm.coleco/midres_tile_018.o obj/alienstorm.coleco/midres_tile_019.o obj/alienstorm.coleco/midres_tile_020.o obj/alienstorm.coleco/midres_tile_021.o obj/alienstorm.coleco/midres_tile_022.o obj/alienstorm.coleco/midres_tile_023.o obj/alienstorm.coleco/midres_tile_025.o obj/alienstorm.coleco/midres_tile_026.o obj/alienstorm.coleco/midres_tile_027.o obj/alienstorm.coleco/midres_tile_028.o obj/alienstorm.coleco/midres_tile_multicolor_000.o obj/alienstorm.coleco/midres_tile_multicolor_001.o obj/alienstorm.coleco/midres_tile_multicolor_002.o obj/alienstorm.coleco/midres_tile_multicolor_003.o obj/alienstorm.coleco/midres_tile_multicolor_004.o obj/alienstorm.coleco/midres_tile_multicolor_005.o obj/alienstorm.coleco/midres_tile_multicolor_006.o obj/alienstorm.coleco/midres_tile_multicolor_007.o obj/alienstorm.coleco/midres_tile_multicolor_008.o obj/alienstorm.coleco/midres_tile_multicolor_009.o obj/alienstorm.coleco/midres_tile_multicolor_010.o obj/alienstorm.coleco/midres_tile_multicolor_011.o obj/alienstorm.coleco/midres_tile_multicolor_012.o obj/alienstorm.coleco/midres_tile_processing_001.o obj/alienstorm.coleco/midres_tile_processing_002.o obj/alienstorm.coleco/midres_tile_processing_004.o obj/alienstorm.coleco/midres_tile_processing_005.o obj/alienstorm.coleco/midres_tile_processing_007.o obj/alienstorm.coleco/midres_tile_processing_009.o obj/alienstorm.coleco/midres_tile_processing_011.o obj/alienstorm.coleco/midres_tile_processing_012.o obj/alienstorm.coleco/midres_tile_processing_016.o obj/alienstorm.coleco/midres_vanilla.o obj/alienstorm.coleco/midres_vdp.o obj/alienstorm.coleco/midres_vic.o obj/alienstorm.coleco/midres_vic20.o obj/alienstorm.coleco/utility_joycheck.o obj/alienstorm.coleco/midres_control_011.o obj/alienstorm.coleco/midres_data.o obj/alienstorm.coleco/main.o obj/alienstorm.coleco/rawdata.o obj/alienstorm.coleco/midres_vdp_impl.o obj/alienstorm.coleco/midres_io.o
	$(CC88) +coleco  -m $(LDFLAGS88) obj/alienstorm.coleco/rawdata.o obj/alienstorm.coleco/midres_io.o obj/alienstorm.coleco/midres_vdp_impl.o obj/alienstorm.coleco/demo_music.o obj/alienstorm.coleco/demo_tile.o obj/alienstorm.coleco/game_air_attack.o obj/alienstorm.coleco/game_alien_storm.o obj/alienstorm.coleco/game_elevator.o obj/alienstorm.coleco/game_totto.o obj/alienstorm.coleco/midres.o obj/alienstorm.coleco/midres_atari.o obj/alienstorm.coleco/midres_atmos.o obj/alienstorm.coleco/midres_ay8910.o obj/alienstorm.coleco/midres_c128.o obj/alienstorm.coleco/midres_c16.o obj/alienstorm.coleco/midres_c64.o obj/alienstorm.coleco/midres_coleco.o obj/alienstorm.coleco/midres_control_000.o obj/alienstorm.coleco/midres_control_001.o obj/alienstorm.coleco/midres_control_002.o obj/alienstorm.coleco/midres_control_003.o obj/alienstorm.coleco/midres_control_004.o obj/alienstorm.coleco/midres_control_005.o obj/alienstorm.coleco/midres_control_007.o obj/alienstorm.coleco/midres_control_010.o obj/alienstorm.coleco/midres_file_001.o obj/alienstorm.coleco/midres_file_003.o obj/alienstorm.coleco/midres_frame_000.o obj/alienstorm.coleco/midres_frame_001.o obj/alienstorm.coleco/midres_gb.o obj/alienstorm.coleco/midres_lm80c.o obj/alienstorm.coleco/midres_msx.o obj/alienstorm.coleco/midres_mtx500.o obj/alienstorm.coleco/midres_music_001.o obj/alienstorm.coleco/midres_music_002.o obj/alienstorm.coleco/midres_music_004.o obj/alienstorm.coleco/midres_music_005.o obj/alienstorm.coleco/midres_music_006.o obj/alienstorm.coleco/midres_plus4.o obj/alienstorm.coleco/midres_pokey.o obj/alienstorm.coleco/midres_screen_000.o obj/alienstorm.coleco/midres_screen_006.o obj/alienstorm.coleco/midres_screen_007.o obj/alienstorm.coleco/midres_screen_010.o obj/alienstorm.coleco/midres_screen_012.o obj/alienstorm.coleco/midres_screen_013.o obj/alienstorm.coleco/midres_sid.o obj/alienstorm.coleco/midres_sound_000.o obj/alienstorm.coleco/midres_sound_001.o obj/alienstorm.coleco/midres_sound_002.o obj/alienstorm.coleco/midres_sound_005.o obj/alienstorm.coleco/midres_sound_006.o obj/alienstorm.coleco/midres_sound_007.o obj/alienstorm.coleco/midres_svi.o obj/alienstorm.coleco/midres_ted.o obj/alienstorm.coleco/midres_tile_000.o obj/alienstorm.coleco/midres_tile_004.o obj/alienstorm.coleco/midres_tile_005.o obj/alienstorm.coleco/midres_tile_006.o obj/alienstorm.coleco/midres_tile_007.o obj/alienstorm.coleco/midres_tile_008.o obj/alienstorm.coleco/midres_tile_009.o obj/alienstorm.coleco/midres_tile_010.o obj/alienstorm.coleco/midres_tile_011.o obj/alienstorm.coleco/midres_tile_012.o obj/alienstorm.coleco/midres_tile_013.o obj/alienstorm.coleco/midres_tile_014.o obj/alienstorm.coleco/midres_tile_015.o obj/alienstorm.coleco/midres_tile_016.o obj/alienstorm.coleco/midres_tile_018.o obj/alienstorm.coleco/midres_tile_019.o obj/alienstorm.coleco/midres_tile_020.o obj/alienstorm.coleco/midres_tile_021.o obj/alienstorm.coleco/midres_tile_022.o obj/alienstorm.coleco/midres_tile_023.o obj/alienstorm.coleco/midres_tile_025.o obj/alienstorm.coleco/midres_tile_026.o obj/alienstorm.coleco/midres_tile_027.o obj/alienstorm.coleco/midres_tile_028.o obj/alienstorm.coleco/midres_tile_multicolor_000.o obj/alienstorm.coleco/midres_tile_multicolor_001.o obj/alienstorm.coleco/midres_tile_multicolor_002.o obj/alienstorm.coleco/midres_tile_multicolor_003.o obj/alienstorm.coleco/midres_tile_multicolor_004.o obj/alienstorm.coleco/midres_tile_multicolor_005.o obj/alienstorm.coleco/midres_tile_multicolor_006.o obj/alienstorm.coleco/midres_tile_multicolor_007.o obj/alienstorm.coleco/midres_tile_multicolor_008.o obj/alienstorm.coleco/midres_tile_multicolor_009.o obj/alienstorm.coleco/midres_tile_multicolor_010.o obj/alienstorm.coleco/midres_tile_multicolor_011.o obj/alienstorm.coleco/midres_tile_multicolor_012.o obj/alienstorm.coleco/midres_tile_processing_001.o obj/alienstorm.coleco/midres_tile_processing_002.o obj/alienstorm.coleco/midres_tile_processing_004.o obj/alienstorm.coleco/midres_tile_processing_005.o obj/alienstorm.coleco/midres_tile_processing_007.o obj/alienstorm.coleco/midres_tile_processing_009.o obj/alienstorm.coleco/midres_tile_processing_011.o obj/alienstorm.coleco/midres_tile_processing_012.o obj/alienstorm.coleco/midres_tile_processing_016.o obj/alienstorm.coleco/midres_vanilla.o obj/alienstorm.coleco/midres_vdp.o obj/alienstorm.coleco/midres_vic.o obj/alienstorm.coleco/midres_vic20.o obj/alienstorm.coleco/utility_joycheck.o obj/alienstorm.coleco/midres_control_011.o obj/alienstorm.coleco/midres_data.o obj/alienstorm.coleco/main.o -create-app 
	$(call COPYFILES,a.rom,$(EXEDIR)/alienstorm.coleco.rom)
	$(call RMFILES,a.*)

# find_main_module(elevator)
# -> src/game_elevator.c

# -------------------------------------------------------------------
# --- ELEVATOR FOR COLECO 
# -------------------------------------------------------------------
# 

.PHONY: elevator.embedded.coleco
elevator.embedded.coleco:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator64.mpic -n zelintro.bin -i $(DATADIR)/elevator1.imf -n elevator1.imf -c src/rawdata.c -h src/rawdata.h
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/elevator.coleco/rawdata.o src/rawdata.c

obj/elevator.coleco/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.coleco/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/elevator.coleco/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.coleco/midres_io.o src/midres_io.asm

obj/elevator.coleco/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__ELEVATOR__ -o $@ $(subst obj/elevator.coleco/,src/,$(@:.o=.c)) 

$(EXEDIR)/elevator.coleco:	elevator.embedded.coleco obj/elevator.coleco/demo_music.o obj/elevator.coleco/demo_tile.o obj/elevator.coleco/game_air_attack.o obj/elevator.coleco/game_alien_storm.o obj/elevator.coleco/game_elevator.o obj/elevator.coleco/game_totto.o obj/elevator.coleco/midres.o obj/elevator.coleco/midres_atari.o obj/elevator.coleco/midres_atmos.o obj/elevator.coleco/midres_ay8910.o obj/elevator.coleco/midres_c128.o obj/elevator.coleco/midres_c16.o obj/elevator.coleco/midres_c64.o obj/elevator.coleco/midres_coleco.o obj/elevator.coleco/midres_control_000.o obj/elevator.coleco/midres_control_001.o obj/elevator.coleco/midres_control_002.o obj/elevator.coleco/midres_control_003.o obj/elevator.coleco/midres_control_004.o obj/elevator.coleco/midres_control_005.o obj/elevator.coleco/midres_control_007.o obj/elevator.coleco/midres_control_010.o obj/elevator.coleco/midres_file_001.o obj/elevator.coleco/midres_file_003.o obj/elevator.coleco/midres_frame_000.o obj/elevator.coleco/midres_frame_001.o obj/elevator.coleco/midres_gb.o obj/elevator.coleco/midres_lm80c.o obj/elevator.coleco/midres_msx.o obj/elevator.coleco/midres_mtx500.o obj/elevator.coleco/midres_music_001.o obj/elevator.coleco/midres_music_002.o obj/elevator.coleco/midres_music_004.o obj/elevator.coleco/midres_music_005.o obj/elevator.coleco/midres_music_006.o obj/elevator.coleco/midres_plus4.o obj/elevator.coleco/midres_pokey.o obj/elevator.coleco/midres_screen_000.o obj/elevator.coleco/midres_screen_006.o obj/elevator.coleco/midres_screen_007.o obj/elevator.coleco/midres_screen_010.o obj/elevator.coleco/midres_screen_012.o obj/elevator.coleco/midres_screen_013.o obj/elevator.coleco/midres_sid.o obj/elevator.coleco/midres_sound_000.o obj/elevator.coleco/midres_sound_001.o obj/elevator.coleco/midres_sound_002.o obj/elevator.coleco/midres_sound_005.o obj/elevator.coleco/midres_sound_006.o obj/elevator.coleco/midres_sound_007.o obj/elevator.coleco/midres_svi.o obj/elevator.coleco/midres_ted.o obj/elevator.coleco/midres_tile_000.o obj/elevator.coleco/midres_tile_004.o obj/elevator.coleco/midres_tile_005.o obj/elevator.coleco/midres_tile_006.o obj/elevator.coleco/midres_tile_007.o obj/elevator.coleco/midres_tile_008.o obj/elevator.coleco/midres_tile_009.o obj/elevator.coleco/midres_tile_010.o obj/elevator.coleco/midres_tile_011.o obj/elevator.coleco/midres_tile_012.o obj/elevator.coleco/midres_tile_013.o obj/elevator.coleco/midres_tile_014.o obj/elevator.coleco/midres_tile_015.o obj/elevator.coleco/midres_tile_016.o obj/elevator.coleco/midres_tile_018.o obj/elevator.coleco/midres_tile_019.o obj/elevator.coleco/midres_tile_020.o obj/elevator.coleco/midres_tile_021.o obj/elevator.coleco/midres_tile_022.o obj/elevator.coleco/midres_tile_023.o obj/elevator.coleco/midres_tile_025.o obj/elevator.coleco/midres_tile_026.o obj/elevator.coleco/midres_tile_027.o obj/elevator.coleco/midres_tile_028.o obj/elevator.coleco/midres_tile_multicolor_000.o obj/elevator.coleco/midres_tile_multicolor_001.o obj/elevator.coleco/midres_tile_multicolor_002.o obj/elevator.coleco/midres_tile_multicolor_003.o obj/elevator.coleco/midres_tile_multicolor_004.o obj/elevator.coleco/midres_tile_multicolor_005.o obj/elevator.coleco/midres_tile_multicolor_006.o obj/elevator.coleco/midres_tile_multicolor_007.o obj/elevator.coleco/midres_tile_multicolor_008.o obj/elevator.coleco/midres_tile_multicolor_009.o obj/elevator.coleco/midres_tile_multicolor_010.o obj/elevator.coleco/midres_tile_multicolor_011.o obj/elevator.coleco/midres_tile_multicolor_012.o obj/elevator.coleco/midres_tile_processing_001.o obj/elevator.coleco/midres_tile_processing_002.o obj/elevator.coleco/midres_tile_processing_004.o obj/elevator.coleco/midres_tile_processing_005.o obj/elevator.coleco/midres_tile_processing_007.o obj/elevator.coleco/midres_tile_processing_009.o obj/elevator.coleco/midres_tile_processing_011.o obj/elevator.coleco/midres_tile_processing_012.o obj/elevator.coleco/midres_tile_processing_016.o obj/elevator.coleco/midres_vanilla.o obj/elevator.coleco/midres_vdp.o obj/elevator.coleco/midres_vic.o obj/elevator.coleco/midres_vic20.o obj/elevator.coleco/utility_joycheck.o obj/elevator.coleco/midres_control_011.o obj/elevator.coleco/midres_data.o obj/elevator.coleco/main.o obj/elevator.coleco/rawdata.o obj/elevator.coleco/midres_vdp_impl.o obj/elevator.coleco/midres_io.o
	$(CC88) +coleco  -m $(LDFLAGS88) obj/elevator.coleco/rawdata.o obj/elevator.coleco/midres_io.o obj/elevator.coleco/midres_vdp_impl.o obj/elevator.coleco/demo_music.o obj/elevator.coleco/demo_tile.o obj/elevator.coleco/game_air_attack.o obj/elevator.coleco/game_alien_storm.o obj/elevator.coleco/game_elevator.o obj/elevator.coleco/game_totto.o obj/elevator.coleco/midres.o obj/elevator.coleco/midres_atari.o obj/elevator.coleco/midres_atmos.o obj/elevator.coleco/midres_ay8910.o obj/elevator.coleco/midres_c128.o obj/elevator.coleco/midres_c16.o obj/elevator.coleco/midres_c64.o obj/elevator.coleco/midres_coleco.o obj/elevator.coleco/midres_control_000.o obj/elevator.coleco/midres_control_001.o obj/elevator.coleco/midres_control_002.o obj/elevator.coleco/midres_control_003.o obj/elevator.coleco/midres_control_004.o obj/elevator.coleco/midres_control_005.o obj/elevator.coleco/midres_control_007.o obj/elevator.coleco/midres_control_010.o obj/elevator.coleco/midres_file_001.o obj/elevator.coleco/midres_file_003.o obj/elevator.coleco/midres_frame_000.o obj/elevator.coleco/midres_frame_001.o obj/elevator.coleco/midres_gb.o obj/elevator.coleco/midres_lm80c.o obj/elevator.coleco/midres_msx.o obj/elevator.coleco/midres_mtx500.o obj/elevator.coleco/midres_music_001.o obj/elevator.coleco/midres_music_002.o obj/elevator.coleco/midres_music_004.o obj/elevator.coleco/midres_music_005.o obj/elevator.coleco/midres_music_006.o obj/elevator.coleco/midres_plus4.o obj/elevator.coleco/midres_pokey.o obj/elevator.coleco/midres_screen_000.o obj/elevator.coleco/midres_screen_006.o obj/elevator.coleco/midres_screen_007.o obj/elevator.coleco/midres_screen_010.o obj/elevator.coleco/midres_screen_012.o obj/elevator.coleco/midres_screen_013.o obj/elevator.coleco/midres_sid.o obj/elevator.coleco/midres_sound_000.o obj/elevator.coleco/midres_sound_001.o obj/elevator.coleco/midres_sound_002.o obj/elevator.coleco/midres_sound_005.o obj/elevator.coleco/midres_sound_006.o obj/elevator.coleco/midres_sound_007.o obj/elevator.coleco/midres_svi.o obj/elevator.coleco/midres_ted.o obj/elevator.coleco/midres_tile_000.o obj/elevator.coleco/midres_tile_004.o obj/elevator.coleco/midres_tile_005.o obj/elevator.coleco/midres_tile_006.o obj/elevator.coleco/midres_tile_007.o obj/elevator.coleco/midres_tile_008.o obj/elevator.coleco/midres_tile_009.o obj/elevator.coleco/midres_tile_010.o obj/elevator.coleco/midres_tile_011.o obj/elevator.coleco/midres_tile_012.o obj/elevator.coleco/midres_tile_013.o obj/elevator.coleco/midres_tile_014.o obj/elevator.coleco/midres_tile_015.o obj/elevator.coleco/midres_tile_016.o obj/elevator.coleco/midres_tile_018.o obj/elevator.coleco/midres_tile_019.o obj/elevator.coleco/midres_tile_020.o obj/elevator.coleco/midres_tile_021.o obj/elevator.coleco/midres_tile_022.o obj/elevator.coleco/midres_tile_023.o obj/elevator.coleco/midres_tile_025.o obj/elevator.coleco/midres_tile_026.o obj/elevator.coleco/midres_tile_027.o obj/elevator.coleco/midres_tile_028.o obj/elevator.coleco/midres_tile_multicolor_000.o obj/elevator.coleco/midres_tile_multicolor_001.o obj/elevator.coleco/midres_tile_multicolor_002.o obj/elevator.coleco/midres_tile_multicolor_003.o obj/elevator.coleco/midres_tile_multicolor_004.o obj/elevator.coleco/midres_tile_multicolor_005.o obj/elevator.coleco/midres_tile_multicolor_006.o obj/elevator.coleco/midres_tile_multicolor_007.o obj/elevator.coleco/midres_tile_multicolor_008.o obj/elevator.coleco/midres_tile_multicolor_009.o obj/elevator.coleco/midres_tile_multicolor_010.o obj/elevator.coleco/midres_tile_multicolor_011.o obj/elevator.coleco/midres_tile_multicolor_012.o obj/elevator.coleco/midres_tile_processing_001.o obj/elevator.coleco/midres_tile_processing_002.o obj/elevator.coleco/midres_tile_processing_004.o obj/elevator.coleco/midres_tile_processing_005.o obj/elevator.coleco/midres_tile_processing_007.o obj/elevator.coleco/midres_tile_processing_009.o obj/elevator.coleco/midres_tile_processing_011.o obj/elevator.coleco/midres_tile_processing_012.o obj/elevator.coleco/midres_tile_processing_016.o obj/elevator.coleco/midres_vanilla.o obj/elevator.coleco/midres_vdp.o obj/elevator.coleco/midres_vic.o obj/elevator.coleco/midres_vic20.o obj/elevator.coleco/utility_joycheck.o obj/elevator.coleco/midres_control_011.o obj/elevator.coleco/midres_data.o obj/elevator.coleco/main.o -create-app 
	$(call COPYFILES,a.rom,$(EXEDIR)/elevator.coleco.rom)
	$(call RMFILES,a.*)

                    
#!!! missing resources for joycheck (coleco)

# find_main_module(totto)
# -> src/game_totto.c

# -------------------------------------------------------------------
# --- TOTTO FOR COLECO 
# -------------------------------------------------------------------
# 

.PHONY: totto.embedded.coleco
totto.embedded.coleco:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -i $(DATADIR)/totto.imf -n totto.imf -i $(DATADIR)/totto3.imf -n totto3.imf -c src/rawdata.c -h src/rawdata.h
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/totto.coleco/rawdata.o src/rawdata.c

obj/totto.coleco/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.coleco/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/totto.coleco/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.coleco/midres_io.o src/midres_io.asm

obj/totto.coleco/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__TOTTO__ -o $@ $(subst obj/totto.coleco/,src/,$(@:.o=.c)) 

$(EXEDIR)/totto.coleco:	totto.embedded.coleco obj/totto.coleco/demo_music.o obj/totto.coleco/demo_tile.o obj/totto.coleco/game_air_attack.o obj/totto.coleco/game_alien_storm.o obj/totto.coleco/game_elevator.o obj/totto.coleco/game_totto.o obj/totto.coleco/midres.o obj/totto.coleco/midres_atari.o obj/totto.coleco/midres_atmos.o obj/totto.coleco/midres_ay8910.o obj/totto.coleco/midres_c128.o obj/totto.coleco/midres_c16.o obj/totto.coleco/midres_c64.o obj/totto.coleco/midres_coleco.o obj/totto.coleco/midres_control_000.o obj/totto.coleco/midres_control_001.o obj/totto.coleco/midres_control_002.o obj/totto.coleco/midres_control_003.o obj/totto.coleco/midres_control_004.o obj/totto.coleco/midres_control_005.o obj/totto.coleco/midres_control_007.o obj/totto.coleco/midres_control_010.o obj/totto.coleco/midres_file_001.o obj/totto.coleco/midres_file_003.o obj/totto.coleco/midres_frame_000.o obj/totto.coleco/midres_frame_001.o obj/totto.coleco/midres_gb.o obj/totto.coleco/midres_lm80c.o obj/totto.coleco/midres_msx.o obj/totto.coleco/midres_mtx500.o obj/totto.coleco/midres_music_001.o obj/totto.coleco/midres_music_002.o obj/totto.coleco/midres_music_004.o obj/totto.coleco/midres_music_005.o obj/totto.coleco/midres_music_006.o obj/totto.coleco/midres_plus4.o obj/totto.coleco/midres_pokey.o obj/totto.coleco/midres_screen_000.o obj/totto.coleco/midres_screen_006.o obj/totto.coleco/midres_screen_007.o obj/totto.coleco/midres_screen_010.o obj/totto.coleco/midres_screen_012.o obj/totto.coleco/midres_screen_013.o obj/totto.coleco/midres_sid.o obj/totto.coleco/midres_sound_000.o obj/totto.coleco/midres_sound_001.o obj/totto.coleco/midres_sound_002.o obj/totto.coleco/midres_sound_005.o obj/totto.coleco/midres_sound_006.o obj/totto.coleco/midres_sound_007.o obj/totto.coleco/midres_svi.o obj/totto.coleco/midres_ted.o obj/totto.coleco/midres_tile_000.o obj/totto.coleco/midres_tile_004.o obj/totto.coleco/midres_tile_005.o obj/totto.coleco/midres_tile_006.o obj/totto.coleco/midres_tile_007.o obj/totto.coleco/midres_tile_008.o obj/totto.coleco/midres_tile_009.o obj/totto.coleco/midres_tile_010.o obj/totto.coleco/midres_tile_011.o obj/totto.coleco/midres_tile_012.o obj/totto.coleco/midres_tile_013.o obj/totto.coleco/midres_tile_014.o obj/totto.coleco/midres_tile_015.o obj/totto.coleco/midres_tile_016.o obj/totto.coleco/midres_tile_018.o obj/totto.coleco/midres_tile_019.o obj/totto.coleco/midres_tile_020.o obj/totto.coleco/midres_tile_021.o obj/totto.coleco/midres_tile_022.o obj/totto.coleco/midres_tile_023.o obj/totto.coleco/midres_tile_025.o obj/totto.coleco/midres_tile_026.o obj/totto.coleco/midres_tile_027.o obj/totto.coleco/midres_tile_028.o obj/totto.coleco/midres_tile_multicolor_000.o obj/totto.coleco/midres_tile_multicolor_001.o obj/totto.coleco/midres_tile_multicolor_002.o obj/totto.coleco/midres_tile_multicolor_003.o obj/totto.coleco/midres_tile_multicolor_004.o obj/totto.coleco/midres_tile_multicolor_005.o obj/totto.coleco/midres_tile_multicolor_006.o obj/totto.coleco/midres_tile_multicolor_007.o obj/totto.coleco/midres_tile_multicolor_008.o obj/totto.coleco/midres_tile_multicolor_009.o obj/totto.coleco/midres_tile_multicolor_010.o obj/totto.coleco/midres_tile_multicolor_011.o obj/totto.coleco/midres_tile_multicolor_012.o obj/totto.coleco/midres_tile_processing_001.o obj/totto.coleco/midres_tile_processing_002.o obj/totto.coleco/midres_tile_processing_004.o obj/totto.coleco/midres_tile_processing_005.o obj/totto.coleco/midres_tile_processing_007.o obj/totto.coleco/midres_tile_processing_009.o obj/totto.coleco/midres_tile_processing_011.o obj/totto.coleco/midres_tile_processing_012.o obj/totto.coleco/midres_tile_processing_016.o obj/totto.coleco/midres_vanilla.o obj/totto.coleco/midres_vdp.o obj/totto.coleco/midres_vic.o obj/totto.coleco/midres_vic20.o obj/totto.coleco/utility_joycheck.o obj/totto.coleco/midres_control_011.o obj/totto.coleco/midres_data.o obj/totto.coleco/main.o obj/totto.coleco/rawdata.o obj/totto.coleco/midres_vdp_impl.o obj/totto.coleco/midres_io.o
	$(CC88) +coleco  -m $(LDFLAGS88) obj/totto.coleco/rawdata.o obj/totto.coleco/midres_io.o obj/totto.coleco/midres_vdp_impl.o obj/totto.coleco/demo_music.o obj/totto.coleco/demo_tile.o obj/totto.coleco/game_air_attack.o obj/totto.coleco/game_alien_storm.o obj/totto.coleco/game_elevator.o obj/totto.coleco/game_totto.o obj/totto.coleco/midres.o obj/totto.coleco/midres_atari.o obj/totto.coleco/midres_atmos.o obj/totto.coleco/midres_ay8910.o obj/totto.coleco/midres_c128.o obj/totto.coleco/midres_c16.o obj/totto.coleco/midres_c64.o obj/totto.coleco/midres_coleco.o obj/totto.coleco/midres_control_000.o obj/totto.coleco/midres_control_001.o obj/totto.coleco/midres_control_002.o obj/totto.coleco/midres_control_003.o obj/totto.coleco/midres_control_004.o obj/totto.coleco/midres_control_005.o obj/totto.coleco/midres_control_007.o obj/totto.coleco/midres_control_010.o obj/totto.coleco/midres_file_001.o obj/totto.coleco/midres_file_003.o obj/totto.coleco/midres_frame_000.o obj/totto.coleco/midres_frame_001.o obj/totto.coleco/midres_gb.o obj/totto.coleco/midres_lm80c.o obj/totto.coleco/midres_msx.o obj/totto.coleco/midres_mtx500.o obj/totto.coleco/midres_music_001.o obj/totto.coleco/midres_music_002.o obj/totto.coleco/midres_music_004.o obj/totto.coleco/midres_music_005.o obj/totto.coleco/midres_music_006.o obj/totto.coleco/midres_plus4.o obj/totto.coleco/midres_pokey.o obj/totto.coleco/midres_screen_000.o obj/totto.coleco/midres_screen_006.o obj/totto.coleco/midres_screen_007.o obj/totto.coleco/midres_screen_010.o obj/totto.coleco/midres_screen_012.o obj/totto.coleco/midres_screen_013.o obj/totto.coleco/midres_sid.o obj/totto.coleco/midres_sound_000.o obj/totto.coleco/midres_sound_001.o obj/totto.coleco/midres_sound_002.o obj/totto.coleco/midres_sound_005.o obj/totto.coleco/midres_sound_006.o obj/totto.coleco/midres_sound_007.o obj/totto.coleco/midres_svi.o obj/totto.coleco/midres_ted.o obj/totto.coleco/midres_tile_000.o obj/totto.coleco/midres_tile_004.o obj/totto.coleco/midres_tile_005.o obj/totto.coleco/midres_tile_006.o obj/totto.coleco/midres_tile_007.o obj/totto.coleco/midres_tile_008.o obj/totto.coleco/midres_tile_009.o obj/totto.coleco/midres_tile_010.o obj/totto.coleco/midres_tile_011.o obj/totto.coleco/midres_tile_012.o obj/totto.coleco/midres_tile_013.o obj/totto.coleco/midres_tile_014.o obj/totto.coleco/midres_tile_015.o obj/totto.coleco/midres_tile_016.o obj/totto.coleco/midres_tile_018.o obj/totto.coleco/midres_tile_019.o obj/totto.coleco/midres_tile_020.o obj/totto.coleco/midres_tile_021.o obj/totto.coleco/midres_tile_022.o obj/totto.coleco/midres_tile_023.o obj/totto.coleco/midres_tile_025.o obj/totto.coleco/midres_tile_026.o obj/totto.coleco/midres_tile_027.o obj/totto.coleco/midres_tile_028.o obj/totto.coleco/midres_tile_multicolor_000.o obj/totto.coleco/midres_tile_multicolor_001.o obj/totto.coleco/midres_tile_multicolor_002.o obj/totto.coleco/midres_tile_multicolor_003.o obj/totto.coleco/midres_tile_multicolor_004.o obj/totto.coleco/midres_tile_multicolor_005.o obj/totto.coleco/midres_tile_multicolor_006.o obj/totto.coleco/midres_tile_multicolor_007.o obj/totto.coleco/midres_tile_multicolor_008.o obj/totto.coleco/midres_tile_multicolor_009.o obj/totto.coleco/midres_tile_multicolor_010.o obj/totto.coleco/midres_tile_multicolor_011.o obj/totto.coleco/midres_tile_multicolor_012.o obj/totto.coleco/midres_tile_processing_001.o obj/totto.coleco/midres_tile_processing_002.o obj/totto.coleco/midres_tile_processing_004.o obj/totto.coleco/midres_tile_processing_005.o obj/totto.coleco/midres_tile_processing_007.o obj/totto.coleco/midres_tile_processing_009.o obj/totto.coleco/midres_tile_processing_011.o obj/totto.coleco/midres_tile_processing_012.o obj/totto.coleco/midres_tile_processing_016.o obj/totto.coleco/midres_vanilla.o obj/totto.coleco/midres_vdp.o obj/totto.coleco/midres_vic.o obj/totto.coleco/midres_vic20.o obj/totto.coleco/utility_joycheck.o obj/totto.coleco/midres_control_011.o obj/totto.coleco/midres_data.o obj/totto.coleco/main.o -create-app 
	$(call COPYFILES,a.rom,$(EXEDIR)/totto.coleco.rom)
	$(call RMFILES,a.*)

# find_main_module(airattack)
# -> src/game_air_attack.c

# -------------------------------------------------------------------
# --- AIRATTACK FOR GB 
# -------------------------------------------------------------------
# 

.PHONY: airattack.embedded.gb
airattack.embedded.gb:
	$(FILE2INCLUDE) -i $(DATADIR)/ztiles.bin -n ztiles.bin -i $(DATADIR)/aatiles4.bin -n zztiles.bin -i $(DATADIR)/aaintrox.pic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h
	$(CC88) +gb $(CFLAGS) -c $(CFLAGS88)  -o obj/airattack.gb/rawdata.o src/rawdata.c

obj/airattack.gb/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.gb/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/airattack.gb/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.gb/midres_io.o src/midres_io.asm

obj/airattack.gb/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +gb $(CFLAGS) -c $(CFLAGS88)  -D__AIRATTACK__ -o $@ $(subst obj/airattack.gb/,src/,$(@:.o=.c)) 

$(EXEDIR)/airattack.gb:	airattack.embedded.gb obj/airattack.gb/demo_music.o obj/airattack.gb/demo_tile.o obj/airattack.gb/game_air_attack.o obj/airattack.gb/game_alien_storm.o obj/airattack.gb/game_elevator.o obj/airattack.gb/game_totto.o obj/airattack.gb/midres.o obj/airattack.gb/midres_atari.o obj/airattack.gb/midres_atmos.o obj/airattack.gb/midres_ay8910.o obj/airattack.gb/midres_c128.o obj/airattack.gb/midres_c16.o obj/airattack.gb/midres_c64.o obj/airattack.gb/midres_coleco.o obj/airattack.gb/midres_control_000.o obj/airattack.gb/midres_control_001.o obj/airattack.gb/midres_control_002.o obj/airattack.gb/midres_control_003.o obj/airattack.gb/midres_control_004.o obj/airattack.gb/midres_control_005.o obj/airattack.gb/midres_control_007.o obj/airattack.gb/midres_control_010.o obj/airattack.gb/midres_file_001.o obj/airattack.gb/midres_file_003.o obj/airattack.gb/midres_frame_000.o obj/airattack.gb/midres_frame_001.o obj/airattack.gb/midres_gb.o obj/airattack.gb/midres_lm80c.o obj/airattack.gb/midres_msx.o obj/airattack.gb/midres_mtx500.o obj/airattack.gb/midres_music_001.o obj/airattack.gb/midres_music_002.o obj/airattack.gb/midres_music_004.o obj/airattack.gb/midres_music_005.o obj/airattack.gb/midres_music_006.o obj/airattack.gb/midres_plus4.o obj/airattack.gb/midres_pokey.o obj/airattack.gb/midres_screen_000.o obj/airattack.gb/midres_screen_006.o obj/airattack.gb/midres_screen_007.o obj/airattack.gb/midres_screen_010.o obj/airattack.gb/midres_screen_012.o obj/airattack.gb/midres_screen_013.o obj/airattack.gb/midres_sid.o obj/airattack.gb/midres_sound_000.o obj/airattack.gb/midres_sound_001.o obj/airattack.gb/midres_sound_002.o obj/airattack.gb/midres_sound_005.o obj/airattack.gb/midres_sound_006.o obj/airattack.gb/midres_sound_007.o obj/airattack.gb/midres_svi.o obj/airattack.gb/midres_ted.o obj/airattack.gb/midres_tile_000.o obj/airattack.gb/midres_tile_004.o obj/airattack.gb/midres_tile_005.o obj/airattack.gb/midres_tile_006.o obj/airattack.gb/midres_tile_007.o obj/airattack.gb/midres_tile_008.o obj/airattack.gb/midres_tile_009.o obj/airattack.gb/midres_tile_010.o obj/airattack.gb/midres_tile_011.o obj/airattack.gb/midres_tile_012.o obj/airattack.gb/midres_tile_013.o obj/airattack.gb/midres_tile_014.o obj/airattack.gb/midres_tile_015.o obj/airattack.gb/midres_tile_016.o obj/airattack.gb/midres_tile_018.o obj/airattack.gb/midres_tile_019.o obj/airattack.gb/midres_tile_020.o obj/airattack.gb/midres_tile_021.o obj/airattack.gb/midres_tile_022.o obj/airattack.gb/midres_tile_023.o obj/airattack.gb/midres_tile_025.o obj/airattack.gb/midres_tile_026.o obj/airattack.gb/midres_tile_027.o obj/airattack.gb/midres_tile_028.o obj/airattack.gb/midres_tile_multicolor_000.o obj/airattack.gb/midres_tile_multicolor_001.o obj/airattack.gb/midres_tile_multicolor_002.o obj/airattack.gb/midres_tile_multicolor_003.o obj/airattack.gb/midres_tile_multicolor_004.o obj/airattack.gb/midres_tile_multicolor_005.o obj/airattack.gb/midres_tile_multicolor_006.o obj/airattack.gb/midres_tile_multicolor_007.o obj/airattack.gb/midres_tile_multicolor_008.o obj/airattack.gb/midres_tile_multicolor_009.o obj/airattack.gb/midres_tile_multicolor_010.o obj/airattack.gb/midres_tile_multicolor_011.o obj/airattack.gb/midres_tile_multicolor_012.o obj/airattack.gb/midres_tile_processing_001.o obj/airattack.gb/midres_tile_processing_002.o obj/airattack.gb/midres_tile_processing_004.o obj/airattack.gb/midres_tile_processing_005.o obj/airattack.gb/midres_tile_processing_007.o obj/airattack.gb/midres_tile_processing_009.o obj/airattack.gb/midres_tile_processing_011.o obj/airattack.gb/midres_tile_processing_012.o obj/airattack.gb/midres_tile_processing_016.o obj/airattack.gb/midres_vanilla.o obj/airattack.gb/midres_vdp.o obj/airattack.gb/midres_vic.o obj/airattack.gb/midres_vic20.o obj/airattack.gb/utility_joycheck.o obj/airattack.gb/midres_control_011.o obj/airattack.gb/midres_data.o obj/airattack.gb/main.o obj/airattack.gb/rawdata.o obj/airattack.gb/midres_vdp_impl.o obj/airattack.gb/midres_io.o
	$(CC88) +gb  -m $(LDFLAGS88) obj/airattack.gb/rawdata.o obj/airattack.gb/midres_io.o obj/airattack.gb/midres_vdp_impl.o obj/airattack.gb/demo_music.o obj/airattack.gb/demo_tile.o obj/airattack.gb/game_air_attack.o obj/airattack.gb/game_alien_storm.o obj/airattack.gb/game_elevator.o obj/airattack.gb/game_totto.o obj/airattack.gb/midres.o obj/airattack.gb/midres_atari.o obj/airattack.gb/midres_atmos.o obj/airattack.gb/midres_ay8910.o obj/airattack.gb/midres_c128.o obj/airattack.gb/midres_c16.o obj/airattack.gb/midres_c64.o obj/airattack.gb/midres_coleco.o obj/airattack.gb/midres_control_000.o obj/airattack.gb/midres_control_001.o obj/airattack.gb/midres_control_002.o obj/airattack.gb/midres_control_003.o obj/airattack.gb/midres_control_004.o obj/airattack.gb/midres_control_005.o obj/airattack.gb/midres_control_007.o obj/airattack.gb/midres_control_010.o obj/airattack.gb/midres_file_001.o obj/airattack.gb/midres_file_003.o obj/airattack.gb/midres_frame_000.o obj/airattack.gb/midres_frame_001.o obj/airattack.gb/midres_gb.o obj/airattack.gb/midres_lm80c.o obj/airattack.gb/midres_msx.o obj/airattack.gb/midres_mtx500.o obj/airattack.gb/midres_music_001.o obj/airattack.gb/midres_music_002.o obj/airattack.gb/midres_music_004.o obj/airattack.gb/midres_music_005.o obj/airattack.gb/midres_music_006.o obj/airattack.gb/midres_plus4.o obj/airattack.gb/midres_pokey.o obj/airattack.gb/midres_screen_000.o obj/airattack.gb/midres_screen_006.o obj/airattack.gb/midres_screen_007.o obj/airattack.gb/midres_screen_010.o obj/airattack.gb/midres_screen_012.o obj/airattack.gb/midres_screen_013.o obj/airattack.gb/midres_sid.o obj/airattack.gb/midres_sound_000.o obj/airattack.gb/midres_sound_001.o obj/airattack.gb/midres_sound_002.o obj/airattack.gb/midres_sound_005.o obj/airattack.gb/midres_sound_006.o obj/airattack.gb/midres_sound_007.o obj/airattack.gb/midres_svi.o obj/airattack.gb/midres_ted.o obj/airattack.gb/midres_tile_000.o obj/airattack.gb/midres_tile_004.o obj/airattack.gb/midres_tile_005.o obj/airattack.gb/midres_tile_006.o obj/airattack.gb/midres_tile_007.o obj/airattack.gb/midres_tile_008.o obj/airattack.gb/midres_tile_009.o obj/airattack.gb/midres_tile_010.o obj/airattack.gb/midres_tile_011.o obj/airattack.gb/midres_tile_012.o obj/airattack.gb/midres_tile_013.o obj/airattack.gb/midres_tile_014.o obj/airattack.gb/midres_tile_015.o obj/airattack.gb/midres_tile_016.o obj/airattack.gb/midres_tile_018.o obj/airattack.gb/midres_tile_019.o obj/airattack.gb/midres_tile_020.o obj/airattack.gb/midres_tile_021.o obj/airattack.gb/midres_tile_022.o obj/airattack.gb/midres_tile_023.o obj/airattack.gb/midres_tile_025.o obj/airattack.gb/midres_tile_026.o obj/airattack.gb/midres_tile_027.o obj/airattack.gb/midres_tile_028.o obj/airattack.gb/midres_tile_multicolor_000.o obj/airattack.gb/midres_tile_multicolor_001.o obj/airattack.gb/midres_tile_multicolor_002.o obj/airattack.gb/midres_tile_multicolor_003.o obj/airattack.gb/midres_tile_multicolor_004.o obj/airattack.gb/midres_tile_multicolor_005.o obj/airattack.gb/midres_tile_multicolor_006.o obj/airattack.gb/midres_tile_multicolor_007.o obj/airattack.gb/midres_tile_multicolor_008.o obj/airattack.gb/midres_tile_multicolor_009.o obj/airattack.gb/midres_tile_multicolor_010.o obj/airattack.gb/midres_tile_multicolor_011.o obj/airattack.gb/midres_tile_multicolor_012.o obj/airattack.gb/midres_tile_processing_001.o obj/airattack.gb/midres_tile_processing_002.o obj/airattack.gb/midres_tile_processing_004.o obj/airattack.gb/midres_tile_processing_005.o obj/airattack.gb/midres_tile_processing_007.o obj/airattack.gb/midres_tile_processing_009.o obj/airattack.gb/midres_tile_processing_011.o obj/airattack.gb/midres_tile_processing_012.o obj/airattack.gb/midres_tile_processing_016.o obj/airattack.gb/midres_vanilla.o obj/airattack.gb/midres_vdp.o obj/airattack.gb/midres_vic.o obj/airattack.gb/midres_vic20.o obj/airattack.gb/utility_joycheck.o obj/airattack.gb/midres_control_011.o obj/airattack.gb/midres_data.o obj/airattack.gb/main.o -create-app 
	$(call COPYFILES,a.gb,$(EXEDIR)/airattack.gb.gb)
	$(call RMFILES,a.*)

# find_main_module(alienstorm)
# -> src/game_alien_storm.c

# -------------------------------------------------------------------
# --- ALIENSTORM FOR GB 
# -------------------------------------------------------------------
# 

.PHONY: alienstorm.embedded.gb
alienstorm.embedded.gb:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +gb $(CFLAGS) -c $(CFLAGS88)  -o obj/alienstorm.gb/rawdata.o src/rawdata.c

obj/alienstorm.gb/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.gb/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/alienstorm.gb/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.gb/midres_io.o src/midres_io.asm

obj/alienstorm.gb/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +gb $(CFLAGS) -c $(CFLAGS88)  -D__ALIENSTORM__ -o $@ $(subst obj/alienstorm.gb/,src/,$(@:.o=.c)) 

$(EXEDIR)/alienstorm.gb:	alienstorm.embedded.gb obj/alienstorm.gb/demo_music.o obj/alienstorm.gb/demo_tile.o obj/alienstorm.gb/game_air_attack.o obj/alienstorm.gb/game_alien_storm.o obj/alienstorm.gb/game_elevator.o obj/alienstorm.gb/game_totto.o obj/alienstorm.gb/midres.o obj/alienstorm.gb/midres_atari.o obj/alienstorm.gb/midres_atmos.o obj/alienstorm.gb/midres_ay8910.o obj/alienstorm.gb/midres_c128.o obj/alienstorm.gb/midres_c16.o obj/alienstorm.gb/midres_c64.o obj/alienstorm.gb/midres_coleco.o obj/alienstorm.gb/midres_control_000.o obj/alienstorm.gb/midres_control_001.o obj/alienstorm.gb/midres_control_002.o obj/alienstorm.gb/midres_control_003.o obj/alienstorm.gb/midres_control_004.o obj/alienstorm.gb/midres_control_005.o obj/alienstorm.gb/midres_control_007.o obj/alienstorm.gb/midres_control_010.o obj/alienstorm.gb/midres_file_001.o obj/alienstorm.gb/midres_file_003.o obj/alienstorm.gb/midres_frame_000.o obj/alienstorm.gb/midres_frame_001.o obj/alienstorm.gb/midres_gb.o obj/alienstorm.gb/midres_lm80c.o obj/alienstorm.gb/midres_msx.o obj/alienstorm.gb/midres_mtx500.o obj/alienstorm.gb/midres_music_001.o obj/alienstorm.gb/midres_music_002.o obj/alienstorm.gb/midres_music_004.o obj/alienstorm.gb/midres_music_005.o obj/alienstorm.gb/midres_music_006.o obj/alienstorm.gb/midres_plus4.o obj/alienstorm.gb/midres_pokey.o obj/alienstorm.gb/midres_screen_000.o obj/alienstorm.gb/midres_screen_006.o obj/alienstorm.gb/midres_screen_007.o obj/alienstorm.gb/midres_screen_010.o obj/alienstorm.gb/midres_screen_012.o obj/alienstorm.gb/midres_screen_013.o obj/alienstorm.gb/midres_sid.o obj/alienstorm.gb/midres_sound_000.o obj/alienstorm.gb/midres_sound_001.o obj/alienstorm.gb/midres_sound_002.o obj/alienstorm.gb/midres_sound_005.o obj/alienstorm.gb/midres_sound_006.o obj/alienstorm.gb/midres_sound_007.o obj/alienstorm.gb/midres_svi.o obj/alienstorm.gb/midres_ted.o obj/alienstorm.gb/midres_tile_000.o obj/alienstorm.gb/midres_tile_004.o obj/alienstorm.gb/midres_tile_005.o obj/alienstorm.gb/midres_tile_006.o obj/alienstorm.gb/midres_tile_007.o obj/alienstorm.gb/midres_tile_008.o obj/alienstorm.gb/midres_tile_009.o obj/alienstorm.gb/midres_tile_010.o obj/alienstorm.gb/midres_tile_011.o obj/alienstorm.gb/midres_tile_012.o obj/alienstorm.gb/midres_tile_013.o obj/alienstorm.gb/midres_tile_014.o obj/alienstorm.gb/midres_tile_015.o obj/alienstorm.gb/midres_tile_016.o obj/alienstorm.gb/midres_tile_018.o obj/alienstorm.gb/midres_tile_019.o obj/alienstorm.gb/midres_tile_020.o obj/alienstorm.gb/midres_tile_021.o obj/alienstorm.gb/midres_tile_022.o obj/alienstorm.gb/midres_tile_023.o obj/alienstorm.gb/midres_tile_025.o obj/alienstorm.gb/midres_tile_026.o obj/alienstorm.gb/midres_tile_027.o obj/alienstorm.gb/midres_tile_028.o obj/alienstorm.gb/midres_tile_multicolor_000.o obj/alienstorm.gb/midres_tile_multicolor_001.o obj/alienstorm.gb/midres_tile_multicolor_002.o obj/alienstorm.gb/midres_tile_multicolor_003.o obj/alienstorm.gb/midres_tile_multicolor_004.o obj/alienstorm.gb/midres_tile_multicolor_005.o obj/alienstorm.gb/midres_tile_multicolor_006.o obj/alienstorm.gb/midres_tile_multicolor_007.o obj/alienstorm.gb/midres_tile_multicolor_008.o obj/alienstorm.gb/midres_tile_multicolor_009.o obj/alienstorm.gb/midres_tile_multicolor_010.o obj/alienstorm.gb/midres_tile_multicolor_011.o obj/alienstorm.gb/midres_tile_multicolor_012.o obj/alienstorm.gb/midres_tile_processing_001.o obj/alienstorm.gb/midres_tile_processing_002.o obj/alienstorm.gb/midres_tile_processing_004.o obj/alienstorm.gb/midres_tile_processing_005.o obj/alienstorm.gb/midres_tile_processing_007.o obj/alienstorm.gb/midres_tile_processing_009.o obj/alienstorm.gb/midres_tile_processing_011.o obj/alienstorm.gb/midres_tile_processing_012.o obj/alienstorm.gb/midres_tile_processing_016.o obj/alienstorm.gb/midres_vanilla.o obj/alienstorm.gb/midres_vdp.o obj/alienstorm.gb/midres_vic.o obj/alienstorm.gb/midres_vic20.o obj/alienstorm.gb/utility_joycheck.o obj/alienstorm.gb/midres_control_011.o obj/alienstorm.gb/midres_data.o obj/alienstorm.gb/main.o obj/alienstorm.gb/rawdata.o obj/alienstorm.gb/midres_vdp_impl.o obj/alienstorm.gb/midres_io.o
	$(CC88) +gb  -m $(LDFLAGS88) obj/alienstorm.gb/rawdata.o obj/alienstorm.gb/midres_io.o obj/alienstorm.gb/midres_vdp_impl.o obj/alienstorm.gb/demo_music.o obj/alienstorm.gb/demo_tile.o obj/alienstorm.gb/game_air_attack.o obj/alienstorm.gb/game_alien_storm.o obj/alienstorm.gb/game_elevator.o obj/alienstorm.gb/game_totto.o obj/alienstorm.gb/midres.o obj/alienstorm.gb/midres_atari.o obj/alienstorm.gb/midres_atmos.o obj/alienstorm.gb/midres_ay8910.o obj/alienstorm.gb/midres_c128.o obj/alienstorm.gb/midres_c16.o obj/alienstorm.gb/midres_c64.o obj/alienstorm.gb/midres_coleco.o obj/alienstorm.gb/midres_control_000.o obj/alienstorm.gb/midres_control_001.o obj/alienstorm.gb/midres_control_002.o obj/alienstorm.gb/midres_control_003.o obj/alienstorm.gb/midres_control_004.o obj/alienstorm.gb/midres_control_005.o obj/alienstorm.gb/midres_control_007.o obj/alienstorm.gb/midres_control_010.o obj/alienstorm.gb/midres_file_001.o obj/alienstorm.gb/midres_file_003.o obj/alienstorm.gb/midres_frame_000.o obj/alienstorm.gb/midres_frame_001.o obj/alienstorm.gb/midres_gb.o obj/alienstorm.gb/midres_lm80c.o obj/alienstorm.gb/midres_msx.o obj/alienstorm.gb/midres_mtx500.o obj/alienstorm.gb/midres_music_001.o obj/alienstorm.gb/midres_music_002.o obj/alienstorm.gb/midres_music_004.o obj/alienstorm.gb/midres_music_005.o obj/alienstorm.gb/midres_music_006.o obj/alienstorm.gb/midres_plus4.o obj/alienstorm.gb/midres_pokey.o obj/alienstorm.gb/midres_screen_000.o obj/alienstorm.gb/midres_screen_006.o obj/alienstorm.gb/midres_screen_007.o obj/alienstorm.gb/midres_screen_010.o obj/alienstorm.gb/midres_screen_012.o obj/alienstorm.gb/midres_screen_013.o obj/alienstorm.gb/midres_sid.o obj/alienstorm.gb/midres_sound_000.o obj/alienstorm.gb/midres_sound_001.o obj/alienstorm.gb/midres_sound_002.o obj/alienstorm.gb/midres_sound_005.o obj/alienstorm.gb/midres_sound_006.o obj/alienstorm.gb/midres_sound_007.o obj/alienstorm.gb/midres_svi.o obj/alienstorm.gb/midres_ted.o obj/alienstorm.gb/midres_tile_000.o obj/alienstorm.gb/midres_tile_004.o obj/alienstorm.gb/midres_tile_005.o obj/alienstorm.gb/midres_tile_006.o obj/alienstorm.gb/midres_tile_007.o obj/alienstorm.gb/midres_tile_008.o obj/alienstorm.gb/midres_tile_009.o obj/alienstorm.gb/midres_tile_010.o obj/alienstorm.gb/midres_tile_011.o obj/alienstorm.gb/midres_tile_012.o obj/alienstorm.gb/midres_tile_013.o obj/alienstorm.gb/midres_tile_014.o obj/alienstorm.gb/midres_tile_015.o obj/alienstorm.gb/midres_tile_016.o obj/alienstorm.gb/midres_tile_018.o obj/alienstorm.gb/midres_tile_019.o obj/alienstorm.gb/midres_tile_020.o obj/alienstorm.gb/midres_tile_021.o obj/alienstorm.gb/midres_tile_022.o obj/alienstorm.gb/midres_tile_023.o obj/alienstorm.gb/midres_tile_025.o obj/alienstorm.gb/midres_tile_026.o obj/alienstorm.gb/midres_tile_027.o obj/alienstorm.gb/midres_tile_028.o obj/alienstorm.gb/midres_tile_multicolor_000.o obj/alienstorm.gb/midres_tile_multicolor_001.o obj/alienstorm.gb/midres_tile_multicolor_002.o obj/alienstorm.gb/midres_tile_multicolor_003.o obj/alienstorm.gb/midres_tile_multicolor_004.o obj/alienstorm.gb/midres_tile_multicolor_005.o obj/alienstorm.gb/midres_tile_multicolor_006.o obj/alienstorm.gb/midres_tile_multicolor_007.o obj/alienstorm.gb/midres_tile_multicolor_008.o obj/alienstorm.gb/midres_tile_multicolor_009.o obj/alienstorm.gb/midres_tile_multicolor_010.o obj/alienstorm.gb/midres_tile_multicolor_011.o obj/alienstorm.gb/midres_tile_multicolor_012.o obj/alienstorm.gb/midres_tile_processing_001.o obj/alienstorm.gb/midres_tile_processing_002.o obj/alienstorm.gb/midres_tile_processing_004.o obj/alienstorm.gb/midres_tile_processing_005.o obj/alienstorm.gb/midres_tile_processing_007.o obj/alienstorm.gb/midres_tile_processing_009.o obj/alienstorm.gb/midres_tile_processing_011.o obj/alienstorm.gb/midres_tile_processing_012.o obj/alienstorm.gb/midres_tile_processing_016.o obj/alienstorm.gb/midres_vanilla.o obj/alienstorm.gb/midres_vdp.o obj/alienstorm.gb/midres_vic.o obj/alienstorm.gb/midres_vic20.o obj/alienstorm.gb/utility_joycheck.o obj/alienstorm.gb/midres_control_011.o obj/alienstorm.gb/midres_data.o obj/alienstorm.gb/main.o -create-app 
	$(call COPYFILES,a.gb,$(EXEDIR)/alienstorm.gb.gb)
	$(call RMFILES,a.*)

# find_main_module(elevator)
# -> src/game_elevator.c

# -------------------------------------------------------------------
# --- ELEVATOR FOR GB 
# -------------------------------------------------------------------
# 

.PHONY: elevator.embedded.gb
elevator.embedded.gb:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevatora.mpic -n zelintro.bin -i $(DATADIR)/elevator1.imf -n elevator1.imf -c src/rawdata.c -h src/rawdata.h
	$(CC88) +gb $(CFLAGS) -c $(CFLAGS88)  -o obj/elevator.gb/rawdata.o src/rawdata.c

obj/elevator.gb/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.gb/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/elevator.gb/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.gb/midres_io.o src/midres_io.asm

obj/elevator.gb/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +gb $(CFLAGS) -c $(CFLAGS88)  -D__ELEVATOR__ -o $@ $(subst obj/elevator.gb/,src/,$(@:.o=.c)) 

$(EXEDIR)/elevator.gb:	elevator.embedded.gb obj/elevator.gb/demo_music.o obj/elevator.gb/demo_tile.o obj/elevator.gb/game_air_attack.o obj/elevator.gb/game_alien_storm.o obj/elevator.gb/game_elevator.o obj/elevator.gb/game_totto.o obj/elevator.gb/midres.o obj/elevator.gb/midres_atari.o obj/elevator.gb/midres_atmos.o obj/elevator.gb/midres_ay8910.o obj/elevator.gb/midres_c128.o obj/elevator.gb/midres_c16.o obj/elevator.gb/midres_c64.o obj/elevator.gb/midres_coleco.o obj/elevator.gb/midres_control_000.o obj/elevator.gb/midres_control_001.o obj/elevator.gb/midres_control_002.o obj/elevator.gb/midres_control_003.o obj/elevator.gb/midres_control_004.o obj/elevator.gb/midres_control_005.o obj/elevator.gb/midres_control_007.o obj/elevator.gb/midres_control_010.o obj/elevator.gb/midres_file_001.o obj/elevator.gb/midres_file_003.o obj/elevator.gb/midres_frame_000.o obj/elevator.gb/midres_frame_001.o obj/elevator.gb/midres_gb.o obj/elevator.gb/midres_lm80c.o obj/elevator.gb/midres_msx.o obj/elevator.gb/midres_mtx500.o obj/elevator.gb/midres_music_001.o obj/elevator.gb/midres_music_002.o obj/elevator.gb/midres_music_004.o obj/elevator.gb/midres_music_005.o obj/elevator.gb/midres_music_006.o obj/elevator.gb/midres_plus4.o obj/elevator.gb/midres_pokey.o obj/elevator.gb/midres_screen_000.o obj/elevator.gb/midres_screen_006.o obj/elevator.gb/midres_screen_007.o obj/elevator.gb/midres_screen_010.o obj/elevator.gb/midres_screen_012.o obj/elevator.gb/midres_screen_013.o obj/elevator.gb/midres_sid.o obj/elevator.gb/midres_sound_000.o obj/elevator.gb/midres_sound_001.o obj/elevator.gb/midres_sound_002.o obj/elevator.gb/midres_sound_005.o obj/elevator.gb/midres_sound_006.o obj/elevator.gb/midres_sound_007.o obj/elevator.gb/midres_svi.o obj/elevator.gb/midres_ted.o obj/elevator.gb/midres_tile_000.o obj/elevator.gb/midres_tile_004.o obj/elevator.gb/midres_tile_005.o obj/elevator.gb/midres_tile_006.o obj/elevator.gb/midres_tile_007.o obj/elevator.gb/midres_tile_008.o obj/elevator.gb/midres_tile_009.o obj/elevator.gb/midres_tile_010.o obj/elevator.gb/midres_tile_011.o obj/elevator.gb/midres_tile_012.o obj/elevator.gb/midres_tile_013.o obj/elevator.gb/midres_tile_014.o obj/elevator.gb/midres_tile_015.o obj/elevator.gb/midres_tile_016.o obj/elevator.gb/midres_tile_018.o obj/elevator.gb/midres_tile_019.o obj/elevator.gb/midres_tile_020.o obj/elevator.gb/midres_tile_021.o obj/elevator.gb/midres_tile_022.o obj/elevator.gb/midres_tile_023.o obj/elevator.gb/midres_tile_025.o obj/elevator.gb/midres_tile_026.o obj/elevator.gb/midres_tile_027.o obj/elevator.gb/midres_tile_028.o obj/elevator.gb/midres_tile_multicolor_000.o obj/elevator.gb/midres_tile_multicolor_001.o obj/elevator.gb/midres_tile_multicolor_002.o obj/elevator.gb/midres_tile_multicolor_003.o obj/elevator.gb/midres_tile_multicolor_004.o obj/elevator.gb/midres_tile_multicolor_005.o obj/elevator.gb/midres_tile_multicolor_006.o obj/elevator.gb/midres_tile_multicolor_007.o obj/elevator.gb/midres_tile_multicolor_008.o obj/elevator.gb/midres_tile_multicolor_009.o obj/elevator.gb/midres_tile_multicolor_010.o obj/elevator.gb/midres_tile_multicolor_011.o obj/elevator.gb/midres_tile_multicolor_012.o obj/elevator.gb/midres_tile_processing_001.o obj/elevator.gb/midres_tile_processing_002.o obj/elevator.gb/midres_tile_processing_004.o obj/elevator.gb/midres_tile_processing_005.o obj/elevator.gb/midres_tile_processing_007.o obj/elevator.gb/midres_tile_processing_009.o obj/elevator.gb/midres_tile_processing_011.o obj/elevator.gb/midres_tile_processing_012.o obj/elevator.gb/midres_tile_processing_016.o obj/elevator.gb/midres_vanilla.o obj/elevator.gb/midres_vdp.o obj/elevator.gb/midres_vic.o obj/elevator.gb/midres_vic20.o obj/elevator.gb/utility_joycheck.o obj/elevator.gb/midres_control_011.o obj/elevator.gb/midres_data.o obj/elevator.gb/main.o obj/elevator.gb/rawdata.o obj/elevator.gb/midres_vdp_impl.o obj/elevator.gb/midres_io.o
	$(CC88) +gb  -m $(LDFLAGS88) obj/elevator.gb/rawdata.o obj/elevator.gb/midres_io.o obj/elevator.gb/midres_vdp_impl.o obj/elevator.gb/demo_music.o obj/elevator.gb/demo_tile.o obj/elevator.gb/game_air_attack.o obj/elevator.gb/game_alien_storm.o obj/elevator.gb/game_elevator.o obj/elevator.gb/game_totto.o obj/elevator.gb/midres.o obj/elevator.gb/midres_atari.o obj/elevator.gb/midres_atmos.o obj/elevator.gb/midres_ay8910.o obj/elevator.gb/midres_c128.o obj/elevator.gb/midres_c16.o obj/elevator.gb/midres_c64.o obj/elevator.gb/midres_coleco.o obj/elevator.gb/midres_control_000.o obj/elevator.gb/midres_control_001.o obj/elevator.gb/midres_control_002.o obj/elevator.gb/midres_control_003.o obj/elevator.gb/midres_control_004.o obj/elevator.gb/midres_control_005.o obj/elevator.gb/midres_control_007.o obj/elevator.gb/midres_control_010.o obj/elevator.gb/midres_file_001.o obj/elevator.gb/midres_file_003.o obj/elevator.gb/midres_frame_000.o obj/elevator.gb/midres_frame_001.o obj/elevator.gb/midres_gb.o obj/elevator.gb/midres_lm80c.o obj/elevator.gb/midres_msx.o obj/elevator.gb/midres_mtx500.o obj/elevator.gb/midres_music_001.o obj/elevator.gb/midres_music_002.o obj/elevator.gb/midres_music_004.o obj/elevator.gb/midres_music_005.o obj/elevator.gb/midres_music_006.o obj/elevator.gb/midres_plus4.o obj/elevator.gb/midres_pokey.o obj/elevator.gb/midres_screen_000.o obj/elevator.gb/midres_screen_006.o obj/elevator.gb/midres_screen_007.o obj/elevator.gb/midres_screen_010.o obj/elevator.gb/midres_screen_012.o obj/elevator.gb/midres_screen_013.o obj/elevator.gb/midres_sid.o obj/elevator.gb/midres_sound_000.o obj/elevator.gb/midres_sound_001.o obj/elevator.gb/midres_sound_002.o obj/elevator.gb/midres_sound_005.o obj/elevator.gb/midres_sound_006.o obj/elevator.gb/midres_sound_007.o obj/elevator.gb/midres_svi.o obj/elevator.gb/midres_ted.o obj/elevator.gb/midres_tile_000.o obj/elevator.gb/midres_tile_004.o obj/elevator.gb/midres_tile_005.o obj/elevator.gb/midres_tile_006.o obj/elevator.gb/midres_tile_007.o obj/elevator.gb/midres_tile_008.o obj/elevator.gb/midres_tile_009.o obj/elevator.gb/midres_tile_010.o obj/elevator.gb/midres_tile_011.o obj/elevator.gb/midres_tile_012.o obj/elevator.gb/midres_tile_013.o obj/elevator.gb/midres_tile_014.o obj/elevator.gb/midres_tile_015.o obj/elevator.gb/midres_tile_016.o obj/elevator.gb/midres_tile_018.o obj/elevator.gb/midres_tile_019.o obj/elevator.gb/midres_tile_020.o obj/elevator.gb/midres_tile_021.o obj/elevator.gb/midres_tile_022.o obj/elevator.gb/midres_tile_023.o obj/elevator.gb/midres_tile_025.o obj/elevator.gb/midres_tile_026.o obj/elevator.gb/midres_tile_027.o obj/elevator.gb/midres_tile_028.o obj/elevator.gb/midres_tile_multicolor_000.o obj/elevator.gb/midres_tile_multicolor_001.o obj/elevator.gb/midres_tile_multicolor_002.o obj/elevator.gb/midres_tile_multicolor_003.o obj/elevator.gb/midres_tile_multicolor_004.o obj/elevator.gb/midres_tile_multicolor_005.o obj/elevator.gb/midres_tile_multicolor_006.o obj/elevator.gb/midres_tile_multicolor_007.o obj/elevator.gb/midres_tile_multicolor_008.o obj/elevator.gb/midres_tile_multicolor_009.o obj/elevator.gb/midres_tile_multicolor_010.o obj/elevator.gb/midres_tile_multicolor_011.o obj/elevator.gb/midres_tile_multicolor_012.o obj/elevator.gb/midres_tile_processing_001.o obj/elevator.gb/midres_tile_processing_002.o obj/elevator.gb/midres_tile_processing_004.o obj/elevator.gb/midres_tile_processing_005.o obj/elevator.gb/midres_tile_processing_007.o obj/elevator.gb/midres_tile_processing_009.o obj/elevator.gb/midres_tile_processing_011.o obj/elevator.gb/midres_tile_processing_012.o obj/elevator.gb/midres_tile_processing_016.o obj/elevator.gb/midres_vanilla.o obj/elevator.gb/midres_vdp.o obj/elevator.gb/midres_vic.o obj/elevator.gb/midres_vic20.o obj/elevator.gb/utility_joycheck.o obj/elevator.gb/midres_control_011.o obj/elevator.gb/midres_data.o obj/elevator.gb/main.o -create-app 
	$(call COPYFILES,a.gb,$(EXEDIR)/elevator.gb.gb)
	$(call RMFILES,a.*)

                    
#!!! missing resources for joycheck (gb)

# find_main_module(totto)
# -> src/game_totto.c

# -------------------------------------------------------------------
# --- TOTTO FOR GB 
# -------------------------------------------------------------------
# 

.PHONY: totto.embedded.gb
totto.embedded.gb:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -i $(DATADIR)/totto.imf -n totto.imf -i $(DATADIR)/totto3.imf -n totto3.imf -c src/rawdata.c -h src/rawdata.h
	$(CC88) +gb $(CFLAGS) -c $(CFLAGS88)  -o obj/totto.gb/rawdata.o src/rawdata.c

obj/totto.gb/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.gb/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/totto.gb/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.gb/midres_io.o src/midres_io.asm

obj/totto.gb/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +gb $(CFLAGS) -c $(CFLAGS88)  -D__TOTTO__ -o $@ $(subst obj/totto.gb/,src/,$(@:.o=.c)) 

$(EXEDIR)/totto.gb:	totto.embedded.gb obj/totto.gb/demo_music.o obj/totto.gb/demo_tile.o obj/totto.gb/game_air_attack.o obj/totto.gb/game_alien_storm.o obj/totto.gb/game_elevator.o obj/totto.gb/game_totto.o obj/totto.gb/midres.o obj/totto.gb/midres_atari.o obj/totto.gb/midres_atmos.o obj/totto.gb/midres_ay8910.o obj/totto.gb/midres_c128.o obj/totto.gb/midres_c16.o obj/totto.gb/midres_c64.o obj/totto.gb/midres_coleco.o obj/totto.gb/midres_control_000.o obj/totto.gb/midres_control_001.o obj/totto.gb/midres_control_002.o obj/totto.gb/midres_control_003.o obj/totto.gb/midres_control_004.o obj/totto.gb/midres_control_005.o obj/totto.gb/midres_control_007.o obj/totto.gb/midres_control_010.o obj/totto.gb/midres_file_001.o obj/totto.gb/midres_file_003.o obj/totto.gb/midres_frame_000.o obj/totto.gb/midres_frame_001.o obj/totto.gb/midres_gb.o obj/totto.gb/midres_lm80c.o obj/totto.gb/midres_msx.o obj/totto.gb/midres_mtx500.o obj/totto.gb/midres_music_001.o obj/totto.gb/midres_music_002.o obj/totto.gb/midres_music_004.o obj/totto.gb/midres_music_005.o obj/totto.gb/midres_music_006.o obj/totto.gb/midres_plus4.o obj/totto.gb/midres_pokey.o obj/totto.gb/midres_screen_000.o obj/totto.gb/midres_screen_006.o obj/totto.gb/midres_screen_007.o obj/totto.gb/midres_screen_010.o obj/totto.gb/midres_screen_012.o obj/totto.gb/midres_screen_013.o obj/totto.gb/midres_sid.o obj/totto.gb/midres_sound_000.o obj/totto.gb/midres_sound_001.o obj/totto.gb/midres_sound_002.o obj/totto.gb/midres_sound_005.o obj/totto.gb/midres_sound_006.o obj/totto.gb/midres_sound_007.o obj/totto.gb/midres_svi.o obj/totto.gb/midres_ted.o obj/totto.gb/midres_tile_000.o obj/totto.gb/midres_tile_004.o obj/totto.gb/midres_tile_005.o obj/totto.gb/midres_tile_006.o obj/totto.gb/midres_tile_007.o obj/totto.gb/midres_tile_008.o obj/totto.gb/midres_tile_009.o obj/totto.gb/midres_tile_010.o obj/totto.gb/midres_tile_011.o obj/totto.gb/midres_tile_012.o obj/totto.gb/midres_tile_013.o obj/totto.gb/midres_tile_014.o obj/totto.gb/midres_tile_015.o obj/totto.gb/midres_tile_016.o obj/totto.gb/midres_tile_018.o obj/totto.gb/midres_tile_019.o obj/totto.gb/midres_tile_020.o obj/totto.gb/midres_tile_021.o obj/totto.gb/midres_tile_022.o obj/totto.gb/midres_tile_023.o obj/totto.gb/midres_tile_025.o obj/totto.gb/midres_tile_026.o obj/totto.gb/midres_tile_027.o obj/totto.gb/midres_tile_028.o obj/totto.gb/midres_tile_multicolor_000.o obj/totto.gb/midres_tile_multicolor_001.o obj/totto.gb/midres_tile_multicolor_002.o obj/totto.gb/midres_tile_multicolor_003.o obj/totto.gb/midres_tile_multicolor_004.o obj/totto.gb/midres_tile_multicolor_005.o obj/totto.gb/midres_tile_multicolor_006.o obj/totto.gb/midres_tile_multicolor_007.o obj/totto.gb/midres_tile_multicolor_008.o obj/totto.gb/midres_tile_multicolor_009.o obj/totto.gb/midres_tile_multicolor_010.o obj/totto.gb/midres_tile_multicolor_011.o obj/totto.gb/midres_tile_multicolor_012.o obj/totto.gb/midres_tile_processing_001.o obj/totto.gb/midres_tile_processing_002.o obj/totto.gb/midres_tile_processing_004.o obj/totto.gb/midres_tile_processing_005.o obj/totto.gb/midres_tile_processing_007.o obj/totto.gb/midres_tile_processing_009.o obj/totto.gb/midres_tile_processing_011.o obj/totto.gb/midres_tile_processing_012.o obj/totto.gb/midres_tile_processing_016.o obj/totto.gb/midres_vanilla.o obj/totto.gb/midres_vdp.o obj/totto.gb/midres_vic.o obj/totto.gb/midres_vic20.o obj/totto.gb/utility_joycheck.o obj/totto.gb/midres_control_011.o obj/totto.gb/midres_data.o obj/totto.gb/main.o obj/totto.gb/rawdata.o obj/totto.gb/midres_vdp_impl.o obj/totto.gb/midres_io.o
	$(CC88) +gb  -m $(LDFLAGS88) obj/totto.gb/rawdata.o obj/totto.gb/midres_io.o obj/totto.gb/midres_vdp_impl.o obj/totto.gb/demo_music.o obj/totto.gb/demo_tile.o obj/totto.gb/game_air_attack.o obj/totto.gb/game_alien_storm.o obj/totto.gb/game_elevator.o obj/totto.gb/game_totto.o obj/totto.gb/midres.o obj/totto.gb/midres_atari.o obj/totto.gb/midres_atmos.o obj/totto.gb/midres_ay8910.o obj/totto.gb/midres_c128.o obj/totto.gb/midres_c16.o obj/totto.gb/midres_c64.o obj/totto.gb/midres_coleco.o obj/totto.gb/midres_control_000.o obj/totto.gb/midres_control_001.o obj/totto.gb/midres_control_002.o obj/totto.gb/midres_control_003.o obj/totto.gb/midres_control_004.o obj/totto.gb/midres_control_005.o obj/totto.gb/midres_control_007.o obj/totto.gb/midres_control_010.o obj/totto.gb/midres_file_001.o obj/totto.gb/midres_file_003.o obj/totto.gb/midres_frame_000.o obj/totto.gb/midres_frame_001.o obj/totto.gb/midres_gb.o obj/totto.gb/midres_lm80c.o obj/totto.gb/midres_msx.o obj/totto.gb/midres_mtx500.o obj/totto.gb/midres_music_001.o obj/totto.gb/midres_music_002.o obj/totto.gb/midres_music_004.o obj/totto.gb/midres_music_005.o obj/totto.gb/midres_music_006.o obj/totto.gb/midres_plus4.o obj/totto.gb/midres_pokey.o obj/totto.gb/midres_screen_000.o obj/totto.gb/midres_screen_006.o obj/totto.gb/midres_screen_007.o obj/totto.gb/midres_screen_010.o obj/totto.gb/midres_screen_012.o obj/totto.gb/midres_screen_013.o obj/totto.gb/midres_sid.o obj/totto.gb/midres_sound_000.o obj/totto.gb/midres_sound_001.o obj/totto.gb/midres_sound_002.o obj/totto.gb/midres_sound_005.o obj/totto.gb/midres_sound_006.o obj/totto.gb/midres_sound_007.o obj/totto.gb/midres_svi.o obj/totto.gb/midres_ted.o obj/totto.gb/midres_tile_000.o obj/totto.gb/midres_tile_004.o obj/totto.gb/midres_tile_005.o obj/totto.gb/midres_tile_006.o obj/totto.gb/midres_tile_007.o obj/totto.gb/midres_tile_008.o obj/totto.gb/midres_tile_009.o obj/totto.gb/midres_tile_010.o obj/totto.gb/midres_tile_011.o obj/totto.gb/midres_tile_012.o obj/totto.gb/midres_tile_013.o obj/totto.gb/midres_tile_014.o obj/totto.gb/midres_tile_015.o obj/totto.gb/midres_tile_016.o obj/totto.gb/midres_tile_018.o obj/totto.gb/midres_tile_019.o obj/totto.gb/midres_tile_020.o obj/totto.gb/midres_tile_021.o obj/totto.gb/midres_tile_022.o obj/totto.gb/midres_tile_023.o obj/totto.gb/midres_tile_025.o obj/totto.gb/midres_tile_026.o obj/totto.gb/midres_tile_027.o obj/totto.gb/midres_tile_028.o obj/totto.gb/midres_tile_multicolor_000.o obj/totto.gb/midres_tile_multicolor_001.o obj/totto.gb/midres_tile_multicolor_002.o obj/totto.gb/midres_tile_multicolor_003.o obj/totto.gb/midres_tile_multicolor_004.o obj/totto.gb/midres_tile_multicolor_005.o obj/totto.gb/midres_tile_multicolor_006.o obj/totto.gb/midres_tile_multicolor_007.o obj/totto.gb/midres_tile_multicolor_008.o obj/totto.gb/midres_tile_multicolor_009.o obj/totto.gb/midres_tile_multicolor_010.o obj/totto.gb/midres_tile_multicolor_011.o obj/totto.gb/midres_tile_multicolor_012.o obj/totto.gb/midres_tile_processing_001.o obj/totto.gb/midres_tile_processing_002.o obj/totto.gb/midres_tile_processing_004.o obj/totto.gb/midres_tile_processing_005.o obj/totto.gb/midres_tile_processing_007.o obj/totto.gb/midres_tile_processing_009.o obj/totto.gb/midres_tile_processing_011.o obj/totto.gb/midres_tile_processing_012.o obj/totto.gb/midres_tile_processing_016.o obj/totto.gb/midres_vanilla.o obj/totto.gb/midres_vdp.o obj/totto.gb/midres_vic.o obj/totto.gb/midres_vic20.o obj/totto.gb/utility_joycheck.o obj/totto.gb/midres_control_011.o obj/totto.gb/midres_data.o obj/totto.gb/main.o -create-app 
	$(call COPYFILES,a.gb,$(EXEDIR)/totto.gb.gb)
	$(call RMFILES,a.*)

# find_main_module(airattack)
# -> src/game_air_attack.c

# -------------------------------------------------------------------
# --- AIRATTACK FOR LM80C 
# -------------------------------------------------------------------
# 

.PHONY: airattack.embedded.lm80c
airattack.embedded.lm80c:
	$(FILE2INCLUDE) -i $(DATADIR)/ztiles.bin -n ztiles.bin -i $(DATADIR)/aatiles4.bin -n zztiles.bin -i $(DATADIR)/aaintrox.pic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h
	$(CC88) +lm80c $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -o obj/airattack.lm80c/rawdata.o src/rawdata.c

obj/airattack.lm80c/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.lm80c/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/airattack.lm80c/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.lm80c/midres_io.o src/midres_io.asm

obj/airattack.lm80c/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +lm80c $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -D__AIRATTACK__ -o $@ $(subst obj/airattack.lm80c/,src/,$(@:.o=.c)) 

$(EXEDIR)/airattack.lm80c:	airattack.embedded.lm80c obj/airattack.lm80c/demo_music.o obj/airattack.lm80c/demo_tile.o obj/airattack.lm80c/game_air_attack.o obj/airattack.lm80c/game_alien_storm.o obj/airattack.lm80c/game_elevator.o obj/airattack.lm80c/game_totto.o obj/airattack.lm80c/midres.o obj/airattack.lm80c/midres_atari.o obj/airattack.lm80c/midres_atmos.o obj/airattack.lm80c/midres_ay8910.o obj/airattack.lm80c/midres_c128.o obj/airattack.lm80c/midres_c16.o obj/airattack.lm80c/midres_c64.o obj/airattack.lm80c/midres_coleco.o obj/airattack.lm80c/midres_control_000.o obj/airattack.lm80c/midres_control_001.o obj/airattack.lm80c/midres_control_002.o obj/airattack.lm80c/midres_control_003.o obj/airattack.lm80c/midres_control_004.o obj/airattack.lm80c/midres_control_005.o obj/airattack.lm80c/midres_control_007.o obj/airattack.lm80c/midres_control_010.o obj/airattack.lm80c/midres_file_001.o obj/airattack.lm80c/midres_file_003.o obj/airattack.lm80c/midres_frame_000.o obj/airattack.lm80c/midres_frame_001.o obj/airattack.lm80c/midres_gb.o obj/airattack.lm80c/midres_lm80c.o obj/airattack.lm80c/midres_msx.o obj/airattack.lm80c/midres_mtx500.o obj/airattack.lm80c/midres_music_001.o obj/airattack.lm80c/midres_music_002.o obj/airattack.lm80c/midres_music_004.o obj/airattack.lm80c/midres_music_005.o obj/airattack.lm80c/midres_music_006.o obj/airattack.lm80c/midres_plus4.o obj/airattack.lm80c/midres_pokey.o obj/airattack.lm80c/midres_screen_000.o obj/airattack.lm80c/midres_screen_006.o obj/airattack.lm80c/midres_screen_007.o obj/airattack.lm80c/midres_screen_010.o obj/airattack.lm80c/midres_screen_012.o obj/airattack.lm80c/midres_screen_013.o obj/airattack.lm80c/midres_sid.o obj/airattack.lm80c/midres_sound_000.o obj/airattack.lm80c/midres_sound_001.o obj/airattack.lm80c/midres_sound_002.o obj/airattack.lm80c/midres_sound_005.o obj/airattack.lm80c/midres_sound_006.o obj/airattack.lm80c/midres_sound_007.o obj/airattack.lm80c/midres_svi.o obj/airattack.lm80c/midres_ted.o obj/airattack.lm80c/midres_tile_000.o obj/airattack.lm80c/midres_tile_004.o obj/airattack.lm80c/midres_tile_005.o obj/airattack.lm80c/midres_tile_006.o obj/airattack.lm80c/midres_tile_007.o obj/airattack.lm80c/midres_tile_008.o obj/airattack.lm80c/midres_tile_009.o obj/airattack.lm80c/midres_tile_010.o obj/airattack.lm80c/midres_tile_011.o obj/airattack.lm80c/midres_tile_012.o obj/airattack.lm80c/midres_tile_013.o obj/airattack.lm80c/midres_tile_014.o obj/airattack.lm80c/midres_tile_015.o obj/airattack.lm80c/midres_tile_016.o obj/airattack.lm80c/midres_tile_018.o obj/airattack.lm80c/midres_tile_019.o obj/airattack.lm80c/midres_tile_020.o obj/airattack.lm80c/midres_tile_021.o obj/airattack.lm80c/midres_tile_022.o obj/airattack.lm80c/midres_tile_023.o obj/airattack.lm80c/midres_tile_025.o obj/airattack.lm80c/midres_tile_026.o obj/airattack.lm80c/midres_tile_027.o obj/airattack.lm80c/midres_tile_028.o obj/airattack.lm80c/midres_tile_multicolor_000.o obj/airattack.lm80c/midres_tile_multicolor_001.o obj/airattack.lm80c/midres_tile_multicolor_002.o obj/airattack.lm80c/midres_tile_multicolor_003.o obj/airattack.lm80c/midres_tile_multicolor_004.o obj/airattack.lm80c/midres_tile_multicolor_005.o obj/airattack.lm80c/midres_tile_multicolor_006.o obj/airattack.lm80c/midres_tile_multicolor_007.o obj/airattack.lm80c/midres_tile_multicolor_008.o obj/airattack.lm80c/midres_tile_multicolor_009.o obj/airattack.lm80c/midres_tile_multicolor_010.o obj/airattack.lm80c/midres_tile_multicolor_011.o obj/airattack.lm80c/midres_tile_multicolor_012.o obj/airattack.lm80c/midres_tile_processing_001.o obj/airattack.lm80c/midres_tile_processing_002.o obj/airattack.lm80c/midres_tile_processing_004.o obj/airattack.lm80c/midres_tile_processing_005.o obj/airattack.lm80c/midres_tile_processing_007.o obj/airattack.lm80c/midres_tile_processing_009.o obj/airattack.lm80c/midres_tile_processing_011.o obj/airattack.lm80c/midres_tile_processing_012.o obj/airattack.lm80c/midres_tile_processing_016.o obj/airattack.lm80c/midres_vanilla.o obj/airattack.lm80c/midres_vdp.o obj/airattack.lm80c/midres_vic.o obj/airattack.lm80c/midres_vic20.o obj/airattack.lm80c/utility_joycheck.o obj/airattack.lm80c/midres_control_011.o obj/airattack.lm80c/midres_data.o obj/airattack.lm80c/main.o obj/airattack.lm80c/rawdata.o obj/airattack.lm80c/midres_vdp_impl.o obj/airattack.lm80c/midres_io.o
	$(CC88) +lm80c  -m $(LDFLAGS88) obj/airattack.lm80c/rawdata.o obj/airattack.lm80c/midres_io.o obj/airattack.lm80c/midres_vdp_impl.o obj/airattack.lm80c/demo_music.o obj/airattack.lm80c/demo_tile.o obj/airattack.lm80c/game_air_attack.o obj/airattack.lm80c/game_alien_storm.o obj/airattack.lm80c/game_elevator.o obj/airattack.lm80c/game_totto.o obj/airattack.lm80c/midres.o obj/airattack.lm80c/midres_atari.o obj/airattack.lm80c/midres_atmos.o obj/airattack.lm80c/midres_ay8910.o obj/airattack.lm80c/midres_c128.o obj/airattack.lm80c/midres_c16.o obj/airattack.lm80c/midres_c64.o obj/airattack.lm80c/midres_coleco.o obj/airattack.lm80c/midres_control_000.o obj/airattack.lm80c/midres_control_001.o obj/airattack.lm80c/midres_control_002.o obj/airattack.lm80c/midres_control_003.o obj/airattack.lm80c/midres_control_004.o obj/airattack.lm80c/midres_control_005.o obj/airattack.lm80c/midres_control_007.o obj/airattack.lm80c/midres_control_010.o obj/airattack.lm80c/midres_file_001.o obj/airattack.lm80c/midres_file_003.o obj/airattack.lm80c/midres_frame_000.o obj/airattack.lm80c/midres_frame_001.o obj/airattack.lm80c/midres_gb.o obj/airattack.lm80c/midres_lm80c.o obj/airattack.lm80c/midres_msx.o obj/airattack.lm80c/midres_mtx500.o obj/airattack.lm80c/midres_music_001.o obj/airattack.lm80c/midres_music_002.o obj/airattack.lm80c/midres_music_004.o obj/airattack.lm80c/midres_music_005.o obj/airattack.lm80c/midres_music_006.o obj/airattack.lm80c/midres_plus4.o obj/airattack.lm80c/midres_pokey.o obj/airattack.lm80c/midres_screen_000.o obj/airattack.lm80c/midres_screen_006.o obj/airattack.lm80c/midres_screen_007.o obj/airattack.lm80c/midres_screen_010.o obj/airattack.lm80c/midres_screen_012.o obj/airattack.lm80c/midres_screen_013.o obj/airattack.lm80c/midres_sid.o obj/airattack.lm80c/midres_sound_000.o obj/airattack.lm80c/midres_sound_001.o obj/airattack.lm80c/midres_sound_002.o obj/airattack.lm80c/midres_sound_005.o obj/airattack.lm80c/midres_sound_006.o obj/airattack.lm80c/midres_sound_007.o obj/airattack.lm80c/midres_svi.o obj/airattack.lm80c/midres_ted.o obj/airattack.lm80c/midres_tile_000.o obj/airattack.lm80c/midres_tile_004.o obj/airattack.lm80c/midres_tile_005.o obj/airattack.lm80c/midres_tile_006.o obj/airattack.lm80c/midres_tile_007.o obj/airattack.lm80c/midres_tile_008.o obj/airattack.lm80c/midres_tile_009.o obj/airattack.lm80c/midres_tile_010.o obj/airattack.lm80c/midres_tile_011.o obj/airattack.lm80c/midres_tile_012.o obj/airattack.lm80c/midres_tile_013.o obj/airattack.lm80c/midres_tile_014.o obj/airattack.lm80c/midres_tile_015.o obj/airattack.lm80c/midres_tile_016.o obj/airattack.lm80c/midres_tile_018.o obj/airattack.lm80c/midres_tile_019.o obj/airattack.lm80c/midres_tile_020.o obj/airattack.lm80c/midres_tile_021.o obj/airattack.lm80c/midres_tile_022.o obj/airattack.lm80c/midres_tile_023.o obj/airattack.lm80c/midres_tile_025.o obj/airattack.lm80c/midres_tile_026.o obj/airattack.lm80c/midres_tile_027.o obj/airattack.lm80c/midres_tile_028.o obj/airattack.lm80c/midres_tile_multicolor_000.o obj/airattack.lm80c/midres_tile_multicolor_001.o obj/airattack.lm80c/midres_tile_multicolor_002.o obj/airattack.lm80c/midres_tile_multicolor_003.o obj/airattack.lm80c/midres_tile_multicolor_004.o obj/airattack.lm80c/midres_tile_multicolor_005.o obj/airattack.lm80c/midres_tile_multicolor_006.o obj/airattack.lm80c/midres_tile_multicolor_007.o obj/airattack.lm80c/midres_tile_multicolor_008.o obj/airattack.lm80c/midres_tile_multicolor_009.o obj/airattack.lm80c/midres_tile_multicolor_010.o obj/airattack.lm80c/midres_tile_multicolor_011.o obj/airattack.lm80c/midres_tile_multicolor_012.o obj/airattack.lm80c/midres_tile_processing_001.o obj/airattack.lm80c/midres_tile_processing_002.o obj/airattack.lm80c/midres_tile_processing_004.o obj/airattack.lm80c/midres_tile_processing_005.o obj/airattack.lm80c/midres_tile_processing_007.o obj/airattack.lm80c/midres_tile_processing_009.o obj/airattack.lm80c/midres_tile_processing_011.o obj/airattack.lm80c/midres_tile_processing_012.o obj/airattack.lm80c/midres_tile_processing_016.o obj/airattack.lm80c/midres_vanilla.o obj/airattack.lm80c/midres_vdp.o obj/airattack.lm80c/midres_vic.o obj/airattack.lm80c/midres_vic20.o obj/airattack.lm80c/utility_joycheck.o obj/airattack.lm80c/midres_control_011.o obj/airattack.lm80c/midres_data.o obj/airattack.lm80c/main.o -create-app 
	$(call COPYFILES,a.PRG,$(EXEDIR)/airattack.lm80c.prg)
	$(call RMFILES,a.*)

                    
#!!! missing resources for alienstorm (lm80c)

# find_main_module(elevator)
# -> src/game_elevator.c

# -------------------------------------------------------------------
# --- ELEVATOR FOR LM80C 
# -------------------------------------------------------------------
# 

.PHONY: elevator.embedded.lm80c
elevator.embedded.lm80c:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevatora.mpic -n zelintro.bin -i $(DATADIR)/elevator1.imf -n elevator1.imf -c src/rawdata.c -h src/rawdata.h
	$(CC88) +lm80c $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -o obj/elevator.lm80c/rawdata.o src/rawdata.c

obj/elevator.lm80c/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.lm80c/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/elevator.lm80c/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.lm80c/midres_io.o src/midres_io.asm

obj/elevator.lm80c/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +lm80c $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -D__ELEVATOR__ -o $@ $(subst obj/elevator.lm80c/,src/,$(@:.o=.c)) 

$(EXEDIR)/elevator.lm80c:	elevator.embedded.lm80c obj/elevator.lm80c/demo_music.o obj/elevator.lm80c/demo_tile.o obj/elevator.lm80c/game_air_attack.o obj/elevator.lm80c/game_alien_storm.o obj/elevator.lm80c/game_elevator.o obj/elevator.lm80c/game_totto.o obj/elevator.lm80c/midres.o obj/elevator.lm80c/midres_atari.o obj/elevator.lm80c/midres_atmos.o obj/elevator.lm80c/midres_ay8910.o obj/elevator.lm80c/midres_c128.o obj/elevator.lm80c/midres_c16.o obj/elevator.lm80c/midres_c64.o obj/elevator.lm80c/midres_coleco.o obj/elevator.lm80c/midres_control_000.o obj/elevator.lm80c/midres_control_001.o obj/elevator.lm80c/midres_control_002.o obj/elevator.lm80c/midres_control_003.o obj/elevator.lm80c/midres_control_004.o obj/elevator.lm80c/midres_control_005.o obj/elevator.lm80c/midres_control_007.o obj/elevator.lm80c/midres_control_010.o obj/elevator.lm80c/midres_file_001.o obj/elevator.lm80c/midres_file_003.o obj/elevator.lm80c/midres_frame_000.o obj/elevator.lm80c/midres_frame_001.o obj/elevator.lm80c/midres_gb.o obj/elevator.lm80c/midres_lm80c.o obj/elevator.lm80c/midres_msx.o obj/elevator.lm80c/midres_mtx500.o obj/elevator.lm80c/midres_music_001.o obj/elevator.lm80c/midres_music_002.o obj/elevator.lm80c/midres_music_004.o obj/elevator.lm80c/midres_music_005.o obj/elevator.lm80c/midres_music_006.o obj/elevator.lm80c/midres_plus4.o obj/elevator.lm80c/midres_pokey.o obj/elevator.lm80c/midres_screen_000.o obj/elevator.lm80c/midres_screen_006.o obj/elevator.lm80c/midres_screen_007.o obj/elevator.lm80c/midres_screen_010.o obj/elevator.lm80c/midres_screen_012.o obj/elevator.lm80c/midres_screen_013.o obj/elevator.lm80c/midres_sid.o obj/elevator.lm80c/midres_sound_000.o obj/elevator.lm80c/midres_sound_001.o obj/elevator.lm80c/midres_sound_002.o obj/elevator.lm80c/midres_sound_005.o obj/elevator.lm80c/midres_sound_006.o obj/elevator.lm80c/midres_sound_007.o obj/elevator.lm80c/midres_svi.o obj/elevator.lm80c/midres_ted.o obj/elevator.lm80c/midres_tile_000.o obj/elevator.lm80c/midres_tile_004.o obj/elevator.lm80c/midres_tile_005.o obj/elevator.lm80c/midres_tile_006.o obj/elevator.lm80c/midres_tile_007.o obj/elevator.lm80c/midres_tile_008.o obj/elevator.lm80c/midres_tile_009.o obj/elevator.lm80c/midres_tile_010.o obj/elevator.lm80c/midres_tile_011.o obj/elevator.lm80c/midres_tile_012.o obj/elevator.lm80c/midres_tile_013.o obj/elevator.lm80c/midres_tile_014.o obj/elevator.lm80c/midres_tile_015.o obj/elevator.lm80c/midres_tile_016.o obj/elevator.lm80c/midres_tile_018.o obj/elevator.lm80c/midres_tile_019.o obj/elevator.lm80c/midres_tile_020.o obj/elevator.lm80c/midres_tile_021.o obj/elevator.lm80c/midres_tile_022.o obj/elevator.lm80c/midres_tile_023.o obj/elevator.lm80c/midres_tile_025.o obj/elevator.lm80c/midres_tile_026.o obj/elevator.lm80c/midres_tile_027.o obj/elevator.lm80c/midres_tile_028.o obj/elevator.lm80c/midres_tile_multicolor_000.o obj/elevator.lm80c/midres_tile_multicolor_001.o obj/elevator.lm80c/midres_tile_multicolor_002.o obj/elevator.lm80c/midres_tile_multicolor_003.o obj/elevator.lm80c/midres_tile_multicolor_004.o obj/elevator.lm80c/midres_tile_multicolor_005.o obj/elevator.lm80c/midres_tile_multicolor_006.o obj/elevator.lm80c/midres_tile_multicolor_007.o obj/elevator.lm80c/midres_tile_multicolor_008.o obj/elevator.lm80c/midres_tile_multicolor_009.o obj/elevator.lm80c/midres_tile_multicolor_010.o obj/elevator.lm80c/midres_tile_multicolor_011.o obj/elevator.lm80c/midres_tile_multicolor_012.o obj/elevator.lm80c/midres_tile_processing_001.o obj/elevator.lm80c/midres_tile_processing_002.o obj/elevator.lm80c/midres_tile_processing_004.o obj/elevator.lm80c/midres_tile_processing_005.o obj/elevator.lm80c/midres_tile_processing_007.o obj/elevator.lm80c/midres_tile_processing_009.o obj/elevator.lm80c/midres_tile_processing_011.o obj/elevator.lm80c/midres_tile_processing_012.o obj/elevator.lm80c/midres_tile_processing_016.o obj/elevator.lm80c/midres_vanilla.o obj/elevator.lm80c/midres_vdp.o obj/elevator.lm80c/midres_vic.o obj/elevator.lm80c/midres_vic20.o obj/elevator.lm80c/utility_joycheck.o obj/elevator.lm80c/midres_control_011.o obj/elevator.lm80c/midres_data.o obj/elevator.lm80c/main.o obj/elevator.lm80c/rawdata.o obj/elevator.lm80c/midres_vdp_impl.o obj/elevator.lm80c/midres_io.o
	$(CC88) +lm80c  -m $(LDFLAGS88) obj/elevator.lm80c/rawdata.o obj/elevator.lm80c/midres_io.o obj/elevator.lm80c/midres_vdp_impl.o obj/elevator.lm80c/demo_music.o obj/elevator.lm80c/demo_tile.o obj/elevator.lm80c/game_air_attack.o obj/elevator.lm80c/game_alien_storm.o obj/elevator.lm80c/game_elevator.o obj/elevator.lm80c/game_totto.o obj/elevator.lm80c/midres.o obj/elevator.lm80c/midres_atari.o obj/elevator.lm80c/midres_atmos.o obj/elevator.lm80c/midres_ay8910.o obj/elevator.lm80c/midres_c128.o obj/elevator.lm80c/midres_c16.o obj/elevator.lm80c/midres_c64.o obj/elevator.lm80c/midres_coleco.o obj/elevator.lm80c/midres_control_000.o obj/elevator.lm80c/midres_control_001.o obj/elevator.lm80c/midres_control_002.o obj/elevator.lm80c/midres_control_003.o obj/elevator.lm80c/midres_control_004.o obj/elevator.lm80c/midres_control_005.o obj/elevator.lm80c/midres_control_007.o obj/elevator.lm80c/midres_control_010.o obj/elevator.lm80c/midres_file_001.o obj/elevator.lm80c/midres_file_003.o obj/elevator.lm80c/midres_frame_000.o obj/elevator.lm80c/midres_frame_001.o obj/elevator.lm80c/midres_gb.o obj/elevator.lm80c/midres_lm80c.o obj/elevator.lm80c/midres_msx.o obj/elevator.lm80c/midres_mtx500.o obj/elevator.lm80c/midres_music_001.o obj/elevator.lm80c/midres_music_002.o obj/elevator.lm80c/midres_music_004.o obj/elevator.lm80c/midres_music_005.o obj/elevator.lm80c/midres_music_006.o obj/elevator.lm80c/midres_plus4.o obj/elevator.lm80c/midres_pokey.o obj/elevator.lm80c/midres_screen_000.o obj/elevator.lm80c/midres_screen_006.o obj/elevator.lm80c/midres_screen_007.o obj/elevator.lm80c/midres_screen_010.o obj/elevator.lm80c/midres_screen_012.o obj/elevator.lm80c/midres_screen_013.o obj/elevator.lm80c/midres_sid.o obj/elevator.lm80c/midres_sound_000.o obj/elevator.lm80c/midres_sound_001.o obj/elevator.lm80c/midres_sound_002.o obj/elevator.lm80c/midres_sound_005.o obj/elevator.lm80c/midres_sound_006.o obj/elevator.lm80c/midres_sound_007.o obj/elevator.lm80c/midres_svi.o obj/elevator.lm80c/midres_ted.o obj/elevator.lm80c/midres_tile_000.o obj/elevator.lm80c/midres_tile_004.o obj/elevator.lm80c/midres_tile_005.o obj/elevator.lm80c/midres_tile_006.o obj/elevator.lm80c/midres_tile_007.o obj/elevator.lm80c/midres_tile_008.o obj/elevator.lm80c/midres_tile_009.o obj/elevator.lm80c/midres_tile_010.o obj/elevator.lm80c/midres_tile_011.o obj/elevator.lm80c/midres_tile_012.o obj/elevator.lm80c/midres_tile_013.o obj/elevator.lm80c/midres_tile_014.o obj/elevator.lm80c/midres_tile_015.o obj/elevator.lm80c/midres_tile_016.o obj/elevator.lm80c/midres_tile_018.o obj/elevator.lm80c/midres_tile_019.o obj/elevator.lm80c/midres_tile_020.o obj/elevator.lm80c/midres_tile_021.o obj/elevator.lm80c/midres_tile_022.o obj/elevator.lm80c/midres_tile_023.o obj/elevator.lm80c/midres_tile_025.o obj/elevator.lm80c/midres_tile_026.o obj/elevator.lm80c/midres_tile_027.o obj/elevator.lm80c/midres_tile_028.o obj/elevator.lm80c/midres_tile_multicolor_000.o obj/elevator.lm80c/midres_tile_multicolor_001.o obj/elevator.lm80c/midres_tile_multicolor_002.o obj/elevator.lm80c/midres_tile_multicolor_003.o obj/elevator.lm80c/midres_tile_multicolor_004.o obj/elevator.lm80c/midres_tile_multicolor_005.o obj/elevator.lm80c/midres_tile_multicolor_006.o obj/elevator.lm80c/midres_tile_multicolor_007.o obj/elevator.lm80c/midres_tile_multicolor_008.o obj/elevator.lm80c/midres_tile_multicolor_009.o obj/elevator.lm80c/midres_tile_multicolor_010.o obj/elevator.lm80c/midres_tile_multicolor_011.o obj/elevator.lm80c/midres_tile_multicolor_012.o obj/elevator.lm80c/midres_tile_processing_001.o obj/elevator.lm80c/midres_tile_processing_002.o obj/elevator.lm80c/midres_tile_processing_004.o obj/elevator.lm80c/midres_tile_processing_005.o obj/elevator.lm80c/midres_tile_processing_007.o obj/elevator.lm80c/midres_tile_processing_009.o obj/elevator.lm80c/midres_tile_processing_011.o obj/elevator.lm80c/midres_tile_processing_012.o obj/elevator.lm80c/midres_tile_processing_016.o obj/elevator.lm80c/midres_vanilla.o obj/elevator.lm80c/midres_vdp.o obj/elevator.lm80c/midres_vic.o obj/elevator.lm80c/midres_vic20.o obj/elevator.lm80c/utility_joycheck.o obj/elevator.lm80c/midres_control_011.o obj/elevator.lm80c/midres_data.o obj/elevator.lm80c/main.o -create-app 
	$(call COPYFILES,a.PRG,$(EXEDIR)/elevator.lm80c.prg)
	$(call RMFILES,a.*)

                    
#!!! missing resources for joycheck (lm80c)

# find_main_module(totto)
# -> src/game_totto.c

# -------------------------------------------------------------------
# --- TOTTO FOR LM80C 
# -------------------------------------------------------------------
# 

.PHONY: totto.embedded.lm80c
totto.embedded.lm80c:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -i $(DATADIR)/totto.imf -n totto.imf -i $(DATADIR)/totto3.imf -n totto3.imf -c src/rawdata.c -h src/rawdata.h
	$(CC88) +lm80c $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -o obj/totto.lm80c/rawdata.o src/rawdata.c

obj/totto.lm80c/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.lm80c/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/totto.lm80c/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.lm80c/midres_io.o src/midres_io.asm

obj/totto.lm80c/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +lm80c $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -D__TOTTO__ -o $@ $(subst obj/totto.lm80c/,src/,$(@:.o=.c)) 

$(EXEDIR)/totto.lm80c:	totto.embedded.lm80c obj/totto.lm80c/demo_music.o obj/totto.lm80c/demo_tile.o obj/totto.lm80c/game_air_attack.o obj/totto.lm80c/game_alien_storm.o obj/totto.lm80c/game_elevator.o obj/totto.lm80c/game_totto.o obj/totto.lm80c/midres.o obj/totto.lm80c/midres_atari.o obj/totto.lm80c/midres_atmos.o obj/totto.lm80c/midres_ay8910.o obj/totto.lm80c/midres_c128.o obj/totto.lm80c/midres_c16.o obj/totto.lm80c/midres_c64.o obj/totto.lm80c/midres_coleco.o obj/totto.lm80c/midres_control_000.o obj/totto.lm80c/midres_control_001.o obj/totto.lm80c/midres_control_002.o obj/totto.lm80c/midres_control_003.o obj/totto.lm80c/midres_control_004.o obj/totto.lm80c/midres_control_005.o obj/totto.lm80c/midres_control_007.o obj/totto.lm80c/midres_control_010.o obj/totto.lm80c/midres_file_001.o obj/totto.lm80c/midres_file_003.o obj/totto.lm80c/midres_frame_000.o obj/totto.lm80c/midres_frame_001.o obj/totto.lm80c/midres_gb.o obj/totto.lm80c/midres_lm80c.o obj/totto.lm80c/midres_msx.o obj/totto.lm80c/midres_mtx500.o obj/totto.lm80c/midres_music_001.o obj/totto.lm80c/midres_music_002.o obj/totto.lm80c/midres_music_004.o obj/totto.lm80c/midres_music_005.o obj/totto.lm80c/midres_music_006.o obj/totto.lm80c/midres_plus4.o obj/totto.lm80c/midres_pokey.o obj/totto.lm80c/midres_screen_000.o obj/totto.lm80c/midres_screen_006.o obj/totto.lm80c/midres_screen_007.o obj/totto.lm80c/midres_screen_010.o obj/totto.lm80c/midres_screen_012.o obj/totto.lm80c/midres_screen_013.o obj/totto.lm80c/midres_sid.o obj/totto.lm80c/midres_sound_000.o obj/totto.lm80c/midres_sound_001.o obj/totto.lm80c/midres_sound_002.o obj/totto.lm80c/midres_sound_005.o obj/totto.lm80c/midres_sound_006.o obj/totto.lm80c/midres_sound_007.o obj/totto.lm80c/midres_svi.o obj/totto.lm80c/midres_ted.o obj/totto.lm80c/midres_tile_000.o obj/totto.lm80c/midres_tile_004.o obj/totto.lm80c/midres_tile_005.o obj/totto.lm80c/midres_tile_006.o obj/totto.lm80c/midres_tile_007.o obj/totto.lm80c/midres_tile_008.o obj/totto.lm80c/midres_tile_009.o obj/totto.lm80c/midres_tile_010.o obj/totto.lm80c/midres_tile_011.o obj/totto.lm80c/midres_tile_012.o obj/totto.lm80c/midres_tile_013.o obj/totto.lm80c/midres_tile_014.o obj/totto.lm80c/midres_tile_015.o obj/totto.lm80c/midres_tile_016.o obj/totto.lm80c/midres_tile_018.o obj/totto.lm80c/midres_tile_019.o obj/totto.lm80c/midres_tile_020.o obj/totto.lm80c/midres_tile_021.o obj/totto.lm80c/midres_tile_022.o obj/totto.lm80c/midres_tile_023.o obj/totto.lm80c/midres_tile_025.o obj/totto.lm80c/midres_tile_026.o obj/totto.lm80c/midres_tile_027.o obj/totto.lm80c/midres_tile_028.o obj/totto.lm80c/midres_tile_multicolor_000.o obj/totto.lm80c/midres_tile_multicolor_001.o obj/totto.lm80c/midres_tile_multicolor_002.o obj/totto.lm80c/midres_tile_multicolor_003.o obj/totto.lm80c/midres_tile_multicolor_004.o obj/totto.lm80c/midres_tile_multicolor_005.o obj/totto.lm80c/midres_tile_multicolor_006.o obj/totto.lm80c/midres_tile_multicolor_007.o obj/totto.lm80c/midres_tile_multicolor_008.o obj/totto.lm80c/midres_tile_multicolor_009.o obj/totto.lm80c/midres_tile_multicolor_010.o obj/totto.lm80c/midres_tile_multicolor_011.o obj/totto.lm80c/midres_tile_multicolor_012.o obj/totto.lm80c/midres_tile_processing_001.o obj/totto.lm80c/midres_tile_processing_002.o obj/totto.lm80c/midres_tile_processing_004.o obj/totto.lm80c/midres_tile_processing_005.o obj/totto.lm80c/midres_tile_processing_007.o obj/totto.lm80c/midres_tile_processing_009.o obj/totto.lm80c/midres_tile_processing_011.o obj/totto.lm80c/midres_tile_processing_012.o obj/totto.lm80c/midres_tile_processing_016.o obj/totto.lm80c/midres_vanilla.o obj/totto.lm80c/midres_vdp.o obj/totto.lm80c/midres_vic.o obj/totto.lm80c/midres_vic20.o obj/totto.lm80c/utility_joycheck.o obj/totto.lm80c/midres_control_011.o obj/totto.lm80c/midres_data.o obj/totto.lm80c/main.o obj/totto.lm80c/rawdata.o obj/totto.lm80c/midres_vdp_impl.o obj/totto.lm80c/midres_io.o
	$(CC88) +lm80c  -m $(LDFLAGS88) obj/totto.lm80c/rawdata.o obj/totto.lm80c/midres_io.o obj/totto.lm80c/midres_vdp_impl.o obj/totto.lm80c/demo_music.o obj/totto.lm80c/demo_tile.o obj/totto.lm80c/game_air_attack.o obj/totto.lm80c/game_alien_storm.o obj/totto.lm80c/game_elevator.o obj/totto.lm80c/game_totto.o obj/totto.lm80c/midres.o obj/totto.lm80c/midres_atari.o obj/totto.lm80c/midres_atmos.o obj/totto.lm80c/midres_ay8910.o obj/totto.lm80c/midres_c128.o obj/totto.lm80c/midres_c16.o obj/totto.lm80c/midres_c64.o obj/totto.lm80c/midres_coleco.o obj/totto.lm80c/midres_control_000.o obj/totto.lm80c/midres_control_001.o obj/totto.lm80c/midres_control_002.o obj/totto.lm80c/midres_control_003.o obj/totto.lm80c/midres_control_004.o obj/totto.lm80c/midres_control_005.o obj/totto.lm80c/midres_control_007.o obj/totto.lm80c/midres_control_010.o obj/totto.lm80c/midres_file_001.o obj/totto.lm80c/midres_file_003.o obj/totto.lm80c/midres_frame_000.o obj/totto.lm80c/midres_frame_001.o obj/totto.lm80c/midres_gb.o obj/totto.lm80c/midres_lm80c.o obj/totto.lm80c/midres_msx.o obj/totto.lm80c/midres_mtx500.o obj/totto.lm80c/midres_music_001.o obj/totto.lm80c/midres_music_002.o obj/totto.lm80c/midres_music_004.o obj/totto.lm80c/midres_music_005.o obj/totto.lm80c/midres_music_006.o obj/totto.lm80c/midres_plus4.o obj/totto.lm80c/midres_pokey.o obj/totto.lm80c/midres_screen_000.o obj/totto.lm80c/midres_screen_006.o obj/totto.lm80c/midres_screen_007.o obj/totto.lm80c/midres_screen_010.o obj/totto.lm80c/midres_screen_012.o obj/totto.lm80c/midres_screen_013.o obj/totto.lm80c/midres_sid.o obj/totto.lm80c/midres_sound_000.o obj/totto.lm80c/midres_sound_001.o obj/totto.lm80c/midres_sound_002.o obj/totto.lm80c/midres_sound_005.o obj/totto.lm80c/midres_sound_006.o obj/totto.lm80c/midres_sound_007.o obj/totto.lm80c/midres_svi.o obj/totto.lm80c/midres_ted.o obj/totto.lm80c/midres_tile_000.o obj/totto.lm80c/midres_tile_004.o obj/totto.lm80c/midres_tile_005.o obj/totto.lm80c/midres_tile_006.o obj/totto.lm80c/midres_tile_007.o obj/totto.lm80c/midres_tile_008.o obj/totto.lm80c/midres_tile_009.o obj/totto.lm80c/midres_tile_010.o obj/totto.lm80c/midres_tile_011.o obj/totto.lm80c/midres_tile_012.o obj/totto.lm80c/midres_tile_013.o obj/totto.lm80c/midres_tile_014.o obj/totto.lm80c/midres_tile_015.o obj/totto.lm80c/midres_tile_016.o obj/totto.lm80c/midres_tile_018.o obj/totto.lm80c/midres_tile_019.o obj/totto.lm80c/midres_tile_020.o obj/totto.lm80c/midres_tile_021.o obj/totto.lm80c/midres_tile_022.o obj/totto.lm80c/midres_tile_023.o obj/totto.lm80c/midres_tile_025.o obj/totto.lm80c/midres_tile_026.o obj/totto.lm80c/midres_tile_027.o obj/totto.lm80c/midres_tile_028.o obj/totto.lm80c/midres_tile_multicolor_000.o obj/totto.lm80c/midres_tile_multicolor_001.o obj/totto.lm80c/midres_tile_multicolor_002.o obj/totto.lm80c/midres_tile_multicolor_003.o obj/totto.lm80c/midres_tile_multicolor_004.o obj/totto.lm80c/midres_tile_multicolor_005.o obj/totto.lm80c/midres_tile_multicolor_006.o obj/totto.lm80c/midres_tile_multicolor_007.o obj/totto.lm80c/midres_tile_multicolor_008.o obj/totto.lm80c/midres_tile_multicolor_009.o obj/totto.lm80c/midres_tile_multicolor_010.o obj/totto.lm80c/midres_tile_multicolor_011.o obj/totto.lm80c/midres_tile_multicolor_012.o obj/totto.lm80c/midres_tile_processing_001.o obj/totto.lm80c/midres_tile_processing_002.o obj/totto.lm80c/midres_tile_processing_004.o obj/totto.lm80c/midres_tile_processing_005.o obj/totto.lm80c/midres_tile_processing_007.o obj/totto.lm80c/midres_tile_processing_009.o obj/totto.lm80c/midres_tile_processing_011.o obj/totto.lm80c/midres_tile_processing_012.o obj/totto.lm80c/midres_tile_processing_016.o obj/totto.lm80c/midres_vanilla.o obj/totto.lm80c/midres_vdp.o obj/totto.lm80c/midres_vic.o obj/totto.lm80c/midres_vic20.o obj/totto.lm80c/utility_joycheck.o obj/totto.lm80c/midres_control_011.o obj/totto.lm80c/midres_data.o obj/totto.lm80c/main.o -create-app 
	$(call COPYFILES,a.PRG,$(EXEDIR)/totto.lm80c.prg)
	$(call RMFILES,a.*)

# find_main_module(airattack)
# -> src/game_air_attack.c

# -------------------------------------------------------------------
# --- AIRATTACK FOR MSX 
# -------------------------------------------------------------------
# 

.PHONY: airattack.embedded.msx
airattack.embedded.msx:
	$(FILE2INCLUDE) -i $(DATADIR)/ztiles.bin -n ztiles.bin -i $(DATADIR)/aatiles4.bin -n zztiles.bin -i $(DATADIR)/aaintrox.pic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -o obj/airattack.msx/rawdata.o src/rawdata.c

obj/airattack.msx/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.msx/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/airattack.msx/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.msx/midres_io.o src/midres_io.asm

obj/airattack.msx/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -D__AIRATTACK__ -o $@ $(subst obj/airattack.msx/,src/,$(@:.o=.c)) 

$(EXEDIR)/airattack.msx:	airattack.embedded.msx obj/airattack.msx/demo_music.o obj/airattack.msx/demo_tile.o obj/airattack.msx/game_air_attack.o obj/airattack.msx/game_alien_storm.o obj/airattack.msx/game_elevator.o obj/airattack.msx/game_totto.o obj/airattack.msx/midres.o obj/airattack.msx/midres_atari.o obj/airattack.msx/midres_atmos.o obj/airattack.msx/midres_ay8910.o obj/airattack.msx/midres_c128.o obj/airattack.msx/midres_c16.o obj/airattack.msx/midres_c64.o obj/airattack.msx/midres_coleco.o obj/airattack.msx/midres_control_000.o obj/airattack.msx/midres_control_001.o obj/airattack.msx/midres_control_002.o obj/airattack.msx/midres_control_003.o obj/airattack.msx/midres_control_004.o obj/airattack.msx/midres_control_005.o obj/airattack.msx/midres_control_007.o obj/airattack.msx/midres_control_010.o obj/airattack.msx/midres_file_001.o obj/airattack.msx/midres_file_003.o obj/airattack.msx/midres_frame_000.o obj/airattack.msx/midres_frame_001.o obj/airattack.msx/midres_gb.o obj/airattack.msx/midres_lm80c.o obj/airattack.msx/midres_msx.o obj/airattack.msx/midres_mtx500.o obj/airattack.msx/midres_music_001.o obj/airattack.msx/midres_music_002.o obj/airattack.msx/midres_music_004.o obj/airattack.msx/midres_music_005.o obj/airattack.msx/midres_music_006.o obj/airattack.msx/midres_plus4.o obj/airattack.msx/midres_pokey.o obj/airattack.msx/midres_screen_000.o obj/airattack.msx/midres_screen_006.o obj/airattack.msx/midres_screen_007.o obj/airattack.msx/midres_screen_010.o obj/airattack.msx/midres_screen_012.o obj/airattack.msx/midres_screen_013.o obj/airattack.msx/midres_sid.o obj/airattack.msx/midres_sound_000.o obj/airattack.msx/midres_sound_001.o obj/airattack.msx/midres_sound_002.o obj/airattack.msx/midres_sound_005.o obj/airattack.msx/midres_sound_006.o obj/airattack.msx/midres_sound_007.o obj/airattack.msx/midres_svi.o obj/airattack.msx/midres_ted.o obj/airattack.msx/midres_tile_000.o obj/airattack.msx/midres_tile_004.o obj/airattack.msx/midres_tile_005.o obj/airattack.msx/midres_tile_006.o obj/airattack.msx/midres_tile_007.o obj/airattack.msx/midres_tile_008.o obj/airattack.msx/midres_tile_009.o obj/airattack.msx/midres_tile_010.o obj/airattack.msx/midres_tile_011.o obj/airattack.msx/midres_tile_012.o obj/airattack.msx/midres_tile_013.o obj/airattack.msx/midres_tile_014.o obj/airattack.msx/midres_tile_015.o obj/airattack.msx/midres_tile_016.o obj/airattack.msx/midres_tile_018.o obj/airattack.msx/midres_tile_019.o obj/airattack.msx/midres_tile_020.o obj/airattack.msx/midres_tile_021.o obj/airattack.msx/midres_tile_022.o obj/airattack.msx/midres_tile_023.o obj/airattack.msx/midres_tile_025.o obj/airattack.msx/midres_tile_026.o obj/airattack.msx/midres_tile_027.o obj/airattack.msx/midres_tile_028.o obj/airattack.msx/midres_tile_multicolor_000.o obj/airattack.msx/midres_tile_multicolor_001.o obj/airattack.msx/midres_tile_multicolor_002.o obj/airattack.msx/midres_tile_multicolor_003.o obj/airattack.msx/midres_tile_multicolor_004.o obj/airattack.msx/midres_tile_multicolor_005.o obj/airattack.msx/midres_tile_multicolor_006.o obj/airattack.msx/midres_tile_multicolor_007.o obj/airattack.msx/midres_tile_multicolor_008.o obj/airattack.msx/midres_tile_multicolor_009.o obj/airattack.msx/midres_tile_multicolor_010.o obj/airattack.msx/midres_tile_multicolor_011.o obj/airattack.msx/midres_tile_multicolor_012.o obj/airattack.msx/midres_tile_processing_001.o obj/airattack.msx/midres_tile_processing_002.o obj/airattack.msx/midres_tile_processing_004.o obj/airattack.msx/midres_tile_processing_005.o obj/airattack.msx/midres_tile_processing_007.o obj/airattack.msx/midres_tile_processing_009.o obj/airattack.msx/midres_tile_processing_011.o obj/airattack.msx/midres_tile_processing_012.o obj/airattack.msx/midres_tile_processing_016.o obj/airattack.msx/midres_vanilla.o obj/airattack.msx/midres_vdp.o obj/airattack.msx/midres_vic.o obj/airattack.msx/midres_vic20.o obj/airattack.msx/utility_joycheck.o obj/airattack.msx/midres_control_011.o obj/airattack.msx/midres_data.o obj/airattack.msx/main.o obj/airattack.msx/rawdata.o obj/airattack.msx/midres_vdp_impl.o obj/airattack.msx/midres_io.o
	$(CC88) +msx -subtype=rom -m $(LDFLAGS88) obj/airattack.msx/rawdata.o obj/airattack.msx/midres_io.o obj/airattack.msx/midres_vdp_impl.o obj/airattack.msx/demo_music.o obj/airattack.msx/demo_tile.o obj/airattack.msx/game_air_attack.o obj/airattack.msx/game_alien_storm.o obj/airattack.msx/game_elevator.o obj/airattack.msx/game_totto.o obj/airattack.msx/midres.o obj/airattack.msx/midres_atari.o obj/airattack.msx/midres_atmos.o obj/airattack.msx/midres_ay8910.o obj/airattack.msx/midres_c128.o obj/airattack.msx/midres_c16.o obj/airattack.msx/midres_c64.o obj/airattack.msx/midres_coleco.o obj/airattack.msx/midres_control_000.o obj/airattack.msx/midres_control_001.o obj/airattack.msx/midres_control_002.o obj/airattack.msx/midres_control_003.o obj/airattack.msx/midres_control_004.o obj/airattack.msx/midres_control_005.o obj/airattack.msx/midres_control_007.o obj/airattack.msx/midres_control_010.o obj/airattack.msx/midres_file_001.o obj/airattack.msx/midres_file_003.o obj/airattack.msx/midres_frame_000.o obj/airattack.msx/midres_frame_001.o obj/airattack.msx/midres_gb.o obj/airattack.msx/midres_lm80c.o obj/airattack.msx/midres_msx.o obj/airattack.msx/midres_mtx500.o obj/airattack.msx/midres_music_001.o obj/airattack.msx/midres_music_002.o obj/airattack.msx/midres_music_004.o obj/airattack.msx/midres_music_005.o obj/airattack.msx/midres_music_006.o obj/airattack.msx/midres_plus4.o obj/airattack.msx/midres_pokey.o obj/airattack.msx/midres_screen_000.o obj/airattack.msx/midres_screen_006.o obj/airattack.msx/midres_screen_007.o obj/airattack.msx/midres_screen_010.o obj/airattack.msx/midres_screen_012.o obj/airattack.msx/midres_screen_013.o obj/airattack.msx/midres_sid.o obj/airattack.msx/midres_sound_000.o obj/airattack.msx/midres_sound_001.o obj/airattack.msx/midres_sound_002.o obj/airattack.msx/midres_sound_005.o obj/airattack.msx/midres_sound_006.o obj/airattack.msx/midres_sound_007.o obj/airattack.msx/midres_svi.o obj/airattack.msx/midres_ted.o obj/airattack.msx/midres_tile_000.o obj/airattack.msx/midres_tile_004.o obj/airattack.msx/midres_tile_005.o obj/airattack.msx/midres_tile_006.o obj/airattack.msx/midres_tile_007.o obj/airattack.msx/midres_tile_008.o obj/airattack.msx/midres_tile_009.o obj/airattack.msx/midres_tile_010.o obj/airattack.msx/midres_tile_011.o obj/airattack.msx/midres_tile_012.o obj/airattack.msx/midres_tile_013.o obj/airattack.msx/midres_tile_014.o obj/airattack.msx/midres_tile_015.o obj/airattack.msx/midres_tile_016.o obj/airattack.msx/midres_tile_018.o obj/airattack.msx/midres_tile_019.o obj/airattack.msx/midres_tile_020.o obj/airattack.msx/midres_tile_021.o obj/airattack.msx/midres_tile_022.o obj/airattack.msx/midres_tile_023.o obj/airattack.msx/midres_tile_025.o obj/airattack.msx/midres_tile_026.o obj/airattack.msx/midres_tile_027.o obj/airattack.msx/midres_tile_028.o obj/airattack.msx/midres_tile_multicolor_000.o obj/airattack.msx/midres_tile_multicolor_001.o obj/airattack.msx/midres_tile_multicolor_002.o obj/airattack.msx/midres_tile_multicolor_003.o obj/airattack.msx/midres_tile_multicolor_004.o obj/airattack.msx/midres_tile_multicolor_005.o obj/airattack.msx/midres_tile_multicolor_006.o obj/airattack.msx/midres_tile_multicolor_007.o obj/airattack.msx/midres_tile_multicolor_008.o obj/airattack.msx/midres_tile_multicolor_009.o obj/airattack.msx/midres_tile_multicolor_010.o obj/airattack.msx/midres_tile_multicolor_011.o obj/airattack.msx/midres_tile_multicolor_012.o obj/airattack.msx/midres_tile_processing_001.o obj/airattack.msx/midres_tile_processing_002.o obj/airattack.msx/midres_tile_processing_004.o obj/airattack.msx/midres_tile_processing_005.o obj/airattack.msx/midres_tile_processing_007.o obj/airattack.msx/midres_tile_processing_009.o obj/airattack.msx/midres_tile_processing_011.o obj/airattack.msx/midres_tile_processing_012.o obj/airattack.msx/midres_tile_processing_016.o obj/airattack.msx/midres_vanilla.o obj/airattack.msx/midres_vdp.o obj/airattack.msx/midres_vic.o obj/airattack.msx/midres_vic20.o obj/airattack.msx/utility_joycheck.o obj/airattack.msx/midres_control_011.o obj/airattack.msx/midres_data.o obj/airattack.msx/main.o -create-app 
	$(call COPYFILES,a.rom,$(EXEDIR)/airattack.msx.rom)
	$(call RMFILES,a.*)

# find_main_module(alienstorm)
# -> src/game_alien_storm.c

# -------------------------------------------------------------------
# --- ALIENSTORM FOR MSX 
# -------------------------------------------------------------------
# 

.PHONY: alienstorm.embedded.msx
alienstorm.embedded.msx:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -o obj/alienstorm.msx/rawdata.o src/rawdata.c

obj/alienstorm.msx/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.msx/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/alienstorm.msx/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.msx/midres_io.o src/midres_io.asm

obj/alienstorm.msx/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -D__ALIENSTORM__ -o $@ $(subst obj/alienstorm.msx/,src/,$(@:.o=.c)) 

$(EXEDIR)/alienstorm.msx:	alienstorm.embedded.msx obj/alienstorm.msx/demo_music.o obj/alienstorm.msx/demo_tile.o obj/alienstorm.msx/game_air_attack.o obj/alienstorm.msx/game_alien_storm.o obj/alienstorm.msx/game_elevator.o obj/alienstorm.msx/game_totto.o obj/alienstorm.msx/midres.o obj/alienstorm.msx/midres_atari.o obj/alienstorm.msx/midres_atmos.o obj/alienstorm.msx/midres_ay8910.o obj/alienstorm.msx/midres_c128.o obj/alienstorm.msx/midres_c16.o obj/alienstorm.msx/midres_c64.o obj/alienstorm.msx/midres_coleco.o obj/alienstorm.msx/midres_control_000.o obj/alienstorm.msx/midres_control_001.o obj/alienstorm.msx/midres_control_002.o obj/alienstorm.msx/midres_control_003.o obj/alienstorm.msx/midres_control_004.o obj/alienstorm.msx/midres_control_005.o obj/alienstorm.msx/midres_control_007.o obj/alienstorm.msx/midres_control_010.o obj/alienstorm.msx/midres_file_001.o obj/alienstorm.msx/midres_file_003.o obj/alienstorm.msx/midres_frame_000.o obj/alienstorm.msx/midres_frame_001.o obj/alienstorm.msx/midres_gb.o obj/alienstorm.msx/midres_lm80c.o obj/alienstorm.msx/midres_msx.o obj/alienstorm.msx/midres_mtx500.o obj/alienstorm.msx/midres_music_001.o obj/alienstorm.msx/midres_music_002.o obj/alienstorm.msx/midres_music_004.o obj/alienstorm.msx/midres_music_005.o obj/alienstorm.msx/midres_music_006.o obj/alienstorm.msx/midres_plus4.o obj/alienstorm.msx/midres_pokey.o obj/alienstorm.msx/midres_screen_000.o obj/alienstorm.msx/midres_screen_006.o obj/alienstorm.msx/midres_screen_007.o obj/alienstorm.msx/midres_screen_010.o obj/alienstorm.msx/midres_screen_012.o obj/alienstorm.msx/midres_screen_013.o obj/alienstorm.msx/midres_sid.o obj/alienstorm.msx/midres_sound_000.o obj/alienstorm.msx/midres_sound_001.o obj/alienstorm.msx/midres_sound_002.o obj/alienstorm.msx/midres_sound_005.o obj/alienstorm.msx/midres_sound_006.o obj/alienstorm.msx/midres_sound_007.o obj/alienstorm.msx/midres_svi.o obj/alienstorm.msx/midres_ted.o obj/alienstorm.msx/midres_tile_000.o obj/alienstorm.msx/midres_tile_004.o obj/alienstorm.msx/midres_tile_005.o obj/alienstorm.msx/midres_tile_006.o obj/alienstorm.msx/midres_tile_007.o obj/alienstorm.msx/midres_tile_008.o obj/alienstorm.msx/midres_tile_009.o obj/alienstorm.msx/midres_tile_010.o obj/alienstorm.msx/midres_tile_011.o obj/alienstorm.msx/midres_tile_012.o obj/alienstorm.msx/midres_tile_013.o obj/alienstorm.msx/midres_tile_014.o obj/alienstorm.msx/midres_tile_015.o obj/alienstorm.msx/midres_tile_016.o obj/alienstorm.msx/midres_tile_018.o obj/alienstorm.msx/midres_tile_019.o obj/alienstorm.msx/midres_tile_020.o obj/alienstorm.msx/midres_tile_021.o obj/alienstorm.msx/midres_tile_022.o obj/alienstorm.msx/midres_tile_023.o obj/alienstorm.msx/midres_tile_025.o obj/alienstorm.msx/midres_tile_026.o obj/alienstorm.msx/midres_tile_027.o obj/alienstorm.msx/midres_tile_028.o obj/alienstorm.msx/midres_tile_multicolor_000.o obj/alienstorm.msx/midres_tile_multicolor_001.o obj/alienstorm.msx/midres_tile_multicolor_002.o obj/alienstorm.msx/midres_tile_multicolor_003.o obj/alienstorm.msx/midres_tile_multicolor_004.o obj/alienstorm.msx/midres_tile_multicolor_005.o obj/alienstorm.msx/midres_tile_multicolor_006.o obj/alienstorm.msx/midres_tile_multicolor_007.o obj/alienstorm.msx/midres_tile_multicolor_008.o obj/alienstorm.msx/midres_tile_multicolor_009.o obj/alienstorm.msx/midres_tile_multicolor_010.o obj/alienstorm.msx/midres_tile_multicolor_011.o obj/alienstorm.msx/midres_tile_multicolor_012.o obj/alienstorm.msx/midres_tile_processing_001.o obj/alienstorm.msx/midres_tile_processing_002.o obj/alienstorm.msx/midres_tile_processing_004.o obj/alienstorm.msx/midres_tile_processing_005.o obj/alienstorm.msx/midres_tile_processing_007.o obj/alienstorm.msx/midres_tile_processing_009.o obj/alienstorm.msx/midres_tile_processing_011.o obj/alienstorm.msx/midres_tile_processing_012.o obj/alienstorm.msx/midres_tile_processing_016.o obj/alienstorm.msx/midres_vanilla.o obj/alienstorm.msx/midres_vdp.o obj/alienstorm.msx/midres_vic.o obj/alienstorm.msx/midres_vic20.o obj/alienstorm.msx/utility_joycheck.o obj/alienstorm.msx/midres_control_011.o obj/alienstorm.msx/midres_data.o obj/alienstorm.msx/main.o obj/alienstorm.msx/rawdata.o obj/alienstorm.msx/midres_vdp_impl.o obj/alienstorm.msx/midres_io.o
	$(CC88) +msx -subtype=rom -m $(LDFLAGS88) obj/alienstorm.msx/rawdata.o obj/alienstorm.msx/midres_io.o obj/alienstorm.msx/midres_vdp_impl.o obj/alienstorm.msx/demo_music.o obj/alienstorm.msx/demo_tile.o obj/alienstorm.msx/game_air_attack.o obj/alienstorm.msx/game_alien_storm.o obj/alienstorm.msx/game_elevator.o obj/alienstorm.msx/game_totto.o obj/alienstorm.msx/midres.o obj/alienstorm.msx/midres_atari.o obj/alienstorm.msx/midres_atmos.o obj/alienstorm.msx/midres_ay8910.o obj/alienstorm.msx/midres_c128.o obj/alienstorm.msx/midres_c16.o obj/alienstorm.msx/midres_c64.o obj/alienstorm.msx/midres_coleco.o obj/alienstorm.msx/midres_control_000.o obj/alienstorm.msx/midres_control_001.o obj/alienstorm.msx/midres_control_002.o obj/alienstorm.msx/midres_control_003.o obj/alienstorm.msx/midres_control_004.o obj/alienstorm.msx/midres_control_005.o obj/alienstorm.msx/midres_control_007.o obj/alienstorm.msx/midres_control_010.o obj/alienstorm.msx/midres_file_001.o obj/alienstorm.msx/midres_file_003.o obj/alienstorm.msx/midres_frame_000.o obj/alienstorm.msx/midres_frame_001.o obj/alienstorm.msx/midres_gb.o obj/alienstorm.msx/midres_lm80c.o obj/alienstorm.msx/midres_msx.o obj/alienstorm.msx/midres_mtx500.o obj/alienstorm.msx/midres_music_001.o obj/alienstorm.msx/midres_music_002.o obj/alienstorm.msx/midres_music_004.o obj/alienstorm.msx/midres_music_005.o obj/alienstorm.msx/midres_music_006.o obj/alienstorm.msx/midres_plus4.o obj/alienstorm.msx/midres_pokey.o obj/alienstorm.msx/midres_screen_000.o obj/alienstorm.msx/midres_screen_006.o obj/alienstorm.msx/midres_screen_007.o obj/alienstorm.msx/midres_screen_010.o obj/alienstorm.msx/midres_screen_012.o obj/alienstorm.msx/midres_screen_013.o obj/alienstorm.msx/midres_sid.o obj/alienstorm.msx/midres_sound_000.o obj/alienstorm.msx/midres_sound_001.o obj/alienstorm.msx/midres_sound_002.o obj/alienstorm.msx/midres_sound_005.o obj/alienstorm.msx/midres_sound_006.o obj/alienstorm.msx/midres_sound_007.o obj/alienstorm.msx/midres_svi.o obj/alienstorm.msx/midres_ted.o obj/alienstorm.msx/midres_tile_000.o obj/alienstorm.msx/midres_tile_004.o obj/alienstorm.msx/midres_tile_005.o obj/alienstorm.msx/midres_tile_006.o obj/alienstorm.msx/midres_tile_007.o obj/alienstorm.msx/midres_tile_008.o obj/alienstorm.msx/midres_tile_009.o obj/alienstorm.msx/midres_tile_010.o obj/alienstorm.msx/midres_tile_011.o obj/alienstorm.msx/midres_tile_012.o obj/alienstorm.msx/midres_tile_013.o obj/alienstorm.msx/midres_tile_014.o obj/alienstorm.msx/midres_tile_015.o obj/alienstorm.msx/midres_tile_016.o obj/alienstorm.msx/midres_tile_018.o obj/alienstorm.msx/midres_tile_019.o obj/alienstorm.msx/midres_tile_020.o obj/alienstorm.msx/midres_tile_021.o obj/alienstorm.msx/midres_tile_022.o obj/alienstorm.msx/midres_tile_023.o obj/alienstorm.msx/midres_tile_025.o obj/alienstorm.msx/midres_tile_026.o obj/alienstorm.msx/midres_tile_027.o obj/alienstorm.msx/midres_tile_028.o obj/alienstorm.msx/midres_tile_multicolor_000.o obj/alienstorm.msx/midres_tile_multicolor_001.o obj/alienstorm.msx/midres_tile_multicolor_002.o obj/alienstorm.msx/midres_tile_multicolor_003.o obj/alienstorm.msx/midres_tile_multicolor_004.o obj/alienstorm.msx/midres_tile_multicolor_005.o obj/alienstorm.msx/midres_tile_multicolor_006.o obj/alienstorm.msx/midres_tile_multicolor_007.o obj/alienstorm.msx/midres_tile_multicolor_008.o obj/alienstorm.msx/midres_tile_multicolor_009.o obj/alienstorm.msx/midres_tile_multicolor_010.o obj/alienstorm.msx/midres_tile_multicolor_011.o obj/alienstorm.msx/midres_tile_multicolor_012.o obj/alienstorm.msx/midres_tile_processing_001.o obj/alienstorm.msx/midres_tile_processing_002.o obj/alienstorm.msx/midres_tile_processing_004.o obj/alienstorm.msx/midres_tile_processing_005.o obj/alienstorm.msx/midres_tile_processing_007.o obj/alienstorm.msx/midres_tile_processing_009.o obj/alienstorm.msx/midres_tile_processing_011.o obj/alienstorm.msx/midres_tile_processing_012.o obj/alienstorm.msx/midres_tile_processing_016.o obj/alienstorm.msx/midres_vanilla.o obj/alienstorm.msx/midres_vdp.o obj/alienstorm.msx/midres_vic.o obj/alienstorm.msx/midres_vic20.o obj/alienstorm.msx/utility_joycheck.o obj/alienstorm.msx/midres_control_011.o obj/alienstorm.msx/midres_data.o obj/alienstorm.msx/main.o -create-app 
	$(call COPYFILES,a.rom,$(EXEDIR)/alienstorm.msx.rom)
	$(call RMFILES,a.*)

# find_main_module(elevator)
# -> src/game_elevator.c

# -------------------------------------------------------------------
# --- ELEVATOR FOR MSX 
# -------------------------------------------------------------------
# 

.PHONY: elevator.embedded.msx
elevator.embedded.msx:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevatora.mpic -n zelintro.bin -i $(DATADIR)/elevator1.imf -n elevator1.imf -c src/rawdata.c -h src/rawdata.h
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -o obj/elevator.msx/rawdata.o src/rawdata.c

obj/elevator.msx/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.msx/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/elevator.msx/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.msx/midres_io.o src/midres_io.asm

obj/elevator.msx/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -D__ELEVATOR__ -o $@ $(subst obj/elevator.msx/,src/,$(@:.o=.c)) 

$(EXEDIR)/elevator.msx:	elevator.embedded.msx obj/elevator.msx/demo_music.o obj/elevator.msx/demo_tile.o obj/elevator.msx/game_air_attack.o obj/elevator.msx/game_alien_storm.o obj/elevator.msx/game_elevator.o obj/elevator.msx/game_totto.o obj/elevator.msx/midres.o obj/elevator.msx/midres_atari.o obj/elevator.msx/midres_atmos.o obj/elevator.msx/midres_ay8910.o obj/elevator.msx/midres_c128.o obj/elevator.msx/midres_c16.o obj/elevator.msx/midres_c64.o obj/elevator.msx/midres_coleco.o obj/elevator.msx/midres_control_000.o obj/elevator.msx/midres_control_001.o obj/elevator.msx/midres_control_002.o obj/elevator.msx/midres_control_003.o obj/elevator.msx/midres_control_004.o obj/elevator.msx/midres_control_005.o obj/elevator.msx/midres_control_007.o obj/elevator.msx/midres_control_010.o obj/elevator.msx/midres_file_001.o obj/elevator.msx/midres_file_003.o obj/elevator.msx/midres_frame_000.o obj/elevator.msx/midres_frame_001.o obj/elevator.msx/midres_gb.o obj/elevator.msx/midres_lm80c.o obj/elevator.msx/midres_msx.o obj/elevator.msx/midres_mtx500.o obj/elevator.msx/midres_music_001.o obj/elevator.msx/midres_music_002.o obj/elevator.msx/midres_music_004.o obj/elevator.msx/midres_music_005.o obj/elevator.msx/midres_music_006.o obj/elevator.msx/midres_plus4.o obj/elevator.msx/midres_pokey.o obj/elevator.msx/midres_screen_000.o obj/elevator.msx/midres_screen_006.o obj/elevator.msx/midres_screen_007.o obj/elevator.msx/midres_screen_010.o obj/elevator.msx/midres_screen_012.o obj/elevator.msx/midres_screen_013.o obj/elevator.msx/midres_sid.o obj/elevator.msx/midres_sound_000.o obj/elevator.msx/midres_sound_001.o obj/elevator.msx/midres_sound_002.o obj/elevator.msx/midres_sound_005.o obj/elevator.msx/midres_sound_006.o obj/elevator.msx/midres_sound_007.o obj/elevator.msx/midres_svi.o obj/elevator.msx/midres_ted.o obj/elevator.msx/midres_tile_000.o obj/elevator.msx/midres_tile_004.o obj/elevator.msx/midres_tile_005.o obj/elevator.msx/midres_tile_006.o obj/elevator.msx/midres_tile_007.o obj/elevator.msx/midres_tile_008.o obj/elevator.msx/midres_tile_009.o obj/elevator.msx/midres_tile_010.o obj/elevator.msx/midres_tile_011.o obj/elevator.msx/midres_tile_012.o obj/elevator.msx/midres_tile_013.o obj/elevator.msx/midres_tile_014.o obj/elevator.msx/midres_tile_015.o obj/elevator.msx/midres_tile_016.o obj/elevator.msx/midres_tile_018.o obj/elevator.msx/midres_tile_019.o obj/elevator.msx/midres_tile_020.o obj/elevator.msx/midres_tile_021.o obj/elevator.msx/midres_tile_022.o obj/elevator.msx/midres_tile_023.o obj/elevator.msx/midres_tile_025.o obj/elevator.msx/midres_tile_026.o obj/elevator.msx/midres_tile_027.o obj/elevator.msx/midres_tile_028.o obj/elevator.msx/midres_tile_multicolor_000.o obj/elevator.msx/midres_tile_multicolor_001.o obj/elevator.msx/midres_tile_multicolor_002.o obj/elevator.msx/midres_tile_multicolor_003.o obj/elevator.msx/midres_tile_multicolor_004.o obj/elevator.msx/midres_tile_multicolor_005.o obj/elevator.msx/midres_tile_multicolor_006.o obj/elevator.msx/midres_tile_multicolor_007.o obj/elevator.msx/midres_tile_multicolor_008.o obj/elevator.msx/midres_tile_multicolor_009.o obj/elevator.msx/midres_tile_multicolor_010.o obj/elevator.msx/midres_tile_multicolor_011.o obj/elevator.msx/midres_tile_multicolor_012.o obj/elevator.msx/midres_tile_processing_001.o obj/elevator.msx/midres_tile_processing_002.o obj/elevator.msx/midres_tile_processing_004.o obj/elevator.msx/midres_tile_processing_005.o obj/elevator.msx/midres_tile_processing_007.o obj/elevator.msx/midres_tile_processing_009.o obj/elevator.msx/midres_tile_processing_011.o obj/elevator.msx/midres_tile_processing_012.o obj/elevator.msx/midres_tile_processing_016.o obj/elevator.msx/midres_vanilla.o obj/elevator.msx/midres_vdp.o obj/elevator.msx/midres_vic.o obj/elevator.msx/midres_vic20.o obj/elevator.msx/utility_joycheck.o obj/elevator.msx/midres_control_011.o obj/elevator.msx/midres_data.o obj/elevator.msx/main.o obj/elevator.msx/rawdata.o obj/elevator.msx/midres_vdp_impl.o obj/elevator.msx/midres_io.o
	$(CC88) +msx -subtype=rom -m $(LDFLAGS88) obj/elevator.msx/rawdata.o obj/elevator.msx/midres_io.o obj/elevator.msx/midres_vdp_impl.o obj/elevator.msx/demo_music.o obj/elevator.msx/demo_tile.o obj/elevator.msx/game_air_attack.o obj/elevator.msx/game_alien_storm.o obj/elevator.msx/game_elevator.o obj/elevator.msx/game_totto.o obj/elevator.msx/midres.o obj/elevator.msx/midres_atari.o obj/elevator.msx/midres_atmos.o obj/elevator.msx/midres_ay8910.o obj/elevator.msx/midres_c128.o obj/elevator.msx/midres_c16.o obj/elevator.msx/midres_c64.o obj/elevator.msx/midres_coleco.o obj/elevator.msx/midres_control_000.o obj/elevator.msx/midres_control_001.o obj/elevator.msx/midres_control_002.o obj/elevator.msx/midres_control_003.o obj/elevator.msx/midres_control_004.o obj/elevator.msx/midres_control_005.o obj/elevator.msx/midres_control_007.o obj/elevator.msx/midres_control_010.o obj/elevator.msx/midres_file_001.o obj/elevator.msx/midres_file_003.o obj/elevator.msx/midres_frame_000.o obj/elevator.msx/midres_frame_001.o obj/elevator.msx/midres_gb.o obj/elevator.msx/midres_lm80c.o obj/elevator.msx/midres_msx.o obj/elevator.msx/midres_mtx500.o obj/elevator.msx/midres_music_001.o obj/elevator.msx/midres_music_002.o obj/elevator.msx/midres_music_004.o obj/elevator.msx/midres_music_005.o obj/elevator.msx/midres_music_006.o obj/elevator.msx/midres_plus4.o obj/elevator.msx/midres_pokey.o obj/elevator.msx/midres_screen_000.o obj/elevator.msx/midres_screen_006.o obj/elevator.msx/midres_screen_007.o obj/elevator.msx/midres_screen_010.o obj/elevator.msx/midres_screen_012.o obj/elevator.msx/midres_screen_013.o obj/elevator.msx/midres_sid.o obj/elevator.msx/midres_sound_000.o obj/elevator.msx/midres_sound_001.o obj/elevator.msx/midres_sound_002.o obj/elevator.msx/midres_sound_005.o obj/elevator.msx/midres_sound_006.o obj/elevator.msx/midres_sound_007.o obj/elevator.msx/midres_svi.o obj/elevator.msx/midres_ted.o obj/elevator.msx/midres_tile_000.o obj/elevator.msx/midres_tile_004.o obj/elevator.msx/midres_tile_005.o obj/elevator.msx/midres_tile_006.o obj/elevator.msx/midres_tile_007.o obj/elevator.msx/midres_tile_008.o obj/elevator.msx/midres_tile_009.o obj/elevator.msx/midres_tile_010.o obj/elevator.msx/midres_tile_011.o obj/elevator.msx/midres_tile_012.o obj/elevator.msx/midres_tile_013.o obj/elevator.msx/midres_tile_014.o obj/elevator.msx/midres_tile_015.o obj/elevator.msx/midres_tile_016.o obj/elevator.msx/midres_tile_018.o obj/elevator.msx/midres_tile_019.o obj/elevator.msx/midres_tile_020.o obj/elevator.msx/midres_tile_021.o obj/elevator.msx/midres_tile_022.o obj/elevator.msx/midres_tile_023.o obj/elevator.msx/midres_tile_025.o obj/elevator.msx/midres_tile_026.o obj/elevator.msx/midres_tile_027.o obj/elevator.msx/midres_tile_028.o obj/elevator.msx/midres_tile_multicolor_000.o obj/elevator.msx/midres_tile_multicolor_001.o obj/elevator.msx/midres_tile_multicolor_002.o obj/elevator.msx/midres_tile_multicolor_003.o obj/elevator.msx/midres_tile_multicolor_004.o obj/elevator.msx/midres_tile_multicolor_005.o obj/elevator.msx/midres_tile_multicolor_006.o obj/elevator.msx/midres_tile_multicolor_007.o obj/elevator.msx/midres_tile_multicolor_008.o obj/elevator.msx/midres_tile_multicolor_009.o obj/elevator.msx/midres_tile_multicolor_010.o obj/elevator.msx/midres_tile_multicolor_011.o obj/elevator.msx/midres_tile_multicolor_012.o obj/elevator.msx/midres_tile_processing_001.o obj/elevator.msx/midres_tile_processing_002.o obj/elevator.msx/midres_tile_processing_004.o obj/elevator.msx/midres_tile_processing_005.o obj/elevator.msx/midres_tile_processing_007.o obj/elevator.msx/midres_tile_processing_009.o obj/elevator.msx/midres_tile_processing_011.o obj/elevator.msx/midres_tile_processing_012.o obj/elevator.msx/midres_tile_processing_016.o obj/elevator.msx/midres_vanilla.o obj/elevator.msx/midres_vdp.o obj/elevator.msx/midres_vic.o obj/elevator.msx/midres_vic20.o obj/elevator.msx/utility_joycheck.o obj/elevator.msx/midres_control_011.o obj/elevator.msx/midres_data.o obj/elevator.msx/main.o -create-app 
	$(call COPYFILES,a.rom,$(EXEDIR)/elevator.msx.rom)
	$(call RMFILES,a.*)

                    
#!!! missing resources for joycheck (msx)

# find_main_module(totto)
# -> src/game_totto.c

# -------------------------------------------------------------------
# --- TOTTO FOR MSX 
# -------------------------------------------------------------------
# 

.PHONY: totto.embedded.msx
totto.embedded.msx:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -i $(DATADIR)/totto.imf -n totto.imf -i $(DATADIR)/totto3.imf -n totto3.imf -c src/rawdata.c -h src/rawdata.h
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -o obj/totto.msx/rawdata.o src/rawdata.c

obj/totto.msx/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.msx/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/totto.msx/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.msx/midres_io.o src/midres_io.asm

obj/totto.msx/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -D__TOTTO__ -o $@ $(subst obj/totto.msx/,src/,$(@:.o=.c)) 

$(EXEDIR)/totto.msx:	totto.embedded.msx obj/totto.msx/demo_music.o obj/totto.msx/demo_tile.o obj/totto.msx/game_air_attack.o obj/totto.msx/game_alien_storm.o obj/totto.msx/game_elevator.o obj/totto.msx/game_totto.o obj/totto.msx/midres.o obj/totto.msx/midres_atari.o obj/totto.msx/midres_atmos.o obj/totto.msx/midres_ay8910.o obj/totto.msx/midres_c128.o obj/totto.msx/midres_c16.o obj/totto.msx/midres_c64.o obj/totto.msx/midres_coleco.o obj/totto.msx/midres_control_000.o obj/totto.msx/midres_control_001.o obj/totto.msx/midres_control_002.o obj/totto.msx/midres_control_003.o obj/totto.msx/midres_control_004.o obj/totto.msx/midres_control_005.o obj/totto.msx/midres_control_007.o obj/totto.msx/midres_control_010.o obj/totto.msx/midres_file_001.o obj/totto.msx/midres_file_003.o obj/totto.msx/midres_frame_000.o obj/totto.msx/midres_frame_001.o obj/totto.msx/midres_gb.o obj/totto.msx/midres_lm80c.o obj/totto.msx/midres_msx.o obj/totto.msx/midres_mtx500.o obj/totto.msx/midres_music_001.o obj/totto.msx/midres_music_002.o obj/totto.msx/midres_music_004.o obj/totto.msx/midres_music_005.o obj/totto.msx/midres_music_006.o obj/totto.msx/midres_plus4.o obj/totto.msx/midres_pokey.o obj/totto.msx/midres_screen_000.o obj/totto.msx/midres_screen_006.o obj/totto.msx/midres_screen_007.o obj/totto.msx/midres_screen_010.o obj/totto.msx/midres_screen_012.o obj/totto.msx/midres_screen_013.o obj/totto.msx/midres_sid.o obj/totto.msx/midres_sound_000.o obj/totto.msx/midres_sound_001.o obj/totto.msx/midres_sound_002.o obj/totto.msx/midres_sound_005.o obj/totto.msx/midres_sound_006.o obj/totto.msx/midres_sound_007.o obj/totto.msx/midres_svi.o obj/totto.msx/midres_ted.o obj/totto.msx/midres_tile_000.o obj/totto.msx/midres_tile_004.o obj/totto.msx/midres_tile_005.o obj/totto.msx/midres_tile_006.o obj/totto.msx/midres_tile_007.o obj/totto.msx/midres_tile_008.o obj/totto.msx/midres_tile_009.o obj/totto.msx/midres_tile_010.o obj/totto.msx/midres_tile_011.o obj/totto.msx/midres_tile_012.o obj/totto.msx/midres_tile_013.o obj/totto.msx/midres_tile_014.o obj/totto.msx/midres_tile_015.o obj/totto.msx/midres_tile_016.o obj/totto.msx/midres_tile_018.o obj/totto.msx/midres_tile_019.o obj/totto.msx/midres_tile_020.o obj/totto.msx/midres_tile_021.o obj/totto.msx/midres_tile_022.o obj/totto.msx/midres_tile_023.o obj/totto.msx/midres_tile_025.o obj/totto.msx/midres_tile_026.o obj/totto.msx/midres_tile_027.o obj/totto.msx/midres_tile_028.o obj/totto.msx/midres_tile_multicolor_000.o obj/totto.msx/midres_tile_multicolor_001.o obj/totto.msx/midres_tile_multicolor_002.o obj/totto.msx/midres_tile_multicolor_003.o obj/totto.msx/midres_tile_multicolor_004.o obj/totto.msx/midres_tile_multicolor_005.o obj/totto.msx/midres_tile_multicolor_006.o obj/totto.msx/midres_tile_multicolor_007.o obj/totto.msx/midres_tile_multicolor_008.o obj/totto.msx/midres_tile_multicolor_009.o obj/totto.msx/midres_tile_multicolor_010.o obj/totto.msx/midres_tile_multicolor_011.o obj/totto.msx/midres_tile_multicolor_012.o obj/totto.msx/midres_tile_processing_001.o obj/totto.msx/midres_tile_processing_002.o obj/totto.msx/midres_tile_processing_004.o obj/totto.msx/midres_tile_processing_005.o obj/totto.msx/midres_tile_processing_007.o obj/totto.msx/midres_tile_processing_009.o obj/totto.msx/midres_tile_processing_011.o obj/totto.msx/midres_tile_processing_012.o obj/totto.msx/midres_tile_processing_016.o obj/totto.msx/midres_vanilla.o obj/totto.msx/midres_vdp.o obj/totto.msx/midres_vic.o obj/totto.msx/midres_vic20.o obj/totto.msx/utility_joycheck.o obj/totto.msx/midres_control_011.o obj/totto.msx/midres_data.o obj/totto.msx/main.o obj/totto.msx/rawdata.o obj/totto.msx/midres_vdp_impl.o obj/totto.msx/midres_io.o
	$(CC88) +msx -subtype=rom -m $(LDFLAGS88) obj/totto.msx/rawdata.o obj/totto.msx/midres_io.o obj/totto.msx/midres_vdp_impl.o obj/totto.msx/demo_music.o obj/totto.msx/demo_tile.o obj/totto.msx/game_air_attack.o obj/totto.msx/game_alien_storm.o obj/totto.msx/game_elevator.o obj/totto.msx/game_totto.o obj/totto.msx/midres.o obj/totto.msx/midres_atari.o obj/totto.msx/midres_atmos.o obj/totto.msx/midres_ay8910.o obj/totto.msx/midres_c128.o obj/totto.msx/midres_c16.o obj/totto.msx/midres_c64.o obj/totto.msx/midres_coleco.o obj/totto.msx/midres_control_000.o obj/totto.msx/midres_control_001.o obj/totto.msx/midres_control_002.o obj/totto.msx/midres_control_003.o obj/totto.msx/midres_control_004.o obj/totto.msx/midres_control_005.o obj/totto.msx/midres_control_007.o obj/totto.msx/midres_control_010.o obj/totto.msx/midres_file_001.o obj/totto.msx/midres_file_003.o obj/totto.msx/midres_frame_000.o obj/totto.msx/midres_frame_001.o obj/totto.msx/midres_gb.o obj/totto.msx/midres_lm80c.o obj/totto.msx/midres_msx.o obj/totto.msx/midres_mtx500.o obj/totto.msx/midres_music_001.o obj/totto.msx/midres_music_002.o obj/totto.msx/midres_music_004.o obj/totto.msx/midres_music_005.o obj/totto.msx/midres_music_006.o obj/totto.msx/midres_plus4.o obj/totto.msx/midres_pokey.o obj/totto.msx/midres_screen_000.o obj/totto.msx/midres_screen_006.o obj/totto.msx/midres_screen_007.o obj/totto.msx/midres_screen_010.o obj/totto.msx/midres_screen_012.o obj/totto.msx/midres_screen_013.o obj/totto.msx/midres_sid.o obj/totto.msx/midres_sound_000.o obj/totto.msx/midres_sound_001.o obj/totto.msx/midres_sound_002.o obj/totto.msx/midres_sound_005.o obj/totto.msx/midres_sound_006.o obj/totto.msx/midres_sound_007.o obj/totto.msx/midres_svi.o obj/totto.msx/midres_ted.o obj/totto.msx/midres_tile_000.o obj/totto.msx/midres_tile_004.o obj/totto.msx/midres_tile_005.o obj/totto.msx/midres_tile_006.o obj/totto.msx/midres_tile_007.o obj/totto.msx/midres_tile_008.o obj/totto.msx/midres_tile_009.o obj/totto.msx/midres_tile_010.o obj/totto.msx/midres_tile_011.o obj/totto.msx/midres_tile_012.o obj/totto.msx/midres_tile_013.o obj/totto.msx/midres_tile_014.o obj/totto.msx/midres_tile_015.o obj/totto.msx/midres_tile_016.o obj/totto.msx/midres_tile_018.o obj/totto.msx/midres_tile_019.o obj/totto.msx/midres_tile_020.o obj/totto.msx/midres_tile_021.o obj/totto.msx/midres_tile_022.o obj/totto.msx/midres_tile_023.o obj/totto.msx/midres_tile_025.o obj/totto.msx/midres_tile_026.o obj/totto.msx/midres_tile_027.o obj/totto.msx/midres_tile_028.o obj/totto.msx/midres_tile_multicolor_000.o obj/totto.msx/midres_tile_multicolor_001.o obj/totto.msx/midres_tile_multicolor_002.o obj/totto.msx/midres_tile_multicolor_003.o obj/totto.msx/midres_tile_multicolor_004.o obj/totto.msx/midres_tile_multicolor_005.o obj/totto.msx/midres_tile_multicolor_006.o obj/totto.msx/midres_tile_multicolor_007.o obj/totto.msx/midres_tile_multicolor_008.o obj/totto.msx/midres_tile_multicolor_009.o obj/totto.msx/midres_tile_multicolor_010.o obj/totto.msx/midres_tile_multicolor_011.o obj/totto.msx/midres_tile_multicolor_012.o obj/totto.msx/midres_tile_processing_001.o obj/totto.msx/midres_tile_processing_002.o obj/totto.msx/midres_tile_processing_004.o obj/totto.msx/midres_tile_processing_005.o obj/totto.msx/midres_tile_processing_007.o obj/totto.msx/midres_tile_processing_009.o obj/totto.msx/midres_tile_processing_011.o obj/totto.msx/midres_tile_processing_012.o obj/totto.msx/midres_tile_processing_016.o obj/totto.msx/midres_vanilla.o obj/totto.msx/midres_vdp.o obj/totto.msx/midres_vic.o obj/totto.msx/midres_vic20.o obj/totto.msx/utility_joycheck.o obj/totto.msx/midres_control_011.o obj/totto.msx/midres_data.o obj/totto.msx/main.o -create-app 
	$(call COPYFILES,a.rom,$(EXEDIR)/totto.msx.rom)
	$(call RMFILES,a.*)

# find_main_module(airattack)
# -> src/game_air_attack.c

# -------------------------------------------------------------------
# --- AIRATTACK FOR MTX500 
# -------------------------------------------------------------------
# 

.PHONY: airattack.embedded.mtx500
airattack.embedded.mtx500:
	$(FILE2INCLUDE) -i $(DATADIR)/ztiles.bin -n ztiles.bin -i $(DATADIR)/aatiles4.bin -n zztiles.bin -i $(DATADIR)/aaintrox.pic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -o obj/airattack.mtx500/rawdata.o src/rawdata.c

obj/airattack.mtx500/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.mtx500/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/airattack.mtx500/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.mtx500/midres_io.o src/midres_io.asm

obj/airattack.mtx500/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -D__AIRATTACK__ -o $@ $(subst obj/airattack.mtx500/,src/,$(@:.o=.c)) 

$(EXEDIR)/airattack.mtx500:	airattack.embedded.mtx500 obj/airattack.mtx500/demo_music.o obj/airattack.mtx500/demo_tile.o obj/airattack.mtx500/game_air_attack.o obj/airattack.mtx500/game_alien_storm.o obj/airattack.mtx500/game_elevator.o obj/airattack.mtx500/game_totto.o obj/airattack.mtx500/midres.o obj/airattack.mtx500/midres_atari.o obj/airattack.mtx500/midres_atmos.o obj/airattack.mtx500/midres_ay8910.o obj/airattack.mtx500/midres_c128.o obj/airattack.mtx500/midres_c16.o obj/airattack.mtx500/midres_c64.o obj/airattack.mtx500/midres_coleco.o obj/airattack.mtx500/midres_control_000.o obj/airattack.mtx500/midres_control_001.o obj/airattack.mtx500/midres_control_002.o obj/airattack.mtx500/midres_control_003.o obj/airattack.mtx500/midres_control_004.o obj/airattack.mtx500/midres_control_005.o obj/airattack.mtx500/midres_control_007.o obj/airattack.mtx500/midres_control_010.o obj/airattack.mtx500/midres_file_001.o obj/airattack.mtx500/midres_file_003.o obj/airattack.mtx500/midres_frame_000.o obj/airattack.mtx500/midres_frame_001.o obj/airattack.mtx500/midres_gb.o obj/airattack.mtx500/midres_lm80c.o obj/airattack.mtx500/midres_msx.o obj/airattack.mtx500/midres_mtx500.o obj/airattack.mtx500/midres_music_001.o obj/airattack.mtx500/midres_music_002.o obj/airattack.mtx500/midres_music_004.o obj/airattack.mtx500/midres_music_005.o obj/airattack.mtx500/midres_music_006.o obj/airattack.mtx500/midres_plus4.o obj/airattack.mtx500/midres_pokey.o obj/airattack.mtx500/midres_screen_000.o obj/airattack.mtx500/midres_screen_006.o obj/airattack.mtx500/midres_screen_007.o obj/airattack.mtx500/midres_screen_010.o obj/airattack.mtx500/midres_screen_012.o obj/airattack.mtx500/midres_screen_013.o obj/airattack.mtx500/midres_sid.o obj/airattack.mtx500/midres_sound_000.o obj/airattack.mtx500/midres_sound_001.o obj/airattack.mtx500/midres_sound_002.o obj/airattack.mtx500/midres_sound_005.o obj/airattack.mtx500/midres_sound_006.o obj/airattack.mtx500/midres_sound_007.o obj/airattack.mtx500/midres_svi.o obj/airattack.mtx500/midres_ted.o obj/airattack.mtx500/midres_tile_000.o obj/airattack.mtx500/midres_tile_004.o obj/airattack.mtx500/midres_tile_005.o obj/airattack.mtx500/midres_tile_006.o obj/airattack.mtx500/midres_tile_007.o obj/airattack.mtx500/midres_tile_008.o obj/airattack.mtx500/midres_tile_009.o obj/airattack.mtx500/midres_tile_010.o obj/airattack.mtx500/midres_tile_011.o obj/airattack.mtx500/midres_tile_012.o obj/airattack.mtx500/midres_tile_013.o obj/airattack.mtx500/midres_tile_014.o obj/airattack.mtx500/midres_tile_015.o obj/airattack.mtx500/midres_tile_016.o obj/airattack.mtx500/midres_tile_018.o obj/airattack.mtx500/midres_tile_019.o obj/airattack.mtx500/midres_tile_020.o obj/airattack.mtx500/midres_tile_021.o obj/airattack.mtx500/midres_tile_022.o obj/airattack.mtx500/midres_tile_023.o obj/airattack.mtx500/midres_tile_025.o obj/airattack.mtx500/midres_tile_026.o obj/airattack.mtx500/midres_tile_027.o obj/airattack.mtx500/midres_tile_028.o obj/airattack.mtx500/midres_tile_multicolor_000.o obj/airattack.mtx500/midres_tile_multicolor_001.o obj/airattack.mtx500/midres_tile_multicolor_002.o obj/airattack.mtx500/midres_tile_multicolor_003.o obj/airattack.mtx500/midres_tile_multicolor_004.o obj/airattack.mtx500/midres_tile_multicolor_005.o obj/airattack.mtx500/midres_tile_multicolor_006.o obj/airattack.mtx500/midres_tile_multicolor_007.o obj/airattack.mtx500/midres_tile_multicolor_008.o obj/airattack.mtx500/midres_tile_multicolor_009.o obj/airattack.mtx500/midres_tile_multicolor_010.o obj/airattack.mtx500/midres_tile_multicolor_011.o obj/airattack.mtx500/midres_tile_multicolor_012.o obj/airattack.mtx500/midres_tile_processing_001.o obj/airattack.mtx500/midres_tile_processing_002.o obj/airattack.mtx500/midres_tile_processing_004.o obj/airattack.mtx500/midres_tile_processing_005.o obj/airattack.mtx500/midres_tile_processing_007.o obj/airattack.mtx500/midres_tile_processing_009.o obj/airattack.mtx500/midres_tile_processing_011.o obj/airattack.mtx500/midres_tile_processing_012.o obj/airattack.mtx500/midres_tile_processing_016.o obj/airattack.mtx500/midres_vanilla.o obj/airattack.mtx500/midres_vdp.o obj/airattack.mtx500/midres_vic.o obj/airattack.mtx500/midres_vic20.o obj/airattack.mtx500/utility_joycheck.o obj/airattack.mtx500/midres_control_011.o obj/airattack.mtx500/midres_data.o obj/airattack.mtx500/main.o obj/airattack.mtx500/rawdata.o obj/airattack.mtx500/midres_vdp_impl.o obj/airattack.mtx500/midres_io.o
	$(CC88) +mtx  -m $(LDFLAGS88) obj/airattack.mtx500/rawdata.o obj/airattack.mtx500/midres_io.o obj/airattack.mtx500/midres_vdp_impl.o obj/airattack.mtx500/demo_music.o obj/airattack.mtx500/demo_tile.o obj/airattack.mtx500/game_air_attack.o obj/airattack.mtx500/game_alien_storm.o obj/airattack.mtx500/game_elevator.o obj/airattack.mtx500/game_totto.o obj/airattack.mtx500/midres.o obj/airattack.mtx500/midres_atari.o obj/airattack.mtx500/midres_atmos.o obj/airattack.mtx500/midres_ay8910.o obj/airattack.mtx500/midres_c128.o obj/airattack.mtx500/midres_c16.o obj/airattack.mtx500/midres_c64.o obj/airattack.mtx500/midres_coleco.o obj/airattack.mtx500/midres_control_000.o obj/airattack.mtx500/midres_control_001.o obj/airattack.mtx500/midres_control_002.o obj/airattack.mtx500/midres_control_003.o obj/airattack.mtx500/midres_control_004.o obj/airattack.mtx500/midres_control_005.o obj/airattack.mtx500/midres_control_007.o obj/airattack.mtx500/midres_control_010.o obj/airattack.mtx500/midres_file_001.o obj/airattack.mtx500/midres_file_003.o obj/airattack.mtx500/midres_frame_000.o obj/airattack.mtx500/midres_frame_001.o obj/airattack.mtx500/midres_gb.o obj/airattack.mtx500/midres_lm80c.o obj/airattack.mtx500/midres_msx.o obj/airattack.mtx500/midres_mtx500.o obj/airattack.mtx500/midres_music_001.o obj/airattack.mtx500/midres_music_002.o obj/airattack.mtx500/midres_music_004.o obj/airattack.mtx500/midres_music_005.o obj/airattack.mtx500/midres_music_006.o obj/airattack.mtx500/midres_plus4.o obj/airattack.mtx500/midres_pokey.o obj/airattack.mtx500/midres_screen_000.o obj/airattack.mtx500/midres_screen_006.o obj/airattack.mtx500/midres_screen_007.o obj/airattack.mtx500/midres_screen_010.o obj/airattack.mtx500/midres_screen_012.o obj/airattack.mtx500/midres_screen_013.o obj/airattack.mtx500/midres_sid.o obj/airattack.mtx500/midres_sound_000.o obj/airattack.mtx500/midres_sound_001.o obj/airattack.mtx500/midres_sound_002.o obj/airattack.mtx500/midres_sound_005.o obj/airattack.mtx500/midres_sound_006.o obj/airattack.mtx500/midres_sound_007.o obj/airattack.mtx500/midres_svi.o obj/airattack.mtx500/midres_ted.o obj/airattack.mtx500/midres_tile_000.o obj/airattack.mtx500/midres_tile_004.o obj/airattack.mtx500/midres_tile_005.o obj/airattack.mtx500/midres_tile_006.o obj/airattack.mtx500/midres_tile_007.o obj/airattack.mtx500/midres_tile_008.o obj/airattack.mtx500/midres_tile_009.o obj/airattack.mtx500/midres_tile_010.o obj/airattack.mtx500/midres_tile_011.o obj/airattack.mtx500/midres_tile_012.o obj/airattack.mtx500/midres_tile_013.o obj/airattack.mtx500/midres_tile_014.o obj/airattack.mtx500/midres_tile_015.o obj/airattack.mtx500/midres_tile_016.o obj/airattack.mtx500/midres_tile_018.o obj/airattack.mtx500/midres_tile_019.o obj/airattack.mtx500/midres_tile_020.o obj/airattack.mtx500/midres_tile_021.o obj/airattack.mtx500/midres_tile_022.o obj/airattack.mtx500/midres_tile_023.o obj/airattack.mtx500/midres_tile_025.o obj/airattack.mtx500/midres_tile_026.o obj/airattack.mtx500/midres_tile_027.o obj/airattack.mtx500/midres_tile_028.o obj/airattack.mtx500/midres_tile_multicolor_000.o obj/airattack.mtx500/midres_tile_multicolor_001.o obj/airattack.mtx500/midres_tile_multicolor_002.o obj/airattack.mtx500/midres_tile_multicolor_003.o obj/airattack.mtx500/midres_tile_multicolor_004.o obj/airattack.mtx500/midres_tile_multicolor_005.o obj/airattack.mtx500/midres_tile_multicolor_006.o obj/airattack.mtx500/midres_tile_multicolor_007.o obj/airattack.mtx500/midres_tile_multicolor_008.o obj/airattack.mtx500/midres_tile_multicolor_009.o obj/airattack.mtx500/midres_tile_multicolor_010.o obj/airattack.mtx500/midres_tile_multicolor_011.o obj/airattack.mtx500/midres_tile_multicolor_012.o obj/airattack.mtx500/midres_tile_processing_001.o obj/airattack.mtx500/midres_tile_processing_002.o obj/airattack.mtx500/midres_tile_processing_004.o obj/airattack.mtx500/midres_tile_processing_005.o obj/airattack.mtx500/midres_tile_processing_007.o obj/airattack.mtx500/midres_tile_processing_009.o obj/airattack.mtx500/midres_tile_processing_011.o obj/airattack.mtx500/midres_tile_processing_012.o obj/airattack.mtx500/midres_tile_processing_016.o obj/airattack.mtx500/midres_vanilla.o obj/airattack.mtx500/midres_vdp.o obj/airattack.mtx500/midres_vic.o obj/airattack.mtx500/midres_vic20.o obj/airattack.mtx500/utility_joycheck.o obj/airattack.mtx500/midres_control_011.o obj/airattack.mtx500/midres_data.o obj/airattack.mtx500/main.o -create-app 
	$(call COPYFILES,a.wav,$(EXEDIR)/airattack.mtx500.wav)
	$(call COPYFILES,a.mtx,$(EXEDIR)/airattack.mtx500.mtx)
	$(call RMFILES,a.*)

# find_main_module(alienstorm)
# -> src/game_alien_storm.c

# -------------------------------------------------------------------
# --- ALIENSTORM FOR MTX500 
# -------------------------------------------------------------------
# 

.PHONY: alienstorm.embedded.mtx500
alienstorm.embedded.mtx500:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -o obj/alienstorm.mtx500/rawdata.o src/rawdata.c

obj/alienstorm.mtx500/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.mtx500/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/alienstorm.mtx500/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.mtx500/midres_io.o src/midres_io.asm

obj/alienstorm.mtx500/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -D__ALIENSTORM__ -o $@ $(subst obj/alienstorm.mtx500/,src/,$(@:.o=.c)) 

$(EXEDIR)/alienstorm.mtx500:	alienstorm.embedded.mtx500 obj/alienstorm.mtx500/demo_music.o obj/alienstorm.mtx500/demo_tile.o obj/alienstorm.mtx500/game_air_attack.o obj/alienstorm.mtx500/game_alien_storm.o obj/alienstorm.mtx500/game_elevator.o obj/alienstorm.mtx500/game_totto.o obj/alienstorm.mtx500/midres.o obj/alienstorm.mtx500/midres_atari.o obj/alienstorm.mtx500/midres_atmos.o obj/alienstorm.mtx500/midres_ay8910.o obj/alienstorm.mtx500/midres_c128.o obj/alienstorm.mtx500/midres_c16.o obj/alienstorm.mtx500/midres_c64.o obj/alienstorm.mtx500/midres_coleco.o obj/alienstorm.mtx500/midres_control_000.o obj/alienstorm.mtx500/midres_control_001.o obj/alienstorm.mtx500/midres_control_002.o obj/alienstorm.mtx500/midres_control_003.o obj/alienstorm.mtx500/midres_control_004.o obj/alienstorm.mtx500/midres_control_005.o obj/alienstorm.mtx500/midres_control_007.o obj/alienstorm.mtx500/midres_control_010.o obj/alienstorm.mtx500/midres_file_001.o obj/alienstorm.mtx500/midres_file_003.o obj/alienstorm.mtx500/midres_frame_000.o obj/alienstorm.mtx500/midres_frame_001.o obj/alienstorm.mtx500/midres_gb.o obj/alienstorm.mtx500/midres_lm80c.o obj/alienstorm.mtx500/midres_msx.o obj/alienstorm.mtx500/midres_mtx500.o obj/alienstorm.mtx500/midres_music_001.o obj/alienstorm.mtx500/midres_music_002.o obj/alienstorm.mtx500/midres_music_004.o obj/alienstorm.mtx500/midres_music_005.o obj/alienstorm.mtx500/midres_music_006.o obj/alienstorm.mtx500/midres_plus4.o obj/alienstorm.mtx500/midres_pokey.o obj/alienstorm.mtx500/midres_screen_000.o obj/alienstorm.mtx500/midres_screen_006.o obj/alienstorm.mtx500/midres_screen_007.o obj/alienstorm.mtx500/midres_screen_010.o obj/alienstorm.mtx500/midres_screen_012.o obj/alienstorm.mtx500/midres_screen_013.o obj/alienstorm.mtx500/midres_sid.o obj/alienstorm.mtx500/midres_sound_000.o obj/alienstorm.mtx500/midres_sound_001.o obj/alienstorm.mtx500/midres_sound_002.o obj/alienstorm.mtx500/midres_sound_005.o obj/alienstorm.mtx500/midres_sound_006.o obj/alienstorm.mtx500/midres_sound_007.o obj/alienstorm.mtx500/midres_svi.o obj/alienstorm.mtx500/midres_ted.o obj/alienstorm.mtx500/midres_tile_000.o obj/alienstorm.mtx500/midres_tile_004.o obj/alienstorm.mtx500/midres_tile_005.o obj/alienstorm.mtx500/midres_tile_006.o obj/alienstorm.mtx500/midres_tile_007.o obj/alienstorm.mtx500/midres_tile_008.o obj/alienstorm.mtx500/midres_tile_009.o obj/alienstorm.mtx500/midres_tile_010.o obj/alienstorm.mtx500/midres_tile_011.o obj/alienstorm.mtx500/midres_tile_012.o obj/alienstorm.mtx500/midres_tile_013.o obj/alienstorm.mtx500/midres_tile_014.o obj/alienstorm.mtx500/midres_tile_015.o obj/alienstorm.mtx500/midres_tile_016.o obj/alienstorm.mtx500/midres_tile_018.o obj/alienstorm.mtx500/midres_tile_019.o obj/alienstorm.mtx500/midres_tile_020.o obj/alienstorm.mtx500/midres_tile_021.o obj/alienstorm.mtx500/midres_tile_022.o obj/alienstorm.mtx500/midres_tile_023.o obj/alienstorm.mtx500/midres_tile_025.o obj/alienstorm.mtx500/midres_tile_026.o obj/alienstorm.mtx500/midres_tile_027.o obj/alienstorm.mtx500/midres_tile_028.o obj/alienstorm.mtx500/midres_tile_multicolor_000.o obj/alienstorm.mtx500/midres_tile_multicolor_001.o obj/alienstorm.mtx500/midres_tile_multicolor_002.o obj/alienstorm.mtx500/midres_tile_multicolor_003.o obj/alienstorm.mtx500/midres_tile_multicolor_004.o obj/alienstorm.mtx500/midres_tile_multicolor_005.o obj/alienstorm.mtx500/midres_tile_multicolor_006.o obj/alienstorm.mtx500/midres_tile_multicolor_007.o obj/alienstorm.mtx500/midres_tile_multicolor_008.o obj/alienstorm.mtx500/midres_tile_multicolor_009.o obj/alienstorm.mtx500/midres_tile_multicolor_010.o obj/alienstorm.mtx500/midres_tile_multicolor_011.o obj/alienstorm.mtx500/midres_tile_multicolor_012.o obj/alienstorm.mtx500/midres_tile_processing_001.o obj/alienstorm.mtx500/midres_tile_processing_002.o obj/alienstorm.mtx500/midres_tile_processing_004.o obj/alienstorm.mtx500/midres_tile_processing_005.o obj/alienstorm.mtx500/midres_tile_processing_007.o obj/alienstorm.mtx500/midres_tile_processing_009.o obj/alienstorm.mtx500/midres_tile_processing_011.o obj/alienstorm.mtx500/midres_tile_processing_012.o obj/alienstorm.mtx500/midres_tile_processing_016.o obj/alienstorm.mtx500/midres_vanilla.o obj/alienstorm.mtx500/midres_vdp.o obj/alienstorm.mtx500/midres_vic.o obj/alienstorm.mtx500/midres_vic20.o obj/alienstorm.mtx500/utility_joycheck.o obj/alienstorm.mtx500/midres_control_011.o obj/alienstorm.mtx500/midres_data.o obj/alienstorm.mtx500/main.o obj/alienstorm.mtx500/rawdata.o obj/alienstorm.mtx500/midres_vdp_impl.o obj/alienstorm.mtx500/midres_io.o
	$(CC88) +mtx  -m $(LDFLAGS88) obj/alienstorm.mtx500/rawdata.o obj/alienstorm.mtx500/midres_io.o obj/alienstorm.mtx500/midres_vdp_impl.o obj/alienstorm.mtx500/demo_music.o obj/alienstorm.mtx500/demo_tile.o obj/alienstorm.mtx500/game_air_attack.o obj/alienstorm.mtx500/game_alien_storm.o obj/alienstorm.mtx500/game_elevator.o obj/alienstorm.mtx500/game_totto.o obj/alienstorm.mtx500/midres.o obj/alienstorm.mtx500/midres_atari.o obj/alienstorm.mtx500/midres_atmos.o obj/alienstorm.mtx500/midres_ay8910.o obj/alienstorm.mtx500/midres_c128.o obj/alienstorm.mtx500/midres_c16.o obj/alienstorm.mtx500/midres_c64.o obj/alienstorm.mtx500/midres_coleco.o obj/alienstorm.mtx500/midres_control_000.o obj/alienstorm.mtx500/midres_control_001.o obj/alienstorm.mtx500/midres_control_002.o obj/alienstorm.mtx500/midres_control_003.o obj/alienstorm.mtx500/midres_control_004.o obj/alienstorm.mtx500/midres_control_005.o obj/alienstorm.mtx500/midres_control_007.o obj/alienstorm.mtx500/midres_control_010.o obj/alienstorm.mtx500/midres_file_001.o obj/alienstorm.mtx500/midres_file_003.o obj/alienstorm.mtx500/midres_frame_000.o obj/alienstorm.mtx500/midres_frame_001.o obj/alienstorm.mtx500/midres_gb.o obj/alienstorm.mtx500/midres_lm80c.o obj/alienstorm.mtx500/midres_msx.o obj/alienstorm.mtx500/midres_mtx500.o obj/alienstorm.mtx500/midres_music_001.o obj/alienstorm.mtx500/midres_music_002.o obj/alienstorm.mtx500/midres_music_004.o obj/alienstorm.mtx500/midres_music_005.o obj/alienstorm.mtx500/midres_music_006.o obj/alienstorm.mtx500/midres_plus4.o obj/alienstorm.mtx500/midres_pokey.o obj/alienstorm.mtx500/midres_screen_000.o obj/alienstorm.mtx500/midres_screen_006.o obj/alienstorm.mtx500/midres_screen_007.o obj/alienstorm.mtx500/midres_screen_010.o obj/alienstorm.mtx500/midres_screen_012.o obj/alienstorm.mtx500/midres_screen_013.o obj/alienstorm.mtx500/midres_sid.o obj/alienstorm.mtx500/midres_sound_000.o obj/alienstorm.mtx500/midres_sound_001.o obj/alienstorm.mtx500/midres_sound_002.o obj/alienstorm.mtx500/midres_sound_005.o obj/alienstorm.mtx500/midres_sound_006.o obj/alienstorm.mtx500/midres_sound_007.o obj/alienstorm.mtx500/midres_svi.o obj/alienstorm.mtx500/midres_ted.o obj/alienstorm.mtx500/midres_tile_000.o obj/alienstorm.mtx500/midres_tile_004.o obj/alienstorm.mtx500/midres_tile_005.o obj/alienstorm.mtx500/midres_tile_006.o obj/alienstorm.mtx500/midres_tile_007.o obj/alienstorm.mtx500/midres_tile_008.o obj/alienstorm.mtx500/midres_tile_009.o obj/alienstorm.mtx500/midres_tile_010.o obj/alienstorm.mtx500/midres_tile_011.o obj/alienstorm.mtx500/midres_tile_012.o obj/alienstorm.mtx500/midres_tile_013.o obj/alienstorm.mtx500/midres_tile_014.o obj/alienstorm.mtx500/midres_tile_015.o obj/alienstorm.mtx500/midres_tile_016.o obj/alienstorm.mtx500/midres_tile_018.o obj/alienstorm.mtx500/midres_tile_019.o obj/alienstorm.mtx500/midres_tile_020.o obj/alienstorm.mtx500/midres_tile_021.o obj/alienstorm.mtx500/midres_tile_022.o obj/alienstorm.mtx500/midres_tile_023.o obj/alienstorm.mtx500/midres_tile_025.o obj/alienstorm.mtx500/midres_tile_026.o obj/alienstorm.mtx500/midres_tile_027.o obj/alienstorm.mtx500/midres_tile_028.o obj/alienstorm.mtx500/midres_tile_multicolor_000.o obj/alienstorm.mtx500/midres_tile_multicolor_001.o obj/alienstorm.mtx500/midres_tile_multicolor_002.o obj/alienstorm.mtx500/midres_tile_multicolor_003.o obj/alienstorm.mtx500/midres_tile_multicolor_004.o obj/alienstorm.mtx500/midres_tile_multicolor_005.o obj/alienstorm.mtx500/midres_tile_multicolor_006.o obj/alienstorm.mtx500/midres_tile_multicolor_007.o obj/alienstorm.mtx500/midres_tile_multicolor_008.o obj/alienstorm.mtx500/midres_tile_multicolor_009.o obj/alienstorm.mtx500/midres_tile_multicolor_010.o obj/alienstorm.mtx500/midres_tile_multicolor_011.o obj/alienstorm.mtx500/midres_tile_multicolor_012.o obj/alienstorm.mtx500/midres_tile_processing_001.o obj/alienstorm.mtx500/midres_tile_processing_002.o obj/alienstorm.mtx500/midres_tile_processing_004.o obj/alienstorm.mtx500/midres_tile_processing_005.o obj/alienstorm.mtx500/midres_tile_processing_007.o obj/alienstorm.mtx500/midres_tile_processing_009.o obj/alienstorm.mtx500/midres_tile_processing_011.o obj/alienstorm.mtx500/midres_tile_processing_012.o obj/alienstorm.mtx500/midres_tile_processing_016.o obj/alienstorm.mtx500/midres_vanilla.o obj/alienstorm.mtx500/midres_vdp.o obj/alienstorm.mtx500/midres_vic.o obj/alienstorm.mtx500/midres_vic20.o obj/alienstorm.mtx500/utility_joycheck.o obj/alienstorm.mtx500/midres_control_011.o obj/alienstorm.mtx500/midres_data.o obj/alienstorm.mtx500/main.o -create-app 
	$(call COPYFILES,a.wav,$(EXEDIR)/alienstorm.mtx500.wav)
	$(call COPYFILES,a.mtx,$(EXEDIR)/alienstorm.mtx500.mtx)
	$(call RMFILES,a.*)

# find_main_module(elevator)
# -> src/game_elevator.c

# -------------------------------------------------------------------
# --- ELEVATOR FOR MTX500 
# -------------------------------------------------------------------
# 

.PHONY: elevator.embedded.mtx500
elevator.embedded.mtx500:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator64.mpic -n zelintro.bin -i $(DATADIR)/elevator1.imf -n elevator1.imf -c src/rawdata.c -h src/rawdata.h
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -o obj/elevator.mtx500/rawdata.o src/rawdata.c

obj/elevator.mtx500/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.mtx500/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/elevator.mtx500/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.mtx500/midres_io.o src/midres_io.asm

obj/elevator.mtx500/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -D__ELEVATOR__ -o $@ $(subst obj/elevator.mtx500/,src/,$(@:.o=.c)) 

$(EXEDIR)/elevator.mtx500:	elevator.embedded.mtx500 obj/elevator.mtx500/demo_music.o obj/elevator.mtx500/demo_tile.o obj/elevator.mtx500/game_air_attack.o obj/elevator.mtx500/game_alien_storm.o obj/elevator.mtx500/game_elevator.o obj/elevator.mtx500/game_totto.o obj/elevator.mtx500/midres.o obj/elevator.mtx500/midres_atari.o obj/elevator.mtx500/midres_atmos.o obj/elevator.mtx500/midres_ay8910.o obj/elevator.mtx500/midres_c128.o obj/elevator.mtx500/midres_c16.o obj/elevator.mtx500/midres_c64.o obj/elevator.mtx500/midres_coleco.o obj/elevator.mtx500/midres_control_000.o obj/elevator.mtx500/midres_control_001.o obj/elevator.mtx500/midres_control_002.o obj/elevator.mtx500/midres_control_003.o obj/elevator.mtx500/midres_control_004.o obj/elevator.mtx500/midres_control_005.o obj/elevator.mtx500/midres_control_007.o obj/elevator.mtx500/midres_control_010.o obj/elevator.mtx500/midres_file_001.o obj/elevator.mtx500/midres_file_003.o obj/elevator.mtx500/midres_frame_000.o obj/elevator.mtx500/midres_frame_001.o obj/elevator.mtx500/midres_gb.o obj/elevator.mtx500/midres_lm80c.o obj/elevator.mtx500/midres_msx.o obj/elevator.mtx500/midres_mtx500.o obj/elevator.mtx500/midres_music_001.o obj/elevator.mtx500/midres_music_002.o obj/elevator.mtx500/midres_music_004.o obj/elevator.mtx500/midres_music_005.o obj/elevator.mtx500/midres_music_006.o obj/elevator.mtx500/midres_plus4.o obj/elevator.mtx500/midres_pokey.o obj/elevator.mtx500/midres_screen_000.o obj/elevator.mtx500/midres_screen_006.o obj/elevator.mtx500/midres_screen_007.o obj/elevator.mtx500/midres_screen_010.o obj/elevator.mtx500/midres_screen_012.o obj/elevator.mtx500/midres_screen_013.o obj/elevator.mtx500/midres_sid.o obj/elevator.mtx500/midres_sound_000.o obj/elevator.mtx500/midres_sound_001.o obj/elevator.mtx500/midres_sound_002.o obj/elevator.mtx500/midres_sound_005.o obj/elevator.mtx500/midres_sound_006.o obj/elevator.mtx500/midres_sound_007.o obj/elevator.mtx500/midres_svi.o obj/elevator.mtx500/midres_ted.o obj/elevator.mtx500/midres_tile_000.o obj/elevator.mtx500/midres_tile_004.o obj/elevator.mtx500/midres_tile_005.o obj/elevator.mtx500/midres_tile_006.o obj/elevator.mtx500/midres_tile_007.o obj/elevator.mtx500/midres_tile_008.o obj/elevator.mtx500/midres_tile_009.o obj/elevator.mtx500/midres_tile_010.o obj/elevator.mtx500/midres_tile_011.o obj/elevator.mtx500/midres_tile_012.o obj/elevator.mtx500/midres_tile_013.o obj/elevator.mtx500/midres_tile_014.o obj/elevator.mtx500/midres_tile_015.o obj/elevator.mtx500/midres_tile_016.o obj/elevator.mtx500/midres_tile_018.o obj/elevator.mtx500/midres_tile_019.o obj/elevator.mtx500/midres_tile_020.o obj/elevator.mtx500/midres_tile_021.o obj/elevator.mtx500/midres_tile_022.o obj/elevator.mtx500/midres_tile_023.o obj/elevator.mtx500/midres_tile_025.o obj/elevator.mtx500/midres_tile_026.o obj/elevator.mtx500/midres_tile_027.o obj/elevator.mtx500/midres_tile_028.o obj/elevator.mtx500/midres_tile_multicolor_000.o obj/elevator.mtx500/midres_tile_multicolor_001.o obj/elevator.mtx500/midres_tile_multicolor_002.o obj/elevator.mtx500/midres_tile_multicolor_003.o obj/elevator.mtx500/midres_tile_multicolor_004.o obj/elevator.mtx500/midres_tile_multicolor_005.o obj/elevator.mtx500/midres_tile_multicolor_006.o obj/elevator.mtx500/midres_tile_multicolor_007.o obj/elevator.mtx500/midres_tile_multicolor_008.o obj/elevator.mtx500/midres_tile_multicolor_009.o obj/elevator.mtx500/midres_tile_multicolor_010.o obj/elevator.mtx500/midres_tile_multicolor_011.o obj/elevator.mtx500/midres_tile_multicolor_012.o obj/elevator.mtx500/midres_tile_processing_001.o obj/elevator.mtx500/midres_tile_processing_002.o obj/elevator.mtx500/midres_tile_processing_004.o obj/elevator.mtx500/midres_tile_processing_005.o obj/elevator.mtx500/midres_tile_processing_007.o obj/elevator.mtx500/midres_tile_processing_009.o obj/elevator.mtx500/midres_tile_processing_011.o obj/elevator.mtx500/midres_tile_processing_012.o obj/elevator.mtx500/midres_tile_processing_016.o obj/elevator.mtx500/midres_vanilla.o obj/elevator.mtx500/midres_vdp.o obj/elevator.mtx500/midres_vic.o obj/elevator.mtx500/midres_vic20.o obj/elevator.mtx500/utility_joycheck.o obj/elevator.mtx500/midres_control_011.o obj/elevator.mtx500/midres_data.o obj/elevator.mtx500/main.o obj/elevator.mtx500/rawdata.o obj/elevator.mtx500/midres_vdp_impl.o obj/elevator.mtx500/midres_io.o
	$(CC88) +mtx  -m $(LDFLAGS88) obj/elevator.mtx500/rawdata.o obj/elevator.mtx500/midres_io.o obj/elevator.mtx500/midres_vdp_impl.o obj/elevator.mtx500/demo_music.o obj/elevator.mtx500/demo_tile.o obj/elevator.mtx500/game_air_attack.o obj/elevator.mtx500/game_alien_storm.o obj/elevator.mtx500/game_elevator.o obj/elevator.mtx500/game_totto.o obj/elevator.mtx500/midres.o obj/elevator.mtx500/midres_atari.o obj/elevator.mtx500/midres_atmos.o obj/elevator.mtx500/midres_ay8910.o obj/elevator.mtx500/midres_c128.o obj/elevator.mtx500/midres_c16.o obj/elevator.mtx500/midres_c64.o obj/elevator.mtx500/midres_coleco.o obj/elevator.mtx500/midres_control_000.o obj/elevator.mtx500/midres_control_001.o obj/elevator.mtx500/midres_control_002.o obj/elevator.mtx500/midres_control_003.o obj/elevator.mtx500/midres_control_004.o obj/elevator.mtx500/midres_control_005.o obj/elevator.mtx500/midres_control_007.o obj/elevator.mtx500/midres_control_010.o obj/elevator.mtx500/midres_file_001.o obj/elevator.mtx500/midres_file_003.o obj/elevator.mtx500/midres_frame_000.o obj/elevator.mtx500/midres_frame_001.o obj/elevator.mtx500/midres_gb.o obj/elevator.mtx500/midres_lm80c.o obj/elevator.mtx500/midres_msx.o obj/elevator.mtx500/midres_mtx500.o obj/elevator.mtx500/midres_music_001.o obj/elevator.mtx500/midres_music_002.o obj/elevator.mtx500/midres_music_004.o obj/elevator.mtx500/midres_music_005.o obj/elevator.mtx500/midres_music_006.o obj/elevator.mtx500/midres_plus4.o obj/elevator.mtx500/midres_pokey.o obj/elevator.mtx500/midres_screen_000.o obj/elevator.mtx500/midres_screen_006.o obj/elevator.mtx500/midres_screen_007.o obj/elevator.mtx500/midres_screen_010.o obj/elevator.mtx500/midres_screen_012.o obj/elevator.mtx500/midres_screen_013.o obj/elevator.mtx500/midres_sid.o obj/elevator.mtx500/midres_sound_000.o obj/elevator.mtx500/midres_sound_001.o obj/elevator.mtx500/midres_sound_002.o obj/elevator.mtx500/midres_sound_005.o obj/elevator.mtx500/midres_sound_006.o obj/elevator.mtx500/midres_sound_007.o obj/elevator.mtx500/midres_svi.o obj/elevator.mtx500/midres_ted.o obj/elevator.mtx500/midres_tile_000.o obj/elevator.mtx500/midres_tile_004.o obj/elevator.mtx500/midres_tile_005.o obj/elevator.mtx500/midres_tile_006.o obj/elevator.mtx500/midres_tile_007.o obj/elevator.mtx500/midres_tile_008.o obj/elevator.mtx500/midres_tile_009.o obj/elevator.mtx500/midres_tile_010.o obj/elevator.mtx500/midres_tile_011.o obj/elevator.mtx500/midres_tile_012.o obj/elevator.mtx500/midres_tile_013.o obj/elevator.mtx500/midres_tile_014.o obj/elevator.mtx500/midres_tile_015.o obj/elevator.mtx500/midres_tile_016.o obj/elevator.mtx500/midres_tile_018.o obj/elevator.mtx500/midres_tile_019.o obj/elevator.mtx500/midres_tile_020.o obj/elevator.mtx500/midres_tile_021.o obj/elevator.mtx500/midres_tile_022.o obj/elevator.mtx500/midres_tile_023.o obj/elevator.mtx500/midres_tile_025.o obj/elevator.mtx500/midres_tile_026.o obj/elevator.mtx500/midres_tile_027.o obj/elevator.mtx500/midres_tile_028.o obj/elevator.mtx500/midres_tile_multicolor_000.o obj/elevator.mtx500/midres_tile_multicolor_001.o obj/elevator.mtx500/midres_tile_multicolor_002.o obj/elevator.mtx500/midres_tile_multicolor_003.o obj/elevator.mtx500/midres_tile_multicolor_004.o obj/elevator.mtx500/midres_tile_multicolor_005.o obj/elevator.mtx500/midres_tile_multicolor_006.o obj/elevator.mtx500/midres_tile_multicolor_007.o obj/elevator.mtx500/midres_tile_multicolor_008.o obj/elevator.mtx500/midres_tile_multicolor_009.o obj/elevator.mtx500/midres_tile_multicolor_010.o obj/elevator.mtx500/midres_tile_multicolor_011.o obj/elevator.mtx500/midres_tile_multicolor_012.o obj/elevator.mtx500/midres_tile_processing_001.o obj/elevator.mtx500/midres_tile_processing_002.o obj/elevator.mtx500/midres_tile_processing_004.o obj/elevator.mtx500/midres_tile_processing_005.o obj/elevator.mtx500/midres_tile_processing_007.o obj/elevator.mtx500/midres_tile_processing_009.o obj/elevator.mtx500/midres_tile_processing_011.o obj/elevator.mtx500/midres_tile_processing_012.o obj/elevator.mtx500/midres_tile_processing_016.o obj/elevator.mtx500/midres_vanilla.o obj/elevator.mtx500/midres_vdp.o obj/elevator.mtx500/midres_vic.o obj/elevator.mtx500/midres_vic20.o obj/elevator.mtx500/utility_joycheck.o obj/elevator.mtx500/midres_control_011.o obj/elevator.mtx500/midres_data.o obj/elevator.mtx500/main.o -create-app 
	$(call COPYFILES,a.wav,$(EXEDIR)/elevator.mtx500.wav)
	$(call COPYFILES,a.mtx,$(EXEDIR)/elevator.mtx500.mtx)
	$(call RMFILES,a.*)

                    
#!!! missing resources for joycheck (mtx500)

# find_main_module(totto)
# -> src/game_totto.c

# -------------------------------------------------------------------
# --- TOTTO FOR MTX500 
# -------------------------------------------------------------------
# 

.PHONY: totto.embedded.mtx500
totto.embedded.mtx500:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -i $(DATADIR)/totto.imf -n totto.imf -i $(DATADIR)/totto3.imf -n totto3.imf -c src/rawdata.c -h src/rawdata.h
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -o obj/totto.mtx500/rawdata.o src/rawdata.c

obj/totto.mtx500/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.mtx500/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/totto.mtx500/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.mtx500/midres_io.o src/midres_io.asm

obj/totto.mtx500/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -DFRAME_BUFFER -D__TOTTO__ -o $@ $(subst obj/totto.mtx500/,src/,$(@:.o=.c)) 

$(EXEDIR)/totto.mtx500:	totto.embedded.mtx500 obj/totto.mtx500/demo_music.o obj/totto.mtx500/demo_tile.o obj/totto.mtx500/game_air_attack.o obj/totto.mtx500/game_alien_storm.o obj/totto.mtx500/game_elevator.o obj/totto.mtx500/game_totto.o obj/totto.mtx500/midres.o obj/totto.mtx500/midres_atari.o obj/totto.mtx500/midres_atmos.o obj/totto.mtx500/midres_ay8910.o obj/totto.mtx500/midres_c128.o obj/totto.mtx500/midres_c16.o obj/totto.mtx500/midres_c64.o obj/totto.mtx500/midres_coleco.o obj/totto.mtx500/midres_control_000.o obj/totto.mtx500/midres_control_001.o obj/totto.mtx500/midres_control_002.o obj/totto.mtx500/midres_control_003.o obj/totto.mtx500/midres_control_004.o obj/totto.mtx500/midres_control_005.o obj/totto.mtx500/midres_control_007.o obj/totto.mtx500/midres_control_010.o obj/totto.mtx500/midres_file_001.o obj/totto.mtx500/midres_file_003.o obj/totto.mtx500/midres_frame_000.o obj/totto.mtx500/midres_frame_001.o obj/totto.mtx500/midres_gb.o obj/totto.mtx500/midres_lm80c.o obj/totto.mtx500/midres_msx.o obj/totto.mtx500/midres_mtx500.o obj/totto.mtx500/midres_music_001.o obj/totto.mtx500/midres_music_002.o obj/totto.mtx500/midres_music_004.o obj/totto.mtx500/midres_music_005.o obj/totto.mtx500/midres_music_006.o obj/totto.mtx500/midres_plus4.o obj/totto.mtx500/midres_pokey.o obj/totto.mtx500/midres_screen_000.o obj/totto.mtx500/midres_screen_006.o obj/totto.mtx500/midres_screen_007.o obj/totto.mtx500/midres_screen_010.o obj/totto.mtx500/midres_screen_012.o obj/totto.mtx500/midres_screen_013.o obj/totto.mtx500/midres_sid.o obj/totto.mtx500/midres_sound_000.o obj/totto.mtx500/midres_sound_001.o obj/totto.mtx500/midres_sound_002.o obj/totto.mtx500/midres_sound_005.o obj/totto.mtx500/midres_sound_006.o obj/totto.mtx500/midres_sound_007.o obj/totto.mtx500/midres_svi.o obj/totto.mtx500/midres_ted.o obj/totto.mtx500/midres_tile_000.o obj/totto.mtx500/midres_tile_004.o obj/totto.mtx500/midres_tile_005.o obj/totto.mtx500/midres_tile_006.o obj/totto.mtx500/midres_tile_007.o obj/totto.mtx500/midres_tile_008.o obj/totto.mtx500/midres_tile_009.o obj/totto.mtx500/midres_tile_010.o obj/totto.mtx500/midres_tile_011.o obj/totto.mtx500/midres_tile_012.o obj/totto.mtx500/midres_tile_013.o obj/totto.mtx500/midres_tile_014.o obj/totto.mtx500/midres_tile_015.o obj/totto.mtx500/midres_tile_016.o obj/totto.mtx500/midres_tile_018.o obj/totto.mtx500/midres_tile_019.o obj/totto.mtx500/midres_tile_020.o obj/totto.mtx500/midres_tile_021.o obj/totto.mtx500/midres_tile_022.o obj/totto.mtx500/midres_tile_023.o obj/totto.mtx500/midres_tile_025.o obj/totto.mtx500/midres_tile_026.o obj/totto.mtx500/midres_tile_027.o obj/totto.mtx500/midres_tile_028.o obj/totto.mtx500/midres_tile_multicolor_000.o obj/totto.mtx500/midres_tile_multicolor_001.o obj/totto.mtx500/midres_tile_multicolor_002.o obj/totto.mtx500/midres_tile_multicolor_003.o obj/totto.mtx500/midres_tile_multicolor_004.o obj/totto.mtx500/midres_tile_multicolor_005.o obj/totto.mtx500/midres_tile_multicolor_006.o obj/totto.mtx500/midres_tile_multicolor_007.o obj/totto.mtx500/midres_tile_multicolor_008.o obj/totto.mtx500/midres_tile_multicolor_009.o obj/totto.mtx500/midres_tile_multicolor_010.o obj/totto.mtx500/midres_tile_multicolor_011.o obj/totto.mtx500/midres_tile_multicolor_012.o obj/totto.mtx500/midres_tile_processing_001.o obj/totto.mtx500/midres_tile_processing_002.o obj/totto.mtx500/midres_tile_processing_004.o obj/totto.mtx500/midres_tile_processing_005.o obj/totto.mtx500/midres_tile_processing_007.o obj/totto.mtx500/midres_tile_processing_009.o obj/totto.mtx500/midres_tile_processing_011.o obj/totto.mtx500/midres_tile_processing_012.o obj/totto.mtx500/midres_tile_processing_016.o obj/totto.mtx500/midres_vanilla.o obj/totto.mtx500/midres_vdp.o obj/totto.mtx500/midres_vic.o obj/totto.mtx500/midres_vic20.o obj/totto.mtx500/utility_joycheck.o obj/totto.mtx500/midres_control_011.o obj/totto.mtx500/midres_data.o obj/totto.mtx500/main.o obj/totto.mtx500/rawdata.o obj/totto.mtx500/midres_vdp_impl.o obj/totto.mtx500/midres_io.o
	$(CC88) +mtx  -m $(LDFLAGS88) obj/totto.mtx500/rawdata.o obj/totto.mtx500/midres_io.o obj/totto.mtx500/midres_vdp_impl.o obj/totto.mtx500/demo_music.o obj/totto.mtx500/demo_tile.o obj/totto.mtx500/game_air_attack.o obj/totto.mtx500/game_alien_storm.o obj/totto.mtx500/game_elevator.o obj/totto.mtx500/game_totto.o obj/totto.mtx500/midres.o obj/totto.mtx500/midres_atari.o obj/totto.mtx500/midres_atmos.o obj/totto.mtx500/midres_ay8910.o obj/totto.mtx500/midres_c128.o obj/totto.mtx500/midres_c16.o obj/totto.mtx500/midres_c64.o obj/totto.mtx500/midres_coleco.o obj/totto.mtx500/midres_control_000.o obj/totto.mtx500/midres_control_001.o obj/totto.mtx500/midres_control_002.o obj/totto.mtx500/midres_control_003.o obj/totto.mtx500/midres_control_004.o obj/totto.mtx500/midres_control_005.o obj/totto.mtx500/midres_control_007.o obj/totto.mtx500/midres_control_010.o obj/totto.mtx500/midres_file_001.o obj/totto.mtx500/midres_file_003.o obj/totto.mtx500/midres_frame_000.o obj/totto.mtx500/midres_frame_001.o obj/totto.mtx500/midres_gb.o obj/totto.mtx500/midres_lm80c.o obj/totto.mtx500/midres_msx.o obj/totto.mtx500/midres_mtx500.o obj/totto.mtx500/midres_music_001.o obj/totto.mtx500/midres_music_002.o obj/totto.mtx500/midres_music_004.o obj/totto.mtx500/midres_music_005.o obj/totto.mtx500/midres_music_006.o obj/totto.mtx500/midres_plus4.o obj/totto.mtx500/midres_pokey.o obj/totto.mtx500/midres_screen_000.o obj/totto.mtx500/midres_screen_006.o obj/totto.mtx500/midres_screen_007.o obj/totto.mtx500/midres_screen_010.o obj/totto.mtx500/midres_screen_012.o obj/totto.mtx500/midres_screen_013.o obj/totto.mtx500/midres_sid.o obj/totto.mtx500/midres_sound_000.o obj/totto.mtx500/midres_sound_001.o obj/totto.mtx500/midres_sound_002.o obj/totto.mtx500/midres_sound_005.o obj/totto.mtx500/midres_sound_006.o obj/totto.mtx500/midres_sound_007.o obj/totto.mtx500/midres_svi.o obj/totto.mtx500/midres_ted.o obj/totto.mtx500/midres_tile_000.o obj/totto.mtx500/midres_tile_004.o obj/totto.mtx500/midres_tile_005.o obj/totto.mtx500/midres_tile_006.o obj/totto.mtx500/midres_tile_007.o obj/totto.mtx500/midres_tile_008.o obj/totto.mtx500/midres_tile_009.o obj/totto.mtx500/midres_tile_010.o obj/totto.mtx500/midres_tile_011.o obj/totto.mtx500/midres_tile_012.o obj/totto.mtx500/midres_tile_013.o obj/totto.mtx500/midres_tile_014.o obj/totto.mtx500/midres_tile_015.o obj/totto.mtx500/midres_tile_016.o obj/totto.mtx500/midres_tile_018.o obj/totto.mtx500/midres_tile_019.o obj/totto.mtx500/midres_tile_020.o obj/totto.mtx500/midres_tile_021.o obj/totto.mtx500/midres_tile_022.o obj/totto.mtx500/midres_tile_023.o obj/totto.mtx500/midres_tile_025.o obj/totto.mtx500/midres_tile_026.o obj/totto.mtx500/midres_tile_027.o obj/totto.mtx500/midres_tile_028.o obj/totto.mtx500/midres_tile_multicolor_000.o obj/totto.mtx500/midres_tile_multicolor_001.o obj/totto.mtx500/midres_tile_multicolor_002.o obj/totto.mtx500/midres_tile_multicolor_003.o obj/totto.mtx500/midres_tile_multicolor_004.o obj/totto.mtx500/midres_tile_multicolor_005.o obj/totto.mtx500/midres_tile_multicolor_006.o obj/totto.mtx500/midres_tile_multicolor_007.o obj/totto.mtx500/midres_tile_multicolor_008.o obj/totto.mtx500/midres_tile_multicolor_009.o obj/totto.mtx500/midres_tile_multicolor_010.o obj/totto.mtx500/midres_tile_multicolor_011.o obj/totto.mtx500/midres_tile_multicolor_012.o obj/totto.mtx500/midres_tile_processing_001.o obj/totto.mtx500/midres_tile_processing_002.o obj/totto.mtx500/midres_tile_processing_004.o obj/totto.mtx500/midres_tile_processing_005.o obj/totto.mtx500/midres_tile_processing_007.o obj/totto.mtx500/midres_tile_processing_009.o obj/totto.mtx500/midres_tile_processing_011.o obj/totto.mtx500/midres_tile_processing_012.o obj/totto.mtx500/midres_tile_processing_016.o obj/totto.mtx500/midres_vanilla.o obj/totto.mtx500/midres_vdp.o obj/totto.mtx500/midres_vic.o obj/totto.mtx500/midres_vic20.o obj/totto.mtx500/utility_joycheck.o obj/totto.mtx500/midres_control_011.o obj/totto.mtx500/midres_data.o obj/totto.mtx500/main.o -create-app 
	$(call COPYFILES,a.wav,$(EXEDIR)/totto.mtx500.wav)
	$(call COPYFILES,a.mtx,$(EXEDIR)/totto.mtx500.mtx)
	$(call RMFILES,a.*)


# -------------------------------------------------------------------
# --- AIRATTACK FOR PLUS4 
# -------------------------------------------------------------------

.PHONY: airattack.embedded.plus4
airattack.embedded.plus4:
	$(FILE2INCLUDE) -i $(DATADIR)/aatiles4.bin -n zztiles.bin -i $(DATADIR)/aaintro16.mpic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h

obj/airattack.plus4/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t plus4 -c $(CFLAGS)  -Osir -Cl -D__AIRATTACK__ -D__CBM__ -o $@ $(subst obj/airattack.plus4/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.plus4: airattack.embedded.plus4 $(subst PLATFORM,airattack.plus4,$(OBJS))
	$(CC) -Ln demoplus4.lbl -t plus4 -C cfg/plus4.cfg $(LDFLAGS) -m $(EXEDIR)/airattack.plus4.map -o $(EXEDIR)/airattack.plus4  $(subst PLATFORM,airattack.plus4,$(OBJS)) $(LIBDIR)/midres.plus4.lib
	$(call RMFILES,$(EXEDIR)/airattack.plus4.d64)
	$(CC1541) -f loader -w $(DATADIR)/airattack_loader4.prg $(EXEDIR)/airattack.plus4.d64 
	$(CC1541) -f airattack -w $(EXEDIR)/airattack.plus4 $(EXEDIR)/airattack.plus4.d64 
	$(CC1541) -f zztiles.bin -w $(DATADIR)/aatiles4.bin $(EXEDIR)/airattack.plus4.d64 
	$(CC1541) -f zzintro.pic -w $(DATADIR)/aaintro16.mpic $(EXEDIR)/airattack.plus4.d64 


# -------------------------------------------------------------------
# --- ALIENSTORM FOR PLUS4 
# -------------------------------------------------------------------

.PHONY: alienstorm.embedded.plus4
alienstorm.embedded.plus4:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h

obj/alienstorm.plus4/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t plus4 -c $(CFLAGS)  -Osir -Cl -D__ALIENSTORM__ -D__CBM__ -o $@ $(subst obj/alienstorm.plus4/,src/,$(@:.o=.c))

$(EXEDIR)/alienstorm.plus4: alienstorm.embedded.plus4 $(subst PLATFORM,alienstorm.plus4,$(OBJS))
	$(CC) -Ln demoplus4.lbl -t plus4 -C cfg/plus4.cfg $(LDFLAGS) -m $(EXEDIR)/alienstorm.plus4.map -o $(EXEDIR)/alienstorm.plus4  $(subst PLATFORM,alienstorm.plus4,$(OBJS)) $(LIBDIR)/midres.plus4.lib
	$(call RMFILES,$(EXEDIR)/alienstorm.plus4.d64)
	$(CC1541) -f loader -w $(DATADIR)/asloader4.prg $(EXEDIR)/alienstorm.plus4.d64 
	$(CC1541) -f alienstorm -w $(EXEDIR)/alienstorm.plus4 $(EXEDIR)/alienstorm.plus4.d64 
	$(CC1541) -f zstiles.bin -w $(DATADIR)/astiles.bin $(EXEDIR)/alienstorm.plus4.d64 
	$(CC1541) -f zstiles1.bin -w $(DATADIR)/astiles1.bin $(EXEDIR)/alienstorm.plus4.d64 
	$(CC1541) -f zstiles2.bin -w $(DATADIR)/astiles2.bin $(EXEDIR)/alienstorm.plus4.d64 


# -------------------------------------------------------------------
# --- ELEVATOR FOR PLUS4 
# -------------------------------------------------------------------

.PHONY: elevator.embedded.plus4
elevator.embedded.plus4:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator16.mpic -n zelintro.bin -i $(DATADIR)/elevator1.imf -n elevator1.imf -c src/rawdata.c -h src/rawdata.h

obj/elevator.plus4/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t plus4 -c $(CFLAGS)  -Osir -Cl -D__ELEVATOR__ -D__CBM__ -o $@ $(subst obj/elevator.plus4/,src/,$(@:.o=.c))

$(EXEDIR)/elevator.plus4: elevator.embedded.plus4 $(subst PLATFORM,elevator.plus4,$(OBJS))
	$(CC) -Ln demoplus4.lbl -t plus4 -C cfg/plus4.cfg $(LDFLAGS) -m $(EXEDIR)/elevator.plus4.map -o $(EXEDIR)/elevator.plus4  $(subst PLATFORM,elevator.plus4,$(OBJS)) $(LIBDIR)/midres.plus4.lib
	$(call RMFILES,$(EXEDIR)/elevator.plus4.d64)
	$(CC1541) -f loader -w $(DATADIR)/elloader4.prg $(EXEDIR)/elevator.plus4.d64 
	$(CC1541) -f elevator -w $(EXEDIR)/elevator.plus4 $(EXEDIR)/elevator.plus4.d64 
	$(CC1541) -f zeltiles.bin -w $(DATADIR)/zeltiles.bin $(EXEDIR)/elevator.plus4.d64 
	$(CC1541) -f zelintro.bin -w $(DATADIR)/elevator16.mpic $(EXEDIR)/elevator.plus4.d64 
	$(CC1541) -f elevator1.imf -w $(DATADIR)/elevator1.imf $(EXEDIR)/elevator.plus4.d64 


# -------------------------------------------------------------------
# --- JOYCHECK FOR PLUS4 
# -------------------------------------------------------------------

.PHONY: joycheck.embedded.plus4
joycheck.embedded.plus4:
	$(FILE2INCLUDE) -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -c src/rawdata.c -h src/rawdata.h

obj/joycheck.plus4/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t plus4 -c $(CFLAGS)  -Osir -Cl -D__JOYCHECK__ -D__CBM__ -o $@ $(subst obj/joycheck.plus4/,src/,$(@:.o=.c))

$(EXEDIR)/joycheck.plus4: joycheck.embedded.plus4 $(subst PLATFORM,joycheck.plus4,$(OBJS))
	$(CC) -Ln demoplus4.lbl -t plus4 -C cfg/plus4.cfg $(LDFLAGS) -m $(EXEDIR)/joycheck.plus4.map -o $(EXEDIR)/joycheck.plus4  $(subst PLATFORM,joycheck.plus4,$(OBJS)) $(LIBDIR)/midres.plus4.lib
	$(call RMFILES,$(EXEDIR)/joycheck.plus4.d64)
	$(CC1541) -f loader -w $(DATADIR)/jcloader4.prg $(EXEDIR)/joycheck.plus4.d64 
	$(CC1541) -f joycheck -w $(EXEDIR)/joycheck.plus4 $(EXEDIR)/joycheck.plus4.d64 
	$(CC1541) -f zdjtiles.bin -w $(DATADIR)/zdjtiles.bin $(EXEDIR)/joycheck.plus4.d64 


# -------------------------------------------------------------------
# --- TOTTO FOR PLUS4 
# -------------------------------------------------------------------

.PHONY: totto.embedded.plus4
totto.embedded.plus4:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -i $(DATADIR)/ttfinal116.mpic -n ttfinal1.mpic -i $(DATADIR)/ttfinal216.mpic -n ttfinal2.mpic -i $(DATADIR)/ttfinal316.mpic -n ttfinal3.mpic -i $(DATADIR)/ttfinal416.mpic -n ttfinal4.mpic -i $(DATADIR)/totto.imf -n totto.imf -i $(DATADIR)/totto3.imf -n totto3.imf -c src/rawdata.c -h src/rawdata.h

obj/totto.plus4/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t plus4 -c $(CFLAGS)  -Osir -Cl -D__TOTTO__ -D__CBM__ -o $@ $(subst obj/totto.plus4/,src/,$(@:.o=.c))

$(EXEDIR)/totto.plus4: totto.embedded.plus4 $(subst PLATFORM,totto.plus4,$(OBJS))
	$(CC) -Ln demoplus4.lbl -t plus4 -C cfg/plus4.cfg $(LDFLAGS) -m $(EXEDIR)/totto.plus4.map -o $(EXEDIR)/totto.plus4  $(subst PLATFORM,totto.plus4,$(OBJS)) $(LIBDIR)/midres.plus4.lib
	$(call RMFILES,$(EXEDIR)/totto.plus4.d64)
	$(CC1541) -f loader -w $(DATADIR)/totto_loader4.prg $(EXEDIR)/totto.plus4.d64 
	$(CC1541) -f totto -w $(EXEDIR)/totto.plus4 $(EXEDIR)/totto.plus4.d64 
	$(CC1541) -f tttiles.bin -w $(DATADIR)/tttiles.bin $(EXEDIR)/totto.plus4.d64 
	$(CC1541) -f tttiles1.bin -w $(DATADIR)/tttiles1.bin $(EXEDIR)/totto.plus4.d64 
	$(CC1541) -f ttfinal1.mpic -w $(DATADIR)/ttfinal116.mpic $(EXEDIR)/totto.plus4.d64 
	$(CC1541) -f ttfinal2.mpic -w $(DATADIR)/ttfinal216.mpic $(EXEDIR)/totto.plus4.d64 
	$(CC1541) -f ttfinal3.mpic -w $(DATADIR)/ttfinal316.mpic $(EXEDIR)/totto.plus4.d64 
	$(CC1541) -f ttfinal4.mpic -w $(DATADIR)/ttfinal416.mpic $(EXEDIR)/totto.plus4.d64 
	$(CC1541) -f totto.imf -w $(DATADIR)/totto.imf $(EXEDIR)/totto.plus4.d64 
	$(CC1541) -f totto3.imf -w $(DATADIR)/totto3.imf $(EXEDIR)/totto.plus4.d64 

# find_main_module(airattack)
# -> src/game_air_attack.c

# -------------------------------------------------------------------
# --- AIRATTACK FOR SVI 
# -------------------------------------------------------------------
# 

.PHONY: airattack.embedded.svi
airattack.embedded.svi:
	$(FILE2INCLUDE) -i $(DATADIR)/ztiles.bin -n ztiles.bin -i $(DATADIR)/aatiles4.bin -n zztiles.bin -i $(DATADIR)/aaintrox.pic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/airattack.svi/rawdata.o src/rawdata.c

obj/airattack.svi/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.svi/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/airattack.svi/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.svi/midres_io.o src/midres_io.asm

obj/airattack.svi/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__AIRATTACK__ -o $@ $(subst obj/airattack.svi/,src/,$(@:.o=.c)) 

$(EXEDIR)/airattack.svi:	airattack.embedded.svi obj/airattack.svi/demo_music.o obj/airattack.svi/demo_tile.o obj/airattack.svi/game_air_attack.o obj/airattack.svi/game_alien_storm.o obj/airattack.svi/game_elevator.o obj/airattack.svi/game_totto.o obj/airattack.svi/midres.o obj/airattack.svi/midres_atari.o obj/airattack.svi/midres_atmos.o obj/airattack.svi/midres_ay8910.o obj/airattack.svi/midres_c128.o obj/airattack.svi/midres_c16.o obj/airattack.svi/midres_c64.o obj/airattack.svi/midres_coleco.o obj/airattack.svi/midres_control_000.o obj/airattack.svi/midres_control_001.o obj/airattack.svi/midres_control_002.o obj/airattack.svi/midres_control_003.o obj/airattack.svi/midres_control_004.o obj/airattack.svi/midres_control_005.o obj/airattack.svi/midres_control_007.o obj/airattack.svi/midres_control_010.o obj/airattack.svi/midres_file_001.o obj/airattack.svi/midres_file_003.o obj/airattack.svi/midres_frame_000.o obj/airattack.svi/midres_frame_001.o obj/airattack.svi/midres_gb.o obj/airattack.svi/midres_lm80c.o obj/airattack.svi/midres_msx.o obj/airattack.svi/midres_mtx500.o obj/airattack.svi/midres_music_001.o obj/airattack.svi/midres_music_002.o obj/airattack.svi/midres_music_004.o obj/airattack.svi/midres_music_005.o obj/airattack.svi/midres_music_006.o obj/airattack.svi/midres_plus4.o obj/airattack.svi/midres_pokey.o obj/airattack.svi/midres_screen_000.o obj/airattack.svi/midres_screen_006.o obj/airattack.svi/midres_screen_007.o obj/airattack.svi/midres_screen_010.o obj/airattack.svi/midres_screen_012.o obj/airattack.svi/midres_screen_013.o obj/airattack.svi/midres_sid.o obj/airattack.svi/midres_sound_000.o obj/airattack.svi/midres_sound_001.o obj/airattack.svi/midres_sound_002.o obj/airattack.svi/midres_sound_005.o obj/airattack.svi/midres_sound_006.o obj/airattack.svi/midres_sound_007.o obj/airattack.svi/midres_svi.o obj/airattack.svi/midres_ted.o obj/airattack.svi/midres_tile_000.o obj/airattack.svi/midres_tile_004.o obj/airattack.svi/midres_tile_005.o obj/airattack.svi/midres_tile_006.o obj/airattack.svi/midres_tile_007.o obj/airattack.svi/midres_tile_008.o obj/airattack.svi/midres_tile_009.o obj/airattack.svi/midres_tile_010.o obj/airattack.svi/midres_tile_011.o obj/airattack.svi/midres_tile_012.o obj/airattack.svi/midres_tile_013.o obj/airattack.svi/midres_tile_014.o obj/airattack.svi/midres_tile_015.o obj/airattack.svi/midres_tile_016.o obj/airattack.svi/midres_tile_018.o obj/airattack.svi/midres_tile_019.o obj/airattack.svi/midres_tile_020.o obj/airattack.svi/midres_tile_021.o obj/airattack.svi/midres_tile_022.o obj/airattack.svi/midres_tile_023.o obj/airattack.svi/midres_tile_025.o obj/airattack.svi/midres_tile_026.o obj/airattack.svi/midres_tile_027.o obj/airattack.svi/midres_tile_028.o obj/airattack.svi/midres_tile_multicolor_000.o obj/airattack.svi/midres_tile_multicolor_001.o obj/airattack.svi/midres_tile_multicolor_002.o obj/airattack.svi/midres_tile_multicolor_003.o obj/airattack.svi/midres_tile_multicolor_004.o obj/airattack.svi/midres_tile_multicolor_005.o obj/airattack.svi/midres_tile_multicolor_006.o obj/airattack.svi/midres_tile_multicolor_007.o obj/airattack.svi/midres_tile_multicolor_008.o obj/airattack.svi/midres_tile_multicolor_009.o obj/airattack.svi/midres_tile_multicolor_010.o obj/airattack.svi/midres_tile_multicolor_011.o obj/airattack.svi/midres_tile_multicolor_012.o obj/airattack.svi/midres_tile_processing_001.o obj/airattack.svi/midres_tile_processing_002.o obj/airattack.svi/midres_tile_processing_004.o obj/airattack.svi/midres_tile_processing_005.o obj/airattack.svi/midres_tile_processing_007.o obj/airattack.svi/midres_tile_processing_009.o obj/airattack.svi/midres_tile_processing_011.o obj/airattack.svi/midres_tile_processing_012.o obj/airattack.svi/midres_tile_processing_016.o obj/airattack.svi/midres_vanilla.o obj/airattack.svi/midres_vdp.o obj/airattack.svi/midres_vic.o obj/airattack.svi/midres_vic20.o obj/airattack.svi/utility_joycheck.o obj/airattack.svi/midres_control_011.o obj/airattack.svi/midres_data.o obj/airattack.svi/main.o obj/airattack.svi/rawdata.o obj/airattack.svi/midres_vdp_impl.o obj/airattack.svi/midres_io.o
	$(CC88) +svi  -m $(LDFLAGS88) obj/airattack.svi/rawdata.o obj/airattack.svi/midres_io.o obj/airattack.svi/midres_vdp_impl.o obj/airattack.svi/demo_music.o obj/airattack.svi/demo_tile.o obj/airattack.svi/game_air_attack.o obj/airattack.svi/game_alien_storm.o obj/airattack.svi/game_elevator.o obj/airattack.svi/game_totto.o obj/airattack.svi/midres.o obj/airattack.svi/midres_atari.o obj/airattack.svi/midres_atmos.o obj/airattack.svi/midres_ay8910.o obj/airattack.svi/midres_c128.o obj/airattack.svi/midres_c16.o obj/airattack.svi/midres_c64.o obj/airattack.svi/midres_coleco.o obj/airattack.svi/midres_control_000.o obj/airattack.svi/midres_control_001.o obj/airattack.svi/midres_control_002.o obj/airattack.svi/midres_control_003.o obj/airattack.svi/midres_control_004.o obj/airattack.svi/midres_control_005.o obj/airattack.svi/midres_control_007.o obj/airattack.svi/midres_control_010.o obj/airattack.svi/midres_file_001.o obj/airattack.svi/midres_file_003.o obj/airattack.svi/midres_frame_000.o obj/airattack.svi/midres_frame_001.o obj/airattack.svi/midres_gb.o obj/airattack.svi/midres_lm80c.o obj/airattack.svi/midres_msx.o obj/airattack.svi/midres_mtx500.o obj/airattack.svi/midres_music_001.o obj/airattack.svi/midres_music_002.o obj/airattack.svi/midres_music_004.o obj/airattack.svi/midres_music_005.o obj/airattack.svi/midres_music_006.o obj/airattack.svi/midres_plus4.o obj/airattack.svi/midres_pokey.o obj/airattack.svi/midres_screen_000.o obj/airattack.svi/midres_screen_006.o obj/airattack.svi/midres_screen_007.o obj/airattack.svi/midres_screen_010.o obj/airattack.svi/midres_screen_012.o obj/airattack.svi/midres_screen_013.o obj/airattack.svi/midres_sid.o obj/airattack.svi/midres_sound_000.o obj/airattack.svi/midres_sound_001.o obj/airattack.svi/midres_sound_002.o obj/airattack.svi/midres_sound_005.o obj/airattack.svi/midres_sound_006.o obj/airattack.svi/midres_sound_007.o obj/airattack.svi/midres_svi.o obj/airattack.svi/midres_ted.o obj/airattack.svi/midres_tile_000.o obj/airattack.svi/midres_tile_004.o obj/airattack.svi/midres_tile_005.o obj/airattack.svi/midres_tile_006.o obj/airattack.svi/midres_tile_007.o obj/airattack.svi/midres_tile_008.o obj/airattack.svi/midres_tile_009.o obj/airattack.svi/midres_tile_010.o obj/airattack.svi/midres_tile_011.o obj/airattack.svi/midres_tile_012.o obj/airattack.svi/midres_tile_013.o obj/airattack.svi/midres_tile_014.o obj/airattack.svi/midres_tile_015.o obj/airattack.svi/midres_tile_016.o obj/airattack.svi/midres_tile_018.o obj/airattack.svi/midres_tile_019.o obj/airattack.svi/midres_tile_020.o obj/airattack.svi/midres_tile_021.o obj/airattack.svi/midres_tile_022.o obj/airattack.svi/midres_tile_023.o obj/airattack.svi/midres_tile_025.o obj/airattack.svi/midres_tile_026.o obj/airattack.svi/midres_tile_027.o obj/airattack.svi/midres_tile_028.o obj/airattack.svi/midres_tile_multicolor_000.o obj/airattack.svi/midres_tile_multicolor_001.o obj/airattack.svi/midres_tile_multicolor_002.o obj/airattack.svi/midres_tile_multicolor_003.o obj/airattack.svi/midres_tile_multicolor_004.o obj/airattack.svi/midres_tile_multicolor_005.o obj/airattack.svi/midres_tile_multicolor_006.o obj/airattack.svi/midres_tile_multicolor_007.o obj/airattack.svi/midres_tile_multicolor_008.o obj/airattack.svi/midres_tile_multicolor_009.o obj/airattack.svi/midres_tile_multicolor_010.o obj/airattack.svi/midres_tile_multicolor_011.o obj/airattack.svi/midres_tile_multicolor_012.o obj/airattack.svi/midres_tile_processing_001.o obj/airattack.svi/midres_tile_processing_002.o obj/airattack.svi/midres_tile_processing_004.o obj/airattack.svi/midres_tile_processing_005.o obj/airattack.svi/midres_tile_processing_007.o obj/airattack.svi/midres_tile_processing_009.o obj/airattack.svi/midres_tile_processing_011.o obj/airattack.svi/midres_tile_processing_012.o obj/airattack.svi/midres_tile_processing_016.o obj/airattack.svi/midres_vanilla.o obj/airattack.svi/midres_vdp.o obj/airattack.svi/midres_vic.o obj/airattack.svi/midres_vic20.o obj/airattack.svi/utility_joycheck.o obj/airattack.svi/midres_control_011.o obj/airattack.svi/midres_data.o obj/airattack.svi/main.o -create-app 
	$(call COPYFILES,a.cas,$(EXEDIR)/airattack.svi.cas)
	$(call RMFILES,a.*)

# find_main_module(alienstorm)
# -> src/game_alien_storm.c

# -------------------------------------------------------------------
# --- ALIENSTORM FOR SVI 
# -------------------------------------------------------------------
# 

.PHONY: alienstorm.embedded.svi
alienstorm.embedded.svi:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/alienstorm.svi/rawdata.o src/rawdata.c

obj/alienstorm.svi/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.svi/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/alienstorm.svi/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.svi/midres_io.o src/midres_io.asm

obj/alienstorm.svi/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__ALIENSTORM__ -o $@ $(subst obj/alienstorm.svi/,src/,$(@:.o=.c)) 

$(EXEDIR)/alienstorm.svi:	alienstorm.embedded.svi obj/alienstorm.svi/demo_music.o obj/alienstorm.svi/demo_tile.o obj/alienstorm.svi/game_air_attack.o obj/alienstorm.svi/game_alien_storm.o obj/alienstorm.svi/game_elevator.o obj/alienstorm.svi/game_totto.o obj/alienstorm.svi/midres.o obj/alienstorm.svi/midres_atari.o obj/alienstorm.svi/midres_atmos.o obj/alienstorm.svi/midres_ay8910.o obj/alienstorm.svi/midres_c128.o obj/alienstorm.svi/midres_c16.o obj/alienstorm.svi/midres_c64.o obj/alienstorm.svi/midres_coleco.o obj/alienstorm.svi/midres_control_000.o obj/alienstorm.svi/midres_control_001.o obj/alienstorm.svi/midres_control_002.o obj/alienstorm.svi/midres_control_003.o obj/alienstorm.svi/midres_control_004.o obj/alienstorm.svi/midres_control_005.o obj/alienstorm.svi/midres_control_007.o obj/alienstorm.svi/midres_control_010.o obj/alienstorm.svi/midres_file_001.o obj/alienstorm.svi/midres_file_003.o obj/alienstorm.svi/midres_frame_000.o obj/alienstorm.svi/midres_frame_001.o obj/alienstorm.svi/midres_gb.o obj/alienstorm.svi/midres_lm80c.o obj/alienstorm.svi/midres_msx.o obj/alienstorm.svi/midres_mtx500.o obj/alienstorm.svi/midres_music_001.o obj/alienstorm.svi/midres_music_002.o obj/alienstorm.svi/midres_music_004.o obj/alienstorm.svi/midres_music_005.o obj/alienstorm.svi/midres_music_006.o obj/alienstorm.svi/midres_plus4.o obj/alienstorm.svi/midres_pokey.o obj/alienstorm.svi/midres_screen_000.o obj/alienstorm.svi/midres_screen_006.o obj/alienstorm.svi/midres_screen_007.o obj/alienstorm.svi/midres_screen_010.o obj/alienstorm.svi/midres_screen_012.o obj/alienstorm.svi/midres_screen_013.o obj/alienstorm.svi/midres_sid.o obj/alienstorm.svi/midres_sound_000.o obj/alienstorm.svi/midres_sound_001.o obj/alienstorm.svi/midres_sound_002.o obj/alienstorm.svi/midres_sound_005.o obj/alienstorm.svi/midres_sound_006.o obj/alienstorm.svi/midres_sound_007.o obj/alienstorm.svi/midres_svi.o obj/alienstorm.svi/midres_ted.o obj/alienstorm.svi/midres_tile_000.o obj/alienstorm.svi/midres_tile_004.o obj/alienstorm.svi/midres_tile_005.o obj/alienstorm.svi/midres_tile_006.o obj/alienstorm.svi/midres_tile_007.o obj/alienstorm.svi/midres_tile_008.o obj/alienstorm.svi/midres_tile_009.o obj/alienstorm.svi/midres_tile_010.o obj/alienstorm.svi/midres_tile_011.o obj/alienstorm.svi/midres_tile_012.o obj/alienstorm.svi/midres_tile_013.o obj/alienstorm.svi/midres_tile_014.o obj/alienstorm.svi/midres_tile_015.o obj/alienstorm.svi/midres_tile_016.o obj/alienstorm.svi/midres_tile_018.o obj/alienstorm.svi/midres_tile_019.o obj/alienstorm.svi/midres_tile_020.o obj/alienstorm.svi/midres_tile_021.o obj/alienstorm.svi/midres_tile_022.o obj/alienstorm.svi/midres_tile_023.o obj/alienstorm.svi/midres_tile_025.o obj/alienstorm.svi/midres_tile_026.o obj/alienstorm.svi/midres_tile_027.o obj/alienstorm.svi/midres_tile_028.o obj/alienstorm.svi/midres_tile_multicolor_000.o obj/alienstorm.svi/midres_tile_multicolor_001.o obj/alienstorm.svi/midres_tile_multicolor_002.o obj/alienstorm.svi/midres_tile_multicolor_003.o obj/alienstorm.svi/midres_tile_multicolor_004.o obj/alienstorm.svi/midres_tile_multicolor_005.o obj/alienstorm.svi/midres_tile_multicolor_006.o obj/alienstorm.svi/midres_tile_multicolor_007.o obj/alienstorm.svi/midres_tile_multicolor_008.o obj/alienstorm.svi/midres_tile_multicolor_009.o obj/alienstorm.svi/midres_tile_multicolor_010.o obj/alienstorm.svi/midres_tile_multicolor_011.o obj/alienstorm.svi/midres_tile_multicolor_012.o obj/alienstorm.svi/midres_tile_processing_001.o obj/alienstorm.svi/midres_tile_processing_002.o obj/alienstorm.svi/midres_tile_processing_004.o obj/alienstorm.svi/midres_tile_processing_005.o obj/alienstorm.svi/midres_tile_processing_007.o obj/alienstorm.svi/midres_tile_processing_009.o obj/alienstorm.svi/midres_tile_processing_011.o obj/alienstorm.svi/midres_tile_processing_012.o obj/alienstorm.svi/midres_tile_processing_016.o obj/alienstorm.svi/midres_vanilla.o obj/alienstorm.svi/midres_vdp.o obj/alienstorm.svi/midres_vic.o obj/alienstorm.svi/midres_vic20.o obj/alienstorm.svi/utility_joycheck.o obj/alienstorm.svi/midres_control_011.o obj/alienstorm.svi/midres_data.o obj/alienstorm.svi/main.o obj/alienstorm.svi/rawdata.o obj/alienstorm.svi/midres_vdp_impl.o obj/alienstorm.svi/midres_io.o
	$(CC88) +svi  -m $(LDFLAGS88) obj/alienstorm.svi/rawdata.o obj/alienstorm.svi/midres_io.o obj/alienstorm.svi/midres_vdp_impl.o obj/alienstorm.svi/demo_music.o obj/alienstorm.svi/demo_tile.o obj/alienstorm.svi/game_air_attack.o obj/alienstorm.svi/game_alien_storm.o obj/alienstorm.svi/game_elevator.o obj/alienstorm.svi/game_totto.o obj/alienstorm.svi/midres.o obj/alienstorm.svi/midres_atari.o obj/alienstorm.svi/midres_atmos.o obj/alienstorm.svi/midres_ay8910.o obj/alienstorm.svi/midres_c128.o obj/alienstorm.svi/midres_c16.o obj/alienstorm.svi/midres_c64.o obj/alienstorm.svi/midres_coleco.o obj/alienstorm.svi/midres_control_000.o obj/alienstorm.svi/midres_control_001.o obj/alienstorm.svi/midres_control_002.o obj/alienstorm.svi/midres_control_003.o obj/alienstorm.svi/midres_control_004.o obj/alienstorm.svi/midres_control_005.o obj/alienstorm.svi/midres_control_007.o obj/alienstorm.svi/midres_control_010.o obj/alienstorm.svi/midres_file_001.o obj/alienstorm.svi/midres_file_003.o obj/alienstorm.svi/midres_frame_000.o obj/alienstorm.svi/midres_frame_001.o obj/alienstorm.svi/midres_gb.o obj/alienstorm.svi/midres_lm80c.o obj/alienstorm.svi/midres_msx.o obj/alienstorm.svi/midres_mtx500.o obj/alienstorm.svi/midres_music_001.o obj/alienstorm.svi/midres_music_002.o obj/alienstorm.svi/midres_music_004.o obj/alienstorm.svi/midres_music_005.o obj/alienstorm.svi/midres_music_006.o obj/alienstorm.svi/midres_plus4.o obj/alienstorm.svi/midres_pokey.o obj/alienstorm.svi/midres_screen_000.o obj/alienstorm.svi/midres_screen_006.o obj/alienstorm.svi/midres_screen_007.o obj/alienstorm.svi/midres_screen_010.o obj/alienstorm.svi/midres_screen_012.o obj/alienstorm.svi/midres_screen_013.o obj/alienstorm.svi/midres_sid.o obj/alienstorm.svi/midres_sound_000.o obj/alienstorm.svi/midres_sound_001.o obj/alienstorm.svi/midres_sound_002.o obj/alienstorm.svi/midres_sound_005.o obj/alienstorm.svi/midres_sound_006.o obj/alienstorm.svi/midres_sound_007.o obj/alienstorm.svi/midres_svi.o obj/alienstorm.svi/midres_ted.o obj/alienstorm.svi/midres_tile_000.o obj/alienstorm.svi/midres_tile_004.o obj/alienstorm.svi/midres_tile_005.o obj/alienstorm.svi/midres_tile_006.o obj/alienstorm.svi/midres_tile_007.o obj/alienstorm.svi/midres_tile_008.o obj/alienstorm.svi/midres_tile_009.o obj/alienstorm.svi/midres_tile_010.o obj/alienstorm.svi/midres_tile_011.o obj/alienstorm.svi/midres_tile_012.o obj/alienstorm.svi/midres_tile_013.o obj/alienstorm.svi/midres_tile_014.o obj/alienstorm.svi/midres_tile_015.o obj/alienstorm.svi/midres_tile_016.o obj/alienstorm.svi/midres_tile_018.o obj/alienstorm.svi/midres_tile_019.o obj/alienstorm.svi/midres_tile_020.o obj/alienstorm.svi/midres_tile_021.o obj/alienstorm.svi/midres_tile_022.o obj/alienstorm.svi/midres_tile_023.o obj/alienstorm.svi/midres_tile_025.o obj/alienstorm.svi/midres_tile_026.o obj/alienstorm.svi/midres_tile_027.o obj/alienstorm.svi/midres_tile_028.o obj/alienstorm.svi/midres_tile_multicolor_000.o obj/alienstorm.svi/midres_tile_multicolor_001.o obj/alienstorm.svi/midres_tile_multicolor_002.o obj/alienstorm.svi/midres_tile_multicolor_003.o obj/alienstorm.svi/midres_tile_multicolor_004.o obj/alienstorm.svi/midres_tile_multicolor_005.o obj/alienstorm.svi/midres_tile_multicolor_006.o obj/alienstorm.svi/midres_tile_multicolor_007.o obj/alienstorm.svi/midres_tile_multicolor_008.o obj/alienstorm.svi/midres_tile_multicolor_009.o obj/alienstorm.svi/midres_tile_multicolor_010.o obj/alienstorm.svi/midres_tile_multicolor_011.o obj/alienstorm.svi/midres_tile_multicolor_012.o obj/alienstorm.svi/midres_tile_processing_001.o obj/alienstorm.svi/midres_tile_processing_002.o obj/alienstorm.svi/midres_tile_processing_004.o obj/alienstorm.svi/midres_tile_processing_005.o obj/alienstorm.svi/midres_tile_processing_007.o obj/alienstorm.svi/midres_tile_processing_009.o obj/alienstorm.svi/midres_tile_processing_011.o obj/alienstorm.svi/midres_tile_processing_012.o obj/alienstorm.svi/midres_tile_processing_016.o obj/alienstorm.svi/midres_vanilla.o obj/alienstorm.svi/midres_vdp.o obj/alienstorm.svi/midres_vic.o obj/alienstorm.svi/midres_vic20.o obj/alienstorm.svi/utility_joycheck.o obj/alienstorm.svi/midres_control_011.o obj/alienstorm.svi/midres_data.o obj/alienstorm.svi/main.o -create-app 
	$(call COPYFILES,a.cas,$(EXEDIR)/alienstorm.svi.cas)
	$(call RMFILES,a.*)

# find_main_module(elevator)
# -> src/game_elevator.c

# -------------------------------------------------------------------
# --- ELEVATOR FOR SVI 
# -------------------------------------------------------------------
# 

.PHONY: elevator.embedded.svi
elevator.embedded.svi:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator64.mpic -n zelintro.bin -i $(DATADIR)/elevator1.imf -n elevator1.imf -c src/rawdata.c -h src/rawdata.h
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/elevator.svi/rawdata.o src/rawdata.c

obj/elevator.svi/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.svi/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/elevator.svi/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.svi/midres_io.o src/midres_io.asm

obj/elevator.svi/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__ELEVATOR__ -o $@ $(subst obj/elevator.svi/,src/,$(@:.o=.c)) 

$(EXEDIR)/elevator.svi:	elevator.embedded.svi obj/elevator.svi/demo_music.o obj/elevator.svi/demo_tile.o obj/elevator.svi/game_air_attack.o obj/elevator.svi/game_alien_storm.o obj/elevator.svi/game_elevator.o obj/elevator.svi/game_totto.o obj/elevator.svi/midres.o obj/elevator.svi/midres_atari.o obj/elevator.svi/midres_atmos.o obj/elevator.svi/midres_ay8910.o obj/elevator.svi/midres_c128.o obj/elevator.svi/midres_c16.o obj/elevator.svi/midres_c64.o obj/elevator.svi/midres_coleco.o obj/elevator.svi/midres_control_000.o obj/elevator.svi/midres_control_001.o obj/elevator.svi/midres_control_002.o obj/elevator.svi/midres_control_003.o obj/elevator.svi/midres_control_004.o obj/elevator.svi/midres_control_005.o obj/elevator.svi/midres_control_007.o obj/elevator.svi/midres_control_010.o obj/elevator.svi/midres_file_001.o obj/elevator.svi/midres_file_003.o obj/elevator.svi/midres_frame_000.o obj/elevator.svi/midres_frame_001.o obj/elevator.svi/midres_gb.o obj/elevator.svi/midres_lm80c.o obj/elevator.svi/midres_msx.o obj/elevator.svi/midres_mtx500.o obj/elevator.svi/midres_music_001.o obj/elevator.svi/midres_music_002.o obj/elevator.svi/midres_music_004.o obj/elevator.svi/midres_music_005.o obj/elevator.svi/midres_music_006.o obj/elevator.svi/midres_plus4.o obj/elevator.svi/midres_pokey.o obj/elevator.svi/midres_screen_000.o obj/elevator.svi/midres_screen_006.o obj/elevator.svi/midres_screen_007.o obj/elevator.svi/midres_screen_010.o obj/elevator.svi/midres_screen_012.o obj/elevator.svi/midres_screen_013.o obj/elevator.svi/midres_sid.o obj/elevator.svi/midres_sound_000.o obj/elevator.svi/midres_sound_001.o obj/elevator.svi/midres_sound_002.o obj/elevator.svi/midres_sound_005.o obj/elevator.svi/midres_sound_006.o obj/elevator.svi/midres_sound_007.o obj/elevator.svi/midres_svi.o obj/elevator.svi/midres_ted.o obj/elevator.svi/midres_tile_000.o obj/elevator.svi/midres_tile_004.o obj/elevator.svi/midres_tile_005.o obj/elevator.svi/midres_tile_006.o obj/elevator.svi/midres_tile_007.o obj/elevator.svi/midres_tile_008.o obj/elevator.svi/midres_tile_009.o obj/elevator.svi/midres_tile_010.o obj/elevator.svi/midres_tile_011.o obj/elevator.svi/midres_tile_012.o obj/elevator.svi/midres_tile_013.o obj/elevator.svi/midres_tile_014.o obj/elevator.svi/midres_tile_015.o obj/elevator.svi/midres_tile_016.o obj/elevator.svi/midres_tile_018.o obj/elevator.svi/midres_tile_019.o obj/elevator.svi/midres_tile_020.o obj/elevator.svi/midres_tile_021.o obj/elevator.svi/midres_tile_022.o obj/elevator.svi/midres_tile_023.o obj/elevator.svi/midres_tile_025.o obj/elevator.svi/midres_tile_026.o obj/elevator.svi/midres_tile_027.o obj/elevator.svi/midres_tile_028.o obj/elevator.svi/midres_tile_multicolor_000.o obj/elevator.svi/midres_tile_multicolor_001.o obj/elevator.svi/midres_tile_multicolor_002.o obj/elevator.svi/midres_tile_multicolor_003.o obj/elevator.svi/midres_tile_multicolor_004.o obj/elevator.svi/midres_tile_multicolor_005.o obj/elevator.svi/midres_tile_multicolor_006.o obj/elevator.svi/midres_tile_multicolor_007.o obj/elevator.svi/midres_tile_multicolor_008.o obj/elevator.svi/midres_tile_multicolor_009.o obj/elevator.svi/midres_tile_multicolor_010.o obj/elevator.svi/midres_tile_multicolor_011.o obj/elevator.svi/midres_tile_multicolor_012.o obj/elevator.svi/midres_tile_processing_001.o obj/elevator.svi/midres_tile_processing_002.o obj/elevator.svi/midres_tile_processing_004.o obj/elevator.svi/midres_tile_processing_005.o obj/elevator.svi/midres_tile_processing_007.o obj/elevator.svi/midres_tile_processing_009.o obj/elevator.svi/midres_tile_processing_011.o obj/elevator.svi/midres_tile_processing_012.o obj/elevator.svi/midres_tile_processing_016.o obj/elevator.svi/midres_vanilla.o obj/elevator.svi/midres_vdp.o obj/elevator.svi/midres_vic.o obj/elevator.svi/midres_vic20.o obj/elevator.svi/utility_joycheck.o obj/elevator.svi/midres_control_011.o obj/elevator.svi/midres_data.o obj/elevator.svi/main.o obj/elevator.svi/rawdata.o obj/elevator.svi/midres_vdp_impl.o obj/elevator.svi/midres_io.o
	$(CC88) +svi  -m $(LDFLAGS88) obj/elevator.svi/rawdata.o obj/elevator.svi/midres_io.o obj/elevator.svi/midres_vdp_impl.o obj/elevator.svi/demo_music.o obj/elevator.svi/demo_tile.o obj/elevator.svi/game_air_attack.o obj/elevator.svi/game_alien_storm.o obj/elevator.svi/game_elevator.o obj/elevator.svi/game_totto.o obj/elevator.svi/midres.o obj/elevator.svi/midres_atari.o obj/elevator.svi/midres_atmos.o obj/elevator.svi/midres_ay8910.o obj/elevator.svi/midres_c128.o obj/elevator.svi/midres_c16.o obj/elevator.svi/midres_c64.o obj/elevator.svi/midres_coleco.o obj/elevator.svi/midres_control_000.o obj/elevator.svi/midres_control_001.o obj/elevator.svi/midres_control_002.o obj/elevator.svi/midres_control_003.o obj/elevator.svi/midres_control_004.o obj/elevator.svi/midres_control_005.o obj/elevator.svi/midres_control_007.o obj/elevator.svi/midres_control_010.o obj/elevator.svi/midres_file_001.o obj/elevator.svi/midres_file_003.o obj/elevator.svi/midres_frame_000.o obj/elevator.svi/midres_frame_001.o obj/elevator.svi/midres_gb.o obj/elevator.svi/midres_lm80c.o obj/elevator.svi/midres_msx.o obj/elevator.svi/midres_mtx500.o obj/elevator.svi/midres_music_001.o obj/elevator.svi/midres_music_002.o obj/elevator.svi/midres_music_004.o obj/elevator.svi/midres_music_005.o obj/elevator.svi/midres_music_006.o obj/elevator.svi/midres_plus4.o obj/elevator.svi/midres_pokey.o obj/elevator.svi/midres_screen_000.o obj/elevator.svi/midres_screen_006.o obj/elevator.svi/midres_screen_007.o obj/elevator.svi/midres_screen_010.o obj/elevator.svi/midres_screen_012.o obj/elevator.svi/midres_screen_013.o obj/elevator.svi/midres_sid.o obj/elevator.svi/midres_sound_000.o obj/elevator.svi/midres_sound_001.o obj/elevator.svi/midres_sound_002.o obj/elevator.svi/midres_sound_005.o obj/elevator.svi/midres_sound_006.o obj/elevator.svi/midres_sound_007.o obj/elevator.svi/midres_svi.o obj/elevator.svi/midres_ted.o obj/elevator.svi/midres_tile_000.o obj/elevator.svi/midres_tile_004.o obj/elevator.svi/midres_tile_005.o obj/elevator.svi/midres_tile_006.o obj/elevator.svi/midres_tile_007.o obj/elevator.svi/midres_tile_008.o obj/elevator.svi/midres_tile_009.o obj/elevator.svi/midres_tile_010.o obj/elevator.svi/midres_tile_011.o obj/elevator.svi/midres_tile_012.o obj/elevator.svi/midres_tile_013.o obj/elevator.svi/midres_tile_014.o obj/elevator.svi/midres_tile_015.o obj/elevator.svi/midres_tile_016.o obj/elevator.svi/midres_tile_018.o obj/elevator.svi/midres_tile_019.o obj/elevator.svi/midres_tile_020.o obj/elevator.svi/midres_tile_021.o obj/elevator.svi/midres_tile_022.o obj/elevator.svi/midres_tile_023.o obj/elevator.svi/midres_tile_025.o obj/elevator.svi/midres_tile_026.o obj/elevator.svi/midres_tile_027.o obj/elevator.svi/midres_tile_028.o obj/elevator.svi/midres_tile_multicolor_000.o obj/elevator.svi/midres_tile_multicolor_001.o obj/elevator.svi/midres_tile_multicolor_002.o obj/elevator.svi/midres_tile_multicolor_003.o obj/elevator.svi/midres_tile_multicolor_004.o obj/elevator.svi/midres_tile_multicolor_005.o obj/elevator.svi/midres_tile_multicolor_006.o obj/elevator.svi/midres_tile_multicolor_007.o obj/elevator.svi/midres_tile_multicolor_008.o obj/elevator.svi/midres_tile_multicolor_009.o obj/elevator.svi/midres_tile_multicolor_010.o obj/elevator.svi/midres_tile_multicolor_011.o obj/elevator.svi/midres_tile_multicolor_012.o obj/elevator.svi/midres_tile_processing_001.o obj/elevator.svi/midres_tile_processing_002.o obj/elevator.svi/midres_tile_processing_004.o obj/elevator.svi/midres_tile_processing_005.o obj/elevator.svi/midres_tile_processing_007.o obj/elevator.svi/midres_tile_processing_009.o obj/elevator.svi/midres_tile_processing_011.o obj/elevator.svi/midres_tile_processing_012.o obj/elevator.svi/midres_tile_processing_016.o obj/elevator.svi/midres_vanilla.o obj/elevator.svi/midres_vdp.o obj/elevator.svi/midres_vic.o obj/elevator.svi/midres_vic20.o obj/elevator.svi/utility_joycheck.o obj/elevator.svi/midres_control_011.o obj/elevator.svi/midres_data.o obj/elevator.svi/main.o -create-app 
	$(call COPYFILES,a.cas,$(EXEDIR)/elevator.svi.cas)
	$(call RMFILES,a.*)

                    
#!!! missing resources for joycheck (svi)

# find_main_module(totto)
# -> src/game_totto.c

# -------------------------------------------------------------------
# --- TOTTO FOR SVI 
# -------------------------------------------------------------------
# 

.PHONY: totto.embedded.svi
totto.embedded.svi:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -i $(DATADIR)/totto.imf -n totto.imf -i $(DATADIR)/totto3.imf -n totto3.imf -c src/rawdata.c -h src/rawdata.h
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/totto.svi/rawdata.o src/rawdata.c

obj/totto.svi/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.svi/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/totto.svi/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.svi/midres_io.o src/midres_io.asm

obj/totto.svi/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__TOTTO__ -o $@ $(subst obj/totto.svi/,src/,$(@:.o=.c)) 

$(EXEDIR)/totto.svi:	totto.embedded.svi obj/totto.svi/demo_music.o obj/totto.svi/demo_tile.o obj/totto.svi/game_air_attack.o obj/totto.svi/game_alien_storm.o obj/totto.svi/game_elevator.o obj/totto.svi/game_totto.o obj/totto.svi/midres.o obj/totto.svi/midres_atari.o obj/totto.svi/midres_atmos.o obj/totto.svi/midres_ay8910.o obj/totto.svi/midres_c128.o obj/totto.svi/midres_c16.o obj/totto.svi/midres_c64.o obj/totto.svi/midres_coleco.o obj/totto.svi/midres_control_000.o obj/totto.svi/midres_control_001.o obj/totto.svi/midres_control_002.o obj/totto.svi/midres_control_003.o obj/totto.svi/midres_control_004.o obj/totto.svi/midres_control_005.o obj/totto.svi/midres_control_007.o obj/totto.svi/midres_control_010.o obj/totto.svi/midres_file_001.o obj/totto.svi/midres_file_003.o obj/totto.svi/midres_frame_000.o obj/totto.svi/midres_frame_001.o obj/totto.svi/midres_gb.o obj/totto.svi/midres_lm80c.o obj/totto.svi/midres_msx.o obj/totto.svi/midres_mtx500.o obj/totto.svi/midres_music_001.o obj/totto.svi/midres_music_002.o obj/totto.svi/midres_music_004.o obj/totto.svi/midres_music_005.o obj/totto.svi/midres_music_006.o obj/totto.svi/midres_plus4.o obj/totto.svi/midres_pokey.o obj/totto.svi/midres_screen_000.o obj/totto.svi/midres_screen_006.o obj/totto.svi/midres_screen_007.o obj/totto.svi/midres_screen_010.o obj/totto.svi/midres_screen_012.o obj/totto.svi/midres_screen_013.o obj/totto.svi/midres_sid.o obj/totto.svi/midres_sound_000.o obj/totto.svi/midres_sound_001.o obj/totto.svi/midres_sound_002.o obj/totto.svi/midres_sound_005.o obj/totto.svi/midres_sound_006.o obj/totto.svi/midres_sound_007.o obj/totto.svi/midres_svi.o obj/totto.svi/midres_ted.o obj/totto.svi/midres_tile_000.o obj/totto.svi/midres_tile_004.o obj/totto.svi/midres_tile_005.o obj/totto.svi/midres_tile_006.o obj/totto.svi/midres_tile_007.o obj/totto.svi/midres_tile_008.o obj/totto.svi/midres_tile_009.o obj/totto.svi/midres_tile_010.o obj/totto.svi/midres_tile_011.o obj/totto.svi/midres_tile_012.o obj/totto.svi/midres_tile_013.o obj/totto.svi/midres_tile_014.o obj/totto.svi/midres_tile_015.o obj/totto.svi/midres_tile_016.o obj/totto.svi/midres_tile_018.o obj/totto.svi/midres_tile_019.o obj/totto.svi/midres_tile_020.o obj/totto.svi/midres_tile_021.o obj/totto.svi/midres_tile_022.o obj/totto.svi/midres_tile_023.o obj/totto.svi/midres_tile_025.o obj/totto.svi/midres_tile_026.o obj/totto.svi/midres_tile_027.o obj/totto.svi/midres_tile_028.o obj/totto.svi/midres_tile_multicolor_000.o obj/totto.svi/midres_tile_multicolor_001.o obj/totto.svi/midres_tile_multicolor_002.o obj/totto.svi/midres_tile_multicolor_003.o obj/totto.svi/midres_tile_multicolor_004.o obj/totto.svi/midres_tile_multicolor_005.o obj/totto.svi/midres_tile_multicolor_006.o obj/totto.svi/midres_tile_multicolor_007.o obj/totto.svi/midres_tile_multicolor_008.o obj/totto.svi/midres_tile_multicolor_009.o obj/totto.svi/midres_tile_multicolor_010.o obj/totto.svi/midres_tile_multicolor_011.o obj/totto.svi/midres_tile_multicolor_012.o obj/totto.svi/midres_tile_processing_001.o obj/totto.svi/midres_tile_processing_002.o obj/totto.svi/midres_tile_processing_004.o obj/totto.svi/midres_tile_processing_005.o obj/totto.svi/midres_tile_processing_007.o obj/totto.svi/midres_tile_processing_009.o obj/totto.svi/midres_tile_processing_011.o obj/totto.svi/midres_tile_processing_012.o obj/totto.svi/midres_tile_processing_016.o obj/totto.svi/midres_vanilla.o obj/totto.svi/midres_vdp.o obj/totto.svi/midres_vic.o obj/totto.svi/midres_vic20.o obj/totto.svi/utility_joycheck.o obj/totto.svi/midres_control_011.o obj/totto.svi/midres_data.o obj/totto.svi/main.o obj/totto.svi/rawdata.o obj/totto.svi/midres_vdp_impl.o obj/totto.svi/midres_io.o
	$(CC88) +svi  -m $(LDFLAGS88) obj/totto.svi/rawdata.o obj/totto.svi/midres_io.o obj/totto.svi/midres_vdp_impl.o obj/totto.svi/demo_music.o obj/totto.svi/demo_tile.o obj/totto.svi/game_air_attack.o obj/totto.svi/game_alien_storm.o obj/totto.svi/game_elevator.o obj/totto.svi/game_totto.o obj/totto.svi/midres.o obj/totto.svi/midres_atari.o obj/totto.svi/midres_atmos.o obj/totto.svi/midres_ay8910.o obj/totto.svi/midres_c128.o obj/totto.svi/midres_c16.o obj/totto.svi/midres_c64.o obj/totto.svi/midres_coleco.o obj/totto.svi/midres_control_000.o obj/totto.svi/midres_control_001.o obj/totto.svi/midres_control_002.o obj/totto.svi/midres_control_003.o obj/totto.svi/midres_control_004.o obj/totto.svi/midres_control_005.o obj/totto.svi/midres_control_007.o obj/totto.svi/midres_control_010.o obj/totto.svi/midres_file_001.o obj/totto.svi/midres_file_003.o obj/totto.svi/midres_frame_000.o obj/totto.svi/midres_frame_001.o obj/totto.svi/midres_gb.o obj/totto.svi/midres_lm80c.o obj/totto.svi/midres_msx.o obj/totto.svi/midres_mtx500.o obj/totto.svi/midres_music_001.o obj/totto.svi/midres_music_002.o obj/totto.svi/midres_music_004.o obj/totto.svi/midres_music_005.o obj/totto.svi/midres_music_006.o obj/totto.svi/midres_plus4.o obj/totto.svi/midres_pokey.o obj/totto.svi/midres_screen_000.o obj/totto.svi/midres_screen_006.o obj/totto.svi/midres_screen_007.o obj/totto.svi/midres_screen_010.o obj/totto.svi/midres_screen_012.o obj/totto.svi/midres_screen_013.o obj/totto.svi/midres_sid.o obj/totto.svi/midres_sound_000.o obj/totto.svi/midres_sound_001.o obj/totto.svi/midres_sound_002.o obj/totto.svi/midres_sound_005.o obj/totto.svi/midres_sound_006.o obj/totto.svi/midres_sound_007.o obj/totto.svi/midres_svi.o obj/totto.svi/midres_ted.o obj/totto.svi/midres_tile_000.o obj/totto.svi/midres_tile_004.o obj/totto.svi/midres_tile_005.o obj/totto.svi/midres_tile_006.o obj/totto.svi/midres_tile_007.o obj/totto.svi/midres_tile_008.o obj/totto.svi/midres_tile_009.o obj/totto.svi/midres_tile_010.o obj/totto.svi/midres_tile_011.o obj/totto.svi/midres_tile_012.o obj/totto.svi/midres_tile_013.o obj/totto.svi/midres_tile_014.o obj/totto.svi/midres_tile_015.o obj/totto.svi/midres_tile_016.o obj/totto.svi/midres_tile_018.o obj/totto.svi/midres_tile_019.o obj/totto.svi/midres_tile_020.o obj/totto.svi/midres_tile_021.o obj/totto.svi/midres_tile_022.o obj/totto.svi/midres_tile_023.o obj/totto.svi/midres_tile_025.o obj/totto.svi/midres_tile_026.o obj/totto.svi/midres_tile_027.o obj/totto.svi/midres_tile_028.o obj/totto.svi/midres_tile_multicolor_000.o obj/totto.svi/midres_tile_multicolor_001.o obj/totto.svi/midres_tile_multicolor_002.o obj/totto.svi/midres_tile_multicolor_003.o obj/totto.svi/midres_tile_multicolor_004.o obj/totto.svi/midres_tile_multicolor_005.o obj/totto.svi/midres_tile_multicolor_006.o obj/totto.svi/midres_tile_multicolor_007.o obj/totto.svi/midres_tile_multicolor_008.o obj/totto.svi/midres_tile_multicolor_009.o obj/totto.svi/midres_tile_multicolor_010.o obj/totto.svi/midres_tile_multicolor_011.o obj/totto.svi/midres_tile_multicolor_012.o obj/totto.svi/midres_tile_processing_001.o obj/totto.svi/midres_tile_processing_002.o obj/totto.svi/midres_tile_processing_004.o obj/totto.svi/midres_tile_processing_005.o obj/totto.svi/midres_tile_processing_007.o obj/totto.svi/midres_tile_processing_009.o obj/totto.svi/midres_tile_processing_011.o obj/totto.svi/midres_tile_processing_012.o obj/totto.svi/midres_tile_processing_016.o obj/totto.svi/midres_vanilla.o obj/totto.svi/midres_vdp.o obj/totto.svi/midres_vic.o obj/totto.svi/midres_vic20.o obj/totto.svi/utility_joycheck.o obj/totto.svi/midres_control_011.o obj/totto.svi/midres_data.o obj/totto.svi/main.o -create-app 
	$(call COPYFILES,a.cas,$(EXEDIR)/totto.svi.cas)
	$(call RMFILES,a.*)


# -------------------------------------------------------------------
# --- AIRATTACK FOR VIC20 
# -------------------------------------------------------------------

.PHONY: airattack.embedded.vic20
airattack.embedded.vic20:
	$(FILE2INCLUDE) -i $(DATADIR)/aatiles20.bin -n zztiles.bin -i $(DATADIR)/aaintro20.mpic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h

obj/airattack.vic20/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS)  -Osir -Cl -D__AIRATTACK__ -D__CBM__ -o $@ $(subst obj/airattack.vic20/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.vic20: airattack.embedded.vic20 $(subst PLATFORM,airattack.vic20,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic20.cfg $(LDFLAGS) -m $(EXEDIR)/airattack.vic20.map -o $(EXEDIR)/airattack.vic20  $(subst PLATFORM,airattack.vic20,$(OBJS)) $(LIBDIR)/midres.vic20.lib
	$(call RMFILES,$(EXEDIR)/airattack.vic20.d64)
	$(CC1541) -f airattack -w $(EXEDIR)/airattack.vic20 $(EXEDIR)/airattack.vic20.d64 
	$(CC1541) -f  -w  $(EXEDIR)/airattack.vic20.d64 
	$(CC1541) -f zztiles.bin -w $(DATADIR)/aatiles20.bin $(EXEDIR)/airattack.vic20.d64 
	$(CC1541) -f zzintro.pic -w $(DATADIR)/aaintro20.mpic $(EXEDIR)/airattack.vic20.d64 


# -------------------------------------------------------------------
# --- ALIENSTORM FOR VIC20 
# -------------------------------------------------------------------

.PHONY: alienstorm.embedded.vic20
alienstorm.embedded.vic20:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h

obj/alienstorm.vic20/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS)  -Osir -Cl -D__ALIENSTORM__ -D__CBM__ -o $@ $(subst obj/alienstorm.vic20/,src/,$(@:.o=.c))

$(EXEDIR)/alienstorm.vic20: alienstorm.embedded.vic20 $(subst PLATFORM,alienstorm.vic20,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic20.cfg $(LDFLAGS) -m $(EXEDIR)/alienstorm.vic20.map -o $(EXEDIR)/alienstorm.vic20  $(subst PLATFORM,alienstorm.vic20,$(OBJS)) $(LIBDIR)/midres.vic20.lib
	$(call RMFILES,$(EXEDIR)/alienstorm.vic20.d64)
	$(CC1541) -f alienstorm -w $(EXEDIR)/alienstorm.vic20 $(EXEDIR)/alienstorm.vic20.d64 
	$(CC1541) -f zstiles.bin -w $(DATADIR)/astiles.bin $(EXEDIR)/alienstorm.vic20.d64 
	$(CC1541) -f zstiles1.bin -w $(DATADIR)/astiles1.bin $(EXEDIR)/alienstorm.vic20.d64 
	$(CC1541) -f zstiles2.bin -w $(DATADIR)/astiles2.bin $(EXEDIR)/alienstorm.vic20.d64 


# -------------------------------------------------------------------
# --- ELEVATOR FOR VIC20 
# -------------------------------------------------------------------

.PHONY: elevator.embedded.vic20
elevator.embedded.vic20:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator20.mpic -n zelintro.bin -i $(DATADIR)/elevator1.imf -n elevator1.imf -c src/rawdata.c -h src/rawdata.h

obj/elevator.vic20/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS)  -Osir -Cl -D__ELEVATOR__ -D__CBM__ -o $@ $(subst obj/elevator.vic20/,src/,$(@:.o=.c))

$(EXEDIR)/elevator.vic20: elevator.embedded.vic20 $(subst PLATFORM,elevator.vic20,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic20.cfg $(LDFLAGS) -m $(EXEDIR)/elevator.vic20.map -o $(EXEDIR)/elevator.vic20  $(subst PLATFORM,elevator.vic20,$(OBJS)) $(LIBDIR)/midres.vic20.lib
	$(call RMFILES,$(EXEDIR)/elevator.vic20.d64)
	$(CC1541) -f elevator -w $(EXEDIR)/elevator.vic20 $(EXEDIR)/elevator.vic20.d64 
	$(CC1541) -f  -w  $(EXEDIR)/elevator.vic20.d64 
	$(CC1541) -f zeltiles.bin -w $(DATADIR)/zeltiles.bin $(EXEDIR)/elevator.vic20.d64 
	$(CC1541) -f zelintro.bin -w $(DATADIR)/elevator20.mpic $(EXEDIR)/elevator.vic20.d64 
	$(CC1541) -f elevator1.imf -w $(DATADIR)/elevator1.imf $(EXEDIR)/elevator.vic20.d64 

                    
#!!! missing resources for joycheck (vic20)


# -------------------------------------------------------------------
# --- TOTTO FOR VIC20 
# -------------------------------------------------------------------

.PHONY: totto.embedded.vic20
totto.embedded.vic20:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -i $(DATADIR)/ttfinal120.mpic -n ttfinal1.mpic -i $(DATADIR)/ttfinal220.mpic -n ttfinal2.mpic -i $(DATADIR)/ttfinal320.mpic -n ttfinal3.mpic -i $(DATADIR)/ttfinal420.mpic -n ttfinal4.mpic -i $(DATADIR)/totto.imf -n totto.imf -i $(DATADIR)/totto3.imf -n totto3.imf -c src/rawdata.c -h src/rawdata.h

obj/totto.vic20/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS)  -Osir -Cl -D__TOTTO__ -D__CBM__ -o $@ $(subst obj/totto.vic20/,src/,$(@:.o=.c))

$(EXEDIR)/totto.vic20: totto.embedded.vic20 $(subst PLATFORM,totto.vic20,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic20.cfg $(LDFLAGS) -m $(EXEDIR)/totto.vic20.map -o $(EXEDIR)/totto.vic20  $(subst PLATFORM,totto.vic20,$(OBJS)) $(LIBDIR)/midres.vic20.lib
	$(call RMFILES,$(EXEDIR)/totto.vic20.d64)
	$(CC1541) -f totto -w $(EXEDIR)/totto.vic20 $(EXEDIR)/totto.vic20.d64 
	$(CC1541) -f  -w  $(EXEDIR)/totto.vic20.d64 
	$(CC1541) -f tttiles.bin -w $(DATADIR)/tttiles.bin $(EXEDIR)/totto.vic20.d64 
	$(CC1541) -f tttiles1.bin -w $(DATADIR)/tttiles1.bin $(EXEDIR)/totto.vic20.d64 
	$(CC1541) -f ttfinal1.mpic -w $(DATADIR)/ttfinal120.mpic $(EXEDIR)/totto.vic20.d64 
	$(CC1541) -f ttfinal2.mpic -w $(DATADIR)/ttfinal220.mpic $(EXEDIR)/totto.vic20.d64 
	$(CC1541) -f ttfinal3.mpic -w $(DATADIR)/ttfinal320.mpic $(EXEDIR)/totto.vic20.d64 
	$(CC1541) -f ttfinal4.mpic -w $(DATADIR)/ttfinal420.mpic $(EXEDIR)/totto.vic20.d64 
	$(CC1541) -f totto.imf -w $(DATADIR)/totto.imf $(EXEDIR)/totto.vic20.d64 
	$(CC1541) -f totto3.imf -w $(DATADIR)/totto3.imf $(EXEDIR)/totto.vic20.d64 


# -------------------------------------------------------------------
# --- AIRATTACK FOR VIC2024 
# -------------------------------------------------------------------

.PHONY: airattack.embedded.vic2024
airattack.embedded.vic2024:
	$(FILE2INCLUDE) -i $(DATADIR)/aatiles20.bin -n zztiles.bin -i $(DATADIR)/aaintro20.mpic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h

obj/airattack.vic2024/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS) -D__24K__ -Osir -Cl -D__AIRATTACK__ -D__CBM__ -o $@ $(subst obj/airattack.vic2024/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.vic2024: airattack.embedded.vic2024 $(subst PLATFORM,airattack.vic2024,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic2024.cfg $(LDFLAGS) -m $(EXEDIR)/airattack.vic20.map -o $(EXEDIR)/airattack.vic2024  $(subst PLATFORM,airattack.vic2024,$(OBJS)) $(LIBDIR)/midres.vic2024.lib
	$(call RMFILES,$(EXEDIR)/airattack.vic2024.d64)
	$(CC1541) -f loader -w $(DATADIR)/airattack_loader2024.prg $(EXEDIR)/airattack.vic2024.d64 
	$(CC1541) -f airattack -w $(EXEDIR)/airattack.vic2024 $(EXEDIR)/airattack.vic2024.d64 
	$(CC1541) -f zztiles.bin -w $(DATADIR)/aatiles20.bin $(EXEDIR)/airattack.vic2024.d64 
	$(CC1541) -f zzintro.pic -w $(DATADIR)/aaintro20.mpic $(EXEDIR)/airattack.vic2024.d64 


# -------------------------------------------------------------------
# --- ALIENSTORM FOR VIC2024 
# -------------------------------------------------------------------

.PHONY: alienstorm.embedded.vic2024
alienstorm.embedded.vic2024:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h

obj/alienstorm.vic2024/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS) -D__24K__ -Osir -Cl -D__ALIENSTORM__ -D__CBM__ -o $@ $(subst obj/alienstorm.vic2024/,src/,$(@:.o=.c))

$(EXEDIR)/alienstorm.vic2024: alienstorm.embedded.vic2024 $(subst PLATFORM,alienstorm.vic2024,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic2024.cfg $(LDFLAGS) -m $(EXEDIR)/alienstorm.vic20.map -o $(EXEDIR)/alienstorm.vic2024  $(subst PLATFORM,alienstorm.vic2024,$(OBJS)) $(LIBDIR)/midres.vic2024.lib
	$(call RMFILES,$(EXEDIR)/alienstorm.vic2024.d64)
	$(CC1541) -f loader -w $(DATADIR)/asloader2024.prg $(EXEDIR)/alienstorm.vic2024.d64 
	$(CC1541) -f alienstorm -w $(EXEDIR)/alienstorm.vic2024 $(EXEDIR)/alienstorm.vic2024.d64 
	$(CC1541) -f zstiles.bin -w $(DATADIR)/astiles.bin $(EXEDIR)/alienstorm.vic2024.d64 
	$(CC1541) -f zstiles1.bin -w $(DATADIR)/astiles1.bin $(EXEDIR)/alienstorm.vic2024.d64 
	$(CC1541) -f zstiles2.bin -w $(DATADIR)/astiles2.bin $(EXEDIR)/alienstorm.vic2024.d64 


# -------------------------------------------------------------------
# --- ELEVATOR FOR VIC2024 
# -------------------------------------------------------------------

.PHONY: elevator.embedded.vic2024
elevator.embedded.vic2024:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator20.mpic -n zelintro.bin -i $(DATADIR)/elevator1.imf -n elevator1.imf -c src/rawdata.c -h src/rawdata.h

obj/elevator.vic2024/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS) -D__24K__ -Osir -Cl -D__ELEVATOR__ -D__CBM__ -o $@ $(subst obj/elevator.vic2024/,src/,$(@:.o=.c))

$(EXEDIR)/elevator.vic2024: elevator.embedded.vic2024 $(subst PLATFORM,elevator.vic2024,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic2024.cfg $(LDFLAGS) -m $(EXEDIR)/elevator.vic20.map -o $(EXEDIR)/elevator.vic2024  $(subst PLATFORM,elevator.vic2024,$(OBJS)) $(LIBDIR)/midres.vic2024.lib
	$(call RMFILES,$(EXEDIR)/elevator.vic2024.d64)
	$(CC1541) -f loader -w $(DATADIR)/elloader2024.prg $(EXEDIR)/elevator.vic2024.d64 
	$(CC1541) -f elevator -w $(EXEDIR)/elevator.vic2024 $(EXEDIR)/elevator.vic2024.d64 
	$(CC1541) -f zeltiles.bin -w $(DATADIR)/zeltiles.bin $(EXEDIR)/elevator.vic2024.d64 
	$(CC1541) -f zelintro.bin -w $(DATADIR)/elevator20.mpic $(EXEDIR)/elevator.vic2024.d64 
	$(CC1541) -f elevator1.imf -w $(DATADIR)/elevator1.imf $(EXEDIR)/elevator.vic2024.d64 


# -------------------------------------------------------------------
# --- JOYCHECK FOR VIC2024 
# -------------------------------------------------------------------

.PHONY: joycheck.embedded.vic2024
joycheck.embedded.vic2024:
	$(FILE2INCLUDE) -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -c src/rawdata.c -h src/rawdata.h

obj/joycheck.vic2024/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS) -D__24K__ -Osir -Cl -D__JOYCHECK__ -D__CBM__ -o $@ $(subst obj/joycheck.vic2024/,src/,$(@:.o=.c))

$(EXEDIR)/joycheck.vic2024: joycheck.embedded.vic2024 $(subst PLATFORM,joycheck.vic2024,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic2024.cfg $(LDFLAGS) -m $(EXEDIR)/joycheck.vic20.map -o $(EXEDIR)/joycheck.vic2024  $(subst PLATFORM,joycheck.vic2024,$(OBJS)) $(LIBDIR)/midres.vic2024.lib
	$(call RMFILES,$(EXEDIR)/joycheck.vic2024.d64)
	$(CC1541) -f loader -w $(DATADIR)/jcloader2024.prg $(EXEDIR)/joycheck.vic2024.d64 
	$(CC1541) -f joycheck -w $(EXEDIR)/joycheck.vic2024 $(EXEDIR)/joycheck.vic2024.d64 
	$(CC1541) -f zdjtiles.bin -w $(DATADIR)/zdjtiles.bin $(EXEDIR)/joycheck.vic2024.d64 


# -------------------------------------------------------------------
# --- TOTTO FOR VIC2024 
# -------------------------------------------------------------------

.PHONY: totto.embedded.vic2024
totto.embedded.vic2024:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -i $(DATADIR)/ttfinal120.mpic -n ttfinal1.mpic -i $(DATADIR)/ttfinal220.mpic -n ttfinal2.mpic -i $(DATADIR)/ttfinal320.mpic -n ttfinal3.mpic -i $(DATADIR)/ttfinal420.mpic -n ttfinal4.mpic -i $(DATADIR)/totto.imf -n totto.imf -i $(DATADIR)/totto3.imf -n totto3.imf -c src/rawdata.c -h src/rawdata.h

obj/totto.vic2024/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS) -D__24K__ -Osir -Cl -D__TOTTO__ -D__CBM__ -o $@ $(subst obj/totto.vic2024/,src/,$(@:.o=.c))

$(EXEDIR)/totto.vic2024: totto.embedded.vic2024 $(subst PLATFORM,totto.vic2024,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic2024.cfg $(LDFLAGS) -m $(EXEDIR)/totto.vic20.map -o $(EXEDIR)/totto.vic2024  $(subst PLATFORM,totto.vic2024,$(OBJS)) $(LIBDIR)/midres.vic2024.lib
	$(call RMFILES,$(EXEDIR)/totto.vic2024.d64)
	$(CC1541) -f loader -w $(DATADIR)/totto_loader2024.prg $(EXEDIR)/totto.vic2024.d64 
	$(CC1541) -f totto -w $(EXEDIR)/totto.vic2024 $(EXEDIR)/totto.vic2024.d64 
	$(CC1541) -f tttiles.bin -w $(DATADIR)/tttiles.bin $(EXEDIR)/totto.vic2024.d64 
	$(CC1541) -f tttiles1.bin -w $(DATADIR)/tttiles1.bin $(EXEDIR)/totto.vic2024.d64 
	$(CC1541) -f ttfinal1.mpic -w $(DATADIR)/ttfinal120.mpic $(EXEDIR)/totto.vic2024.d64 
	$(CC1541) -f ttfinal2.mpic -w $(DATADIR)/ttfinal220.mpic $(EXEDIR)/totto.vic2024.d64 
	$(CC1541) -f ttfinal3.mpic -w $(DATADIR)/ttfinal320.mpic $(EXEDIR)/totto.vic2024.d64 
	$(CC1541) -f ttfinal4.mpic -w $(DATADIR)/ttfinal420.mpic $(EXEDIR)/totto.vic2024.d64 
	$(CC1541) -f totto.imf -w $(DATADIR)/totto.imf $(EXEDIR)/totto.vic2024.d64 
	$(CC1541) -f totto3.imf -w $(DATADIR)/totto3.imf $(EXEDIR)/totto.vic2024.d64 

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
