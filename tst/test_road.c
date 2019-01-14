#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "interface.h"
#include "list.h"
#include "road.h"
#include "tile.h"


void test__init()
{
    printf("\t%s", __func__);

    struct position p = {0, 0};
    struct move m = {VALID, 0, CARD_ROAD_STRAIGHT_CITY, p, NORTH, POS_CENTER};
    struct tile t;
    tile__init(&t, &m);

    struct road *r = road__init(&t, 1);

    assert( r != NULL );
    assert( r->begin = &t );
    assert( r->end = &t );
    assert( !r->closed );
    assert( r->length == 1 );
    assert( r->thieves[0] == 1 );
    for (int i = 1 ; i < MAX_NB_PLAYERS ; i++) {
	assert( r->thieves[i] == 0 );
    }

    road__free(r);

    printf(BOLD(GREEN("\t\t\t\tPASSED"))"\n");
}


void test__add_tile()
{
    printf("\t%s", __func__);

    struct position p = {5, 5};
    struct move m = {VALID, 0, CARD_ROAD_STRAIGHT_CITY, p, NORTH, POS_CENTER};
    struct tile t;
    tile__init(&t, &m);

    struct position p2 = {5, 6};
    struct move m2 = {VALID, 1, CARD_JUNCTION_CITY, p2, NORTH, POS_NORTH};
    struct tile t2;
    tile__init(&t2, &m2);

    struct position p3 = {5, 4};
    struct move m3 = {VALID, 2, CARD_ROAD_TURN_LEFT_CITY, p3, SOUTH, NO_MEEPLE};
    struct tile t3;
    tile__init(&t3, &m3);

    struct road *r = road__init(&t, 0);

    road__add_tile(r, &t2, 0, 1);

    assert( r->begin == &t );
    assert( r->end == &t2 );

    assert( r->length == 2 );
    assert( r->thieves[0] == 0 );
    assert( r->thieves[1] == 1 );
    for (int i = 2 ; i < MAX_NB_PLAYERS ; i++) {
	assert( r->thieves[i] == 0 );
    }

    road__add_tile(r, &t3, 1, 0);

    assert( r->begin == &t3 );
    assert( r->end == &t2 );

    assert( r->length == 3 );
    assert( r->thieves[0] == 0 );
    assert( r->thieves[1] == 1 );
    for (int i = 2 ; i < MAX_NB_PLAYERS ; i++) {
	assert( r->thieves[i] == 0 );
    }

    road__free(r);

    printf(BOLD(GREEN("\t\t\t\tPASSED"))"\n");
}


void test__is_closed()
{
    printf("\t%s", __func__);

    struct position p = {5, 5};
    struct move m = {VALID, 0, CARD_ROAD_STRAIGHT_CITY, p, NORTH, POS_CENTER};
    struct tile t;
    tile__init(&t, &m);

    struct position p2 = {5, 6};
    struct move m2 = {VALID, 1, CARD_JUNCTION_CITY, p2, NORTH, POS_NORTH};
    struct tile t2;
    tile__init(&t2, &m2);

    struct position p3 = {5, 4};
    struct move m3 = {VALID, 2, CARD_ROAD_TURN_LEFT_CITY, p3, SOUTH, NO_MEEPLE};
    struct tile t3;
    tile__init(&t3, &m3);

    struct position p4 = {6, 4};
    struct move m4 = {VALID, 1, CARD_JUNCTION_FOUR, p4, SOUTH, NO_MEEPLE};
    struct tile t4;
    tile__init(&t4, &m4);

    struct road *r = road__init(&t, 0);
    assert( !road__is_closed(r) );

    road__add_tile(r, &t2, 0, 1);
    assert( !road__is_closed(r) );

    road__add_tile(r, &t3, 1, 0);
    assert( !road__is_closed(r) );

    road__add_tile(r, &t4, 1, 0);
    assert( road__is_closed(r) );

    road__free(r);

    printf(BOLD(GREEN("\t\t\t\tPASSED"))"\n");
}


