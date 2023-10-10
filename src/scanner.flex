%{
    #include "../inc/parser.h"
    #include "../inc/encoder.h"
%}
SINGLE_COMMENT  \/\/[^\n\r]*
BLOCK_COMMENT   \/\*[^*]*\*+([^*\/][^*]*\*+)*\/
STRING          \"([^\"\0\n\t]|(\\.))*\"
CHAR            \'(\\.|\\0x[0-7][0-9a-fA-F]|.)\'
FLOAT           [-+]?[0-9]*\.[0-9]|[-+]?[0-9]+([eE][-+]?[0-9]+)?
INTEGER         [-+]?[0-9]+
LETTER          [a-zA-Z]
IDENT           (_|{LETTER}|\s)(_|{LETTER}|\s|[0-9])* 
%%
[[:space:]]+            /* skip whitespace */
{SINGLE_COMMENT}        
{BLOCK_COMMENT}  

array                   {   return TOKEN_ARRAY;          }
auto                    {   return TOKEN_AUTO;           }
boolean                 {   return TOKEN_BOOL;           }
char                    {   return TOKEN_CHAR;           }
else                    {   return TOKEN_ELSE;           }
false                   {   return TOKEN_FALSE;          }
for                     {   return TOKEN_FOR;            }
float                   {   return TOKEN_FLOAT;          }
function                {   return TOKEN_FUNC;           }
if                      {   return TOKEN_IF;             }
integer                 {   return TOKEN_INT;            }
print                   {   return TOKEN_PRINT;          }        
return                  {   return TOKEN_RETURN;         }
string                  {   return TOKEN_STRING;         }
true                    {   return TOKEN_TRUE;           }
void                    {   return TOKEN_VOID;           }
while                   {   return TOKEN_WHILE;          }

&&                      {   return TOKEN_AND;            }
\|\|                    {   return TOKEN_OR;             }
==                      {   return TOKEN_EQ;             }
\<\=                    {   return TOKEN_LE;             }
\>\=                    {   return TOKEN_GE;             }
\<                      {   return TOKEN_LT;             }
\>                      {   return TOKEN_GT;             }
\!\=                    {   return TOKEN_NE;             }
\!                      {   return TOKEN_NOT;            }

:                       {   return TOKEN_COLON;          }
;                       {   return TOKEN_SEMICOLON;      }
,                       {   return TOKEN_COMMA;          }
\[                      {   return TOKEN_LBRACKET;       }
\]                      {   return TOKEN_RBRACKET;       }
\{                      {   return TOKEN_LBRACE;         }
\}                      {   return TOKEN_RBRACE;         }
\(                      {   return TOKEN_LPAREN;         }
\)                      {   return TOKEN_RPAREN;         }

\=                      {   return TOKEN_ASSIGN;         }
\^                      {   return TOKEN_EXPONENT;       }
\+\+                    {   return TOKEN_INCREMENT;      }
--                      {   return TOKEN_DECREMENT;      }
\+                      {   return TOKEN_ADD;            }
\-                      {   return TOKEN_HYPHEN;         }
\*                      {   return TOKEN_ASTERISK;       }
\/                      {   return TOKEN_FSLASH;         }
%                       {   return TOKEN_PERCENT;        }

{CHAR}                  {   
                            char c_decoded;
                            if (char_decode(yytext, &c_decoded) == 0) 
                                return TOKEN_TYPE_CHAR; 
                            else {
                                    printf("scanning failed.\n");
                                    exit(1);
                            }              
                        }
{STRING}                {
                            char s_decoded[256];
                            char s_encoded[1280];
                            if (string_decode(yytext, s_decoded) == 0) {
                                if (string_encode(s_decoded, s_encoded) == 0) 
                                    return TOKEN_TYPE_STRING;
                                else {
                                    printf("scanning failed.\n");
                                    exit(1);
                                }
                            } else {
                                    printf("scanning failed.\n");
                                    exit(1);
                            }          
                        }
{INTEGER}               {   return TOKEN_TYPE_INT;       }
{FLOAT}                 {   return TOKEN_TYPE_FLOAT;     }
{IDENT}                 {   return TOKEN_IDENT;          }       

.                       {   
                            printf("scanning failed.\n");
                            return TOKEN_ERROR;          
                        }
%%
int yywrap()            {   
                            printf("scanning succeed.\n"); 
                            return 1;                    
                        }