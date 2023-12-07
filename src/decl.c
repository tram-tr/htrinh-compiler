#include "../inc/decl.h"
#include <string.h>
#include <stdlib.h>

extern int resolve_error;
extern int typecheck_error;
extern int codegen_error;
extern FILE *fp;

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next ) {
    struct decl* d = malloc(sizeof(struct decl));
    d->name = name;
    d->type = type;
    d->value = value;
    d->code = code;
    d->symbol = 0;
    d->next = next;
    d->local_count = 0;
    d->param_count = 0;
    d->var_count = 0;
    d->has_return = 1;
    return d;
}

void decl_print( struct decl *d, int indent ) {
    if (d == 0) return;

    indent_print(indent);
    printf("%s: ", d->name);
    type_print(d->type);

    if(d->value) {
        printf(" = ");
        expr_print(d->value);
        printf(";\n");

    } else if (d->code) {
        printf(" = ");
        stmt_print(d->code, indent);

    } else printf(";\n");
    
    decl_print(d->next, indent);
}

void indent_print( int indent ) {
    for (int i = 0; i < indent; i++)
        printf("\t");
}

// create a new symbol, and then bind it to the name of the declaration in the current scope
void decl_resolve ( struct scope *s, struct decl *d ) {
    if (d == 0) return;

    symbol_t kind = scope_level(s) ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
    
    d->symbol = symbol_create(kind, d->type, d->name);
    
    // resolve expression
    expr_resolve(s, d->value);
    int bind = scope_bind(s, d->name, d->symbol);
    
    // symbol is already in the symbol table
    if (bind == 1)
        resolve_error++;

    d->symbol->which = s->which;

    // enter a scope
    // resolve parameters and code
    if (d->code != 0) {
        s = scope_enter(s);
        param_list_resolve(s, d->type->params);
        d->param_count = s->which;

        stmt_resolve(s, d->code->body);
        d->var_count = d->param_count + (s->which - d->param_count);
        d->local_count = d->var_count - d->param_count;

        s = scope_exit(s);
    }

    decl_resolve(s, d->next);
}

