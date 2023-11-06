
#ifndef PARAM_LIST_H
#define PARAM_LIST_H

#include "type.h"
#include "scope.h"
#include <stdio.h>


struct expr;
struct scope;

struct param_list {
	char *name;
	struct type *type;
	struct symbol *symbol;
	struct param_list *next;
};

struct param_list * param_list_create( char *name, struct type *type, struct param_list *next );
void param_list_print( struct param_list *a );

void param_list_resolve( struct scope *s, struct param_list *a );
int param_equals(struct param_list *a, struct param_list *b);

#endif