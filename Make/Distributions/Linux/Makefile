#Linux COMPILATION
CFLAGS += `pkg-config --cflags sdl2 SDL2_image SDL2_ttf`
LDLIBS += `pkg-config --libs sdl2 SDL2_image SDL2_ttf` -lm

all: ${OBJ}
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDLIBS)


$(foreach f, $(TARGET), $(eval $(f):))

-include ${DEP}