void decl_typecheck ( struct decl *d ) {
    if (d == 0)
        return;

    struct type *t, *l;

    // check for subtype void
    if (d->type->kind == TYPE_VOID) {
        //printf("type error: cannot declare void type\n");
        type_error_print(ERR_VOID, 0, d->value, 0, 0, 0, 0);
        typecheck_error++;
    }

    // check for subtype of type array
    if (d->symbol->type->kind == TYPE_ARRAY) {
        t = d->symbol->type;
        while (t->subtype != 0) {
            t = t->subtype;
        }
        if (t->kind == TYPE_VOID || t->kind == TYPE_FUNCTION) {
            type_error_print(ERR_BAD_ARR_TYPE, d, 0, 0, 0, 0, 0);
            typecheck_error++;
        }
    }

    if (d->value != 0) {
        // check global declarations
        if (d->symbol->kind == SYMBOL_GLOBAL) {
            switch (d->value->kind) {
                case EXPR_INT:
                case EXPR_FLOAT:
                case EXPR_STRING:
                case EXPR_CHAR:
                case EXPR_BOOL:
                case EXPR_ARR_LITERAL:
                    break;
                default:
                    type_error_print(ERR_CONST, d, 0, 0, 0, 0, 0);
                    typecheck_error++;

            }
        }

        // check if array being declared with a positive integer length
        if (d->value->kind == EXPR_ARR_LITERAL) {
            if(d->symbol->kind != SYMBOL_GLOBAL)
                type_error_print(ERR_ARR_LIT_GLOBAL, d, 0, 0, 0, 0, 0);
            d->symbol->type->arr_expr = expr_copy(d->type->arr_expr);
            struct type *literal = type_convert_arr_literal(d->value);
            if (literal != 0) {
                t = d->symbol->type;
                l = literal;
                // check for subtype
                while(t->subtype && l->subtype) {
                    // convert type auto
                    if(t->subtype->kind == TYPE_AUTO) {
                        t->subtype = type_copy(l->subtype);
                        printf("type notice: type of %s is now ", d->name);
                        type_print(d->symbol->type);
                        printf("\n");
                        break;
                    }
                    t = t->subtype;
                    l = l->subtype;
                }
            }

            d->value->type_err = type_copy(literal);
            if(type_compare(d->symbol->type, literal) == 1) {
                type_error_print(ERR_DECL_VAL, d, 0, 0, 0, 0, 0);
                typecheck_error++;
            }
        }
        else {
            // compare declared type and value type
            t = expr_typecheck(d->value);
            if(d->symbol->type->kind == TYPE_AUTO) {
                if(t->kind == TYPE_AUTO) {
                    type_error_print(ERR_AUTO, d, 0, 0, 0, 0, 0);
                    typecheck_error++;
                }

                d->symbol->type = type_copy(t);
                printf("type notice: type of %s is now ", d->name);
                type_print(t); 
                printf("\n");
            }

            d->value->type_err = type_copy(t);

            // handle auto type
            if (d->symbol->type->kind == TYPE_AUTO) {
                if(t->kind == TYPE_AUTO) {
                    /*printf("type error: cannot infer auto\n");
                    decl_print(d, 1);*/
                    type_error_print(ERR_AUTO, d, 0, 0, 0, 0, 0);
                    typecheck_error++;
                }
                d->symbol->type = type_copy(t);
                printf("type notice: type of %s is now ", d->name);
                type_print(d->symbol->type);
                printf("\n");
            }

            if(t->kind != d->symbol->type->kind) {
                type_error_print(ERR_DECL_VAL, d, 0, 0, 0, 0, 0);
                typecheck_error++;
            }
        }
        
    }

    if (param_typecheck(d->symbol->type->params) == 1) {
        type_error_print(ERR_FUNC_AUTO_PARAM, d, 0, 0, 0, 0, 0);
        typecheck_error++;
    }

    if(d->code != 0) {
        stmt_typecheck(d->code, d);

        // auto type function
        if(d->symbol->type->subtype->kind == TYPE_AUTO) {
            d->symbol->type->subtype = type_create(TYPE_VOID, 0, 0);
            printf("type notice: return type of function %s is now ", d->name);
            type_print(d->symbol->type->subtype);
            printf("\n");
        }
    }

    // check for a return if non-void
    if (d->symbol->type->kind == TYPE_FUNCTION && d->symbol->type->subtype->kind != TYPE_VOID  && d->has_return == 1 && d->code && strcmp(d->name, "main")) {
        //printf("type error: non-void function %s missing return value\n", d->name);
        type_error_print(ERR_NO_RETURN, d, 0, 0, 0, 0, 0);        
        typecheck_error++;
    }
 
    // check next
    decl_typecheck(d->next);
    return;
}

