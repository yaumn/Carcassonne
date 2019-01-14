SRC = $(wildcard *.c)
LIBS = $(SRC:.c=.so)
COMMON_SRC = $(wildcard ../common/*.c)
COMMON_SRC := $(filter-out ../common/move1.c ../common/move2.c, $(COMMON_SRC)) # Exclude move1.c for the moment as it provoques a segfault

.PHONY: all
all: $(LIBS)


%.so: %.c
	${CC} ${CFLAGS} -o $@ -I ../common -fPIC -shared $< $(COMMON_SRC)


.PHONY: clean
clean:
	rm -f *.so *.o *~
