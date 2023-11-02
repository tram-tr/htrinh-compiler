#ifndef EXPR_H
#define EXPR_H

#include "symbol.h"
#include "decl.h"
#include "scope.h"

typedef enum {
	EXPR_ASSIGN = 0,
	EXPR_OR = 10,
	EXPR_AND = 20,
	EXPR_EQ = 30,
	EXPR_LE,
	EXPR_GE,
	EXPR_LT,
	EXPR_GT,
	EXPR_NE,
	EXPR_ADD = 40,
	EXPR_SUB,
	EXPR_MUL = 50,
	EXPR_DIV,
	EXPR_MOD,
	EXPR_EXP = 60,
	EXPR_NOT = 70,
	EXPR_NEG,
	EXPR_INC = 80,
	EXPR_DEC,
	EXPR_FUNC_CALL = 90,
	EXPR_INT = 100,
	EXPR_FLOAT,
	EXPR_BOOL,
	EXPR_CHAR,
	EXPR_STRING,
	EXPR_ARR,
	EXPR_ARR_LITERAL,
	EXPR_GROUP,
	EXPR_IDENT
	/* many more kinds of exprs to add here */
} expr_t;

struct expr {
	/* used by all kinds of exprs */
	expr_t kind;
	struct expr *left;
	struct expr *right;
	struct expr *next;
	struct expr *mid;

	/* used by various leaf exprs */
	const char *name;
	int int_literal;
	float float_literal;
	int bool_literal;
	const char * string_literal;
	const char * char_literal;
	struct symbol *symbol;
	int group;
};

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right );
struct expr * expr_create_mid( expr_t kind, const char *n, struct expr *mid );
struct expr * expr_create_name( const char *n );
struct expr * expr_create_integer_literal( int c );
struct expr * expr_create_float_literal( float c );
struct expr * expr_create_boolean_literal( int c );
struct expr * expr_create_string_literal( const char *str );
struct expr * expr_create_char_literal( const char *c );

void expr_print( struct expr *e );
int compare_expr( struct expr *expr, struct expr *expr_next, int right );

void expr_resolve( struct scope *s, struct expr *e);

#endif