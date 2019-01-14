#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "reference.h"
#include "board.h"
#include "farm.h"
#include "road.h"
#include "city.h"
#include "interface.h"
#include "move.h"
#include "tile.h"


/*
struct move *possible_move(struct board* b, enum card_id c, unsigned int player_id)
int move__is_valid(struct board* b, struct move* m)
int meeple_in(int tab[])
int id__not_in(struct list *ref_list, int* ids[], int n, int id_value)
void move__get_all_possible_moves(struct board *board, struct list *possible_moves, const enum card_id c, const int player_id)
*/

int meeple__first(void * zone, enum biome type);
void get_tiles_to_tests(struct move *, int *, int *);
int move__is_valid_aux(struct board *, struct move *);


void test__move_tiles_to_tests()
{
  int error = 0;
  printf("\t%s", __func__);
  //initialize the move_test
  struct move move_test;
  move_test.check = FAILED;
  move_test.player = 1;
  move_test.onto.x = 42;
  move_test.onto.y = 42;
  //initialize the board and required variables
  struct board b;
  board__init(&b);
  struct tile *tiles[4] = { board__get_tile(&b, move_test.onto.x-1, move_test.onto.y), board__get_tile(&b, move_test.onto.x, move_test.onto.y-1),
                            board__get_tile(&b, move_test.onto.x+1, move_test.onto.y), board__get_tile(&b, move_test.onto.x, move_test.onto.y+1)};
  enum place posi[4] = {POS_SOUTH, POS_EAST, POS_NORTH, POS_WEST};
  int nb_tests = 0;
  int tests[4] = {0, 0, 0, 0};
  //test 1 : CARD_MONASTERY_ROAD WEST
  move_test.card = CARD_MONASTERY_ROAD;
  move_test.dir = WEST;
  move_test.place = POS_SOUTH;
  get_tiles_to_tests(&move_test, tests, &nb_tests);
  if(nb_tests != 1){
    error++;
    printf("ERROR : should have only one card to test\n");
  }
  if(tiles[tests[0]] != board__get_tile(&b, 42, 43)){
    error++;
    printf("ERROR : the tile to test is not the good one (42,43)\n");
  }
  if(posi[tests[0]] != POS_WEST){
    error++;
    printf("ERROR : the pos is not the good one (POS_WEST) %d\n", tests[0]);
  }
  //test 2 : CARD_ROAD_TURN_RIGHT_CITY EAST
  move_test.card = CARD_ROAD_TURN_RIGHT_CITY;
  move_test.dir = EAST;
  move_test.place = POS_SOUTH;
  get_tiles_to_tests(&move_test, tests, &nb_tests);
  if(nb_tests != 2){
    error++;
    printf("ERROR : should have only one card to test\n");
  }
  if(tiles[tests[1]] != board__get_tile(&b, 43, 42)){
    error++;
    printf("ERROR : the tile to test is not the good one (43,42)\n");
  }
  if(tiles[tests[0]] != board__get_tile(&b, 42, 41)){
    error++;
    printf("ERROR : the tile to test is not the good one (42,41)\n");
  }
  if(posi[tests[0]] != POS_EAST){
    error++;
    printf("ERROR : the pos is not the good one (POS_EAST) %d\n", tests[1]);
  }
  if(posi[tests[1]] != POS_NORTH){
    error++;
    printf("ERROR : the pos is not the good one (POS_NORTH) %d\n", tests[0]);
  }
  //test 3 : CARD_ROAD_STRAIGHT SOUTH
  move_test.card = CARD_ROAD_STRAIGHT;
  move_test.dir = SOUTH;
  move_test.place = POS_SOUTH;
  get_tiles_to_tests(&move_test, tests, &nb_tests);
  if(nb_tests != 2){
    error++;
    printf("ERROR : should have only one card to test\n");
  }
  if(tiles[tests[0]] != board__get_tile(&b, 43, 42)){
    error++;
    printf("ERROR : the tile to test is not the good one (43,42)\n");
  }
  if(tiles[tests[1]] != board__get_tile(&b, 41, 42)){
    error++;
    printf("ERROR : the tile to test is not the good one (41,42)\n");
  }
  if(posi[tests[0]] != POS_NORTH){
    error++;
    printf("ERROR : the pos is not the good one (POS_NORTH) %d\n", tests[0]);
  }
  if(posi[tests[1]] != POS_SOUTH){
    error++;
    printf("ERROR : the pos is not the good one (POS_SOUTH) %d\n", tests[1]);
  }
  //test 4 : CARD_ROAD_TURN_LEFT_CITY NORTH
  move_test.card = CARD_ROAD_TURN_LEFT_CITY;
  move_test.dir = NORTH;
  move_test.place = POS_SOUTH;
  get_tiles_to_tests(&move_test, tests, &nb_tests);
  if(nb_tests != 2){
    error++;
    printf("ERROR : should have only one card to test\n");
  }
  if(tiles[tests[0]] != board__get_tile(&b, 41, 42)){
    error++;
    printf("ERROR : the tile to test is not the good one (41,42)\n");
  }
  if(tiles[tests[1]] != board__get_tile(&b, 42, 41)){
    error++;
    printf("ERROR : the tile to test is not the good one (42,41)\n");
  }
  if(posi[tests[1]] != POS_EAST){
    error++;
    printf("ERROR : the pos is not the good one (POS_EAST) %d\n", tests[1]);
  }
  if(posi[tests[0]] != POS_SOUTH){
    error++;
    printf("ERROR : the pos is not the good one (POS_SOUTH  ) %d\n", tests[0]);
  }
  //test 5 : CARD_ROAD_TURN WEST
  move_test.card = CARD_ROAD_TURN;
  move_test.dir = WEST;
  move_test.place = POS_SOUTH;
  get_tiles_to_tests(&move_test, tests, &nb_tests);
  if(nb_tests != 2){
    error++;
    printf("ERROR : should have only one card to test\n");
  }
  if(tiles[tests[1]] != board__get_tile(&b, 43, 42)){
    error++;
    printf("ERROR : the tile to test is not the good one (43,42)\n");
  }
  if(tiles[tests[0]] != board__get_tile(&b, 42, 43)){
    error++;
    printf("ERROR : the tile to test is not the good one (42,41)\n");
  }
  if(posi[tests[0]] != POS_WEST){
    error++;
    printf("ERROR : the pos is not the good one (POS_WEST) %d\n", tests[1]);
  }
  if(posi[tests[1]] != POS_NORTH){
    error++;
    printf("ERROR : the pos is not the good one (POS_NORTH) %d\n", tests[0]);
  }
  //test 6 : CARD_JUNCTION_CITY SOUTH POS_SOUTH
  move_test.card = CARD_JUNCTION_CITY;
  move_test.dir = SOUTH;
  move_test.place = POS_SOUTH;
  get_tiles_to_tests(&move_test, tests, &nb_tests);
  if(nb_tests != 1){
    error++;
    printf("ERROR : should have only one card to test\n");
  }
  if(tiles[tests[0]] != board__get_tile(&b, 41, 42)){
    error++;
    printf("ERROR : the tile to test is not the good one (42,41)\n");
  }
  if(posi[tests[0]] != POS_SOUTH){
    error++;
    printf("ERROR : the pos is not the good one (POS_SOUTH) %d\n", tests[1]);
  }
  //test 7 : CARD_JUNCTION_THREE EAST POS_WEST
  move_test.card = CARD_JUNCTION_CITY;
  move_test.dir = EAST;
  move_test.place = POS_WEST;
  get_tiles_to_tests(&move_test, tests, &nb_tests);
  if(nb_tests != 1){
    error++;
    printf("ERROR : should have only one card to test\n");
  }
  if(tiles[tests[0]] != board__get_tile(&b, 41, 42)){
    error++;
    printf("ERROR : the tile to test is not the good one (42,41)\n");
  }
  if(posi[tests[0]] != POS_SOUTH){
    error++;
    printf("ERROR : the pos is not the good one (POS_SOUTH) %d\n", tests[1]);
  }
  //test 8 : CARD_JUNCTION_FOUR SOUTH POS_NORTH
  move_test.card = CARD_JUNCTION_CITY;
  move_test.dir = SOUTH;
  move_test.place = POS_NORTH;
  get_tiles_to_tests(&move_test, tests, &nb_tests);
  if(nb_tests != 1){
    error++;
    printf("ERROR : should have only one card to test\n");
  }
  if(tiles[tests[0]] != board__get_tile(&b, 43, 42)){
    error++;
    printf("ERROR : the tile to test is not the good one (42,41)\n");
  }
  if(posi[tests[0]] != POS_NORTH){
    error++;
    printf("ERROR : the pos is not the good one (POS_NORTH) %d\n", tests[1]);
  }
  //free dynamic variables
  board__free(&b);
  //end of the tests
  if(!error){
    printf(BOLD(GREEN("\t\tPASSED"))"\n");
  }
}


