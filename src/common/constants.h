#ifndef CONSTANTS_H
#define CONSTANTS_H


#define MAX_NB_PLAYERS 5
#define CARDS_STACK_SIZE 72
#define FIELD_WIDTH ((CARDS_STACK_SIZE * 2) - 1)
#define FIELD_HEIGHT ((CARDS_STACK_SIZE * 2) - 1)
#define DIFFERENT_CARDS_NB 24
#define NO_ID -1
#define NB_IDS 8
#define INITIAL_NB_FOLLOWERS 7
#define MAX_NB_ZONES_PER_TILE 4

#define TILE_SIZE 86
#define FOLLOWER_SIZE 40
#define MAX_NB_TILES_PER_COLUMN 20

#define BOLD(text) "\x1b[1m" text "\x1b[22m"
#define UNDERLINED(text) "\x1b[4m" text "\x1b[24m"

#define RED(text) "\x1b[31m" text "\x1b[0m"
#define GREEN(text) "\x1b[32m" text "\x1b[0m"
#define YELLOW(text) "\x1b[33m" text "\x1b[0m"
#define BLUE(text) "\x1b[34m" text "\x1b[0m"


#endif // CONSTANTS_H
