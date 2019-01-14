/**
 * \file city.c
 */

#include <stdlib.h>
#include <stdio.h>

#include "constants.h"
#include "interface.h"
#include "board.h"
#include "reference.h"
#include "list.h"
#include "tile.h"
#include "city.h"

#define SUCCESS 0
#define ERROR -1

#define TRUE 1
#define FALSE 0

#define DEFAULT_REF = 100


struct city * city__pointer(struct city *c)
{
  return c->pointer;
}

struct city *city__create(struct move *first_tile, struct board *gameboard)
{
  struct city *new_city = malloc( sizeof( struct city ) );

  new_city->id = list__size( &( gameboard->cities ) );
  new_city->pointer = new_city;

  list__init( &new_city->tiles, NULL, NULL );
  list__push_back( &new_city->tiles , first_tile );

  new_city->complete = FALSE;

  for ( int i = 0; i < MAX_NB_PLAYERS; ++i ) {
    new_city->knights[i] = 0;
  }
  return new_city;
}

struct tile * city__get_neighbour_tile( struct tile * city_tile, int side, struct board * gameboard )
{
  struct tile * neighbour_tile;

  switch ( side ) {
    case NORTH:
    neighbour_tile = board__get_tile( gameboard, city_tile->move->onto.x - 1, city_tile->move->onto.y );
    break;
    case WEST:
    neighbour_tile = board__get_tile( gameboard, city_tile->move->onto.x, city_tile->move->onto.y - 1 );
    break;
    case SOUTH:
    neighbour_tile = board__get_tile( gameboard, city_tile->move->onto.x + 1, city_tile->move->onto.y );
    break;
    case EAST:
    neighbour_tile = board__get_tile( gameboard, city_tile->move->onto.x, city_tile->move->onto.y + 1 );
    break;
    default:
    return NULL;
    break;
  }
  return neighbour_tile;
}

struct city * city__get_tile_city( struct tile * neighbour_tile, int side, struct board * gameboard )
{
  struct city * city = NULL;

  switch ( side % 4 ) {
    case NORTH:
    city = ref__get( &(gameboard->ref_cities), neighbour_tile->north.id1 );
    break;
    case WEST:
    city = ref__get( &(gameboard->ref_cities), neighbour_tile->west.id1 );
    break;
    case SOUTH:
    city = ref__get( &(gameboard->ref_cities), neighbour_tile->south.id1);
    break;
    case EAST:
    city = ref__get( &(gameboard->ref_cities), neighbour_tile->east.id1 );
    break;
    default:
    break;
  }
  return city;
}

int city__change_edge_id( struct tile * to_change, int side, struct city * city, struct board * gameboard )
{
  int ref_id = -1;
  switch ( side ) {
    case NORTH:
    ref_id = to_change->north.id1;
    break;
    case WEST:
    ref_id = to_change->west.id1;
    break;
    case SOUTH:
    ref_id = to_change->south.id1;
    break;
    case EAST:
    ref_id = to_change->east.id1;
    break;
    default:
    return ERROR;
    break;
  }
  ref__modif( &(gameboard->ref_cities), ref_id, city );

  return SUCCESS;
}

int city__update_side( int *already_connected,
                       struct board *gameboard,
                       struct tile *city_tile,
                       struct city **c,
                       int side )
{
  struct tile *neighbour_tile = NULL;
  struct city *neighbour_city = NULL;
  struct city *new_city = NULL;

  if ( *already_connected == FALSE ) {
    new_city = city__create( city_tile->move, gameboard );
    city__change_edge_id( city_tile, side, new_city, gameboard );
    list__push_back( &(gameboard->cities), new_city );
    *c = new_city;
    if ( ( city_tile->move->card != CARD_PLAIN_TUNNEL ) && ( city_tile->move->card != CARD_PLAIN_TWO_CITIES ) ) {
      *already_connected = TRUE;
    }
  }
  neighbour_tile = city__get_neighbour_tile( city_tile, side, gameboard );
  if ( tile__played( neighbour_tile ) ) {
    neighbour_city = city__get_tile_city( neighbour_tile, side + 2, gameboard ); // +2 in order to obtain the side next to our current tile
    city__merge( *c, neighbour_city, gameboard );
    *c = city__get_tile_city( city_tile, side, gameboard );
  }

  return SUCCESS;
}

