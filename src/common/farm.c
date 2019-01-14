//farm.c

//Libraries
#include <stdlib.h>


#include "farm.h"
#include "list.h"
#include "interface.h"
#include "reference.h"
#include "tile.h"
#include "city.h"



//Functions

/*brief : build a new farm and returns the farm id
 * the function initialise the new farm in the stack,
 * its field are also initialised.
 * Moreover the farm is stored in the farms' list and is referenced in the reference list.
 *param :
 *  farms : a struct list containing the farms, each farm is unique in this list
 *  ref_farms : a list containing the farms' reference, two references can lead to the same farm
 *return :
 *  ref_id : the reference attributed to the new created farm
 */
int farm__new(struct list *farms, struct list *ref_farms)
{
    struct farm * f = malloc(sizeof(struct farm));
    list__push_back(farms, f);
    f->id = list__size(farms)-1;
    for(int i = 0; i < MAX_NB_PLAYERS ; i++){
	f->farmers[i] = 0;
    }
    f->cities = ref__init();
    int ref_id = ref__add(ref_farms, (void *) f);
    return ref_id;
}

/*brief : a copy function used to create the farms' list.
 * This do nothing because the memory allocation is dealt with manually.
 *param :
 *  farm : a struct farm pointer casted in a void pointer
 *return :
 *  farm : the parameter
 */
void *farm__copy(void * farm)
{
    return farm;
}

/*brief : remove a farm from farms' list while letting the reference list untouched.
 *param :
 *  ref_farms : a list containing the farms' reference, two references can lead to the same farm
 *  farms : a struct list containing the farms, each farm is unique in this list
 *  id : the farm id in ref_farms
 *return : nothing
 */
void farm__remove(struct list *farms, struct farm *f)
{
    list__remove(farms, f->id);
}

/*brief : free a struct farm and its fields
 *param :
 *  farm : a void pointer from a struct farm pointer
 *return : nothing
 */
void farm__free(void *farm)
{
    struct farm *f = farm;
    ref__free(f->cities);
    free(f);
}


/*brief : Merge two farms by their ids. If the ids referenced two different farms, then it merges them.
 * To do so, the farmers arrays are merged as long as the cities list.
 * The second farm is deleted in farms list since it is no longer unique.
 * The second farm's id refered to the first farm at the end.
 *param :
 *  farms : a struct list containing the farms, each farm is unique in this list
 *  ref_farms : a list containing the farms' reference, two references can lead to the same farm
 *  id1 : first farm's id
 *  id2 : second farm's id
 *return : nothing
 */
void farm__merge(struct list *ref_farms, int id1, int id2)
{
    struct farm *farm1 = ref__get(ref_farms, id1);
    struct farm *farm2 = ref__get(ref_farms, id2);
    if(farm1 != farm2) {
    	for (int i = 0; i < MAX_NB_PLAYERS ; i ++ ) {
    	    farm1->farmers[i] = farm1->farmers[i] + farm2->farmers[i];
    	}
        ref__merge(farm1->cities, farm2->cities);
        farm2->id = -1;
      for(size_t i = 0 ; i < list__size(ref_farms); i++) {
    	   if(ref__get(ref_farms, i) == farm2) {
           ref__modif(ref_farms, i, farm1);
         }
       }
    }
}

/*brief : add a city to a farm's cities list if its no already in.
 *param :
 *  farm : a struct farm pointer
 *  city : a void pointer casted from a struct city pointer
 *return : nothing
 */
void farm__add_city(struct farm *farm, void * city)
{
    size_t n = list__size(farm->cities);
    size_t i = 0;
    while (i < n && ref__get(farm->cities, i) != city ) {
	i++;
    }
    if (i == n){
	ref__add(farm->cities, city);
    }
}

/*brief :add a farmer to a farm's farmers list.
 *param :
 *  farm : a struct farm pointer
 *  id_player : represents the meeple's owner
 *return : nothing
 */
void farm__add_farmer(struct farm *farm, int id_player)
{
    if (id_player >= 0 && id_player < MAX_NB_PLAYERS){
	farm->farmers[id_player] = farm->farmers[id_player] + 1;
    }
}
/*brief : test if a farm is already pointed out by an element of a reference array.
 *param :
 *  ref_farms : a list containing the farms' reference, two references can lead to the same farm
 *  defined_ids : a reference array in which each element points to a different farm
 *  n : defined_ids length
 *  id_value : the tested element
 *return :
 *  a boolean
 */
