#include "../inc/type.h"
#include "../inc/expr.h"
#include <stdlib.h>

extern int typecheck_error;

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params ) {
    struct type* t = malloc(sizeof(struct type));
    t->kind = kind;
    t->params = params;
    t->subtype = subtype;
    t->arr_expr = 0;
    return t;
}

void type_print( struct type *t ) {
    if (t == 0) return;
    switch (t->kind) {
        case TYPE_VOID:
            printf("void");
            break;
        case TYPE_BOOLEAN:
            printf("boolean");
            break;
        case TYPE_CHARACTER:
            printf("char");
            break;
        case TYPE_INTEGER:
            printf("integer");
            break;
        case TYPE_FLOAT:
            printf("float");
            break;
        case TYPE_STRING:
            printf("string");
            break;
        case TYPE_ARRAY:
            printf("array[");
            expr_print(t->arr_expr);
            printf("] ");
            type_print(t->subtype);
            break;
        case TYPE_FUNCTION:
            printf("function ");
            type_print(t->subtype);
            printf(" (");
            if (t->params != 0) {
                printf(" ");
                param_list_print(t->params);
                printf(" ");
            }
            printf(")");
            break;
        case TYPE_AUTO:
            printf("auto");
            break;
    }
}

int type_compare(struct type *a, struct type *b) {
    if (!a && !b) return 0;
    if (!a || !b) return 1;

    if(a->kind == b->kind && type_compare(a->subtype, b->subtype) == 0 && param_compare(a->params, b->params) == 0 && expr_value_compare(a->arr_expr, b->arr_expr) == 0) 
        return 0;
    else return 1;
    
}

struct type * type_copy(struct type *t) {
    if(t == 0) 
        return 0;

    // copy subtype and copy arr_expr
    struct type *subtype = type_copy(t->subtype);
    if(subtype != 0) 
        subtype->arr_expr = expr_copy(t->subtype->arr_expr);

    // copy base type and copy arr_expr
    struct param_list * a = param_copy(t->params);
    struct type *base = type_create(t->kind, subtype, a);
    if (base != 0) 
        base->arr_expr = expr_copy(t->arr_expr);
 
    return base;
}

void type_delete( struct type *t ) {
    if (t == 0)
        return;
    free(t);
}

// convert type of array literal to array type
struct type * type_convert_arr_literal( struct expr *e ) {
    if (e == 0)
        return 0;

    if (e->kind != EXPR_ARR_LITERAL)
        return expr_typecheck(e);

    int p = 0;
    struct type *first;
    struct expr *curr = e->mid;

    while (curr) {
        if (p == 0)
            first = expr_typecheck(e->mid);
        else {
            if (type_compare(first, expr_typecheck(curr)) == 1) {
                type_error_print(ERR_ARR_LIT_TYPE, 0, e, 0, 0, 0, 0);
                typecheck_error++;
            }
        }

        curr = curr->next;
        p++;
    }


    // convert into an array
    struct type *t = type_create(TYPE_ARRAY, type_convert_arr_literal(e->mid), 0);
    t->arr_expr = expr_create_integer_literal(p);
    
    return t; 
}

