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
#  - c64: single executable for Commodore 64 (named on disk: "midres-single")
#  - c64ovl: overlayed executable for Commodore 64 (named on disk: "midres")
#  - vic20: single executable for unexpanded VIC 20 (named on disk: "midres-single")
#  - vic20ovl: overlayed executable for Commodore VIC20 (named on disk: "midres")
#  - vic2024: single executable for 24K VIC 20 (named on disk: "midres-single")
#  - c16: single executable for Commodore 16 (named on disk: "midres-single")
#  - plus4: single executable for Plus 4 (named on disk: "midres-single")
TARGETS := plus4

# Given demonstrations:
#  - SLIDESHOW - a slideshow with some images converted using img2midres
#  - DRAWING - an animation using drawing primitives (v1.1)
#  - BITBLIT - an animation using bit blits primivites (v1.2)
#  - TILE - an animation using tiles primivites (v1.3)
DEMO := 

# Given games:
#  - AIR_ATTACK
GAME := AIR_ATTACK

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
  RMDIR = rmdir $(subst /,\,$1)
  RMFILES = $(if exist $(subst /,\,$1), del /f $(subst /,\,$1))
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

###############################################################################
## COMPILATION / LINKING OPTIONS
###############################################################################

CFLAGS := -D__DEMO_$(DEMO)__ -D__GAME_$(GAME)__
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

# We generate the list of paths where the object files for each target will 
# end, so that we can generate them in advance (as paths).
TARGETOBJDIR := $(foreach TARGET,$(TARGETS),obj/$(TARGET))

# This is the path where all executables will be put.
EXEDIR := exe

# Similarly, we expand the set of executables that are required.
EXES := $(foreach TARGET,$(TARGETS),$(EXEDIR)/$(PROGRAMNAME).$(TARGET))

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

#$(CC1541) -f test.pic -w $(DATADIR)/test.pic.prg $(EXEDIR)/$(PROGRAMNAME).c64.d64  
#$(CC1541) -f test.col -w $(DATADIR)/test.col.prg $(EXEDIR)/$(PROGRAMNAME).c64.d64  
#$(CC1541) -f test.cpic -w $(DATADIR)/test.cpic.prg $(EXEDIR)/$(PROGRAMNAME).c64.d64  

## VIC20 ------------------------------------------------------------------------

# Let's define rules to compile the demo under VIC 20 as a one and single 
# executable file. This compilation will fails since there is no enough RAM.
obj/vic20/%.o:	$(SOURCES)
	$(CC) -t vic20 -c $(CFLAGS) -Osir -Cl -T -l $(@:.o=.map) -D__CBM__ -C cfg/vic20.cfg -o $@ $(subst obj/vic20/,src/,$(@:.o=.c))

$(EXEDIR)/$(PROGRAMNAME).vic20:	$(subst PLATFORM,vic20,$(OBJS))
	$(CC) -Ln demo20.lbl -t vic20 $(LDFLAGS) -m $(EXEDIR)/$(PROGRAMNAME).vic20.map -C cfg/vic20.cfg -o $(EXEDIR)/$(PROGRAMNAME).vic20 $(subst PLATFORM,vic20,$(OBJS))
	$(CC1541) -f $(PROGRAMNAME)-single -w $(EXEDIR)/$(PROGRAMNAME).vic20 $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f slideshow -w $(DATADIR)/slideshow20.dat $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f image2001.mpic -w $(DATADIR)/image2001.mpic $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f image2002.mpic -w $(DATADIR)/image2002.mpic $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f image2003.mpic -w $(DATADIR)/image2003.mpic $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f image2004.mpic -w $(DATADIR)/image2004.mpic $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f tiles.bin -w $(DATADIR)/tiles.bin $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles.bin $(EXEDIR)/$(PROGRAMNAME).vic20.d64  

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
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles.bin $(EXEDIR)/$(PROGRAMNAME).vic2024.d64  

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
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles.bin $(EXEDIR)/$(PROGRAMNAME).vic20ovl.d64  

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
	$(CC1541) -f aatiles.bin -w $(DATADIR)/aatiles.bin $(EXEDIR)/$(PROGRAMNAME).plus4.d64  

###############################################################################
## FINAL RULES
###############################################################################

$(EXEDIR):
	$(call MKDIR,$@)

$(TARGETOBJDIR):
	$(call MKDIR,$@)

$(DATADIR):
	$(call MKDIR,$@)

all: $(EXEDIR) $(TARGETOBJDIR) $(EXES)

clean:
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).c64.d64)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).vic20.d64)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).vic2024.d64)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).c16.d64)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).plus4.d64)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).c64)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).c64ovl*)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).vic20)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).vic2024)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).vic20ovl*)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).c16)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).plus4)
	$(foreach EXE,$(EXES),$(call RMFILES,$(EXE)))
	$(foreach OBJECT,$(OBJECTS),$(call RMFILES,$(OBJECT)))
