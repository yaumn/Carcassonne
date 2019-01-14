//test_farm.c

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "constants.h"
#include "farm.h"
#include "list.h"
#include "reference.h"
#include "tile.h"
#include "city.h"

/*
void test_farm_***()
{
  int error = 0;
  printf("Testing farm__*** ...\n");
  struct list *ref_farms = ref__init();
  struct list farms;
  list__init(&farms, farm__copy, farm__free);
  int id1 = farm__new(&farms, ref_farms);
  int id2 = farm__new(&farms, ref_farms);
  if(){
    error++;
    printf("\t\tERROR : *** \n");
  }
  if(){
    error++;
    printf("\t\tERROR : *** \n");
  }
  if (!error){
    printf("    Tests PASSED\n");
  }
  farm__remove(ref_farms, &farms, 0);
  ref__free(ref_farms);
  list__free(&farms);
}
*/

void test__farm__new_remove_free()
{
  int error = 0;
  printf("\t%s", __func__);
  struct list *ref_farms = ref__init();
  struct list farms;
  list__init(&farms, farm__copy, farm__free);
  int id1 = farm__new(&farms, ref_farms);
  int id2 = farm__new(&farms, ref_farms);
  if(list__size(&farms) != 2 && list__size(ref_farms) != 2){
    error++;
    printf("\t\tERROR : lists' length souhld be equal to 2 \n");
  }
  if(ref_farms->d[id1] != farms.d[id1] && ref_farms->d[id2] != farms.d[id2]){
    error++;
    printf("\t\tERROR : the ref list and the farm list should be equal\n");
  }
  farm__remove(&farms, ref__get(ref_farms,0));
  ref__free(ref_farms);
  list__free(&farms);
  if (!error){
    printf(BOLD(GREEN("\t\tPASSED"))"\n");
  }
}

void test__farm__add_farmer()
{
  int error = 0;
  printf("\t%s", __func__);
  struct farm *f = malloc(sizeof(struct farm));
  f->id = 0;
  f->cities = ref__init();
  for(int i = 0; i <MAX_NB_PLAYERS; i++){
    f->farmers[i] = 0;
  }
  for(int i = 0; i <MAX_NB_PLAYERS; i++){
    farm__add_farmer(f, i);
    //printf("le joueur %d a %d farmers\n",i, f->farmers[i] );
  }
  if(f->farmers[0] != 1 || f->farmers[1] != 1
    || f->farmers[2] != 1 || f->farmers[3] != 1
    || f->farmers[4] != 1){
    error++;
    printf("\t\tERROR : All players should have a farmer \n");
  }
  ref__free(f->cities);
  free(f);
  if(!error){
    printf(BOLD(GREEN("\t\t\tPASSED"))"\n");
  }
}

void test__farm__add_city()
{
  int error = 0;
  printf("\t%s", __func__);
  struct farm *f = malloc(sizeof(struct farm));
  f->id = 0;
  f->cities = ref__init();
  for(int i = 0; i <MAX_NB_PLAYERS; i++){
    f->farmers[i] = 0;
  }
  struct city **c = malloc(sizeof(struct city *)*3);
  for(int i =0; i < 3 ; i ++){
    c[i] = malloc(sizeof(char));
  }
  farm__add_city(f, c[0]);
  if(f->cities->size != 1){
    error++;
    printf("\t\tERROR : There should be a city jouxting the farm \n");
  }
  farm__add_city(f, c[0]);
  if(f->cities->size != 1){
    error++;
    printf("\t\tERROR : There should be only one city jouxting the farm \n");
  }
  farm__add_city(f, c[1]);
  if(f->cities->size != 2){
    error++;
    printf("\t\tERROR : There should be two cities jouxting the farm \n");
  }
  farm__add_city(f, c[2]);
  if(f->cities->size != 3){
    error++;
    printf("\t\tERROR : There should be three cities jouxting the farm \n");
  }
  for(int i = 0; i < 3 ; i++){
    free(c[i]);
  }
  free(c);
  ref__free(f->cities);
  free(f);
  if(!error){
    printf(BOLD(GREEN("\t\t\tPASSED"))"\n");
  }
}

