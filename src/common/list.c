#include <stdlib.h>

#include "list.h"


static void shift_left(void **begin, void **end)
{
    for ( ; begin != end ; begin++) {
	*begin = *(begin + 1);
    }
}


static void shift_right(void **begin, void **end)
{
    for ( ; begin != end ; end--) {
	*end = *(end - 1);
    }
}


static void *default_copy(void *e)
{
    return e;
}


static void default_free(void *e)
{
    (void)e;
}


void list__init(struct list *list, void *(*copy_element)(void *), void (*free_element)(void *))
{
    list->d = NULL;
    list->size = 0;
    list->capacity = 0;
    list->copy_element = copy_element;
    list->free_element = free_element;

    if (copy_element == NULL) {
	list->copy_element = default_copy;
    }
    if (free_element == NULL) {
	list->free_element = default_free;
    }
}


int list__insert(struct list *list, void *e, const size_t index)
{
    if (e == NULL || list == NULL || index > list->size) {
	return !SUCCESS;
    }

    if (list->size == list->capacity) {
	const size_t new_capacity = (list->capacity == 0) ? LIST_INIT_CAPACITY : list->capacity * 2;
	void **tmp = realloc(list->d, sizeof(void *) * new_capacity);
	if (tmp == NULL) {
	    return !SUCCESS;
	}

    list->d = tmp;
	list->capacity = new_capacity;
    }

    shift_right(list->d + index, list->d + list->size);

    void *new = list->copy_element(e);
    if (new == NULL) {
	return !SUCCESS;
    }
    list->d[list->size] = new;
    list->size++;

    return SUCCESS;
}


int list__remove(struct list *list, const size_t index)
{
    if (list == NULL || index >= list->size) {
	return !SUCCESS;
    }

    list->free_element(list->d[index]);
    shift_left(list->d + index, list->d + list->size - 1);
    list->size--;

    if (list->size <= list->capacity / 2) {
	if (list->capacity / 2 == 0) {
	    list__free(list);
	    list__init(list, list->copy_element, list->free_element);
	}

	const size_t new_capacity = list->capacity / 2;
	void **tmp = realloc(list->d, sizeof(void *) * new_capacity);
	if (tmp == NULL) {
	    return !SUCCESS;
	}

	list->d = tmp;
	list->capacity = new_capacity;
    }

    return SUCCESS;
}


int list__push_back(struct list *list, void *e)
{
    return list__insert(list, e, list->size);
}


int list__pop_back(struct list *list)
{
    return list__remove(list, list->size - 1);
}


size_t list__size(const struct list *list)
{
    return list->size;
}


void list__free(struct list *list)
{
    for (size_t i = 0 ; i < list->size ; i++) {
    list->free_element(list->d[i]);
    }
    free(list->d);
}


void *list__get(struct list *list, const size_t i){
  if (i >= list->size) {
    return NULL;
  }
  return list->d[i];
}
