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

# Enter the name of the program here. This is also the name of any secondary 
# files, such as disk or cassette images.
PROGRAMNAME := midres

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
#  - atari: single executable for ATARI 800 (named on disk: "midres.exe")
#  - ataricol: single executable for ATARI XL (named on disk: "midres.exe")

TARGETS := $(target)

# Given demonstrations:
#  - SLIDESHOW - a slideshow with some images converted using img2midres
#  - DRAWING - an animation using drawing primitives (v1.1)
#  - BITBLIT - an animation using bit blits primivites (v1.2)
#  - TILE - an animation using tiles primivites (v1.3)
DEMO := $(demo)

# Given tutorials:
#  - MCTILE
TUTORIAL := $(tutorial)

#ATARGETS := airattack.c64 airattack.vic2024 airattack.plus4 airattack.c128 airattack.atari
#ATARGETS += totto.c64 totto.vic2024 totto.plus4 totto.c128

ATARGETS := $(game).$(target)

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
else
  CC := cl65
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

###############################################################################
## COMPILATION / LINKING OPTIONS
###############################################################################

CFLAGS := -D__DEMO_$(DEMO)__ -D__GAME_$(GAME)__ -D__TUTORIAL_$(TUTORIAL)__
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
SOURCES := $(wildcard src/*.c)

# Let's calculate what the names of the object files could be. Usually, there 
# will be one for each source. Object files are stored in a separate location 
# for each target environment. 
OBJS := $(addsuffix .o,$(basename $(addprefix obj/PLATFORM/,$(SOURCES:src/%=%))))

# Here we expand every single object produced, according to each expected 
# environment. In this way you get the complete list of all object files to be 
# compiled separately, each according to the compiler suitable for that 
# environment.
OBJECTS := $(foreach TARGET,$(TARGETS),$(subst PLATFORM,$(TARGET),$(OBJS)))
OBJECTS += $(foreach ATARGET,$(ATARGETS),$(subst PLATFORM,$(ATARGET),$(OBJS)))

# We generate the list of paths where the object files for each target will 
# end, so that we can generate them in advance (as paths).
TARGETOBJDIR := $(foreach TARGET,$(TARGETS),obj/$(TARGET))
TARGETOBJDIR += $(foreach ATARGET,$(ATARGETS),obj/$(ATARGET))

# This is the path where all executables will be put.
EXEDIR := exe

# Similarly, we expand the set of executables that are required.
EXES := $(foreach TARGET,$(TARGETS),$(EXEDIR)/$(PROGRAMNAME).$(TARGET))
EXES += $(foreach ATARGET,$(ATARGETS),$(EXEDIR)/$(ATARGET))

# This is the path where all data will be put.
DATADIR := data

###############################################################################
## PLATFORMS' RULES
###############################################################################

##-----------------------------------------------------------------------------
## CC65
##-----------------------------------------------------------------------------

## C64 ------------------------------------------------------------------------

# Let's define rules to compile the demo under C=64 as a one and single 
# executable file. This compilation is used as a "functional check", to
# be sure that the source implementation is correct. Moreover, the executable 
# file will be put on a D64 1541 image, along with the overlay version
# generated by other rules.
obj/c64/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c64 -c $(CFLAGS) -Osir -Cl -D__CBM__ -o $@ $(subst obj/c64/,src/,$(@:.o=.c))

$(EXEDIR)/$(PROGRAMNAME).c64:	$(subst PLATFORM,c64,$(OBJS))
	$(CC) -Ln demo64.lbl -t c64 $(LDFLAGS) -m $(EXEDIR)/$(PROGRAMNAME).c64.map -o $(EXEDIR)/$(PROGRAMNAME).c64 $(subst PLATFORM,c64,$(OBJS))
	$(CC1541) -f $(PROGRAMNAME)-single -w $(EXEDIR)/$(PROGRAMNAME).c64 $(EXEDIR)/$(PROGRAMNAME).c64.d64  
	$(CC1541) -f slideshow -w $(DATADIR)/slideshow64.dat $(EXEDIR)/$(PROGRAMNAME).c64.d64  
	$(CC1541) -f image6401.mpic -w $(DATADIR)/image6401.mpic $(EXEDIR)/$(PROGRAMNAME).c64.d64  
	$(CC1541) -f image6402.mpic -w $(DATADIR)/image6402.mpic $(EXEDIR)/$(PROGRAMNAME).c64.d64  
	$(CC1541) -f image6403.mpic -w $(DATADIR)/image6403.mpic $(EXEDIR)/$(PROGRAMNAME).c64.d64  
	$(CC1541) -f image6404.mpic -w $(DATADIR)/image6404.mpic $(EXEDIR)/$(PROGRAMNAME).c64.d64  
	$(CC1541) -f tiles.bin -w $(DATADIR)/tiles.bin $(EXEDIR)/$(PROGRAMNAME).c64.d64  
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles.bin $(EXEDIR)/$(PROGRAMNAME).c64.d64  
	$(CC1541) -f aaintro.mpic -w $(DATADIR)/aaintro64.mpic $(EXEDIR)/$(PROGRAMNAME).c64.d64  
	$(CC1541) -f mctile.bin -w $(DATADIR)/tutorial_mctile.bin $(EXEDIR)/$(PROGRAMNAME).c64.d64  

# Let's define rules to compile the demo under C=64 as the overlay version.
# Moreover, all the executable files will be put on a D64 1541 image, 
# along with the single file version.
obj/c64ovl/%.o:	$(SOURCES)
	$(CC) -t c64 -T -l $(@:.o=.asm) -c $(CFLAGS) -D__CBM__ -D__OVERLAY__ -D__OVERLAY__MIDRES__ -o $@ $(subst obj/c64ovl/,src/,$(@:.o=.c)) 

# This rule will produce the final binary file for C=64 platform.
$(EXEDIR)/$(PROGRAMNAME).c64ovl:	$(subst PLATFORM,c64ovl,$(OBJS))
	$(CC) -Ln demo64.lbl -t c64 $(LDFLAGS) -m $(EXEDIR)/$(PROGRAMNAME).c64ovl.map -C cfg/c64-overlay.cfg  -o $(EXEDIR)/$(PROGRAMNAME).c64ovl $(subst PLATFORM,c64ovl,$(OBJS))
	$(CC1541) -f $(PROGRAMNAME) -w $(EXEDIR)/$(PROGRAMNAME).c64ovl $(EXEDIR)/$(PROGRAMNAME).c64ovl.d64  
	$(CC1541) -f mr1 -w $(EXEDIR)/$(PROGRAMNAME).c64ovl.screen $(EXEDIR)/$(PROGRAMNAME).c64ovl.d64  
	$(CC1541) -f mr2 -w $(EXEDIR)/$(PROGRAMNAME).c64ovl.drawing $(EXEDIR)/$(PROGRAMNAME).c64ovl.d64  
	$(CC1541) -f mr3 -w $(EXEDIR)/$(PROGRAMNAME).c64ovl.screen2 $(EXEDIR)/$(PROGRAMNAME).c64ovl.d64  
	$(CC1541) -f mr4 -w $(EXEDIR)/$(PROGRAMNAME).c64ovl.drawing2 $(EXEDIR)/$(PROGRAMNAME).c64ovl.d64  
	$(CC1541) -f mr5 -w $(EXEDIR)/$(PROGRAMNAME).c64ovl.bitblit $(EXEDIR)/$(PROGRAMNAME).c64ovl.d64  
	$(CC1541) -f slideshow -w $(DATADIR)/slideshow64.dat $(EXEDIR)/$(PROGRAMNAME).c64ovl.d64  
	$(CC1541) -f image6401.mpic -w $(DATADIR)/image6401.mpic $(EXEDIR)/$(PROGRAMNAME).c64ovl.d64  
	$(CC1541) -f image6402.mpic -w $(DATADIR)/image6402.mpic $(EXEDIR)/$(PROGRAMNAME).c64ovl.d64  
	$(CC1541) -f image6403.mpic -w $(DATADIR)/image6403.mpic $(EXEDIR)/$(PROGRAMNAME).c64ovl.d64  
	$(CC1541) -f image6404.mpic -w $(DATADIR)/image6404.mpic $(EXEDIR)/$(PROGRAMNAME).c64ovl.d64  
	$(CC1541) -f tiles.bin -w $(DATADIR)/tiles.bin $(EXEDIR)/$(PROGRAMNAME).c64ovl.d64  
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles.bin $(EXEDIR)/$(PROGRAMNAME).c64ovl.d64  
	$(CC1541) -f aaintro.mpic -w $(DATADIR)/aaintro64.mpic $(EXEDIR)/$(PROGRAMNAME).c64ovl.d64  
	$(CC1541) -f mctile.bin -w $(DATADIR)/tutorial_mctile.bin $(EXEDIR)/$(PROGRAMNAME).c64ovl.d64  

#$(CC1541) -f test.pic -w $(DATADIR)/test.pic.prg $(EXEDIR)/$(PROGRAMNAME).c64.d64  
#$(CC1541) -f test.col -w $(DATADIR)/test.col.prg $(EXEDIR)/$(PROGRAMNAME).c64.d64  
#$(CC1541) -f test.cpic -w $(DATADIR)/test.cpic.prg $(EXEDIR)/$(PROGRAMNAME).c64.d64  

## VIC20 ------------------------------------------------------------------------

# Let's define rules to compile the demo under VIC 20 as a one and single 
# executable file. This compilation will fails since there is no enough RAM.
obj/vic20/%.o:	$(SOURCES)
	$(CC) -t vic20 -c $(CFLAGS) -Osir -Cl -T -l $(@:.o=.map) -D__CBM__ -C cfg/vic20.cfg -o $@ $(subst obj/vic20/,src/,$(@:.o=.c))

obj/vic20/midres_vic20s.o:	$(SOURCES)
	$(CA) -t vic20 -o $@ $(subst obj/vic20/,src/,$(@:.o=.s))

$(EXEDIR)/$(PROGRAMNAME).vic20:	$(subst PLATFORM,vic20,$(OBJS))
	$(CC) -Ln demo20.lbl -t vic20 $(LDFLAGS) -m $(EXEDIR)/$(PROGRAMNAME).vic20.map -C cfg/vic20.cfg -o $(EXEDIR)/$(PROGRAMNAME).vic20 $(subst PLATFORM,vic20,$(OBJS))
	$(CC1541) -f $(PROGRAMNAME)-single -w $(EXEDIR)/$(PROGRAMNAME).vic20 $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f slideshow -w $(DATADIR)/slideshow20.dat $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f image2001.mpic -w $(DATADIR)/image2001.mpic $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f image2002.mpic -w $(DATADIR)/image2002.mpic $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f image2003.mpic -w $(DATADIR)/image2003.mpic $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f image2004.mpic -w $(DATADIR)/image2004.mpic $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f tiles.bin -w $(DATADIR)/tiles.bin $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles20.bin $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f aaintro.mpic -w $(DATADIR)/aaintro20.mpic $(EXEDIR)/$(PROGRAMNAME).vic20.d64  

# Let's define rules to compile the demo under VIC 20 as a one and single 
# executable file. This compilation will fails since there is no enough RAM.
obj/vic2024/%.o:	$(SOURCES)
	$(CC) -t vic20 -c $(CFLAGS) -Osir -Cl -T -l $(@:.o=.map) -D__24K__ -D__CBM__ -C cfg/vic20-32k.cfg -o $@ $(subst obj/vic2024/,src/,$(@:.o=.c))

$(EXEDIR)/$(PROGRAMNAME).vic2024:	$(subst PLATFORM,vic2024,$(OBJS))
	$(CC) -Ln demo20.lbl -t vic20 $(LDFLAGS) -m $(EXEDIR)/$(PROGRAMNAME).vic2024.map -C cfg/vic20-32k.cfg -o $(EXEDIR)/$(PROGRAMNAME).vic2024 $(subst PLATFORM,vic2024,$(OBJS))
	$(CC1541) -f loader -w $(DATADIR)/loader2024.prg $(EXEDIR)/$(PROGRAMNAME).vic2024.d64  
	$(CC1541) -f $(PROGRAMNAME)-single -w $(EXEDIR)/$(PROGRAMNAME).vic2024 $(EXEDIR)/$(PROGRAMNAME).vic2024.d64  
	$(CC1541) -f slideshow -w $(DATADIR)/slideshow20.dat $(EXEDIR)/$(PROGRAMNAME).vic2024.d64  
	$(CC1541) -f image2001.mpic -w $(DATADIR)/image2001.mpic $(EXEDIR)/$(PROGRAMNAME).vic2024.d64  
	$(CC1541) -f image2002.mpic -w $(DATADIR)/image2002.mpic $(EXEDIR)/$(PROGRAMNAME).vic2024.d64  
	$(CC1541) -f image2003.mpic -w $(DATADIR)/image2003.mpic $(EXEDIR)/$(PROGRAMNAME).vic2024.d64  
	$(CC1541) -f image2004.mpic -w $(DATADIR)/image2004.mpic $(EXEDIR)/$(PROGRAMNAME).vic2024.d64  
	$(CC1541) -f tiles.bin -w $(DATADIR)/tiles.bin $(EXEDIR)/$(PROGRAMNAME).vic2024.d64  
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles20.bin $(EXEDIR)/$(PROGRAMNAME).vic2024.d64  
	$(CC1541) -f aaintro.mpic -w $(DATADIR)/aaintro20.mpic $(EXEDIR)/$(PROGRAMNAME).vic2024.d64  
	$(CC1541) -f mctile.bin -w $(DATADIR)/tutorial_mctile.bin $(EXEDIR)/$(PROGRAMNAME).vic2024.d64  

# Let's define rules to compile the demo under VIC20 as the overlay version.
# This is the only way to compile this program in order to be able to be 
# executed by this platform. All the executable files will be put on a 
# D64 1541 image.
obj/vic20ovl/%.o:	$(SOURCES)
	$(CC) -t vic20 -c $(CFLAGS) -Osir -Cl -T -l $(@:.o=.map) -D__CBM__ -D__OVERLAY__ -D__OVERLAY__MIDRES__ -C cfg/vic20-overlay.cfg -o $@ $(subst obj/vic20ovl/,src/,$(@:.o=.c))

$(EXEDIR)/$(PROGRAMNAME).vic20ovl:	$(subst PLATFORM,vic20ovl,$(OBJS))
	$(CC) -Ln demo20.lbl -t vic20 $(LDFLAGS) -m $(EXEDIR)/$(PROGRAMNAME).vic20ovl.map -C cfg/vic20-overlay.cfg  -o $(EXEDIR)/$(PROGRAMNAME).vic20ovl $(subst PLATFORM,vic20ovl,$(OBJS))
	$(CC1541) -f $(PROGRAMNAME) -w $(EXEDIR)/$(PROGRAMNAME).vic20ovl $(EXEDIR)/$(PROGRAMNAME).vic20ovl.d64  
	$(CC1541) -f mr1 -w $(EXEDIR)/$(PROGRAMNAME).vic20ovl.screen $(EXEDIR)/$(PROGRAMNAME).vic20ovl.d64  
	$(CC1541) -f mr2 -w $(EXEDIR)/$(PROGRAMNAME).vic20ovl.drawing $(EXEDIR)/$(PROGRAMNAME).vic20ovl.d64  
	$(CC1541) -f mr3 -w $(EXEDIR)/$(PROGRAMNAME).vic20ovl.screen2 $(EXEDIR)/$(PROGRAMNAME).vic20ovl.d64  
	$(CC1541) -f mr4 -w $(EXEDIR)/$(PROGRAMNAME).vic20ovl.drawing2 $(EXEDIR)/$(PROGRAMNAME).vic20ovl.d64  
	$(CC1541) -f mr5 -w $(EXEDIR)/$(PROGRAMNAME).vic20ovl.bitblit $(EXEDIR)/$(PROGRAMNAME).vic20ovl.d64  
	$(CC1541) -f slideshow -w $(DATADIR)/slideshow20.dat $(EXEDIR)/$(PROGRAMNAME).vic20ovl.d64  
	$(CC1541) -f image2001.mpic -w $(DATADIR)/image2001.mpic $(EXEDIR)/$(PROGRAMNAME).vic20ovl.d64  
	$(CC1541) -f image2002.mpic -w $(DATADIR)/image2002.mpic $(EXEDIR)/$(PROGRAMNAME).vic20ovl.d64  
	$(CC1541) -f image2003.mpic -w $(DATADIR)/image2003.mpic $(EXEDIR)/$(PROGRAMNAME).vic20ovl.d64  
	$(CC1541) -f image2004.mpic -w $(DATADIR)/image2004.mpic $(EXEDIR)/$(PROGRAMNAME).vic20ovl.d64  
	$(CC1541) -f tiles.bin -w $(DATADIR)/tiles.bin $(EXEDIR)/$(PROGRAMNAME).vic20ovl.d64  
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles20.bin $(EXEDIR)/$(PROGRAMNAME).vic20ovl.d64  
	$(CC1541) -f aaintro.mpic -w $(DATADIR)/aaintro20.mpic $(EXEDIR)/$(PROGRAMNAME).vic20ovl.d64  
	$(CC1541) -f mctile.bin -w $(DATADIR)/tutorial_mctile.bin $(EXEDIR)/$(PROGRAMNAME).vic20ovl.d64  

## C16 ------------------------------------------------------------------------

# Let's define rules to compile the demo under C16 as a one and single 
# executable file. This compilation is used as a "functional check", to
# be sure that the source implementation is correct. Moreover, the executable 
# file will be put on a D64 1541 image, along with the overlay version
# generated by other rules.
obj/c16/%.o:	$(SOURCES)
	$(CC) -t c16 -c $(CFLAGS) -Osir -Cl -T -l $(@:.o=.map) -D__CBM__ -C cfg/c16.cfg -o $@ $(subst obj/c16/,src/,$(@:.o=.c))

$(EXEDIR)/$(PROGRAMNAME).c16:	$(subst PLATFORM,c16,$(OBJS))
	$(CC) -Ln demo16.lbl -t c16 $(LDFLAGS) -m $(EXEDIR)/$(PROGRAMNAME).c16.map -C cfg/c16.cfg -o $(EXEDIR)/$(PROGRAMNAME).c16 $(subst PLATFORM,c16,$(OBJS))
	$(CC1541) -f $(PROGRAMNAME)-single -w $(EXEDIR)/$(PROGRAMNAME).c16 $(EXEDIR)/$(PROGRAMNAME).c16.d64  
	$(CC1541) -f slideshow -w $(DATADIR)/slideshow16.dat $(EXEDIR)/$(PROGRAMNAME).c16.d64  
	$(CC1541) -f image1601.mpic -w $(DATADIR)/image1601.mpic $(EXEDIR)/$(PROGRAMNAME).c16.d64  
	$(CC1541) -f image1602.mpic -w $(DATADIR)/image1602.mpic $(EXEDIR)/$(PROGRAMNAME).c16.d64  
	$(CC1541) -f image1603.mpic -w $(DATADIR)/image1603.mpic $(EXEDIR)/$(PROGRAMNAME).c16.d64  
	$(CC1541) -f image1604.mpic -w $(DATADIR)/image1604.mpic $(EXEDIR)/$(PROGRAMNAME).c16.d64  
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles.bin $(EXEDIR)/$(PROGRAMNAME).c16.d64  
	$(CC1541) -f aaintro.mpic -w $(DATADIR)/aaintro16.mpic $(EXEDIR)/$(PROGRAMNAME).c16.d64  
	$(CC1541) -f mctile.bin -w $(DATADIR)/tutorial_mctile.bin $(EXEDIR)/$(PROGRAMNAME).c16.d64  

## PLUS4 ------------------------------------------------------------------------

# Let's define rules to compile the demo under C16 as a one and single 
# executable file. This compilation is used as a "functional check", to
# be sure that the source implementation is correct. Moreover, the executable 
# file will be put on a D64 1541 image, along with the overlay version
# generated by other rules.
obj/plus4/%.o:	$(SOURCES)
	$(CC) -t plus4 -c $(CFLAGS) -Osir -Cl -T -l $(@:.o=.map) -D__CBM__ -C cfg/plus4.cfg -o $@ $(subst obj/plus4/,src/,$(@:.o=.c))

$(EXEDIR)/$(PROGRAMNAME).plus4:	$(subst PLATFORM,plus4,$(OBJS))
	$(CC) -Ln demo4.lbl -t plus4 $(LDFLAGS) -m $(EXEDIR)/$(PROGRAMNAME).plus4.map -C cfg/plus4.cfg -o $(EXEDIR)/$(PROGRAMNAME).plus4 $(subst PLATFORM,plus4,$(OBJS))
	$(CC1541) -f loader -w $(DATADIR)/loader4.prg $(EXEDIR)/$(PROGRAMNAME).plus4.d64  
	$(CC1541) -f $(PROGRAMNAME)-single -w $(EXEDIR)/$(PROGRAMNAME).plus4 $(EXEDIR)/$(PROGRAMNAME).plus4.d64  
	$(CC1541) -f slideshow -w $(DATADIR)/slideshow16.dat $(EXEDIR)/$(PROGRAMNAME).plus4.d64  
	$(CC1541) -f image1601.mpic -w $(DATADIR)/image1601.mpic $(EXEDIR)/$(PROGRAMNAME).plus4.d64  
	$(CC1541) -f image1602.mpic -w $(DATADIR)/image1602.mpic $(EXEDIR)/$(PROGRAMNAME).plus4.d64  
	$(CC1541) -f image1603.mpic -w $(DATADIR)/image1603.mpic $(EXEDIR)/$(PROGRAMNAME).plus4.d64  
	$(CC1541) -f image1604.mpic -w $(DATADIR)/image1604.mpic $(EXEDIR)/$(PROGRAMNAME).plus4.d64  
	$(CC1541) -f tiles.bin -w $(DATADIR)/tiles.bin $(EXEDIR)/$(PROGRAMNAME).plus4.d64  
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles4.bin $(EXEDIR)/$(PROGRAMNAME).plus4.d64  
	$(CC1541) -f aaintro.mpic -w $(DATADIR)/aaintro16.mpic $(EXEDIR)/$(PROGRAMNAME).plus4.d64  
	$(CC1541) -f mctile.bin -w $(DATADIR)/tutorial_mctile.bin $(EXEDIR)/$(PROGRAMNAME).plus4.d64  

## C128 ------------------------------------------------------------------------

# Let's define rules to compile the demo under C=128 as a one and single 
# executable file. This compilation is used as a "functional check", to
# be sure that the source implementation is correct. Moreover, the executable 
# file will be put on a D64 1541 image, along with the overlay version
# generated by other rules.
obj/c128/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t c128 -c $(CFLAGS) -Osir -Cl -D__CBM__ -o $@ $(subst obj/c128/,src/,$(@:.o=.c))

$(EXEDIR)/$(PROGRAMNAME).c128:	$(subst PLATFORM,c128,$(OBJS))
	$(CC) -Ln demo128.lbl -t c128 $(LDFLAGS) -m $(EXEDIR)/$(PROGRAMNAME).c128.map -o $(EXEDIR)/$(PROGRAMNAME).c128 $(subst PLATFORM,c128,$(OBJS))
	$(CC1541) -f $(PROGRAMNAME)-single -w $(EXEDIR)/$(PROGRAMNAME).c128 $(EXEDIR)/$(PROGRAMNAME).c128.d64  
	$(CC1541) -f slideshow -w $(DATADIR)/slideshow64.dat $(EXEDIR)/$(PROGRAMNAME).c128.d64  
	$(CC1541) -f image6401.mpic -w $(DATADIR)/image6401.mpic $(EXEDIR)/$(PROGRAMNAME).c128.d64  
	$(CC1541) -f image6402.mpic -w $(DATADIR)/image6402.mpic $(EXEDIR)/$(PROGRAMNAME).c128.d64  
	$(CC1541) -f image6403.mpic -w $(DATADIR)/image6403.mpic $(EXEDIR)/$(PROGRAMNAME).c128.d64  
	$(CC1541) -f image6404.mpic -w $(DATADIR)/image6404.mpic $(EXEDIR)/$(PROGRAMNAME).c128.d64  
	$(CC1541) -f tiles.bin -w $(DATADIR)/tiles.bin $(EXEDIR)/$(PROGRAMNAME).c128.d64  
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles.bin $(EXEDIR)/$(PROGRAMNAME).c128.d64  
	$(CC1541) -f aaintro.mpic -w $(DATADIR)/aaintro64.mpic $(EXEDIR)/$(PROGRAMNAME).c128.d64  
	$(CC1541) -f mctile.bin -w $(DATADIR)/tutorial_mctile.bin $(EXEDIR)/$(PROGRAMNAME).c128.d64  

# Let's define rules to compile the demo under C=128 as the overlay version.
# Moreover, all the executable files will be put on a D64 1541 image, 
# along with the single file version.
obj/c128ovl/%.o:	$(SOURCES)
	$(CC) -t c128 -T -l $(@:.o=.asm) -c $(CFLAGS) -D__CBM__ -D__OVERLAY__ -D__OVERLAY__MIDRES__ -o $@ $(subst obj/c128ovl/,src/,$(@:.o=.c)) 

# This rule will produce the final binary file for C=64 platform.
$(EXEDIR)/$(PROGRAMNAME).c128ovl:	$(subst PLATFORM,c128ovl,$(OBJS))
	$(CC) -Ln demo128.lbl -t c128 $(LDFLAGS) -m $(EXEDIR)/$(PROGRAMNAME).c128ovl.map -C cfg/c128-overlay.cfg  -o $(EXEDIR)/$(PROGRAMNAME).c128ovl $(subst PLATFORM,c128ovl,$(OBJS))
	$(CC1541) -f $(PROGRAMNAME) -w $(EXEDIR)/$(PROGRAMNAME).c128ovl $(EXEDIR)/$(PROGRAMNAME).c128ovl.d64  
	$(CC1541) -f mr1 -w $(EXEDIR)/$(PROGRAMNAME).c128ovl.screen $(EXEDIR)/$(PROGRAMNAME).c128ovl.d64  
	$(CC1541) -f mr2 -w $(EXEDIR)/$(PROGRAMNAME).c128ovl.drawing $(EXEDIR)/$(PROGRAMNAME).c128ovl.d64  
	$(CC1541) -f mr3 -w $(EXEDIR)/$(PROGRAMNAME).c128ovl.screen2 $(EXEDIR)/$(PROGRAMNAME).c128ovl.d64  
	$(CC1541) -f mr4 -w $(EXEDIR)/$(PROGRAMNAME).c128ovl.drawing2 $(EXEDIR)/$(PROGRAMNAME).c128ovl.d64  
	$(CC1541) -f mr5 -w $(EXEDIR)/$(PROGRAMNAME).c128ovl.bitblit $(EXEDIR)/$(PROGRAMNAME).c128ovl.d64  
	$(CC1541) -f slideshow -w $(DATADIR)/slideshow64.dat $(EXEDIR)/$(PROGRAMNAME).c128ovl.d64  
	$(CC1541) -f image6401.mpic -w $(DATADIR)/image6401.mpic $(EXEDIR)/$(PROGRAMNAME).c128ovl.d64  
	$(CC1541) -f image6402.mpic -w $(DATADIR)/image6402.mpic $(EXEDIR)/$(PROGRAMNAME).c128ovl.d64  
	$(CC1541) -f image6403.mpic -w $(DATADIR)/image6403.mpic $(EXEDIR)/$(PROGRAMNAME).c128ovl.d64  
	$(CC1541) -f image6404.mpic -w $(DATADIR)/image6404.mpic $(EXEDIR)/$(PROGRAMNAME).c128ovl.d64  
	$(CC1541) -f tiles.bin -w $(DATADIR)/tiles.bin $(EXEDIR)/$(PROGRAMNAME).c128ovl.d64  
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles.bin $(EXEDIR)/$(PROGRAMNAME).c128ovl.d64  
	$(CC1541) -f aaintro.mpic -w $(DATADIR)/aaintro64.mpic $(EXEDIR)/$(PROGRAMNAME).c128ovl.d64  
	$(CC1541) -f mctile.bin -w $(DATADIR)/tutorial_mctile.bin $(EXEDIR)/$(PROGRAMNAME).c128ovl.d64  

#$(CC1541) -f test.pic -w $(DATADIR)/test.pic.prg $(EXEDIR)/$(PROGRAMNAME).c64.d64  
#$(CC1541) -f test.col -w $(DATADIR)/test.col.prg $(EXEDIR)/$(PROGRAMNAME).c64.d64  
#$(CC1541) -f test.cpic -w $(DATADIR)/test.cpic.prg $(EXEDIR)/$(PROGRAMNAME).c64.d64  

# Let's define rules to compile the demo under ATARI as a one and single 
# executable file. This compilation is used as a "functional check", to
# be sure that the source implementation is correct.
obj/atari/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t atari -c $(CFLAGS) -Osir -Cl -o $@ $(subst obj/atari/,src/,$(@:.o=.c))

$(EXEDIR)/$(PROGRAMNAME).atari:	$(subst PLATFORM,atari,$(OBJS))
	$(CC) -Ln demoatari.lbl -t atari $(LDFLAGS) -m $(EXEDIR)/$(PROGRAMNAME).atari.map -C cfg/atari.cfg -o $(EXEDIR)/$(PROGRAMNAME).atari $(subst PLATFORM,atari,$(OBJS))
	$(call RMFILES,$(EXEDIR)/atr/*.*)
	$(call COPYFILES,$(DIR2ATR_HOME)/dos25/dos.sys,$(EXEDIR)/atr/dos.sys)
	$(call COPYFILES,$(EXEDIR)/$(PROGRAMNAME).atari,$(EXEDIR)/atr/$(PROGRAMNAME).exe)
	$(call COPYFILES,$(DATADIR)/slideshowa.dat,$(EXEDIR)/atr/slideshow)
	$(call COPYFILES,$(DATADIR)/imagea01.pic,$(EXEDIR)/atr/imagea01.pic)
	$(call COPYFILES,$(DATADIR)/imagea02.pic,$(EXEDIR)/atr/imagea02.pic)
	$(call COPYFILES,$(DATADIR)/imagea03.pic,$(EXEDIR)/atr/imagea03.pic)
	$(call COPYFILES,$(DATADIR)/imagea04.pic,$(EXEDIR)/atr/imagea04.pic)
	$(call COPYFILES,$(DATADIR)/ztiles.bin,$(EXEDIR)/atr/ztiles.bin)
	$(call COPYFILES,$(DATADIR)/tiles.bin,$(EXEDIR)/atr/tiles.bin)
	$(call COPYFILES,$(DATADIR)/tutorial_mctile.bin,$(EXEDIR)/atr/mctile.bin)
	$(DIR2ATR) -S -p -B $(DIR2ATR_HOME)/dos25/bootcode $(EXEDIR)/$(PROGRAMNAME).atari.atr $(EXEDIR)/atr
	$(ATRAUTORUN) -i $(EXEDIR)/$(PROGRAMNAME).atari.atr -o $(EXEDIR)/$(PROGRAMNAME).atari.atr -f $(PROGRAMNAME).exe

# Let's define rules to compile the demo under ATARI XL as a one and single 
# executable file. This compilation is used as a "functional check", to
# be sure that the source implementation is correct.
obj/ataricol/%.o:	$(SOURCES)
	$(CC) -T -l $(@:.o=.asm) -t atari -c $(CFLAGS) -Osir -Cl -D__ATARICOL__ -o $@ $(subst obj/ataricol/,src/,$(@:.o=.c))

$(EXEDIR)/$(PROGRAMNAME).ataricol:	$(subst PLATFORM,ataricol,$(OBJS))
	$(CC) -Ln demoataricol.lbl -t atari $(LDFLAGS) -m $(EXEDIR)/$(PROGRAMNAME).ataricol.map -D__ATARICOL__ -C cfg/atari.cfg -o $(EXEDIR)/$(PROGRAMNAME).ataricol $(subst PLATFORM,ataricol,$(OBJS))
	$(call RMFILES,$(EXEDIR)/atr/*.*)
	$(call COPYFILES,$(DIR2ATR_HOME)/dos25/dos.sys,$(EXEDIR)/atr/dos.sys)
	$(call COPYFILES,$(EXEDIR)/$(PROGRAMNAME).ataricol,$(EXEDIR)/atr/$(PROGRAMNAME).exe)
	$(call COPYFILES,$(DATADIR)/slideshowa.dat,$(EXEDIR)/atr/slideshow)
	$(call COPYFILES,$(DATADIR)/imagea01.pic,$(EXEDIR)/atr/imagea01.pic)
	$(call COPYFILES,$(DATADIR)/imagea02.pic,$(EXEDIR)/atr/imagea02.pic)
	$(call COPYFILES,$(DATADIR)/imagea03.pic,$(EXEDIR)/atr/imagea03.pic)
	$(call COPYFILES,$(DATADIR)/imagea04.pic,$(EXEDIR)/atr/imagea04.pic)
	$(call COPYFILES,$(DATADIR)/ztiles.bin,$(EXEDIR)/atr/ztiles.bin)
	$(call COPYFILES,$(DATADIR)/tiles.bin,$(EXEDIR)/atr/tiles.bin)
	$(call COPYFILES,$(DATADIR)/tutorial_mctile.bin,$(EXEDIR)/atr/mctile.bin)
	$(DIR2ATR) -S -p -B $(DIR2ATR_HOME)/dos25/bootcode $(EXEDIR)/$(PROGRAMNAME).ataricol.atr $(EXEDIR)/atr
	$(ATRAUTORUN) -i $(EXEDIR)/$(PROGRAMNAME).atari.atr -o $(EXEDIR)/$(PROGRAMNAME).atari.atr -f $(PROGRAMNAME).exe

###############################################################################
##
###############################################################################

obj/airattack.c64/%.o:	$(SOURCES)
	$(CC) -t c64 -c -D__GAME_AIR_ATTACK__ -Osir -Cl -D__CBM__ -o $@ $(subst obj/airattack.c64/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.c64:	$(subst PLATFORM,airattack.c64,$(OBJS))
	$(CC) -t c64 $(LDFLAGS) -o $(EXEDIR)/airattack.c64 $(subst PLATFORM,airattack.c64,$(OBJS))
	$(CC1541) -f airattack -w $(EXEDIR)/airattack.c64 $(EXEDIR)/airattack.c64.d64  
	$(CC1541) -f zztiles.bin -w $(DATADIR)/aatiles.bin $(EXEDIR)/airattack.c64.d64  
	$(CC1541) -f zzintro.pic -w $(DATADIR)/aaintro64.mpic $(EXEDIR)/airattack.c64.d64  

obj/airattack.vic2024/%.o:	$(SOURCES)
	$(CC) -t vic20 -c -D__GAME_AIR_ATTACK__ -D__24K__ -C cfg/vic20-32k.cfg -Osir -Cl -D__CBM__ -o $@ $(subst obj/airattack.vic2024/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.vic2024:	$(subst PLATFORM,airattack.vic2024,$(OBJS))
	$(CC) -t vic20 $(LDFLAGS) -m $(EXEDIR)/airattack.vic2024.map -C cfg/vic20-32k.cfg -o $(EXEDIR)/airattack.vic2024 $(subst PLATFORM,airattack.vic2024,$(OBJS))
	$(CC1541) -f loader -w $(DATADIR)/airattack_loader2024.prg $(EXEDIR)/airattack.vic2024.d64  
	$(CC1541) -f airattack -w $(EXEDIR)/airattack.vic2024 $(EXEDIR)/airattack.vic2024.d64  
	$(CC1541) -f zztiles.bin -w $(DATADIR)/aatiles20.bin $(EXEDIR)/airattack.vic2024.d64  
	$(CC1541) -f zzintro.pic -w $(DATADIR)/aaintro20.mpic $(EXEDIR)/airattack.vic2024.d64  

obj/airattack.plus4/%.o:	$(SOURCES)
	$(CC) -t plus4 -c -D__GAME_AIR_ATTACK__ -Osir -Cl -T -l $(@:.o=.map) -D__CBM__ -C cfg/plus4.cfg -o $@ $(subst obj/airattack.plus4/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.plus4:	$(subst PLATFORM,airattack.plus4,$(OBJS))
	$(CC) -t plus4 $(LDFLAGS) -m $(EXEDIR)/airattack.plus4.map -C cfg/plus4.cfg -o $(EXEDIR)/airattack.plus4 $(subst PLATFORM,airattack.plus4,$(OBJS))
	$(CC1541) -f loader -w $(DATADIR)/airattack_loader4.prg $(EXEDIR)/airattack.plus4.d64  
	$(CC1541) -f airattack -w $(EXEDIR)/airattack.plus4 $(EXEDIR)/airattack.plus4.d64  
	$(CC1541) -f zztiles.bin -w $(DATADIR)/aatiles4.bin $(EXEDIR)/airattack.plus4.d64  
	$(CC1541) -f zzintro.pic -w $(DATADIR)/aaintro16.mpic $(EXEDIR)/airattack.plus4.d64  

obj/airattack.c128/%.o:	$(SOURCES)
	$(CC) -t c128 -c -D__GAME_AIR_ATTACK__ -Osir -Cl -D__CBM__ -o $@ $(subst obj/airattack.c128/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.c128:	$(subst PLATFORM,airattack.c128,$(OBJS))
	$(CC) -t c128 $(LDFLAGS) -o $(EXEDIR)/airattack.c128 $(subst PLATFORM,airattack.c128,$(OBJS))
	$(CC1541) -f airattack -w $(EXEDIR)/airattack.c128 $(EXEDIR)/airattack.c128.d64  
	$(CC1541) -f zztiles.bin -w $(DATADIR)/aatiles.bin $(EXEDIR)/airattack.c128.d64  
	$(CC1541) -f zzintro.pic -w $(DATADIR)/aaintro64.mpic $(EXEDIR)/airattack.c128.d64  

obj/airattack.atari/%.o:	$(SOURCES)
	$(CC) -t atari -c -D__GAME_AIR_ATTACK__ -Osir -Cl -o $@ $(subst obj/airattack.atari/,src/,$(@:.o=.c))

$(EXEDIR)/airattack.atari:	$(subst PLATFORM,airattack.atari,$(OBJS))
	$(CC) -t atari $(LDFLAGS) -o $(EXEDIR)/airattack.atari $(subst PLATFORM,airattack.atari,$(OBJS))
	$(call RMFILES,$(EXEDIR)/atr/*.*)
	$(call COPYFILES,$(DIR2ATR_HOME)/dos25/dos.sys,$(EXEDIR)/atr/dos.sys)
	$(call COPYFILES,$(EXEDIR)/airattack.atari,$(EXEDIR)/atr/game.exe)
	$(call COPYFILES,$(DATADIR)/ztiles.bin,$(EXEDIR)/atr/ztiles.bin)
	$(call COPYFILES,$(DATADIR)/aatiles4.bin,$(EXEDIR)/atr/zztiles.bin)
	$(call COPYFILES,$(DATADIR)/aaintroa.pic,$(EXEDIR)/atr/zzintro.pic)
	$(DIR2ATR) -S -p -B $(DIR2ATR_HOME)/dos25/bootcode $(EXEDIR)/airattack.atari.atr $(EXEDIR)/atr
	$(ATRAUTORUN) -i $(EXEDIR)/$(PROGRAMNAME).atari.atr -o $(EXEDIR)/$(PROGRAMNAME).atari.atr -f game.exe

###############################################################################
##
###############################################################################

obj/totto.c64/%.o:	$(SOURCES)
	$(CC) -t c64 -c -D__GAME_TOTTO__ -Osir -Cl -D__CBM__ -o $@ $(subst obj/totto.c64/,src/,$(@:.o=.c))

$(EXEDIR)/totto.c64:	$(subst PLATFORM,totto.c64,$(OBJS))
	$(CC) -t c64 $(LDFLAGS) -o $(EXEDIR)/totto.c64 $(subst PLATFORM,totto.c64,$(OBJS))
	$(CC1541) -f totto -w $(EXEDIR)/totto.c64 $(EXEDIR)/totto.c64.d64  
	$(CC1541) -f tttiles.bin -w $(DATADIR)/tttiles.bin $(EXEDIR)/totto.c64.d64  
	$(CC1541) -f tttiles1.bin -w $(DATADIR)/tttiles1.bin $(EXEDIR)/totto.c64.d64  
	$(CC1541) -f ttfinal1.mpic -w $(DATADIR)/ttfinal164.mpic $(EXEDIR)/totto.c64.d64  
	$(CC1541) -f ttfinal2.mpic -w $(DATADIR)/ttfinal264.mpic $(EXEDIR)/totto.c64.d64  
	$(CC1541) -f ttfinal3.mpic -w $(DATADIR)/ttfinal364.mpic $(EXEDIR)/totto.c64.d64  
	$(CC1541) -f ttfinal4.mpic -w $(DATADIR)/ttfinal464.mpic $(EXEDIR)/totto.c64.d64  

obj/totto.vic2024/%.o:	$(SOURCES)
	$(CC) -t vic20 -c -D__GAME_TOTTO__ -D__24K__ -C cfg/vic20-32k.cfg -Osir -Cl -D__CBM__ -o $@ $(subst obj/totto.vic2024/,src/,$(@:.o=.c))

$(EXEDIR)/totto.vic2024:	$(subst PLATFORM,totto.vic2024,$(OBJS))
	$(CC) -t vic20 $(LDFLAGS) -m $(EXEDIR)/totto.vic2024.map -C cfg/vic20-32k.cfg -o $(EXEDIR)/totto.vic2024 $(subst PLATFORM,totto.vic2024,$(OBJS))
	$(CC1541) -f loader -w $(DATADIR)/totto_loader2024.prg $(EXEDIR)/totto.vic2024.d64  
	$(CC1541) -f totto -w $(EXEDIR)/totto.vic2024 $(EXEDIR)/totto.vic2024.d64  
	$(CC1541) -f tttiles.bin -w $(DATADIR)/tttiles.bin $(EXEDIR)/totto.vic2024.d64  
	$(CC1541) -f tttiles1.bin -w $(DATADIR)/tttiles1.bin $(EXEDIR)/totto.vic2024.d64  
	$(CC1541) -f ttfinal1.mpic -w $(DATADIR)/ttfinal120.mpic $(EXEDIR)/totto.vic2024.d64  
	$(CC1541) -f ttfinal2.mpic -w $(DATADIR)/ttfinal220.mpic $(EXEDIR)/totto.vic2024.d64  
	$(CC1541) -f ttfinal3.mpic -w $(DATADIR)/ttfinal320.mpic $(EXEDIR)/totto.vic2024.d64  
	$(CC1541) -f ttfinal4.mpic -w $(DATADIR)/ttfinal420.mpic $(EXEDIR)/totto.vic2024.d64  

obj/totto.plus4/%.o:	$(SOURCES)
	$(CC) -t plus4 -c -D__GAME_TOTTO__ -Osir -Cl -T -l $(@:.o=.map) -D__CBM__ -C cfg/plus4.cfg -o $@ $(subst obj/totto.plus4/,src/,$(@:.o=.c))

$(EXEDIR)/totto.plus4:	$(subst PLATFORM,totto.plus4,$(OBJS))
	$(CC) -t plus4 $(LDFLAGS) -m $(EXEDIR)/totto.plus4.map -C cfg/plus4.cfg -o $(EXEDIR)/totto.plus4 $(subst PLATFORM,totto.plus4,$(OBJS))
	$(CC1541) -f loader -w $(DATADIR)/totto_loader4.prg $(EXEDIR)/totto.plus4.d64  
	$(CC1541) -f totto -w $(EXEDIR)/totto.plus4 $(EXEDIR)/totto.plus4.d64  
	$(CC1541) -f tttiles.bin -w $(DATADIR)/tttiles.bin $(EXEDIR)/totto.plus4.d64  
	$(CC1541) -f tttiles1.bin -w $(DATADIR)/tttiles1.bin $(EXEDIR)/totto.plus4.d64  
	$(CC1541) -f ttfinal1.mpic -w $(DATADIR)/ttfinal116.mpic $(EXEDIR)/totto.plus4.d64  
	$(CC1541) -f ttfinal2.mpic -w $(DATADIR)/ttfinal216.mpic $(EXEDIR)/totto.plus4.d64  
	$(CC1541) -f ttfinal3.mpic -w $(DATADIR)/ttfinal316.mpic $(EXEDIR)/totto.plus4.d64  
	$(CC1541) -f ttfinal4.mpic -w $(DATADIR)/ttfinal416.mpic $(EXEDIR)/totto.plus4.d64  

obj/totto.c128/%.o:	$(SOURCES)
	$(CC) -t c128 -c -D__GAME_TOTTO__ -Osir -Cl -D__CBM__ -o $@ $(subst obj/totto.c128/,src/,$(@:.o=.c))

$(EXEDIR)/totto.c128:	$(subst PLATFORM,totto.c128,$(OBJS))
	$(CC) -t c128 $(LDFLAGS) -o $(EXEDIR)/totto.c128 $(subst PLATFORM,totto.c128,$(OBJS))
	$(CC1541) -f totto -w $(EXEDIR)/totto.c128 $(EXEDIR)/totto.c128.d64  
	$(CC1541) -f tttiles.bin -w $(DATADIR)/tttiles.bin $(EXEDIR)/totto.c128.d64  
	$(CC1541) -f tttiles1.bin -w $(DATADIR)/tttiles1.bin $(EXEDIR)/totto.c128.d64  
	$(CC1541) -f ttfinal1.mpic -w $(DATADIR)/ttfinal164.mpic $(EXEDIR)/totto.c128.d64  
	$(CC1541) -f ttfinal2.mpic -w $(DATADIR)/ttfinal264.mpic $(EXEDIR)/totto.c128.d64  
	$(CC1541) -f ttfinal3.mpic -w $(DATADIR)/ttfinal364.mpic $(EXEDIR)/totto.c128.d64  
	$(CC1541) -f ttfinal4.mpic -w $(DATADIR)/ttfinal464.mpic $(EXEDIR)/totto.c128.d64  

###############################################################################
## FINAL RULES
###############################################################################

$(EXEDIR):
	$(call MKDIR,$@)

$(EXEDIR)/atr:
	$(call MKDIR,$@)

$(TARGETOBJDIR):
	$(call MKDIR,$@)

$(DATADIR):
	$(call MKDIR,$@)

all: $(EXEDIR) $(EXEDIR)/atr $(TARGETOBJDIR) $(EXES)

clean:
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).c64.d64)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).vic20.d64)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).vic2024.d64)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).c16.d64)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).plus4.d64)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).atari.atr)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).ataricol.atr)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).c64)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).c64ovl*)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).vic20)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).vic2024)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).vic20ovl*)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).c16)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).plus4)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).atari)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).ataricol)
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
	$(foreach EXE,$(EXES),$(call RMFILES,$(EXE)))
	$(foreach OBJECT,$(OBJECTS),$(call RMFILES,$(OBJECT)))
