#include "constants.h"
#include "tile.h"


void rotation(struct tile *tile, int direction, enum biome biomes[])
{
    tile->north.biome = biomes[(0 - direction + 4) % 4];
    tile->west.biome = biomes[(1 - direction + 4) % 4];
    tile->south.biome = biomes[(2 - direction + 4) % 4];
    tile->east.biome = biomes[(3 - direction + 4) % 4];
}


int get_id(enum direction cardinal_pt, int num_id, enum direction shift)
{
    return ((cardinal_pt + shift) % 4) * 2 + (num_id - 1);
}


char *tile__card_to_str(const enum card_id c)
{
    switch (c) {
	case CARD_MONASTERY_ROAD:
	    return "Monastery road";
	case CARD_MONASTERY_ALONE:
	    return "Monastery alone";
	case CARD_CITY_ALL_SIDES:
	    return "City all sides";
	case CARD_ROAD_STRAIGHT_CITY:
	    return "Road straight city";
	case CARD_CITY_ONE_SIDE:
	    return "City one side";
	case CARD_CITY_TUNNEL_SHLD:
	    return "City tunnel shield";
	case CARD_CITY_TUNNEL:
	    return "City tunnel";
	case CARD_PLAIN_TUNNEL:
	    return "Plain tunnel";
	case CARD_PLAIN_TWO_CITIES:
	    return "Plain two cities";
	case CARD_ROAD_TURN_RIGHT_CITY:
	    return "Road turn right city";
	case CARD_ROAD_TURN_LEFT_CITY:
	    return "Road turn left city";
	case CARD_JUNCTION_CITY:
	    return "Junction city";
	case CARD_PLAIN_CITY_SHLD:
	    return "Plain city shield";
	case CARD_PLAIN_CITY:
	    return "Plain city";
	case CARD_PLAIN_CITY_ROAD_SHLD:
	    return "Plain city road shield";
	case CARD_PLAIN_CITY_ROAD:
	    return "Plain city road";
	case CARD_CITY_THREE_SHLD:
	    return "City three shield";
	case CARD_CITY_THREE:
	    return "City three";
	case CARD_CITY_THREE_ROAD_SHLD:
	    return "City three road shield";
	case CARD_CITY_THREE_ROAD:
	    return "City three road";
	case CARD_ROAD_STRAIGHT:
	    return "Road straight";
	case CARD_ROAD_TURN:
	    return "Road turn";
	case CARD_JUNCTION_THREE:
	    return "Junction three";
	case CARD_JUNCTION_FOUR:
	    return "Junction four";
	default:
	    return "Invalid card";
    }
}


char *tile__meeple_place_to_str(const enum place p)
{
    switch (p) {
	case NO_MEEPLE:
	    return "No meeple";
	case POS_NORTH_EAST:
	    return "North east";
	case POS_NORTH:
	    return "North";
	case POS_NORTH_WEST:
	    return "North west";
	case POS_WEST_NORTH:
	    return "West north";
	case POS_WEST:
	    return "West";
	case POS_WEST_SOUTH:
	    return "West south";
	case POS_SOUTH_WEST:
	    return "South west";
	case POS_SOUTH:
	    return "South";
	case POS_SOUTH_EAST:
	    return "South east";
	case POS_EAST_SOUTH:
	    return "East south";
	case POS_EAST:
	    return "East";
	case POS_EAST_NORTH:
	    return "East north";
	case POS_CENTER:
	    return "Center";
	default:
	    return "";
    }
}


char *tile__direction_to_str(const enum direction d)
{
    switch (d) {
	case NORTH:
	    return "North";
	case WEST:
	    return "West";
	case SOUTH:
	    return "South";
	case EAST:
	    return "East";
	default:
	    return "";
    }
}


char *tile__biome_to_str(const enum biome b)
{
    switch (b) {
	case FARM:
	    return "Farm";
	case ROAD:
	    return "Road";
	case CITY:
	    return "City";
	default:
	    return "";
    }
}


