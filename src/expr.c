#include "../inc/expr.h"

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right ) {
    struct expr* e = malloc(sizeof(struct expr));
    e->kind = kind;
    e->left = left;
    e->right = right;
    return e;
}

struct expr * expr_create_name( const char *n ) {
    struct expr* e = expr_create(EXPR_IDENT, NULL, NULL);
    e->name = n; 
    return e;
}

struct expr * expr_create_integer_literal( int c ) {
    struct expr* e = expr_create(EXPR_INT, NULL, NULL);
    e->int_literal = c;
}

struct expr * expr_create_float_literal( float c ) {
    struct expr* e = expr_create(EXPR_FLOAT, NULL, NULL);
    e->float_literal = c;
}

struct expr * expr_create_boolean_literal( int c ) {
    struct expr* e = expr_create(EXPR_BOOL, NULL, NULL);
    e->bool_literal = c;
}

struct expr * expr_create_char_literal( const char* c ) {
    struct expr* e = expr_create(EXPR_CHAR, NULL, NULL);
    e->char_literal = c;
    return e;
}

struct expr * expr_create_string_literal( const char *str ) {
    struct expr* e = expr_create(EXPR_STRING, NULL, NULL);
    e->string_literal = str;
    return e;
}