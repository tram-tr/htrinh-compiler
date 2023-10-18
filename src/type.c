#include "../inc/type.h"

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params, struct expr *arr_expr ) {
    struct type* t = malloc(sizeof(struct type));
    t->kind = kind;
    t->params = params;
    t->subtype = subtype;
    t->arr_expr = arr_expr;
    return t;
}