void type_error_print(error_t err, struct decl *d, struct expr *e1, struct expr *e2, struct param_list *a, struct stmt *s, struct type *t) {
    if (d == 0 && e1 == 0 & e2 == 0 && a == 0 && s == 0 && t == 0)
        return;

    struct decl *dt;
    struct expr *et;

    printf("type error: ");

    if (d != 0) {
        dt = d->next;
        d->next = 0;
    }

    switch (err) {
        case ERR_NOT:
            printf("cannot perform logical operator for non-boolean values\n\t\t\t");
            expr_print(e1);
            printf(": ");
            expr_print(e1->right);
            printf(" is type of ");
            type_print(e1->right->type_err);
            printf("\n");
            break;

        case ERR_NEG:
            printf("cannot use negation operator for non-numeric values\n\t\t\t");
            expr_print(e1);
            printf(": ");
            expr_print(e1->right);
            printf(" is type of ");
            type_print(e1->right->type_err);
            printf("\n");
            break;

        case ERR_INC:
            printf("cannot use increment operator for non-integer values\n\t\t\t");
            expr_print(e1);
            printf(": ");
            expr_print(e1->left);
            printf(" is type of ");
            type_print(e1->left->type_err);
            printf("\n");
            break;

        case ERR_DEC:
            printf("cannot use decrement operator for non-integer values\n\t\t\t");
            expr_print(e1);
            printf(": ");
            expr_print(e1->left);
            printf(" is of type of ");
            type_print(e1->left->type_err);
            printf("\n");
            break;

        case ERR_MATH:
            printf("cannot perform arithmetic operation\n\t\t\t");
            type_error_print_math(e1);
            printf("\n");
            break;

        case ERR_COMP:
            printf("cannot compare different types\n\t\t\t");
            expr_print(e1);
            printf(": ");
            expr_print(e1->left);
            printf(" is type of ");
            type_print(e1->left->type_err);
            printf(" but ");
            expr_print(e1->right);
            printf(" is type of ");
            type_print(e1->right->type_err);
            printf("\n");
            break;

        case ERR_COMP_INVALID_TYPE:
            printf("cannot comapre type of void, function, or array\n\t\t\t");
            expr_print(e1);
            printf(": ");
            expr_print(e1->left);
            printf(" is type of ");
            type_print(e1->left->type_err);
            printf(" but ");
            expr_print(e1->right);
            printf(" is type of ");
            type_print(e1->right->type_err);
            printf("\n");
            break;

        case ERR_LOGIC:
            printf("cannot perform logical operator for non-boolean values\n\t\t\t");
            expr_print(e1);
            printf(": ");
            expr_print(e1->left);
            printf(" is type of ");
            type_print(e1->left->type_err);
            printf(" but ");
            expr_print(e1->right);
            printf(" is type of ");
            type_print(e1->right->type_err);
            printf("\n");
            break;

        case ERR_ASSIGN:
            printf("cannot assign different types\n");
            printf("\t\t\tcannot assign ");
            expr_print(e1->right);
            printf(" which has type of ");
            type_print(e1->right->type_err);
            printf(" to ");
            expr_print(e1->left);
            printf(" which has type of ");
            type_print(e1->left->type_err);
            printf("\n");
            break;

        case ERR_ASSIGN_LEFT:
            printf("invalid assignment\n\t\t\t");
            expr_print(e1);
            printf(" can only assign to idents or array expressions, but ");
            expr_print(e1->left);
            printf(" is type of ");
            type_print(e1->left->type_err);
            printf("\n");
            break;

        case ERR_FUNC_MISSING_ARGS:
            printf("function call does not have enough arguments\n\t\t\t");
            printf("expecting %s(", e1->name);
            param_list_print(a);
            printf(") but received ");
            et = e1;
            while(et) {
                expr_print(et);
                if(et->next)
                    printf(", ");
                et = et->next;
            }
            printf("\n");
            break;

        case ERR_FUNC_MANY_ARGS:
            printf("function call has too many arguments\n\t\t\t");
            printf("expecting %s(", e1->name);
            param_list_print(a);
            printf(") but received ");
            et = e1;
            while(et) {
                expr_print(et);
                if(et->next)
                    printf(", ");
                et = et->next;
            }
            printf("\n");
            break;

        case ERR_FUNC_PARAM:
            printf("params do not match with definition\n\t\t\t");
            expr_print(e1);
            printf(": %s expects type of ", e1->name);
            type_print(a->type);
            printf(" but received ");
            expr_print(a->err);
            printf(" is a ");
            type_print(expr_typecheck(a->err));
            printf("\n");
            break;

        case ERR_VOID:
            printf("cannot declare void type\n\t\t\t");
            expr_print(e1);
            printf("\n");
            break;

        case ERR_CONST:
            printf("cannot declare non-literal value in global scope\n");
            decl_print(d, 3);
            break;

        case ERR_AUTO:
            printf("cannot infer auto type\n");
            decl_print(d, 3);
            break;

        case ERR_NO_RETURN:
            printf("non-void function must have return\n");
            printf("\t\t\tfunction %s is declared with return type ", d->name);
            type_print(d->type->subtype);
            printf("\n");
            break;

        case ERR_ARR_LIT_TYPE:
            printf("array literals do not match\n\t\t\t");
            expr_print(e1);
            printf("\n");
            break;

        case ERR_FUNC_TYPE:
            printf("return type does not match function declaration type\n");
            printf("\t\t\tfunction %s expects return type of ", d->name);
            type_print(d->symbol->type->subtype);
            printf(" but received type of ");
            if (t != 0) 
                type_print(t); 
            else printf("void");
            printf("\n");
            break;

        case ERR_FUNC_AUTO_PARAM:
            printf("param list cannot have type auto\n");
            s = d->code;
            d->code = 0;
            decl_print(d, 3);
            d->code = s;
            break;

        case ERR_WHILE_COND:
            printf("while statement cannot have non-boolean condition\n");
            printf("\t\t\twhile (");
            expr_print(e1);
            printf("): expects a boolean expression but received type of ");
            type_print(t);
            printf("\n");
            break;

        case ERR_FOR_COND:
            printf("for statement must have a boolean condition\n");
            printf("\t\t\tfor (");
            expr_print(s->init_expr);
            printf(";");
            expr_print(s->expr);
            printf(";");
            expr_print(s->next_expr);
            printf("): expects the second expression to be boolean but received type of ");
            type_print(t);
            printf("\n");
            break;

        case ERR_IF_COND:
            printf("if statement cannot have non-boolean condition\n");
            printf("\t\t\tif (");
            expr_print(e1);
            printf("): expects a boolean expression but received type of ");
            type_print(t);
            printf("\n");
            break;

        case ERR_DECL_VAL:
            printf("declaraction type and value type do not match\n");
            printf("\t\t\tcannot assign %s which is of type ", d->name);
            type_print(d->type);
            printf(" to ");
            expr_print(d->value);
            printf(" which is of type ");
            type_print(d->value->type_err);
            printf("\n");
            break;

        case ERR_NON_ARR:
            printf("cannot index non-arrays\n\t\t\t");
            expr_print(e1);
            printf("\n");
            break;

        case ERR_ARR_INDEX:
            printf("index of array must be an integer\n\t\t\t");
            expr_print(e1);
            printf(":");
            expr_print(e2);
            printf(" should be of type integer, but is of type ");
            type_print(e2->type_err);
            printf("\n");
            break;

        case ERR_BAD_ARR_TYPE:
            printf("array cannot have type of void or function\n");
            decl_print(d, 3);
            break;

        case ERR_ARR_LIT_GLOBAL:
            printf("only global arrays may have an initializer\n");
            decl_print(d, 3);

        default:
            break;
    }

    if(d != 0)
        d->next = dt;
}

