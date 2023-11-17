#ifndef TYPE_H
#define TYPE_H

#include "param_list.h"

struct decl;
struct stmt;

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

typedef enum {
    ERR_INC,
    ERR_DEC,
    ERR_NOT,
    ERR_NEG,
    ERR_ASSIGN,
    ERR_MATH,
    ERR_LOGIC,
    ERR_COMP,
    ERR_ASSIGN_LEFT,
    ERR_COMP_INVALID_TYPE,
    ERR_FUNC_MISSING_ARGS,
    ERR_FUNC_MANY_ARGS,
    ERR_IF_COND,
    ERR_WHILE_COND,
    ERR_FOR_COND,
    ERR_DECL_VAL,
    ERR_FUNC_TYPE,
    ERR_VOID,
    ERR_NO_RETURN,
    ERR_FUNC_PARAM,
    ERR_FUNC_AUTO_PARAM,
    ERR_NON_ARR,
    ERR_ARR_INDEX,
    ERR_BAD_ARR_TYPE,
    ERR_ARR_LIT_TYPE,
    ERR_ARR_LIT_GLOBAL,
    ERR_AUTO,
    ERR_CONST,
} error_t;

struct type {
	type_t kind;
	struct param_list *params;
	struct type *subtype;
	struct expr *arr_expr;
};

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params );
void          type_print( struct type *t );

void type_print( struct type *t );
int type_compare( struct type *a, struct type *b );
struct type * type_copy( struct type *t );
void type_delete( struct type *t );
struct type * type_convert_arr_literal( struct expr *e );

void type_error_print(error_t err, struct decl *d, struct expr *e1, struct expr *e2, struct param_list *a, struct stmt *s, struct type *t);
void type_error_print_math(struct expr *e);

#endif