#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "interface.h"
#include "list.h"
#include "move.h"


unsigned int player_id;
unsigned int nb_players;


char const * get_player_name()
{
    return "Random client";
}


void initialize(unsigned int id, unsigned int n_players)
{
    player_id = id;
    nb_players = n_players;
}


struct move play(enum card_id card_id, struct move const previous_moves[], size_t n_moves)
{
    struct board board;
    board__init(&board);

    for (size_t i = 1 ; i < n_moves ; i++) {
         if (previous_moves[i].check == VALID) {
	     board__add_tile(&board, &((struct move *) previous_moves)[i]);
	     board__add_move(&board, previous_moves[i]);
         }
    }

    struct list possible_moves;
    list__init(&possible_moves, move_copy, free);

    struct move m;
    m.player = player_id;
    m.card = card_id;

    move__get_all_possible_moves(&board, &possible_moves, card_id, player_id);

    // Choose randomly
    m = *((struct move *)list__get(&possible_moves, rand() % list__size(&possible_moves)));

    board__free(&board);
    list__free(&possible_moves);

    m.onto.x += 72;
    m.onto.y += 72;
    return m;
}


void finalize()
{ }
