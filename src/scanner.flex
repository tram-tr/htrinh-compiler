%{
    #include "../inc/token.h"
%}
SINGLE_COMMENT  \/\/[^\n\r]*
BLOCK_COMMENT   \/\*[^*]*\*+([^*\/][^*]*\*+)*\/
STRING          \"([^\"\0\n\t]|(\\.))*\"
CHAR            \'(\\.|\\0x[0-7][0-9a-fA-F]|.)\'
FLOAT           [-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?
INTEGER         [-+]?[0-9]+
IDENT           ([a-zA-Z]|_)+([a-zA-Z]|[0-9]|_)* 
%%
(" "|\t|\n|\s)          /* skip whitespace */
{SINGLE_COMMENT}        {   return TOKEN_SINGLE_COMMENT; }
{BLOCK_COMMENT}         {   return TOKEN_BLOCK_COMMENT;  }

array                   {   return TOKEN_ARRAY;          }
auto                    {   return TOKEN_AUTO;           }
bool                    {   return TOKEN_BOOL;           }
char                    {   return TOKEN_CHAR;           }
else                    {   return TOKEN_ELSE;           }
false                   {   return TOKEN_FALSE;          }
for                     {   return TOKEN_FOR;            }
function                {   return TOKEN_FUNC;           }
if                      {   return TOKEN_IF;             }
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

\?                      {   return TOKEN_TERNARY;        }
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

{CHAR}                  {   return TOKEN_TYPE_CHAR;      }
{STRING}                {   return TOKEN_TYPE_STRING;    }
{INTEGER}               {   return TOKEN_TYPE_INT;       }
{FLOAT}                 {   return TOKEN_TYPE_FLOAT;     }
{IDENT}                 {   return TOKEN_IDENT;          }

.                       {   return TOKEN_ERROR;          }
%%
int yywrap()            {   return 1;                    }