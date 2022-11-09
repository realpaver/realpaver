/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

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
#line 31 "parser_bison.ypp"

#include "realpaver/Constraint.hpp"
#include "realpaver/Exception.hpp"

#line 62 "parser_bison.hpp"

/* Token kinds.  */
#ifndef REALPAVER_BISON_TOKENTYPE
# define REALPAVER_BISON_TOKENTYPE
  enum realpaver_bison_tokentype
  {
    REALPAVER_BISON_EMPTY = -2,
    REALPAVER_BISON_EOF = 0,       /* "end of file"  */
    REALPAVER_BISON_error = 256,   /* error  */
    REALPAVER_BISON_UNDEF = 257,   /* "invalid token"  */
    TK_IDENT = 258,                /* TK_IDENT  */
    TK_INT = 259,                  /* TK_INT  */
    TK_REAL = 260,                 /* TK_REAL  */
    TK_STR_VAR = 261,              /* TK_STR_VAR  */
    TK_STR_CONST = 262,            /* TK_STR_CONST  */
    TK_STR_CTR = 263,              /* TK_STR_CTR  */
    TK_STR_OBJ = 264,              /* TK_STR_OBJ  */
    TK_STR_FUN = 265,              /* TK_STR_FUN  */
    TK_STR_ALIAS = 266,            /* TK_STR_ALIAS  */
    TK_STR_INT = 267,              /* TK_STR_INT  */
    TK_STR_TOL = 268,              /* TK_STR_TOL  */
    TK_STR_BIN = 269,              /* TK_STR_BIN  */
    TK_TABLE = 270,                /* TK_TABLE  */
    TK_TOL = 271,                  /* TK_TOL  */
    TK_EQ = 272,                   /* TK_EQ  */
    TK_EQ_EQ = 273,                /* TK_EQ_EQ  */
    TK_LE = 274,                   /* TK_LE  */
    TK_GE = 275,                   /* TK_GE  */
    TK_IN = 276,                   /* TK_IN  */
    TK_LT = 277,                   /* TK_LT  */
    TK_GT = 278,                   /* TK_GT  */
    TK_PLUS = 279,                 /* TK_PLUS  */
    TK_MINUS = 280,                /* TK_MINUS  */
    TK_MUL = 281,                  /* TK_MUL  */
    TK_DIV = 282,                  /* TK_DIV  */
    TK_POW = 283,                  /* TK_POW  */
    TK_CARET = 284,                /* TK_CARET  */
    TK_SQR = 285,                  /* TK_SQR  */
    TK_SQRT = 286,                 /* TK_SQRT  */
    TK_LOG = 287,                  /* TK_LOG  */
    TK_EXP = 288,                  /* TK_EXP  */
    TK_TAN = 289,                  /* TK_TAN  */
    TK_COS = 290,                  /* TK_COS  */
    TK_SIN = 291,                  /* TK_SIN  */
    TK_LPAR = 292,                 /* TK_LPAR  */
    TK_RPAR = 293,                 /* TK_RPAR  */
    TK_LSBR = 294,                 /* TK_LSBR  */
    TK_RSBR = 295,                 /* TK_RSBR  */
    TK_LCBR = 296,                 /* TK_LCBR  */
    TK_RCBR = 297,                 /* TK_RCBR  */
    TK_PIPE = 298,                 /* TK_PIPE  */
    TK_COMMA = 299,                /* TK_COMMA  */
    TK_SCOLON = 300,               /* TK_SCOLON  */
    TK_COLON = 301,                /* TK_COLON  */
    TK_SGN = 302,                  /* TK_SGN  */
    TK_ABS = 303,                  /* TK_ABS  */
    TK_MIN = 304,                  /* TK_MIN  */
    TK_MAX = 305,                  /* TK_MAX  */
    TK_MINUS_INF = 306,            /* TK_MINUS_INF  */
    TK_PLUS_INF = 307,             /* TK_PLUS_INF  */
    TK_UMINUS = 308,               /* TK_UMINUS  */
    TK_UPLUS = 309                 /* TK_UPLUS  */
  };
  typedef enum realpaver_bison_tokentype realpaver_bison_token_kind_t;
#endif
/* Token kinds.  */
#define REALPAVER_BISON_EMPTY -2
#define REALPAVER_BISON_EOF 0
#define REALPAVER_BISON_error 256
#define REALPAVER_BISON_UNDEF 257
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
#define TK_STR_TOL 268
#define TK_STR_BIN 269
#define TK_TABLE 270
#define TK_TOL 271
#define TK_EQ 272
#define TK_EQ_EQ 273
#define TK_LE 274
#define TK_GE 275
#define TK_IN 276
#define TK_LT 277
#define TK_GT 278
#define TK_PLUS 279
#define TK_MINUS 280
#define TK_MUL 281
#define TK_DIV 282
#define TK_POW 283
#define TK_CARET 284
#define TK_SQR 285
#define TK_SQRT 286
#define TK_LOG 287
#define TK_EXP 288
#define TK_TAN 289
#define TK_COS 290
#define TK_SIN 291
#define TK_LPAR 292
#define TK_RPAR 293
#define TK_LSBR 294
#define TK_RSBR 295
#define TK_LCBR 296
#define TK_RCBR 297
#define TK_PIPE 298
#define TK_COMMA 299
#define TK_SCOLON 300
#define TK_COLON 301
#define TK_SGN 302
#define TK_ABS 303
#define TK_MIN 304
#define TK_MAX 305
#define TK_MINUS_INF 306
#define TK_PLUS_INF 307
#define TK_UMINUS 308
#define TK_UPLUS 309

/* Value type.  */
#if ! defined REALPAVER_BISON_STYPE && ! defined REALPAVER_BISON_STYPE_IS_DECLARED
union REALPAVER_BISON_STYPE
{
#line 37 "parser_bison.ypp"

  char u_str[256];
  realpaver::TermRep* u_term;

#line 195 "parser_bison.hpp"

};
typedef union REALPAVER_BISON_STYPE REALPAVER_BISON_STYPE;
# define REALPAVER_BISON_STYPE_IS_TRIVIAL 1
# define REALPAVER_BISON_STYPE_IS_DECLARED 1
#endif


extern REALPAVER_BISON_STYPE realpaver_bison_lval;


int realpaver_bison_parse (void);


#endif /* !YY_REALPAVER_BISON_PARSER_BISON_HPP_INCLUDED  */