void test__move_first_meeple()
{
  int error = 0;
  printf("\t%s", __func__);
  //initialize the tested zones
  struct list l;
  list__init(&l, NULL, NULL);
  struct farm farm = {0, {0, 0, 0, 0, 0}, NULL};
  struct city city = {0, NULL, l, 0, {0,0,0,0,0}};
  struct road road = {NULL, NULL, 0, 0, 0, 0, {0,0,0,0,0}};
  //test 1 : empty zones
  if(!meeple__first(&farm, FARM)){
    error++;
    printf("ERROR : there should be no meeple in the farm\n");
  }
  if(!meeple__first(&city, CITY)){
    error++;
    printf("ERROR : there should be no meeple in the city\n");
  }
  if(!meeple__first(&road, ROAD)){
    error++;
    printf("ERROR : there should be no meeple on the road\n");
  }
  //tes 2 : zones already occupied
  farm.farmers[4] = 1;
  city.knights[3] = 1;
  road.thieves[2] = 1;
  if(meeple__first(&farm, FARM)){
    error++;
    printf("ERROR : there should already be some meeple in the farm\n");
  }
  if(meeple__first(&city, CITY)){
    error++;
    printf("ERROR : there should already be some meeple in the city\n");
  }
  if(meeple__first(&road, ROAD)){
    error++;
    printf("ERROR : there should already be some meeple on the road\n");
  }
  list__free(&l);
  //end of the tests
  if(!error){
    printf(BOLD(GREEN("\t\t\tPASSED"))"\n");
  }
}

