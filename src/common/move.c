#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "list.h"
#include "reference.h"
#include "board.h"
#include "move.h"

#include "city.h"
#include "cloister.h"
#include "farm.h"
#include "road.h"


void *move_copy(void *m)
{
    struct move *move = m;
    struct move *new = malloc(sizeof(struct move));
    assert( new != NULL );
    memcpy(new, move, sizeof(struct move));
    return new;
}

void get_tiles_to_tests(struct move *m, int tests[], int * nb_tests )
{
  if (m->card == CARD_MONASTERY_ROAD
    || m->card == CARD_CITY_THREE_ROAD
    || m->card == CARD_CITY_THREE_ROAD_SHLD){
    *nb_tests = 1;
    tests[0] = (m->dir + 2) % 4;
  }
  if (m->card == CARD_ROAD_TURN_RIGHT_CITY
      ||m->card == CARD_PLAIN_CITY_ROAD_SHLD
      ||m->card == CARD_PLAIN_CITY_ROAD){
    *nb_tests = 2;
    tests[0] = (m->dir + 2) %4;
    tests[1] = (m->dir + 3) %4;
  }
  if (m->card == CARD_ROAD_STRAIGHT_CITY
      ||m->card == CARD_ROAD_STRAIGHT){
    *nb_tests = 2;
    tests[0] = (m->dir + 0) %4;
    tests[1] = (m->dir + 2) %4;
  }
  if(m->card == CARD_ROAD_TURN_LEFT_CITY){
    *nb_tests = 2;
    tests[0] = (m->dir + 0) %4;
    tests[1] = (m->dir + 1) %4;
  }
  if(m->card == CARD_ROAD_TURN){
    *nb_tests = 2;
    tests[0] = (m->dir + 2) %4;
    tests[1] = (m->dir + 1) %4;
  }
  if(m->card == CARD_JUNCTION_CITY){
    *nb_tests = 1;
    if(m->place == POS_NORTH){
      tests[0] = (m->dir + 0) %4;
    }
    if(m->place == POS_WEST){
      tests[0] = (m->dir + 1) %4;
    }
    if(m->place == POS_SOUTH){
      tests[0] = (m->dir + 2) %4;
    }
  }
  if(m->card == CARD_JUNCTION_THREE){
    *nb_tests = 1;
    if(m->place == POS_WEST){
      tests[0] = (m->dir + 1) %4;
    }
    if(m->place == POS_SOUTH){
      tests[0] = (m->dir + 2) %4;
    }
    if(m->place == POS_EAST){
      tests[0] = (m->dir + 3) %4;
    }
  }
  if(m->card == CARD_JUNCTION_FOUR){
    *nb_tests = 1;
    if(m->place == POS_NORTH){
      tests[0] = (m->dir + 0) %4;
    }
    if(m->place == POS_WEST){
      tests[0] = (m->dir + 1) %4;
    }
    if(m->place == POS_SOUTH){
      tests[0] = (m->dir + 2) %4;
    }
    if(m->place == POS_EAST){
      tests[0] = (m->dir + 3) %4;
    }
  }
}


void move__get_all_possible_moves(struct board *board, struct list *possible_moves, const enum card_id c, const int player_id)
{
  struct move m;
  m.player = player_id;
  m.card = c;
  // Get all the possible moves
  for (size_t i = 0 ; i < list__size(&board->edges) ; i++) {
    m.onto = *((struct position *)list__get(&board->edges, i));
    for (enum direction dir = NORTH ; dir < NORTH + 4 ; dir++) {
      m.dir = dir;
      int k = 0;
      m.place = allowed_positions[c][k];
      while (m.place != NO_MEEPLE) {
        if (move__is_valid(board, &m)) {
          list__push_back(possible_moves, &m);
        }
        k++;
        m.place = allowed_positions[c][k];
      }
      if (move__is_valid(board, &m)) {
        list__push_back(possible_moves, &m);
      }
    }
  }
}


