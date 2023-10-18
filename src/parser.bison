%define parse.trace

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

extern char* yytext;
extern int yylex();
extern int yyerror(char *str);

%}

%%

program     : decl_list
            |
            { return 1; }                                                                                                                                                          
            ;  

/* EXPRESSIONS */
exprs_or_e  : expr_list                                                                                  
            |                                                                                      
            ;

expr_list   : expr TOKEN_COMMA expr_list                                                                        
            | expr                                                                                             
            ;

expr_or_e   : expr                                                                                    
            |                                                                                                                                                                                       
            ;

/* parse from higher to lower level of precedence */

expr        : expr1 TOKEN_ASSIGN expr                                                                                                                                              
            | expr1                                                                                      
            ;

expr1       : expr1 TOKEN_OR expr2                                                           
            | expr2
            ;             
                                                                                            

expr2       : expr2 TOKEN_AND expr3                                                              
            | expr3
            ;   
                                                                                                      

expr3       : expr3 TOKEN_LT expr4                                                                
            | expr3 TOKEN_LE expr4                                                                   
            | expr3 TOKEN_GT expr4                                                                   
            | expr3 TOKEN_GE expr4                                                                   
            | expr3 TOKEN_EQ expr4                                                                  
            | expr3 TOKEN_NE expr4                                                                  
            | expr4                                                                                        
            ;

expr4       : expr4 TOKEN_ADD expr5                                                          
            | expr4 TOKEN_HYPHEN expr5                                                      
            | expr5                                                                             
            ;

expr5       : expr5 TOKEN_ASTERISK expr6                                                               
            | expr5 TOKEN_FSLASH expr6                                                                     
            | expr5 TOKEN_PERCENT expr6                                                                        
            | expr6                                                                                        
            ;

expr6       : expr7 TOKEN_EXPONENT expr6                                                                      
            | expr7                                                                                              
            ;

expr7       : TOKEN_HYPHEN expr7                                                                              
            | TOKEN_NOT expr7                                                                               
            | expr8                                                                                       
            ;

expr8       : expr8 TOKEN_INCREMENT                                                                 
            | expr8 TOKEN_DECREMENT                                                                
            | expr9                                                                                  
            ;

expr9       : TOKEN_LPAREN expr TOKEN_RPAREN                                                          
            | ident TOKEN_LPAREN exprs_or_e TOKEN_RPAREN                                                      
            | ident arr_idx                                                                                        
            | atomic                                                                                       
            ;

atomic      : TOKEN_TYPE_INT 
            | TOKEN_TYPE_FLOAT                                                                              
            | TOKEN_TYPE_STRING                                                                         
            | TOKEN_TYPE_CHAR                                                                            
            | TOKEN_TRUE                                                                                  
            | TOKEN_FALSE                                                                                
            | TOKEN_LBRACE expr_list TOKEN_RBRACE                                                                                       
            | ident                                                                                      
            ;

arr_idx     : TOKEN_LBRACKET expr TOKEN_RBRACKET arr_idx                                                   
            | TOKEN_LBRACKET expr TOKEN_RBRACKET                                                  
            ;

/* IDENTIFIERS  */
ident       : TOKEN_IDENT                                                                                          
            ;

/* TYPES */
type        : TOKEN_CHAR                                                                             
            | TOKEN_STRING                                                                             
            | TOKEN_INT                                                                                 
            | TOKEN_FLOAT                                                                                  
            | TOKEN_BOOL                                                                                  
            | TOKEN_AUTO                                                                                   
            | TOKEN_VOID                                                                                   
            | TOKEN_ARRAY TOKEN_LBRACKET expr_or_e TOKEN_RBRACKET type                              
            | TOKEN_FUNC type TOKEN_LPAREN params_or_e TOKEN_RPAREN                                             
            ;

/* DECLARATIONS */
decl        : ident TOKEN_COLON type TOKEN_ASSIGN TOKEN_LBRACE stmts_or_e TOKEN_RBRACE
            | ident TOKEN_COLON type TOKEN_ASSIGN expr TOKEN_SEMICOLON                                                           
            | ident TOKEN_COLON type TOKEN_SEMICOLON 
            | ident TOKEN_COLON type                                                       
            ;
            
decl_list   : decl decl_list                                                                            
            |                                                                                             
            ;

/* PARAMETERS */
params_or_e : param_list                                                                  
            |                                                                                          
            ;

param_list  : param TOKEN_COMMA param_list                                                              
            | param                                                                                     
            ;

param       : ident TOKEN_COLON type                                                            
            ;


/* STATEMENTS */
stmts_or_e  : stmt_list                                                                                  
            |                                                                                      
            ;

stmt_list   : stmt stmt_list                                                                          
            | stmt                                                                                                
            ;

stmt        : stmt1                                                                                      
            | stmt2                                                                                      
            ;

/* open statements */
stmt1       : TOKEN_FOR TOKEN_LPAREN expr_or_e TOKEN_SEMICOLON expr_or_e TOKEN_SEMICOLON expr_or_e TOKEN_RPAREN stmt1
            | TOKEN_WHILE TOKEN_LPAREN expr TOKEN_RPAREN stmt1                                   
            | TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt2 TOKEN_ELSE stmt1                                                   
            | TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt 
            ;                                                                     

/* closed statementa */
stmt2       : TOKEN_FOR TOKEN_LPAREN expr_or_e TOKEN_SEMICOLON expr_or_e TOKEN_SEMICOLON expr_or_e TOKEN_RPAREN stmt2
            | TOKEN_WHILE TOKEN_LPAREN expr TOKEN_RPAREN stmt2                                          
            | TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt2 TOKEN_ELSE stmt2                                                                                                                     
            | stmt3                                                                                    
            ;

/* single statement */
stmt3       : TOKEN_PRINT exprs_or_e TOKEN_SEMICOLON  
            | TOKEN_RETURN expr_or_e TOKEN_SEMICOLON                                                         
            | TOKEN_LBRACE stmts_or_e TOKEN_RBRACE                                                     
            | expr TOKEN_SEMICOLON                                                                        
            | decl                                                                                        
            ; 

%%

int yyerror(char* str) {
    printf("parse error: %s\n", str);
    return 1;
}
 
