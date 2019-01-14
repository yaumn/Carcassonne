TEST_SRC = $(wildcard *.c)
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_BIN = $(basename $(TEST_SRC))
#TEST_BIN := $(filter-out test_city, $(TEST_BIN))
TEST_COV = $(subst test_, cov_, $(TEST_BIN))
SRC_DIR = ../src
DEPENDENCIES = $(SRC_DIR)/common/list.c $(SRC_DIR)/common/reference.c $(SRC_DIR)/common/tile.c $(SRC_DIR)/common/board.c $(SRC_DIR)/common/road.c $(SRC_DIR)/common/city.c $(SRC_DIR)/common/farm.c $(SRC_DIR)/common/cloister.c $(SRC_DIR)/common/move.c
CFLAGS += -std=c99
COVERAGE = -Wall -fprofile-arcs -ftest-coverage

.PHONY:
all: $(TEST_BIN)

.PHONY:
clean:
	rm -f *.o *.gcno *.gcov *.gcda cov_* $(TEST_BIN)

.PHONY: $(TEST_BIN)
$(TEST_BIN):
	@$(CC) $(CFLAGS) -Wall -I $(SRC_DIR)/common $@.c $(DEPENDENCIES)  -o $@ &&\
	./$@

.PHONY: coverage
coverage: $(TEST_COV)

.PHONY: $(TEST_COV)
$(TEST_COV):
	@$(CC) $(CFLAGS) -Wall -fprofile-arcs -ftest-coverage -I $(SRC_DIR)/common $(subst cov_,test_,$@.c) $(DEPENDENCIES)  -o $@ &&\
	./$@ &&\
	gcov $(subst cov_,test_,$@.c) &&\
	gcov $(subst cov_,,$@.gc)

#.PHONY: test_road
#test_road: test_road.c
#	$(CC) $(CFLAGS) -I $(SRC_DIR) -I $(SRC_DIR)/common test_road.c $(SRC_DIR)/common/road.c $(SRC_DIR)/common/list.c $(SRC_DIR)/common/tile.c -o test_road

#.PHONY: test_cloister
#test_cloister: test_cloister.c
#	$(CC) $(CFLAGS) -I $(SRC_DIR)/common test_cloister.c $(SRC_DIR)/common/cloister.c $(SRC_DIR)/common/list.c $(SRC_DIR)/common/tile.c $(SRC_DIR)/common/board.c $(SRC_DIR)/common/road.c $(SRC_DIR)/common/city.c $(SRC_DIR)/common/farm.c $(SRC_DIR)/common/reference.c -o test_cloister

#.PHONY: test_tile
#test_tile: test_tile.c
#	$(CC) $(CFLAGS) -I $(SRC_DIR)/common test_tile.c $(SRC_DIR)/common/list.c $(SRC_DIR)/common/tile.c -o test_tile

#.PHONY: test_farm
#test_farm: test_farm.c
#	$(CC) $(CFLAGS) -I $(SRC_DIR)/common test_farm.c $(DEPENDENCIES) -o test_farm

#.PHONY: test_board
#test_board: test_board.c
#	$(CC) $(CFLAGS) -I $(SRC_DIR)/common test_board.c $(DEPENDENCIES) -o test_board

#.PHONY: test_ref
#test_ref: test_reference.c
#	$(CC) $(CFLAGS) -I $(SRC_DIR)/common test_reference.c $(SRC_DIR)/common/list.c $(SRC_DIR)/common/reference.c -o test_ref

#.PHONY: test_city
#test_city: test_city.c
#	$(CC) $(CFLAGS) -g3 -std=c99 -I $(SRC_DIR) -I $(SRC_DIR)/common test_city.c $(SRC_DIR)/common/list.c $(SRC_DIR)/common/board.c $(SRC_DIR)/common/reference.c $(SRC_DIR)/common/city.c $(SRC_DIR)/common/road.c $(SRC_DIR)/common/cloister.c $(SRC_DIR)/common/farm.c $(SRC_DIR)/common/tile.c -o test_city

#.PHONY: test_move
#test_move: test_move.c
#	$(CC) $(CFLAGS) -I $(SRC_DIR)/common test_move.c $(SRC_DIR)/common/list.c $(SRC_DIR)/common/board.c $(SRC_DIR)/common/reference.c $(SRC_DIR)/common/move.c $(SRC_DIR)/common/tile.c $(SRC_DIR)/common/city.c $(SRC_DIR)/common/road.c $(SRC_DIR)/common/cloister.c $(SRC_DIR)/common/farm.c -o test_move
