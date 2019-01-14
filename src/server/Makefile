SRC = $(wildcard *.c)
SRC += $(wildcard ../common/*.c)
SRC := $(filter-out gui.c ../common/move1.c ../common/move2.c, $(SRC)) # Exclude move1.c for the moment as it provoques a segfault
OBJ = $(SRC:.c=.o)

LIBS = -ldl
INCLUDES = -I ../common
ifeq ($(ENABLE_GUI), 1)
LIBS += -lSDL -lSDL_image -lSDL_gfx
INCLUDES += -I /usr/include/SDL
CFLAGS += -DENABLE_GUI
SRC += gui.c
endif


server: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LIBS)


%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<


.PHONY: clean
clean:
	rm -f server *.o *~
