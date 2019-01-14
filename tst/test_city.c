#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "constants.h"
#include "list.h"
#include "reference.h"
#include "tile.h"
#include "board.h"
#include "city.h"

#define TRUE 1
#define FALSE 0


void test__city__count_points()
{
  printf( "\ttest adding tiles in a city \t \t" );

  struct board gameboard;
  struct tile first_tile;
  struct move first_move = {
    VALID,
    1,
    CARD_PLAIN_CITY,
    {0, 0},
    NORTH,
    NO_MEEPLE };

  struct move second_move = {
    VALID,
    1,
    CARD_PLAIN_CITY,
    {0, 1},
    NORTH,
    NO_MEEPLE };

  struct move third_move = {
    VALID,
    1,
    CARD_PLAIN_CITY,
    {0, 2},
    NORTH,
    NO_MEEPLE };

  tile__init( &first_tile, &first_move );
  board__init( &gameboard );

  struct city * city = city__create( &first_move, &gameboard );

  assert( city__count_points( city ) == 1 ); // city not closed

  assert( city__count_points( city ) == 1 ); // one tile in city

  city__add_tile( &second_move, city, &gameboard );

  assert( city__count_points( city ) == 2 ); // two tiles in the city

  city__add_tile( &third_move, city, &gameboard );

  city->complete = TRUE;

  assert( city__count_points( city ) == 6 ); // three tiles city

  city->complete = FALSE;

  city__free( city );
  board__free( &gameboard );

  printf( BOLD(GREEN( "PASSED" )) );
  printf( "\n" );
}

void test__city__add_meeple()
{
  printf( "\ttest adding meeples \t\t" );

  struct board gameboard;

  struct move first_move = {
    VALID,
    1,
    CARD_PLAIN_CITY,
    {0, 0},
    NORTH,
    POS_NORTH }; // meeple in this one

    struct move second_move = {
      VALID,
      1,
      CARD_PLAIN_CITY,
      {0, 1},
      NORTH,
      NO_MEEPLE }; // no meeple

      board__init( &gameboard );

  struct city * city = city__create( &first_move, &gameboard );

  city__add_meeple( &first_move, city, &gameboard ); // meeple in city

  assert( city->knights[0] == 0 );
  assert( city->knights[1] == 1 );
  assert( city->knights[2] == 0 );

  city__add_meeple( &second_move, city, &gameboard ); // no meeple

  assert( city->knights[0] == 0 );
  assert( city->knights[1] == 1 );
  assert( city->knights[2] == 0 );

  city__free( city );
  board__free( &gameboard );

  printf( BOLD(GREEN("\tPASSED")) );
  printf( "\n" );
}

void test__city__create()
{
  printf( "\ttest city__create \t" );

  struct board gameboard;

  struct move first_move = {
    VALID,
    1,
    CARD_PLAIN_CITY,
    {0, 0},
    NORTH,
    POS_NORTH }; // meeple in this one

  board__init( &gameboard );

  struct city * city1 = city__create( &first_move, &gameboard );
  list__push_back( &( gameboard.cities ), city1 );

  assert( city1->id == 1 ); // first city was created in board__init() in the board
  assert( list__size( &city1->tiles ) == 1 ); // 1 tiles
  assert( city1->complete == FALSE );

  for (int i = 0; i < MAX_NB_PLAYERS; ++i ) {
    assert( city1->knights[i] == 0 );
  }

  struct move second_move = {
    VALID,
    1,
    CARD_CITY_ONE_SIDE,
    {0, 2},
    EAST,
    NO_MEEPLE };

  struct city * city2 = city__create( &second_move, &gameboard );

  assert( city2->id == 2 ); // second city in the board
  assert( list__size( &city2->tiles ) == 1 ); // 1 tiles
  assert( city2->complete == FALSE );

  for (int i = 0; i < MAX_NB_PLAYERS; ++i ) {
    assert( city2->knights[i] == 0 );
  }

  board__free( &gameboard );
  city__free( city2 );

  printf( BOLD(GREEN("\t\tPASSED")) );
  printf( "\n" );
}

void test__city__two_cities_check_in()
{
  struct board gameboard;

  struct move first_move = {
    VALID,
    1,
    CARD_PLAIN_CITY,
    {1, 1},
    NORTH,
    POS_NORTH }; // meeple in this one

  struct move second_move = {
    VALID,
    1,
    CARD_PLAIN_CITY,
    {1, 2},
    NORTH,
    NO_MEEPLE }; // no meeple

  board__init( &gameboard );

  struct city * city = city__create( &first_move, &gameboard );

  city__add_tile( &second_move, city, &gameboard );

  board__free( &gameboard );
  city__free( city );
}

void test__city__check_closure()
{
  printf( "\ttest city__check_closure \t" );

  struct board gameboard;

  struct move first_move = {
    VALID,
    1,
    CARD_CITY_ONE_SIDE,
    {2, 2},
    NORTH,
    POS_NORTH }; // meeple in this one

  struct move second_move = {
    VALID,
    1,
    CARD_CITY_ONE_SIDE,
    {2, 1},
    SOUTH,
    NO_MEEPLE }; // no meeple

  board__init( &gameboard );

  struct city * city = city__create( &first_move, &gameboard );

  assert( city__check_closure( city, &gameboard ) == FALSE ); // just one tile, city not closed

  city__add_tile( &second_move, city, &gameboard ); //tile to close the city

  board__add_tile( &gameboard, &first_move ); //first tile
  board__add_tile( &gameboard, &second_move ); //second tile

  assert( city__check_closure( city, &gameboard ) == TRUE ); // city closed

  city__free( city );
  board__free( &gameboard );

  printf( BOLD(GREEN("\tPASSED")) );
  printf( "\n" );
}

