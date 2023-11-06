#include "../inc/symbol.h"
#include <stdlib.h>

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name ) {
    struct symbol *s = malloc(sizeof(struct symbol));
    s->kind = kind;
    s->type = type;
    s->name = name;
    s->which = 0;
    s->defined = 1;
    return s;
}

void symbol_print( struct symbol *s ) {
    switch (s->kind) {
        case SYMBOL_LOCAL:
            printf("%s resolves to local %d\n", s->name, s->which);
            break;
        case SYMBOL_GLOBAL:
            printf("%s resolves to global %s\n", s->name, s->name);
            break;
        case SYMBOL_PARAM:
            printf("%s resolves to param %d\n", s->name, s->which);
            break;
    }
}