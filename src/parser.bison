%define parse.trace

%token TOKEN_EOF 0
/* types */
%token TOKEN_IDENT
%token TOKEN_TYPE_INT
%token TOKEN_TYPE_FLOAT        
%token TOKEN_TYPE_STRING
%token TOKEN_TYPE_CHAR

/* keywords */
%token TOKEN_ARRAY
%token TOKEN_AUTO
%token TOKEN_BOOL
%token TOKEN_CHAR
%token TOKEN_ELSE
%token TOKEN_FALSE
%token TOKEN_FLOAT
%token TOKEN_FOR
%token TOKEN_FUNC
%token TOKEN_IF
%token TOKEN_INT
%token TOKEN_PRINT
%token TOKEN_RETURN
%token TOKEN_STRING
%token TOKEN_TRUE
%token TOKEN_VOID
%token TOKEN_WHILE

/* symbols */
%token TOKEN_AND
%token TOKEN_OR
%token TOKEN_EQ
%token TOKEN_LE
%token TOKEN_GE
%token TOKEN_LT
%token TOKEN_GT
%token TOKEN_NE
%token TOKEN_NOT

%token TOKEN_COLON
%token TOKEN_SEMICOLON
%token TOKEN_COMMA
%token TOKEN_LBRACKET
%token TOKEN_RBRACKET
%token TOKEN_LBRACE
%token TOKEN_RBRACE
%token TOKEN_LPAREN
%token TOKEN_RPAREN
%token TOKEN_ASSIGN
%token TOKEN_EXPONENT
%token TOKEN_INCREMENT
%token TOKEN_DECREMENT
%token TOKEN_ADD
%token TOKEN_HYPHEN
%token TOKEN_ASTERISK
%token TOKEN_FSLASH
%token TOKEN_PERCENT

%token TOKEN_SINGLE_COMMENT
%token TOKEN_BLOCK_COMMENT
%token TOKEN_ERROR 

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/decl.h"
#include "../inc/expr.h"
#include "../inc/stmt.h"
#include "../inc/symbol.h"

extern char* yytext;
extern int yylex();
extern int yyerror(char *str);
struct decl * parser_result;
%}

%union {
    struct decl *decl;
    struct expr *expr;
    struct stmt *stmt;
    struct symbol *symbol;
    struct type *type;
    struct param_list *param_list;
    char *name;
};

%type <decl> program decl_list decl
%type <expr> exprs_or_e expr_list expr_or_e expr expr1 expr2 expr3 expr4 expr5 expr6 expr7 expr8 expr9 atomic arr_idx
%type <stmt> stmts_or_e stmt_list stmt stmt1 stmt2 stmt3
%type <type> type
%type <param_list> params_or_e param_list param
%type <name> ident

%%

program     : decl_list TOKEN_EOF
            { $$ = $1; parser_result = $$; }
            ;  

/* EXPRESSIONS */
exprs_or_e  : expr_list
            { $$ = $1; }
            |
            { $$ = 0; }   
            ;

expr_list   : expr TOKEN_COMMA expr_list
            { $1->next = $3; $$ = $1; }
            | expr
            { $$ = $1; }  
            ;

expr_or_e   : expr 
            { $$ = $1; }
            |  
            { $$ = 0; }
            ;

/* parse from higher to lower level of precedence */

expr        : expr1 TOKEN_ASSIGN expr
            { $$ = expr_create(EXPR_ASSIGN, $1, $3); }
            | expr1
            { $$ = $1; }
            ;

expr1       : expr1 TOKEN_OR expr2
            { $$ = expr_create(EXPR_OR, $1, $3); }
            | expr2
            { $$ = $1; }
            ;             
 

expr2       : expr2 TOKEN_AND expr3 
            { $$ = expr_create(EXPR_AND, $1, $3); }
            | expr3
            { $$ = $1; }
            ;
           

expr3       : expr3 TOKEN_LT expr4
            { $$ = expr_create(EXPR_LT, $1, $3); }
            | expr3 TOKEN_LE expr4
            { $$ = expr_create(EXPR_LE, $1, $3); }
            | expr3 TOKEN_GT expr4
            { $$ = expr_create(EXPR_GT, $1, $3); }
            | expr3 TOKEN_GE expr4
            { $$ = expr_create(EXPR_GE, $1, $3); }
            | expr3 TOKEN_EQ expr4
            { $$ = expr_create(EXPR_EQ, $1, $3); }
            | expr3 TOKEN_NE expr4
            { $$ = expr_create(EXPR_NE, $1, $3); }
            | expr4
            { $$ = $1; }
            ;

