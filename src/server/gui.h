#ifndef GUI_H
#define GUI_H


#include "constants.h"


struct SDL_Surface;


struct gui {
    struct SDL_Surface *screen;
    float tile_scaling_factor;
    int loaded_tiles;
};


int gui__init(struct gui *gui);

void gui__free();

void gui__load_tile(struct gui *gui, struct move *move);

void gui__wait_for_quit_event();

char *gui__error_message();


#endif // GUI_H
