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

#ifndef YY_REALPAVER_BISON_PARSER_BISON_HPP_INCLUDED
# define YY_REALPAVER_BISON_PARSER_BISON_HPP_INCLUDED
/* Debug traces.  */
#ifndef REALPAVER_BISON_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define REALPAVER_BISON_DEBUG 1
#  else
#   define REALPAVER_BISON_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define REALPAVER_BISON_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined REALPAVER_BISON_DEBUG */
#if REALPAVER_BISON_DEBUG
extern int realpaver_bison_debug;
#endif
/* "%code requires" blocks.  */
#line 26 "parser_bison.ypp" /* yacc.c:1909  */

#include "realpaver/Constraint.hpp"
#include "realpaver/Exception.hpp"

#line 57 "parser_bison.hpp" /* yacc.c:1909  */

/* Token type.  */
#ifndef REALPAVER_BISON_TOKENTYPE
# define REALPAVER_BISON_TOKENTYPE
  enum realpaver_bison_tokentype
  {
    TK_IDENT = 258,
    TK_INT = 259,
    TK_REAL = 260,
    TK_STR_VAR = 261,
    TK_STR_CONST = 262,
    TK_STR_CTR = 263,
    TK_STR_OBJ = 264,
    TK_STR_FUN = 265,
    TK_STR_ALIAS = 266,
    TK_STR_INT = 267,
    TK_EQ = 268,
    TK_EQ_EQ = 269,
    TK_LE = 270,
    TK_GE = 271,
    TK_IN = 272,
    TK_LT = 273,
    TK_GT = 274,
    TK_PLUS = 275,
    TK_MINUS = 276,
    TK_MUL = 277,
    TK_DIV = 278,
    TK_POW = 279,
    TK_SQRT = 280,
    TK_LOG = 281,
    TK_EXP = 282,
    TK_TAN = 283,
    TK_COS = 284,
    TK_SIN = 285,
    TK_LPAR = 286,
    TK_RPAR = 287,
    TK_LSBR = 288,
    TK_RSBR = 289,
    TK_COMMA = 290,
    TK_SCOLON = 291,
    TK_COLON = 292,
    TK_SGN = 293,
    TK_ABS = 294,
    TK_MIN = 295,
    TK_MAX = 296,
    TK_UMINUS = 297,
    TK_UPLUS = 298
  };
#endif
/* Tokens.  */
#define TK_IDENT 258
#define TK_INT 259
#define TK_REAL 260
#define TK_STR_VAR 261
#define TK_STR_CONST 262
#define TK_STR_CTR 263
#define TK_STR_OBJ 264
#define TK_STR_FUN 265
#define TK_STR_ALIAS 266
#define TK_STR_INT 267
#define TK_EQ 268
#define TK_EQ_EQ 269
#define TK_LE 270
#define TK_GE 271
#define TK_IN 272
#define TK_LT 273
#define TK_GT 274
#define TK_PLUS 275
#define TK_MINUS 276
#define TK_MUL 277
#define TK_DIV 278
#define TK_POW 279
#define TK_SQRT 280
#define TK_LOG 281
#define TK_EXP 282
#define TK_TAN 283
#define TK_COS 284
#define TK_SIN 285
#define TK_LPAR 286
#define TK_RPAR 287
#define TK_LSBR 288
#define TK_RSBR 289
#define TK_COMMA 290
#define TK_SCOLON 291
#define TK_COLON 292
#define TK_SGN 293
#define TK_ABS 294
#define TK_MIN 295
#define TK_MAX 296
#define TK_UMINUS 297
#define TK_UPLUS 298

/* Value type.  */
#if ! defined REALPAVER_BISON_STYPE && ! defined REALPAVER_BISON_STYPE_IS_DECLARED

union REALPAVER_BISON_STYPE
{
#line 32 "parser_bison.ypp" /* yacc.c:1909  */

  char u_str[256];
  realpaver::TermRep* u_term;

#line 160 "parser_bison.hpp" /* yacc.c:1909  */
};

typedef union REALPAVER_BISON_STYPE REALPAVER_BISON_STYPE;
# define REALPAVER_BISON_STYPE_IS_TRIVIAL 1
# define REALPAVER_BISON_STYPE_IS_DECLARED 1
#endif


extern REALPAVER_BISON_STYPE realpaver_bison_lval;

int realpaver_bison_parse (void);

#endif /* !YY_REALPAVER_BISON_PARSER_BISON_HPP_INCLUDED  */
