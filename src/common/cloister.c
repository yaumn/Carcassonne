#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "cloister.h"


void cloisters__update(struct list* l, struct move *m)
{
  if((m->card == CARD_MONASTERY_ROAD 
      || m->card == CARD_MONASTERY_ALONE)
     && m->place == POS_CENTER){
    struct cloister* c = cloister__new();
    if(c != NULL){
      c->position.x = m->onto.x;
      c->position.y = m->onto.y;
      c->id_player = m->player;
      list__push_back(l, c);
    }
  }
}

void cloister__assign_points(struct cloister *c, int points[])
{
  if(c->id_player != NO_ID)
    points[c->id_player] += c->points;
}

// This function calculate the new number of points associated to the cloister
// and replaces it in the structure.
// When we will have a score tab, this function would have to modify it.
void cloister__update_points(struct board *board, struct cloister *c)
{
    int count = 0;
    int x = c->position.x;
    int y = c->position.y;
    for(int i = x - 1 ; i < x + 2 ; i++){
      for(int j = y - 1 ; j < y + 2 ; j++){
	if(i >= -CARDS_STACK_SIZE && 
	   j >= -CARDS_STACK_SIZE && 
	   i <= CARDS_STACK_SIZE  && 
	   j <= CARDS_STACK_SIZE    ){
	  count += tile__played(board__get_tile(board, i, j));
	}
      }
    }
    c->points = count;
}

// This function allocates a place for a new struct cloister and returns a pointer to the new cloister 
struct cloister *cloister__new()
{
  struct cloister* c = malloc(sizeof(struct cloister));
  assert( c != NULL );
  return c;
}

void cloister__free(void *cloister)
{
  free(cloister);
}

int cloister__is_finished(struct cloister * c)
{
  return (c->points == 9);
}

int cloister__get_id_player(struct cloister *c)
{
    return c->id_player; 
}


int cloister__get_points(struct cloister *c)
{
    return c->points;
}


struct position cloister__get_position(struct cloister *c)
{
    return c->position;
}


