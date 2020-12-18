# /****************************************************************************
#  * midres - Portable midres library for retrocomputers                      *
#  *                                                                          *
#  * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
#  *--------------------------------------------------------------------------*
#  * MAKEFILE                                                                 *
#  ****************************************************************************/
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

# Given GAMES:
#  - totto
#  - airattack
#  - alienstorm
#  - elevator

ifdef game
ifdef target
ATARGETS ?= $(game).$(target)
LTARGETS ?= $(target)
endif
endif

# Given UTILITY:
#  - joycheck

ifdef utility
ifdef target
ATARGETS ?= $(utility).$(target)
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

###############################################################################
## PLATFORMS' RULES
###############################################################################

# Makefile generated for following platforms: msx,msxc,svi,mtx500,coleco,atmos,atari,vic20,vic2024,c64,c128,c16,plus4
# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR MSX 
# -------------------------------------------------------------------

lib/midres.msx.lib:

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR MSX 
# -------------------------------------------------------------------

midres.embedded.msx:
	$(FILE2INCLUDE) -i $(DATADIR)/mtiles.bin -n mtiles.bin -i $(DATADIR)/tiles.bin -n tiles.bin -i $(DATADIR)/tutorial_mctile.bin -n tutorial_mctile.bin -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/msx/rawdata.o src/rawdata.c

obj/msx/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/msx/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/msx/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/msx/midres_io.o src/midres_io.asm

obj/msx/%.o:	$(SOURCES)
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o $@ $(subst obj/msx/,src/,$(@:.o=.c))

$(EXEDIR)/midres.msx:	midres.embedded.msx $(subst PLATFORM,msx,$(OBJS)) $(subst PLATFORM,msx,$(LIB_OBJS)) obj/msx/rawdata.o obj/msx/midres_vdp_impl.o obj/msx/midres_io.o
	$(CC88) +msx -subtype=rom -m $(LDFLAGS88) obj/msx/rawdata.o obj/msx/midres_io.o obj/msx/midres_vdp_impl.o $(subst PLATFORM,msx,$(LIB_OBJS)) $(subst PLATFORM,msx,$(OBJS)) -o $(EXEDIR)/midres.msx -create-app 
	$(call COPYFILES,$(EXEDIR)/midres.rom,$(EXEDIR)/midres.msx.rom)

# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR MSXC 
# -------------------------------------------------------------------

lib/midres.msxc.lib:

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR MSXC 
# -------------------------------------------------------------------

midres.embedded.msxc:
	$(FILE2INCLUDE) -i $(DATADIR)/mtiles.bin -n mtiles.bin -i $(DATADIR)/tiles.bin -n tiles.bin -i $(DATADIR)/tutorial_mctile.bin -n tutorial_mctile.bin -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_II -o obj/msxc/rawdata.o src/rawdata.c

obj/msxc/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/msxc/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/msxc/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/msxc/midres_io.o src/midres_io.asm

obj/msxc/%.o:	$(SOURCES)
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_II -o $@ $(subst obj/msxc/,src/,$(@:.o=.c))

$(EXEDIR)/midres.msxc:	midres.embedded.msxc $(subst PLATFORM,msxc,$(OBJS)) $(subst PLATFORM,msxc,$(LIB_OBJS)) obj/msxc/rawdata.o obj/msxc/midres_vdp_impl.o obj/msxc/midres_io.o
	$(CC88) +msx -subtype=rom -m $(LDFLAGS88) obj/msxc/rawdata.o obj/msxc/midres_io.o obj/msxc/midres_vdp_impl.o $(subst PLATFORM,msxc,$(LIB_OBJS)) $(subst PLATFORM,msxc,$(OBJS)) -o $(EXEDIR)/midres.msxc -create-app 
	$(call COPYFILES,$(EXEDIR)/midres.rom,$(EXEDIR)/midres.msxc.rom)

# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR SVI 
# -------------------------------------------------------------------

lib/midres.svi.lib:

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR SVI 
# -------------------------------------------------------------------

midres.embedded.svi:
	$(FILE2INCLUDE) -i $(DATADIR)/mtiles.bin -n mtiles.bin -i $(DATADIR)/tiles.bin -n tiles.bin -i $(DATADIR)/tutorial_mctile.bin -n tutorial_mctile.bin -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/svi/rawdata.o src/rawdata.c

obj/svi/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/svi/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/svi/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/svi/midres_io.o src/midres_io.asm

obj/svi/%.o:	$(SOURCES)
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o $@ $(subst obj/svi/,src/,$(@:.o=.c))

$(EXEDIR)/midres.svi:	midres.embedded.svi $(subst PLATFORM,svi,$(OBJS)) $(subst PLATFORM,svi,$(LIB_OBJS)) obj/svi/rawdata.o obj/svi/midres_vdp_impl.o obj/svi/midres_io.o
	$(CC88) +svi  -m $(LDFLAGS88) obj/svi/rawdata.o obj/svi/midres_io.o obj/svi/midres_vdp_impl.o $(subst PLATFORM,svi,$(LIB_OBJS)) $(subst PLATFORM,svi,$(OBJS)) -o $(EXEDIR)/midres.svi -create-app 
	$(call COPYFILES,$(EXEDIR)/midres.cas,$(EXEDIR)/midres.svi.cas)

# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR MTX500 
# -------------------------------------------------------------------

lib/midres.mtx500.lib:

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR MTX500 
# -------------------------------------------------------------------

midres.embedded.mtx500:
	$(FILE2INCLUDE) -i $(DATADIR)/mtiles.bin -n mtiles.bin -i $(DATADIR)/tiles.bin -n tiles.bin -i $(DATADIR)/tutorial_mctile.bin -n tutorial_mctile.bin -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/mtx500/rawdata.o src/rawdata.c

obj/mtx500/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/mtx500/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/mtx500/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/mtx500/midres_io.o src/midres_io.asm

obj/mtx500/%.o:	$(SOURCES)
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o $@ $(subst obj/mtx500/,src/,$(@:.o=.c))

$(EXEDIR)/midres.mtx500:	midres.embedded.mtx500 $(subst PLATFORM,mtx500,$(OBJS)) $(subst PLATFORM,mtx500,$(LIB_OBJS)) obj/mtx500/rawdata.o obj/mtx500/midres_vdp_impl.o obj/mtx500/midres_io.o
	$(CC88) +mtx  -m $(LDFLAGS88) obj/mtx500/rawdata.o obj/mtx500/midres_io.o obj/mtx500/midres_vdp_impl.o $(subst PLATFORM,mtx500,$(LIB_OBJS)) $(subst PLATFORM,mtx500,$(OBJS)) -o $(EXEDIR)/midres.mtx500 -create-app 
	$(call COPYFILES,$(EXEDIR)/midres.mtx500.cas,$(EXEDIR)/midres.mtx500.mtx)

# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR COLECO 
# -------------------------------------------------------------------

lib/midres.coleco.lib:

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR COLECO 
# -------------------------------------------------------------------

midres.embedded.coleco:
	$(FILE2INCLUDE) -i $(DATADIR)/mtiles.bin -n mtiles.bin -i $(DATADIR)/tiles.bin -n tiles.bin -i $(DATADIR)/tutorial_mctile.bin -n tutorial_mctile.bin -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/coleco/rawdata.o src/rawdata.c

obj/coleco/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/coleco/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/coleco/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/coleco/midres_io.o src/midres_io.asm

obj/coleco/%.o:	$(SOURCES)
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o $@ $(subst obj/coleco/,src/,$(@:.o=.c))

$(EXEDIR)/midres.coleco:	midres.embedded.coleco $(subst PLATFORM,coleco,$(OBJS)) $(subst PLATFORM,coleco,$(LIB_OBJS)) obj/coleco/rawdata.o obj/coleco/midres_vdp_impl.o obj/coleco/midres_io.o
	$(CC88) +coleco  -m $(LDFLAGS88) obj/coleco/rawdata.o obj/coleco/midres_io.o obj/coleco/midres_vdp_impl.o $(subst PLATFORM,coleco,$(LIB_OBJS)) $(subst PLATFORM,coleco,$(OBJS)) -o $(EXEDIR)/midres.coleco -create-app 
	$(call COPYFILES,$(EXEDIR)/midres.rom,$(EXEDIR)/midres.coleco.rom)

# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR ATMOS 
# -------------------------------------------------------------------