void decl_codegen(struct decl *d) {
    if (d == 0)
        return;
    
    fprintf(fp, "# DECL\n");
    // global decl
    if (d->symbol->kind == SYMBOL_GLOBAL) {
        switch (d->symbol->type->kind) {
            case TYPE_FUNCTION:
                if (d->code != 0) {
                    global_label(d);
                    decl_prologue(d);
                    stmt_codegen(d->code);
                    decl_epilogue(d);
                }
                break;
            case TYPE_STRING:
                fprintf(fp, ".data\n");
                fprintf(fp, ".global %s\n", symbol_codegen(d->symbol));
                fprintf(fp, "%s:\n", symbol_codegen(d->symbol));
                if (d->value != 0) {
                    int label = label_create();
                    fprintf(fp, ".quad %s\n", label_name(label));

                    fprintf(fp, ".data\n");
                    fprintf(fp, ".global %s\n", label_name(label));
                    fprintf(fp, "%s:\n", label_name(label));

                    fprintf(fp, ".string %s\n", d->value->string_literal); 
                }
                else fprintf(fp, ".quad 0\n");
                break;
            case TYPE_CHARACTER:
                global_label(d);
                if (d->value != 0) 
                    fprintf(fp, ".quad %d\n", d->value->char_literal[1]);
                else
                    fprintf(fp, ".quad 0\n");
                break;
            case TYPE_INTEGER:
                global_label(d);
                fprintf(fp, ".quad %d\n", (d->value ? d->value->int_literal : 0));
                break;
            case TYPE_FLOAT:
                printf("codegen error: floating not supported.\n");
                codegen_error++;
                break;
            case TYPE_BOOLEAN:
                global_label(d);
                fprintf(fp, "\t.quad %d\n", (d->value ? d->value->bool_literal : 0));
                break;
            case TYPE_AUTO:
                break;
            case TYPE_ARRAY:
                global_label(d);
                struct expr *curr = 0;
                if (d->value != 0)
                    curr = d->value->mid;
                for (int i = 0; i < d->type->arr_expr->int_literal; i++) {
                    fprintf(fp, ".quad %d\n", d->value ? curr->int_literal : 0);
                    if (curr != 0) curr = curr->next;
                }
                break;
        }

    // param and local decl
    } else {
        switch (d->symbol->type->kind) {
            case TYPE_STRING:
            case TYPE_BOOLEAN:
            case TYPE_CHARACTER:
            case TYPE_INTEGER:
                if (d->value) {
                    fprintf(fp, "# code for value of decl %s\n", d->name);
                    expr_codegen(d->value);
                    fprintf(fp, "# code for decl %s\n", d->name);
                    fprintf(fp, "\tmovq %s, %s\n", scratch_name(d->value->reg), symbol_codegen(d->symbol));
                    scratch_free(d->value->reg);
                } 
                break;
            case TYPE_FLOAT:
                printf("codegen error: floating not supported.\n");
                codegen_error++;
                break;
            case TYPE_ARRAY:
                printf("codegen error: local array declaration not supported.\n");
                codegen_error++;
                break;

            case TYPE_FUNCTION:
                printf("error: local function declaration not supported.\n");
                codegen_error++;
                break;
            case TYPE_AUTO:
                break;
        }
    }
   
   decl_codegen(d->next);
}

void global_label( struct decl *d ) {
    if (d == 0)
        return;

    if (d->symbol->type->kind == TYPE_FUNCTION) {
        fprintf(fp, ".text\n");
        fprintf(fp, ".global %s\n", d->name);
        fprintf(fp, "%s:\n", d->name);
    } else {
        fprintf(fp, ".data\n");
        fprintf(fp, ".global %s\n", d->name);
        fprintf(fp, "%s:\n", d->name);
    }
}

void decl_prologue( struct decl *d ) {
    if (d == 0) return;

    fprintf(fp, "# start of function prologue\n");
    // setup frame
    fprintf(fp, "\tpushq %%rbp\n");
    fprintf(fp, "\tmovq %%rsp, %%rbp\n\n");

    // save params
    for (int i = 0; i < d->param_count; i++) 
        fprintf(fp, "\tpushq %s\n", arg_name(i));
    
    // allocate space for local variables
    fprintf(fp, "\tsubq $%d, %%rsp\n", d->local_count * 8 );

    // save callee saved registers
    fprintf(fp, "\tpushq %%rbx\n");
    fprintf(fp, "\tpushq %%r12\n");
    fprintf(fp, "\tpushq %%r13\n");
    fprintf(fp, "\tpushq %%r14\n");
    fprintf(fp, "\tpushq %%r15\n");
    fprintf(fp, "# end of functiop prologue\n");
}

void decl_epilogue( struct decl *d ){
    fprintf(fp, "# start of function epilogue\n");
    fprintf(fp, ".%s_epilogue:\n", d->name);

    // restore callee-saved registers
    fprintf(fp, "\tpopq %%r15\n");
    fprintf(fp, "\tpopq %%r14\n");
    fprintf(fp, "\tpopq %%r13\n");
    fprintf(fp, "\tpopq %%r12\n");
    fprintf(fp, "\tpopq %%rbx\n");
    
    // restore frame
    // reset stack to base pointer
    fprintf(fp, "\tmovq %%rbp, %%rsp\n");
    // restore the old base pointer
    fprintf(fp, "\tpopq %%rbp\n");
    // return to caller
    fprintf(fp, "\tret\n");

    fprintf(fp, "# end of function epilogue\n");
}