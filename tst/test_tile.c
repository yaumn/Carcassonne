#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "interface.h"
#include "tile.h"


static int contains(int *t[], const int n, int *v)
{
    for (int i = 0 ; i < n ; i++) {
	if (t[i] == v) {
	    return 1;
	}
    }
    return 0;
}


void test__init()
{
    printf("\t%s", __func__);

    struct position p = {0, 0};
    struct move m = {VALID, 0, CARD_JUNCTION_CITY, p, NORTH, POS_NORTH};
    struct tile t;
    tile__init(&t, &m);

    assert( t.north.biome == ROAD );
    assert( t.south.biome == ROAD );
    assert( t.west.biome == ROAD );
    assert( t.east.biome == CITY );

    assert( t.north.id1 == NO_ID );
    assert( t.north.id2 == NO_ID );
    assert( t.south.id1 == NO_ID );
    assert( t.south.id2 == NO_ID );
    assert( t.west.id1 == NO_ID );
    assert( t.west.id2 == NO_ID );
    assert( t.east.id1 == NO_ID );
    assert( t.east.id2 == NO_ID );

    assert( t.follower_type == THIEF );
    assert( t.move == &m );
    assert( !t.played );

    struct position p2 = {1, 1};
    struct move m2 = {VALID, 0, CARD_ROAD_TURN_RIGHT_CITY, p2, NORTH, POS_NORTH};
    struct tile t2;
    tile__init(&t2, &m2);

    assert( t2.north.biome == CITY );
    assert( t2.south.biome == ROAD );
    assert( t2.west.biome == FARM );
    assert( t2.east.biome == ROAD );

    assert( t2.north.id1 == NO_ID );
    assert( t2.north.id2 == NO_ID );
    assert( t2.south.id1 == NO_ID );
    assert( t2.south.id2 == NO_ID );
    assert( t2.west.id1 == NO_ID );
    assert( t2.west.id2 == NO_ID );
    assert( t2.east.id1 == NO_ID );
    assert( t2.east.id2 == NO_ID );

    assert( t2.follower_type == KNIGHT );
    assert( t2.move == &m2 );
    assert( !t2.played );

    struct position p3 = {2, 1};
    struct move m3 = {VALID, 0, CARD_CITY_TUNNEL_SHLD, p3, NORTH, POS_NORTH};
    struct tile t3;
    tile__init(&t3, &m3);

    assert( t3.north.biome == FARM );
    assert( t3.south.biome == FARM );
    assert( t3.west.biome == CITY );
    assert( t3.east.biome == CITY );

    assert( t3.north.id1 == NO_ID );
    assert( t3.north.id2 == NO_ID );
    assert( t3.south.id1 == NO_ID );
    assert( t3.south.id2 == NO_ID );
    assert( t3.west.id1 == NO_ID );
    assert( t3.west.id2 == NO_ID );
    assert( t3.east.id1 == NO_ID );
    assert( t3.east.id2 == NO_ID );

    assert( t3.follower_type == FARMER );
    assert( t3.move == &m3 );
    assert( !t3.played );

    struct position p4 = {2, 2};
    struct move m4 = {VALID, 0, CARD_MONASTERY_ROAD, p4, NORTH, POS_CENTER};
    struct tile t4;
    tile__init(&t4, &m4);

    assert( t4.north.biome == FARM );
    assert( t4.south.biome == ROAD );
    assert( t4.west.biome == FARM );
    assert( t4.east.biome == FARM );

    assert( t4.north.id1 == NO_ID );
    assert( t4.north.id2 == NO_ID );
    assert( t4.south.id1 == NO_ID );
    assert( t4.south.id2 == NO_ID );
    assert( t4.west.id1 == NO_ID );
    assert( t4.west.id2 == NO_ID );
    assert( t4.east.id1 == NO_ID );
    assert( t4.east.id2 == NO_ID );

    assert( t4.follower_type == MONK );
    assert( t4.move == &m4 );
    assert( !t4.played );

    struct position p5 = {2, 3};
    struct move m5 = {VALID, 0, CARD_CITY_THREE_ROAD_SHLD, p5, EAST, NO_MEEPLE};
    struct tile t5;
    tile__init(&t5, &m5);

    assert( t5.north.biome == CITY );
    assert( t5.south.biome == CITY );
    assert( t5.west.biome == ROAD );
    assert( t5.east.biome == CITY );

    assert( t5.north.id1 == NO_ID );
    assert( t5.north.id2 == NO_ID );
    assert( t5.south.id1 == NO_ID );
    assert( t5.south.id2 == NO_ID );
    assert( t5.west.id1 == NO_ID );
    assert( t5.west.id2 == NO_ID );
    assert( t5.east.id1 == NO_ID );
    assert( t5.east.id2 == NO_ID );

    assert( t5.follower_type == LAST_FOLLOWER_TYPE );
    assert( t5.move == &m5 );
    assert( !t5.played );

    printf(BOLD(GREEN("\t\t\t\tPASSED"))"\n");
}


