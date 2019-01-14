#ifndef BOARD_H
#define BOARD_H


#include "constants.h"
#include "list.h"
#include "tile.h"


struct move;


/**
 * \brief The structure representing the game board
 * The bord contains an array of tiles and lists of the different zones and their references.
 * Moreover, it contains the cards stack and the moves already played.
 */
struct board {
    struct tile field[FIELD_WIDTH][FIELD_HEIGHT];
    
    struct list cities;
    struct list cloisters;
    struct list farms;
    struct list roads;
    
    struct list ref_cities;
    struct list ref_farms;

    struct list edges;
    
    enum card_id cards_stack[CARDS_STACK_SIZE];
    int drawn_cards;
    struct move *moves;
    int moves_nb;

    int points[MAX_NB_PLAYERS];
    int followers_left[MAX_NB_PLAYERS];
};


/**
 * \brief Initializes a board
 * \param board : a pointer to the board to initialize
 */
void board__init(struct board *board);


/**
 * \brief Frees the memory allocated for the board's fields.
 * \param board : a pointer to the board
 */
void board__free(struct board *board);


struct tile *board__get_tile(struct board *board, const int x, const int y);


/**
 * \brief Adds a tile on the field of the board
 * \param board : a pointer to the board
 * \param move : a pointer to the move
 */
void board__add_tile(struct board *board, struct move *move);


/**
 * \brief Indicates whether the game is finished (i.e. all the cards have been drawn)
 * \param board : a pointer to the board
 * \return 1 if the game is finished, 0 otherwise
 */
int board__game_is_finished(const struct board *board);


/**
 * \brief Draws a card from de the cards' stack
 * \param board : a pointer to the board
 * \return the card's id
 */
enum card_id board__draw_card(struct board *board);


/**
 * \brief Adds a move to the list of move of the board
 * \param board : a pointer to the board
 * \param move : a copy of the move to add
 */
void board__add_move(struct board *board, const struct move move);


/**
 * \brief Updates the points of the players
 * \param board : a pointer to the board
 */
void board__update_points(struct board *board);


#endif // BOARD_H
