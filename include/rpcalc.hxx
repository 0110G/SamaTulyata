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

#ifndef YY_YY_SRC_RPCALC_HXX_INCLUDED
# define YY_YY_SRC_RPCALC_HXX_INCLUDED
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
    CHAR = 258,
    NUM = 259,
    EQ = 260,
    GRETEREQ = 261,
    LESSEQ = 262,
    GRETER = 263,
    LESS = 264,
    EQEQ = 265,
    AND = 266,
    OR = 267,
    PLUS = 268,
    MINUS = 269,
    MULT = 270,
    DIV = 271,
    NOTEQ = 272,
    UNDER = 273
  };
#endif
/* Tokens.  */
#define CHAR 258
#define NUM 259
#define EQ 260
#define GRETEREQ 261
#define LESSEQ 262
#define GRETER 263
#define LESS 264
#define EQEQ 265
#define AND 266
#define OR 267
#define PLUS 268
#define MINUS 269
#define MULT 270
#define DIV 271
#define NOTEQ 272
#define UNDER 273

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 28 "./src/rpcalc.yxx" /* yacc.c:1909  */

    char     literal  ;
    int      constant ;
    char*    operetor ;
    EXPR  PresPlusExpr;

#line 97 "./src/rpcalc.hxx" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_RPCALC_HXX_INCLUDED  */
