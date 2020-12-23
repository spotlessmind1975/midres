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
else
  CC := cl65
  AR := ar65
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
CFLAGS88 := -D__Z80__
LDFLAGS := 
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
EXES := $(foreach TARGET,$(TARGETS),$(EXEDIR)/midres.$(TARGET))
EXES += $(foreach ATARGET,$(ATARGETS),$(EXEDIR)/$(ATARGET))

LIBS := $(foreach TARGET,$(TARGETS),$(LIBDIR)/midres.$(TARGET).lib)
LIBS += $(foreach TARGET,$(LTARGETS),$(LIBDIR)/midres.$(TARGET).lib)

# This is the path where all data will be put.
DATADIR := data
<?php
?>