void test__farm__not_in()
{
  int error = 0;
  printf("\t%s", __func__);
  struct list *ref_farms = ref__init();
  struct list farms;
  list__init(&farms, farm__copy, farm__free);
  int ids[15];
  int defined_ids[10];
  int res;
  for(int i = 0; i < 15 ; i ++){
    ids[i] = farm__new(&farms, ref_farms);
    if (i < 10){
      defined_ids[i] = ids[i];
    }
  }
  for(int i = 0; i < 15 ; i++){
    res = farm__not_in(ref_farms, defined_ids, 10, i);
    if( i < 10 && res){
      error++;
      printf("\t\tERROR : the %d farm is already in\n", ids[i]);
    }
    if( i >= 10 && !res){
      error++;
      printf("\t\tERROR : the %d farm is not in \n", ids[i]);
    }
  }
  ref__free(ref_farms);
  list__free(&farms);
  if (!error){
    printf(BOLD(GREEN("\t\t\tPASSED"))"\n");
  }
}

void test__farm__update()
{
  int error = 0;
  printf("\t%s", __func__);
  struct list *ref_farms = ref__init();
  struct list farms;
  list__init(&farms, farm__copy, farm__free);
  int id = farm__new(&farms, ref_farms);
  (void) id;
  // factis cities to test the options
  struct list *ref_cities = ref__init();
  struct city *c1 = malloc(sizeof(int));
  struct city *c2 = malloc(sizeof(int));
  int id_c1 = ref__add(ref_cities, c1);
  int id_c2 = ref__add(ref_cities, c2);
  // factis tile ids
  int *ids[8];
  for(int i = 0; i < 8; i++){
    ids[i] = malloc(sizeof(int));
  }
  // a struct tile_zone_info used for testing
  struct tile_zone_info info;
  // case 1 :
  // creating a field without jouxting cities nor meeple
  // initialisation
  info.biome = FARM;
  info.has_meeple = -1;
  info.city1 = NULL;
  info.city2 = NULL;
  info.nb_ids = 2;
  for(int i = 0; i < 2; i++){
    *(ids[i])= NO_ID;
    info.ids[i] = ids[i];
  }
  farm__update(&farms, ref_farms, ref_cities, &info);
  if(list__size(&farms) != 2){
    error++;
    printf("\t\tERROR : case 1 : the new farm has not been created \n");
  }
  if(*(ids[0]) != 1 || *(ids[1]) != 1){
    error++;
    printf("\t\tERROR : case 1 : ids are not initialised \n");
  }
  // case 2 :
  // creating a field with jouxting cities
  info.biome = FARM;
  info.has_meeple = -1;
  info.city1 = &id_c1;
  info.city2 = &id_c2;
  info.nb_ids = 2;
  for(int i = 2; i < 4; i++){
    *(ids[i])= NO_ID;
    info.ids[i-2] = ids[i];
  }
  farm__update(&farms, ref_farms, ref_cities, &info);
  if(list__size(&farms) != 3){
    error++;
    printf("\t\tERROR : case 2 : the new farm has not been created \n");
  }
  if(*(ids[0]) != 1 || *(ids[1]) != 1 || *(ids[2]) != 2 || *(ids[3]) != 2 ){
    error++;
    printf("\t\tERROR : case 2 : ids are not initialised \n");
  }
  struct farm *farm_test = ref__get(ref_farms, 2);
  if( list__size(farm_test->cities) != 2 ){
    error++;
    printf("\t\tERROR : case 2 : cities have not been add\n");
  }
  //case 3 :
  // updating a field without jouxting cities nor farmers
  info.biome = FARM;
  info.has_meeple = -1;
  info.city1 = NULL;
  info.city2 = NULL;
  info.nb_ids = 3;
  for(int i = 4; i < 6; i++){
    *(ids[i])= NO_ID;
    info.ids[i-4] = ids[i];
  }
  info.ids[2] = ids[0];
  farm__update(&farms, ref_farms, ref_cities, &info);
  if(list__size(&farms) != 3){
    error++;
    printf("\t\tERROR : case 3 : the farm 1 has not been update \n");
  }
  if(*(ids[0]) != 1 || *(ids[1]) != 1 ||
   *(ids[2]) != 2 || *(ids[3]) != 2 ||
   *(ids[4]) != 1 || *(ids[5]) != 1 ){
    error++;
    printf("\t\tERROR : case 3 : ids are not initialised \n");
  }
  farm_test = ref__get(ref_farms, 1);
  if( list__size(farm_test->cities) != 0 ){
    error++;
    printf("\t\tERROR : case 3 : city number is not null\n");
  }
  // case 4 :
  // updating a field with a farmer
  info.biome = FARM;
  info.has_meeple = 3;
  info.city1 = NULL;
  info.city2 = NULL;
  info.nb_ids = 2;
  for(int i = 2; i < 4; i++){
    info.ids[i-2] = ids[i];
  }
  farm__update(&farms, ref_farms, ref_cities, &info);
  if(list__size(&farms) != 3){
    error++;
    printf("\t\tERROR : case 4 : the farm 2 has not been update \n");
  }
  if(*(ids[0]) != 1 || *(ids[1]) != 1 ||
   *(ids[2]) != 2 || *(ids[3]) != 2 ||
   *(ids[4]) != 1 || *(ids[5]) != 1 ){
    error++;
    printf("\t\tERROR : case 4 : ids are not good \n");
  }
  farm_test = ref__get(ref_farms, 2);
  if( list__size(farm_test->cities) != 2 ){
    error++;
    printf("\t\tERROR : case 4 : city number is not two\n");
  }
  if (farm_test->farmers[3] != 1) {
    error++;
    printf("\t\tERROR : case 4 : player 3 has not got is farmer placed\n");
  }
  // case 5 :
  // merging two farms wich both have cities and farmers
  info.biome = FARM;
  info.has_meeple = 3;
  info.city1 = &id_c1;
  info.city2 = &id_c2;
  info.nb_ids = 5;
  for(int i = 6; i < 8; i++){
    *(ids[i])= NO_ID;
    info.ids[i-6] = ids[i];
  }
  info.ids[2] = ids[0];
  info.ids[3] = ids[1];
  info.ids[4] = ids[2];
  farm__update(&farms, ref_farms, ref_cities, &info);
  if(list__size(&farms) != 3){
    error++;
    printf("\t\tERROR : case 5 : the new farm has not been created \n");
  }
  if(*(ids[0]) != 1 || *(ids[1]) != 1 ||
   *(ids[2]) != 2 || *(ids[3]) != 2 ||
   *(ids[4]) != 1 || *(ids[5]) != 1 ||
   *(ids[6]) != 1 || *(ids[7]) != 1){
    error++;
    printf("\t\tERROR : case 5 : ids are not initialised \n");
  }
  farm_test = ref__get(ref_farms, 1);
  if( list__size(farm_test->cities) != 2 ){
    error++;
    printf("\t\tERROR : case 5 : cities have not been add\n");
  }
  if( list__size(ref_farms) != 3 ){
    error++;
    printf("\t\tERROR : case 5 : mess up with ref_farms\n");
  }
  if( ref__get(ref_farms, 1) != ref__get(ref_farms, 2) ){
    error++;
    printf("\t\tERROR : case 5 : the ids in ref are not pointing to the same farm\n");
  }
  if (farm_test->farmers[3] != 2) {
    error++;
    printf("\t\tERROR : case 5 : player 3 has not got is farmer placed\n");
  }
  for(int i = 0; i < 8 ; i++){
    free(ids[i]);
  }
  free(c1);
  free(c2);
  ref__free(ref_farms);
  list__free(&farms);
  ref__free(ref_cities);
  if (!error){
    printf(BOLD(GREEN("\t\t\tPASSED"))"\n");
  }
}

