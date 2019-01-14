#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "constants.h"
#include "interface.h"


void test__init()
{
    printf("\t%s", __func__);

    struct board board;

    board__init(&board);

    for (int i = 0 ; i < (CARDS_STACK_SIZE * 2) - 1 ; i++) {
	for (int j = 0 ; j < (CARDS_STACK_SIZE * 2) - 1 ; j++) {
	    assert( !board.field[i][j].played
		    || (i == CARDS_STACK_SIZE && j == CARDS_STACK_SIZE && board.field[i][j].played) );
	}
    }

    assert( list__size(&board.cities) == 1 );
    assert( list__size(&board.cloisters) == 0 );
    assert( list__size(&board.farms) == 2 );
    assert( list__size(&board.roads) == 1 );
    assert( list__size(&board.ref_cities) == 1 );
    assert( list__size(&board.ref_farms) == 2 );

    for (int i = 0 ; i < CARDS_STACK_SIZE ; i++) {
	assert( board.cards_stack[i] >= CARD_MONASTERY_ROAD
		&& board.cards_stack[i] < LAST_CARD );
    }
    assert( board.drawn_cards == 1 );
    assert( board.moves_nb == 1 );

    for (int i = 0 ; i < MAX_NB_PLAYERS ; i++) {
	assert( board.points[i] == 0 );
	assert( board.followers_left[i] == INITIAL_NB_FOLLOWERS );
    }

    board__free(&board);

    printf(BOLD(GREEN("\t\t\t\tPASSED"))"\n");
}


void test__add_tile()
{
    printf("\t%s", __func__);

    struct board board;

    board__init(&board);

    struct position p = {-1, 0};
    struct move m = {VALID, 0, CARD_JUNCTION_CITY, p, NORTH, POS_NORTH};

    board__add_tile(&board, &m);

    assert( tile__played(board__get_tile(&board, -1, 0)) );

    assert( list__size(&board.cities) == 2 );
    assert( list__size(&board.cloisters) == 0 );
    assert( list__size(&board.farms) == 3 );
    assert( list__size(&board.roads) == 3 );
    assert( list__size(&board.ref_cities) == 2 );
    assert( list__size(&board.ref_farms) == 3 );

    board__free(&board);

    printf(BOLD(GREEN("\t\t\t\tPASSED"))"\n");
}


void test__game_is_finished()
{
    printf("\t%s", __func__);

    struct board board;

    board__init(&board);
    assert( !board__game_is_finished(&board) );

    board.drawn_cards = CARDS_STACK_SIZE;
    assert( board__game_is_finished(&board) );

    board__free(&board);

    printf(BOLD(GREEN("\t\t\tPASSED"))"\n");
}


void test__draw_card()
{
    printf("\t%s", __func__);

    struct board board;

    board__init(&board);

    for (int i = 1 ; i < CARDS_STACK_SIZE ; i++) {
	const enum card_id drawn = board__draw_card(&board);
	assert( drawn >= CARD_MONASTERY_ROAD
		&& drawn < LAST_CARD );
	assert( board.drawn_cards == i + 1 );
    }

    assert( board__draw_card(&board) == LAST_CARD );
    assert( board.drawn_cards == CARDS_STACK_SIZE );

    board__free(&board);

    printf(BOLD(GREEN("\t\t\t\tPASSED"))"\n");
}


void test__add_move()
{
    printf("\t%s", __func__);

    struct board board;

    board__init(&board);

    struct position p = {0, 0};
    struct move m = {VALID, 0, CARD_JUNCTION_CITY, p, NORTH, POS_NORTH};

    for (int i = 1 ; i < CARDS_STACK_SIZE + 5; i++) {
	board__add_move(&board, m);
	assert( board.moves_nb == i + 1 );
	assert( memcmp(&m, &board.moves[i], sizeof(struct move)) == 0 );
    }

    board__free(&board);

    printf(BOLD(GREEN("\t\t\t\tPASSED"))"\n");
}


void test__update_points()
{
    printf("\t%s", __func__);

    struct board board;

    board__init(&board);

    struct position p = {-1, 0};
    struct move m = {VALID, 0, CARD_JUNCTION_CITY, p, NORTH, POS_WEST};

    board__add_tile(&board, &m);

    struct position p2 = {-2, 0};
    struct move m2 = {VALID, 1, CARD_ROAD_STRAIGHT, p2, NORTH, POS_CENTER};

    board__add_tile(&board, &m2);

    board__update_points(&board);

    assert( board.points[0] == 1 );
    assert( board.points[1] == 2 );
    assert( board.points[2] == 0 );
    assert( board.points[3] == 0 );
    assert( board.points[4] == 0 );

    board__free(&board);

    printf(BOLD(GREEN("\t\t\tPASSED"))"\n");
}


int main()
{
    printf("Testing board...\n");

    test__init();
    test__add_tile();
    test__game_is_finished();
    test__draw_card();
    test__add_move();
    test__update_points();

    return 0;
}
