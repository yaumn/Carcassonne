#ifndef TILE_H
#define TILE_H


#include "interface.h"


/**
 * \brief An enumeration representing the different types of zones
 */
enum biome {
    CITY,
    FARM,
    ROAD,
    LAST_BIOME
};


/**
 * \brief An enumeration representing the different types of followers
 */
enum follower_type {
    FARMER,
    KNIGHT,
    MONK,
    THIEF,
    LAST_FOLLOWER_TYPE
};


/**
 * \brief A structure representing one of the edge of a tile (a tile has 4 edges)
 * An edge can be either connecting a city, a farm and a road separating two farms.
 * Both id fields are the ids of the zones connected by the edge.  
 */
struct edge {
    enum biome biome;
    int id1;
    int id2;
};


/**
 * \brief The structure representing a tile
 * A tile has 4 edges and possibly a follower. Moreover a tile is associated
 * with a move and has a boolean indicating whether it has been played.
 */
struct tile {
    struct edge north, south, east, west; /*!< The four edges of a tile !*/
    struct move *move; /*!< The move associated with the tile !*/
    int played; /*!< A boolean indicating whether the tile has been played !*/
    enum follower_type follower_type; /*!< The type of follower on the tile !*/
};


/**
 * \brief A structure representing the information of a tile's zone
 * A zone can be either a farm or a road (so a tile has often several tile_zone_info associated) 
 * This is a structure that is generated when a new tile is added to the board. It is used 
 * to make connections between the zones of different tiles but after the tile adding is done,
 * the tile_zone_info is freed.
 */
struct tile_zone_info {
    enum biome biome; /*!< The biome of the zone !*/
    int *ids[NB_IDS]; /*!< An array of pointer representing the ids of a tile !*/
    int nb_ids; /*!< The number of ids of the zone (i.e. the actual size of the array ids) !*/
    int has_meeple; /*!< A boolean indicating whether the zone has a meeple !*/
    int *city1;
    int *city2;
};


/**
 * \brief Initializes a tile according to a given move
 * \param tile : a pointer to the tile to initialize
 * \param move : a pointer to the move associated with the tile
 */
void tile__init(struct tile *tile, struct move * const move);


/**
 * \brief Generates zones' information of a tile
 * \param tile : a pointer to the tile
 * \param zones_info : an array of zones info to fill
 * \param nb_zones : the number of different zones
 */
void tile__fill_zones_info(struct tile *tile, struct tile_zone_info zones_info[], int *nb_zones);


/**
 * \brief Indicates whether a tile has been played 
 * \param : a pointer to the tile
 * \return 1 if the tile has been played, 0 otherwise
 */
int tile__played(const struct tile *tile);

void rotation(struct tile *tile, int direction, enum biome biomes[]);

char *tile__card_to_str(const enum card_id c);

char *tile__meeple_place_to_str(const enum place p);

char *tile__direction_to_str(const enum direction d);


char *tile__biome_to_str(const enum biome b);

#endif // TILE_H