void test__fill_zone_info()
{
    printf("\t%s", __func__);

    struct position p = {0, 0};
    struct move m = {VALID, 0, CARD_PLAIN_CITY_ROAD, p, NORTH, POS_NORTH};
    struct tile t;
    tile__init(&t, &m);

    t.north.id1 = 0;
    t.north.id2 = 0;
    t.south.id1 = 1;
    t.south.id2 = 2;
    t.west.id1 = 0;
    t.west.id2 = 0;
    t.east.id1 = 1;
    t.east.id2 = 2;

    struct tile_zone_info zone_info[4];
    int nb_zone_info = 0;

    tile__fill_zones_info(&t, zone_info, &nb_zone_info);

    assert( nb_zone_info == 3 );

    int zone_city = -1;
    int zone_farm1 = -1; // farm next to city
    int zone_farm2 = -1; // the other farm

    for (int i = 0 ; i < 3 ; i++) {
	assert( (zone_info[i].biome == CITY && zone_city == -1)
		|| (zone_info[i].biome == FARM && (zone_farm1 == -1 || zone_farm2 == -1)) );
	if (zone_info[i].biome == CITY) {
	    assert( zone_info[i].nb_ids == 4 );
	    assert( contains(zone_info[i].ids, 4, &t.north.id1) && contains(zone_info[i].ids, 4, &t.north.id2)
		    && contains(zone_info[i].ids, 4, &t.west.id1) && contains(zone_info[i].ids, 4, &t.west.id2) );
	    zone_city = 1;
	    assert( zone_info[i].has_meeple == 0 );
	} else {
	    assert( zone_info[i].nb_ids == 2);
	    if (contains(zone_info[i].ids, 2, &t.east.id2) || contains(zone_info[i].ids, 2, &t.south.id1)) {
		assert( contains(zone_info[i].ids, 2, &t.east.id2)
			&& contains(zone_info[i].ids, 2, &t.south.id1) );
		zone_farm1 = 1;
		assert( zone_info[i].has_meeple == NO_ID );
	    } else {
		assert( contains(zone_info[i].ids, 2, &t.east.id1)
			&& contains(zone_info[i].ids, 2, &t.south.id2) );
		zone_farm2 = 1;
		assert( zone_info[i].has_meeple == NO_ID );
	    }
	}
    }

    struct position p2 = {1, 0};
    struct move m2 = {VALID, 0, CARD_CITY_THREE_SHLD, p2, WEST, NO_MEEPLE};
    struct tile t2;
    tile__init(&t2, &m2);

    struct tile_zone_info zone_info2[4];

    tile__fill_zones_info(&t2, zone_info2, &nb_zone_info);
    assert( nb_zone_info == 2 );

    zone_city = -1;
    zone_farm1 = -1;

    for (int i = 0 ; i < nb_zone_info ; i++) {
	assert( (zone_info2[i].biome == CITY && zone_city == -1)
		|| (zone_info2[i].biome == FARM && zone_farm1 == -1) );
	if (zone_info2[i].biome == CITY) {
	    assert( zone_info2[i].nb_ids == 6 );
	    assert( contains(zone_info2[i].ids, 6, &t2.north.id1) && contains(zone_info2[i].ids, 6, &t2.north.id2)
		    && contains(zone_info2[i].ids, 6, &t2.west.id1) && contains(zone_info2[i].ids, 6, &t2.west.id2)
		    && contains(zone_info2[i].ids, 6, &t2.south.id1) && contains(zone_info2[i].ids, 6, &t2.south.id2) );
	    zone_city = 1;
	    assert( zone_info2[i].has_meeple == NO_ID );
	} else {
	    assert( zone_info2[i].nb_ids == 2);
	    assert( contains(zone_info2[i].ids, 2, &t2.east.id1) && contains(zone_info2[i].ids, 2, &t2.east.id2) );
	    zone_farm1 = 1;
	    assert( zone_info2[i].has_meeple == NO_ID );
	}
    }

    struct position p3 = {1, 1};
    struct move m3 = {VALID, 0, CARD_JUNCTION_FOUR, p3, NORTH, POS_NORTH_EAST};
    struct tile t3;
    tile__init(&t3, &m3);

    struct tile_zone_info zone_info3[4];

    tile__fill_zones_info(&t3, zone_info3, &nb_zone_info);
    assert( nb_zone_info == 4 );

    zone_farm1 = -1;
    zone_farm2 = -1;
    int zone_farm3 = -1;
    int zone_farm4 = -1;

    for (int i = 0 ; i < nb_zone_info ; i++) {
	assert( zone_info3[i].biome == FARM );
	assert( zone_info3[i].nb_ids == 2 );
	if (contains(zone_info3[i].ids, 2, &t3.north.id2) || contains(zone_info3[i].ids, 2, &t3.west.id1)) {
	    zone_farm1 = 1;
	    assert( contains(zone_info3[i].ids, 2, &t3.north.id2) && contains(zone_info3[i].ids, 2, &t3.west.id1) );
	} else if (contains(zone_info3[i].ids, 2, &t3.north.id1) || contains(zone_info3[i].ids, 2, &t3.east.id2)) {
	    zone_farm2 = 1;
	    assert( contains(zone_info3[i].ids, 2, &t3.north.id1) && contains(zone_info3[i].ids, 2, &t3.east.id2) );
	} else if (contains(zone_info3[i].ids, 2, &t3.west.id2) || contains(zone_info3[i].ids, 2, &t3.south.id1)) {
	    zone_farm3 = 1;
	    assert( contains(zone_info3[i].ids, 2, &t3.west.id2) && contains(zone_info3[i].ids, 2, &t3.south.id1) );
	} else if (contains(zone_info3[i].ids, 2, &t3.east.id1) || contains(zone_info3[i].ids, 2, &t3.south.id2)) {
	    zone_farm4 = 1;
	    assert( contains(zone_info3[i].ids, 2, &t3.east.id1) && contains(zone_info3[i].ids, 2, &t3.south.id2) );
	} else {
	    assert( 0 );
	}
    }

    assert( zone_farm1 && zone_farm2 && zone_farm3 && zone_farm4 );

    printf(BOLD(GREEN("\t\t\tPASSED"))"\n");
}


int main()
{
    printf("Testing tile...\n");

    test__init();
    test__fill_zone_info();

    return 0;
}
