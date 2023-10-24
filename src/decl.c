#include "../inc/decl.h"
#include <stdlib.h>

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next ) {
    struct decl* d = malloc(sizeof(struct decl));
    d->name = name;
    d->type = type;
    d->value = value;
    d->code = code;
    d->symbol = 0;
    d->next = next;
    return d;
}

void decl_print( struct decl *d, int indent ) {
    if (d == 0) return;
    for (int i = 0; i < indent; i++)
        printf("\t");

    printf("%s: ", d->name);
    type_print(d->type);

    if(d->value) {
        printf(" = ");
        expr_print(d->value);
        printf(";");

    } else if (d->code) {
        printf(" =\n");
        stmt_print(d->code, indent);

    } else printf(";");
    
    decl_print(d->next, indent);
}