int city__update_ids( struct tile * tile, struct board * gameboard )
{
  int card = tile->move->card;
  int second = FALSE; //!< boolean set to TRUE when a first city board is set
  int current_ref_id = list__size( &(gameboard->ref_cities) );
  struct edge * current_edge = NULL;

  int * default_pointer = &card;

  list__push_back( &(gameboard->ref_cities), default_pointer );

  for ( int i = 0; i < 4; ++i ) {
    switch ( i ) {
      case NORTH:
      current_edge = &(tile->north);
      break;
      case WEST:
      current_edge = &(tile->west);
      break;
      case SOUTH:
      current_edge = &(tile->south);
      break;
      case EAST:
      current_edge = &(tile->east);
      break;
      default:
      return ERROR;
      break;
    }
    if ( current_edge->biome == CITY ) {
      if ( ( card == CARD_PLAIN_TUNNEL ) || ( card == CARD_PLAIN_TWO_CITIES ) ) {
        if ( second == FALSE ) {
          second = TRUE;
        } else {
          ++current_ref_id;
          list__push_back( &(gameboard->ref_cities), default_pointer );
        }
      }

      current_edge->id1 = current_ref_id;
      current_edge->id2 = current_ref_id;
    }
  }
  return SUCCESS;
}

int city__update( struct board *gameboard, struct tile *new_tile )
{
  int added_to_neighbour = FALSE;
  struct city *tile_city = NULL;
  int city_to_update[4] = { FALSE, FALSE, FALSE, FALSE };

  if ( new_tile->north.biome == CITY ) {
    city_to_update[NORTH] = TRUE;
  }
  if ( new_tile->west.biome == CITY ) {
    city_to_update[WEST] = TRUE;
  }
  if ( new_tile->south.biome == CITY ) {
    city_to_update[SOUTH] = TRUE;
  }
  if ( new_tile->east.biome == CITY ) {
    city_to_update[EAST] = TRUE;
  }
  city__update_ids( new_tile, gameboard );

  for ( int side = 0; side < 4; ++side ) {
    if ( city_to_update[side] == TRUE ) {
      city__update_side( &added_to_neighbour, gameboard, new_tile, &tile_city, side );
    }
  }
  city__add_meeple( new_tile->move, tile_city, gameboard );
  for ( int side = 0; side < 4; ++side ) {
    if ( city_to_update[side] == TRUE ) {
      tile_city = city__get_tile_city( new_tile, side, gameboard );
      tile_city->complete = city__check_closure( tile_city, gameboard );
    }
  }
  return SUCCESS;
}

int city__add_tile( struct move *added_tile, struct city *c, struct board *gameboard )
{
  if ( added_tile == NULL ) {
    return ERROR;
  }
  if ( c == NULL ) {
    return ERROR;
  }
  list__push_back( &c->tiles, added_tile );
  city__add_meeple( added_tile, c, gameboard );
  return SUCCESS;
}