void type_error_print_math(struct expr *e) {
    expr_print(e);
    printf(": cannot ");
    switch (e->kind) {
        case EXPR_ADD:
            printf("add ");
            expr_print(e->right);
            printf(" which is of type ");
            type_print(expr_typecheck(e->right));
            printf(" to ");
            expr_print(e->left);
            printf(" which is of type ");
            type_print(expr_typecheck(e->left));
            break;

        case EXPR_SUB:
            printf("subtract ");
            expr_print(e->right);
            printf(" which is of type ");
            type_print(expr_typecheck(e->right));
            printf(" from ");
            expr_print(e->left);
            printf(" which is of type ");
            type_print(expr_typecheck(e->left));
            break;

        case EXPR_MUL:
            printf("multiply ");
            expr_print(e->left);
            printf(" which is of type ");
            type_print(expr_typecheck(e->left));
            printf(" by ");
            expr_print(e->right);
            printf(" which is of type ");
            type_print(expr_typecheck(e->right));
            break;

        case EXPR_DIV:
            printf("divide ");
            expr_print(e->left);
            printf(" which is of type ");
            type_print(expr_typecheck(e->left));
            printf(" by ");
            expr_print(e->right);
            printf(" which is of type ");
            type_print(expr_typecheck(e->right));
            break;

        case EXPR_MOD:
            printf("mod ");
            expr_print(e->left);
            printf(" which is of type ");
            type_print(expr_typecheck(e->left));
            printf(" by ");
            expr_print(e->right);
            printf(" which is of type ");
            type_print(expr_typecheck(e->right));
            break;

        case EXPR_EXP:
            printf("raise ");
            expr_print(e->left);
            printf(" which is of type ");
            type_print(expr_typecheck(e->left));
            printf(" to the power of ");
            expr_print(e->right);
            printf(" which is of type ");
            type_print(expr_typecheck(e->right));
            break;

        default:
            break;
    }
}