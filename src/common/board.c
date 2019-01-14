#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "city.h"
#include "cloister.h"
#include "farm.h"
#include "road.h"


void *position_copy(void *p)
{
    struct position *pos = p;
    struct position *new = malloc(sizeof(struct position));
    assert( new != NULL );
    memcpy(new, pos, sizeof(struct position));
    return new;
}


void update_edges(struct board *board)
{
  int above_free = 1;
  int below_free = 1;
  int left_free = 1;
  int right_free = 1;

  const struct move current_move = board->moves[board->moves_nb - 1];

  for (size_t j = 0 ; j < list__size(&board->edges) ; j++) {
    struct position * const p = list__get(&board->edges, j);
    if (p->x == current_move.onto.x
      && p->y == current_move.onto.y) {
      list__remove(&board->edges, j);
    } else if ((p->x == current_move.onto.x && p->y == current_move.onto.y - 1)
              || tile__played(board__get_tile(board, current_move.onto.x, current_move.onto.y - 1))) {
      above_free = 0;
    } else if ((p->x == current_move.onto.x
              && p->y == current_move.onto.y + 1)
              || tile__played(board__get_tile(board, current_move.onto.x, current_move.onto.y + 1))) {
      below_free = 0;
    } else if ((p->x == current_move.onto.x - 1
                && p->y == current_move.onto.y)
                || tile__played(board__get_tile(board, current_move.onto.x - 1, current_move.onto.y))) {
      left_free = 0;
    } else if ((p->x == current_move.onto.x + 1
              && p->y == current_move.onto.y)
              || tile__played(board__get_tile(board, current_move.onto.x + 1, current_move.onto.y))) {
      right_free = 0;
    }
  }

  if (above_free) {
    struct position pos = {current_move.onto.x, current_move.onto.y - 1};
    list__push_back(&board->edges, &pos);
  }
  if (below_free) {
    struct position pos = {current_move.onto.x, current_move.onto.y + 1};
    list__push_back(&board->edges, &pos);
  }
  if (left_free) {
    struct position pos = {current_move.onto.x - 1, current_move.onto.y};
    list__push_back(&board->edges, &pos);
  }
  if (right_free) {
    struct position pos = {current_move.onto.x + 1, current_move.onto.y};
    list__push_back(&board->edges, &pos);
  }
}


static void init_cards_stack(enum card_id cards_stack[])
{
    // Nb of each different cards (corresponding to the order of enum card_id in interface.h
    int cards_nb[DIFFERENT_CARDS_NB] = {2, 4, 1, 3, 5, 2,
					1, 3, 2, 3, 3, 3,
					2, 3, 2, 3, 1, 3,
					2, 1, 8, 9, 4, 1};

    int i = 1; // index of board->cards_stack
    int j = 0; // index of cards_nb
    cards_stack[0] = CARD_ROAD_STRAIGHT_CITY;
    while (i < CARDS_STACK_SIZE) {
	for (int k = 0 ; k < cards_nb[j] ; k++, i++) {
	    // CARD_MONASTERY_ROAD is the first of the enum card_id
	    cards_stack[i] = CARD_MONASTERY_ROAD + j;
	}
	j++;
    }

    // Shuffle the stack
    for (int i = 1 ; i < CARDS_STACK_SIZE ; i++) {
	const enum card_id tmp = cards_stack[i];
	const int random_index = (rand() % (CARDS_STACK_SIZE - 1)) + 1;
	cards_stack[i] = cards_stack[random_index];
	cards_stack[random_index] = tmp;
    }
}


/**
 * \brief Search for completed zones and give back followers if necessary
 *
 * Even if the zone is completed, the function does not give any point
 * as it will be done for all the zones at the end of the game.
 * Therefore the followers are NOT removed from their zones as it is needed
 * to compute the points but each cell of the followers_left array is increased consequently
 *
 * \param board : a pointer to the game board
 */
static void check_for_completed_zones(struct board *board)
{
    for (size_t i = 0 ; i < list__size(&board->cities) ; i++) {
	struct city *c = board->cities.d[i];
	if (city__complete(c)) {
        struct move *m = c->tiles.d[0];
        m->check = VALID;
	    for (int j = 0 ; j < MAX_NB_PLAYERS ; j++) {
        board->followers_left[j] += c->knights[j];
	    }
        city__update_points(c, board->points);
        for (int j = 0 ; j < MAX_NB_PLAYERS ; j++) {
            c->knights[j] = 0;
            }
	}
    }

    for (size_t i = 0 ; i < list__size(&board->cloisters) ; i++) {
	struct cloister *c = board->cloisters.d[i];
	cloister__update_points(board, c);
	if (cloister__is_finished(c) && c->id_player != NO_ID) {
	    board->followers_left[c->id_player]++;
	    cloister__assign_points(c, board->points);
	    c->id_player = NO_ID;
	}
    }

    for (size_t i = 0 ; i < list__size(&board->roads) ; i++) {
    struct road *r = board->roads.d[i];
	if (road__is_closed(r)) {
	    for (int j = 0 ; j < MAX_NB_PLAYERS ; j++) {
        board->followers_left[j] += r->thieves[j];
	    }
	    road__update_points(r, board->points);
        for (int j = 0 ; j < MAX_NB_PLAYERS ; j++) {
            r->thieves[j] = 0;
        }
	}
    }
}


