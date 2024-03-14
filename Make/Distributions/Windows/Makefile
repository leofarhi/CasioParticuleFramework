#WINDOWS COMPILATION
CFLAGS += -I $(LIBS_DIR)/DistributionCases/SDL2_WIN_Include/include -DWIN32
LDLIBS += -L $(LIBS_DIR)/DistributionCases/SDL2_WIN_Include/lib -lmingw32 -lSDL2 -lgdi32 -luser32 -lole32 -loleaut32 -lmingw32 -limm32 -lwinmm -lversion -lSetupAPI -lSDL2 -lSDL2_image -lSDL2_ttf -lfreetype

all: ${OBJ}
	$(CC) $(CFLAGS) -o $(TARGET).exe $(OBJ) $(LDLIBS)
	cp $(LIBS_DIR)/DistributionCases/BIN_SDL/* $(OUTPUT)


$(foreach f, $(TARGET), $(eval $(f):))

-include ${DEP}