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

#ifndef YY_PARSER_BISON_PARSER_BISON_HPP_INCLUDED
# define YY_PARSER_BISON_PARSER_BISON_HPP_INCLUDED
/* Debug traces.  */
#ifndef PARSER_BISON_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define PARSER_BISON_DEBUG 1
#  else
#   define PARSER_BISON_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define PARSER_BISON_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined PARSER_BISON_DEBUG */
#if PARSER_BISON_DEBUG
extern int parser_bison_debug;
#endif
/* "%code requires" blocks.  */
#line 21 "parser_bison.ypp" /* yacc.c:1909  */


#line 55 "parser_bison.hpp" /* yacc.c:1909  */

/* Token type.  */
#ifndef PARSER_BISON_TOKENTYPE
# define PARSER_BISON_TOKENTYPE
  enum parser_bison_tokentype
  {
    TK_IDENT = 258,
    TK_INT = 259,
    TK_REAL = 260,
    TK_STR_VAR = 261,
    TK_STR_CONST = 262,
    TK_STR_INT = 263,
    TK_STR_REAL = 264,
    TK_EQ = 265,
    TK_LE = 266,
    TK_GE = 267,
    TK_IN = 268,
    TK_LT = 269,
    TK_GT = 270,
    TK_PLUS = 271,
    TK_MINUS = 272,
    TK_MUL = 273,
    TK_DIV = 274,
    TK_POW = 275,
    TK_SQRT = 276,
    TK_LOG = 277,
    TK_EXP = 278,
    TK_TAN = 279,
    TK_COS = 280,
    TK_SIN = 281,
    TK_LPAR = 282,
    TK_RPAR = 283,
    TK_LSBR = 284,
    TK_RSBR = 285,
    TK_COMMA = 286,
    TK_SCOLON = 287,
    TK_COLON = 288,
    TK_SGN = 289,
    TK_ABS = 290,
    TK_MIN = 291,
    TK_MAX = 292,
    TK_UMINUS = 293,
    TK_UPLUS = 294
  };
#endif
/* Tokens.  */
#define TK_IDENT 258
#define TK_INT 259
#define TK_REAL 260
#define TK_STR_VAR 261
#define TK_STR_CONST 262
#define TK_STR_INT 263
#define TK_STR_REAL 264
#define TK_EQ 265
#define TK_LE 266
#define TK_GE 267
#define TK_IN 268
#define TK_LT 269
#define TK_GT 270
#define TK_PLUS 271
#define TK_MINUS 272
#define TK_MUL 273
#define TK_DIV 274
#define TK_POW 275
#define TK_SQRT 276
#define TK_LOG 277
#define TK_EXP 278
#define TK_TAN 279
#define TK_COS 280
#define TK_SIN 281
#define TK_LPAR 282
#define TK_RPAR 283
#define TK_LSBR 284
#define TK_RSBR 285
#define TK_COMMA 286
#define TK_SCOLON 287
#define TK_COLON 288
#define TK_SGN 289
#define TK_ABS 290
#define TK_MIN 291
#define TK_MAX 292
#define TK_UMINUS 293
#define TK_UPLUS 294

/* Value type.  */
#if ! defined PARSER_BISON_STYPE && ! defined PARSER_BISON_STYPE_IS_DECLARED

union PARSER_BISON_STYPE
{
#line 25 "parser_bison.ypp" /* yacc.c:1909  */

   char u_str[256];

#line 149 "parser_bison.hpp" /* yacc.c:1909  */
};

typedef union PARSER_BISON_STYPE PARSER_BISON_STYPE;
# define PARSER_BISON_STYPE_IS_TRIVIAL 1
# define PARSER_BISON_STYPE_IS_DECLARED 1
#endif


extern PARSER_BISON_STYPE parser_bison_lval;

int parser_bison_parse (void);

#endif /* !YY_PARSER_BISON_PARSER_BISON_HPP_INCLUDED  */
