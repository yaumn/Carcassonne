#ifndef MOVE_H
#define MOVE_H


struct board;
struct move;

void *move_copy(void *m);

int move__is_valid(struct board* b, struct move* m);

void move__get_all_possible_moves(struct board *board, struct list *possible_moves, const enum card_id c, const int played_id);

struct move *possible_move(struct board* b, enum card_id, unsigned int player_id);


#endif // MOVE_H
