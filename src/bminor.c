#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../inc/encoder.h" 
#include "../inc/parser.h" 
#include "../inc/decl.h"
#include "../inc/hash_table.h"

extern FILE *yyin;
extern int yylex();
extern char *yytext;
extern int yyparse();
extern int yydebug;
typedef enum yytokentype token_t;
extern struct decl* parser_result;
int resolve_error;

#define TOKEN_EOF 0

void print_usage(const char *program) {
    printf("usage: %s [options]\n", program);
    printf("options:\n");
    printf("\t--encode input.bminor\n");
    printf("\t--scan input.bminor\n");
    printf("\t--parse input.bminor\n");
    printf("\t--print input.bminor\n");
    printf("\t--resolve input.bminor\n");
}

int scan() {
    int flag = 0;
    while (1) {
        token_t t = yylex();
        switch (t){
        case TOKEN_SINGLE_COMMENT:
            printf("%-20s %20s\n", "SINGLE_COMMENT", yytext);
            break;
        case TOKEN_BLOCK_COMMENT:
            printf("%-20s %20s\n", "BLOCK_COMMENT", yytext);
            break;
        case TOKEN_ARRAY:
            printf("%-20s %20s\n", "ARRAY", yytext);
            break;
        case TOKEN_AUTO:
            printf("%-20s %20s\n", "AUTO", yytext);
            break;
        case TOKEN_BOOL:
            printf("%-20s %20s\n", "BOOL", yytext);
            break;
        case TOKEN_CHAR:
            printf("%-20s %20s\n", "CHAR", yytext);
            break;
        case TOKEN_ELSE:
            printf("%-20s %20s\n", "ELSE", yytext);
            break;
        case TOKEN_FALSE:
            printf("%-20s %20s\n", "BOOLEAN_LITERAL", yytext);
            break;
        case TOKEN_FOR:
            printf("%-20s %20s\n", "FOR", yytext);
            break;
        case TOKEN_FUNC:
            printf("%-20s %20s\n", "FUNCTION", yytext);
            break;
        case TOKEN_IF:
            printf("%-20s %20s\n", "IF", yytext);
            break;
        case TOKEN_INT:
            printf("%-20s %20s\n", "INT", yytext);
            break;
        case TOKEN_PRINT:
            printf("%-20s %20s\n", "PRINT", yytext);
            break;
        case TOKEN_RETURN:
            printf("%-20s %20s\n", "RETURN", yytext);
            break;
        case TOKEN_STRING:
            printf("%-20s %20s\n", "STRING", yytext);
            break;
        case TOKEN_TRUE:
            printf("%-20s %20s\n", "BOOLEAN_LITERAL", yytext);
            break;
        case TOKEN_VOID:
            printf("%-20s %20s\n", "VOID", yytext);
            break;
        case TOKEN_WHILE:
            printf("%-20s %20s\n", "WHILE", yytext);
            break;
        case TOKEN_AND:
            printf("%-20s %20s\n", "AND", yytext);
            break;
        case TOKEN_OR:
            printf("%-20s %20s\n", "OR", yytext);
            break;
        case TOKEN_EQ:
            printf("%-20s %20s\n", "EQ", yytext);
            break;
        case TOKEN_LE:
            printf("%-20s %20s\n", "LE", yytext);
            break;
        case TOKEN_GE:
            printf("%-20s %20s\n", "GE", yytext);
            break;
        case TOKEN_LT:
            printf("%-20s %20s\n", "LT", yytext);
            break;
        case TOKEN_GT:
            printf("%-20s %20s\n", "GT", yytext);
            break;
        case TOKEN_NE:
            printf("%-20s %20s\n", "NE", yytext);
            break;
        case TOKEN_NOT:
            printf("%-20s %20s\n", "NOT", yytext);
            break;
        case TOKEN_COLON:
            printf("%-20s %20s\n", "COLON", yytext);
            break;
        case TOKEN_SEMICOLON:
            printf("%-20s %20s\n", "SEMICOLON", yytext);
            break;
        case TOKEN_COMMA:
            printf("%-20s %20s\n", "COMMA", yytext);
            break;
        case TOKEN_LBRACKET:
            printf("%-20s %20s\n", "LEFT_BRACKET", yytext);
            break;
        case TOKEN_RBRACKET :
            printf("%-20s %20s\n", "RIGHT_BRACKET", yytext);
            break;
        case TOKEN_LBRACE:
            printf("%-20s %20s\n", "LEFT_BRACE", yytext);
            break;
        case TOKEN_RBRACE:
            printf("%-20s %20s\n", "RIGHT_BRACE", yytext);
            break;
        case TOKEN_LPAREN:
            printf("%-20s %20s\n", "LEFT_PARENTHESIS", yytext);
            break;
        case TOKEN_RPAREN:
            printf("%-20s %20s\n", "RIGHT_PARENTHESIS", yytext);
            break;
        case TOKEN_ASSIGN:
            printf("%-20s %20s\n", "ASSIGN", yytext);
            break;
        case TOKEN_EXPONENT:
            printf("%-20s %20s\n", "EXPONENT", yytext);
            break;
        case TOKEN_INCREMENT:
            printf("%-20s %20s\n", "INCREMENT", yytext);
            break;
        case TOKEN_DECREMENT:
            printf("%-20s %20s\n", "DECREMENT", yytext);
            break;
        case TOKEN_ADD:
            printf("%-20s %20s\n", "ADD", yytext);
            break;
        case TOKEN_HYPHEN:
            printf("%-20s %20s\n", "HYPHEN", yytext);
            break;
        case TOKEN_ASTERISK:
            printf("%-20s %20s\n", "ASTERISK", yytext);
            break;
        case TOKEN_FSLASH:
            printf("%-20s %20s\n", "FORWARD SLASH", yytext);
            break;
        case TOKEN_PERCENT:
            printf("%-20s %20s\n", "PERCENT", yytext);
            break;
        case TOKEN_TYPE_CHAR: {
            /*char c_decoded;
            if (char_decode(yytext, &c_decoded) == 0) {
                if (c_decoded == 0)
                    printf("%-20s %20c\n", "CHAR_LITERAL", "(null)");
                else printf("%-20s %20c\n", "CHAR_LITERAL", c_decoded);
            }
            else {
                printf("scan error: %s contains invalid character(s).\n", yytext);
                flag = 1;
            }*/
            printf("%-20s %20s\n", "CHAR_LITERAL", yytext);
            break;
        }
        case TOKEN_TYPE_STRING: {
            /*char s_decoded[256];
            char s_encoded[1280];
            if (string_decode(yytext, s_decoded) == 0) {
                if (string_encode(s_decoded, s_encoded) == 0) 
                    printf("%-20s %20s\n", "STRING_LITERAL", s_encoded);
                else {
                    printf("scan error: %s contains invalid character(s).\n", yytext);
                    flag = 1;
                }
            } else {
                printf("scan error: %s contains invalid character(s).\n", yytext);
                flag = 1;
            }*/
            printf("%-20s %20s\n", "STRING_LITERAL", yytext);
            break;
        }
        case TOKEN_TYPE_INT:
            printf("%-20s %20s\n", "INT_LITERAL", yytext);
            break;
        case TOKEN_TYPE_FLOAT:
            printf("%-20s %20s\n", "FLOAT_LITERAL", yytext);
            break;
        case TOKEN_IDENT:
            if (strlen(yytext) <= 256)
                printf("%-20s %20s\n", "IDENTIFIER", yytext);
            else {
                printf("scan error: identifier exceeds maximum length.\n");
                flag = 1;
            }
            break;
        case TOKEN_ERROR:
            printf("scan error: %s is not invalid token.\n", yytext);
            flag = 1;
            break;
        case TOKEN_EOF:
            flag = -1;
            break;
        default:
            break;
        }

        if (flag == -1) break;
        if (flag == 1)
            return 1;
    }
    return 0;
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        print_usage(argv[0]);
        return 1;
    }

    char *file;

    if (strcmp(argv[1], "--encode") == 0) {
        file = argv[2];
        yyin = fopen(file, "r");

        if (!yyin) {
            print_usage(argv[0]);
            return 1;
        }

        char input[1280];
        char decoded[256];
        char encoded[1280];

        fgets(input, sizeof(input), yyin);
        fclose(yyin);

        if (string_decode(input, decoded) == 0) {
            if (string_encode(decoded, encoded) == 0) printf("\"%s\"\n", encoded);            
            else {
                printf("encoding failed.\n");
                return 1;
            }
        }
        else 
            return 1;

    } else if (strcmp(argv[1], "--scan") == 0) {
        file = argv[2];
        yyin = fopen(file, "r");
        
        if (!yyin) {
            print_usage(argv[0]);
            return 1;
        }
        if (scan() == 1) {
            return 1;
        }
        
    } else if (strcmp(argv[1], "--parse") == 0) {
        file = argv[2];
        yyin = fopen(file, "r");
        if (!yyin) {
            print_usage(argv[0]);
            return 1;
        }

        //yydebug = 1;
        int y = yyparse();
        if (y == 1 || y == 2) {
            printf("parsing failed.\n");
            return 1;
        }
        printf("parsing succed.\n");
        
    } else if (strcmp(argv[1], "--print") == 0) {
        file = argv[2];
        yyin = fopen(file, "r");
        if (!yyin) {
            print_usage(argv[0]);
            return 1;
        }
        int y = yyparse();
        if (y) {
            printf("parse failed.\n");
            return 1;
        }
        decl_print(parser_result, 0);
    
    } else if (strcmp(argv[1], "--resolve") == 0) {
        file = argv[2];
        yyin = fopen(file, "r");
        if (!yyin) {
            print_usage(argv[0]);
            return 1;
        }
        int y = yyparse();
        if (y) {
            printf("parse failed.\n");
            return 1;
        }

        resolve_error = 0;
        
        struct hash_table *h = hash_table_create(0, 0);
        struct scope *s = scope_create(1, h, 0, 0);

        decl_resolve(s, parser_result);

        if (resolve_error != 0)
            return 1;
    }

    return 0;
}
