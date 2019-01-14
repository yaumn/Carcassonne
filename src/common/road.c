#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "road.h"
#include "tile.h"


/**
 * \brief Converts a direction into a place
 * That is to say if dir = NORTH, it will return POS_NORTH
 */
static enum place direction_to_place(const enum direction dir)
{
    return POS_NORTH + 3 * dir;
}


static int adjacent(const enum place p1, const enum place p2)
{
    return abs(p1 - p2) == 6;
}


static enum place place_rotation(const enum place p, const enum direction dir)
{
    enum place new = p + (3 * dir);
    return (new % ((new > 12) ? 12 : 13));
}


static int tile_is_next_to_road(const struct road *road, const struct tile *tile, const enum direction dir)
{
    return (road->begin == tile && (road->begin_road_place == LAST_POS
				    || adjacent(direction_to_place(dir), road->begin_road_place)))
	|| (road->end == tile && (road->end_road_place == LAST_POS
				  || adjacent(direction_to_place(dir), road->end_road_place)));
}


/**
 * \brief This function returns whether a follower must be added to a road when it's added a tile
 * If the added tile is a single-road tile, then the follower is always. Things gets more complicated
 * when the added tile is a multi-road tile because if the follower is not on the road connecting
 * the added tile and the struct road, then the follower must not be added.
 *
 * \param road_place : if the road end is a multi-road tile, it indicates which road it represents.
 *                     if the road is a single-road tile, then it is equal to LAST_POS
 * \param place_of_road_in_relation_to_added_tile : indicates where the road end is in relation to
 *                                                  the added tile
 * \param follower_place : the place of the follower on the added tile
 * \param nb_edges_connected_by_road : indicates how many roads the added tile connects (i.e. if it
 *                                    is a single or a multi-road tile)
 */
static int add_follower(const enum place road_place, const enum direction direction_in_relation_to_added_tile,
			const enum place follower_place, const int nb_edges_connected_by_road)
{
    return nb_edges_connected_by_road > 2 ?
	(road_place != LAST_POS && adjacent(follower_place, road_place))
	|| (road_place == LAST_POS && direction_to_place(direction_in_relation_to_added_tile) == follower_place)
	: 1;
}


static int closes_road(const struct tile *tile) 
{
    return tile->move->card == CARD_MONASTERY_ROAD
            || tile->move->card == CARD_JUNCTION_CITY
            || tile->move->card == CARD_CITY_THREE_ROAD_SHLD
            || tile->move->card == CARD_CITY_THREE_ROAD
            || tile->move->card == CARD_JUNCTION_THREE
            || tile->move->card == CARD_JUNCTION_FOUR;

}


struct road *road__init(struct tile *tile, const int add_follower)
{
    struct road *road = malloc(sizeof(struct road));
    assert( road != NULL && tile != NULL);

    road->begin = tile;
    road->end = tile;
    road->length = 1;
    road->closed = 0;
    for (int i = 0 ; i < MAX_NB_PLAYERS ; i++) {
        road->thieves[i] = 0;
    }
    
    if (add_follower && tile->follower_type == THIEF && tile->move->player < MAX_NB_PLAYERS
            && tile->move->place > NO_MEEPLE && tile->move->place < LAST_POS) {
        road->thieves[tile->move->player] = 1;
    }

    return road;
}


void road__free(void *road)
{
    free(road);
}


int road__is_closed(const struct road *road)
{
    return road->closed;
}


void road__add_tile(struct road *road, struct tile *tile, const int add_at_beginning, const int add_follower)
{
    if (add_at_beginning) {
        road->begin = tile;
    } else {
        road->end = tile;
    }

    road->length++;

    road->closed = road->length > 1 && ((closes_road(road->begin) && closes_road(road->end))
            || road->begin == road->end);

    if (add_follower && tile->follower_type == THIEF && tile->move->player < MAX_NB_PLAYERS
            && tile->move->place > NO_MEEPLE && tile->move->place < LAST_POS) {
        road->thieves[tile->move->player]++;
    }
}


void road__update_points(const struct road *road, int points[])
{
    int greatest_thieves_nb = 0;
    
    for (int i = 0 ; i < MAX_NB_PLAYERS ; i++) {
        if (road->thieves[i] > greatest_thieves_nb) {
            greatest_thieves_nb = road->thieves[i];
        }
    }

    if (greatest_thieves_nb == 0) {
	return;
    }

    for (int i = 0 ; i < MAX_NB_PLAYERS ; i++) {
        if (road->thieves[i] == greatest_thieves_nb) {
            points[i] += road->length;
        }
    }
}