void test__merge()
{
    printf("\t%s", __func__);

    struct position p = {5, 5};
    struct move m = {VALID, 0, CARD_ROAD_STRAIGHT_CITY, p, NORTH, POS_CENTER};
    struct tile t;
    tile__init(&t, &m);

    struct position p2 = {5, 3};
    struct move m2 = {VALID, 2, CARD_ROAD_TURN_LEFT_CITY, p2, NORTH, POS_NORTH};
    struct tile t2;
    tile__init(&t2, &m2);

    struct position p3 = {5, 4};
    struct move m3 = {VALID, 2, CARD_ROAD_STRAIGHT, p3, SOUTH, NO_MEEPLE};
    struct tile t3;
    tile__init(&t3, &m3);

    struct road *r = road__init(&t, 1);
    struct road *r2 = road__init(&t2, 1);

    road__add_tile(r, &t3, 0, 0);
    road__add_tile(r2, &t3, 0, 0);

    road__merge(r, r2);

    assert( r->begin == &t );
    assert( r->end == &t2 );

    assert( r->length == 3 );
    assert( r->thieves[0] == 1 );
    assert( r->thieves[1] == 0 );
    assert( r->thieves[2] == 1 );
    for (int i = 3 ; i < MAX_NB_PLAYERS ; i++) {
	assert( r->thieves[i] == 0 );
    }

    road__free(r);
    road__free(r2);

    printf(BOLD(GREEN("\t\t\t\tPASSED"))"\n");
}


void test__update_points()
{
    printf("\t%s", __func__);

    struct position p = {5, 5};
    struct move m = {VALID, 0, CARD_ROAD_STRAIGHT_CITY, p, NORTH, POS_CENTER};
    struct tile t;
    tile__init(&t, &m);

    struct position p2 = {5, 3};
    struct move m2 = {VALID, 2, CARD_ROAD_TURN_LEFT_CITY, p2, NORTH, POS_NORTH};
    struct tile t2;
    tile__init(&t2, &m2);

    struct position p3 = {5, 4};
    struct move m3 = {VALID, 2, CARD_ROAD_STRAIGHT, p3, SOUTH, NO_MEEPLE};
    struct tile t3;
    tile__init(&t3, &m3);

    struct road *r = road__init(&t, 1);
    struct road *r2 = road__init(&t2, 1);

    road__add_tile(r, &t3, 0, 0);
    road__add_tile(r2, &t3, 1, 0);

    road__merge(r, r2);

    int points[MAX_NB_PLAYERS] = {0};

    road__update_points(r, points);

    assert( points[0] == 3 );
    assert( points[1] == 0 );
    assert( points[2] == 3 );
    for (int i = 3 ; i < MAX_NB_PLAYERS ; i++) {
	assert( points[i] == 0 );
    }

    road__free(r);
    road__free(r2);

    printf(BOLD(GREEN("\t\t\tPASSED"))"\n");
}