void tile__init(struct tile *tile, struct move * const move)
{
    tile->move = move;
    tile->played = 0;
    tile->follower_type = LAST_FOLLOWER_TYPE;
    tile->north.id1 = NO_ID;
    tile->north.id2 = NO_ID;
    tile->south.id1 = NO_ID;
    tile->south.id2 = NO_ID;
    tile->east.id1 = NO_ID;
    tile->east.id2 = NO_ID;
    tile->west.id1 = NO_ID;
    tile->west.id2 = NO_ID;


    enum biome biomes[4] = {LAST_BIOME, LAST_BIOME, LAST_BIOME, LAST_BIOME};
    //biomes represents the biome from respectively NORTH, WEST, SOUTH, EAST
    switch (move->card) {
	case CARD_MONASTERY_ROAD:
	    biomes[0] = FARM; //NORTH
	    biomes[1] = FARM; //WEST
	    biomes[2] = ROAD; //SOUTH
	    biomes[3] = FARM; //EAST
	    rotation(tile, move->dir, biomes);


	    if (move->place == POS_NORTH) {
		tile->follower_type = FARMER;
	    } else if (move->place == POS_SOUTH) {
		tile->follower_type = THIEF;
	    } else if (move->place == POS_CENTER) {
		tile->follower_type = MONK;
     	    }
	    break;
	case CARD_MONASTERY_ALONE:
	    biomes[0] = FARM; //NORTH
	    biomes[1] = FARM; //WEST
	    biomes[2] = FARM; //SOUTH
	    biomes[3] = FARM; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_NORTH) {
		tile->follower_type = FARMER;
	    } else if (move->place == POS_CENTER) {
		tile->follower_type = MONK;
	    }
	    break;
	case CARD_CITY_ALL_SIDES:
	    biomes[0] = CITY; //NORTH
	    biomes[1] = CITY; //WEST
	    biomes[2] = CITY; //SOUTH
	    biomes[3] = CITY; //EAST
	    rotation(tile, move->dir, biomes);
	    tile->follower_type = KNIGHT;//to delete ?

	    if (move->place == POS_CENTER) {
		tile->follower_type = KNIGHT;
	    }
	    break;
	case CARD_ROAD_STRAIGHT_CITY:
	    biomes[0] = ROAD; //NORTH
	    biomes[1] = FARM; //WEST
	    biomes[2] = ROAD; //SOUTH
	    biomes[3] = CITY; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_WEST || move->place == POS_NORTH_EAST) {
		tile->follower_type = FARMER;
	    } else if (move->place == POS_EAST) {
		tile->follower_type = KNIGHT;
	    } else if (move->place == POS_CENTER) {
		tile->follower_type = THIEF;
	    }
	    break;
	case CARD_CITY_ONE_SIDE:
	    biomes[0] = CITY; //NORTH
	    biomes[1] = FARM; //WEST
	    biomes[2] = FARM; //SOUTH
	    biomes[3] = FARM; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_NORTH) {
		tile->follower_type = KNIGHT;
	    } else if (move->place == POS_CENTER) {
		tile->follower_type = FARMER;
	    }
	    break;
	case CARD_CITY_TUNNEL_SHLD:
	    biomes[0] = FARM; //NORTH
	    biomes[1] = CITY; //WEST
	    biomes[2] = FARM; //SOUTH
	    biomes[3] = CITY; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_NORTH || move->place == POS_SOUTH) {
		tile->follower_type = FARMER;
	    } else if (move->place == POS_CENTER) {
		tile->follower_type = KNIGHT;
	    }
	    break;
	case CARD_CITY_TUNNEL:
	    biomes[0] = CITY; //NORTH
	    biomes[1] = FARM; //WEST
	    biomes[2] = CITY; //SOUTH
	    biomes[3] = FARM; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_WEST || move->place == POS_EAST) {
		tile->follower_type = FARMER;
	    } else if (move->place == POS_CENTER) {
		tile->follower_type = KNIGHT;
	    }
	    break;
	case CARD_PLAIN_TUNNEL:
	    biomes[0] = FARM; //NORTH
	    biomes[1] = CITY; //WEST
	    biomes[2] = FARM; //SOUTH
	    biomes[3] = CITY; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_WEST || move->place == POS_EAST) {
		tile->follower_type = KNIGHT;
	    } else if (move->place == POS_CENTER) {
		tile->follower_type = FARMER;
	    }
	    break;
	case CARD_PLAIN_TWO_CITIES:
	    biomes[0] = FARM; //NORTH
	    biomes[1] = FARM; //WEST
	    biomes[2] = CITY; //SOUTH
	    biomes[3] = CITY; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_SOUTH || move->place == POS_EAST) {
		tile->follower_type = KNIGHT;
	    } else if (move->place == POS_CENTER) {
		tile->follower_type = FARMER;
	    }
	    break;
	case CARD_ROAD_TURN_RIGHT_CITY:
	    biomes[0] = CITY; //NORTH
	    biomes[1] = FARM; //WEST
	    biomes[2] = ROAD; //SOUTH
	    biomes[3] = ROAD; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_NORTH) {
		tile->follower_type = KNIGHT;
	    } else if (move->place == POS_WEST || move->place == POS_SOUTH_EAST) {
		tile->follower_type = FARMER;
	    } else if (move->place == POS_SOUTH) {
		tile->follower_type = THIEF;
	    }
	    break;
	case CARD_ROAD_TURN_LEFT_CITY:
	    biomes[0] = ROAD; //NORTH
	    biomes[1] = ROAD; //WEST
	    biomes[2] = FARM; //SOUTH
	    biomes[3] = CITY; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_NORTH_WEST || move->place == POS_SOUTH) {
		tile->follower_type = FARMER;
	    } else if (move->place == POS_EAST) {
		tile->follower_type = KNIGHT;
	    } else if (move->place == POS_NORTH) {
		tile->follower_type = THIEF;
	    }
	    break;
	case CARD_JUNCTION_CITY:
	    biomes[0] = ROAD; //NORTH
	    biomes[1] = ROAD; //WEST
	    biomes[2] = ROAD; //SOUTH
	    biomes[3] = CITY; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_NORTH || move->place == POS_WEST || move->place == POS_SOUTH) {
		tile->follower_type = THIEF;
	    } else if (move->place == POS_NORTH_WEST || move->place == POS_SOUTH_WEST || move->place == POS_NORTH_EAST) {
		tile->follower_type = FARMER;
	    } else if (move->place == POS_EAST) {
		tile->follower_type = KNIGHT;
	    }
	    break;
	case CARD_PLAIN_CITY_SHLD:
	    biomes[0] = CITY; //NORTH
	    biomes[1] = CITY; //WEST
	    biomes[2] = FARM; //SOUTH
	    biomes[3] = FARM; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_NORTH) {
		tile->follower_type = KNIGHT;
	    } else if (move->place == POS_SOUTH) {
		tile->follower_type = FARMER;
	    }
	    break;
	case CARD_PLAIN_CITY:
	    biomes[0] = CITY; //NORTH
	    biomes[1] = CITY; //WEST
	    biomes[2] = FARM; //SOUTH
	    biomes[3] = FARM; //EAST
	    rotation(tile, move->dir, biomes);


	    if (move->place == POS_NORTH) {
		tile->follower_type = KNIGHT;
	    } else if (move->place == POS_SOUTH) {
		tile->follower_type = FARMER;
	    }
	    break;
	case CARD_PLAIN_CITY_ROAD_SHLD:
	    biomes[0] = CITY; //NORTH
	    biomes[1] = CITY; //WEST
	    biomes[2] = ROAD; //SOUTH
	    biomes[3] = ROAD; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_NORTH) {
		tile->follower_type = KNIGHT;
	    } else if (move->place == POS_SOUTH) {
		tile->follower_type = THIEF;
	    } else if (move->place == POS_SOUTH_WEST || move->place == POS_SOUTH_EAST) {
		tile->follower_type = FARMER;
	    }
	    break;
	case CARD_PLAIN_CITY_ROAD:
	    biomes[0] = CITY; //NORTH
	    biomes[1] = CITY; //WEST
	    biomes[2] = ROAD; //SOUTH
	    biomes[3] = ROAD; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_NORTH) {
		tile->follower_type = KNIGHT;
	    } else if (move->place == POS_SOUTH) {
		tile->follower_type = THIEF;
	    } else if (move->place == POS_SOUTH_WEST || move->place == POS_SOUTH_EAST) {
		tile->follower_type = FARMER;
	    }
	    break;
	case CARD_CITY_THREE_SHLD:
	    biomes[0] = CITY; //NORTH
	    biomes[1] = CITY; //WEST
	    biomes[2] = FARM; //SOUTH
	    biomes[3] = CITY; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_SOUTH) {
		tile->follower_type = FARMER;
	    } else if (move->place == POS_CENTER) {
		tile->follower_type = KNIGHT;
	    }
	    break;
	case CARD_CITY_THREE:
	    biomes[0] = CITY; //NORTH
	    biomes[1] = CITY; //WEST
	    biomes[2] = FARM; //SOUTH
	    biomes[3] = CITY; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_SOUTH) {
		tile->follower_type = FARMER;
	    } else if (move->place == POS_CENTER) {
		tile->follower_type = KNIGHT;
	    }
	    break;
	case CARD_CITY_THREE_ROAD_SHLD:
	    biomes[0] = CITY; //NORTH
	    biomes[1] = CITY; //WEST
	    biomes[2] = ROAD; //SOUTH
	    biomes[3] = CITY; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_SOUTH) {
		tile->follower_type = THIEF;
	    } else if (move->place == POS_SOUTH_WEST || move->place == POS_SOUTH_EAST) {
		tile->follower_type = FARMER;
	    } else if (move->place == POS_CENTER) {
		tile->follower_type = KNIGHT;
	    }
	    break;
	case CARD_CITY_THREE_ROAD:
	    biomes[0] = CITY; //NORTH
	    biomes[1] = CITY; //WEST
	    biomes[2] = ROAD; //SOUTH
	    biomes[3] = CITY; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_SOUTH) {
		tile->follower_type = THIEF;
	    } else if (move->place == POS_SOUTH_WEST || move->place == POS_SOUTH_EAST) {
		tile->follower_type = FARMER;
	    } else if (move->place == POS_CENTER) {
		tile->follower_type = KNIGHT;
	    }
	    break;
	case CARD_ROAD_STRAIGHT:
	    biomes[0] = ROAD; //NORTH
	    biomes[1] = FARM; //WEST
	    biomes[2] = ROAD; //SOUTH
	    biomes[3] = FARM; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_WEST || move->place == POS_EAST) {
		tile->follower_type = FARMER;
	    } else if (move->place == POS_CENTER) {
		tile->follower_type = THIEF;
	    }
	    break;
	case CARD_ROAD_TURN:
	    biomes[0] = FARM; //NORTH
	    biomes[1] = ROAD; //WEST
	    biomes[2] = ROAD; //SOUTH
	    biomes[3] = FARM; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_NORTH_EAST || move->place == POS_SOUTH_WEST) {
		tile->follower_type = FARMER;
	    } else if (move->place == POS_WEST) {
		tile->follower_type = THIEF;
	    }
	    break;
	case CARD_JUNCTION_THREE:
	    biomes[0] = FARM; //NORTH
	    biomes[1] = ROAD; //WEST
	    biomes[2] = ROAD; //SOUTH
	    biomes[3] = ROAD; //EAST
	    rotation(tile, move->dir, biomes);

	    if (move->place == POS_NORTH || move->place == POS_SOUTH_WEST || move->place == POS_SOUTH_EAST) {
		tile->follower_type = FARMER;
	    } else if (move->place == POS_WEST || move->place == POS_SOUTH || move->place == POS_EAST) {
		tile->follower_type = THIEF;
	    }
	    break;
	case CARD_JUNCTION_FOUR:
	    biomes[0] = ROAD; //NORTH
	    biomes[1] = ROAD; //WEST
	    biomes[2] = ROAD; //SOUTH
	    biomes[3] = ROAD; //EAST
	    rotation(tile, move->dir, biomes);

        if (move->place == POS_NORTH_WEST || move->place == POS_NORTH_EAST
		|| move->place == POS_SOUTH_EAST || move->place == POS_SOUTH_WEST) {
		tile->follower_type = FARMER;
	    } else if (move->place == POS_NORTH || move->place == POS_WEST
		       || move->place == POS_SOUTH || move->place == POS_EAST) {
		tile->follower_type = THIEF;
	    }
	    break;
	default:
	    break;
    }
}


