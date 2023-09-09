#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_EOF = 0,      // end of string
    TOKEN_SINGLE_COMMENT,
    TOKEN_BLOCK_COMMENT,

    /* types */
    TOKEN_IDENT,        // identifiers
    TOKEN_TYPE_INT,
    TOKEN_TYPE_FLOAT,        
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_CHAR,

    /* keywords */
    TOKEN_ARRAY,
    TOKEN_AUTO,
    TOKEN_BOOL,
    TOKEN_CHAR,
    TOKEN_ELSE,
    TOKEN_FALSE,
    TOKEN_FLOAT,
    TOKEN_FOR,
    TOKEN_FUNC,
    TOKEN_IF,
    TOKEN_INT,
    TOKEN_PRINT,
    TOKEN_RETURN,
    TOKEN_STRING,
    TOKEN_TRUE,
    TOKEN_VOID,
    TOKEN_WHILE,

    /* symbols */
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_EQ,
    TOKEN_LE,
    TOKEN_GE,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_NE,
    TOKEN_NOT,

    TOKEN_TERNARY,
    TOKEN_COLON,
    TOKEN_SEMICOLON,
    TOKEN_COMMA,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_ASSIGN,
    TOKEN_EXPONENT,
    TOKEN_INCREMENT,
    TOKEN_DECREMENT,
    TOKEN_ADD,
    TOKEN_HYPHEN,
    TOKEN_ASTERISK,
    TOKEN_FSLASH,
    TOKEN_PERCENT,

    TOKEN_ERROR,        // errors
} token_t;

#endif

