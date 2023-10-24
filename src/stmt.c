#include "../inc/stmt.h"
#include <stdlib.h>

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
    s->else_if = 0;
    return s; 
}

void stmt_print( struct stmt *s, int indent ) {
    if (s == 0) return;
    if (s->else_if == 0 && s->kind != STMT_DECL) {
        for (int i = 0; i < indent; i++)
            printf("\t");
    }
    switch (s->kind) {
        case STMT_DECL:
            decl_print(s->decl, indent);

            stmt_print(s->next, indent);
            break;

        case STMT_EXPR:
            expr_print(s->expr);
            printf(";\n");
            
            stmt_print(s->next, indent);
            break;

        case STMT_IF_ELSE:
            printf("if (");
            expr_print(s->expr);
            printf(") \n");

            if (s->body->kind == STMT_BLOCK) stmt_print(s->body, indent);
            else stmt_print(s->body, indent + 1);

            for (int i = 0; i < indent; i++)
                printf("\t");
                  
            if (s->else_body) {
                printf("else ");
                if (s->else_body->kind == STMT_IF_ELSE) {
                    s->else_body->else_if = 1;
                    stmt_print(s->else_body, indent); 
                } else {
                    printf("\n");
                    if (s->else_body->kind == STMT_BLOCK) stmt_print(s->else_body, indent);
                    else stmt_print(s->else_body, indent + 1);
                } 
            } else printf("\n");

            stmt_print(s->next, indent);
            break;

        case STMT_FOR:
            printf("for ("); 
            expr_print(s->init_expr);
            printf(";");
            expr_print(s->expr);
            printf(";");
            expr_print(s->next_expr);
            printf(") ");
            
            if(s->body) {
                if (s->else_body->kind == STMT_BLOCK) stmt_print(s->else_body, indent);
                else stmt_print(s->else_body, indent + 1);
            }
            
            stmt_print(s->next, indent);
            break;

        case STMT_WHILE:
            printf("while (");
            expr_print(s->expr);
            printf(") ");

            if(s->body) {
                if (s->else_body->kind == STMT_BLOCK) stmt_print(s->else_body, indent);
                else stmt_print(s->else_body, indent + 1);
            } 
            
            stmt_print(s->next, indent);
            break;

        case STMT_PRINT:
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
            printf("return ");
            expr_print(s->expr);
            printf(";\n");

            stmt_print(s->next, indent);
            break;
        
        case STMT_BLOCK:
            printf("{\n");
            stmt_print(s->body, indent + 1);
            for (int i = 0; i < indent; i++)
                printf("\t");
                
            printf("}\n");
            break;

    }
}