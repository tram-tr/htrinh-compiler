#include "../inc/param_list.h"
#include <stdlib.h>

extern int resolve_error;

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

//  enter a new variable declaration for each parameter of a function
void param_list_resolve( struct scope *s, struct param_list *a ) {
    if (a == 0) return;

    struct param_list *t = a;
    while (t != 0) {
        struct symbol *sym = symbol_create(SYMBOL_PARAM, t->type, t->name);
        int bind = scope_bind(s, t->name, sym);
        if (bind == 1) {
            resolve_error++;
            printf("resolve error: %s is already defined\n", a->name);
        }
        sym->which = s->which;
        t = t->next;
    }
}