expr4       : expr4 TOKEN_ADD expr5 
            { $$ = expr_create(EXPR_ADD, $1, $3); }
            | expr4 TOKEN_HYPHEN expr5
            { $$ = expr_create(EXPR_SUB, $1, $3); }
            | expr5 
            { $$ = $1; }
            ;

expr5       : expr5 TOKEN_ASTERISK expr6
            { $$ = expr_create(EXPR_MUL, $1, $3); }
            | expr5 TOKEN_FSLASH expr6
            { $$ = expr_create(EXPR_DIV, $1, $3); }
            | expr5 TOKEN_PERCENT expr6
            { $$ = expr_create(EXPR_MOD, $1, $3); }
            | expr6
            { $$ = $1; }
            ;

expr6       : expr7 TOKEN_EXPONENT expr6 
            { $$ = expr_create(EXPR_EXP, $1, $3); }
            | expr7
            { $$ = $1; }
            ;

expr7       : TOKEN_HYPHEN expr7
            { $$ = expr_create(EXPR_NEG, 0, $2); }
            | TOKEN_NOT expr7
            { $$ = expr_create(EXPR_NOT, 0, $2); }
            | expr8
            { $$ = $1; }
            ;

expr8       : expr8 TOKEN_INCREMENT
            { $$ = expr_create(EXPR_INC, $1, 0); }
            | expr8 TOKEN_DECREMENT
            { $$ = expr_create(EXPR_DEC, $1, 0); }
            | expr9
            { $$ = $1; }
            ;

expr9       : TOKEN_LPAREN expr TOKEN_RPAREN
            { $2->group = 1; $$ = $2; }
            | ident TOKEN_LPAREN exprs_or_e TOKEN_RPAREN 
            { $$ = expr_create_mid(EXPR_FUNC_CALL, $1, $3); }
            | ident arr_idx
            { $$ = expr_create_mid(EXPR_ARR, $1, $2); }
            | atomic
            { $$ = $1; }
            ;

atomic      : TOKEN_TYPE_INT
            { $$ = expr_create_integer_literal(atoi(yytext)); }
            | TOKEN_TYPE_FLOAT
            { $$ = expr_create_float_literal(atof(yytext)); }
            | TOKEN_TYPE_STRING
            { $$ = expr_create_string_literal(strdup(yytext)); } 
            | TOKEN_TYPE_CHAR
            { $$ = expr_create_char_literal(strdup(yytext)); }
            | TOKEN_TRUE
            { $$ = expr_create_boolean_literal(0); }
            | TOKEN_FALSE
            { $$ = expr_create_boolean_literal(1); }
            | TOKEN_LBRACE expr_list TOKEN_RBRACE
            { $$ = expr_create_mid(EXPR_ARR_LITERAL, 0, $2); }
            | ident 
            { $$ = expr_create_name($1); }
            ;

arr_idx     : TOKEN_LBRACKET expr TOKEN_RBRACKET arr_idx
            { $2->next = $4; $$ = $2; }
            | TOKEN_LBRACKET expr TOKEN_RBRACKET
            { $$ = $2; }
            ;

/* IDENTIFIERS  */
ident       : TOKEN_IDENT 
            { $$ = strdup(yytext); }
            ;

/* TYPES */
type        : TOKEN_CHAR 
            { $$ = type_create(TYPE_CHARACTER, 0, 0); }
            | TOKEN_STRING 
            { $$ = type_create(TYPE_STRING, 0, 0); }
            | TOKEN_INT
            { $$ = type_create(TYPE_INTEGER, 0, 0); }
            | TOKEN_FLOAT
            { $$ = type_create(TYPE_FLOAT, 0, 0); }
            | TOKEN_BOOL
            { $$ = type_create(TYPE_BOOLEAN, 0, 0); }
            | TOKEN_AUTO 
            { $$ = type_create(TYPE_AUTO, 0, 0); }
            | TOKEN_VOID 
            { $$ = type_create(TYPE_VOID, 0, 0); }
            | TOKEN_ARRAY TOKEN_LBRACKET expr_or_e TOKEN_RBRACKET type
            { $$ = type_create(TYPE_ARRAY, $5, 0); $$->arr_expr = $3; }
            | TOKEN_FUNC type TOKEN_LPAREN params_or_e TOKEN_RPAREN
            { $$ = type_create(TYPE_FUNCTION, $2, $4); }
            ;

