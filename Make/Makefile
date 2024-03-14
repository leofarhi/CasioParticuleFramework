# Makefile
#/*<COMPILATOR_FIELD>*/
CC = gcc
#/*</COMPILATOR_FIELD>*/

CPPFLAGS = -MMD
CFLAGS = #-Wall -Wextra #-fsanitize=address
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
SourceDirs += ../Sources ../Sources/include ../Sources/src ../Libs/Essential/Dict ../Libs/Essential/List ../Libs/Essential/Tuple
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
include Distributions/Windows/Makefile
#/*</INCLUDE_MAKEFILE>*/

#/*<CFLAGS_AND_LDLIBS_FIELD>*/
CFLAGS += -I ../Libs/Essential/Dict -I ../Libs/Essential/List -I ../Libs/Essential/Tuple 

CFLAGS += -DDIST_WINDOWS

LDLIBS += -L ../Libs/Essential/Dict -L ../Libs/Essential/List -L ../Libs/Essential/Tuple 
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