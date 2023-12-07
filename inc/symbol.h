
#ifndef SYMBOL_H
#define SYMBOL_H

#include "type.h"
#include <string.h>

typedef enum {
	SYMBOL_LOCAL,
	SYMBOL_PARAM,
	SYMBOL_GLOBAL
} symbol_t;

struct symbol {
	symbol_t kind;
	struct type *type;
	char *name;
	int defined;
	int which; // // ordinal position of local variables and parameters
};

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name );
void symbol_print( struct symbol *s );
char * symbol_codegen( struct symbol *s );

#endif