void test__shitty_cards() //test for the two cards with 2 cities
{
  printf( "\ttest two_cities cards \t\t" );

  struct board gameboard;

  struct move first_move = {
    VALID,
    1,
    CARD_PLAIN_TWO_CITIES,
    {1, 1},
    NORTH,
    NO_MEEPLE };

  board__init( &gameboard );

  board__add_tile( &gameboard, &first_move );

  struct tile * first_tile = board__get_tile( &gameboard, first_move.onto.x, first_move.onto.y );

  city__update( &gameboard, first_tile );

  board__free(&gameboard);

  printf( BOLD(GREEN("\tPASSED")) );
  printf("\n");
}

void test__city__merge()
{
  printf( "\ttest city__merge \t " );

  struct board gameboard;

  struct move first_move = {
    VALID,
    1,
    CARD_CITY_ONE_SIDE,
    {2, 2},
    WEST,
    NO_MEEPLE }; // meeple in this one

  struct move second_move = {
    VALID,
    1,
    CARD_CITY_ONE_SIDE,
    {0, 2},
    EAST,
    NO_MEEPLE }; // no meeple

  struct move fusion_move = {
    VALID,
    1,
    CARD_CITY_TUNNEL_SHLD,
    {1, 2},
    NORTH,
    NO_MEEPLE }; //move that would connect first and second

  board__init( &gameboard );

  struct city * city1 = city__create( &first_move, &gameboard );
  list__push_back( &( gameboard.cities ), city1 );
  list__push_back( &( gameboard.ref_cities ), &( city1->id ) );

  struct city * city2 = city__create( &second_move, &gameboard );
  list__push_back( &( gameboard.cities ), city2 );
  list__push_back( &( gameboard.ref_cities ), &( city2->id ) );

  city__add_tile( &fusion_move, city1, &gameboard );

  city__merge( city1, city2, &gameboard );

  board__free( &gameboard );

  printf( BOLD(GREEN("\t\tPASSED") ));
  printf( "\n" );
}

void test__city__update()
{
  printf( "\ttest city__update ... \n" );

  printf( "\t    test create one city \t" );

  struct board gameboard;

  struct move first_move = {
    VALID,
    1,
    CARD_CITY_ONE_SIDE,
    {2, 2},
    WEST,
    NO_MEEPLE }; // meeple in this one

  struct move second_move = {
    VALID,
    1,
    CARD_CITY_ONE_SIDE,
    {0, 2},
    EAST,
    NO_MEEPLE }; // no meeple

  struct move fusion_move = {
    VALID,
    1,
    CARD_CITY_TUNNEL_SHLD,
    {1, 2},
    NORTH,
    POS_CENTER }; //move that would connect first and second

  board__init( &gameboard );

  board__add_tile( &gameboard, &first_move );

  struct tile * first_tile = board__get_tile( &gameboard, first_move.onto.x, first_move.onto.y );

  //city__update( &gameboard, first_tile );
  //called in board__add_tile

  assert( list__size( &(gameboard.cities) ) == 2 );

  assert( * ( ( int * ) list__get( &(gameboard.ref_cities), 0 ) ) == 0 );

  assert( first_tile->west.biome == CITY );

  printf( BOLD(GREEN("\tPASSED")) );

  printf( "\n" );

  printf("\t    test add second city \t " );

  board__add_tile( &gameboard, &second_move );

  struct tile * second_tile = board__get_tile( &gameboard, second_move.onto.x, second_move.onto.y );


  assert( list__size( &(gameboard.cities) ) == 3 );

  assert( second_tile->east.biome == CITY );

  assert( * ( ( int * ) list__get( &(gameboard.ref_cities), 1 ) ) == 1 );

  board__add_tile( &gameboard, &fusion_move );

  assert( ( (struct city *) gameboard.cities.d[1])->knights[1] = 1 );

  board__free(&gameboard);

  printf( BOLD(GREEN("\tPASSED")) );

  printf( "\n" );
}

void test__initial_tile()
{
  printf( "\ttest update initial tile \t " );

  struct board gameboard;

  struct move first_move = {
    VALID,
    1,
    CARD_CITY_ONE_SIDE,
    {0, -1},
    SOUTH,
    NO_MEEPLE }; // meeple in this one

  board__init( &gameboard );

  board__add_tile( &gameboard, &first_move );

  struct tile * first_tile = board__get_tile( &gameboard, first_move.onto.x, first_move.onto.y );

  city__update( &gameboard, first_tile );

  board__free(&gameboard);

  printf( BOLD(GREEN("\tPASSED")) );

  printf( "\n" );
}


int main()
{
  printf("Testing cities ...\n");
  test__city__create();
  test__city__count_points();
  test__city__add_meeple();
  test__city__check_closure();
  test__city__update();
  test__shitty_cards();
  test__city__merge();
  test__initial_tile();
  return 0;
}
