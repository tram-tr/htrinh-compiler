
#ifndef DECL_H
#define DECL_H

#include "scope.h"
#include "type.h"
#include "stmt.h"
#include "expr.h"
#include "scratch.h"
#include <stdio.h>

struct decl {
	char *name;
	struct type *type;
	struct expr *value;
	struct stmt *code;
	struct symbol *symbol;
	struct decl *next;
	int local_count;
	int param_count;
	int var_count;
	int has_return;
};

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next );
void decl_print( struct decl *d, int indent );
void indent_print( int indent );
void decl_resolve ( struct scope *s, struct decl *d );
void decl_typecheck ( struct decl *d );
void decl_codegen( struct decl *d );
void decl_prologue( struct decl *d );
void decl_epilogue( struct decl *d );
void global_label( struct decl *d );

#endif