//reference.c

#include <stdlib.h>

#include "interface.h"
#include "list.h"


void *el__cpy (void *e)
{
  return e;
}


void el__free(void *e)
{
  (void) e;
}


void * ref__get(struct list *l, int id)
{
  return l->d[id];
}


void ref__modif(struct list *l, const int id, void *e)
{
  l->d[id] = e;
}


int ref__add(struct list *l, void *e)
{
  int id = list__size(l);
  list__insert(l, e, id);
  return id;
}


struct list *ref__init()
{
  struct list *l = malloc(sizeof(struct list));
  list__init(l, el__cpy, el__free);
  return l;
}

void ref__free(struct list *l)
{
  list__free(l);
  free(l);
}

void ref__merge(struct list *l1, struct list *l2)
{
  int n = list__size(l1);
  int m = list__size(l2);
  for(int j = 0; j < m ; j++) {
    int i = 0;
    while (i < n && (ref__get(l1, i) != ref__get(l2, j))) {
      i++;
    }
    if ( i == n) {
      ref__add(l1, ref__get(l2, j));
      n++;
    }
  }
}
