# Makefile
#/*<COMPILATOR_FIELD>*/
CC = gcc
#/*</COMPILATOR_FIELD>*/

CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -Werror# -fsanitize=address -g3 -std=c99 -O0
LDFLAGS = -fsanitize=address
LDLIBS = -lm

#/*<OUTPUT_FIELD>*/
OUTPUT = ../Build/bin
#/*</OUTPUT_FIELD>*/

#/*<BUILD_FIELD>*/
BUILD = ../Build/build
#/*</BUILD_FIELD>*/

#/*<TITLE_FIELD>*/
TITLE = Engine
#/*</TITLE_FIELD>*/

TARGET = $(OUTPUT)/$(TITLE)

#/*<LIBS_DIR_FIELD>*/
LIBS_DIR = ../Libs
#/*</LIBS_DIR_FIELD>*/

#/*<SOURCE_DIR_FIELD>*/
SourceDirs += ../Sources ../Sources/include ../Sources/src ../Libs/DistributionCases/PSP ../Libs/DistributionCases/PSP/common ../Libs/DistributionCases/PSP/MyKeyboard ../Libs/Essential/Dict ../Libs/Essential/List ../Libs/Essential/Tuple ../Libs/ParticuleEngine/Psp/essential ../Libs/ParticuleEngine/All/include/essential ../Libs/ParticuleEngine/All/src/essential
#/*</SOURCE_DIR_FIELD>*/

#print all folder in SourceDirs
#$(info $(SourceDirs))


SRC += $(foreach folder,$(SourceDirs),$(wildcard $(folder)/*.c))
#remove duplicate
SRC := $(sort $(SRC))
OBJ += ${SRC:.c=.o}
DEP += ${SRC:.c=.d}
PRG += ${SRC:.c=}


#/*<INCLUDE_MAKEFILE>*/
include Distributions/PSP/Makefile
#/*</INCLUDE_MAKEFILE>*/

#/*<CFLAGS_AND_LDLIBS_FIELD>*/
CFLAGS += -I ../Libs/DistributionCases/PSP -I ../Libs/Essential/Dict -I ../Libs/Essential/List -I ../Libs/Essential/Tuple -I ../Libs/ParticuleEngine/Psp/essential -I ../Libs/ParticuleEngine/All/include/essential -I ../Libs/ParticuleEngine/All/src/essential 

CFLAGS += -DDIST_PSP -DPROJECT_NAME=\"Engine\"

LDLIBS += -L ../Libs/DistributionCases/PSP -L ../Libs/Essential/Dict -L ../Libs/Essential/List -L ../Libs/Essential/Tuple -L ../Libs/ParticuleEngine/Psp/essential -L ../Libs/ParticuleEngine/All/include/essential -L ../Libs/ParticuleEngine/All/src/essential 
#/*</CFLAGS_AND_LDLIBS_FIELD>*/

#/*<DEBUG_FIELD>*/
CFLAGS += -DDEBUG_MODE
#/*</DEBUG_FIELD>*/

ifeq (clean,$(MAKECMDGOALS))

.PHONY: clean

clean:
	rm -rf $(BUILD)/*
	$(RM) $(OBJ)
	$(RM) $(EXE)
	rm -f ${DEP}
	rm -f ${PRG}
	$(RM) -f $(EXE)
	$(RM) -rf $(OUTPUT)/*

endif