// Changes the value of the knights table in the city according to the move
void city__add_meeple( struct move *added, struct city *c, struct board *gameboard )
{
  struct city * city_double_1;
  struct city * city_double_2;
  struct tile * move_tile = board__get_tile( gameboard, added->onto.x, added->onto.y );

  switch ( added->card ) {
    case CARD_CITY_ALL_SIDES:
    case CARD_CITY_TUNNEL_SHLD:
    case CARD_CITY_TUNNEL:
    case CARD_CITY_THREE:
    case CARD_CITY_THREE_SHLD:
    case CARD_CITY_THREE_ROAD:
    case CARD_CITY_THREE_ROAD_SHLD:
    if ( added->place == POS_CENTER )
      c->knights[ added->player ] += 1;
    break;
    case CARD_CITY_ONE_SIDE:
    case CARD_ROAD_TURN_RIGHT_CITY:
    if ( added->place == POS_NORTH )
      c->knights[ added->player ] += 1;
    break;
    case CARD_ROAD_STRAIGHT_CITY:
    case CARD_ROAD_TURN_LEFT_CITY:
    case CARD_JUNCTION_CITY:
    if ( added->place == POS_EAST )
      c->knights[ added->player ] += 1;
    break;
    case CARD_PLAIN_CITY:
    case CARD_PLAIN_CITY_SHLD:
    case CARD_PLAIN_CITY_ROAD:
    case CARD_PLAIN_CITY_ROAD_SHLD:
    if ( added->place == POS_NORTH )
      c->knights[ added->player ] += 1;
    break;

    case CARD_PLAIN_TUNNEL:
    // two cities W, E
    city_double_1 = city__get_tile_city( move_tile, WEST + added->dir, gameboard );
    city_double_2 = city__get_tile_city( move_tile, EAST + added->dir, gameboard );
    if ( added->place == POS_WEST ) {
      city_double_1->knights[ added->player ] += 1;
    } else if ( added->place == POS_EAST ) {
      city_double_2->knights[ added->player ] += 1;
    }
    break;

    case CARD_PLAIN_TWO_CITIES:
    // two cities S, E
    city_double_1 = city__get_tile_city( move_tile, SOUTH + added->dir, gameboard );
    city_double_2 = city__get_tile_city( move_tile, EAST + added->dir, gameboard );
    if ( added->place == POS_SOUTH ) {
      city_double_1->knights[ added->player ] += 1;
    } else if ( added->place == POS_EAST ) {
      city_double_2->knights[ added->player ] += 1;
    }
    break;

    default:
    break;
  }
}

int city__merge(struct city *c1, struct city *c2, struct board *gameboard)
{
  if ( c1 == NULL ) {
    return ERROR;
  }
  if ( c2 == NULL ) {
    return ERROR;
  }
  if ( c1->id == c2->id ) {
    return ERROR;
  }

  struct city *lost, *preserved;
  if ( c1->id > c2->id ) {
    lost = c1;
    preserved = c2;
  } else {
    lost = c2;
    preserved = c1;
  }
  lost->pointer = preserved;
  for ( size_t i = 0; i < list__size( &lost->tiles ); ++i ) {  // tiles fusion
    list__push_back( &preserved->tiles, lost->tiles.d[i] );
  }

  // meeples fusion
  for ( size_t i = 0; i < MAX_NB_PLAYERS; ++i ) {
    preserved->knights[i] += lost->knights[i];
  }

  for ( size_t i = 0; i < list__size( &( gameboard->ref_cities ) ); ++i ) {
    if ( ( ( struct city * ) gameboard->ref_cities.d[i] )->id  == lost->id ) {
      gameboard->ref_cities.d[i] = preserved;
    }
  }

  return SUCCESS;
}

int city__two_cities_check_in( struct city *c, int dir, struct move *pos_move, struct board *gameboard )
{
  struct city *border_city = NULL;
  struct tile *move_tile = board__get_tile( gameboard, pos_move->onto.x, pos_move->onto.y );
  int border_tile_ref;
  switch ( dir ) {
    case NORTH:
    border_tile_ref = move_tile->north.id1;
    break;
    case WEST:
    border_tile_ref = move_tile->west.id1;
    break;
    case SOUTH:
    border_tile_ref = move_tile->south.id1;
    break;
    case EAST:
    border_tile_ref = move_tile->east.id1;
    break;
    default:
    return ERROR;
    break;
  }
  border_city = ref__get( &(gameboard->ref_cities), border_tile_ref );
  if ( border_city->id == c->id ) {
    return TRUE;
  }
  return FALSE;
}

