#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>

#include "interface.h"
#include "gui.h"
#include "tile.h"


static void compute_follower_position(SDL_Rect *r, enum place p, const enum direction dir, const int scaled_tile_size)
{
    if (p <= NO_MEEPLE && p >= LAST_POS) {
	return;
    }

    if (p == POS_CENTER) {
	r->x += (scaled_tile_size / 3);
	r->y += (scaled_tile_size / 3);
    } else {
	// Change meeple position according to dir
        p = p + (3 * dir);
	p %= (p > 12) ? 12 : 13;
	
	switch (p) {
	    case POS_NORTH:
		r->x += scaled_tile_size / 3;
		break;
	    case POS_WEST:
		r->y += scaled_tile_size / 3;
		break;
	    case POS_SOUTH:
		r->x += scaled_tile_size / 3;
		r->y += 2 * scaled_tile_size / 3;
		break;
	    case POS_EAST:
		r->x += 2 * scaled_tile_size / 3;
	        r->y += scaled_tile_size / 3;
		break;
	    case POS_WEST_SOUTH: case POS_SOUTH_WEST:
		r->y += 2 * scaled_tile_size / 3;
		break;
	    case POS_SOUTH_EAST: case POS_EAST_SOUTH:
		r->x += 2 * scaled_tile_size / 3;
		r->y += 2 * scaled_tile_size / 3;
		break;
	    case POS_EAST_NORTH: case POS_NORTH_EAST:
		r->x += 2 * scaled_tile_size / 3;
		break;
	    default:
		break;
	}
    }
}


int gui__init(struct gui *gui)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
	return 0;
    }
    SDL_WM_SetCaption("Carcassonne", NULL);

    SDL_Surface *s = SDL_SetVideoMode(0, 0, 32, SDL_FULLSCREEN); 
    
    gui->screen = SDL_SetVideoMode(s->w - 50, s->h - 150, 32, SDL_HWSURFACE);
    SDL_FreeSurface(s);
    if (gui->screen == NULL) {
	return 0;
    }
 
    SDL_Flip(gui->screen);

    gui->tile_scaling_factor = gui->screen->h / (float)(MAX_NB_TILES_PER_COLUMN * TILE_SIZE);
    gui->loaded_tiles = 0;
    return 1;
}


void gui__free()
{
    SDL_Quit();
}


void gui__load_tile(struct gui *gui, struct move *move)
{
    SDL_Surface *image = NULL;

    char filename[20];

    if (gui->loaded_tiles == 0) {
	snprintf(filename, 20, "img/starting.bmp");
    } else if (gui->loaded_tiles < CARDS_STACK_SIZE) {
	snprintf(filename, 20, "img/%d.bmp", move->card);
    } else {
	return;
    }

    image = SDL_LoadBMP(filename);
    
    if (image == NULL) {
	printf(BOLD(YELLOW("WARNING: Couldn't load image %s: %s"))"\n", filename, gui__error_message());
	return;
    }

    const int scaled_tile_size = TILE_SIZE * gui->tile_scaling_factor - 4;

    SDL_Surface *rot = rotozoomSurface(image, 90.0 * move->dir, gui->tile_scaling_factor, 1);
    SDL_FreeSurface(image);
    
    SDL_Rect dst_rect;
    dst_rect.x = (gui->screen->w / 2) - (-move->onto.y * scaled_tile_size) + move->onto.y;
    dst_rect.y = (gui->screen->h / 2) - (-move->onto.x * scaled_tile_size) + move->onto.x;

    SDL_Rect src_rect;
    src_rect.x = 2;
    src_rect.y = 2;
    src_rect.w = scaled_tile_size;
    src_rect.h = scaled_tile_size;

    // rotozoom may cause image a rotated image to be slightly bigger than the original one.
    // Thus, the rotated image may have one or two black lines/columns, which may put
    // the tiles out of alignment
    if (rot->w != scaled_tile_size
	&& rot->h != scaled_tile_size
	&& (move->dir == EAST || move->dir == SOUTH)) {
	src_rect.x += 2;
	src_rect.y += 2;	
    }


    SDL_BlitSurface(rot, &src_rect, gui->screen, &dst_rect);
    SDL_FreeSurface(rot);
    
    if (move->place != NO_MEEPLE) {
	char filename[22];
	snprintf(filename, 22, "img/follower_%d.png", move->player);
	image = IMG_Load(filename);
	compute_follower_position(&dst_rect, move->place, move->dir, scaled_tile_size);

	const float follower_scaling_factor = (scaled_tile_size / 3.0) / FOLLOWER_SIZE;
	rot = rotozoomSurface(image, 0.0, follower_scaling_factor, 1);
	SDL_BlitSurface(rot, NULL, gui->screen, &dst_rect);

	SDL_FreeSurface(rot);
	SDL_FreeSurface(image);
    }
    
    SDL_Flip(gui->screen);
    gui->loaded_tiles++;
}


void gui__wait_for_quit_event()
{
    SDL_Event event;
    int close = 0;
    while(!close)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                close = 1;
                break;
        }
    }
}


char *gui__error_message()
{
    return SDL_GetError();
}
