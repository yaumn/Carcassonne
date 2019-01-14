#ifndef PLAYER_H
#define PLAYER_H


#include "interface.h"


struct player {
    char const *(*get_name)(void);
    void (*initialize_player)(unsigned int, unsigned int);
    struct move (*play_move)(enum card_id, struct move const *, size_t);
    void (*finalize_player)(void);
};


#endif // PLAYER_H