void test__move_is_valid()
{
  int error = 0;
  printf("\t%s", __func__);
  //initialize
  struct board b;
  board__init(&b);
  //initialize the move_test
  int returned_value = 666;
  struct move move_test;
  move_test.check = FAILED;
  move_test.player = 1;
  move_test.onto.x = 42;
  move_test.onto.y = 42;
  move_test.card = CARD_MONASTERY_ALONE;
  move_test.dir = NORTH;
  move_test.place = POS_CENTER;
  //test 0 : valid id player
  move_test.player = -1;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 0){
    error++;
    printf("ERROR : the id player is not valid (< 0) (%d while 0 expected)\n", returned_value);
  }
  move_test.player = 5;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 0){
    error++;
    printf("ERROR : the id player is not valid (>= MAX_NB_PLAYERS) (%d while 0 expected)\n", returned_value);
  }
  move_test.player = 0;
  //test 1 : valid id card
  move_test.card = -1;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 1){
    error++;
    printf("ERROR : the id card is not valid (< 0) (%d while 1 expected)\n", returned_value);
  }
  move_test.card = LAST_CARD;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 1){
    error++;
    printf("ERROR : the id player is not valid (>= LAST_CARD) (%d while 1 expected)\n", returned_value);
  }
  move_test.card = CARD_MONASTERY_ALONE;
  //test 2 : valid direction
  move_test.dir = -1;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 2){
    error++;
    printf("ERROR : the direction is not valid (< 0) (%d while 2 expected)\n", returned_value);
  }
  move_test.dir = 4;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 2){
    error++;
    printf("ERROR : the id player is not valid (>= LAST_DIR) (%d while 2 expected)\n", returned_value);
  }
  move_test.dir = NORTH;
  //test 3 : valid tile (e.g. the position is not yet occupied)
  move_test.onto.x = 0;
  move_test.onto.y = 0;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 3){
    error++;
    printf("ERROR : the tile is not valid (already played) (%d while 3 expected)\n", returned_value);
  }
  //test 4 : valid position (e.g. the position is next to an already played tile)
  move_test.onto.x = 42;
  move_test.onto.y = 42;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 4){
    error++;
    printf("ERROR : the tile is not valid (unconnected to other tiles) (%d while 4 expected)\n", returned_value);
  }
  //test 5 : valid matching edges
  move_test.onto.x = 0;
  move_test.onto.y = -1;
  move_test.card = CARD_CITY_ALL_SIDES;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 5){
    error++;
    printf("ERROR : the edges does not match (city to farm) (%d while 5 expected)\n", returned_value);
  }
  move_test.onto.x = 1;
  move_test.onto.y = 0;
  move_test.card = CARD_MONASTERY_ALONE;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 6){
    error++;
    printf("ERROR : the edges does not match (road to farm) (%d while 6 expected)\n", returned_value);
  }
  move_test.onto.x = -1;
  move_test.onto.y = 0;
  move_test.card = CARD_CITY_ALL_SIDES;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 7){
    error++;
    printf("ERROR : the edges does not match (city to road) (%d while 7 expected)\n", returned_value);
  }
  move_test.onto.x = 0;
  move_test.onto.y = 1;
  move_test.card = CARD_MONASTERY_ALONE;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 8){
    error++;
    printf("ERROR : the edges does not match (city to farm) (%d while 8 expected)\n", returned_value);
  }
  // test 6 : valid no meeple placed
  move_test.onto.x = 0;
  move_test.onto.y = -1;
  move_test.card = CARD_MONASTERY_ALONE;
  move_test.place = NO_MEEPLE;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 1006){
    error++;
    printf("ERROR : the move should be valid (%d while 1006 expected)\n", returned_value);
  }
  //test 7 : valid no more meeple
  move_test.place = POS_CENTER;
  move_test.player = 1;
  b.followers_left[1] = 0;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 9){
    error++;
    printf("ERROR : the should not be able to place a meeple (%d while 9 expected)\n", returned_value);
  }
  //test 8 : valid place for the meeple
  b.followers_left[1] = 5;
  move_test.card = CARD_MONASTERY_ROAD;
  move_test.place = POS_SOUTH_WEST;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 10){
    error++;
    printf("ERROR : the meeple place is not avaible (%d while 10 expected)\n", returned_value);
  }
  //test 9 : valid move for a monk
  move_test.place = POS_CENTER;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 1005){
    error++;
    printf("ERROR : the move should be valid with a monk (%d while 1005 expected)\n", returned_value);
  }
  // test 10 : valid move for a thief
  move_test.card = CARD_ROAD_STRAIGHT;
  move_test.place = POS_CENTER;
  move_test.onto.x = 1;
  move_test.onto.y = 0;
  move_test.dir = NORTH;
  move_test.player = 1;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 1004){
    error++;
    printf("ERROR : the move should be valid with a thief (%d while 1004 expected)\n", returned_value);
  }
  board__add_tile(&b, &move_test);
  move_test.card = CARD_JUNCTION_FOUR;
  move_test.place = POS_NORTH;
  move_test.onto.x = 2;
  move_test.onto.y = 0;
  move_test.dir = NORTH;
  move_test.player = 3;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 11){
    error++;
    printf("ERROR : the move should be unvalid with a thief (%d while 11 expected)\n", returned_value);
  }
  move_test.place = POS_SOUTH;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 1004){
    error++;
    printf("ERROR : the move should be valid with a thief (%d while 1004 expected)\n", returned_value);
  }
  board__add_tile(&b, &move_test);
  move_test.onto.x = 3;
  move_test.onto.y = -1;
  board__add_tile(&b, &move_test);
  move_test.onto.x = 3;
  move_test.onto.y = 0;
  move_test.card = CARD_ROAD_TURN;
  move_test.dir = EAST;
  move_test.place = POS_WEST;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 11){
    error++;
    printf("ERROR : the move should be valid without a thief (%d while 11 expected)\n", returned_value);
  }
  move_test.onto.x = 2;
  move_test.onto.y = -1;
  move_test.dir = WEST;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 1004){
    error++;
    printf("ERROR : the move should be valid with a thief  (%d while 1004 expected)\n", returned_value);
  }
  move_test.onto.x = 4;
  move_test.onto.y = -1;
  move_test.place = NO_MEEPLE;
  move_test.card = CARD_ROAD_STRAIGHT;
  move_test.dir = NORTH;
  board__add_tile(&b, &move_test);
  move_test.onto.x = 5;
  move_test.place = POS_CENTER;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 11){
    error++;
    printf("ERROR : the move should not be valid with a thief  (%d while 11 expected)\n", returned_value);
  }
  // test 11 : valid move for a knight
  board__free(&b);
  board__init(&b);
  move_test.card = CARD_CITY_ALL_SIDES;
  move_test.place = POS_CENTER;
  move_test.onto.x = 0;
  move_test.onto.y = 1;
  move_test.dir = NORTH;
  move_test.player = 1;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 1002){
    error++;
    printf("ERROR : the move should be valid with a knight (%d while 1002 expected)\n", returned_value);
  }
  move_test.place = NO_MEEPLE;
  board__add_move(&b, move_test);
  board__add_tile(&b, &move_test);
  move_test.card = CARD_PLAIN_TWO_CITIES;
  move_test.place = POS_SOUTH;
  move_test.onto.x = 0;
  move_test.onto.y = 2;
  move_test.dir = SOUTH;
  move_test.player = 1;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 1003){
    error++;
    printf("ERROR : the move should be valid with a knight (%d while 1003 expected)\n", returned_value);
  }
  board__add_move(&b, move_test);
  board__add_tile(&b, &move_test);
  move_test.card = CARD_CITY_ALL_SIDES;
  move_test.place = NO_MEEPLE;
  move_test.onto.x = -1;
  move_test.onto.y = 2;
  move_test.dir = SOUTH;
  move_test.player = 1;
  board__add_move(&b, move_test);
  board__add_tile(&b, &move_test);
  move_test.card = CARD_CITY_ALL_SIDES;
  move_test.place = POS_CENTER;
  move_test.onto.x = -1;
  move_test.onto.y = 1;
  move_test.dir = NORTH;
  move_test.player = 1;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 1001){
    error++;
    printf("ERROR : the move should not be valid with a knight (%d while 1001 expected)\n", returned_value);
  }  //test 12 : valid with farmer
  move_test.card = CARD_CITY_ONE_SIDE;
  move_test.place = NO_MEEPLE;
  move_test.onto.x = 1;
  move_test.onto.y = 1;
  move_test.dir = NORTH;
  move_test.player = 1;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 1006){
    error++;
    printf("ERROR : the move should be valid without a farmer (%d while 1006 expected)\n", returned_value);
  }
  board__add_move(&b, move_test);
  board__add_tile(&b, &move_test);
  move_test.card = CARD_CITY_ONE_SIDE;
  move_test.place = POS_CENTER;
  move_test.onto.x = 1;
  move_test.onto.y = 2;
  move_test.dir = SOUTH;
  move_test.player = 1;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 1001){
    error++;
    printf("ERROR : the move should be valid with a farmer (%d while 1001 expected)\n", returned_value);
  }
  board__add_move(&b, move_test);
  board__add_tile(&b, &move_test);
  move_test.card = CARD_ROAD_TURN;
  move_test.place = POS_SOUTH_WEST;
  move_test.onto.x = 0;
  move_test.onto.y = 3;
  move_test.dir = WEST;
  move_test.player = 1;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 1003){
    error++;
    printf("ERROR : the move should be valid with a farmer (%d while 1001 expected)\n", returned_value);
  }
  board__add_move(&b, move_test);
  board__add_tile(&b, &move_test);
  move_test.card = CARD_MONASTERY_ROAD;
  move_test.place = POS_NORTH;
  move_test.onto.x = 1;
  move_test.onto.y = 3;
  move_test.dir = SOUTH;
  move_test.player = 1;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 12){
    error++;
    printf("ERROR : the move should be valid with a farmer (%d while 1001 expected)\n", returned_value);
  }
  move_test.card = CARD_ROAD_TURN;
  move_test.place = POS_SOUTH_WEST;
  move_test.onto.x = 0;
  move_test.onto.y = 4;
  move_test.dir = NORTH;
  move_test.player = 1;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 12){
    error++;
    printf("ERROR : the move should be valid with a farmer (%d while 1001 expected)\n", returned_value);
  }
  move_test.card = CARD_ROAD_TURN;
  move_test.place = POS_NORTH_EAST;
  move_test.onto.x = 1;
  move_test.onto.y = 0;
  move_test.dir = EAST;
  move_test.player = 1;
  returned_value = move__is_valid_aux(&b, &move_test);
  if(returned_value != 12){
    error++;
    printf("ERROR : the move should be valid with a farmer (%d while 12 expected)\n", returned_value);
  }
  //freeing variables
  board__free(&b);
  //end of the tests
  if(!error){
    printf(BOLD(GREEN("\t\t\tPASSED"))"\n");
  }
}

