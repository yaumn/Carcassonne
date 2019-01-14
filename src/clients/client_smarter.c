#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "city.h"
#include "cloister.h"
#include "farm.h"
#include "interface.h"
#include "list.h"
#include "move.h"
#include "road.h"


unsigned int player_id;
unsigned int nb_players;


void copy_city(struct city *dest, struct city *src)
{
    *dest = *src;
    dest->tiles.d = malloc(sizeof(void *) * src->tiles.capacity);
}


void copy_farm(struct farm *dest, struct farm *src)
{
    *dest = *src;
    dest->cities = malloc(sizeof(struct list));
    *dest->cities = *src->cities;
    dest->cities->d = malloc(sizeof(void *) * src->cities->capacity);
}


void copy_board(struct board *dest, const struct board *src)
{
    *dest = *src;
    
    if (list__size(&src->roads) > 0) {
        dest->roads.d = malloc(sizeof(void *) * src->roads.capacity);
    }
    if (list__size(&src->cities) > 0) {
        dest->cities.d = malloc(sizeof(void *) * src->cities.capacity);
    } 
    if (list__size(&src->farms) > 0) {
        dest->farms.d = malloc(sizeof(void *) * src->farms.capacity);
    }
    if (list__size(&src->cloisters) > 0) {
        dest->cloisters.d = malloc(sizeof(void *) * src->cloisters.capacity);
    }
    if (list__size(&src->ref_cities) > 0) {
        dest->ref_cities.d = malloc(sizeof(void *) * src->ref_cities.capacity);
    }
    if (list__size(&src->ref_farms) > 0) {
        dest->ref_farms.d = malloc(sizeof(void *) * src->ref_farms.capacity);
    }
    if (list__size(&src->edges) > 0) {
        dest->edges.d = malloc(sizeof(void *) * src->edges.capacity);
    }
    dest->moves = malloc(sizeof(struct move) * src->moves_nb);
    memcpy(dest->moves, src->moves, sizeof(struct move) * src->moves_nb);

    // Copy roads
    for (size_t i = 0 ; i < list__size(&src->roads) ; i++) {
	dest->roads.d[i] = malloc(sizeof(struct road));
        struct road *rdest = dest->roads.d[i];
        struct road *r = src->roads.d[i];
        *rdest = *r;
        struct position pos = r->begin->move->onto;
        rdest->begin = board__get_tile(dest, pos.x, pos.y);
        pos = r->end->move->onto;
        rdest->end = board__get_tile(dest, pos.x, pos.y);
    }

    // Copy cities
    for (size_t i = 0 ; i < list__size(&src->cities) ; i++) {
	dest->cities.d[i] = malloc(sizeof(struct city));
	struct city *c = src->cities.d[i];
        struct city *cdest = dest->cities.d[i];
	if (c->pointer == c) {
            copy_city(cdest, c);
            cdest->pointer = cdest;
            memcpy(cdest->tiles.d, c->tiles.d, sizeof(void *) * list__size(&c->tiles));
	}
    }

    for (size_t i = 0 ; i < list__size(&src->cities) ; i++) {
        struct city *c = src->cities.d[i];
        struct city *cdest = dest->cities.d[i];
	if (c->pointer != c) {
            copy_city(cdest, c);
            memcpy(cdest->tiles.d, c->tiles.d, sizeof(void *) * list__size(&c->tiles));
	    for (size_t j = 0 ; j < list__size(&src->cities) ; j++) {
		if (c->pointer == src->cities.d[j]) {
                    cdest->pointer = dest->cities.d[j];
		}
	    }
	}
    }

    // Copy farms
    for (size_t i = 0 ; i < list__size(&src->farms) ; i++) {
	dest->farms.d[i] = malloc(sizeof(struct farm));
	copy_farm(dest->farms.d[i], src->farms.d[i]);
        struct farm *f = src->farms.d[i];
	for (size_t j = 0 ; j < list__size(f->cities) ; j++) {
	    for (size_t k = 0 ; k < list__size(&src->cities) ; k++) {
		if (src->cities.d[k] == f->cities->d[j]) {
		    ((struct farm *)dest->farms.d[i])->cities->d[j] = dest->cities.d[k];
		}
	    }
	}
    }

    // Copy cloisters
    for (size_t i = 0 ; i < list__size(&src->cloisters) ; i++) {
        dest->cloisters.d[i] = malloc(sizeof(struct cloister));
        memcpy(dest->cloisters.d[i], src->cloisters.d[i], sizeof(struct cloister));
    }

    // Copy ref_cities
    for (size_t i = 0 ; i < list__size(&src->ref_cities) ; i++) {
        for (size_t j = 0 ; j < list__size(&src->cities) ; j++) {
            if (src->ref_cities.d[i] == src->cities.d[j]) {
                dest->ref_cities.d[i] = dest->cities.d[j];
            }
        }
    }

    // Copy ref_farms
    for (size_t i = 0 ; i < list__size(&src->ref_farms) ; i++) {
	for (size_t j = 0 ; j < list__size(&src->farms) ; j++) {
	    if (src->ref_farms.d[i] == src->farms.d[j]) {
		dest->ref_farms.d[i] = dest->farms.d[j];
	    }
	}
    }

    // Copy edges
    for (size_t i = 0 ; i < list__size(&src->edges) ; i++) {
	dest->edges.d[i] = malloc(sizeof(struct position));
	memcpy(dest->edges.d[i], src->edges.d[i], sizeof(struct position));
    }
}


