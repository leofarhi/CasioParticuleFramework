#---------------------------------------------------------------------------------
#.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
# DATA is a list of directories containing binary data
# GRAPHICS is a list of directories containing files to be processed by grit
#
# All directories are specified relative to the project directory where
# the makefile is found
#
#---------------------------------------------------------------------------------
#TARGET		:=	"hello"#$(notdir $(CURDIR))
#BUILD		:=	build
ListDir		+=  Libs/NDS

SOURCES		:=	src $(ListDir)#src src/SubFolder
ASSETS 		:=  build/assets
DATA		:=	$(ASSETS)/BinaryData
INCLUDES	:=	$(ListDir)
GRAPHICS	:=	$(ASSETS)/GraphicsData
NITRODATA	:=	$(ASSETS)/NitroData

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH		:=	-mthumb -mthumb-interwork

CFLAGS	:=	-march=armv5te -mtune=arm946e-s \
			$(ARCH)

CFLAGS	+=	$(INCLUDE) -DARM9 -DNDS_MODE
CXXFLAGS	:=	$(CFLAGS) -fno-rtti -fno-exceptions

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-specs=ds_arm9.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:= -lfilesystem -lNE -lfat -lnds9 -lm
 
 
#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(LIBNDS) $(PORTLIBS) $(DEVKITPRO)/nitro-engine

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------


ifneq ($(BUILDDIR), $(CURDIR))
#---------------------------------------------------------------------------------
export TOPDIR	:=	$(CURDIR)

export OUTPUT	:=	$(CURDIR)/$(TARGET)
 
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
					$(foreach dir,$(GRAPHICS),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

ifneq ($(strip $(NITRODATA)),)
	export NITRO_FILES	:=	$(CURDIR)/$(NITRODATA)
endif

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))
BMPFILES	:=	$(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.bmp)))
PNGFILES	:=	$(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.png)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES_BIN   :=	$(addsuffix .o,$(BINFILES))

export OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export OFILES := $(PNGFILES:.png=.o) $(BMPFILES:.bmp=.o) $(OFILES_BIN) $(OFILES_SOURCES)

export HFILES := $(PNGFILES:.png=.h) $(BMPFILES:.bmp=.h) $(addsuffix .h,$(subst .,_,$(BINFILES)))
 
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-iquote $(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)
 
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
					$(foreach dir,$(INCLUDES),-L$(CURDIR)/$(dir))

#LIBPATHS	+=	-L $(CURDIR)/Libs/List
#INCLUDE		+=	-I $(CURDIR)/Libs/List

#LIBPATHS	+=	-L $(CURDIR)/Libs/NDS
#INCLUDE		+=	-I $(CURDIR)/Libs/NDS

#add Listdir to include
INCLUDE		+=	$(foreach dir,$(ListDir),-I$(CURDIR)/$(dir))
LIBPATHS	+=	$(foreach dir,$(ListDir),-L$(CURDIR)/$(dir))
#add .h to .o in OFILES
OFILES 		+=	$(foreach dir,$(ListDir),$(notdir $(wildcard $(dir)/*.h:.h=.o)))

.PHONY: $(BUILD) clean
 
#make bin folder and move .elf and .nds to it
nds_mode: $(BUILD)
#	@mkdir -p $(OUTPUT)

#---------------------------------------------------------------------------------
$(BUILD):
	@echo $(CURDIR)
	@[ -d $@ ] || mkdir -p $@
	@make BUILDDIR=`cd $(BUILD) && pwd` --no-print-directory -C $(BUILD) -f $(CURDIR)/MakefileDS

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).nds


#---------------------------------------------------------------------------------
else
 
DEPENDS	:=	$(OFILES:.o=.d)
 
#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------

$(OUTPUT).nds	: 	$(OUTPUT).elf
$(OUTPUT).nds	: 	$(shell find $(TOPDIR)/$(NITRODATA))
$(OUTPUT).elf	:	$(OFILES)


#---------------------------------------------------------------------------------
# The bin2o rule should be copied and modified
# for each extension used in the data directories
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
%.bin.o %_bin.h : %.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
# This rule creates assembly source files using grit
# grit takes an image file and a .grit describing how the file is to be processed
# add additional rules like this for each image extension
# you use in the graphics folders 
#---------------------------------------------------------------------------------
%.s %.h	: %.bmp %.grit
#---------------------------------------------------------------------------------
	grit $< -fts -o$*

#---------------------------------------------------------------------------------
%.s %.h	: %.png %.grit
#---------------------------------------------------------------------------------
	grit $< -fts -o$*


-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
