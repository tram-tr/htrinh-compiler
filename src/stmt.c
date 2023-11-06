#include "../inc/stmt.h"
#include <stdlib.h>

extern int resolve_error;

struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next ) {
    struct stmt * s = malloc(sizeof(struct stmt));
    s->kind = kind;
    s->decl = decl;
    s->init_expr = init_expr;
    s->expr = expr;
    s->next_expr = next_expr;
    s->body = body;
    s->else_body = else_body;
    s->next = next;
    s->else_if = 1;
    s->no_indent = 1;
    s->in_func = 1;
    return s; 
}

void stmt_print( struct stmt *s, int indent ) {
    if (s == 0) return;
    switch (s->kind) {
        case STMT_DECL:
            decl_print(s->decl, indent);
            stmt_print(s->next, indent);
            break;

        case STMT_EXPR:
            indent_print(indent);
            expr_print(s->expr);
            printf(";\n");
            stmt_print(s->next, indent);
            break;

        case STMT_IF_ELSE:
            if (s->else_if == 1) indent_print(indent);
            
            printf("if (");
            expr_print(s->expr);
            printf(") ");

            if (s->body->kind == STMT_BLOCK) stmt_print(s->body, indent);
            else {
                printf("{\n");
                stmt_print(s->body, indent + 1);
                indent_print(indent);
                printf("}\n");
            }
                  
            if (s->else_body != 0) {
                indent_print(indent);
                printf("else ");
                if (s->else_body->kind == STMT_BLOCK) stmt_print(s->else_body, indent);
                else {
                    if (s->else_body->kind == STMT_IF_ELSE) stmt_print(s->else_body, indent);
                    else {
                        printf("{\n");
                        stmt_print(s->else_body, indent + 1);
                        indent_print(indent);
                        printf("}\n");
                    }
                }
            }

            stmt_print(s->next, indent);
            break;

        case STMT_FOR:
            indent_print(indent);
            printf("for ("); 
            if (s->init_expr != 0) expr_print(s->init_expr);
            printf(";");
            if (s->expr != 0) {
                printf(" ");
                expr_print(s->expr);
            }
            printf(";");
            if (s->next_expr != 0) {
                printf(" ");
                expr_print(s->next_expr);
            }
            printf(") ");
            
            if(s->body) {
                if (s->body->kind == STMT_BLOCK) stmt_print(s->body, indent);
                else {
                    printf("{\n");
                    stmt_print(s->body, indent + 1);
                    indent_print(indent);
                    printf("}\n");
                }
            }
            
            stmt_print(s->next, indent);
            break;

        case STMT_WHILE:
            indent_print(indent);
            printf("while (");
            expr_print(s->expr);
            printf(") ");

            if(s->body) {
                if (s->body->kind == STMT_BLOCK) stmt_print(s->body, indent);
                else {
                    printf("{\n");
                    stmt_print(s->body, indent + 1);
                    indent_print(indent);
                    printf("}\n");
                }
            } 
            
            stmt_print(s->next, indent);
            break;

        case STMT_PRINT:
            indent_print(indent);
            printf("print ");
            struct expr *t = s->expr;
            while(t) {
                expr_print(t);
                if(t->next)
                    printf(", ");
                t = t->next;
            }
            printf(";\n");

            stmt_print(s->next, indent);
            break;

        case STMT_RETURN:
            indent_print(indent);
            printf("return ");
            expr_print(s->expr);
            printf(";\n");

            stmt_print(s->next, indent);
            break;
        
        case STMT_BLOCK:
            if(s->else_if == 0 || s->no_indent == 0); 
            else indent_print(indent);
            
            printf("{\n");
            stmt_print(s->body, indent + 1);
            indent_print(indent);
            printf("}\n");

            stmt_print(s->next, indent);
            break;

    }
}

// call the appropriate resolve on each of its sub-components
void stmt_resolve( struct scope *sc, struct stmt *st) {
    if (st == 0) return;
    int which;
    switch (st->kind) {
        case STMT_DECL:
            decl_resolve(sc, st->decl);
            break;

        case STMT_IF_ELSE:
            expr_resolve(sc, st->expr);
            stmt_resolve(sc, st->body);
            stmt_resolve(sc, st->else_body);
            break;

        case STMT_FOR:
            expr_resolve(sc, st->init_expr);
            expr_resolve(sc, st->expr);
            expr_resolve(sc, st->next_expr);
            stmt_resolve(sc, st->body);
            break;

        case STMT_WHILE:
            expr_resolve(sc, st->expr);
            stmt_resolve(sc, st->body);
            break;

        // enter and leave a new scope
        case STMT_BLOCK:
            which = sc->which;
            if (st->in_func == 0)
                stmt_resolve(sc, st->body);
            else {
                scope_enter(sc);
                sc->next->which = which;
                stmt_resolve(sc->next, st->body);
                sc->which = sc->next->which;
                scope_exit(sc->next);
            }
            break;

        case STMT_EXPR:
        case STMT_PRINT:
        case STMT_RETURN:
            expr_resolve(sc, st->expr);
            break;
    }

    stmt_resolve(sc, st->next);
}