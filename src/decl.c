#include "../inc/decl.h"

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next ) {
    struct decl* d = malloc(sizeof(struct decl));
    d->name = name;
    d->type = type;
    d->value = value;
    d->code = code;
    d->symbol = NULL;
    d->next = next;
    return d;
}

void decl_print( struct decl *d, int indent ) {
    
}