void test__move_get_all_possible_moves()
{
  int error = 0;
  printf("\t%s", __func__);
  //initialize variables
  struct board b;
  board__init(&b);
  struct list list_test;
  list__init(&list_test, move_copy, free);
  enum card_id card_test = LAST_CARD;
  //test 1 : only one preexistent edges matching with all the tile edges
  card_test = CARD_MONASTERY_ALONE;
  move__get_all_possible_moves(&b, &list_test, card_test, 0);
  if( list__size(&list_test) != 4*3){
    error++;
    printf("ERROR : there should be 12 (4*3) possible move and not %ld\n", list__size(&list_test));
  }
  //test 2 : two preexistent edges matching with all the tile edges
  card_test = CARD_JUNCTION_FOUR;
  list__free(&list_test);
  list__init(&list_test, move_copy, free);
  assert((list__size(&list_test) == 0));
  move__get_all_possible_moves(&b, &list_test, card_test, 0);
  if( list__size(&list_test) != 4*9*2){
    error++;
    printf("ERROR : there should be 72 (4*9*2) possible move and not %ld\n", list__size(&list_test));
  }
  //test 3 : two preexistent edges matching with all three edges and one preexistent edge matching with one edge
  card_test = CARD_JUNCTION_THREE;
  list__free(&list_test);
  list__init(&list_test, move_copy, free);
  assert((list__size(&list_test) == 0) && 2 ==2);
  move__get_all_possible_moves(&b, &list_test, card_test, 0);
  if( list__size(&list_test) != (3*7*2+1*7)){
    error++;
    printf("ERROR : there should be %d (3*7*2+1*7) possible move and not %ld\n", (3*7*2+1*7), list__size(&list_test));
  }
  //test 4 : no preexistent edge matching with any tile edges
  card_test = CARD_JUNCTION_THREE;
  b.field[72][72].north.biome = CITY;
  b.field[72][72].south.biome = CITY;
  b.field[72][72].east.biome = CITY;
  b.field[72][72].west.biome = CITY;
  b.moves[0].card = CARD_CITY_ALL_SIDES;
  list__free(&list_test);
  list__init(&list_test, move_copy, free);
  assert((list__size(&list_test) == 0) && 3==3);
  move__get_all_possible_moves(&b, &list_test, card_test, 0);
  if( list__size(&list_test) != (0)){
    error++;
    printf("ERROR : there should be %d (0) possible move and not %ld\n", (0), list__size(&list_test));
  }
  //free variables
  list__free(&list_test);
  board__free(&b);
  //test end
  if(!error){
    printf(BOLD(GREEN("\tPASSED"))"\n");
  }
}

int main()
{
  printf("Testing move...\n");

  test__move_tiles_to_tests();
  test__move_first_meeple();
  test__move_is_valid();
  test__move_get_all_possible_moves();
}