void tile__fill_zones_info(struct tile *tile, struct tile_zone_info links[], int *nb_links)
{
    int *oriented_ids[NB_IDS];
    oriented_ids[0] = &(tile->north.id1);
    oriented_ids[1] = &(tile->north.id2);
    oriented_ids[2] = &(tile->west.id1);
    oriented_ids[3] = &(tile->west.id2);
    oriented_ids[4] = &(tile->south.id1);
    oriented_ids[5] = &(tile->south.id2);
    oriented_ids[6] = &(tile->east.id1);
    oriented_ids[7] = &(tile->east.id2);

    for(int i = 0 ; i < 4 ; i++){
	links[i].has_meeple = NO_ID;
    }

    switch (tile->move->card) {
	case CARD_MONASTERY_ROAD:
	    *nb_links = 1;
	    links[0].biome = FARM;
	    links[0].nb_ids = 8;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    for(int i = 0 ; i < links[0].nb_ids ; i++){
		links[0].ids[i] = oriented_ids[i];
	    }
	    if(tile->move->place == POS_NORTH){
		links[0].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_MONASTERY_ALONE:
	    *nb_links = 1;
	    links[0].biome = FARM;
	    links[0].nb_ids = 8;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    for(int i = 0 ; i < links[0].nb_ids ; i++){
		links[0].ids[i] = oriented_ids[i];
	    }
	    if(tile->move->place == POS_NORTH){
		links[0].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_CITY_ALL_SIDES:
	    *nb_links = 1;
	    links[0].biome = CITY;
	    links[0].nb_ids = 8;
	    for(int i = 0 ; i < links[0].nb_ids ; i++){
		links[0].ids[i] = oriented_ids[i];
	    }
	    if(tile->move->place == POS_CENTER){
		links[0].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_ROAD_STRAIGHT_CITY:
	    *nb_links = 3;
	    // biome 1
        links[0].biome = CITY;
	    links[0].nb_ids = 2;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(EAST, 1, tile->move->dir)]; //east 1
	    links[0].ids[1] = oriented_ids[get_id(EAST, 2, tile->move->dir)]; //east 2
	    if(tile->move->place == POS_EAST){
		links[0].has_meeple = tile->move->player;
	    }
	    // biome 2
	    links[1].biome = FARM;
	    links[1].nb_ids = 4;
	    links[1].city1 = NULL;
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(NORTH, 2, tile->move->dir)]; //north 2
	    links[1].ids[1] = oriented_ids[get_id(WEST, 1, tile->move->dir)]; //west 1
	    links[1].ids[2] = oriented_ids[get_id(WEST, 2, tile->move->dir)]; //west 2
	    links[1].ids[3] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)]; //south 1
	    if(tile->move->place == POS_WEST){
		links[1].has_meeple = tile->move->player;
	    }
	    // biome 3
	    links[2].biome = FARM;
	    links[2].nb_ids = 2;
	    links[2].city1 = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    links[2].city2 = NULL;
	    links[2].ids[0] = oriented_ids[get_id(NORTH, 1, tile->move->dir)]; //north 1
	    links[2].ids[1] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)]; //south 2
	    if(tile->move->place == POS_NORTH_EAST){
		links[2].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_CITY_ONE_SIDE:
	    *nb_links = 2;
	    // biome 1 CITY
	    links[0].biome = CITY;
	    links[0].nb_ids = 2;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(NORTH, 1, tile->move->dir)]; //north 1
	    links[0].ids[1] = oriented_ids[get_id(NORTH, 2, tile->move->dir)]; //north 2
	    if(tile->move->place == POS_NORTH){
		links[0].has_meeple = tile->move->player;
	    }
	    // biome 2 FARM
	    links[1].biome = FARM;
	    links[1].nb_ids = 6;
	    links[1].city1 = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)]; //west 1
	    links[1].ids[1] = oriented_ids[get_id(WEST, 2, tile->move->dir)]; //west 2
	    links[1].ids[2] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)]; //south 1
	    links[1].ids[3] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)]; //south 2
	    links[1].ids[4] = oriented_ids[get_id(EAST, 1, tile->move->dir)]; //east 1
	    links[1].ids[5] = oriented_ids[get_id(EAST, 2, tile->move->dir)]; //east 2
	    if(tile->move->place == POS_CENTER){
		links[1].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_CITY_TUNNEL_SHLD:
	    *nb_links = 3;
    // biome 1 CITY
    links[0].biome = CITY;
    links[0].nb_ids = 4;
    links[0].city1 = NULL;
    links[0].city2 = NULL;
    links[0].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)]; //west 1
    links[0].ids[1] = oriented_ids[get_id(WEST, 2, tile->move->dir)]; //west 2
    links[0].ids[2] = oriented_ids[get_id(EAST, 1, tile->move->dir)]; //east 1
    links[0].ids[3] = oriented_ids[get_id(EAST, 2, tile->move->dir)]; //east 2
    if(tile->move->place == POS_CENTER){
      links[0].has_meeple = tile->move->player;
    }
	    // biome 1 FARM
	    links[1].biome = FARM;
	    links[1].nb_ids = 2;
	    links[1].city1 = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(NORTH, 1, tile->move->dir)]; //north 1
	    links[1].ids[1] = oriented_ids[get_id(NORTH, 2, tile->move->dir)]; //north 2
	    if(tile->move->place == POS_NORTH){
        links[1].has_meeple = tile->move->player;
  }
	    // biom 3 FARM
	    links[2].biome = FARM;
	    links[2].nb_ids = 2;
	    links[2].city1 = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    links[2].city2 = NULL;
	    links[2].ids[0] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)]; //south 1
	    links[2].ids[1] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)]; //south 2
	    if(tile->move->place == POS_SOUTH){
		links[2].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_CITY_TUNNEL:
	    *nb_links = 3;
      // biome 2 CITY
      links[0].biome = CITY;
      links[0].nb_ids = 4;
      links[0].city1 = NULL;
      links[0].city2 = NULL;
      links[0].ids[0] = oriented_ids[get_id(NORTH, 1, tile->move->dir)]; //north 1
      links[0].ids[1] = oriented_ids[get_id(NORTH, 2, tile->move->dir)]; //north 2
      links[0].ids[2] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)]; //south 1
      links[0].ids[3] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)]; //south 2
      if(tile->move->place == POS_CENTER){
        links[0].has_meeple = tile->move->player;
      }
	    // biome 1 FARM
	    links[1].biome = FARM;
	    links[1].nb_ids = 2;
	    links[1].city1 = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)]; //west 1
	    links[1].ids[1] = oriented_ids[get_id(WEST, 2, tile->move->dir)]; //west 2
	    if(tile->move->place == POS_WEST){
		links[1].has_meeple = tile->move->player;
	    }
	    // biom 3 FARM
	    links[2].biome = FARM;
	    links[2].nb_ids = 2;
	    links[2].city1 = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[2].city2 = NULL;
	    links[2].ids[0] = oriented_ids[get_id(EAST, 1, tile->move->dir)]; //EAST 1
	    links[2].ids[1] = oriented_ids[get_id(EAST, 2, tile->move->dir)]; //EAST 2
	    if(tile->move->place == POS_EAST){
		links[2].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_PLAIN_TUNNEL:
	    *nb_links = 3;
	    // biome 1 CITY
	    links[0].biome = CITY;
	    links[0].nb_ids = 2;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)]; //west 1
	    links[0].ids[1] = oriented_ids[get_id(WEST, 2, tile->move->dir)]; //west 2
	    if(tile->move->place == POS_WEST){
		links[0].has_meeple = tile->move->player;
	    }
      // biom 3 CITY
      links[1].biome = CITY;
      links[1].nb_ids = 2;
      links[1].city1 = NULL;
      links[1].city2 = NULL;
      links[1].ids[0] = oriented_ids[get_id(EAST, 1, tile->move->dir)]; //EAST 1
      links[1].ids[1] = oriented_ids[get_id(EAST, 2, tile->move->dir)]; //EAST 2
      if(tile->move->place == POS_EAST){
        links[1].has_meeple = tile->move->player;
      }
	    // biome 2 FARM
	    links[2].biome = FARM;
	    links[2].nb_ids = 4;
	    links[2].city1 = oriented_ids[get_id(WEST, 1, tile->move->dir)];
	    links[2].city2 =oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    links[2].ids[0] = oriented_ids[get_id(NORTH, 1, tile->move->dir)]; //north 1
	    links[2].ids[1] = oriented_ids[get_id(NORTH, 2, tile->move->dir)]; //north 2
	    links[2].ids[2] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)]; //south 1
	    links[2].ids[3] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)]; //south 2
	    if(tile->move->place == POS_CENTER){
		links[2].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_PLAIN_TWO_CITIES:
	    *nb_links = 3;
	    // biome 1 CITY
	    links[0].biome = CITY;
	    links[0].nb_ids = 2;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)]; //south 1
	    links[0].ids[1] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)]; //south 2
	    if(tile->move->place == POS_SOUTH){
		links[0].has_meeple = tile->move->player;
	    }
      // biom 3 CITY
      links[1].biome = CITY;
      links[1].nb_ids = 2;
      links[1].city1 = NULL;
      links[1].city2 = NULL;
      links[1].ids[0] = oriented_ids[get_id(EAST, 1, tile->move->dir)]; //EAST 1
      links[1].ids[1] = oriented_ids[get_id(EAST, 2, tile->move->dir)]; //EAST 2
      if(tile->move->place == POS_EAST){
        links[1].has_meeple = tile->move->player;
      }
	    // biome 2 FARM
	    links[2].biome = FARM;
	    links[2].nb_ids = 4;
	    links[2].city1 =  oriented_ids[get_id(SOUTH, 1, tile->move->dir)];
	    links[2].city2 =  oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    links[2].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)]; //north 1
	    links[2].ids[1] = oriented_ids[get_id(WEST, 2, tile->move->dir)]; //north 2
	    links[2].ids[2] = oriented_ids[get_id(NORTH, 1, tile->move->dir)]; //south 1
	    links[2].ids[3] = oriented_ids[get_id(NORTH, 2, tile->move->dir)]; //south 2
	    if(tile->move->place == POS_CENTER){
		links[2].has_meeple = tile->move->player;
  }
	    break;
	case CARD_ROAD_TURN_RIGHT_CITY:
	    *nb_links = 3;
	    // biome 1 CITY north
	    links[0].biome = CITY;
	    links[0].nb_ids = 2;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(NORTH, 1, tile->move->dir)]; //north 1
	    links[0].ids[1] = oriented_ids[get_id(NORTH, 2, tile->move->dir)]; //north 2
	    if(tile->move->place == POS_NORTH){
		links[0].has_meeple = tile->move->player;
	    }
	    // biome 2 FARM next to city
	    links[1].biome = FARM;
	    links[1].nb_ids = 4;
	    links[1].city1 =  oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)]; //west 1
	    links[1].ids[1] = oriented_ids[get_id(WEST, 2, tile->move->dir)]; //west 2
	    links[1].ids[2] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)]; //south 1
	    links[1].ids[3] = oriented_ids[get_id(EAST, 2, tile->move->dir)]; //east 2
	    if(tile->move->place == POS_WEST){
		links[1].has_meeple = tile->move->player;
	    }
	    // biom 3 FARM
	    links[2].biome = FARM;
	    links[2].nb_ids = 2;
	    links[2].city1 = NULL;
	    links[2].city2 = NULL;
	    links[2].ids[0] = oriented_ids[get_id(EAST, 1, tile->move->dir)]; //east 1
	    links[2].ids[1] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)]; //south 2
	    if(tile->move->place == POS_SOUTH_EAST){
		links[2].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_ROAD_TURN_LEFT_CITY:
	    *nb_links = 3;
	    // biome 1 CITY
	    links[0].biome = CITY;
	    links[0].nb_ids = 2;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    links[0].ids[1] = oriented_ids[get_id(EAST, 2, tile->move->dir)];
	    if(tile->move->place == POS_EAST){
		links[0].has_meeple = tile->move->player;
	    }
	    // biome 2 FARM next to city
	    links[1].biome = FARM;
	    links[1].nb_ids = 4;
	    links[1].city1 = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[1].ids[1] = oriented_ids[get_id(WEST, 2, tile->move->dir)];
	    links[1].ids[2] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)];
	    links[1].ids[3] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH){
		links[1].has_meeple = tile->move->player;
	    }
	    // biom 3 FARM
	    links[2].biome = FARM;
	    links[2].nb_ids = 2;
	    links[2].city1 = NULL;
	    links[2].city2 = NULL;
	    links[2].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)];
	    links[2].ids[1] = oriented_ids[get_id(NORTH, 2, tile->move->dir)];
	    if(tile->move->place == POS_NORTH_WEST){
		links[2].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_JUNCTION_CITY:
	    *nb_links = 4;
	    // biome 1 CITY
	    links[0].biome = CITY;
	    links[0].nb_ids = 2;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    links[0].ids[1] = oriented_ids[get_id(EAST, 2, tile->move->dir)];
	    if(tile->move->place == POS_EAST){
		links[0].has_meeple = tile->move->player;
	    }
	    // biome 2 FARM next to city
	    links[1].biome = FARM;
	    links[1].nb_ids = 2;
	    links[1].city1 = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[1].ids[1] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)];
	    if(tile->move->place == POS_NORTH_EAST){
		links[1].has_meeple = tile->move->player;
	    }
	    // biom 3 FARM
	    links[2].biome = FARM;
	    links[2].nb_ids = 2;
	    links[2].city1 = NULL;
	    links[2].city2 = NULL;
	    links[2].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)];
	    links[2].ids[1] = oriented_ids[get_id(NORTH, 2, tile->move->dir)];
	    if(tile->move->place == POS_NORTH_WEST){
		links[2].has_meeple = tile->move->player;
	    }
	    // biom 4 FARM
	    links[3].biome = FARM;
	    links[3].nb_ids = 2;
	    links[3].city1 = NULL;
	    links[3].city2 = NULL;
	    links[3].ids[0] = oriented_ids[get_id(WEST, 2, tile->move->dir)];
	    links[3].ids[1] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH_WEST){
		links[3].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_PLAIN_CITY_SHLD:
	    *nb_links = 2;
	    // biome 1 CITY
	    links[0].biome = CITY;
	    links[0].nb_ids = 4;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)];
	    links[0].ids[1] = oriented_ids[get_id(WEST, 2, tile->move->dir)];
	    links[0].ids[2] = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[0].ids[3] = oriented_ids[get_id(NORTH, 2, tile->move->dir)];
	    if(tile->move->place == POS_NORTH){
		links[0].has_meeple = tile->move->player;
	    }
	    // biome 2 FARM next to city
	    links[1].biome = FARM;
	    links[1].nb_ids = 4;
	    links[1].city1 = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(EAST, 2, tile->move->dir)];
	    links[1].ids[1] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)];
	    links[1].ids[2] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)];
	    links[1].ids[3] = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH){
		links[1].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_PLAIN_CITY:
	    *nb_links = 2;
	    // biome 1 CITY
	    links[0].biome = CITY;
	    links[0].nb_ids = 4;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)];
	    links[0].ids[1] = oriented_ids[get_id(WEST, 2, tile->move->dir)];
	    links[0].ids[2] = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[0].ids[3] = oriented_ids[get_id(NORTH, 2, tile->move->dir)];
	    if(tile->move->place == POS_NORTH){
		links[0].has_meeple = tile->move->player;
	    }
	    // biome 2 FARM next to city
	    links[1].biome = FARM;
	    links[1].nb_ids = 4;
	    links[1].city1 = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(EAST, 2, tile->move->dir)];
	    links[1].ids[1] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)];
	    links[1].ids[2] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)];
	    links[1].ids[3] = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH){
		links[1].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_PLAIN_CITY_ROAD_SHLD:
	    *nb_links = 3;
	    // biome 1 CITY
	    links[0].biome = CITY;
	    links[0].nb_ids = 4;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)];
	    links[0].ids[1] = oriented_ids[get_id(WEST, 2, tile->move->dir)];
	    links[0].ids[2] = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[0].ids[3] = oriented_ids[get_id(NORTH, 2, tile->move->dir)];
	    if(tile->move->place == POS_NORTH){
		links[0].has_meeple = tile->move->player;
	    }
	    // biome 2 FARM next to city
	    links[1].biome = FARM;
	    links[1].nb_ids = 2;
	    links[1].city1 = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(EAST, 2, tile->move->dir)];
	    links[1].ids[1] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH_WEST){
		links[1].has_meeple = tile->move->player;
	    }
	    // biome 3 FARM
	    links[2].biome = FARM;
	    links[2].nb_ids = 2;
	    links[2].city1 = NULL;
	    links[2].city2 = NULL;
	    links[2].ids[0] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)];
	    links[2].ids[1] = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH_EAST){
		links[2].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_PLAIN_CITY_ROAD:
	    *nb_links = 3;
	    // biome 1 CITY
	    links[0].biome = CITY;
	    links[0].nb_ids = 4;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)];
	    links[0].ids[1] = oriented_ids[get_id(WEST, 2, tile->move->dir)];
	    links[0].ids[2] = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[0].ids[3] = oriented_ids[get_id(NORTH, 2, tile->move->dir)];
	    if(tile->move->place == POS_NORTH){
		links[0].has_meeple = tile->move->player;
	    }
	    // biome 2 FARM next to city
	    links[1].biome = FARM;
	    links[1].nb_ids = 2;
	    links[1].city1 = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(EAST, 2, tile->move->dir)];
	    links[1].ids[1] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH_WEST){
		links[1].has_meeple = tile->move->player;
	    }
	    // biome 3 FARM
	    links[2].biome = FARM;
	    links[2].nb_ids = 2;
	    links[2].city1 = NULL;
	    links[2].city2 = NULL;
	    links[2].ids[0] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)];
	    links[2].ids[1] = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH_EAST){
		links[2].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_CITY_THREE_SHLD:
	    *nb_links = 2;
	    // biome 1 CITY
	    links[0].biome = CITY;
	    links[0].nb_ids = 6;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)];
	    links[0].ids[1] = oriented_ids[get_id(WEST, 2, tile->move->dir)];
	    links[0].ids[2] = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[0].ids[3] = oriented_ids[get_id(NORTH, 2, tile->move->dir)];
	    links[0].ids[4] = oriented_ids[get_id(EAST, 2, tile->move->dir)];
	    links[0].ids[5] = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    if(tile->move->place == POS_CENTER){
		links[0].has_meeple = tile->move->player;
	    }
	    // biome 2 FARM next to city
	    links[1].biome = FARM;
	    links[1].nb_ids = 2;
	    links[1].city1 = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)];
	    links[1].ids[1] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH){
		links[1].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_CITY_THREE:
	    *nb_links = 2;
	    // biome 1 CITY
	    links[0].biome = CITY;
	    links[0].nb_ids = 6;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)];
	    links[0].ids[1] = oriented_ids[get_id(WEST, 2, tile->move->dir)];
	    links[0].ids[2] = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[0].ids[3] = oriented_ids[get_id(NORTH, 2, tile->move->dir)];
	    links[0].ids[4] = oriented_ids[get_id(EAST, 2, tile->move->dir)];
	    links[0].ids[5] = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    if(tile->move->place == POS_CENTER){
		links[0].has_meeple = tile->move->player;
	    }
	    // biome 2 FARM next to city
	    links[1].biome = FARM;
	    links[1].nb_ids = 2;
	    links[1].city1 = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)];
	    links[1].ids[1] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH){
		links[1].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_CITY_THREE_ROAD_SHLD:
	    *nb_links = 3;
	    // biome 1 CITY
	    links[0].biome = CITY;
	    links[0].nb_ids = 6;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)];
	    links[0].ids[1] = oriented_ids[get_id(WEST, 2, tile->move->dir)];
	    links[0].ids[2] = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[0].ids[3] = oriented_ids[get_id(NORTH, 2, tile->move->dir)];
	    links[0].ids[4] = oriented_ids[get_id(EAST, 2, tile->move->dir)];
	    links[0].ids[5] = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    if(tile->move->place == POS_CENTER){
		links[0].has_meeple = tile->move->player;
	    }
	    // biome 2 FARM next to city
	    links[1].biome = FARM;
	    links[1].nb_ids = 1;
	    links[1].city1 = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH_EAST){
		links[1].has_meeple = tile->move->player;
	    }
	    // biom 3 FARM next to city
	    links[2].biome = FARM;
	    links[2].nb_ids = 1;
	    links[2].city1 = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[2].city2 = NULL;
	    links[2].ids[0] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH_WEST){
		links[2].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_CITY_THREE_ROAD:
	    *nb_links = 3;
	    // biome 1 CITY
	    links[0].biome = CITY;
	    links[0].nb_ids = 6;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)];
	    links[0].ids[1] = oriented_ids[get_id(WEST, 2, tile->move->dir)];
	    links[0].ids[2] = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[0].ids[3] = oriented_ids[get_id(NORTH, 2, tile->move->dir)];
	    links[0].ids[4] = oriented_ids[get_id(EAST, 2, tile->move->dir)];
	    links[0].ids[5] = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    if(tile->move->place == POS_CENTER){
		links[0].has_meeple = tile->move->player;
	    }
	    // biome 2 FARM next to city
	    links[1].biome = FARM;
	    links[1].nb_ids = 1;
	    links[1].city1 = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH_EAST){
		links[1].has_meeple = tile->move->player;
	    }
	    // biom 3 FARM next to city
	    links[2].biome = FARM;
	    links[2].nb_ids = 1;
	    links[2].city1 = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[2].city2 = NULL;
	    links[2].ids[0] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH_WEST){
		links[2].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_ROAD_STRAIGHT:
	    *nb_links = 2;
	    // biome 1 FARM
	    links[0].biome = FARM;
	    links[0].nb_ids = 4;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)];
	    links[0].ids[1] = oriented_ids[get_id(WEST, 2, tile->move->dir)];
	    links[0].ids[2] = oriented_ids[get_id(NORTH, 2, tile->move->dir)];
	    links[0].ids[3] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)];
	    if(tile->move->place == POS_WEST){
		links[0].has_meeple = tile->move->player;
	    }
	    // biome 2 FARM
	    links[1].biome = FARM;
	    links[1].nb_ids = 4;
	    links[1].city1 = NULL;
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[1].ids[1] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)];
	    links[1].ids[2] = oriented_ids[get_id(EAST, 2, tile->move->dir)];
	    links[1].ids[3] = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    if(tile->move->place == POS_EAST) {
		links[1].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_ROAD_TURN:
	    *nb_links = 2;
	    // biome 1 FARM
	    links[0].biome = FARM;
	    links[0].nb_ids = 6;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)];
	    links[0].ids[1] = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[0].ids[2] = oriented_ids[get_id(NORTH, 2, tile->move->dir)];
	    links[0].ids[3] = oriented_ids[get_id(EAST, 2, tile->move->dir)];
	    links[0].ids[4] = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    links[0].ids[5] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)];
	    if(tile->move->place == POS_NORTH_EAST){
		links[0].has_meeple = tile->move->player;
	    }
	    // biome 2 FARM
	    links[1].biome = FARM;
	    links[1].nb_ids = 2;
	    links[1].city1 = NULL;
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(WEST, 2, tile->move->dir)];
	    links[1].ids[1] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH_WEST){
		links[1].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_JUNCTION_THREE:
	    *nb_links = 3;
	    // biome 1 FARM
	    links[0].biome = FARM;
	    links[0].nb_ids = 4;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)];
	    links[0].ids[1] = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[0].ids[2] = oriented_ids[get_id(NORTH, 2, tile->move->dir)];
	    links[0].ids[3] = oriented_ids[get_id(EAST, 2, tile->move->dir)];
	    if(tile->move->place == POS_NORTH){
		links[0].has_meeple = tile->move->player;
	    }
	    // biome 2 FARM
	    links[1].biome = FARM;
	    links[1].nb_ids = 2;
	    links[1].city1 = NULL;
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(WEST, 2, tile->move->dir)];
	    links[1].ids[1] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH_WEST){
		links[1].has_meeple = tile->move->player;
	    }
	    // biome 3 FARM
	    links[2].biome = FARM;
	    links[2].nb_ids = 2;
	    links[2].city1 = NULL;
	    links[2].city2 = NULL;
	    links[2].ids[0] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)];
	    links[2].ids[1] = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH_EAST){
		links[2].has_meeple = tile->move->player;
	    }
	    break;
	case CARD_JUNCTION_FOUR:
	    *nb_links = 4;
	    // biome 1 FARM
	    links[0].biome = FARM;
	    links[0].nb_ids = 2;
	    links[0].city1 = NULL;
	    links[0].city2 = NULL;
	    links[0].ids[0] = oriented_ids[get_id(WEST, 1, tile->move->dir)];
	    links[0].ids[1] = oriented_ids[get_id(NORTH, 2, tile->move->dir)];
	    if(tile->move->place == POS_NORTH_WEST){
		links[0].has_meeple = tile->move->player;
	    }
	    // biome 2 FARM
	    links[1].biome = FARM;
	    links[1].nb_ids = 2;
	    links[1].city1 = NULL;
	    links[1].city2 = NULL;
	    links[1].ids[0] = oriented_ids[get_id(WEST, 2, tile->move->dir)];
	    links[1].ids[1] = oriented_ids[get_id(SOUTH, 1, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH_WEST){
		links[1].has_meeple = tile->move->player;
	    }
	    // biome 3 FARM
	    links[2].biome = FARM;
	    links[2].nb_ids = 2;
	    links[2].city1 = NULL;
	    links[2].city2 = NULL;
	    links[2].ids[0] = oriented_ids[get_id(SOUTH, 2, tile->move->dir)];
	    links[2].ids[1] = oriented_ids[get_id(EAST, 1, tile->move->dir)];
	    if(tile->move->place == POS_SOUTH_EAST){
		links[2].has_meeple = tile->move->player;
	    }
	    // biome 4 FARM
	    links[3].biome = FARM;
	    links[3].nb_ids = 2;
	    links[3].city1 = NULL;
	    links[3].city2 = NULL;
	    links[3].ids[0] = oriented_ids[get_id(NORTH, 1, tile->move->dir)];
	    links[3].ids[1] = oriented_ids[get_id(EAST, 2, tile->move->dir)];
	    if(tile->move->place == POS_NORTH_EAST){
		links[3].has_meeple = tile->move->player;
	    }
	    break;
	case LAST_CARD: default:
	    break;
    }
    for (int i = 0 ; i < *nb_links ; i++){
        for (int j = links[i].nb_ids ; j < NB_IDS ; j++){
            links[i].ids[j] = NULL;
        }
    }
}


int tile__played(const struct tile *tile)
{
    return tile->played;
}