void test__farm__merge()
{
  int error = 0;
  printf("\t%s", __func__);
  struct list *ref_farms = ref__init();
  struct list farms;
  list__init(&farms, farm__copy, farm__free);
  int id1 = farm__new(&farms, ref_farms);
  int id2 = farm__new(&farms, ref_farms);
  farm__merge(ref_farms, id1, id2);
  if (list__size(&farms) != 2){
    error++;
    printf("\t\tERROR : the farm list should have two elements\n");
  }
  if (((struct farm *)farms.d[1])->id != -1){
    error++;
    printf("\t\tERROR : the farm is no longer consider\n");
  }
  if (list__size(ref_farms) != 2){
    error++;
    printf("\t\tERROR : the ref_farm list should have two elements\n");
  }
  if (ref__get(ref_farms, id1) != ref__get(ref_farms, id2)){
    error++;
    printf("\t\tERROR : the ref_farm list elements should both lead to the same farm\n");
  }
  farm__remove(&farms, ref__get(ref_farms, 0));
  ref__free(ref_farms);
  list__free(&farms);
  if (!error){
    printf(BOLD(GREEN("\t\t\tPASSED"))"\n");
  }
}

void test__farm__update_cities()
{
  int error = 0;
  printf("\t%s", __func__);
  //initialise farm variables
  struct list *ref_farms = ref__init();
  struct list farms;
  list__init(&farms, farm__copy, farm__free);
  int id1 = farm__new(&farms, ref_farms);
  int id2 = farm__new(&farms, ref_farms);
  struct farm *farm_test1 = ref__get(ref_farms, id1);
  struct farm *farm_test2 = ref__get(ref_farms, id2);
  //initialise cities and city->pointers
  struct city cities[6];
  for(int i = 0 ; i < 6 ; i++){
    cities[i].pointer = &(cities[i]);
  }
  //test 1 : normal case, all cities are still avaible
  for (int i = 0; i < 3 ; i++){
    ref__add(farm_test1->cities, &(cities[i]));
  }
  for (int i = 3; i < 6 ; i++){
    ref__add(farm_test2->cities, &(cities[i]));
  }
  farm__update_cities(&farms);
  if(list__size(farm_test1->cities) != 3){
    error++;
    printf("\t\tERROR : there are 3 cities jouxting farm 1 and not %ld\n", list__size(farm_test1->cities));
  }
  if(list__size(farm_test2->cities) != 3){
    error++;
    printf("\t\tERROR : there are 3 cities jouxting farm 2 and not %ld\n", list__size(farm_test2->cities));
  }
  //test 2 : some cities are no longer avaible
  for(int i = 0 ; i < 3 ; i++){
    cities[i*2].pointer = &(cities[i*2 +1]);
  }
  farm__update_cities(&farms);
  if(list__size(farm_test1->cities) != 2){
    error++;
    printf("\t\tERROR : there are 2 cities jouxting farm 1 and not %ld\n", list__size(farm_test1->cities));
  }
  if(ref__get(farm_test1->cities, 0) != &(cities[1])){
    error++;
    printf("\t\tERROR : the pointer cities jouxting farm 1 should be %p and not %p\n", &(cities[1]), ref__get(farm_test1->cities, 0));
  }
  if(ref__get(farm_test1->cities, 1) != &(cities[3])){
    error++;
    printf("\t\tERROR : the pointer cities jouxting farm 1 should be %p and not %p\n", &(cities[3]), ref__get(farm_test1->cities, 1));
  }
  if(list__size(farm_test2->cities) != 2){
    error++;
    printf("\t\tERROR : there are 2 cities jouxting farm 2 and not %ld\n", list__size(farm_test2->cities));
  }
  if(ref__get(farm_test2->cities, 0) != &(cities[3])){
    error++;
    printf("\t\tERROR : the pointer cities jouxting farm 2 should be %p and not %p\n", &(cities[3]), ref__get(farm_test2->cities, 0));
  }
  if(ref__get(farm_test2->cities, 1) != &(cities[5])){
    error++;
    printf("\t\tERROR : the pointer cities jouxting farm 2 should be %p and not %p\n", &(cities[5]), ref__get(farm_test2->cities, 1));
  }
  //freeing variables
  ref__free(ref_farms);
  list__free(&farms);
  if (!error){
    printf(BOLD(GREEN("\t\tPASSED"))"\n");
  }
}

