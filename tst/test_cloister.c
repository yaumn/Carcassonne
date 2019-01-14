#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "interface.h"
#include "cloister.h"
#include "road.h"
#include "list.h"
#include "board.h"
#include "tile.h"


void test__cloisters_update()
{
  printf("\t%s", __func__);
  struct list l;
  list__init(&l, NULL, cloister__free);
  struct move m;
  m.card = CARD_MONASTERY_ROAD;
  m.place = POS_CENTER;
  m.onto.x = 2;
  m.onto.y = 3;
  m.player = 1;
  cloisters__update(&l,&m);
  struct cloister *c = list__get(&l,0);
  assert(list__size(&l) == 1);
  assert(c->position.x == 2);
  assert(c->position.y == 3);
  assert(c->id_player == 1);
  m.place = POS_NORTH;
  cloisters__update(&l,&m);
  assert(list__size(&l) == 1);
  list__free(&l);
  printf(BOLD(GREEN("\t\t\tPASSED"))"\n");
}


void test__cloister_assign_points()
{
  printf("\t%s", __func__);
  struct cloister c;
  c.points = 6;
  c.id_player = 0;
  int points[] = {0,0,0,2,3};
  cloister__assign_points(&c, points);
  assert(points[0] == 6);
  c.id_player = 3;
  cloister__assign_points(&c, points);
  assert(points[3] == 8);
  printf(BOLD(GREEN("\t\tPASSED"))"\n");
}


void test__cloister_update_points()
{
  printf("\t%s", __func__);
  struct board b;
  board__init(&b);
  struct cloister c;
  c.points = 0;
  c.id_player = 0;
  c.position.x = 1;
  c.position.y = 1;
  b.field[70][70].played = 0;
  b.field[71][70].played = 0;
  b.field[72][70].played = 1;
  b.field[70][71].played = 1;
  b.field[71][71].played = 1;
  b.field[72][71].played = 1;
  b.field[70][72].played = 1;
  b.field[71][72].played = 1;
  b.field[72][72].played = 1;
  cloister__update_points(&b,&c);
  assert(c.points == 7);
  board__free(&b);
  printf(BOLD(GREEN("\t\tPASSED"))"\n");
}


void test__cloister_is_finished()
{
  printf("\t%s", __func__);
  struct cloister c;
  c.points = 2;
  assert(!cloister__is_finished(&c));
  c.points = 9;
  assert(cloister__is_finished(&c));
  printf(BOLD(GREEN("\t\tPASSED"))"\n");
}

void test__cloister_get_id_player()
{
  printf("\t%s", __func__);
  struct cloister c;
  c.id_player = 2;
  assert(cloister__get_id_player(&c) == 2);
  c.id_player = 3;
  assert(cloister__get_id_player(&c) == 3);
  printf(BOLD(GREEN("\t\tPASSED"))"\n");
}


void test__cloister_get_points()
{
  printf("\t%s", __func__);
  struct cloister c;
  c.points = 2;
  assert(cloister__get_points(&c) == 2);
  c.points = 3;
  assert(cloister__get_points(&c) == 3);
  printf(BOLD(GREEN("\t\tPASSED"))"\n");
}


void test__cloister_get_position()
{
  printf("\t%s", __func__);
  struct cloister* c = malloc(sizeof(struct cloister));
  c->position.x = 2;
  c->position.y = 3;
  struct position pos = cloister__get_position(c);
  assert(pos.x == 2 && pos.y == 3);
  free(c);
  printf(BOLD(GREEN("\t\tPASSED"))"\n");
}


int main()
{
  printf("Testing cloister ...\n");

  test__cloisters_update();
  test__cloister_assign_points();
  test__cloister_update_points();
  test__cloister_is_finished();
  test__cloister_get_id_player();
  test__cloister_get_points();
  test__cloister_get_position();
  return 0;
}