int farm__not_in(struct list *ref_farms, int defined_ids[], int n, int id_value)
{
    int incr = 0;
    while(incr < n && ref__get(ref_farms, defined_ids[incr]) != ref__get(ref_farms, id_value)) {
	incr++;
    }
    return incr == n;
}

/*brief : update farm's zones with a struct tile_zone_info.
 * This funciton determine if a new farm is to be created,
 * if a pre-existent farm is to be extented or if several farms are to be merged.
 * Moreover it update the cities next to the farm and if a farmers is to be added.
 *param :
 *  farms : a struct list containing the farms, each farm is unique in this list
 *  ref_farms : a list containing the farms' reference, two references can lead to the same farm
 *  ref_cities : a cities list. It is used to found the cities to add.
 *  link : a struct tile_zone_info which contains all the updating informations.
 *return : nothing
 */
void farm__update(struct list *farms, struct list *ref_farms, struct list *ref_cities, struct tile_zone_info *info)
{
    int defined_ids[NB_IDS] = {NO_ID, NO_ID, NO_ID, NO_ID, NO_ID, NO_ID, NO_ID, NO_ID};
    int n = 0;
    for( int i = 0 ; i < info->nb_ids ; i++) {
	int id_value = *(info->ids[i]);
	if (id_value != NO_ID && farm__not_in(ref_farms, defined_ids, n, id_value)){
	    defined_ids[n] = id_value;
	    n++;
	}
    }
    if (n == 0) {
	int new_id = farm__new(farms, ref_farms);
	for(int i = 0 ; i < info->nb_ids ; i++) {
	    *(info->ids[i]) = new_id;
	}
    }
    if ( n == 1) {
	for(int i = 0 ; i < info->nb_ids ; i++) {
	    *(info->ids[i]) = defined_ids[n-1];
	}
    }
    if (n >= 2){
	for(int i = 1 ; i < n ; i++) {
	    if(ref__get(ref_farms, defined_ids[0]) != ref__get(ref_farms, defined_ids[i])){
		farm__merge(ref_farms, defined_ids[0], defined_ids[i]);
	    }
	}
	for(int i = 0 ; i < info->nb_ids ; i++) {
	    if(*(info->ids[i]) == NO_ID){
		*(info->ids[i]) = defined_ids[0];
	    }
	}
    }
    if(info->has_meeple >= 0) {
	farm__add_farmer(ref__get(ref_farms, *(info->ids[0])), info->has_meeple);
    }
    if (info->city1 != NULL) {
	farm__add_city(ref__get(ref_farms, *(info->ids[0])), ref__get(ref_cities, *(info->city1)));
    }
    if (info->city2 != NULL) {
	farm__add_city(ref__get(ref_farms, *(info->ids[0])), ref__get(ref_cities, *(info->city2)));
    }
}

/*brief :
  *param :
  *   farms : the farm list
  *   cities : the city list
  *return : nothing
*/

void farm__update_cities(struct list * farms)
{
    int n = list__size(farms);
    for (int i = 0; i < n ; i++){
      struct farm *f = list__get(farms, i);
      int m = list__size(f->cities);
      for (int j = 0; j < m ; j++){
        struct city *c = ref__get(f->cities, j);
        while (city__pointer(c) != c){
          c = city__pointer(c);
        }
        int k = 0;
        while (k < m && (c != ref__get(f->cities, k) || k == j) ){
          k++;
        }
        if( k == m){
          ref__modif(f->cities, j , c);
        } else {
          list__remove(f->cities, j);
          m--;
        }
      }
    }
}

/*brief : add the points from farm zones to the points table
 *param :
 *  farm : the farm list
 *  points : the points table
 * return : nothing
 */
void farm__update_points(struct farm *farm, int *points)
{
    if(farm->id != -1){
	int max_farmers = 0;
	int farm_points = 0;
	int nb_cities = list__size(farm->cities);
	// counting the farm points due to city closed
	for(int j = 0; j < nb_cities ; j++){
	    if ( city__complete(ref__get(farm->cities, j)) == 1){
	      farm_points = farm_points + 3;
	      }
	}
	// shearching the max number of farmers
	for(int k = 0 ; k < MAX_NB_PLAYERS ; k++){
	    if( farm->farmers[k] > max_farmers ){
		      max_farmers = farm->farmers[k];
	    }
	}
	// distribute the farm points to the players having the maximum of farmers
	if ( max_farmers > 0){
	    for(int k = 0 ; k < MAX_NB_PLAYERS ; k++){
		if( farm->farmers[k] == max_farmers ){
		    points[k] = points[k] + farm_points;
		}
	    }
	}
    }
}