void test__update()
{
    printf("\t%s", __func__);

    struct list roads;
    list__init(&roads, NULL, road__free);

    // First tile

    struct position p = {0, 0};
    struct move m = {VALID, -1, CARD_ROAD_STRAIGHT_CITY, p, NORTH, NO_MEEPLE};
    struct tile t;
    tile__init(&t, &m);
    struct tile north = {.played = 0};
    struct tile west = {.played = 0};
    struct tile south = {.played = 0};
    struct tile east = {.played = 0};

    struct tile *tab[5] = {&north, &west, &south, &east, &t};

    road__update(&roads, tab);

    assert( list__size(&roads) == 1 );
    struct road *r = list__get(&roads, 0);
    assert( r->begin == &t );
    assert( r->end == &t );
    assert( r->begin_road_place == LAST_POS );
    assert( r->end_road_place == LAST_POS );
    assert( r->length == 1 );
    assert( !road__is_closed(r) );
    for (int i = 0 ; i < MAX_NB_PLAYERS ; i++) {
	assert( r->thieves[i] == 0 );
    }

    t.played = 1;

    // Second tile

    struct position p2 = {-1, 0};
    struct move m2 = {VALID, 2, CARD_JUNCTION_CITY, p2, NORTH, POS_NORTH};
    struct tile t2;
    tile__init(&t2, &m2);

    tab[2] = &t;
    tab[4] = &t2;

    road__update(&roads, tab);

    assert( list__size(&roads) == 3 );
    assert( r->begin == &t2 );
    assert( r->end == &t );
    assert( r->begin_road_place == POS_SOUTH );
    assert( r->end_road_place == LAST_POS );
    assert( r->length == 2 );
    assert( !road__is_closed(r) );

    for (int i = 1 ; i < MAX_NB_PLAYERS ; i++) {
	assert( r->thieves[i] == 0 );
    }

    struct road *r2 = list__get(&roads, 1);
    assert( r2->begin == &t2 );
    assert( r2->end == &t2 );
    assert( r2->begin_road_place == POS_NORTH );
    assert( r2->end_road_place == POS_NORTH );
    assert( r2->length == 1 );
    assert( !road__is_closed(r2) );

    assert( r2->thieves[0] == 0 );
    assert( r2->thieves[1] == 0 );
    assert( r2->thieves[2] == 1 );
    for (int i = 3 ; i < MAX_NB_PLAYERS ; i++) {
	assert( r2->thieves[i] == 0 );
    }


    struct road *r3 = list__get(&roads, 2);
    assert( r3->begin == &t2 );
    assert( r3->end == &t2 );
    assert( r3->begin_road_place == POS_WEST );
    assert( r3->end_road_place == POS_WEST );
    assert( r3->length == 1 );
    assert( !road__is_closed(r3) );

    for (int i = 0 ; i < MAX_NB_PLAYERS ; i++) {
	assert( r3->thieves[i] == 0 );
    }

    t2.played = 1;

    // Third tile

    struct position p3 = {-1, -1};
    struct move m3 = {VALID, 0, CARD_ROAD_TURN, p3, SOUTH, POS_WEST};
    struct tile t3;
    tile__init(&t3, &m3);

    tab[2] = &south;
    tab[3] = &t2;
    tab[4] = &t3;

    road__update(&roads, tab);

    assert( list__size(&roads) == 3 );
    assert( r3->begin == &t3 );
    assert( r3->end == &t2 );
    assert( r3->begin_road_place == LAST_POS );
    assert( r3->end_road_place == POS_WEST );
    assert( r3->length == 2 );
    assert( !road__is_closed(r3) );

    assert( r3->thieves[0] == 1 );
    for (int i = 1 ; i < MAX_NB_PLAYERS ; i++) {
	assert( r3->thieves[i] == 0 );
    }

    t3.played = 1;

    // Fourth tile

    struct position p4 = {-2, 0};
    struct move m4 = {VALID, 1, CARD_ROAD_TURN, p4, NORTH, NO_MEEPLE};
    struct tile t4;
    tile__init(&t4, &m4);

    tab[0] = &north;
    tab[1] = &west;
    tab[2] = &t2;
    tab[3] = &east;
    tab[4] = &t4;

    road__update(&roads, tab);

    assert( list__size(&roads) == 3 );
    assert( r2->begin == &t4 );
    assert( r2->end == &t2 );
    assert( r2->begin_road_place == LAST_POS );
    assert( r2->end_road_place == POS_NORTH );
    assert( r2->length == 2 );
    assert( !road__is_closed(r2) );

    assert( r2->thieves[0] == 0 );
    assert( r2->thieves[1] == 0 );
    assert( r2->thieves[2] == 1 );
    for (int i = 3 ; i < MAX_NB_PLAYERS ; i++) {
	assert( r2->thieves[i] == 0 );
    }

    t4.played = 1;

    // Fifth tile

    struct position p5 = {-2, -1};
    struct move m5 = {VALID, 2, CARD_ROAD_TURN, p5, WEST, NO_MEEPLE};
    struct tile t5;
    tile__init(&t5, &m5);

    tab[2] = &t3;
    tab[3] = &t4;
    tab[4] = &t5;

    road__update(&roads, tab);

    assert( list__size(&roads) == 2 );
    assert( r2->begin == &t2 );
    assert( r2->end == &t2 );
    assert( r2->begin_road_place == POS_WEST );
    assert( r2->end_road_place == POS_NORTH );
    assert( r2->length == 4 );
    assert( road__is_closed(r2) );

    assert( r2->thieves[0] == 1 );
    assert( r2->thieves[1] == 0 );
    assert( r2->thieves[2] == 1 );
    for (int i = 3 ; i < MAX_NB_PLAYERS ; i++) {
	assert( r2->thieves[i] == 0 );
    }

    list__free(&roads);

    printf(BOLD(GREEN("\t\t\t\tPASSED"))"\n");
}


int main()
{
    printf("Testing road...\n");

    test__init();
    test__add_tile();
    test__is_closed();
    test__merge();
    test__update_points();
    test__update();

    return 0;
}
