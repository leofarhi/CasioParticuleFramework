#PSP COMPILATION
OBJS = $(OBJ) Libs/PSP/common/callbacks.o Libs/PSP/common/common-sce.o Libs/PSP/MyKeyboard/MyKeyboard.o

INCDIR = $(ListDir)
LIBS= $(LDLIBS) -lpspgum -lpspgu -lm -lpspaudio -lpspmp3 -lpspaudiocodec -lpspaudiolib -lpspsdk -lpsprtc -lpsputility
#LIBS+= -lpng -lz -lpspctrl -lpspdebug -lpspdisplay -lpspge -lpspkernel -lpspnet -lpspnet_adhoc -lpspnet_adhocctl -lpspnet_adhocmatching -lpspuser -lpsputility#-lpspnet_adhocssl -lpspnet_inet -lpspnet_resolver -lpspsemaphore

CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = $(TITLE)
#PSP_EBOOT_ICON = ICON0.PNG

#add -DPSP_MODE
CFLAGS += -DPSP_MODE

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
#move EBOOT.PBP to bin folder
psp_mode: all
	mv $(EXTRA_TARGETS) $(OUTPUT)/$(EXTRA_TARGETS)
	mv PARAM.SFO $(OUTPUT)/PARAM.SFO