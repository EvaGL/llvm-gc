#include "gc.h"
#include <stdlib.h>
typedef struct _list {
    int meta[2];
    struct _list *next;
} list;

list* create() {
    list* l = malloc(sizeof(list));
    l->meta[0] = 1;
    l->meta[1] = 2 * sizeof(int);
    l->next = NULL;
    return l;
}

int main() {
    list* f = create();
    f->next = create();
    f->next->next = create();
    
    create();
    create();

    list* c = create();
    c->next = create();
    c->next->next = create();
    c->next->next->next = create();

    c->next->next->next->next = c->next;
    gc();
    
    c = NULL;
    gc();

    return 0;
}