int city__verify_border( struct move *pos_move, int *around, struct board *gameboard )
{
  struct tile *neighbour_tile = NULL;

  for ( int i = 0; i < 4; ++i) {
    if ( around[i] != 0 ) {
      switch ( i ) {
        case NORTH:
        neighbour_tile = board__get_tile( gameboard, pos_move->onto.x - 1, pos_move->onto.y );
        break;
        case WEST:
        neighbour_tile = board__get_tile( gameboard, pos_move->onto.x, pos_move->onto.y - 1 );
        break;
        case SOUTH:
        neighbour_tile = board__get_tile( gameboard, pos_move->onto.x + 1, pos_move->onto.y );
        break;
        case EAST:
        neighbour_tile = board__get_tile( gameboard, pos_move->onto.x, pos_move->onto.y + 1 );
        break;
        default:
        return ERROR;
      }
      if ( neighbour_tile->played == FALSE ){
        return FALSE;
      }
    }
  }
  return TRUE;
}

int city__locally_closed( struct city *checked, struct move *pos_move, struct board *gameboard )
{
  int needed_around[4] = {0};

  switch ( pos_move->card ) {
    case CARD_CITY_ALL_SIDES:
    needed_around[ ( NORTH + pos_move->dir ) % 4 ] = TRUE;
    needed_around[ ( SOUTH + pos_move->dir ) % 4 ] = TRUE;
    needed_around[ ( EAST + pos_move->dir ) % 4 ] = TRUE;
    needed_around[ ( WEST + pos_move->dir ) % 4 ] = TRUE;
    break;
    case CARD_CITY_THREE:
    case CARD_CITY_THREE_SHLD:
    case CARD_CITY_THREE_ROAD:
    case CARD_CITY_THREE_ROAD_SHLD:
    needed_around[ ( NORTH + pos_move->dir ) % 4 ] = TRUE;
    needed_around[ ( SOUTH + pos_move->dir ) % 4 ] = FALSE;
    needed_around[ ( EAST + pos_move->dir ) % 4 ] = TRUE;
    needed_around[ ( WEST + pos_move->dir ) % 4 ] = TRUE;
    break;
    case CARD_CITY_ONE_SIDE:
    case CARD_ROAD_TURN_RIGHT_CITY:
    needed_around[ ( NORTH + pos_move->dir ) % 4 ] = TRUE;
    needed_around[ ( SOUTH + pos_move->dir ) % 4 ] = FALSE;
    needed_around[ ( EAST + pos_move->dir ) % 4 ] = FALSE;
    needed_around[ ( WEST + pos_move->dir ) % 4 ] = FALSE;
    break;
    case CARD_ROAD_STRAIGHT_CITY:
    case CARD_ROAD_TURN_LEFT_CITY:
    case CARD_JUNCTION_CITY:
    needed_around[ ( NORTH + pos_move->dir ) % 4 ] = FALSE;
    needed_around[ ( SOUTH + pos_move->dir ) % 4 ] = FALSE;
    needed_around[ ( EAST + pos_move->dir ) % 4 ] = TRUE;
    needed_around[ ( WEST + pos_move->dir ) % 4 ] = FALSE;
    break;
    case CARD_PLAIN_CITY:
    case CARD_PLAIN_CITY_SHLD:
    case CARD_PLAIN_CITY_ROAD:
    case CARD_PLAIN_CITY_ROAD_SHLD:
    // city W, N
    needed_around[ ( NORTH + pos_move->dir ) % 4 ] = TRUE;
    needed_around[ ( SOUTH + pos_move->dir ) % 4 ] = FALSE;
    needed_around[ ( EAST + pos_move->dir ) % 4 ] = FALSE;
    needed_around[ ( WEST + pos_move->dir ) % 4 ] = TRUE;
    break;
    case CARD_CITY_TUNNEL_SHLD:
    // city W, E
    needed_around[ ( NORTH + pos_move->dir ) % 4 ] = FALSE;
    needed_around[ ( SOUTH + pos_move->dir ) % 4 ] = FALSE;
    needed_around[ ( EAST + pos_move->dir ) % 4 ] = TRUE;
    needed_around[ ( WEST + pos_move->dir ) % 4 ] = TRUE;
    break;
    case CARD_CITY_TUNNEL:
    // city N, S
    needed_around[ ( NORTH + pos_move->dir ) % 4 ] = TRUE;
    needed_around[ ( SOUTH + pos_move->dir ) % 4 ] = TRUE;
    needed_around[ ( EAST + pos_move->dir ) % 4 ] = FALSE;
    needed_around[ ( WEST + pos_move->dir ) % 4 ] = FALSE;
    break;
    case CARD_PLAIN_TUNNEL:
    // two cities W, E
    needed_around[ ( NORTH + pos_move->dir ) % 4 ] = FALSE;
    needed_around[ ( SOUTH + pos_move->dir ) % 4 ] = FALSE;
    needed_around[ ( EAST + pos_move->dir ) % 4 ] = city__two_cities_check_in( checked, ( EAST + pos_move->dir ) % 4, pos_move, gameboard );
    needed_around[ ( WEST + pos_move->dir ) % 4 ] = city__two_cities_check_in( checked, ( WEST + pos_move->dir ) % 4, pos_move, gameboard );
    break;
    case CARD_PLAIN_TWO_CITIES:
    // two cities S, E
    needed_around[ ( NORTH + pos_move->dir ) % 4 ] = FALSE;
    needed_around[ ( SOUTH + pos_move->dir ) % 4 ] = city__two_cities_check_in( checked, ( SOUTH + pos_move->dir ) % 4, pos_move, gameboard );
    needed_around[ ( EAST + pos_move->dir ) % 4 ] = city__two_cities_check_in( checked, ( EAST + pos_move->dir ) % 4, pos_move, gameboard );
    needed_around[ ( WEST + pos_move->dir ) % 4 ] = FALSE;
    break;
    default:
    break;
  }
  return city__verify_border( pos_move, needed_around, gameboard );
}

