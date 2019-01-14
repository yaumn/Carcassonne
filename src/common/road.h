#ifndef ROAD_H
#define ROAD_H


#include "constants.h"
#include "interface.h"


struct list;
struct tile;


/**
 * \brief The structure representing a road
 *
 * It is not necessary to store all the tiles of the road, only both extremities
 * and its length. Moreover to attribute the points, all the thieves on the road
 * and its owners must be remembered. 
 */
struct road {
    struct tile *begin; /*!< The first extremity of the road !*/
    struct tile *end; /*!< The other extremity of the road !*/
    enum place begin_road_place;
    enum place end_road_place;
    int length; /*!< The number of tiles of the road !*/
    int closed;
    int thieves[MAX_NB_PLAYERS]; /*!< The number of followers each player has !*/
};


/**
 * \brief Creates and initializes a new road
 *
 * \param tile : a pointer to only tile of the new road
 * \param add_follower : a boolean indicating whether the follower
 *                       on the tile must be added to the road
 * \return The newly created road
 */
struct road *road__init(struct tile *tile, const int add_follower);


/**
 * \brief Frees the memory allocated for the given road
 * \param road : a pointer to the road to be freed
 */
void road__free(void *road);


/**
 * \brief Adds a new tile to the given road
 * \param road : a pointer to the road
 * \param tile : a pointer to the tile to add
 * \param add_at_beginning : a boolean indicating whether the tile must be
 *                           added at the beginning of the road
 * \param add_follower : a boolean indicating whether the follower
 *                       on the tile must be added to the road
 */
void road__add_tile(struct road *road, struct tile *tile,
                    const int add_at_beginning, const int add_follower);


/**
 * \brief Indicates whether a road is closed, i.e. both begin and end are road-closing tiles
 * \param road : a pointer to the road
 * \return 1 if the road is closed, 0 otherwise
 */
int road__is_closed(const struct road *road);


/**
 * \brief Update the points array according to the thieves and the length of the road
 * \param road : a pointer to the road
 * \param points : the array of points to update
 */
void road__update_points(const struct road *road, int points[]);


/**
 * \brief Merges two roads into road1
 * WARNING : To merge two roads, the tile in common must be added beforehand into BOTH roads
 * \param road1 : one of the roads to merge
 * \param road2 : the other road to merge
 */
void road__merge(struct road *road1, struct road *road2);


void road__update(struct list *roads, struct tile *tiles[]);

#endif // ROAD_H
