#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "list.h"


static void *copy_int(void *e)
{
    int *new = malloc(sizeof(int));
    *new = *((int *)e);
    return new;
}


void test__init()
{
    printf("\t%s", __func__);

    struct list l;
    list__init(&l, copy_int, free);

    assert( l.d == NULL );
    assert( l.size == 0 );
    assert( l.capacity == 0 );

    assert( list__remove(&l, 0) != SUCCESS );

    list__free(&l);

    printf(BOLD(GREEN("\t\t\t\tPASSED"))"\n");
}


void test__add()
{
    printf("\t%s", __func__);

    struct list l;
    list__init(&l, copy_int, free);

    int a = 3, b = 67, c = -54, d = 0;

    assert( list__insert(&l, &a, 3) != SUCCESS );
    assert( list__insert(&l, &a, 0) == SUCCESS );
    assert( list__insert(&l, &b, 0) == SUCCESS );
    assert( list__insert(&l, &c, 1) == SUCCESS );
    assert( list__push_back(&l, &d) == SUCCESS );

    assert( (*(int *)l.d[0]) == 3 );
    assert( (*(int *)l.d[1]) == 67 );
    assert( (*(int *)l.d[2]) == -54 );
    assert( (*(int *)l.d[3]) == 0 );

    list__free(&l);

    printf(BOLD(GREEN("\t\t\t\tPASSED"))"\n");
}


void test__remove()
{
    printf("\t%s", __func__);

    struct list l;
    list__init(&l, copy_int, free);

    int a = 3, b = 67, c = -54, d = 0;

    list__push_back(&l, &a);
    list__push_back(&l, &b);
    list__push_back(&l, &c);
    list__push_back(&l, &d);

    assert( list__pop_back(&l) == SUCCESS );
    assert( list__remove(&l, 3) != SUCCESS );
    assert( list__remove(&l, 0) == SUCCESS );
    assert( (*(int *)l.d[0]) == 67 );
    assert( (*(int *)l.d[1]) == -54 );

    list__free(&l);

    printf(BOLD(GREEN("\t\t\t\tPASSED"))"\n");
}


void test__size_and_capacity_change()
{
    printf("\t%s", __func__);

    struct list l;
    list__init(&l, copy_int, free);

    int a = 3, b = 67, c = -54, d = 0;

    list__push_back(&l, &a);
    assert( l.size == 1 );
    assert( l.capacity == 2 );

    list__push_back(&l, &b);
    assert( l.size = 2 );
    assert( l.capacity == 2 );

    list__push_back(&l, &c);
    assert( l.size = 3 );
    assert( l.capacity == 4 );

    list__push_back(&l, &d);
    assert( l.size = 4 );
    assert( l.capacity == 4 );

    list__pop_back(&l);
    assert( l.size == 3 );
    assert( l.capacity == 4 );

    list__pop_back(&l);
    assert( l.size == 2 );
    assert( l.capacity == 2 );

    list__pop_back(&l);
    assert( l.size == 1 );
    assert( l.capacity == 1 );

    list__pop_back(&l);
    assert( l.size == 0 );
    assert( l.capacity == 0 );

    list__free(&l);

    printf(BOLD(GREEN("\t\tPASSED"))"\n");
}


int main()
{
    printf("Testing list...\n");

    test__init();
    test__add();
    test__remove();
    test__size_and_capacity_change();

    return 0;
}