void board__init(struct board *board)
{
    for (int i = 0 ; i < ((CARDS_STACK_SIZE) * 2) - 1 ; i++) {
	for (int j = 0 ; j < ((CARDS_STACK_SIZE) * 2) - 1 ; j++) {
	    board->field[i][j].played = 0;
	}
    }

    list__init(&board->cities, NULL, city__free);
    list__init(&board->cloisters, NULL, cloister__free);
    list__init(&board->farms, NULL, farm__free);
    list__init(&board->roads, NULL, road__free);
    list__init(&board->ref_cities, NULL, NULL);
    list__init(&board->ref_farms, NULL, NULL);
    list__init(&board->edges, position_copy, free);

    init_cards_stack(board->cards_stack);
    board->drawn_cards = 1;
    board->moves = malloc(sizeof(struct move) * CARDS_STACK_SIZE); // There will be at least CARDS_STACK_SIZE moves
    assert( board->moves != NULL );
    board->moves_nb = 0;

    for (int i = 0 ; i < MAX_NB_PLAYERS ; i++) {
	board->points[i] = 0;
	board->followers_left[i] = INITIAL_NB_FOLLOWERS;
    }

    struct move m = {VALID, NO_ID, CARD_ROAD_STRAIGHT_CITY, (struct position){0, 0}, NORTH, NO_MEEPLE};
    board__add_move(board, m);
    board__add_tile(board, &board->moves[board->moves_nb - 1]);
    update_edges(board);
}


void board__free(struct board *board)
{
    list__free(&board->cities);
    list__free(&board->cloisters);
    list__free(&board->farms);
    list__free(&board->roads);
    list__free(&board->ref_cities);
    list__free(&board->ref_farms);
    list__free(&board->edges);

    free(board->moves);
}


struct tile *board__get_tile(struct board *board, const int x, const int y)
{
    if (x + 72 >= 143 || x + 72 < 0 || y + 72 >= 143 || y + 72 < 0){
	return NULL;
    } else {
	return &(board->field[-x + CARDS_STACK_SIZE][-y + CARDS_STACK_SIZE]);
    }
}


void board__add_tile(struct board *board, struct move *move)
{
    struct tile * t = board__get_tile(board, move->onto.x, move->onto.y);
    struct tile * above = board__get_tile(board, move->onto.x - 1, move->onto.y);
    struct tile * left = board__get_tile(board, move->onto.x, move->onto.y - 1);
    struct tile * below = board__get_tile(board, move->onto.x + 1, move->onto.y);
    struct tile * right = board__get_tile(board, move->onto.x, move->onto.y + 1);
    tile__init(t, move);

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

    t->played = 1;

    struct tile_zone_info zone_info[MAX_NB_ZONES_PER_TILE];
    int nb_zones = 0;
    tile__fill_zones_info(t, zone_info, &nb_zones);
    if (t->north.biome == CITY || t->west.biome == CITY
            || t->south.biome == CITY || t->east.biome == CITY) {
            city__update(board, t);
    }
    for (int i = 0 ; i < nb_zones ; i++) {
        if (zone_info[i].biome == FARM) {
            farm__update(&board->farms, &board->ref_farms, &board->ref_cities, &zone_info[i]);
        }
    }

    cloisters__update(&board->cloisters, move);

    if (t->north.biome == ROAD || t->west.biome == ROAD
            || t->south.biome == ROAD || t->east.biome == ROAD) {
        struct tile *tiles[5] = {above, left, below, right, t};
        road__update(&board->roads, tiles);
    }

    if (move->place != NO_MEEPLE) {
        board->followers_left[move->player]--;
    }
    check_for_completed_zones(board);
}


int board__game_is_finished(const struct board *board)
{
    return board->drawn_cards == CARDS_STACK_SIZE;
}


enum card_id board__draw_card(struct board *board)
{
    if (board->drawn_cards == CARDS_STACK_SIZE) {
	return LAST_CARD;
    }
    board->drawn_cards++;
    return board->cards_stack[board->drawn_cards - 1];
}


void board__add_move(struct board *board, const struct move move)
{
    if (board->moves_nb >= CARDS_STACK_SIZE) {
	struct move * const tmp = realloc(board->moves, sizeof(struct move) * (board->moves_nb + 1));
	assert( tmp != NULL );
	board->moves = tmp;
    }
    board->moves[board->moves_nb] = move;
    board->moves_nb++;
    update_edges(board);
}


void board__update_points(struct board *board)
{
    for (size_t i = 0 ; i < list__size(&board->cities) ; i++) {
	struct city *c = board->cities.d[i];
    if (!city__complete(c) && c == city__pointer(c)) {
	    city__update_points(c, board->points);
	}
    }

    for (size_t i = 0 ; i < list__size(&board->cloisters) ; i++) {
	struct cloister *c = board->cloisters.d[i];
	if (!cloister__is_finished(c)) {
	    cloister__assign_points(c, board->points);
	}
    }

    farm__update_cities(&board->farms);
    for (size_t i = 0 ; i < list__size(&board->farms) ; i++) {
        farm__update_points((struct farm *)board->farms.d[i], board->points);
    }

    for (size_t i = 0 ; i < list__size(&board->roads) ; i++) {
	struct road *r = board->roads.d[i];
	if (!road__is_closed(r)) {
	    road__update_points(r, board->points);
	}
    }
}