/* DECLARATIONS */
decl        : ident TOKEN_COLON type TOKEN_ASSIGN TOKEN_LBRACE stmts_or_e TOKEN_RBRACE
            { $$ = decl_create($1, $3, 0, stmt_create(STMT_BLOCK, 0, 0, 0, 0, $6, 0, 0), 0); }
            | ident TOKEN_COLON type TOKEN_ASSIGN expr TOKEN_SEMICOLON
            { $$ = decl_create($1, $3, $5, 0, 0); }
            | ident TOKEN_COLON type TOKEN_SEMICOLON
            { $$ = decl_create($1, $3, 0, 0, 0); }
            | ident TOKEN_COLON type
            { $$ = decl_create($1, $3, 0, 0, 0); }
            ;
            
decl_list   : decl decl_list
            { $1->next = $2; $$ = $1; }
            | 
            { $$ = 0; }
            ;

/* PARAMETERS */
params_or_e : param_list
            { $$ = $1; }        
            |  
            { $$ = 0; }     
            ;

param_list  : param TOKEN_COMMA param_list 
            { $1->next = $3; $$ = $1; }   
            | param
            { $$ = $1; }        
            ;

param       : ident TOKEN_COLON type
            { $$ = param_list_create($1, $3, 0); }
            ;

/* STATEMENTS */
stmts_or_e  : stmt_list
            { $$ = $1; }
            |
            { $$ = 0; }
            ;

stmt_list   : stmt stmt_list
            { $1->next = $2; $$ = $1; }
            | stmt
            { $$ = $1; }
            ;

stmt        : stmt1 
            { $$ = $1; }
            | stmt2  
            { $$ = $1; }
            ;

/* open statements */
stmt1       : TOKEN_FOR TOKEN_LPAREN expr_or_e TOKEN_SEMICOLON expr_or_e TOKEN_SEMICOLON expr_or_e TOKEN_RPAREN stmt1
            { $9->no_indent = 0; $$ = stmt_create(STMT_FOR, 0, $3, $5, $7, $9, 0, 0); }
            | TOKEN_WHILE TOKEN_LPAREN expr TOKEN_RPAREN stmt1
            { $5->no_indent = 0; $$ = stmt_create(STMT_WHILE, 0, 0, $3, 0, $5, 0, 0); }
            | TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt2 TOKEN_ELSE stmt1 
            { $5->no_indent = 0; $7->else_if = 0; $$ = stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5, $7, 0); }
            | TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt 
            { $5->no_indent = 0; $$ = stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5, 0, 0); }
            ;

/* closed statementa */
stmt2       : TOKEN_FOR TOKEN_LPAREN expr_or_e TOKEN_SEMICOLON expr_or_e TOKEN_SEMICOLON expr_or_e TOKEN_RPAREN stmt2
            { $9->no_indent = 0; $$ = stmt_create(STMT_FOR, 0, $3, $5, $7, $9, 0, 0); }
            | TOKEN_WHILE TOKEN_LPAREN expr TOKEN_RPAREN stmt2
            { $5->no_indent = 0; $$ = stmt_create(STMT_WHILE, 0, 0, $3, 0, $5, 0, 0); }
            | TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt2 TOKEN_ELSE stmt2
            { $5->no_indent = 0; $7->else_if = 0; $$ = stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5, $7, 0); }
            | stmt3
            { $$ = $1; }
            ;

/* single statement */
stmt3       : TOKEN_PRINT exprs_or_e TOKEN_SEMICOLON
            { $$ = stmt_create(STMT_PRINT, 0, 0, $2, 0, 0, 0, 0); }
            | TOKEN_RETURN expr_or_e TOKEN_SEMICOLON
            { $$ = stmt_create(STMT_RETURN, 0, 0, $2, 0, 0, 0, 0); }
            | TOKEN_LBRACE stmts_or_e TOKEN_RBRACE   
            { $$ = stmt_create(STMT_BLOCK, 0, 0, 0, 0, $2, 0, 0); }
            | expr TOKEN_SEMICOLON
            { $$ = stmt_create(STMT_EXPR, 0, 0, $1, 0, 0, 0, 0); }
            | decl
            { $$ = stmt_create(STMT_DECL, $1, 0, 0, 0, 0, 0, 0); }
            ; 
%%

int yyerror(char* str) {
    printf("parse error: %s\n", str);
    return 1;
}
 
