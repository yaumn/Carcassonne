#ifndef CITY__H
#define CITY__H


#include "constants.h"
#include "interface.h"
#include "list.h"


struct board;
struct tile;


/**
 * \file city.h
 */

/**
 * \struct city
 * \brief Structure made to represent cities
 */
struct city{
    int id;                     //!< Unique identifier for each city (index in board.cities)
    struct city * pointer;     //!< Pointer to the mother city
    struct list tiles;          //!< List of pointers to the moves corresponding to city tiles
    int complete;               //!< Boolean stating wether the city is complete
    int knights[MAX_NB_PLAYERS];   //!< Table indexed by players id containing the number of knights they possess in the city
};

/**
 * \brief Returns the mother city pointer
 * \param the city from which we want the mother city pointer
 * \return a city pointer
 */
struct city *city__pointer( struct city *c );


/**
 * \brief Creates a new city and allocates memory for it
 *        All created cities must be freed with city__free()
 * \param first_tile is a pointer to the move that placed the first tile of the city
 * \param gameboard is a poiter to the gameboard structure
 * \return a pointer to the created city
 */
struct city *city__create( struct move *first_tile, struct board *gameboard );

/**
 * \brief Calls all other function to keep the cities up to date after the placement of a new tile
 *        Checks for towns on all 4 sides of the tile
 *        Also modifies the ids on a tile's border
 * \param gameboard is a pointer to the game gameboard
 * \param new_tile is the tile structure newly placed
 * \return 0 if success, -1 if error
 */
int city__update( struct board *gameboard, struct tile *new_tile );

/**
 * \brief Updates one city edge of a given tile
 *        Intermediate to city__update
 * \param already_connected is a boolean stating wether the city has already been created on another side of the tile
 * \param gameboard is the board
 * \param city_tile is the tile being updated
 * \param side is a number that give the side to upadate with the convention enum dir from \file interface.h
 * \return SUCCESS or FAILURE
 */
int city__update_side( int *already_connected, struct board *gameboard, struct tile *city_tile, struct city **c, int side );

/**
 * \brief Adds a tile to a city
 * \param added_tile is the struct move corresponding to the placement of the tile adde to the city
 * \param c is the city in which we add the tile
 * \return 0 if success, -1 if not
 */
int city__add_tile( struct move *added_tile, struct city *c, struct board *gameboard );

/**
 * \brief Function called by city__add_tile() to change the "knights" table in c
 * \param added is a pointer to the move corresponding to the turn
 * \param c is a pointer to the city being modified
 * \param gamevoard a pointer to the board
 * \return none
 */
void city__add_meeple( struct move *added, struct city *c, struct board *gameboard );

/**
 * \brief Function to make the fusion of two cities
 * \param c1 the first city
 * \param c2 the second city
 * \param gameboard a pointer to the board to suppress one of the cities
 * \return 0 if success, -1 if error
 */
int city__merge( struct city *c1, struct city *c2, struct board *gameboard );

/**
 * \brief Intermediate function that states if the studied border of a two towns card
 *        is in the city
 * \param c a pointer to a city
 * \param dir a direction indicating the studied border
 * \param pos_move a pointer to yhe move played
 * \param gameboard a pointer to the board to suppress one of the cities
 * \return TRUE if the dir border of pos_move is in c
 * \return FALSE in the countrary case
 */
int city__two_cities_check_in( struct city *c, int dir, struct move *pos_move, struct board *gameboard );

/**
 * \brief Intermediate function for city__locally_closed()
 * \param checked a pointer to the city studied
 * \param pos_move a pointer to the move played
 * \param around a table of 4 booleans indexed by directions stating wether the borderis a city
 * \param gameboard a pointer to the board
 * \return TRUE if all sides that are TRUE in around are closed
 */
int city__verify_border( struct move *pos_move, int *around, struct board *gameboard );

/**
 * \brief Checks if all city sides of a tile are closed
 * \param checked a pointer to the city studied
 * \param pos_move a pointer to the move played
 * \param gameboard a pointer to the board
 * \return TRUE if all sides are closed
 */
int city__locally_closed( struct city *checked, struct move *pos_move, struct board *gameboard );

/**
 * \brief Checks if the city c is closed
 * \param c the considerated city
 * \param gameboard a pointer to the board
 * \return 0 if success, -1 in case of error
 */
int city__check_closure( struct city *c, struct board *gameboard );

/**
 * \brief Says if the city is closed
 * \param c : the considerated city pointer
 * \return 1 if the city is closed, 0 if not
 */
int city__complete( struct city * c );

/**
 * \brief Counts how much points a city is worth
 *        Be sure to run city__check_closure() before
 *        It also fills the "knights" table in c
 * \param c the city studied
 * \return the value of c in points
 */
int city__count_points( struct city *c );

/**
 * \brief Attributes city points to the players
 * \param city the city considerated
 * \param points a table of size MAX_NB_PLAYERS
 */
void city__update_points( struct city * city, int * points );

/**
 * \brief frees all allocated memory for a city
 * \param c a pointer to a city
 * \return 0 if succes, -1 if any error occured
 */
void city__free( void *city );

#endif
