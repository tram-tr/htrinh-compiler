#include "../inc/decl.h"
#include <stdlib.h>

extern int resolve_error;

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next ) {
    struct decl* d = malloc(sizeof(struct decl));
    d->name = name;
    d->type = type;
    d->value = value;
    d->code = code;
    d->symbol = 0;
    d->next = next;
    d->local_count = 0;
    d->param_count = 0;
    d->var_count = 0;
    return d;
}

void decl_print( struct decl *d, int indent ) {
    if (d == 0) return;

    indent_print(indent);
    printf("%s: ", d->name);
    type_print(d->type);

    if(d->value) {
        printf(" = ");
        expr_print(d->value);
        printf(";\n");

    } else if (d->code) {
        printf(" = ");
        stmt_print(d->code, indent);

    } else printf(";\n");
    
    decl_print(d->next, indent);
}

void indent_print( int indent ) {
    for (int i = 0; i < indent; i++)
        printf("\t");
}

// create a new symbol, and then bind it to the name of the declaration in the current scope
void decl_resolve ( struct scope *s, struct decl *d ) {
    if (d == 0 || s == 0) return;

    symbol_t kind = scope_level(s) > 1 ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
    
    d->symbol = symbol_create(kind, d->type, d->name);
    
    // resolve expression
    if (d->value != 0) {
        expr_resolve(s, d->value);
        int bind = scope_bind(s, d->name, d->symbol);
        // symbol is already in the symbol table
        if (bind == 1) {
            resolve_error++;
            printf("resolve error: %s is already defined\n", d->name);
        }
    }

    d->symbol->which = s->which;

    // enter a scope
    // resolve parameters and code
    if (d->code != 0) {
        scope_enter(s);

        param_list_resolve(s, d->type->params);
        d->param_count = s->which;

        stmt_resolve(s, d->code);
        d->var_count = d->param_count + (s->which - d->param_count);
        d->local_count = d->var_count - d->param_count;

        scope_exit(s);
    }

    decl_resolve(s, d->next);
}