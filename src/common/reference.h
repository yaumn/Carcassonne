//reference.h

#ifndef REFERENCE_H
#define REFERENCE_H

void * ref__get(struct list *l, int id);
void ref__modif(struct list *l, const int id, void *e);
int ref__add(struct list *l, void *e);
struct list *ref__init();
void ref__free(struct list *l);
void ref__merge(struct list *l1, struct list *l2);

#endif