int city__check_closure(struct city *c, struct board *gameboard)
{
  struct move *studied;

  for( size_t i = 0; i < list__size( &(c->tiles) ); ++i ) {
    studied = c->tiles.d[i];
    if ( city__locally_closed( c, studied, gameboard ) == FALSE ) {
      return FALSE;
    }
  }
  return TRUE;
}

int city__complete(struct city *c)
{
  return c->complete;
}

int city__count_points(struct city *c)
{
  int points = 0;
  size_t size = list__size( &c->tiles );
  for ( unsigned int i = 0; i < size; ++i ) {
    switch ( ( ( struct move * ) c->tiles.d[i])->card ) {
      case CARD_CITY_THREE_SHLD:
      case CARD_PLAIN_CITY_SHLD:
      case CARD_CITY_TUNNEL_SHLD:
      case CARD_CITY_THREE_ROAD_SHLD:
      case CARD_PLAIN_CITY_ROAD_SHLD:
      case CARD_CITY_ALL_SIDES:
      points += 1;
      break;
      default:
      break;
    }
  }
  points += size;
  if ( city__complete( c ) ) {
    return 2 * points;
  }
  return points;
}

void city__update_points( struct city * city, int * points )
{
  int city_points = city__count_points( city );
  int max = 0;

  for ( unsigned int i = 0; i < MAX_NB_PLAYERS; ++i ) {
    if ( city->knights[i] > max ) {
      max = city->knights[i];
    }
  }
  for ( unsigned int i = 0; i < MAX_NB_PLAYERS; ++i ) {
    if ( ( city->knights[i] == max ) && ( max != 0 ) ) {
      points[i] += city_points;
    }
  }
}

void city__free(void *c)
{
    struct city *cit = c;
    list__free( &cit->tiles );
  free( cit );
}