void road__merge(struct road *road1, struct road *road2)
{
    struct tile *merging_tile = NULL;
    if (road1->begin == road2->begin) {
	merging_tile = road1->begin;
        road1->begin = road2->end;
        road1->begin_road_place = road2->end_road_place;
    } else if (road1->begin == road2->end) {
	merging_tile = road1->begin;
        road1->begin = road2->begin;
        road1->begin_road_place = road2->begin_road_place;
    } else if (road1->end == road2->begin) {
	merging_tile = road1->end;
        road1->end = road2->end;
        road1->end_road_place = road2->end_road_place;
    } else if (road1->end == road2->end) {
	merging_tile = road1->end;
        road1->end = road2->begin;
        road1->end_road_place = road2->begin_road_place;
    } else {
        return;
    }

    road1->length += road2->length;
    road1->length -= (road1->begin == road1->end) ? 2 : 1;

    road1->closed = (closes_road(road1->begin) && closes_road(road1->end))
            || (road1->begin == road1->end && road1->length > 1);

    // Merge the lists of followers
    for (int i = 0 ; i < MAX_NB_PLAYERS ; i++) {
        road1->thieves[i] += road2->thieves[i];
    }
    if (merging_tile->move->place != NO_MEEPLE && merging_tile->follower_type == THIEF) {
	road1->thieves[merging_tile->move->player]--;
    }
}


void road__update(struct list *roads, struct tile *tiles[])
{
    // Find which sides the road connects
    const int connects[4] = { tiles[4]->north.biome == ROAD,
                              tiles[4]->west.biome == ROAD,
                              tiles[4]->south.biome == ROAD,
                              tiles[4]->east.biome == ROAD };

    const int nb_edges_connected_by_road = (tiles[4]->north.biome == ROAD)
            + (tiles[4]->west.biome == ROAD)
            + (tiles[4]->south.biome == ROAD)
            + (tiles[4]->east.biome == ROAD);

    const enum place rotated_follower_place = place_rotation(tiles[4]->move->place,
							     tiles[4]->move->dir);
    
    // Maybe create (a) new road(s)
    int nb_roads_created = 0;
    if (nb_edges_connected_by_road > 2) {
        for (unsigned int i = 0 ; i < 4 ; i++) {
            if (connects[i] && !tile__played(tiles[i])) {
                struct road *new = road__init(tiles[4], rotated_follower_place == direction_to_place(i));
                new->begin_road_place = direction_to_place(i);
                new->end_road_place = new->begin_road_place;
                list__push_back(roads, new);
		nb_roads_created++;
            }
        }
    } else {
        int create_new_road = 1;
        for (int i = 0 ; i < 4 ; i++) {
            if (connects[i] && tile__played(tiles[i])) {
                create_new_road = 0;
            }
        }
        if (create_new_road) {
            struct road *new = road__init(tiles[4], tiles[4]->follower_type == THIEF);
            new->begin_road_place = LAST_POS;
            new->end_road_place = LAST_POS;
            list__push_back(roads, new);
            return;
        }
    }
    
    // Loop through the roads to find which tiles the new tile is adjacent to
    int nb_of_extended_roads = 0;
    int roads_to_merge_id[2] = {-1, -1}; // With one tile, only two roads can be merged
    for (size_t i = 0 ; i < list__size(roads) - nb_roads_created ; i++) {
        struct road *r = list__get(roads, i);
        if (road__is_closed(r)) {
            continue;
        }

        int nb_ends_extended = 0 ; // Used to know if the road was extended by both ends (i.e. it is now closed)
        for (int j = 0 ; j < 4 ; j++) {
            if (connects[j] && tile__played(tiles[j]) && tile_is_next_to_road(r, tiles[j], j)) {           
		if (nb_ends_extended == 0) {
		    if (r->begin == tiles[j]) {
			road__add_tile(r, tiles[4], 1, add_follower(r->begin_road_place, j, rotated_follower_place,
								    nb_edges_connected_by_road));
			r->begin_road_place = nb_edges_connected_by_road > 2 ? direction_to_place(j) : LAST_POS;
		    } else {
			road__add_tile(r, tiles[4], 0, add_follower(r->end_road_place, j, rotated_follower_place,
								    nb_edges_connected_by_road));
			r->end_road_place = nb_edges_connected_by_road > 2 ? direction_to_place(j) : LAST_POS;
		    }

		    // Only merge roads if the new tile is a tile with only one road
		    if (nb_edges_connected_by_road <= 2) {
			nb_of_extended_roads++;
			roads_to_merge_id[nb_of_extended_roads - 1] = i;
		    }
		    nb_ends_extended++;
		} else {
		    if (r->begin == tiles[4]) {
			r->end = tiles[4];
		    } else {
			r->begin = tiles[4];
		    }
		    r->closed = 1;
		}
	    }
        }
    }

    // If two roads must be merged
    if (roads_to_merge_id[1] != -1) {
        road__merge(list__get(roads, roads_to_merge_id[0]),
		    list__get(roads, roads_to_merge_id[1]));
        list__remove(roads, roads_to_merge_id[1]);
    }
}