int meeple__first(void * zone, enum biome type)
{
  int meeples = 0;
  if(type == FARM ){
    struct farm *f = zone;
    for(int i = 0; i < MAX_NB_PLAYERS; i++){
      meeples += f->farmers[i];
    }
  }
  if(type == CITY ){
    struct city *c = zone;
    for(int i = 0; i < MAX_NB_PLAYERS; i++){
      meeples += c->knights[i];
    }
  }
  if(type == ROAD){
    struct road *r = zone;
    for(int i = 0; i < MAX_NB_PLAYERS; i++){
      meeples += r->thieves[i];
    }
  }

  return meeples == 0;
}


int id__not_in(struct list *ref_list, int ids[], int n, int id_value)
{
  int incr = 0;
  while(incr < n && ref__get(ref_list, ids[incr]) != ref__get(ref_list, id_value)) {
    incr++;
  }
  return incr == n;
}


int move__is_valid_aux(struct board* b, struct move* m)
{
    m->check = FAILED;
    // verification id player
    if (m->player >= MAX_NB_PLAYERS){
	     return 0;
    }

    // verification id card
    if (m->card < 0 || m->card >= LAST_CARD){
	     return 1;
    }

    // verification direction
    if (m->dir < 0 || m->dir > 3){
	     return 2;
    }

    //verification tile has not been played yet
    if (tile__played(board__get_tile(b, m->onto.x, m->onto.y))) {
	     return 3;
    }


    int south = tile__played(board__get_tile(b, m->onto.x+1, m->onto.y));
    int west = tile__played(board__get_tile(b, m->onto.x, m->onto.y-1));
    int east = tile__played(board__get_tile(b, m->onto.x, m->onto.y+1));
    int north = tile__played(board__get_tile(b, m->onto.x-1, m->onto.y));

    // verification played tile is connected to the other tiles
    if (!(east || north || south || west)){
	     return 4;
    }

    struct tile *t = board__get_tile(b, m->onto.x, m->onto.y);
    struct tile * above = board__get_tile(b, m->onto.x - 1, m->onto.y);
    struct tile * left = board__get_tile(b, m->onto.x, m->onto.y - 1);
    struct tile * below = board__get_tile(b, m->onto.x + 1, m->onto.y);
    struct tile * right = board__get_tile(b, m->onto.x, m->onto.y + 1);
    tile__init(t, m);

    if (above != NULL && above->played) {
	     t->north.id1 = above->south.id2;
	     t->north.id2 = above->south.id1;
    }
    if (left != NULL && left->played) {
	     t->west.id1 = left->east.id2;
	     t->west.id2 = left->east.id1;
    }
    if (below != NULL && below->played) {
	     t->south.id1 = below->north.id2;
	     t->south.id2 = below->north.id1;
    }
    if (right != NULL && right->played) {
	     t->east.id1 = right->west.id2;
	     t->east.id2 = right->west.id1;
    }


    // verification matching edges
    if (east &&
    	t->east.biome != board__get_tile(b, m->onto.x, m->onto.y+1)->west.biome){
    	return 5;
    }
    if (north &&
    	t->north.biome != board__get_tile(b, m->onto.x-1, m->onto.y)->south.biome){
    	return 6;
    }
    if (south &&
    	t->south.biome != board__get_tile(b, m->onto.x+1, m->onto.y)->north.biome){
    	return 7;
    }
    if (west &&
    	t->west.biome != board__get_tile(b, m->onto.x, m->onto.y-1)->east.biome){
    	return 8;
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////// VERIFICATION MEEPLE ////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////

    // verification no meeple played
    if(m->place == NO_MEEPLE){
      m->check = VALID;
      return 1006;
    }

    // verification can play a meeple
    if(b->followers_left[m->player] <= 0){
      return 9;
    }

    // verification meeple is on an allowed position
    if(m->place != NO_MEEPLE){
      int i = 0;
      int test = 0;
      enum place pos = allowed_positions[m->card][i];
      while (pos != NO_MEEPLE){
        test = test || (m->place == pos);
        i++;
        pos = allowed_positions[m->card][i];
      }
      if (!test){
        return 10;
      }
    }

    // verification the meeple is a monk
    if(t->follower_type == MONK){
      m->check = VALID;
      return 1005;
    }

    // verification the meeple is a thief
    if(t->follower_type == THIEF){
      struct tile *tiles[4] = {board__get_tile(b, m->onto.x-1, m->onto.y),
                                board__get_tile(b, m->onto.x, m->onto.y-1),
                                board__get_tile(b, m->onto.x+1, m->onto.y),
                                board__get_tile(b, m->onto.x, m->onto.y+1)};
      enum place posi[4] = {POS_SOUTH, POS_EAST, POS_NORTH, POS_WEST};
      int nb_tests = 0;
      int tests[4] = {0, 0, 0, 0};
      get_tiles_to_tests(m, tests, &nb_tests);
      int i = 0;
      while (i < nb_tests){
        int n = list__size(&(b->roads));
        int j = 0;
        while (j < n){
          struct road *r = list__get(&(b->roads), j);
          if( tiles[tests[i]]->played && (tiles[tests[i]] == r->begin) && (posi[tests[i]] == r->begin_road_place || r->begin_road_place == LAST_POS)){
              if (!meeple__first(r, ROAD)){
                i = 666;
              }
              j = n;
          }
          if( tiles[tests[i]]->played && (tiles[tests[i]] == r->end) && (posi[tests[i]] == r->end_road_place || r->end_road_place == LAST_POS)){
              if (!meeple__first(r, ROAD)){
                i = 666;
              }
              j = n;
          }
          j++;
        }
        i++;
      }
      if (i == nb_tests){
        m->check = VALID;
        return 1004;
      } else {
        return 11;
      }
    }

    //verification meeple is a knight or a farmer
    struct tile_zone_info links[4];
    int nb_links = 0;
    tile__fill_zones_info(t, links, &nb_links);
    //recuperation of the concerned zone
    struct tile_zone_info* meeple_zone;
    meeple_zone = NULL;
    for(int l=0; l<nb_links; l++){
      if(links[l].has_meeple != -1){
        meeple_zone = &links[l];
      }
    }
    //declaration of the zone's biome
    struct list *ref_list = NULL;
    if (t->follower_type == FARMER){
      ref_list = &(b->ref_farms);
    }
    if (t->follower_type == KNIGHT){
      ref_list = &(b->ref_cities);
    }

    int defined_ids[NB_IDS] = {NO_ID, NO_ID, NO_ID, NO_ID, NO_ID, NO_ID, NO_ID, NO_ID};
    int n = 0;
    for( int i = 0 ; i < meeple_zone->nb_ids ; i++) {
      int id_value = *(meeple_zone->ids[i]);
      if (id_value != NO_ID && id__not_in(ref_list, defined_ids, n, id_value) ){
          defined_ids[n] = id_value;
          n++;
      }
    }
    //case of a new zone
    if (n == 0) {
      m->check = VALID;
      return 1003;
    }
    //case of an updating zone
    if ( n == 1 && meeple__first(ref__get(ref_list, defined_ids[0]), meeple_zone->biome)) {
      m->check = VALID;
      return 1002;
    }
    if (n >= 2){ // case of an updating zone with at least one merging
      int i = 0;
      while ( i < n && meeple__first(ref__get(ref_list, defined_ids[i]), meeple_zone->biome)){
        i++;
      }
      if( i == n ){
        m->check = VALID;
        return 1001;
      }
    }
      return 12;
}


int move__is_valid(struct board* b, struct move *m)
{
  if(move__is_valid_aux(b, m) >= 1000){
    return 1;
  } else {
    return 0;
  }
}
