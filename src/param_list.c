#include "../inc/param_list.h"
#include <stdlib.h>

struct param_list * param_list_create( char *name, struct type *type, struct param_list *next ) {
    struct param_list* p = malloc(sizeof(struct param_list));
    p->name = name;
    p->type = type;
    p->next = next;
    return p;
}

void param_list_print( struct param_list *a ) {
    if (a == 0) return;

    printf("%s: ", a->name);
    type_print(a->type);

    if (a->next != 0)
        printf(", ");
    param_list_print(a->next);
}