void test__farm__update_points()
{
  int error = 0;
  printf("\t%s", __func__);
  //initialise farm variables
  struct list *ref_farms = ref__init();
  struct list farms;
  list__init(&farms, farm__copy, farm__free);
  int id1 = farm__new(&farms, ref_farms);
  struct farm *farm_test1 = ref__get(ref_farms, id1);
  //initialise cities and city->pointers
  struct city cities[6];
  for(int i = 0 ; i < 6 ; i++){
    cities[i].pointer = &(cities[i]);
    cities[i].complete = 1;
  }
  //initialise points array
  int points[5] = {0, 0, 0, 0, 0};
  //test 1 : no meeple on the field
  for (int i = 0; i < 6 ; i++){
    ref__add(farm_test1->cities, &(cities[i]));
  }
  farm__update_points(farm_test1, points);
  for (int i = 0; i < 5 ; i++){
    if(points[i] != 0){
      error++;
      printf("\t\tERROR : player %d should have 0 pts from farms and not %d\n", i, points[i]);
    }
  }
  //test 2 : the player 2 owns the farm
  for(int i = 0 ; i < 3 ; i++){
    farm_test1->farmers[i] = i+3;
  }
  farm__update_points(farm_test1, points);
  for (int i = 0; i < 5 ; i++){
    if( i != 2 && points[i] != 0){
      error++;
      printf("\t\tERROR : player %d should have 0 pts from farms and not %d\n", i, points[i]);
    }
    if( i == 2 && points[i] != 18){
      error++;
      printf("\t\tERROR : player %d should have 18 pts from farms and not %d\n", i, points[i]);
    }
  }
  //freeing variables
  ref__free(ref_farms);
  list__free(&farms);
  if (!error){
    printf(BOLD(GREEN("\t\tPASSED"))"\n");
  }
}



int main()
{
  printf("Testing farm ... \n");

  test__farm__new_remove_free();
  test__farm__add_farmer();
  test__farm__add_city();
  test__farm__not_in();
  test__farm__merge();
  test__farm__update();
  test__farm__update_cities();
  test__farm__update_points();
  return 0;

}
