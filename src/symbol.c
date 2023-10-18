#include "../inc/symbol.h"

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name ) {
    struct symbol *s = malloc(sizeof(struct symbol));
    s->kind = kind;
    s->type = type;
    s->name = name;
    s->which = 0;
    return s;
}