static int has_follower_majority(int followers[], const int id)
{
    int biggest_followers_nb = followers[0];
    int majority_id = 0;
    for (int i = 1 ; i < MAX_NB_PLAYERS ; i++) {
	if (biggest_followers_nb < followers[i]) {
	    biggest_followers_nb = followers[i];
	    majority_id = i;
	} else if (biggest_followers_nb == followers[i] && i == id) {
	    majority_id = id;
	}
    }
    return majority_id;
}


char const * get_player_name()
{
    return "Smarter client";
}


void initialize(unsigned int id, unsigned int n_players)
{
    player_id = id;
    nb_players = n_players;
}


struct move play(enum card_id card_id, struct move const previous_moves[], size_t n_moves)
{
    struct list possible_moves;
    list__init(&possible_moves, move_copy, free);

    struct move m;
    m.player = player_id;
    m.card = card_id;

    struct list smarter_moves;
    list__init(&smarter_moves, move_copy, free);

    struct board board;
    board__init(&board);
    for (size_t i = 1 ; i < n_moves ; i++) {
        if (previous_moves[i].check == VALID) {
            board__add_tile(&board, &((struct move *) previous_moves)[i]);
            board__add_move(&board, previous_moves[i]);
	}
    }
    move__get_all_possible_moves(&board, &possible_moves, card_id, player_id);

    for (size_t i = 0 ; i < list__size(&possible_moves) ; i++) {
        // board2 is allocated dynamically so that the stack is not overflowed
        // Indeed, board is a big structure in terms of memory
        struct board *board2 = malloc(sizeof(struct board));
        copy_board(board2, &board);

        struct move *m = list__get(&possible_moves, i);
	// If a follower can be placed on a monastery, always do it
	if (m->card == CARD_MONASTERY_ROAD || m->card == CARD_MONASTERY_ALONE) {
            if ((m->place == POS_CENTER && board2->followers_left[player_id] > 0)
		/*|| board.followers_left[player_id] == 0*/) {
		list__push_back(&smarter_moves, m);
	    }
	}	

	// Remember which zones were closed or not
        const size_t cities_size = list__size(&board2->cities);
        const size_t cloisters_size = list__size(&board2->cloisters);
        const size_t roads_size = list__size(&board2->roads);

        int *closed_cities = malloc(sizeof(int) * cities_size);
        int *closed_cloisters = malloc(sizeof(int) * cloisters_size);
        int *closed_roads = malloc(sizeof(int) * roads_size);

	assert( closed_cities != NULL && closed_cloisters != NULL && closed_roads != NULL );

        for (size_t j = 0 ; j < cities_size ; j++) {
            closed_cities[j] = city__complete((struct city *)list__get(&board2->cities, j));
	}
        for (size_t j = 0 ; j < cloisters_size ; j++) {
            struct cloister *c = list__get(&board2->cloisters, j);
            cloister__update_points(board2, c);
            closed_cloisters[j] = cloister__is_finished(c);
        }
        for (size_t j = 0 ; j < roads_size ; j++) {
            closed_roads[j] = road__is_closed((struct road *)list__get(&board2->roads, j));
	}

        board__add_tile(board2, m);

        int road_merged_index = -1;
        // Two roads have been merged
        if (roads_size > list__size(&board2->roads)) {
            for (size_t j = 0 ; j < list__size(&board2->roads) ; j++) {
                struct road *r1 = (struct road *)board.roads.d[j];
                struct road *r2 = (struct road *)board2->roads.d[j];
                if (r1->begin->move != r2->begin->move || r1->end->move != r2->end->move) {
                    road_merged_index = j;
                }
            }
        }

        for (size_t j = 0 ; j < cities_size ; j++) {
            struct city *c = (struct city *)list__get(&board2->cities, j);
            if (closed_cities[j] != city__complete(c)) {
		if (has_follower_majority(c->knights, player_id)) {
		    list__push_back(&smarter_moves, m);
		}
	    }
	}
        for (size_t j = 0 ; j < cloisters_size ; j++) {
            struct cloister *c = list__get(&board2->cloisters, j);
            cloister__update_points(board2, c);
            if (closed_cloisters[j] != cloister__is_finished(c)) {
		if (cloister__get_id_player(c)) {
		    list__push_back(&smarter_moves, m);
		}
	    }
        }

        // If two roads have been merged, we check if the new road is beneficial to the player
        if (road_merged_index != -1) {
            struct road *r = (struct road *)list__get(&board2->roads, road_merged_index);
            if (has_follower_majority(r->thieves, player_id)) {
                list__push_back(&smarter_moves, m);
            }
        } else {
	    for (size_t j = 0 ; j < roads_size ; j++) {
		struct road *r = (struct road *)list__get(&board2->roads, j);
		if (closed_roads[j] != road__is_closed(r)) {
		    if (has_follower_majority(r->thieves, player_id)) {
			list__push_back(&smarter_moves, m);
		    }
		}
	    }
        }

        free(closed_cities);
        free(closed_roads);
        free(closed_cloisters);

        board__free(board2);
        free(board2);
        board__get_tile(&board, m->onto.x, m->onto.y)->played = 0;
    }

	
    // If no smarter move was found, go with random moves
    if (list__size(&smarter_moves) > 0) {
	m = *((struct move *)list__get(&smarter_moves, rand() % list__size(&smarter_moves)));
    } else {
	m = *((struct move *)list__get(&possible_moves, rand() % list__size(&possible_moves)));
    }


    list__free(&smarter_moves);
    list__free(&possible_moves);
    board__free(&board);

    m.onto.x += 72;
    m.onto.y += 72;
    return m;
}


void finalize()
{ }
