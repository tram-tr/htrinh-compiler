#ifndef TYPE_H
#define TYPE_H

#include "param_list.h"

typedef enum {
	TYPE_VOID,
	TYPE_BOOLEAN,
	TYPE_CHARACTER,
	TYPE_INTEGER,
	TYPE_FLOAT,
	TYPE_STRING,
	TYPE_ARRAY,
	TYPE_FUNCTION,
	TYPE_AUTO
} type_t;

struct type {
	type_t kind;
	struct param_list *params;
	struct type *subtype;
	struct expr *arr_expr;
};

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params );
void          type_print( struct type *t );

void type_resolve( struct scope *s, struct type *t );

#endif