#include "../inc/param_list.h"
#include <stdlib.h>

extern int resolve_error;
extern int typecheck_error;

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
        if (bind == 1)
            resolve_error++;
        sym->which = s->which;
        t = t->next;
    }
}

int param_compare( struct param_list *a, struct param_list *b ) {
    if (!a && !b) return 0;
    if (!a || !b) return 1;

    int err = 0;
    if(type_compare(a->type, b->type) == 1) 
        err++;

    if(param_compare(a->next, b->next) == 1) 
        err++;

    return err;
}

int param_typecheck( struct param_list *a ) {
    struct param_list *t = a;
    while (a != 0) {
        // check for auto type
        if(a->type->kind == TYPE_AUTO)
            return 1;
        a = a->next;
    } 
    return 0;
}

struct param_list * param_copy( struct param_list *a ) {
    if (a == 0)
        return 0;

    return param_list_create(a->name, a->type, param_copy(a->next));
}