obj/atmos/%.o:	$(LIB_INCLUDES) $(LIB_SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t atmos -c $(CFLAGS)  -Osir -Cl   -o $@ $(subst obj/atmos/,src/,$(@:.o=.c))

$(LIBDIR)/midres.atmos.lib:	$(LIB_INCLUDES) $(subst PLATFORM,atmos,$(LIB_OBJS))
	$(AR) r $(LIBDIR)/midres.atmos.lib $(subst PLATFORM,atmos,$(LIB_OBJS))


# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR ATARI 
# -------------------------------------------------------------------

obj/atari/%.o:	$(LIB_INCLUDES) $(LIB_SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t atari -c $(CFLAGS)  -Osir -Cl   -o $@ $(subst obj/atari/,src/,$(@:.o=.c))

$(LIBDIR)/midres.atari.lib:	$(LIB_INCLUDES) $(subst PLATFORM,atari,$(LIB_OBJS))
	$(AR) r $(LIBDIR)/midres.atari.lib $(subst PLATFORM,atari,$(LIB_OBJS))

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR ATARI 
# -------------------------------------------------------------------

obj/atari/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t atari -c $(CFLAGS)  -Osir -Cl  -o $@ $(subst obj/atari/,src/,$(@:.o=.c))

$(EXEDIR)/midres.atari: $(subst PLATFORM,atari,$(OBJS))
	$(CC) -Ln demoatari.lbl -t atari -C cfg/atari.cfg  $(LDFLAGS) -m $(EXEDIR)/midres.atari.map -o $(EXEDIR)/midres.atari $(subst PLATFORM,atari,$(OBJS)) $(LIBDIR)/midres.atari.lib
	$(call RMFILES,$(EXEDIR)/atr/*.*)
	$(call COPYFILES,$(DIR2ATR_HOME)/dos25/dos.sys,$(EXEDIR)/atr/dos.sys)
	$(call COPYFILES,$(EXEDIR)/midres.exe,$(EXEDIR)/atr/midres-single)
	$(call COPYFILES,$(DATADIR)/slideshowa.dat,$(EXEDIR)/atr/slideshow)
	$(call COPYFILES,$(DATADIR)/imagea01.mpic,$(EXEDIR)/atr/imagea01.mpic)
	$(call COPYFILES,$(DATADIR)/imagea02.mpic,$(EXEDIR)/atr/imagea02.mpic)
	$(call COPYFILES,$(DATADIR)/imagea03.mpic,$(EXEDIR)/atr/imagea03.mpic)
	$(call COPYFILES,$(DATADIR)/imagea04.mpic,$(EXEDIR)/atr/imagea04.mpic)
	$(call COPYFILES,$(DATADIR)/ztiles.bin,$(EXEDIR)/atr/ztiles.bin)
	$(call COPYFILES,$(DATADIR)/tiles.bin,$(EXEDIR)/atr/tiles.bin)
	$(call COPYFILES,$(DATADIR)/tutorial_mctile.bin,$(EXEDIR)/atr/mctile.bin)
	$(call COPYFILES,$(DATADIR)/testcard.bin,$(EXEDIR)/atr/testcard.bin)
	$(call COPYFILES,$(DATADIR)/zdjtiles.bin,$(EXEDIR)/atr/zdjtiles.bin)
	$(call COPYFILES,$(DATADIR)/zeltiles.bin,$(EXEDIR)/atr/zeltiles.bin)
	$(DIR2ATR) -S -p -B $(DIR2ATR_HOME)/dos25/bootcode $(EXEDIR)/midres.atari.atr $(EXEDIR)/atr
	$(ATRAUTORUN) -i $(EXEDIR)/midres.atari.atr -o $(EXEDIR)/midres.atari.atr -f midres-single

# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR VIC20 
# -------------------------------------------------------------------

obj/vic20/%.o:	$(LIB_INCLUDES) $(LIB_SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS)  -Osir -Cl -D__CBM__  -o $@ $(subst obj/vic20/,src/,$(@:.o=.c))

$(LIBDIR)/midres.vic20.lib:	$(LIB_INCLUDES) $(subst PLATFORM,vic20,$(LIB_OBJS))
	$(AR) r $(LIBDIR)/midres.vic20.lib $(subst PLATFORM,vic20,$(LIB_OBJS))

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR VIC20 
# -------------------------------------------------------------------

obj/vic20/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS)  -Osir -Cl -D__CBM__ -o $@ $(subst obj/vic20/,src/,$(@:.o=.c))

$(EXEDIR)/midres.vic20: $(subst PLATFORM,vic20,$(OBJS))
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


# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR VIC2024 
# -------------------------------------------------------------------

obj/vic2024/%.o:	$(LIB_INCLUDES) $(LIB_SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS) -D__24K__ -Osir -Cl -D__CBM__  -o $@ $(subst obj/vic2024/,src/,$(@:.o=.c))

$(LIBDIR)/midres.vic2024.lib:	$(LIB_INCLUDES) $(subst PLATFORM,vic2024,$(LIB_OBJS))
	$(AR) r $(LIBDIR)/midres.vic2024.lib $(subst PLATFORM,vic2024,$(LIB_OBJS))

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR VIC2024 
# -------------------------------------------------------------------

obj/vic2024/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS) -D__24K__ -Osir -Cl -D__CBM__ -o $@ $(subst obj/vic2024/,src/,$(@:.o=.c))

$(EXEDIR)/midres.vic2024: $(subst PLATFORM,vic2024,$(OBJS))
	$(CC) -Ln demovic2024.lbl -t vic20 -C cfg/vic2024.cfg  $(LDFLAGS) -m $(EXEDIR)/midres.vic2024.map -o $(EXEDIR)/midres.vic2024 $(subst PLATFORM,vic2024,$(OBJS)) $(LIBDIR)/midres.vic2024.lib
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


# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR C64 
# -------------------------------------------------------------------

obj/c64/%.o:	$(LIB_INCLUDES) $(LIB_SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c64 -c $(CFLAGS)  -Osir -Cl -D__CBM__  -o $@ $(subst obj/c64/,src/,$(@:.o=.c))

$(LIBDIR)/midres.c64.lib:	$(LIB_INCLUDES) $(subst PLATFORM,c64,$(LIB_OBJS))
	$(AR) r $(LIBDIR)/midres.c64.lib $(subst PLATFORM,c64,$(LIB_OBJS))

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR C64 
# -------------------------------------------------------------------

obj/c64/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c64 -c $(CFLAGS)  -Osir -Cl -D__CBM__ -o $@ $(subst obj/c64/,src/,$(@:.o=.c))

$(EXEDIR)/midres.c64: $(subst PLATFORM,c64,$(OBJS))
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


# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR C128 
# -------------------------------------------------------------------

obj/c128/%.o:	$(LIB_INCLUDES) $(LIB_SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c128 -c $(CFLAGS)  -Osir -Cl -D__CBM__  -o $@ $(subst obj/c128/,src/,$(@:.o=.c))

$(LIBDIR)/midres.c128.lib:	$(LIB_INCLUDES) $(subst PLATFORM,c128,$(LIB_OBJS))
	$(AR) r $(LIBDIR)/midres.c128.lib $(subst PLATFORM,c128,$(LIB_OBJS))

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR C128 
# -------------------------------------------------------------------

obj/c128/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c128 -c $(CFLAGS)  -Osir -Cl -D__CBM__ -o $@ $(subst obj/c128/,src/,$(@:.o=.c))

$(EXEDIR)/midres.c128: $(subst PLATFORM,c128,$(OBJS))
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


# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR C16 
# -------------------------------------------------------------------

obj/c16/%.o:	$(LIB_INCLUDES) $(LIB_SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c16 -c $(CFLAGS)  -Osir -Cl -D__CBM__  -o $@ $(subst obj/c16/,src/,$(@:.o=.c))

$(LIBDIR)/midres.c16.lib:	$(LIB_INCLUDES) $(subst PLATFORM,c16,$(LIB_OBJS))
	$(AR) r $(LIBDIR)/midres.c16.lib $(subst PLATFORM,c16,$(LIB_OBJS))

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR C16 
# -------------------------------------------------------------------

obj/c16/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c16 -c $(CFLAGS)  -Osir -Cl -D__CBM__ -o $@ $(subst obj/c16/,src/,$(@:.o=.c))

$(EXEDIR)/midres.c16: $(subst PLATFORM,c16,$(OBJS))
	$(CC) -Ln democ16.lbl -t c16 -C cfg/c16.cfg  $(LDFLAGS) -m $(EXEDIR)/midres.c16.map -o $(EXEDIR)/midres.c16 $(subst PLATFORM,c16,$(OBJS)) $(LIBDIR)/midres.c16.lib
	$(call RMFILES,$(EXEDIR)/midres.c16.d64)
	$(CC1541) -f midres-single -w $(EXEDIR)/midres.c16 $(EXEDIR)/midres.c16.d64 
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


# -------------------------------------------------------------------
# --- MIDRES LIBRARY FOR PLUS4 
# -------------------------------------------------------------------

obj/plus4/%.o:	$(LIB_INCLUDES) $(LIB_SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t plus4 -c $(CFLAGS)  -Osir -Cl -D__CBM__  -o $@ $(subst obj/plus4/,src/,$(@:.o=.c))

$(LIBDIR)/midres.plus4.lib:	$(LIB_INCLUDES) $(subst PLATFORM,plus4,$(LIB_OBJS))
	$(AR) r $(LIBDIR)/midres.plus4.lib $(subst PLATFORM,plus4,$(LIB_OBJS))

# -------------------------------------------------------------------
# --- DEMO/TUTORIALS FOR PLUS4 
# -------------------------------------------------------------------

obj/plus4/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t plus4 -c $(CFLAGS)  -Osir -Cl -D__CBM__ -o $@ $(subst obj/plus4/,src/,$(@:.o=.c))

$(EXEDIR)/midres.plus4: $(subst PLATFORM,plus4,$(OBJS))
	$(CC) -Ln demoplus4.lbl -t plus4 -C cfg/plus4.cfg  $(LDFLAGS) -m $(EXEDIR)/midres.plus4.map -o $(EXEDIR)/midres.plus4 $(subst PLATFORM,plus4,$(OBJS)) $(LIBDIR)/midres.plus4.lib
	$(call RMFILES,$(EXEDIR)/midres.plus4.d64)
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


# -------------------------------------------------------------------
# --- ALIENSTORM FOR MSX 
# -------------------------------------------------------------------

alienstorm.embedded.msx:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/alienstorm.msx/rawdata.o src/rawdata.c

obj/alienstorm.msx/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.msx/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/alienstorm.msx/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.msx/midres_io.o src/midres_io.asm

obj/alienstorm.msx/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__ALIENSTORM__ -o $@ $(subst obj/alienstorm.msx/,src/,$(@:.o=.c)) 

$(EXEDIR)/alienstorm.msx:	alienstorm.embedded.msx $(subst PLATFORM,alienstorm.msx,$(OBJS)) $(subst PLATFORM,alienstorm.msx,$(LIB_OBJS)) obj/alienstorm.msx/rawdata.o obj/alienstorm.msx/midres_vdp_impl.o obj/alienstorm.msx/midres_io.o
	$(CC88) +msx -subtype=rom -m $(LDFLAGS88) obj/alienstorm.msx/rawdata.o obj/alienstorm.msx/midres_io.o obj/alienstorm.msx/midres_vdp_impl.o $(subst PLATFORM,alienstorm.msx,$(LIB_OBJS)) $(subst PLATFORM,alienstorm.msx,$(OBJS)) -o $(EXEDIR)/alienstorm.msx -create-app 
	$(call COPYFILES,$(EXEDIR)/alienstorm.rom,$(EXEDIR)/alienstorm.msx.rom)


# -------------------------------------------------------------------
# --- ELEVATOR FOR MSX 
# -------------------------------------------------------------------

elevator.embedded.msx:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevatora.mpic -n zelintro.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/elevator.msx/rawdata.o src/rawdata.c

obj/elevator.msx/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.msx/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/elevator.msx/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.msx/midres_io.o src/midres_io.asm

obj/elevator.msx/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__ELEVATOR__ -o $@ $(subst obj/elevator.msx/,src/,$(@:.o=.c)) 

$(EXEDIR)/elevator.msx:	elevator.embedded.msx $(subst PLATFORM,elevator.msx,$(OBJS)) $(subst PLATFORM,elevator.msx,$(LIB_OBJS)) obj/elevator.msx/rawdata.o obj/elevator.msx/midres_vdp_impl.o obj/elevator.msx/midres_io.o
	$(CC88) +msx -subtype=rom -m $(LDFLAGS88) obj/elevator.msx/rawdata.o obj/elevator.msx/midres_io.o obj/elevator.msx/midres_vdp_impl.o $(subst PLATFORM,elevator.msx,$(LIB_OBJS)) $(subst PLATFORM,elevator.msx,$(OBJS)) -o $(EXEDIR)/elevator.msx -create-app 
	$(call COPYFILES,$(EXEDIR)/elevator.rom,$(EXEDIR)/elevator.msx.rom)


# -------------------------------------------------------------------
# --- AIRATTACK FOR MSX 
# -------------------------------------------------------------------

airattack.embedded.msx:
	$(FILE2INCLUDE) -i $(DATADIR)/ztiles.bin -n ztiles.bin -i $(DATADIR)/aatiles4.bin -n zztiles.bin -i $(DATADIR)/aaintroa.pic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/airattack.msx/rawdata.o src/rawdata.c

obj/airattack.msx/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.msx/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/airattack.msx/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.msx/midres_io.o src/midres_io.asm

obj/airattack.msx/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__AIRATTACK__ -o $@ $(subst obj/airattack.msx/,src/,$(@:.o=.c)) 

$(EXEDIR)/airattack.msx:	airattack.embedded.msx $(subst PLATFORM,airattack.msx,$(OBJS)) $(subst PLATFORM,airattack.msx,$(LIB_OBJS)) obj/airattack.msx/rawdata.o obj/airattack.msx/midres_vdp_impl.o obj/airattack.msx/midres_io.o
	$(CC88) +msx -subtype=rom -m $(LDFLAGS88) obj/airattack.msx/rawdata.o obj/airattack.msx/midres_io.o obj/airattack.msx/midres_vdp_impl.o $(subst PLATFORM,airattack.msx,$(LIB_OBJS)) $(subst PLATFORM,airattack.msx,$(OBJS)) -o $(EXEDIR)/airattack.msx -create-app 
	$(call COPYFILES,$(EXEDIR)/airattack.rom,$(EXEDIR)/airattack.msx.rom)


# -------------------------------------------------------------------
# --- TOTTO FOR MSX 
# -------------------------------------------------------------------

totto.embedded.msx:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/totto.msx/rawdata.o src/rawdata.c

obj/totto.msx/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.msx/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/totto.msx/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.msx/midres_io.o src/midres_io.asm

obj/totto.msx/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__TOTTO__ -o $@ $(subst obj/totto.msx/,src/,$(@:.o=.c)) 

$(EXEDIR)/totto.msx:	totto.embedded.msx $(subst PLATFORM,totto.msx,$(OBJS)) $(subst PLATFORM,totto.msx,$(LIB_OBJS)) obj/totto.msx/rawdata.o obj/totto.msx/midres_vdp_impl.o obj/totto.msx/midres_io.o
	$(CC88) +msx -subtype=rom -m $(LDFLAGS88) obj/totto.msx/rawdata.o obj/totto.msx/midres_io.o obj/totto.msx/midres_vdp_impl.o $(subst PLATFORM,totto.msx,$(LIB_OBJS)) $(subst PLATFORM,totto.msx,$(OBJS)) -o $(EXEDIR)/totto.msx -create-app 
	$(call COPYFILES,$(EXEDIR)/totto.rom,$(EXEDIR)/totto.msx.rom)

                    
#!!! missing resources for joycheck (msx)


# -------------------------------------------------------------------
# --- ALIENSTORM FOR MSXC 
# -------------------------------------------------------------------

alienstorm.embedded.msxc:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_II -o obj/alienstorm.msxc/rawdata.o src/rawdata.c

obj/alienstorm.msxc/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.msxc/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/alienstorm.msxc/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.msxc/midres_io.o src/midres_io.asm

obj/alienstorm.msxc/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_II -D__ALIENSTORM__ -o $@ $(subst obj/alienstorm.msxc/,src/,$(@:.o=.c)) 

$(EXEDIR)/alienstorm.msxc:	alienstorm.embedded.msxc $(subst PLATFORM,alienstorm.msxc,$(OBJS)) $(subst PLATFORM,alienstorm.msxc,$(LIB_OBJS)) obj/alienstorm.msxc/rawdata.o obj/alienstorm.msxc/midres_vdp_impl.o obj/alienstorm.msxc/midres_io.o
	$(CC88) +msx -subtype=rom -m $(LDFLAGS88) obj/alienstorm.msxc/rawdata.o obj/alienstorm.msxc/midres_io.o obj/alienstorm.msxc/midres_vdp_impl.o $(subst PLATFORM,alienstorm.msxc,$(LIB_OBJS)) $(subst PLATFORM,alienstorm.msxc,$(OBJS)) -o $(EXEDIR)/alienstorm.msxc -create-app 
	$(call COPYFILES,$(EXEDIR)/alienstorm.rom,$(EXEDIR)/alienstorm.msxc.rom)


# -------------------------------------------------------------------
# --- ELEVATOR FOR MSXC 
# -------------------------------------------------------------------

elevator.embedded.msxc:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator64.mpic -n zelintro.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_II -o obj/elevator.msxc/rawdata.o src/rawdata.c

obj/elevator.msxc/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.msxc/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/elevator.msxc/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.msxc/midres_io.o src/midres_io.asm

obj/elevator.msxc/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_II -D__ELEVATOR__ -o $@ $(subst obj/elevator.msxc/,src/,$(@:.o=.c)) 

$(EXEDIR)/elevator.msxc:	elevator.embedded.msxc $(subst PLATFORM,elevator.msxc,$(OBJS)) $(subst PLATFORM,elevator.msxc,$(LIB_OBJS)) obj/elevator.msxc/rawdata.o obj/elevator.msxc/midres_vdp_impl.o obj/elevator.msxc/midres_io.o
	$(CC88) +msx -subtype=rom -m $(LDFLAGS88) obj/elevator.msxc/rawdata.o obj/elevator.msxc/midres_io.o obj/elevator.msxc/midres_vdp_impl.o $(subst PLATFORM,elevator.msxc,$(LIB_OBJS)) $(subst PLATFORM,elevator.msxc,$(OBJS)) -o $(EXEDIR)/elevator.msxc -create-app 
	$(call COPYFILES,$(EXEDIR)/elevator.rom,$(EXEDIR)/elevator.msxc.rom)


# -------------------------------------------------------------------
# --- AIRATTACK FOR MSXC 
# -------------------------------------------------------------------

airattack.embedded.msxc:
	$(FILE2INCLUDE) -i $(DATADIR)/ztiles.bin -n ztiles.bin -i $(DATADIR)/aatiles4.bin -n zztiles.bin -i $(DATADIR)/aaintroa.pic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_II -o obj/airattack.msxc/rawdata.o src/rawdata.c

obj/airattack.msxc/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.msxc/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/airattack.msxc/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.msxc/midres_io.o src/midres_io.asm

obj/airattack.msxc/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_II -D__AIRATTACK__ -o $@ $(subst obj/airattack.msxc/,src/,$(@:.o=.c)) 

$(EXEDIR)/airattack.msxc:	airattack.embedded.msxc $(subst PLATFORM,airattack.msxc,$(OBJS)) $(subst PLATFORM,airattack.msxc,$(LIB_OBJS)) obj/airattack.msxc/rawdata.o obj/airattack.msxc/midres_vdp_impl.o obj/airattack.msxc/midres_io.o
	$(CC88) +msx -subtype=rom -m $(LDFLAGS88) obj/airattack.msxc/rawdata.o obj/airattack.msxc/midres_io.o obj/airattack.msxc/midres_vdp_impl.o $(subst PLATFORM,airattack.msxc,$(LIB_OBJS)) $(subst PLATFORM,airattack.msxc,$(OBJS)) -o $(EXEDIR)/airattack.msxc -create-app 
	$(call COPYFILES,$(EXEDIR)/airattack.rom,$(EXEDIR)/airattack.msxc.rom)


# -------------------------------------------------------------------
# --- TOTTO FOR MSXC 
# -------------------------------------------------------------------

totto.embedded.msxc:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_II -o obj/totto.msxc/rawdata.o src/rawdata.c

obj/totto.msxc/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.msxc/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/totto.msxc/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.msxc/midres_io.o src/midres_io.asm

obj/totto.msxc/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +msx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_II -D__TOTTO__ -o $@ $(subst obj/totto.msxc/,src/,$(@:.o=.c)) 

$(EXEDIR)/totto.msxc:	totto.embedded.msxc $(subst PLATFORM,totto.msxc,$(OBJS)) $(subst PLATFORM,totto.msxc,$(LIB_OBJS)) obj/totto.msxc/rawdata.o obj/totto.msxc/midres_vdp_impl.o obj/totto.msxc/midres_io.o
	$(CC88) +msx -subtype=rom -m $(LDFLAGS88) obj/totto.msxc/rawdata.o obj/totto.msxc/midres_io.o obj/totto.msxc/midres_vdp_impl.o $(subst PLATFORM,totto.msxc,$(LIB_OBJS)) $(subst PLATFORM,totto.msxc,$(OBJS)) -o $(EXEDIR)/totto.msxc -create-app 
	$(call COPYFILES,$(EXEDIR)/totto.rom,$(EXEDIR)/totto.msxc.rom)

                    
#!!! missing resources for joycheck (msxc)


# -------------------------------------------------------------------
# --- ALIENSTORM FOR SVI 
# -------------------------------------------------------------------

alienstorm.embedded.svi:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/alienstorm.svi/rawdata.o src/rawdata.c

obj/alienstorm.svi/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.svi/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/alienstorm.svi/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.svi/midres_io.o src/midres_io.asm

obj/alienstorm.svi/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__ALIENSTORM__ -o $@ $(subst obj/alienstorm.svi/,src/,$(@:.o=.c)) 

$(EXEDIR)/alienstorm.svi:	alienstorm.embedded.svi $(subst PLATFORM,alienstorm.svi,$(OBJS)) $(subst PLATFORM,alienstorm.svi,$(LIB_OBJS)) obj/alienstorm.svi/rawdata.o obj/alienstorm.svi/midres_vdp_impl.o obj/alienstorm.svi/midres_io.o
	$(CC88) +svi  -m $(LDFLAGS88) obj/alienstorm.svi/rawdata.o obj/alienstorm.svi/midres_io.o obj/alienstorm.svi/midres_vdp_impl.o $(subst PLATFORM,alienstorm.svi,$(LIB_OBJS)) $(subst PLATFORM,alienstorm.svi,$(OBJS)) -o $(EXEDIR)/alienstorm.svi -create-app 
	$(call COPYFILES,$(EXEDIR)/alienstorm.cas,$(EXEDIR)/alienstorm.svi.cas)


# -------------------------------------------------------------------
# --- ELEVATOR FOR SVI 
# -------------------------------------------------------------------

elevator.embedded.svi:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator64.mpic -n zelintro.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/elevator.svi/rawdata.o src/rawdata.c

obj/elevator.svi/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.svi/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/elevator.svi/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.svi/midres_io.o src/midres_io.asm

obj/elevator.svi/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__ELEVATOR__ -o $@ $(subst obj/elevator.svi/,src/,$(@:.o=.c)) 

$(EXEDIR)/elevator.svi:	elevator.embedded.svi $(subst PLATFORM,elevator.svi,$(OBJS)) $(subst PLATFORM,elevator.svi,$(LIB_OBJS)) obj/elevator.svi/rawdata.o obj/elevator.svi/midres_vdp_impl.o obj/elevator.svi/midres_io.o
	$(CC88) +svi  -m $(LDFLAGS88) obj/elevator.svi/rawdata.o obj/elevator.svi/midres_io.o obj/elevator.svi/midres_vdp_impl.o $(subst PLATFORM,elevator.svi,$(LIB_OBJS)) $(subst PLATFORM,elevator.svi,$(OBJS)) -o $(EXEDIR)/elevator.svi -create-app 
	$(call COPYFILES,$(EXEDIR)/elevator.cas,$(EXEDIR)/elevator.svi.cas)


# -------------------------------------------------------------------
# --- AIRATTACK FOR SVI 
# -------------------------------------------------------------------

airattack.embedded.svi:
	$(FILE2INCLUDE) -i $(DATADIR)/ztiles.bin -n ztiles.bin -i $(DATADIR)/aatiles4.bin -n zztiles.bin -i $(DATADIR)/aaintroa.pic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/airattack.svi/rawdata.o src/rawdata.c

obj/airattack.svi/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.svi/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/airattack.svi/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.svi/midres_io.o src/midres_io.asm

obj/airattack.svi/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__AIRATTACK__ -o $@ $(subst obj/airattack.svi/,src/,$(@:.o=.c)) 

$(EXEDIR)/airattack.svi:	airattack.embedded.svi $(subst PLATFORM,airattack.svi,$(OBJS)) $(subst PLATFORM,airattack.svi,$(LIB_OBJS)) obj/airattack.svi/rawdata.o obj/airattack.svi/midres_vdp_impl.o obj/airattack.svi/midres_io.o
	$(CC88) +svi  -m $(LDFLAGS88) obj/airattack.svi/rawdata.o obj/airattack.svi/midres_io.o obj/airattack.svi/midres_vdp_impl.o $(subst PLATFORM,airattack.svi,$(LIB_OBJS)) $(subst PLATFORM,airattack.svi,$(OBJS)) -o $(EXEDIR)/airattack.svi -create-app 
	$(call COPYFILES,$(EXEDIR)/airattack.cas,$(EXEDIR)/airattack.svi.cas)


# -------------------------------------------------------------------
# --- TOTTO FOR SVI 
# -------------------------------------------------------------------

totto.embedded.svi:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/totto.svi/rawdata.o src/rawdata.c

obj/totto.svi/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.svi/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/totto.svi/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.svi/midres_io.o src/midres_io.asm

obj/totto.svi/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +svi $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__TOTTO__ -o $@ $(subst obj/totto.svi/,src/,$(@:.o=.c)) 

$(EXEDIR)/totto.svi:	totto.embedded.svi $(subst PLATFORM,totto.svi,$(OBJS)) $(subst PLATFORM,totto.svi,$(LIB_OBJS)) obj/totto.svi/rawdata.o obj/totto.svi/midres_vdp_impl.o obj/totto.svi/midres_io.o
	$(CC88) +svi  -m $(LDFLAGS88) obj/totto.svi/rawdata.o obj/totto.svi/midres_io.o obj/totto.svi/midres_vdp_impl.o $(subst PLATFORM,totto.svi,$(LIB_OBJS)) $(subst PLATFORM,totto.svi,$(OBJS)) -o $(EXEDIR)/totto.svi -create-app 
	$(call COPYFILES,$(EXEDIR)/totto.cas,$(EXEDIR)/totto.svi.cas)

                    
#!!! missing resources for joycheck (svi)


# -------------------------------------------------------------------
# --- ALIENSTORM FOR MTX500 
# -------------------------------------------------------------------

alienstorm.embedded.mtx500:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/alienstorm.mtx500/rawdata.o src/rawdata.c

obj/alienstorm.mtx500/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.mtx500/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/alienstorm.mtx500/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.mtx500/midres_io.o src/midres_io.asm

obj/alienstorm.mtx500/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__ALIENSTORM__ -o $@ $(subst obj/alienstorm.mtx500/,src/,$(@:.o=.c)) 

$(EXEDIR)/alienstorm.mtx500:	alienstorm.embedded.mtx500 $(subst PLATFORM,alienstorm.mtx500,$(OBJS)) $(subst PLATFORM,alienstorm.mtx500,$(LIB_OBJS)) obj/alienstorm.mtx500/rawdata.o obj/alienstorm.mtx500/midres_vdp_impl.o obj/alienstorm.mtx500/midres_io.o
	$(CC88) +mtx  -m $(LDFLAGS88) obj/alienstorm.mtx500/rawdata.o obj/alienstorm.mtx500/midres_io.o obj/alienstorm.mtx500/midres_vdp_impl.o $(subst PLATFORM,alienstorm.mtx500,$(LIB_OBJS)) $(subst PLATFORM,alienstorm.mtx500,$(OBJS)) -o $(EXEDIR)/alienstorm.mtx500 -create-app 
	$(call COPYFILES,$(EXEDIR)/alienstorm.mtx500.cas,$(EXEDIR)/alienstorm.mtx500.mtx)


# -------------------------------------------------------------------
# --- ELEVATOR FOR MTX500 
# -------------------------------------------------------------------

elevator.embedded.mtx500:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator64.mpic -n zelintro.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/elevator.mtx500/rawdata.o src/rawdata.c

obj/elevator.mtx500/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.mtx500/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/elevator.mtx500/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.mtx500/midres_io.o src/midres_io.asm

obj/elevator.mtx500/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__ELEVATOR__ -o $@ $(subst obj/elevator.mtx500/,src/,$(@:.o=.c)) 

$(EXEDIR)/elevator.mtx500:	elevator.embedded.mtx500 $(subst PLATFORM,elevator.mtx500,$(OBJS)) $(subst PLATFORM,elevator.mtx500,$(LIB_OBJS)) obj/elevator.mtx500/rawdata.o obj/elevator.mtx500/midres_vdp_impl.o obj/elevator.mtx500/midres_io.o
	$(CC88) +mtx  -m $(LDFLAGS88) obj/elevator.mtx500/rawdata.o obj/elevator.mtx500/midres_io.o obj/elevator.mtx500/midres_vdp_impl.o $(subst PLATFORM,elevator.mtx500,$(LIB_OBJS)) $(subst PLATFORM,elevator.mtx500,$(OBJS)) -o $(EXEDIR)/elevator.mtx500 -create-app 
	$(call COPYFILES,$(EXEDIR)/elevator.mtx500.cas,$(EXEDIR)/elevator.mtx500.mtx)


# -------------------------------------------------------------------
# --- AIRATTACK FOR MTX500 
# -------------------------------------------------------------------

airattack.embedded.mtx500:
	$(FILE2INCLUDE) -i $(DATADIR)/ztiles.bin -n ztiles.bin -i $(DATADIR)/aatiles4.bin -n zztiles.bin -i $(DATADIR)/aaintroa.pic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/airattack.mtx500/rawdata.o src/rawdata.c

obj/airattack.mtx500/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.mtx500/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/airattack.mtx500/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.mtx500/midres_io.o src/midres_io.asm

obj/airattack.mtx500/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__AIRATTACK__ -o $@ $(subst obj/airattack.mtx500/,src/,$(@:.o=.c)) 

$(EXEDIR)/airattack.mtx500:	airattack.embedded.mtx500 $(subst PLATFORM,airattack.mtx500,$(OBJS)) $(subst PLATFORM,airattack.mtx500,$(LIB_OBJS)) obj/airattack.mtx500/rawdata.o obj/airattack.mtx500/midres_vdp_impl.o obj/airattack.mtx500/midres_io.o
	$(CC88) +mtx  -m $(LDFLAGS88) obj/airattack.mtx500/rawdata.o obj/airattack.mtx500/midres_io.o obj/airattack.mtx500/midres_vdp_impl.o $(subst PLATFORM,airattack.mtx500,$(LIB_OBJS)) $(subst PLATFORM,airattack.mtx500,$(OBJS)) -o $(EXEDIR)/airattack.mtx500 -create-app 
	$(call COPYFILES,$(EXEDIR)/airattack.mtx500.cas,$(EXEDIR)/airattack.mtx500.mtx)


# -------------------------------------------------------------------
# --- TOTTO FOR MTX500 
# -------------------------------------------------------------------

totto.embedded.mtx500:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/totto.mtx500/rawdata.o src/rawdata.c

obj/totto.mtx500/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.mtx500/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/totto.mtx500/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.mtx500/midres_io.o src/midres_io.asm

obj/totto.mtx500/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +mtx $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__TOTTO__ -o $@ $(subst obj/totto.mtx500/,src/,$(@:.o=.c)) 

$(EXEDIR)/totto.mtx500:	totto.embedded.mtx500 $(subst PLATFORM,totto.mtx500,$(OBJS)) $(subst PLATFORM,totto.mtx500,$(LIB_OBJS)) obj/totto.mtx500/rawdata.o obj/totto.mtx500/midres_vdp_impl.o obj/totto.mtx500/midres_io.o
	$(CC88) +mtx  -m $(LDFLAGS88) obj/totto.mtx500/rawdata.o obj/totto.mtx500/midres_io.o obj/totto.mtx500/midres_vdp_impl.o $(subst PLATFORM,totto.mtx500,$(LIB_OBJS)) $(subst PLATFORM,totto.mtx500,$(OBJS)) -o $(EXEDIR)/totto.mtx500 -create-app 
	$(call COPYFILES,$(EXEDIR)/totto.mtx500.cas,$(EXEDIR)/totto.mtx500.mtx)

                    
#!!! missing resources for joycheck (mtx500)


# -------------------------------------------------------------------
# --- ALIENSTORM FOR COLECO 
# -------------------------------------------------------------------

alienstorm.embedded.coleco:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/alienstorm.coleco/rawdata.o src/rawdata.c

obj/alienstorm.coleco/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.coleco/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/alienstorm.coleco/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/alienstorm.coleco/midres_io.o src/midres_io.asm

obj/alienstorm.coleco/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__ALIENSTORM__ -o $@ $(subst obj/alienstorm.coleco/,src/,$(@:.o=.c)) 

$(EXEDIR)/alienstorm.coleco:	alienstorm.embedded.coleco $(subst PLATFORM,alienstorm.coleco,$(OBJS)) $(subst PLATFORM,alienstorm.coleco,$(LIB_OBJS)) obj/alienstorm.coleco/rawdata.o obj/alienstorm.coleco/midres_vdp_impl.o obj/alienstorm.coleco/midres_io.o
	$(CC88) +coleco  -m $(LDFLAGS88) obj/alienstorm.coleco/rawdata.o obj/alienstorm.coleco/midres_io.o obj/alienstorm.coleco/midres_vdp_impl.o $(subst PLATFORM,alienstorm.coleco,$(LIB_OBJS)) $(subst PLATFORM,alienstorm.coleco,$(OBJS)) -o $(EXEDIR)/alienstorm.coleco -create-app 
	$(call COPYFILES,$(EXEDIR)/alienstorm.rom,$(EXEDIR)/alienstorm.coleco.rom)


# -------------------------------------------------------------------
# --- ELEVATOR FOR COLECO 
# -------------------------------------------------------------------

elevator.embedded.coleco:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator64.mpic -n zelintro.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/elevator.coleco/rawdata.o src/rawdata.c

obj/elevator.coleco/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.coleco/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/elevator.coleco/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/elevator.coleco/midres_io.o src/midres_io.asm

obj/elevator.coleco/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__ELEVATOR__ -o $@ $(subst obj/elevator.coleco/,src/,$(@:.o=.c)) 

$(EXEDIR)/elevator.coleco:	elevator.embedded.coleco $(subst PLATFORM,elevator.coleco,$(OBJS)) $(subst PLATFORM,elevator.coleco,$(LIB_OBJS)) obj/elevator.coleco/rawdata.o obj/elevator.coleco/midres_vdp_impl.o obj/elevator.coleco/midres_io.o
	$(CC88) +coleco  -m $(LDFLAGS88) obj/elevator.coleco/rawdata.o obj/elevator.coleco/midres_io.o obj/elevator.coleco/midres_vdp_impl.o $(subst PLATFORM,elevator.coleco,$(LIB_OBJS)) $(subst PLATFORM,elevator.coleco,$(OBJS)) -o $(EXEDIR)/elevator.coleco -create-app 
	$(call COPYFILES,$(EXEDIR)/elevator.rom,$(EXEDIR)/elevator.coleco.rom)


# -------------------------------------------------------------------
# --- AIRATTACK FOR COLECO 
# -------------------------------------------------------------------

airattack.embedded.coleco:
	$(FILE2INCLUDE) -i $(DATADIR)/ztiles.bin -n ztiles.bin -i $(DATADIR)/aatiles4.bin -n zztiles.bin -i $(DATADIR)/aaintroa.pic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/airattack.coleco/rawdata.o src/rawdata.c

obj/airattack.coleco/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.coleco/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/airattack.coleco/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/airattack.coleco/midres_io.o src/midres_io.asm

obj/airattack.coleco/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__AIRATTACK__ -o $@ $(subst obj/airattack.coleco/,src/,$(@:.o=.c)) 

$(EXEDIR)/airattack.coleco:	airattack.embedded.coleco $(subst PLATFORM,airattack.coleco,$(OBJS)) $(subst PLATFORM,airattack.coleco,$(LIB_OBJS)) obj/airattack.coleco/rawdata.o obj/airattack.coleco/midres_vdp_impl.o obj/airattack.coleco/midres_io.o
	$(CC88) +coleco  -m $(LDFLAGS88) obj/airattack.coleco/rawdata.o obj/airattack.coleco/midres_io.o obj/airattack.coleco/midres_vdp_impl.o $(subst PLATFORM,airattack.coleco,$(LIB_OBJS)) $(subst PLATFORM,airattack.coleco,$(OBJS)) -o $(EXEDIR)/airattack.coleco -create-app 
	$(call COPYFILES,$(EXEDIR)/airattack.rom,$(EXEDIR)/airattack.coleco.rom)


# -------------------------------------------------------------------
# --- TOTTO FOR COLECO 
# -------------------------------------------------------------------

totto.embedded.coleco:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -c src/rawdata.c -h src/rawdata.h
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -o obj/totto.coleco/rawdata.o src/rawdata.c

obj/totto.coleco/midres_vdp_impl.o:	src/midres_vdp_impl.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.coleco/midres_vdp_impl.o src/midres_vdp_impl.asm

obj/totto.coleco/midres_io.o:	src/midres_io.asm
	$(ASM88) -D__SCCZ80 -m -s -mz80 -oobj/totto.coleco/midres_io.o src/midres_io.asm

obj/totto.coleco/%.o:	$(SOURCES) $(LIB_SOURCES)
	$(CC88) +coleco $(CFLAGS) -c $(CFLAGS88) -DGRAPHIC_MODE_I -D__TOTTO__ -o $@ $(subst obj/totto.coleco/,src/,$(@:.o=.c)) 

$(EXEDIR)/totto.coleco:	totto.embedded.coleco $(subst PLATFORM,totto.coleco,$(OBJS)) $(subst PLATFORM,totto.coleco,$(LIB_OBJS)) obj/totto.coleco/rawdata.o obj/totto.coleco/midres_vdp_impl.o obj/totto.coleco/midres_io.o
	$(CC88) +coleco  -m $(LDFLAGS88) obj/totto.coleco/rawdata.o obj/totto.coleco/midres_io.o obj/totto.coleco/midres_vdp_impl.o $(subst PLATFORM,totto.coleco,$(LIB_OBJS)) $(subst PLATFORM,totto.coleco,$(OBJS)) -o $(EXEDIR)/totto.coleco -create-app 
	$(call COPYFILES,$(EXEDIR)/totto.rom,$(EXEDIR)/totto.coleco.rom)

                    
#!!! missing resources for joycheck (coleco)


# -------------------------------------------------------------------
# --- ALIENSTORM FOR ATARI 
# -------------------------------------------------------------------

alienstorm.embedded.atari:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h

obj/alienstorm.atari/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t atari -c $(CFLAGS)  -Osir -Cl -D__ALIENSTORM__  -o $@ $(subst obj/alienstorm.atari/,src/,$(@:.o=.c))

$(EXEDIR)/alienstorm.atari: alienstorm.embedded.atari $(subst PLATFORM,alienstorm.atari,$(OBJS))
	$(CC) -Ln demoatari.lbl -t atari -C cfg/atari.cfg $(LDFLAGS) -m $(EXEDIR)/alienstorm.atari.map -o $(EXEDIR)/alienstorm.atari $(subst PLATFORM,alienstorm.atari,$(OBJS)) $(LIBDIR)/midres.atari.lib
	$(call RMFILES,$(EXEDIR)/atr/*.*)
	$(call COPYFILES,$(DIR2ATR_HOME)/dos25/dos.sys,$(EXEDIR)/atr/dos.sys)
	$(call COPYFILES,$(EXEDIR)/alienstorm.atari,$(EXEDIR)/atr/game.exe)
	$(call COPYFILES,$(DATADIR)/astiles.bin,$(EXEDIR)/atr/zstiles.bin)
	$(call COPYFILES,$(DATADIR)/astiles1.bin,$(EXEDIR)/atr/zstiles1.bin)
	$(call COPYFILES,$(DATADIR)/astiles2.bin,$(EXEDIR)/atr/zstiles2.bin)
	$(DIR2ATR) -S -p -B $(DIR2ATR_HOME)/dos25/bootcode $(EXEDIR)/midres.atari.atr $(EXEDIR)/atr
	$(ATRAUTORUN) -i $(EXEDIR)/midres.atari.atr -o $(EXEDIR)/midres.atari.atr -f game.exe

# -------------------------------------------------------------------
# --- ELEVATOR FOR ATARI 
# -------------------------------------------------------------------

elevator.embedded.atari:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator64.mpic -n zelintro.bin -c src/rawdata.c -h src/rawdata.h

obj/elevator.atari/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t atari -c $(CFLAGS)  -Osir -Cl -D__ELEVATOR__  -o $@ $(subst obj/elevator.atari/,src/,$(@:.o=.c))

$(EXEDIR)/elevator.atari: elevator.embedded.atari $(subst PLATFORM,elevator.atari,$(OBJS))
	$(CC) -Ln demoatari.lbl -t atari -C cfg/atari.cfg $(LDFLAGS) -m $(EXEDIR)/elevator.atari.map -o $(EXEDIR)/elevator.atari $(subst PLATFORM,elevator.atari,$(OBJS)) $(LIBDIR)/midres.atari.lib
	$(call RMFILES,$(EXEDIR)/atr/*.*)
	$(call COPYFILES,$(DIR2ATR_HOME)/dos25/dos.sys,$(EXEDIR)/atr/dos.sys)
	$(call COPYFILES,$(EXEDIR)/elevator.atari,$(EXEDIR)/atr/game.exe)
	$(call COPYFILES,$(DATADIR)/zeltiles.bin,$(EXEDIR)/atr/zeltiles.bin)
	$(call COPYFILES,$(DATADIR)/elevator64.mpic,$(EXEDIR)/atr/zelintro.bin)
	$(DIR2ATR) -S -p -B $(DIR2ATR_HOME)/dos25/bootcode $(EXEDIR)/midres.atari.atr $(EXEDIR)/atr
	$(ATRAUTORUN) -i $(EXEDIR)/midres.atari.atr -o $(EXEDIR)/midres.atari.atr -f game.exe

# -------------------------------------------------------------------
# --- AIRATTACK FOR ATARI 
# -------------------------------------------------------------------

airattack.embedded.atari:
	$(FILE2INCLUDE) -i $(DATADIR)/aatiles4.bin -n zztiles.bin -i $(DATADIR)/aaintroa.pic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h

obj/airattack.atari/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t atari -c $(CFLAGS)  -Osir -Cl -D__AIRATTACK__  -o $@ $(subst obj/airattack.atari/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.atari: airattack.embedded.atari $(subst PLATFORM,airattack.atari,$(OBJS))
	$(CC) -Ln demoatari.lbl -t atari -C cfg/atari.cfg $(LDFLAGS) -m $(EXEDIR)/airattack.atari.map -o $(EXEDIR)/airattack.atari $(subst PLATFORM,airattack.atari,$(OBJS)) $(LIBDIR)/midres.atari.lib
	$(call RMFILES,$(EXEDIR)/atr/*.*)
	$(call COPYFILES,$(DIR2ATR_HOME)/dos25/dos.sys,$(EXEDIR)/atr/dos.sys)
	$(call COPYFILES,$(EXEDIR)/airattack.atari,$(EXEDIR)/atr/game.exe)
	$(call COPYFILES,$(DATADIR)/aatiles4.bin,$(EXEDIR)/atr/zztiles.bin)
	$(call COPYFILES,$(DATADIR)/aaintroa.pic,$(EXEDIR)/atr/zzintro.pic)
	$(DIR2ATR) -S -p -B $(DIR2ATR_HOME)/dos25/bootcode $(EXEDIR)/midres.atari.atr $(EXEDIR)/atr
	$(ATRAUTORUN) -i $(EXEDIR)/midres.atari.atr -o $(EXEDIR)/midres.atari.atr -f game.exe
                    
#!!! missing resources for totto (atari)


# -------------------------------------------------------------------
# --- JOYCHECK FOR ATARI 
# -------------------------------------------------------------------

joycheck.embedded.atari:
	$(FILE2INCLUDE) -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -c src/rawdata.c -h src/rawdata.h

obj/joycheck.atari/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t atari -c $(CFLAGS)  -Osir -Cl -D__JOYCHECK__  -o $@ $(subst obj/joycheck.atari/,src/,$(@:.o=.c))

$(EXEDIR)/joycheck.atari: joycheck.embedded.atari $(subst PLATFORM,joycheck.atari,$(OBJS))
	$(CC) -Ln demoatari.lbl -t atari -C cfg/atari.cfg $(LDFLAGS) -m $(EXEDIR)/joycheck.atari.map -o $(EXEDIR)/joycheck.atari $(subst PLATFORM,joycheck.atari,$(OBJS)) $(LIBDIR)/midres.atari.lib
	$(call RMFILES,$(EXEDIR)/atr/*.*)
	$(call COPYFILES,$(DIR2ATR_HOME)/dos25/dos.sys,$(EXEDIR)/atr/dos.sys)
	$(call COPYFILES,$(EXEDIR)/joycheck.atari,$(EXEDIR)/atr/game.exe)
	$(call COPYFILES,$(DATADIR)/zdjtiles.bin,$(EXEDIR)/atr/zdjtiles.bin)
	$(DIR2ATR) -S -p -B $(DIR2ATR_HOME)/dos25/bootcode $(EXEDIR)/midres.atari.atr $(EXEDIR)/atr
	$(ATRAUTORUN) -i $(EXEDIR)/midres.atari.atr -o $(EXEDIR)/midres.atari.atr -f game.exe

# -------------------------------------------------------------------
# --- ALIENSTORM FOR VIC20 
# -------------------------------------------------------------------

alienstorm.embedded.vic20:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h

obj/alienstorm.vic20/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS)  -Osir -Cl -D__ALIENSTORM__ -D__CBM__ -o $@ $(subst obj/alienstorm.vic20/,src/,$(@:.o=.c))

$(EXEDIR)/alienstorm.vic20: alienstorm.embedded.vic20 $(subst PLATFORM,alienstorm.vic20,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic20.cfg $(LDFLAGS) -m $(EXEDIR)/alienstorm.vic20.map -o $(EXEDIR)/alienstorm.vic20 $(subst PLATFORM,alienstorm.vic20,$(OBJS)) $(LIBDIR)/midres.vic20.lib
	$(call RMFILES,$(EXEDIR)/alienstorm.vic20.d64)
	$(CC1541) -f $(EXEDIR)/alienstorm -w $(EXEDIR)/alienstorm.vic20 $(EXEDIR)/alienstorm.vic20.d64 
	$(CC1541) -f zstiles.bin -w $(DATADIR)/astiles.bin $(EXEDIR)/alienstorm.vic20.d64 
	$(CC1541) -f zstiles1.bin -w $(DATADIR)/astiles1.bin $(EXEDIR)/alienstorm.vic20.d64 
	$(CC1541) -f zstiles2.bin -w $(DATADIR)/astiles2.bin $(EXEDIR)/alienstorm.vic20.d64 


# -------------------------------------------------------------------
# --- ELEVATOR FOR VIC20 
# -------------------------------------------------------------------

elevator.embedded.vic20:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator20.mpic -n zelintro.bin -c src/rawdata.c -h src/rawdata.h

obj/elevator.vic20/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS)  -Osir -Cl -D__ELEVATOR__ -D__CBM__ -o $@ $(subst obj/elevator.vic20/,src/,$(@:.o=.c))

$(EXEDIR)/elevator.vic20: elevator.embedded.vic20 $(subst PLATFORM,elevator.vic20,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic20.cfg $(LDFLAGS) -m $(EXEDIR)/elevator.vic20.map -o $(EXEDIR)/elevator.vic20 $(subst PLATFORM,elevator.vic20,$(OBJS)) $(LIBDIR)/midres.vic20.lib
	$(call RMFILES,$(EXEDIR)/elevator.vic20.d64)
	$(CC1541) -f $(EXEDIR)/elevator -w $(EXEDIR)/elevator.vic20 $(EXEDIR)/elevator.vic20.d64 
	$(CC1541) -f zeltiles.bin -w $(DATADIR)/zeltiles.bin $(EXEDIR)/elevator.vic20.d64 
	$(CC1541) -f zelintro.bin -w $(DATADIR)/elevator20.mpic $(EXEDIR)/elevator.vic20.d64 


# -------------------------------------------------------------------
# --- AIRATTACK FOR VIC20 
# -------------------------------------------------------------------

airattack.embedded.vic20:
	$(FILE2INCLUDE) -i $(DATADIR)/aatiles20.bin -n zztiles.bin -i $(DATADIR)/aaintro20.mpic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h

obj/airattack.vic20/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS)  -Osir -Cl -D__AIRATTACK__ -D__CBM__ -o $@ $(subst obj/airattack.vic20/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.vic20: airattack.embedded.vic20 $(subst PLATFORM,airattack.vic20,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic20.cfg $(LDFLAGS) -m $(EXEDIR)/airattack.vic20.map -o $(EXEDIR)/airattack.vic20 $(subst PLATFORM,airattack.vic20,$(OBJS)) $(LIBDIR)/midres.vic20.lib
	$(call RMFILES,$(EXEDIR)/airattack.vic20.d64)
	$(CC1541) -f $(EXEDIR)/airattack -w $(EXEDIR)/airattack.vic20 $(EXEDIR)/airattack.vic20.d64 
	$(CC1541) -f zztiles.bin -w $(DATADIR)/aatiles20.bin $(EXEDIR)/airattack.vic20.d64 
	$(CC1541) -f zzintro.pic -w $(DATADIR)/aaintro20.mpic $(EXEDIR)/airattack.vic20.d64 


# -------------------------------------------------------------------
# --- TOTTO FOR VIC20 
# -------------------------------------------------------------------

totto.embedded.vic20:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -i $(DATADIR)/ttfinal120.mpic -n ttfinal1.mpic -i $(DATADIR)/ttfinal220.mpic -n ttfinal2.mpic -i $(DATADIR)/ttfinal320.mpic -n ttfinal3.mpic -i $(DATADIR)/ttfinal420.mpic -n ttfinal4.mpic -c src/rawdata.c -h src/rawdata.h

obj/totto.vic20/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS)  -Osir -Cl -D__TOTTO__ -D__CBM__ -o $@ $(subst obj/totto.vic20/,src/,$(@:.o=.c))

$(EXEDIR)/totto.vic20: totto.embedded.vic20 $(subst PLATFORM,totto.vic20,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic20.cfg $(LDFLAGS) -m $(EXEDIR)/totto.vic20.map -o $(EXEDIR)/totto.vic20 $(subst PLATFORM,totto.vic20,$(OBJS)) $(LIBDIR)/midres.vic20.lib
	$(call RMFILES,$(EXEDIR)/totto.vic20.d64)
	$(CC1541) -f $(EXEDIR)/totto -w $(EXEDIR)/totto.vic20 $(EXEDIR)/totto.vic20.d64 
	$(CC1541) -f tttiles.bin -w $(DATADIR)/tttiles.bin $(EXEDIR)/totto.vic20.d64 
	$(CC1541) -f tttiles1.bin -w $(DATADIR)/tttiles1.bin $(EXEDIR)/totto.vic20.d64 
	$(CC1541) -f ttfinal1.mpic -w $(DATADIR)/ttfinal120.mpic $(EXEDIR)/totto.vic20.d64 
	$(CC1541) -f ttfinal2.mpic -w $(DATADIR)/ttfinal220.mpic $(EXEDIR)/totto.vic20.d64 
	$(CC1541) -f ttfinal3.mpic -w $(DATADIR)/ttfinal320.mpic $(EXEDIR)/totto.vic20.d64 
	$(CC1541) -f ttfinal4.mpic -w $(DATADIR)/ttfinal420.mpic $(EXEDIR)/totto.vic20.d64 


# -------------------------------------------------------------------
# --- JOYCHECK FOR VIC20 
# -------------------------------------------------------------------

joycheck.embedded.vic20:
	$(FILE2INCLUDE) -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -c src/rawdata.c -h src/rawdata.h

obj/joycheck.vic20/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS)  -Osir -Cl -D__JOYCHECK__ -D__CBM__ -o $@ $(subst obj/joycheck.vic20/,src/,$(@:.o=.c))

$(EXEDIR)/joycheck.vic20: joycheck.embedded.vic20 $(subst PLATFORM,joycheck.vic20,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic20.cfg $(LDFLAGS) -m $(EXEDIR)/joycheck.vic20.map -o $(EXEDIR)/joycheck.vic20 $(subst PLATFORM,joycheck.vic20,$(OBJS)) $(LIBDIR)/midres.vic20.lib
	$(call RMFILES,$(EXEDIR)/joycheck.vic20.d64)
	$(CC1541) -f $(EXEDIR)/joycheck -w $(EXEDIR)/joycheck.vic20 $(EXEDIR)/joycheck.vic20.d64 
	$(CC1541) -f zdjtiles.bin -w $(DATADIR)/zdjtiles.bin $(EXEDIR)/joycheck.vic20.d64 


# -------------------------------------------------------------------
# --- ALIENSTORM FOR VIC2024 
# -------------------------------------------------------------------

alienstorm.embedded.vic2024:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h

obj/alienstorm.vic2024/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS) -D__24K__ -Osir -Cl -D__ALIENSTORM__ -D__CBM__ -o $@ $(subst obj/alienstorm.vic2024/,src/,$(@:.o=.c))

$(EXEDIR)/alienstorm.vic2024: alienstorm.embedded.vic2024 $(subst PLATFORM,alienstorm.vic2024,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic2024.cfg $(LDFLAGS) -m $(EXEDIR)/alienstorm.vic20.map -o $(EXEDIR)/alienstorm.vic2024 $(subst PLATFORM,alienstorm.vic2024,$(OBJS)) $(LIBDIR)/midres.vic2024.lib
	$(call RMFILES,$(EXEDIR)/alienstorm.vic2024.d64)
	$(CC1541) -f $(EXEDIR)/alienstorm -w $(EXEDIR)/alienstorm.vic2024 $(EXEDIR)/alienstorm.vic2024.d64 
	$(CC1541) -f zstiles.bin -w $(DATADIR)/astiles.bin $(EXEDIR)/alienstorm.vic2024.d64 
	$(CC1541) -f zstiles1.bin -w $(DATADIR)/astiles1.bin $(EXEDIR)/alienstorm.vic2024.d64 
	$(CC1541) -f zstiles2.bin -w $(DATADIR)/astiles2.bin $(EXEDIR)/alienstorm.vic2024.d64 


# -------------------------------------------------------------------
# --- ELEVATOR FOR VIC2024 
# -------------------------------------------------------------------

elevator.embedded.vic2024:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator20.mpic -n zelintro.bin -c src/rawdata.c -h src/rawdata.h

obj/elevator.vic2024/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS) -D__24K__ -Osir -Cl -D__ELEVATOR__ -D__CBM__ -o $@ $(subst obj/elevator.vic2024/,src/,$(@:.o=.c))

$(EXEDIR)/elevator.vic2024: elevator.embedded.vic2024 $(subst PLATFORM,elevator.vic2024,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic2024.cfg $(LDFLAGS) -m $(EXEDIR)/elevator.vic20.map -o $(EXEDIR)/elevator.vic2024 $(subst PLATFORM,elevator.vic2024,$(OBJS)) $(LIBDIR)/midres.vic2024.lib
	$(call RMFILES,$(EXEDIR)/elevator.vic2024.d64)
	$(CC1541) -f $(EXEDIR)/elevator -w $(EXEDIR)/elevator.vic2024 $(EXEDIR)/elevator.vic2024.d64 
	$(CC1541) -f zeltiles.bin -w $(DATADIR)/zeltiles.bin $(EXEDIR)/elevator.vic2024.d64 
	$(CC1541) -f zelintro.bin -w $(DATADIR)/elevator20.mpic $(EXEDIR)/elevator.vic2024.d64 


# -------------------------------------------------------------------
# --- AIRATTACK FOR VIC2024 
# -------------------------------------------------------------------

airattack.embedded.vic2024:
	$(FILE2INCLUDE) -i $(DATADIR)/aatiles20.bin -n zztiles.bin -i $(DATADIR)/aaintro20.mpic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h

obj/airattack.vic2024/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS) -D__24K__ -Osir -Cl -D__AIRATTACK__ -D__CBM__ -o $@ $(subst obj/airattack.vic2024/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.vic2024: airattack.embedded.vic2024 $(subst PLATFORM,airattack.vic2024,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic2024.cfg $(LDFLAGS) -m $(EXEDIR)/airattack.vic20.map -o $(EXEDIR)/airattack.vic2024 $(subst PLATFORM,airattack.vic2024,$(OBJS)) $(LIBDIR)/midres.vic2024.lib
	$(call RMFILES,$(EXEDIR)/airattack.vic2024.d64)
	$(CC1541) -f $(EXEDIR)/airattack -w $(EXEDIR)/airattack.vic2024 $(EXEDIR)/airattack.vic2024.d64 
	$(CC1541) -f zztiles.bin -w $(DATADIR)/aatiles20.bin $(EXEDIR)/airattack.vic2024.d64 
	$(CC1541) -f zzintro.pic -w $(DATADIR)/aaintro20.mpic $(EXEDIR)/airattack.vic2024.d64 


# -------------------------------------------------------------------
# --- TOTTO FOR VIC2024 
# -------------------------------------------------------------------

totto.embedded.vic2024:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -i $(DATADIR)/ttfinal120.mpic -n ttfinal1.mpic -i $(DATADIR)/ttfinal220.mpic -n ttfinal2.mpic -i $(DATADIR)/ttfinal320.mpic -n ttfinal3.mpic -i $(DATADIR)/ttfinal420.mpic -n ttfinal4.mpic -c src/rawdata.c -h src/rawdata.h

obj/totto.vic2024/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS) -D__24K__ -Osir -Cl -D__TOTTO__ -D__CBM__ -o $@ $(subst obj/totto.vic2024/,src/,$(@:.o=.c))

$(EXEDIR)/totto.vic2024: totto.embedded.vic2024 $(subst PLATFORM,totto.vic2024,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic2024.cfg $(LDFLAGS) -m $(EXEDIR)/totto.vic20.map -o $(EXEDIR)/totto.vic2024 $(subst PLATFORM,totto.vic2024,$(OBJS)) $(LIBDIR)/midres.vic2024.lib
	$(call RMFILES,$(EXEDIR)/totto.vic2024.d64)
	$(CC1541) -f $(EXEDIR)/totto -w $(EXEDIR)/totto.vic2024 $(EXEDIR)/totto.vic2024.d64 
	$(CC1541) -f tttiles.bin -w $(DATADIR)/tttiles.bin $(EXEDIR)/totto.vic2024.d64 
	$(CC1541) -f tttiles1.bin -w $(DATADIR)/tttiles1.bin $(EXEDIR)/totto.vic2024.d64 
	$(CC1541) -f ttfinal1.mpic -w $(DATADIR)/ttfinal120.mpic $(EXEDIR)/totto.vic2024.d64 
	$(CC1541) -f ttfinal2.mpic -w $(DATADIR)/ttfinal220.mpic $(EXEDIR)/totto.vic2024.d64 
	$(CC1541) -f ttfinal3.mpic -w $(DATADIR)/ttfinal320.mpic $(EXEDIR)/totto.vic2024.d64 
	$(CC1541) -f ttfinal4.mpic -w $(DATADIR)/ttfinal420.mpic $(EXEDIR)/totto.vic2024.d64 


# -------------------------------------------------------------------
# --- JOYCHECK FOR VIC2024 
# -------------------------------------------------------------------

joycheck.embedded.vic2024:
	$(FILE2INCLUDE) -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -c src/rawdata.c -h src/rawdata.h

obj/joycheck.vic2024/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t vic20 -c $(CFLAGS) -D__24K__ -Osir -Cl -D__JOYCHECK__ -D__CBM__ -o $@ $(subst obj/joycheck.vic2024/,src/,$(@:.o=.c))

$(EXEDIR)/joycheck.vic2024: joycheck.embedded.vic2024 $(subst PLATFORM,joycheck.vic2024,$(OBJS))
	$(CC) -Ln demovic20.lbl -t vic20 -C cfg/vic2024.cfg $(LDFLAGS) -m $(EXEDIR)/joycheck.vic20.map -o $(EXEDIR)/joycheck.vic2024 $(subst PLATFORM,joycheck.vic2024,$(OBJS)) $(LIBDIR)/midres.vic2024.lib
	$(call RMFILES,$(EXEDIR)/joycheck.vic2024.d64)
	$(CC1541) -f $(EXEDIR)/joycheck -w $(EXEDIR)/joycheck.vic2024 $(EXEDIR)/joycheck.vic2024.d64 
	$(CC1541) -f zdjtiles.bin -w $(DATADIR)/zdjtiles.bin $(EXEDIR)/joycheck.vic2024.d64 


# -------------------------------------------------------------------
# --- ALIENSTORM FOR C64 
# -------------------------------------------------------------------

alienstorm.embedded.c64:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h

obj/alienstorm.c64/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c64 -c $(CFLAGS)  -Osir -Cl -D__ALIENSTORM__ -D__CBM__ -o $@ $(subst obj/alienstorm.c64/,src/,$(@:.o=.c))

$(EXEDIR)/alienstorm.c64: alienstorm.embedded.c64 $(subst PLATFORM,alienstorm.c64,$(OBJS))
	$(CC) -Ln democ64.lbl -t c64 -C cfg/c64.cfg $(LDFLAGS) -m $(EXEDIR)/alienstorm.c64.map -o $(EXEDIR)/alienstorm.c64 $(subst PLATFORM,alienstorm.c64,$(OBJS)) $(LIBDIR)/midres.c64.lib
	$(call RMFILES,$(EXEDIR)/alienstorm.c64.d64)
	$(CC1541) -f $(EXEDIR)/alienstorm -w $(EXEDIR)/alienstorm.c64 $(EXEDIR)/alienstorm.c64.d64 
	$(CC1541) -f zstiles.bin -w $(DATADIR)/astiles.bin $(EXEDIR)/alienstorm.c64.d64 
	$(CC1541) -f zstiles1.bin -w $(DATADIR)/astiles1.bin $(EXEDIR)/alienstorm.c64.d64 
	$(CC1541) -f zstiles2.bin -w $(DATADIR)/astiles2.bin $(EXEDIR)/alienstorm.c64.d64 


# -------------------------------------------------------------------
# --- ELEVATOR FOR C64 
# -------------------------------------------------------------------

elevator.embedded.c64:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator64.mpic -n zelintro.bin -c src/rawdata.c -h src/rawdata.h

obj/elevator.c64/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c64 -c $(CFLAGS)  -Osir -Cl -D__ELEVATOR__ -D__CBM__ -o $@ $(subst obj/elevator.c64/,src/,$(@:.o=.c))

$(EXEDIR)/elevator.c64: elevator.embedded.c64 $(subst PLATFORM,elevator.c64,$(OBJS))
	$(CC) -Ln democ64.lbl -t c64 -C cfg/c64.cfg $(LDFLAGS) -m $(EXEDIR)/elevator.c64.map -o $(EXEDIR)/elevator.c64 $(subst PLATFORM,elevator.c64,$(OBJS)) $(LIBDIR)/midres.c64.lib
	$(call RMFILES,$(EXEDIR)/elevator.c64.d64)
	$(CC1541) -f $(EXEDIR)/elevator -w $(EXEDIR)/elevator.c64 $(EXEDIR)/elevator.c64.d64 
	$(CC1541) -f zeltiles.bin -w $(DATADIR)/zeltiles.bin $(EXEDIR)/elevator.c64.d64 
	$(CC1541) -f zelintro.bin -w $(DATADIR)/elevator64.mpic $(EXEDIR)/elevator.c64.d64 


# -------------------------------------------------------------------
# --- AIRATTACK FOR C64 
# -------------------------------------------------------------------

airattack.embedded.c64:
	$(FILE2INCLUDE) -i $(DATADIR)/aatiles.bin -n zztiles.bin -i $(DATADIR)/aaintro64.mpic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h

obj/airattack.c64/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c64 -c $(CFLAGS)  -Osir -Cl -D__AIRATTACK__ -D__CBM__ -o $@ $(subst obj/airattack.c64/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.c64: airattack.embedded.c64 $(subst PLATFORM,airattack.c64,$(OBJS))
	$(CC) -Ln democ64.lbl -t c64 -C cfg/c64.cfg $(LDFLAGS) -m $(EXEDIR)/airattack.c64.map -o $(EXEDIR)/airattack.c64 $(subst PLATFORM,airattack.c64,$(OBJS)) $(LIBDIR)/midres.c64.lib
	$(call RMFILES,$(EXEDIR)/airattack.c64.d64)
	$(CC1541) -f $(EXEDIR)/airattack -w $(EXEDIR)/airattack.c64 $(EXEDIR)/airattack.c64.d64 
	$(CC1541) -f zztiles.bin -w $(DATADIR)/aatiles.bin $(EXEDIR)/airattack.c64.d64 
	$(CC1541) -f zzintro.pic -w $(DATADIR)/aaintro64.mpic $(EXEDIR)/airattack.c64.d64 


# -------------------------------------------------------------------
# --- TOTTO FOR C64 
# -------------------------------------------------------------------

totto.embedded.c64:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -i $(DATADIR)/ttfinal164.mpic -n ttfinal1.mpic -i $(DATADIR)/ttfinal264.mpic -n ttfinal2.mpic -i $(DATADIR)/ttfinal364.mpic -n ttfinal3.mpic -i $(DATADIR)/ttfinal464.mpic -n ttfinal4.mpic -c src/rawdata.c -h src/rawdata.h

obj/totto.c64/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c64 -c $(CFLAGS)  -Osir -Cl -D__TOTTO__ -D__CBM__ -o $@ $(subst obj/totto.c64/,src/,$(@:.o=.c))

$(EXEDIR)/totto.c64: totto.embedded.c64 $(subst PLATFORM,totto.c64,$(OBJS))
	$(CC) -Ln democ64.lbl -t c64 -C cfg/c64.cfg $(LDFLAGS) -m $(EXEDIR)/totto.c64.map -o $(EXEDIR)/totto.c64 $(subst PLATFORM,totto.c64,$(OBJS)) $(LIBDIR)/midres.c64.lib
	$(call RMFILES,$(EXEDIR)/totto.c64.d64)
	$(CC1541) -f $(EXEDIR)/totto -w $(EXEDIR)/totto.c64 $(EXEDIR)/totto.c64.d64 
	$(CC1541) -f tttiles.bin -w $(DATADIR)/tttiles.bin $(EXEDIR)/totto.c64.d64 
	$(CC1541) -f tttiles1.bin -w $(DATADIR)/tttiles1.bin $(EXEDIR)/totto.c64.d64 
	$(CC1541) -f ttfinal1.mpic -w $(DATADIR)/ttfinal164.mpic $(EXEDIR)/totto.c64.d64 
	$(CC1541) -f ttfinal2.mpic -w $(DATADIR)/ttfinal264.mpic $(EXEDIR)/totto.c64.d64 
	$(CC1541) -f ttfinal3.mpic -w $(DATADIR)/ttfinal364.mpic $(EXEDIR)/totto.c64.d64 
	$(CC1541) -f ttfinal4.mpic -w $(DATADIR)/ttfinal464.mpic $(EXEDIR)/totto.c64.d64 


# -------------------------------------------------------------------
# --- JOYCHECK FOR C64 
# -------------------------------------------------------------------

joycheck.embedded.c64:
	$(FILE2INCLUDE) -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -c src/rawdata.c -h src/rawdata.h

obj/joycheck.c64/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c64 -c $(CFLAGS)  -Osir -Cl -D__JOYCHECK__ -D__CBM__ -o $@ $(subst obj/joycheck.c64/,src/,$(@:.o=.c))

$(EXEDIR)/joycheck.c64: joycheck.embedded.c64 $(subst PLATFORM,joycheck.c64,$(OBJS))
	$(CC) -Ln democ64.lbl -t c64 -C cfg/c64.cfg $(LDFLAGS) -m $(EXEDIR)/joycheck.c64.map -o $(EXEDIR)/joycheck.c64 $(subst PLATFORM,joycheck.c64,$(OBJS)) $(LIBDIR)/midres.c64.lib
	$(call RMFILES,$(EXEDIR)/joycheck.c64.d64)
	$(CC1541) -f $(EXEDIR)/joycheck -w $(EXEDIR)/joycheck.c64 $(EXEDIR)/joycheck.c64.d64 
	$(CC1541) -f zdjtiles.bin -w $(DATADIR)/zdjtiles.bin $(EXEDIR)/joycheck.c64.d64 


# -------------------------------------------------------------------
# --- ALIENSTORM FOR C128 
# -------------------------------------------------------------------

alienstorm.embedded.c128:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h

obj/alienstorm.c128/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c128 -c $(CFLAGS)  -Osir -Cl -D__ALIENSTORM__ -D__CBM__ -o $@ $(subst obj/alienstorm.c128/,src/,$(@:.o=.c))

$(EXEDIR)/alienstorm.c128: alienstorm.embedded.c128 $(subst PLATFORM,alienstorm.c128,$(OBJS))
	$(CC) -Ln democ128.lbl -t c128 -C cfg/c128.cfg $(LDFLAGS) -m $(EXEDIR)/alienstorm.c128.map -o $(EXEDIR)/alienstorm.c128 $(subst PLATFORM,alienstorm.c128,$(OBJS)) $(LIBDIR)/midres.c128.lib
	$(call RMFILES,$(EXEDIR)/alienstorm.c128.d64)
	$(CC1541) -f $(EXEDIR)/alienstorm -w $(EXEDIR)/alienstorm.c128 $(EXEDIR)/alienstorm.c128.d64 
	$(CC1541) -f zstiles.bin -w $(DATADIR)/astiles.bin $(EXEDIR)/alienstorm.c128.d64 
	$(CC1541) -f zstiles1.bin -w $(DATADIR)/astiles1.bin $(EXEDIR)/alienstorm.c128.d64 
	$(CC1541) -f zstiles2.bin -w $(DATADIR)/astiles2.bin $(EXEDIR)/alienstorm.c128.d64 


# -------------------------------------------------------------------
# --- ELEVATOR FOR C128 
# -------------------------------------------------------------------

elevator.embedded.c128:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator64.mpic -n zelintro.bin -c src/rawdata.c -h src/rawdata.h

obj/elevator.c128/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c128 -c $(CFLAGS)  -Osir -Cl -D__ELEVATOR__ -D__CBM__ -o $@ $(subst obj/elevator.c128/,src/,$(@:.o=.c))

$(EXEDIR)/elevator.c128: elevator.embedded.c128 $(subst PLATFORM,elevator.c128,$(OBJS))
	$(CC) -Ln democ128.lbl -t c128 -C cfg/c128.cfg $(LDFLAGS) -m $(EXEDIR)/elevator.c128.map -o $(EXEDIR)/elevator.c128 $(subst PLATFORM,elevator.c128,$(OBJS)) $(LIBDIR)/midres.c128.lib
	$(call RMFILES,$(EXEDIR)/elevator.c128.d64)
	$(CC1541) -f $(EXEDIR)/elevator -w $(EXEDIR)/elevator.c128 $(EXEDIR)/elevator.c128.d64 
	$(CC1541) -f zeltiles.bin -w $(DATADIR)/zeltiles.bin $(EXEDIR)/elevator.c128.d64 
	$(CC1541) -f zelintro.bin -w $(DATADIR)/elevator64.mpic $(EXEDIR)/elevator.c128.d64 


# -------------------------------------------------------------------
# --- AIRATTACK FOR C128 
# -------------------------------------------------------------------

airattack.embedded.c128:
	$(FILE2INCLUDE) -i $(DATADIR)/aatiles.bin -n zztiles.bin -i $(DATADIR)/aaintro64.mpic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h

obj/airattack.c128/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c128 -c $(CFLAGS)  -Osir -Cl -D__AIRATTACK__ -D__CBM__ -o $@ $(subst obj/airattack.c128/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.c128: airattack.embedded.c128 $(subst PLATFORM,airattack.c128,$(OBJS))
	$(CC) -Ln democ128.lbl -t c128 -C cfg/c128.cfg $(LDFLAGS) -m $(EXEDIR)/airattack.c128.map -o $(EXEDIR)/airattack.c128 $(subst PLATFORM,airattack.c128,$(OBJS)) $(LIBDIR)/midres.c128.lib
	$(call RMFILES,$(EXEDIR)/airattack.c128.d64)
	$(CC1541) -f $(EXEDIR)/airattack -w $(EXEDIR)/airattack.c128 $(EXEDIR)/airattack.c128.d64 
	$(CC1541) -f zztiles.bin -w $(DATADIR)/aatiles.bin $(EXEDIR)/airattack.c128.d64 
	$(CC1541) -f zzintro.pic -w $(DATADIR)/aaintro64.mpic $(EXEDIR)/airattack.c128.d64 


# -------------------------------------------------------------------
# --- TOTTO FOR C128 
# -------------------------------------------------------------------

totto.embedded.c128:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -i $(DATADIR)/ttfinal164.mpic -n ttfinal1.mpic -i $(DATADIR)/ttfinal264.mpic -n ttfinal2.mpic -i $(DATADIR)/ttfinal364.mpic -n ttfinal3.mpic -i $(DATADIR)/ttfinal464.mpic -n ttfinal4.mpic -c src/rawdata.c -h src/rawdata.h

obj/totto.c128/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c128 -c $(CFLAGS)  -Osir -Cl -D__TOTTO__ -D__CBM__ -o $@ $(subst obj/totto.c128/,src/,$(@:.o=.c))

$(EXEDIR)/totto.c128: totto.embedded.c128 $(subst PLATFORM,totto.c128,$(OBJS))
	$(CC) -Ln democ128.lbl -t c128 -C cfg/c128.cfg $(LDFLAGS) -m $(EXEDIR)/totto.c128.map -o $(EXEDIR)/totto.c128 $(subst PLATFORM,totto.c128,$(OBJS)) $(LIBDIR)/midres.c128.lib
	$(call RMFILES,$(EXEDIR)/totto.c128.d64)
	$(CC1541) -f $(EXEDIR)/totto -w $(EXEDIR)/totto.c128 $(EXEDIR)/totto.c128.d64 
	$(CC1541) -f tttiles.bin -w $(DATADIR)/tttiles.bin $(EXEDIR)/totto.c128.d64 
	$(CC1541) -f tttiles1.bin -w $(DATADIR)/tttiles1.bin $(EXEDIR)/totto.c128.d64 
	$(CC1541) -f ttfinal1.mpic -w $(DATADIR)/ttfinal164.mpic $(EXEDIR)/totto.c128.d64 
	$(CC1541) -f ttfinal2.mpic -w $(DATADIR)/ttfinal264.mpic $(EXEDIR)/totto.c128.d64 
	$(CC1541) -f ttfinal3.mpic -w $(DATADIR)/ttfinal364.mpic $(EXEDIR)/totto.c128.d64 
	$(CC1541) -f ttfinal4.mpic -w $(DATADIR)/ttfinal464.mpic $(EXEDIR)/totto.c128.d64 


# -------------------------------------------------------------------
# --- JOYCHECK FOR C128 
# -------------------------------------------------------------------

joycheck.embedded.c128:
	$(FILE2INCLUDE) -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -c src/rawdata.c -h src/rawdata.h

obj/joycheck.c128/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c128 -c $(CFLAGS)  -Osir -Cl -D__JOYCHECK__ -D__CBM__ -o $@ $(subst obj/joycheck.c128/,src/,$(@:.o=.c))

$(EXEDIR)/joycheck.c128: joycheck.embedded.c128 $(subst PLATFORM,joycheck.c128,$(OBJS))
	$(CC) -Ln democ128.lbl -t c128 -C cfg/c128.cfg $(LDFLAGS) -m $(EXEDIR)/joycheck.c128.map -o $(EXEDIR)/joycheck.c128 $(subst PLATFORM,joycheck.c128,$(OBJS)) $(LIBDIR)/midres.c128.lib
	$(call RMFILES,$(EXEDIR)/joycheck.c128.d64)
	$(CC1541) -f $(EXEDIR)/joycheck -w $(EXEDIR)/joycheck.c128 $(EXEDIR)/joycheck.c128.d64 
	$(CC1541) -f zdjtiles.bin -w $(DATADIR)/zdjtiles.bin $(EXEDIR)/joycheck.c128.d64 


# -------------------------------------------------------------------
# --- ALIENSTORM FOR C16 
# -------------------------------------------------------------------

alienstorm.embedded.c16:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h

obj/alienstorm.c16/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c16 -c $(CFLAGS)  -Osir -Cl -D__ALIENSTORM__ -D__CBM__ -o $@ $(subst obj/alienstorm.c16/,src/,$(@:.o=.c))

$(EXEDIR)/alienstorm.c16: alienstorm.embedded.c16 $(subst PLATFORM,alienstorm.c16,$(OBJS))
	$(CC) -Ln democ16.lbl -t c16 -C cfg/c16.cfg $(LDFLAGS) -m $(EXEDIR)/alienstorm.c16.map -o $(EXEDIR)/alienstorm.c16 $(subst PLATFORM,alienstorm.c16,$(OBJS)) $(LIBDIR)/midres.c16.lib
	$(call RMFILES,$(EXEDIR)/alienstorm.c16.d64)
	$(CC1541) -f $(EXEDIR)/alienstorm -w $(EXEDIR)/alienstorm.c16 $(EXEDIR)/alienstorm.c16.d64 
	$(CC1541) -f zstiles.bin -w $(DATADIR)/astiles.bin $(EXEDIR)/alienstorm.c16.d64 
	$(CC1541) -f zstiles1.bin -w $(DATADIR)/astiles1.bin $(EXEDIR)/alienstorm.c16.d64 
	$(CC1541) -f zstiles2.bin -w $(DATADIR)/astiles2.bin $(EXEDIR)/alienstorm.c16.d64 


# -------------------------------------------------------------------
# --- ELEVATOR FOR C16 
# -------------------------------------------------------------------

elevator.embedded.c16:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator16.mpic -n zelintro.bin -c src/rawdata.c -h src/rawdata.h

obj/elevator.c16/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c16 -c $(CFLAGS)  -Osir -Cl -D__ELEVATOR__ -D__CBM__ -o $@ $(subst obj/elevator.c16/,src/,$(@:.o=.c))

$(EXEDIR)/elevator.c16: elevator.embedded.c16 $(subst PLATFORM,elevator.c16,$(OBJS))
	$(CC) -Ln democ16.lbl -t c16 -C cfg/c16.cfg $(LDFLAGS) -m $(EXEDIR)/elevator.c16.map -o $(EXEDIR)/elevator.c16 $(subst PLATFORM,elevator.c16,$(OBJS)) $(LIBDIR)/midres.c16.lib
	$(call RMFILES,$(EXEDIR)/elevator.c16.d64)
	$(CC1541) -f $(EXEDIR)/elevator -w $(EXEDIR)/elevator.c16 $(EXEDIR)/elevator.c16.d64 
	$(CC1541) -f zeltiles.bin -w $(DATADIR)/zeltiles.bin $(EXEDIR)/elevator.c16.d64 
	$(CC1541) -f zelintro.bin -w $(DATADIR)/elevator16.mpic $(EXEDIR)/elevator.c16.d64 


# -------------------------------------------------------------------
# --- AIRATTACK FOR C16 
# -------------------------------------------------------------------

airattack.embedded.c16:
	$(FILE2INCLUDE) -i $(DATADIR)/aatiles4.bin -n zztiles.bin -i $(DATADIR)/aaintro16.mpic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h

obj/airattack.c16/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c16 -c $(CFLAGS)  -Osir -Cl -D__AIRATTACK__ -D__CBM__ -o $@ $(subst obj/airattack.c16/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.c16: airattack.embedded.c16 $(subst PLATFORM,airattack.c16,$(OBJS))
	$(CC) -Ln democ16.lbl -t c16 -C cfg/c16.cfg $(LDFLAGS) -m $(EXEDIR)/airattack.c16.map -o $(EXEDIR)/airattack.c16 $(subst PLATFORM,airattack.c16,$(OBJS)) $(LIBDIR)/midres.c16.lib
	$(call RMFILES,$(EXEDIR)/airattack.c16.d64)
	$(CC1541) -f $(EXEDIR)/airattack -w $(EXEDIR)/airattack.c16 $(EXEDIR)/airattack.c16.d64 
	$(CC1541) -f zztiles.bin -w $(DATADIR)/aatiles4.bin $(EXEDIR)/airattack.c16.d64 
	$(CC1541) -f zzintro.pic -w $(DATADIR)/aaintro16.mpic $(EXEDIR)/airattack.c16.d64 


# -------------------------------------------------------------------
# --- TOTTO FOR C16 
# -------------------------------------------------------------------

totto.embedded.c16:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -i $(DATADIR)/ttfinal116.mpic -n ttfinal1.mpic -i $(DATADIR)/ttfinal216.mpic -n ttfinal2.mpic -i $(DATADIR)/ttfinal316.mpic -n ttfinal3.mpic -i $(DATADIR)/ttfinal416.mpic -n ttfinal4.mpic -c src/rawdata.c -h src/rawdata.h

obj/totto.c16/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c16 -c $(CFLAGS)  -Osir -Cl -D__TOTTO__ -D__CBM__ -o $@ $(subst obj/totto.c16/,src/,$(@:.o=.c))

$(EXEDIR)/totto.c16: totto.embedded.c16 $(subst PLATFORM,totto.c16,$(OBJS))
	$(CC) -Ln democ16.lbl -t c16 -C cfg/c16.cfg $(LDFLAGS) -m $(EXEDIR)/totto.c16.map -o $(EXEDIR)/totto.c16 $(subst PLATFORM,totto.c16,$(OBJS)) $(LIBDIR)/midres.c16.lib
	$(call RMFILES,$(EXEDIR)/totto.c16.d64)
	$(CC1541) -f $(EXEDIR)/totto -w $(EXEDIR)/totto.c16 $(EXEDIR)/totto.c16.d64 
	$(CC1541) -f tttiles.bin -w $(DATADIR)/tttiles.bin $(EXEDIR)/totto.c16.d64 
	$(CC1541) -f tttiles1.bin -w $(DATADIR)/tttiles1.bin $(EXEDIR)/totto.c16.d64 
	$(CC1541) -f ttfinal1.mpic -w $(DATADIR)/ttfinal116.mpic $(EXEDIR)/totto.c16.d64 
	$(CC1541) -f ttfinal2.mpic -w $(DATADIR)/ttfinal216.mpic $(EXEDIR)/totto.c16.d64 
	$(CC1541) -f ttfinal3.mpic -w $(DATADIR)/ttfinal316.mpic $(EXEDIR)/totto.c16.d64 
	$(CC1541) -f ttfinal4.mpic -w $(DATADIR)/ttfinal416.mpic $(EXEDIR)/totto.c16.d64 

                    
#!!! missing resources for joycheck (c16)


# -------------------------------------------------------------------
# --- ALIENSTORM FOR PLUS4 
# -------------------------------------------------------------------

alienstorm.embedded.plus4:
	$(FILE2INCLUDE) -i $(DATADIR)/astiles.bin -n zstiles.bin -i $(DATADIR)/astiles1.bin -n zstiles1.bin -i $(DATADIR)/astiles2.bin -n zstiles2.bin -c src/rawdata.c -h src/rawdata.h

obj/alienstorm.plus4/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t plus4 -c $(CFLAGS)  -Osir -Cl -D__ALIENSTORM__ -D__CBM__ -o $@ $(subst obj/alienstorm.plus4/,src/,$(@:.o=.c))

$(EXEDIR)/alienstorm.plus4: alienstorm.embedded.plus4 $(subst PLATFORM,alienstorm.plus4,$(OBJS))
	$(CC) -Ln demoplus4.lbl -t plus4 -C cfg/plus4.cfg $(LDFLAGS) -m $(EXEDIR)/alienstorm.plus4.map -o $(EXEDIR)/alienstorm.plus4 $(subst PLATFORM,alienstorm.plus4,$(OBJS)) $(LIBDIR)/midres.plus4.lib
	$(call RMFILES,$(EXEDIR)/alienstorm.plus4.d64)
	$(CC1541) -f $(EXEDIR)/alienstorm -w $(EXEDIR)/alienstorm.plus4 $(EXEDIR)/alienstorm.plus4.d64 
	$(CC1541) -f zstiles.bin -w $(DATADIR)/astiles.bin $(EXEDIR)/alienstorm.plus4.d64 
	$(CC1541) -f zstiles1.bin -w $(DATADIR)/astiles1.bin $(EXEDIR)/alienstorm.plus4.d64 
	$(CC1541) -f zstiles2.bin -w $(DATADIR)/astiles2.bin $(EXEDIR)/alienstorm.plus4.d64 


# -------------------------------------------------------------------
# --- ELEVATOR FOR PLUS4 
# -------------------------------------------------------------------

elevator.embedded.plus4:
	$(FILE2INCLUDE) -i $(DATADIR)/zeltiles.bin -n zeltiles.bin -i $(DATADIR)/elevator16.mpic -n zelintro.bin -c src/rawdata.c -h src/rawdata.h

obj/elevator.plus4/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t plus4 -c $(CFLAGS)  -Osir -Cl -D__ELEVATOR__ -D__CBM__ -o $@ $(subst obj/elevator.plus4/,src/,$(@:.o=.c))

$(EXEDIR)/elevator.plus4: elevator.embedded.plus4 $(subst PLATFORM,elevator.plus4,$(OBJS))
	$(CC) -Ln demoplus4.lbl -t plus4 -C cfg/plus4.cfg $(LDFLAGS) -m $(EXEDIR)/elevator.plus4.map -o $(EXEDIR)/elevator.plus4 $(subst PLATFORM,elevator.plus4,$(OBJS)) $(LIBDIR)/midres.plus4.lib
	$(call RMFILES,$(EXEDIR)/elevator.plus4.d64)
	$(CC1541) -f $(EXEDIR)/elevator -w $(EXEDIR)/elevator.plus4 $(EXEDIR)/elevator.plus4.d64 
	$(CC1541) -f zeltiles.bin -w $(DATADIR)/zeltiles.bin $(EXEDIR)/elevator.plus4.d64 
	$(CC1541) -f zelintro.bin -w $(DATADIR)/elevator16.mpic $(EXEDIR)/elevator.plus4.d64 


# -------------------------------------------------------------------
# --- AIRATTACK FOR PLUS4 
# -------------------------------------------------------------------

airattack.embedded.plus4:
	$(FILE2INCLUDE) -i $(DATADIR)/aatiles4.bin -n zztiles.bin -i $(DATADIR)/aaintro16.mpic -n zzintro.pic -c src/rawdata.c -h src/rawdata.h

obj/airattack.plus4/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t plus4 -c $(CFLAGS)  -Osir -Cl -D__AIRATTACK__ -D__CBM__ -o $@ $(subst obj/airattack.plus4/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.plus4: airattack.embedded.plus4 $(subst PLATFORM,airattack.plus4,$(OBJS))
	$(CC) -Ln demoplus4.lbl -t plus4 -C cfg/plus4.cfg $(LDFLAGS) -m $(EXEDIR)/airattack.plus4.map -o $(EXEDIR)/airattack.plus4 $(subst PLATFORM,airattack.plus4,$(OBJS)) $(LIBDIR)/midres.plus4.lib
	$(call RMFILES,$(EXEDIR)/airattack.plus4.d64)
	$(CC1541) -f $(EXEDIR)/airattack -w $(EXEDIR)/airattack.plus4 $(EXEDIR)/airattack.plus4.d64 
	$(CC1541) -f zztiles.bin -w $(DATADIR)/aatiles4.bin $(EXEDIR)/airattack.plus4.d64 
	$(CC1541) -f zzintro.pic -w $(DATADIR)/aaintro16.mpic $(EXEDIR)/airattack.plus4.d64 


# -------------------------------------------------------------------
# --- TOTTO FOR PLUS4 
# -------------------------------------------------------------------

totto.embedded.plus4:
	$(FILE2INCLUDE) -i $(DATADIR)/tttiles.bin -n tttiles.bin -i $(DATADIR)/tttiles1.bin -n tttiles1.bin -i $(DATADIR)/ttfinal116.mpic -n ttfinal1.mpic -i $(DATADIR)/ttfinal216.mpic -n ttfinal2.mpic -i $(DATADIR)/ttfinal316.mpic -n ttfinal3.mpic -i $(DATADIR)/ttfinal416.mpic -n ttfinal4.mpic -c src/rawdata.c -h src/rawdata.h

obj/totto.plus4/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t plus4 -c $(CFLAGS)  -Osir -Cl -D__TOTTO__ -D__CBM__ -o $@ $(subst obj/totto.plus4/,src/,$(@:.o=.c))

$(EXEDIR)/totto.plus4: totto.embedded.plus4 $(subst PLATFORM,totto.plus4,$(OBJS))
	$(CC) -Ln demoplus4.lbl -t plus4 -C cfg/plus4.cfg $(LDFLAGS) -m $(EXEDIR)/totto.plus4.map -o $(EXEDIR)/totto.plus4 $(subst PLATFORM,totto.plus4,$(OBJS)) $(LIBDIR)/midres.plus4.lib
	$(call RMFILES,$(EXEDIR)/totto.plus4.d64)
	$(CC1541) -f $(EXEDIR)/totto -w $(EXEDIR)/totto.plus4 $(EXEDIR)/totto.plus4.d64 
	$(CC1541) -f tttiles.bin -w $(DATADIR)/tttiles.bin $(EXEDIR)/totto.plus4.d64 
	$(CC1541) -f tttiles1.bin -w $(DATADIR)/tttiles1.bin $(EXEDIR)/totto.plus4.d64 
	$(CC1541) -f ttfinal1.mpic -w $(DATADIR)/ttfinal116.mpic $(EXEDIR)/totto.plus4.d64 
	$(CC1541) -f ttfinal2.mpic -w $(DATADIR)/ttfinal216.mpic $(EXEDIR)/totto.plus4.d64 
	$(CC1541) -f ttfinal3.mpic -w $(DATADIR)/ttfinal316.mpic $(EXEDIR)/totto.plus4.d64 
	$(CC1541) -f ttfinal4.mpic -w $(DATADIR)/ttfinal416.mpic $(EXEDIR)/totto.plus4.d64 


# -------------------------------------------------------------------
# --- JOYCHECK FOR PLUS4 
# -------------------------------------------------------------------

joycheck.embedded.plus4:
	$(FILE2INCLUDE) -i $(DATADIR)/zdjtiles.bin -n zdjtiles.bin -c src/rawdata.c -h src/rawdata.h

obj/joycheck.plus4/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t plus4 -c $(CFLAGS)  -Osir -Cl -D__JOYCHECK__ -D__CBM__ -o $@ $(subst obj/joycheck.plus4/,src/,$(@:.o=.c))

$(EXEDIR)/joycheck.plus4: joycheck.embedded.plus4 $(subst PLATFORM,joycheck.plus4,$(OBJS))
	$(CC) -Ln demoplus4.lbl -t plus4 -C cfg/plus4.cfg $(LDFLAGS) -m $(EXEDIR)/joycheck.plus4.map -o $(EXEDIR)/joycheck.plus4 $(subst PLATFORM,joycheck.plus4,$(OBJS)) $(LIBDIR)/midres.plus4.lib
	$(call RMFILES,$(EXEDIR)/joycheck.plus4.d64)
	$(CC1541) -f $(EXEDIR)/joycheck -w $(EXEDIR)/joycheck.plus4 $(EXEDIR)/joycheck.plus4.d64 
	$(CC1541) -f zdjtiles.bin -w $(DATADIR)/zdjtiles.bin $(EXEDIR)/joycheck.plus4.d64 

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
