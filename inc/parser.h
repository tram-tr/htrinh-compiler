/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_INC_PARSER_H_INCLUDED
# define YY_YY_INC_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOKEN_EOF = 0,
    TOKEN_IDENT = 258,
    TOKEN_TYPE_INT = 259,
    TOKEN_TYPE_FLOAT = 260,
    TOKEN_TYPE_STRING = 261,
    TOKEN_TYPE_CHAR = 262,
    TOKEN_ARRAY = 263,
    TOKEN_AUTO = 264,
    TOKEN_BOOL = 265,
    TOKEN_CHAR = 266,
    TOKEN_ELSE = 267,
    TOKEN_FALSE = 268,
    TOKEN_FLOAT = 269,
    TOKEN_FOR = 270,
    TOKEN_FUNC = 271,
    TOKEN_IF = 272,
    TOKEN_INT = 273,
    TOKEN_PRINT = 274,
    TOKEN_RETURN = 275,
    TOKEN_STRING = 276,
    TOKEN_TRUE = 277,
    TOKEN_VOID = 278,
    TOKEN_WHILE = 279,
    TOKEN_AND = 280,
    TOKEN_OR = 281,
    TOKEN_EQ = 282,
    TOKEN_LE = 283,
    TOKEN_GE = 284,
    TOKEN_LT = 285,
    TOKEN_GT = 286,
    TOKEN_NE = 287,
    TOKEN_NOT = 288,
    TOKEN_COLON = 289,
    TOKEN_SEMICOLON = 290,
    TOKEN_COMMA = 291,
    TOKEN_LBRACKET = 292,
    TOKEN_RBRACKET = 293,
    TOKEN_LBRACE = 294,
    TOKEN_RBRACE = 295,
    TOKEN_LPAREN = 296,
    TOKEN_RPAREN = 297,
    TOKEN_ASSIGN = 298,
    TOKEN_EXPONENT = 299,
    TOKEN_INCREMENT = 300,
    TOKEN_DECREMENT = 301,
    TOKEN_ADD = 302,
    TOKEN_HYPHEN = 303,
    TOKEN_ASTERISK = 304,
    TOKEN_FSLASH = 305,
    TOKEN_PERCENT = 306,
    TOKEN_SINGLE_COMMENT = 307,
    TOKEN_BLOCK_COMMENT = 308,
    TOKEN_ERROR = 309
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 79 "src/parser.bison" /* yacc.c:1909  */

    struct decl *decl;
    struct expr *expr;
    struct stmt *stmt;
    struct symbol *symbol;
    struct type *type;
    struct param_list *param_list;
    char *name;

#line 120 "inc/parser.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INC_PARSER_H_INCLUDED  */
