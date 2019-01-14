#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "interface.h"
#include "list.h"
#include "move.h"


unsigned int player_id;
unsigned int nb_players;


void to_lower_str(char *str)
{
    while (*str != '\0') {
	*str = tolower(*str);
	str++;
    }
}


static enum direction str_to_direction(char *dir)
{
    to_lower_str(dir);
    
    if (strcmp(dir, "north\n") == 0) {
	return NORTH;
    } else if (strcmp(dir, "west\n") == 0) {
	return WEST;
    } else if (strcmp(dir, "south\n") == 0) {
	return SOUTH;
    }  else if (strcmp(dir, "east\n") == 0) {
	return EAST;
    } else {
	return -1;
    }
}


static enum place str_to_place(char *place)
{
    to_lower_str(place);
   
    if (strcmp(place, "none\n") == 0) {
	return NO_MEEPLE;
    } else if (strcmp(place, "north east\n") == 0) {
	return POS_NORTH_EAST;
    } else if (strcmp(place, "north\n") == 0) {
	return POS_NORTH;
    } else if (strcmp(place, "north west\n") == 0) {
	return POS_NORTH_WEST;
    } else if (strcmp(place, "west north\n") == 0) {
	return POS_WEST_NORTH;
    } else if (strcmp(place, "west\n") == 0) {
	return POS_WEST;
    } else if (strcmp(place, "west south\n") == 0) {
	return POS_WEST_SOUTH;
    } else if (strcmp(place, "south west\n") == 0) {
	return POS_SOUTH_WEST;
    } else if (strcmp(place, "south\n") == 0) {
	return POS_SOUTH;
    } else if (strcmp(place, "south east\n") == 0) {
	return POS_SOUTH_EAST;
    } else if (strcmp(place, "east south\n") == 0) {
	return POS_EAST_SOUTH;
    } else if (strcmp(place, "east\n") == 0) {
	return POS_EAST;
    } else if (strcmp(place, "east north\n") == 0) {
	return POS_EAST_NORTH;
    } else if (strcmp(place, "center\n") == 0) {
	return POS_CENTER;
    } else {
	return LAST_POS;
    }
}


static void clean_buffer(const char *buffer, FILE *fp)
{
    char *p = strchr(buffer,'\n');
    if (p != NULL) {
        *p = 0;
    } else {
        int c;
        while ((c = fgetc(fp)) != '\n' && c != EOF);
    }
}


char const * get_player_name()
{
    return "Human client";
}


void initialize(unsigned int id, unsigned int n_players)
{
    player_id = id;
    nb_players = n_players;
}


struct move play(enum card_id card_id, struct move const previous_moves[], size_t n_moves)
{
    struct board board;
    board__init(&board);

    for (size_t i = 1 ; i < n_moves ; i++) {
         if (previous_moves[i].check == VALID) {
	     board__add_tile(&board, &((struct move *) previous_moves)[i]);
	     board__add_move(&board, previous_moves[i]);
         }
    }

    struct move m;
    m.player = player_id;
    m.card = card_id;

    do {
	int input_again = 1;
	while (input_again) {
	    printf("Enter a valid position (ex: 6, 2) : ");
	    char coord[10];
	    fgets(coord, sizeof(coord), stdin);
	    if (sscanf(coord, "%d, %d", &m.onto.x, &m.onto.y)) {
		input_again = 0;
	    }
	    clean_buffer(coord, stdin);
	}
	
	input_again = 1;
	while (input_again) {
	    printf("Enter a valid meeple position (ex: north east) or none for no meeple : ");
	    char place_str[15];
	    if (fgets(place_str, sizeof(place_str), stdin)) {
		const enum place place = str_to_place(place_str);
		if (place != LAST_POS) {
		    m.place = place;
		    input_again = 0;
		}
	    }
	    clean_buffer(place_str, stdin);
	}
	
	input_again = 1;
	while (input_again) {
	    printf("Enter a valid orientation (ex: south) : ");
	    char orientation[7];
	    if (fgets(orientation, sizeof(orientation), stdin)) {
		const enum direction dir = str_to_direction(orientation);
		if (dir >= NORTH && dir <= EAST) {
		    m.dir = dir;
		    input_again = 0;
		}
	    }
	    clean_buffer(orientation, stdin);
	}
    } while (!move__is_valid(&board, &m));
    
    board__free(&board);
    return m;
}


void finalize()
{ }
