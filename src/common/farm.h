//farm.h

#ifndef FARM_H
#define FARM_H


#include "constants.h"
#include "list.h"

struct tile_zone_info;


struct farm{
    int id;
    int farmers[MAX_NB_PLAYERS];
    struct list *cities;
};


int farm__new(struct list *farms, struct list *ref_farms);

void *farm__copy(void *farm);

void farm__free(void *farm);

void farm__remove(struct list *, struct farm *);

void farm__merge(struct list *ref_farms, int id1, int id2);

//add a city to a farm's cities list
void farm__add_city(struct farm *farm, void * city);

//add a farmer to a farm's farmers list
void farm__add_farmer(struct farm *farm, int id_player);

int farm__not_in(struct list *ref_farms, int *defined_ids, int n, int id_value);

void farm__update(struct list *farms, struct list *ref_farms, struct list *ref_cities, struct tile_zone_info *info);

void farm__update_cities(struct list * farms);

void farm__update_points(struct farm *farms, int *points);
#endif
