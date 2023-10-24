#include "../inc/expr.h"
#include <stdlib.h>

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right ) {
    struct expr* e = malloc(sizeof(struct expr));
    if (kind == EXPR_FUNC || kind == EXPR_ARRAY) {
        e = left;
        e->kind = kind;
        e->mid = right;
    }
    else {
        e->kind = kind;
        if (e->kind == EXPR_GROUP) e->mid = right;
        else {
            e->left = left;
            e->right = right;
        }
    }
    return e;
}

struct expr * expr_create_name( const char *n ) {
    struct expr* e = expr_create(EXPR_IDENT, 0, 0);
    e->name = n; 
    return e;
}

struct expr * expr_create_integer_literal( int c ) {
    struct expr* e = expr_create(EXPR_INT, 0, 0);
    e->int_literal = c;
}

struct expr * expr_create_float_literal( float c ) {
    struct expr* e = expr_create(EXPR_FLOAT, 0, 0);
    e->float_literal = c;
}

struct expr * expr_create_boolean_literal( int c ) {
    struct expr* e = expr_create(EXPR_BOOL, 0, 0);
    e->bool_literal = c;
}

struct expr * expr_create_char_literal( const char *c ) {
    struct expr* e = expr_create(EXPR_CHAR, 0, 0);
    e->char_literal = c;
    return e;
}

struct expr * expr_create_string_literal( const char *str ) {
    struct expr* e = expr_create(EXPR_STRING, 0, 0);
    e->string_literal = str;
    return e;
}

void expr_print( struct expr *e ) {
    if (e == 0) return;
    if(e->left != 0) {
        if(compare_expr(e, e->left, 1) == 0) {
            printf("(");
            expr_print(e->left);
            printf(")");
        } else expr_print(e->left);
    }
    struct expr *t = 0;
    switch (e->kind) {
        case EXPR_ASSIGN:
            printf("=");
            break;
        case EXPR_AND:
            printf("&&");
            break;
        case EXPR_OR:
            printf("||");
            break;
        case EXPR_EQ:
            printf("==");
            break;
        case EXPR_LE:
            printf("<=");
            break;
        case EXPR_GE:
            printf(">=");
            break;
        case EXPR_LT:
            printf("<");
            break;
        case EXPR_GT:
            printf(">");
            break;
        case EXPR_NE:
            printf("!=");
            break;
        case EXPR_NOT:
            printf("!");
            break;
        case EXPR_NEG:
            printf("-");
            break;
        case EXPR_INC:
            printf("++");
            break;
        case EXPR_DEC:
            printf("--");
            break;
        case EXPR_ADD:
            printf("+");
            break;
        case EXPR_SUB:
            printf("-");
            break;
        case EXPR_MUL:
            printf("*");
            break;
        case EXPR_MOD:
            printf("%");
            break;
        case EXPR_EXP:
            printf("^");
            break;
        case EXPR_DIV:
            printf("/");
            break;
        case EXPR_INT:
            printf("%d", e->int_literal);
            break;
        case EXPR_FLOAT:
            printf("%f", e->float_literal);
            break;
        case EXPR_BOOL:
            if (e->bool_literal == 0)   printf("true");
            else printf("false");
            break;
        case EXPR_CHAR:
            printf("%c", e->char_literal);
            break;
        case EXPR_STRING:
            printf("%s", e->string_literal);
            break;
        case EXPR_FUNC:
            printf("%s(", e->name);
            t = e->mid;
            while(t) {
                expr_print(t);
                if(t->next)
                    printf(", ");
                t = t->next;
            }
            printf(")");
            break;
        case EXPR_ARRAY:
            printf("%s", e->name);
            t = e->mid;
            while(t) {
                printf("[");
                expr_print(t);
                if(t->next)
                    printf("]");
                t = t->next;
            }
            printf("]");
            break;
        case EXPR_ARRAY_LITERAL:
            printf("{");
            expr_print(e->mid);
            t = e->next->mid;
            while(t) {
                printf(", ");
                expr_print(t);
                t = t->next;
            };
            printf("}");
            break;
        case EXPR_GROUP:
            expr_print(e->mid);
            break;
        case EXPR_IDENT:
            printf("%s", e->name);
            break;
    }
    if(e->right != 0) {
        if(compare_expr(e, e->right, 0) == 0) {
            printf("(");
            expr_print(e->right);
            printf(")");
        } else expr_print(e->right);
    }
}

int compare_expr( struct expr *expr, struct expr *expr_next, int right ) {
    if(expr_next->kind == EXPR_ASSIGN || expr_next->kind == EXPR_EXP)
        right = !right;
    
    if(expr_next->kind/10 > expr->kind/10) return 1;

    if(expr_next->kind/10 == expr->kind/10) {
        if (expr_next->group && right == 0) return 1;
        else return 0;
    }
    else return 1;
}