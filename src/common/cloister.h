#ifndef CLOISTER_H
#define CLOISTER_H


#include "interface.h"


struct board;
struct list;

struct cloister
{
  int id_player;
  int points;
  struct position position;
};

void cloisters__update(struct list* l, struct move *m);

void cloister__assign_points(struct cloister *c, int points[]);

struct cloister* cloister__new(void);

void cloister__free(void *cloister);

void cloister__update_points(struct board *board, struct cloister *c);

int cloister__is_finished(struct cloister*);

int cloister__get_id_player(struct cloister*);

int cloister__get_points(struct cloister*);

struct position cloister__get_position(struct cloister*);


#endif //CLOISTER_H
