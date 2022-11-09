/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         REALPAVER_BISON_STYPE
/* Substitute the variable and function names.  */
#define yyparse         realpaver_bison_parse
#define yylex           realpaver_bison_lex
#define yyerror         realpaver_bison_error
#define yydebug         realpaver_bison_debug
#define yynerrs         realpaver_bison_nerrs
#define yylval          realpaver_bison_lval
#define yychar          realpaver_bison_char

/* First part of user prologue.  */
#line 1 "parser_bison.ypp"

#include <cstring>
#include <sstream>
#include <string>
#include "realpaver/Param.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/SymbolTable.hpp"

#undef YYINITDEPTH
#define YYINITDEPTH 5000

extern char* realpaver_bison_text;
extern int realpaver_bison_lineno;
extern std::string realpaver_parse_error;
extern realpaver::Problem* realpaver_bison_problem;
extern realpaver::Param* realpaver_bison_param;
extern realpaver::SymbolTable* realpaver_bison_symtab;

extern std::vector<realpaver::Variable>* realpaver_var_vector;
extern std::vector<realpaver::Interval>* realpaver_itv_vector;

extern int realpaver_bison_lex(void);
int realpaver_bison_error(const char* str);

extern realpaver::FunctionSymbol* realpaver_fun_symbol;

#line 106 "parser_bison.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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

#line 154 "parser_bison.cpp"

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

#line 287 "parser_bison.cpp"

};
typedef union REALPAVER_BISON_STYPE REALPAVER_BISON_STYPE;
# define REALPAVER_BISON_STYPE_IS_TRIVIAL 1
# define REALPAVER_BISON_STYPE_IS_DECLARED 1
#endif


extern REALPAVER_BISON_STYPE realpaver_bison_lval;


int realpaver_bison_parse (void);


#endif /* !YY_REALPAVER_BISON_PARSER_BISON_HPP_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TK_IDENT = 3,                   /* TK_IDENT  */
  YYSYMBOL_TK_INT = 4,                     /* TK_INT  */
  YYSYMBOL_TK_REAL = 5,                    /* TK_REAL  */
  YYSYMBOL_TK_STR_VAR = 6,                 /* TK_STR_VAR  */
  YYSYMBOL_TK_STR_CONST = 7,               /* TK_STR_CONST  */
  YYSYMBOL_TK_STR_CTR = 8,                 /* TK_STR_CTR  */
  YYSYMBOL_TK_STR_OBJ = 9,                 /* TK_STR_OBJ  */
  YYSYMBOL_TK_STR_FUN = 10,                /* TK_STR_FUN  */
  YYSYMBOL_TK_STR_ALIAS = 11,              /* TK_STR_ALIAS  */
  YYSYMBOL_TK_STR_INT = 12,                /* TK_STR_INT  */
  YYSYMBOL_TK_STR_TOL = 13,                /* TK_STR_TOL  */
  YYSYMBOL_TK_STR_BIN = 14,                /* TK_STR_BIN  */
  YYSYMBOL_TK_TABLE = 15,                  /* TK_TABLE  */
  YYSYMBOL_TK_TOL = 16,                    /* TK_TOL  */
  YYSYMBOL_TK_EQ = 17,                     /* TK_EQ  */
  YYSYMBOL_TK_EQ_EQ = 18,                  /* TK_EQ_EQ  */
  YYSYMBOL_TK_LE = 19,                     /* TK_LE  */
  YYSYMBOL_TK_GE = 20,                     /* TK_GE  */
  YYSYMBOL_TK_IN = 21,                     /* TK_IN  */
  YYSYMBOL_TK_LT = 22,                     /* TK_LT  */
  YYSYMBOL_TK_GT = 23,                     /* TK_GT  */
  YYSYMBOL_TK_PLUS = 24,                   /* TK_PLUS  */
  YYSYMBOL_TK_MINUS = 25,                  /* TK_MINUS  */
  YYSYMBOL_TK_MUL = 26,                    /* TK_MUL  */
  YYSYMBOL_TK_DIV = 27,                    /* TK_DIV  */
  YYSYMBOL_TK_POW = 28,                    /* TK_POW  */
  YYSYMBOL_TK_CARET = 29,                  /* TK_CARET  */
  YYSYMBOL_TK_SQR = 30,                    /* TK_SQR  */
  YYSYMBOL_TK_SQRT = 31,                   /* TK_SQRT  */
  YYSYMBOL_TK_LOG = 32,                    /* TK_LOG  */
  YYSYMBOL_TK_EXP = 33,                    /* TK_EXP  */
  YYSYMBOL_TK_TAN = 34,                    /* TK_TAN  */
  YYSYMBOL_TK_COS = 35,                    /* TK_COS  */
  YYSYMBOL_TK_SIN = 36,                    /* TK_SIN  */
  YYSYMBOL_TK_LPAR = 37,                   /* TK_LPAR  */
  YYSYMBOL_TK_RPAR = 38,                   /* TK_RPAR  */
  YYSYMBOL_TK_LSBR = 39,                   /* TK_LSBR  */
  YYSYMBOL_TK_RSBR = 40,                   /* TK_RSBR  */
  YYSYMBOL_TK_LCBR = 41,                   /* TK_LCBR  */
  YYSYMBOL_TK_RCBR = 42,                   /* TK_RCBR  */
  YYSYMBOL_TK_PIPE = 43,                   /* TK_PIPE  */
  YYSYMBOL_TK_COMMA = 44,                  /* TK_COMMA  */
  YYSYMBOL_TK_SCOLON = 45,                 /* TK_SCOLON  */
  YYSYMBOL_TK_COLON = 46,                  /* TK_COLON  */
  YYSYMBOL_TK_SGN = 47,                    /* TK_SGN  */
  YYSYMBOL_TK_ABS = 48,                    /* TK_ABS  */
  YYSYMBOL_TK_MIN = 49,                    /* TK_MIN  */
  YYSYMBOL_TK_MAX = 50,                    /* TK_MAX  */
  YYSYMBOL_TK_MINUS_INF = 51,              /* TK_MINUS_INF  */
  YYSYMBOL_TK_PLUS_INF = 52,               /* TK_PLUS_INF  */
  YYSYMBOL_TK_UMINUS = 53,                 /* TK_UMINUS  */
  YYSYMBOL_TK_UPLUS = 54,                  /* TK_UPLUS  */
  YYSYMBOL_YYACCEPT = 55,                  /* $accept  */
  YYSYMBOL_model = 56,                     /* model  */
  YYSYMBOL_statement = 57,                 /* statement  */
  YYSYMBOL_const_statement = 58,           /* const_statement  */
  YYSYMBOL_const_list = 59,                /* const_list  */
  YYSYMBOL_const_tail = 60,                /* const_tail  */
  YYSYMBOL_const_def = 61,                 /* const_def  */
  YYSYMBOL_var_statement = 62,             /* var_statement  */
  YYSYMBOL_var_list = 63,                  /* var_list  */
  YYSYMBOL_var_tail = 64,                  /* var_tail  */
  YYSYMBOL_var_def = 65,                   /* var_def  */
  YYSYMBOL_66_1 = 66,                      /* $@1  */
  YYSYMBOL_var_dom = 67,                   /* var_dom  */
  YYSYMBOL_68_2 = 68,                      /* $@2  */
  YYSYMBOL_var_info = 69,                  /* var_info  */
  YYSYMBOL_alias_statement = 70,           /* alias_statement  */
  YYSYMBOL_alias_list = 71,                /* alias_list  */
  YYSYMBOL_alias_tail = 72,                /* alias_tail  */
  YYSYMBOL_alias_def = 73,                 /* alias_def  */
  YYSYMBOL_ctr_statement = 74,             /* ctr_statement  */
  YYSYMBOL_ctr_list = 75,                  /* ctr_list  */
  YYSYMBOL_ctr_tail = 76,                  /* ctr_tail  */
  YYSYMBOL_ctr_def = 77,                   /* ctr_def  */
  YYSYMBOL_78_3 = 78,                      /* $@3  */
  YYSYMBOL_obj_statement = 79,             /* obj_statement  */
  YYSYMBOL_obj_list = 80,                  /* obj_list  */
  YYSYMBOL_obj_tail = 81,                  /* obj_tail  */
  YYSYMBOL_obj_def = 82,                   /* obj_def  */
  YYSYMBOL_fun_statement = 83,             /* fun_statement  */
  YYSYMBOL_fun_list = 84,                  /* fun_list  */
  YYSYMBOL_fun_tail = 85,                  /* fun_tail  */
  YYSYMBOL_fun_def = 86,                   /* fun_def  */
  YYSYMBOL_87_4 = 87,                      /* $@4  */
  YYSYMBOL_arg_list = 88,                  /* arg_list  */
  YYSYMBOL_arg_tail = 89,                  /* arg_tail  */
  YYSYMBOL_arg_def = 90,                   /* arg_def  */
  YYSYMBOL_table_list_var = 91,            /* table_list_var  */
  YYSYMBOL_table_list_var_tail = 92,       /* table_list_var_tail  */
  YYSYMBOL_table_list_var_def = 93,        /* table_list_var_def  */
  YYSYMBOL_table_list_val = 94,            /* table_list_val  */
  YYSYMBOL_95_5 = 95,                      /* $@5  */
  YYSYMBOL_table_list_val_tail = 96,       /* table_list_val_tail  */
  YYSYMBOL_table_list_val_def = 97,        /* table_list_val_def  */
  YYSYMBOL_const_expr = 98,                /* const_expr  */
  YYSYMBOL_expr = 99,                      /* expr  */
  YYSYMBOL_fun_call = 100,                 /* fun_call  */
  YYSYMBOL_101_6 = 101,                    /* $@6  */
  YYSYMBOL_expr_list = 102,                /* expr_list  */
  YYSYMBOL_103_7 = 103,                    /* $@7  */
  YYSYMBOL_expr_tail = 104,                /* expr_tail  */
  YYSYMBOL_exponent = 105,                 /* exponent  */
  YYSYMBOL_integer = 106,                  /* integer  */
  YYSYMBOL_real = 107,                     /* real  */
  YYSYMBOL_itv = 108,                      /* itv  */
  YYSYMBOL_itv_left = 109,                 /* itv_left  */
  YYSYMBOL_itv_right = 110,                /* itv_right  */
  YYSYMBOL_ident = 111                     /* ident  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined REALPAVER_BISON_STYPE_IS_TRIVIAL && REALPAVER_BISON_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  60
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   361

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  110
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  215

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   309


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54
};

#if REALPAVER_BISON_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    77,    77,    78,    82,    83,    84,    85,    86,    87,
      91,    95,    99,   100,   104,   130,   134,   138,   139,   144,
     143,   166,   177,   176,   197,   198,   204,   221,   225,   229,
     230,   234,   251,   255,   259,   260,   264,   272,   280,   289,
     288,   321,   325,   329,   330,   334,   345,   359,   363,   367,
     368,   373,   372,   399,   403,   404,   408,   423,   427,   428,
     432,   453,   452,   472,   473,   477,   481,   485,   492,   507,
     514,   521,   528,   535,   542,   549,   555,   559,   565,   571,
     577,   583,   589,   595,   601,   607,   613,   619,   626,   633,
     637,   641,   701,   705,   709,   717,   716,   750,   749,   758,
     759,   763,   767,   774,   791,   808,   828,   832,   840,   844,
     852
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if REALPAVER_BISON_DEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TK_IDENT", "TK_INT",
  "TK_REAL", "TK_STR_VAR", "TK_STR_CONST", "TK_STR_CTR", "TK_STR_OBJ",
  "TK_STR_FUN", "TK_STR_ALIAS", "TK_STR_INT", "TK_STR_TOL", "TK_STR_BIN",
  "TK_TABLE", "TK_TOL", "TK_EQ", "TK_EQ_EQ", "TK_LE", "TK_GE", "TK_IN",
  "TK_LT", "TK_GT", "TK_PLUS", "TK_MINUS", "TK_MUL", "TK_DIV", "TK_POW",
  "TK_CARET", "TK_SQR", "TK_SQRT", "TK_LOG", "TK_EXP", "TK_TAN", "TK_COS",
  "TK_SIN", "TK_LPAR", "TK_RPAR", "TK_LSBR", "TK_RSBR", "TK_LCBR",
  "TK_RCBR", "TK_PIPE", "TK_COMMA", "TK_SCOLON", "TK_COLON", "TK_SGN",
  "TK_ABS", "TK_MIN", "TK_MAX", "TK_MINUS_INF", "TK_PLUS_INF", "TK_UMINUS",
  "TK_UPLUS", "$accept", "model", "statement", "const_statement",
  "const_list", "const_tail", "const_def", "var_statement", "var_list",
  "var_tail", "var_def", "$@1", "var_dom", "$@2", "var_info",
  "alias_statement", "alias_list", "alias_tail", "alias_def",
  "ctr_statement", "ctr_list", "ctr_tail", "ctr_def", "$@3",
  "obj_statement", "obj_list", "obj_tail", "obj_def", "fun_statement",
  "fun_list", "fun_tail", "fun_def", "$@4", "arg_list", "arg_tail",
  "arg_def", "table_list_var", "table_list_var_tail", "table_list_var_def",
  "table_list_val", "$@5", "table_list_val_tail", "table_list_val_def",
  "const_expr", "expr", "fun_call", "$@6", "expr_list", "$@7", "expr_tail",
  "exponent", "integer", "real", "itv", "itv_left", "itv_right", "ident", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-103)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     105,     9,     9,   211,   -30,     9,     9,    21,   -21,  -103,
    -103,  -103,  -103,  -103,  -103,  -103,  -103,   -15,  -103,  -103,
     -13,    22,  -103,  -103,  -103,   248,   248,    16,    19,    24,
      27,    29,    43,    50,    65,   248,    95,   248,    66,    68,
      85,    87,  -103,     2,   205,  -103,  -103,  -103,  -103,    96,
     248,   248,  -103,    77,  -103,    91,    99,  -103,    93,   122,
    -103,   105,     9,  -103,    -4,     9,  -103,   248,   103,   119,
     119,   248,   248,   248,   248,   248,   248,   248,   248,    25,
    -103,  -103,   153,   106,    52,   248,   248,   248,   248,   211,
    -103,   248,   248,   248,   248,   248,   248,   248,    55,  -103,
     153,   153,   -30,  -103,     9,  -103,  -103,     9,  -103,   248,
    -103,  -103,  -103,   110,  -103,  -103,  -103,   115,   -11,   128,
     143,   161,   239,   275,   281,   291,  -103,   170,  -103,   297,
     307,     1,    11,  -103,   153,   153,   153,    15,    15,   119,
     119,  -103,  -103,  -103,   248,  -103,  -103,     9,  -103,   153,
    -103,     9,    55,  -103,  -103,  -103,  -103,  -103,  -103,  -103,
    -103,  -103,   118,  -103,  -103,   248,   248,   153,   121,   123,
     116,  -103,    84,   129,   120,  -103,   127,  -103,   313,   323,
     132,  -103,   167,     9,  -103,  -103,   176,  -103,   152,     9,
    -103,  -103,  -103,  -103,   248,  -103,   248,  -103,  -103,   156,
    -103,  -103,   153,     4,   166,  -103,  -103,  -103,  -103,   172,
     168,  -103,     4,  -103,  -103
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     9,   110,    15,    17,    19,    10,
      12,     0,   103,   104,    39,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    32,    34,     0,    90,    92,    93,    94,    91,
       0,     0,    41,    43,    47,    49,     0,    27,    29,     0,
       1,     2,     0,    16,     0,     0,    11,     0,     0,    76,
      75,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     107,   106,    68,     0,     0,     0,     0,     0,     0,     0,
      33,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      45,    46,     0,    42,     0,    48,    51,     0,    28,     0,
       3,    18,    21,     0,    20,    13,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,     0,    81,     0,
       0,     0,     0,    35,    36,    37,    38,    69,    70,    71,
      72,    73,   101,   102,     0,    44,    50,     0,    30,    31,
      22,     0,     0,    78,    77,    82,    83,    86,    84,    85,
     109,   108,     0,    79,    80,     0,     0,    97,     0,     0,
      54,    56,    24,     0,    58,    60,     0,   105,     0,     0,
      99,    96,     0,     0,    53,    25,     0,    23,     0,     0,
      57,    74,    87,    88,     0,    98,     0,    55,    26,     0,
      59,   100,    52,     0,     0,    61,    67,    66,    65,     0,
      63,    40,     0,    62,    64
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -103,   150,  -103,  -103,   162,  -103,  -103,  -103,   159,  -103,
    -103,  -103,  -103,  -103,  -103,  -103,   126,  -103,  -103,  -103,
     139,  -103,  -103,  -103,  -103,   135,  -103,  -103,  -103,   134,
    -103,  -103,  -103,    57,  -103,  -103,    60,  -103,  -103,    44,
    -103,  -103,  -103,   -60,    -3,  -103,  -103,    61,  -103,  -103,
     117,   -95,   -94,  -102,  -103,  -103,     0
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     7,     8,     9,    19,    66,    20,    10,    16,    63,
      17,    64,   114,   172,   187,    11,    57,   108,    58,    12,
      42,    90,    43,    68,    13,    52,   103,    53,    14,    54,
     105,    55,   147,   169,   184,   170,   173,   190,   174,   204,
     210,   213,   205,    81,    82,    45,   144,   168,   180,   195,
     141,    46,    47,    48,    83,   162,    49
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      44,    18,    21,   142,   143,    56,    59,   116,    22,    23,
     112,   150,    15,    94,    95,    96,    97,   113,    98,    50,
      51,    60,    69,    70,    61,    94,    95,    96,    97,    62,
      98,    65,    79,   152,    84,    94,    95,    96,    97,    67,
      98,    96,    97,    36,    98,   165,    89,   100,   101,    94,
      95,    96,    97,    71,    98,   166,    72,   142,   143,    22,
      23,    73,    18,   126,    74,    21,    75,   161,   118,   119,
     120,   121,   122,   123,   124,   125,    94,    95,    96,    97,
      76,    98,   129,   130,   131,   132,    44,    77,   134,   135,
     136,   137,   138,   139,   140,   128,   185,   186,    15,    22,
      23,   208,    78,    85,    56,    86,   149,    59,   206,   207,
     208,     1,     2,     3,     4,     5,     6,   206,   207,    25,
      26,   102,    87,    27,    88,    28,    29,    30,    31,    32,
      33,    34,    35,    99,    36,   104,   106,   107,    37,   109,
     117,   167,    38,    39,    40,    41,    80,   171,    98,    36,
     127,   175,    94,    95,    96,    97,   151,    98,   177,   181,
     183,   182,   178,   179,   189,   191,   153,    94,    95,    96,
      97,   188,    98,    15,    22,    23,   194,    94,    95,    96,
      97,   154,    98,   171,   196,    94,    95,    96,    97,   175,
      98,   167,   198,   202,    25,    26,   199,   203,    27,   155,
      28,    29,    30,    31,    32,    33,    34,    35,   209,    36,
     211,   110,   212,    37,    15,    22,    23,    38,    39,    40,
      41,   111,   160,    91,    92,    93,    24,   115,   133,    94,
      95,    96,    97,   148,    98,    25,    26,   145,   146,    27,
     197,    28,    29,    30,    31,    32,    33,    34,    35,   200,
      36,    15,    22,    23,    37,   201,   214,     0,    38,    39,
      40,    41,     0,    94,    95,    96,    97,     0,    98,   176,
       0,     0,    25,    26,     0,     0,    27,   156,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,     0,     0,
       0,    37,     0,     0,     0,    38,    39,    40,    41,    94,
      95,    96,    97,     0,    98,    94,    95,    96,    97,     0,
      98,     0,     0,   157,     0,    94,    95,    96,    97,   158,
      98,    94,    95,    96,    97,     0,    98,     0,     0,   159,
       0,    94,    95,    96,    97,   163,    98,    94,    95,    96,
      97,     0,    98,     0,     0,   164,     0,    94,    95,    96,
      97,   192,    98,     0,     0,     0,     0,     0,     0,     0,
       0,   193
};

static const yytype_int16 yycheck[] =
{
       3,     1,     2,    98,    98,     5,     6,    67,     4,     5,
      14,   113,     3,    24,    25,    26,    27,    21,    29,    49,
      50,     0,    25,    26,    45,    24,    25,    26,    27,    44,
      29,    44,    35,    44,    37,    24,    25,    26,    27,    17,
      29,    26,    27,    39,    29,    44,    44,    50,    51,    24,
      25,    26,    27,    37,    29,    44,    37,   152,   152,     4,
       5,    37,    62,    38,    37,    65,    37,   127,    71,    72,
      73,    74,    75,    76,    77,    78,    24,    25,    26,    27,
      37,    29,    85,    86,    87,    88,    89,    37,    91,    92,
      93,    94,    95,    96,    97,    43,    12,    13,     3,     4,
       5,   203,    37,    37,   104,    37,   109,   107,   203,   203,
     212,     6,     7,     8,     9,    10,    11,   212,   212,    24,
      25,    44,    37,    28,    37,    30,    31,    32,    33,    34,
      35,    36,    37,    37,    39,    44,    37,    44,    43,    17,
      37,   144,    47,    48,    49,    50,    51,   147,    29,    39,
      44,   151,    24,    25,    26,    27,    41,    29,    40,    38,
      44,    38,   165,   166,    44,    38,    38,    24,    25,    26,
      27,    42,    29,     3,     4,     5,    44,    24,    25,    26,
      27,    38,    29,   183,    17,    24,    25,    26,    27,   189,
      29,   194,    16,   196,    24,    25,    44,    41,    28,    38,
      30,    31,    32,    33,    34,    35,    36,    37,    42,    39,
      38,    61,    44,    43,     3,     4,     5,    47,    48,    49,
      50,    62,    52,    18,    19,    20,    15,    65,    89,    24,
      25,    26,    27,   107,    29,    24,    25,   102,   104,    28,
     183,    30,    31,    32,    33,    34,    35,    36,    37,   189,
      39,     3,     4,     5,    43,   194,   212,    -1,    47,    48,
      49,    50,    -1,    24,    25,    26,    27,    -1,    29,   152,
      -1,    -1,    24,    25,    -1,    -1,    28,    38,    30,    31,
      32,    33,    34,    35,    36,    37,    -1,    39,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    47,    48,    49,    50,    24,
      25,    26,    27,    -1,    29,    24,    25,    26,    27,    -1,
      29,    -1,    -1,    38,    -1,    24,    25,    26,    27,    38,
      29,    24,    25,    26,    27,    -1,    29,    -1,    -1,    38,
      -1,    24,    25,    26,    27,    38,    29,    24,    25,    26,
      27,    -1,    29,    -1,    -1,    38,    -1,    24,    25,    26,
      27,    38,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     6,     7,     8,     9,    10,    11,    56,    57,    58,
      62,    70,    74,    79,    83,     3,    63,    65,   111,    59,
      61,   111,     4,     5,    15,    24,    25,    28,    30,    31,
      32,    33,    34,    35,    36,    37,    39,    43,    47,    48,
      49,    50,    75,    77,    99,   100,   106,   107,   108,   111,
      49,    50,    80,    82,    84,    86,   111,    71,    73,   111,
       0,    45,    44,    64,    66,    44,    60,    17,    78,    99,
      99,    37,    37,    37,    37,    37,    37,    37,    37,    99,
      51,    98,    99,   109,    99,    37,    37,    37,    37,    44,
      76,    18,    19,    20,    24,    25,    26,    27,    29,    37,
      99,    99,    44,    81,    44,    85,    37,    44,    72,    17,
      56,    63,    14,    21,    67,    59,    98,    37,    99,    99,
      99,    99,    99,    99,    99,    99,    38,    44,    43,    99,
      99,    99,    99,    75,    99,    99,    99,    99,    99,    99,
      99,   105,   106,   107,   101,    80,    84,    87,    71,    99,
     108,    41,    44,    38,    38,    38,    38,    38,    38,    38,
      52,    98,   110,    38,    38,    44,    44,    99,   102,    88,
      90,   111,    68,    91,    93,   111,   105,    40,    99,    99,
     103,    38,    38,    44,    89,    12,    13,    69,    42,    44,
      92,    38,    38,    38,    44,   104,    17,    88,    16,    44,
      91,   102,    99,    41,    94,    97,   106,   107,   108,    42,
      95,    38,    44,    96,    94
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    55,    56,    56,    57,    57,    57,    57,    57,    57,
      58,    59,    60,    60,    61,    62,    63,    64,    64,    66,
      65,    67,    68,    67,    69,    69,    69,    70,    71,    72,
      72,    73,    74,    75,    76,    76,    77,    77,    77,    78,
      77,    79,    80,    81,    81,    82,    82,    83,    84,    85,
      85,    87,    86,    88,    89,    89,    90,    91,    92,    92,
      93,    95,    94,    96,    96,    97,    97,    97,    98,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,   101,   100,   103,   102,   104,
     104,   105,   105,   106,   107,   108,   109,   109,   110,   110,
     111
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     3,     1,     1,     1,     1,     1,     1,
       2,     2,     0,     2,     3,     2,     2,     0,     2,     0,
       3,     1,     0,     4,     0,     1,     2,     2,     2,     0,
       2,     3,     2,     2,     0,     2,     3,     3,     3,     0,
      11,     2,     2,     0,     2,     2,     2,     2,     2,     0,
       2,     0,     7,     2,     0,     2,     1,     2,     0,     2,
       1,     0,     3,     0,     2,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     6,     2,     2,     4,     4,     4,
       4,     3,     4,     4,     4,     4,     4,     6,     6,     3,
       1,     1,     1,     1,     1,     0,     5,     0,     3,     0,
       2,     1,     1,     1,     1,     5,     1,     1,     1,     1,
       1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = REALPAVER_BISON_EMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == REALPAVER_BISON_EMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use REALPAVER_BISON_error or REALPAVER_BISON_UNDEF. */
#define YYERRCODE REALPAVER_BISON_UNDEF


/* Enable debugging if requested.  */
#if REALPAVER_BISON_DEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !REALPAVER_BISON_DEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !REALPAVER_BISON_DEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = REALPAVER_BISON_EMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == REALPAVER_BISON_EMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= REALPAVER_BISON_EOF)
    {
      yychar = REALPAVER_BISON_EOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == REALPAVER_BISON_error)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = REALPAVER_BISON_UNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = REALPAVER_BISON_EMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 14: /* const_def: ident TK_EQ const_expr  */
#line 105 "parser_bison.ypp"
    {
      realpaver::Term::SharedRep e((yyvsp[0].u_term));

      realpaver::Interval x = e->evalConst();
      if (x.isEmpty())
      {
        std::ostringstream os;
        os << "Constant [" << (yyvsp[-2].u_str) << "] equal to an empty interval";
        realpaver_bison_error(os.str().c_str());
        YYABORT;
      }

      if (realpaver_bison_symtab->hasSymbol((yyvsp[-2].u_str)))
      {
        std::ostringstream os;
        os << "Symbol [" << (yyvsp[-2].u_str) << "] already used";
        realpaver_bison_error(os.str().c_str());
        YYABORT;         
      }

      realpaver_bison_symtab->insertConstant((yyvsp[-2].u_str), x);
    }
#line 1585 "parser_bison.cpp"
    break;

  case 19: /* $@1: %empty  */
#line 144 "parser_bison.ypp"
    {
      if (realpaver_bison_symtab->hasSymbol((yyvsp[0].u_str)))
      {
        std::ostringstream os;
        os << "Symbol [" << (yyvsp[0].u_str) << "] already used";
        realpaver_bison_error(os.str().c_str());
        YYABORT;         
      }

      realpaver::Variable v =
         realpaver_bison_problem->addRealVar(realpaver::Interval::universe(),
                                             (yyvsp[0].u_str));

      realpaver::Tolerance tol = realpaver_bison_param->getTolParam("XTOL");
      v.setTolerance(tol);

      realpaver_bison_symtab->insertVariable((yyvsp[0].u_str), v);
    }
#line 1608 "parser_bison.cpp"
    break;

  case 21: /* var_dom: TK_STR_BIN  */
#line 167 "parser_bison.ypp"
    {
      realpaver::Variable v = realpaver_bison_problem->lastVar();
      v.setDomain(0, 1);
      v.setDiscrete();
      v.setTolerance(realpaver::Tolerance::makeAbs(0.0));

      // the domain must also be assigned in the problem's initial region
      realpaver_bison_problem->setDomain(v, 0, 1);
    }
#line 1622 "parser_bison.cpp"
    break;

  case 22: /* $@2: %empty  */
#line 177 "parser_bison.ypp"
    {
      realpaver::Term::SharedRep e((yyvsp[0].u_term));
      realpaver::Interval x = e->evalConst();

      if (x.isEmpty())
      {
         realpaver_bison_error("Empty domain");
         YYABORT;
      }

      realpaver::Variable v = realpaver_bison_problem->lastVar();
      v.setDomain(x);

      // the domain must also be assigned in the problem's initial region
      realpaver_bison_problem->setDomain(v, x);
    }
#line 1643 "parser_bison.cpp"
    break;

  case 25: /* var_info: TK_STR_INT  */
#line 199 "parser_bison.ypp"
    {
      realpaver::Variable v = realpaver_bison_problem->lastVar();
      v.setDiscrete();
      v.setTolerance(realpaver::Tolerance::makeAbs(0.0));    
    }
#line 1653 "parser_bison.cpp"
    break;

  case 26: /* var_info: TK_STR_TOL TK_TOL  */
#line 205 "parser_bison.ypp"
    {
      try
      {
        realpaver::Tolerance tol(realpaver_bison_text);
        realpaver::Variable v = realpaver_bison_problem->lastVar();
        v.setTolerance(tol);
      }
      catch(realpaver::Exception& e)
      {
        realpaver_bison_error("Bad tolerance");
        YYABORT;
      }
    }
#line 1671 "parser_bison.cpp"
    break;

  case 31: /* alias_def: ident TK_EQ expr  */
#line 235 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[0].u_term)));

      if (realpaver_bison_symtab->hasSymbol((yyvsp[-2].u_str)))
      {
        std::ostringstream os;
        os << "Symbol [" << (yyvsp[-2].u_str) << "] already used";
        realpaver_bison_error(os.str().c_str());
        YYABORT;         
      }

      realpaver_bison_symtab->insertAlias((yyvsp[-2].u_str), t);
    }
#line 1689 "parser_bison.cpp"
    break;

  case 36: /* ctr_def: expr TK_EQ_EQ expr  */
#line 265 "parser_bison.ypp"
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term)));
      realpaver::Term rhs(realpaver::Term::SharedRep((yyvsp[0].u_term)));

      realpaver::Constraint c(lhs == rhs);
      realpaver_bison_problem->addCtr(c);
    }
#line 1701 "parser_bison.cpp"
    break;

  case 37: /* ctr_def: expr TK_LE expr  */
#line 273 "parser_bison.ypp"
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term)));
      realpaver::Term rhs(realpaver::Term::SharedRep((yyvsp[0].u_term)));

      realpaver::Constraint c(lhs <= rhs);
      realpaver_bison_problem->addCtr(c);
    }
#line 1713 "parser_bison.cpp"
    break;

  case 38: /* ctr_def: expr TK_GE expr  */
#line 281 "parser_bison.ypp"
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term)));
      realpaver::Term rhs(realpaver::Term::SharedRep((yyvsp[0].u_term)));

      realpaver::Constraint c(lhs >= rhs);
      realpaver_bison_problem->addCtr(c);
    }
#line 1725 "parser_bison.cpp"
    break;

  case 39: /* $@3: %empty  */
#line 289 "parser_bison.ypp"
    {
      if (realpaver_var_vector != nullptr)
         realpaver_var_vector->clear();
      else
        realpaver_var_vector = new std::vector<realpaver::Variable>();

      if (realpaver_itv_vector != nullptr)
         realpaver_itv_vector->clear();
      else
        realpaver_itv_vector = new std::vector<realpaver::Interval>();
    }
#line 1741 "parser_bison.cpp"
    break;

  case 40: /* ctr_def: TK_TABLE $@3 TK_LPAR TK_LCBR table_list_var TK_RCBR TK_COMMA TK_LCBR table_list_val TK_RCBR TK_RPAR  */
#line 302 "parser_bison.ypp"
    {

      if (realpaver_var_vector->size() == 0 ||
          realpaver_itv_vector->size() % realpaver_var_vector->size() != 0)
      {
         realpaver_bison_error("Bad initialization of a table constraint");
         YYABORT;
      }

      realpaver::Constraint c( table(realpaver_var_vector->data(),
                                     realpaver_var_vector->size(),
                                     realpaver_itv_vector->data(),
                                     realpaver_itv_vector->size()) );

      realpaver_bison_problem->addCtr(c);
    }
#line 1762 "parser_bison.cpp"
    break;

  case 45: /* obj_def: TK_MIN expr  */
#line 335 "parser_bison.ypp"
    {
      realpaver::Term f(realpaver::Term::SharedRep((yyvsp[0].u_term)));
      if (f.isConstant())
      {
        realpaver_bison_error("Constant objective function");
        YYABORT;
      }

      realpaver_bison_problem->addObjective(MIN(f));
    }
#line 1777 "parser_bison.cpp"
    break;

  case 46: /* obj_def: TK_MAX expr  */
#line 346 "parser_bison.ypp"
    {
      realpaver::Term f(realpaver::Term::SharedRep((yyvsp[0].u_term)));
      if (f.isConstant())
      {
        realpaver_bison_error("Constant objective function");
        YYABORT;
      }

      realpaver_bison_problem->addObjective(MAX(f));       
    }
#line 1792 "parser_bison.cpp"
    break;

  case 51: /* $@4: %empty  */
#line 373 "parser_bison.ypp"
    {       
      if (realpaver_bison_symtab->hasSymbol((yyvsp[-1].u_str)))
      {
        std::ostringstream os;
        os << "Symbol [" << (yyvsp[-1].u_str) << "] already used";
        realpaver_bison_error(os.str().c_str());
        YYABORT;         
      }

      realpaver_fun_symbol = realpaver_bison_symtab->insertFunction((yyvsp[-1].u_str));
    }
#line 1808 "parser_bison.cpp"
    break;

  case 52: /* fun_def: ident TK_LPAR $@4 arg_list TK_RPAR TK_EQ expr  */
#line 385 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[0].u_term)));
      bool ok = realpaver_fun_symbol->setTerm(t);      
      realpaver_fun_symbol = nullptr;

      if (!ok)
      {
        realpaver_bison_error("Bad variable in a function expression");
        YYABORT;                  
      }
    }
#line 1824 "parser_bison.cpp"
    break;

  case 56: /* arg_def: ident  */
#line 409 "parser_bison.ypp"
    {
      if (realpaver_fun_symbol->hasArgument((yyvsp[0].u_str)))
      {
        std::ostringstream os;
        os << "Argument [" << (yyvsp[0].u_str) << "] already used";
        realpaver_bison_error(os.str().c_str());
        YYABORT;
      }

      realpaver_fun_symbol->addArgument((yyvsp[0].u_str));
    }
#line 1840 "parser_bison.cpp"
    break;

  case 60: /* table_list_var_def: ident  */
#line 433 "parser_bison.ypp"
    {
      realpaver::VariableSymbol* vs =
         realpaver_bison_symtab->findVariable((yyvsp[0].u_str));

      if (vs != nullptr)
      {
         realpaver_var_vector->push_back(vs->getVar());
      }
      else
      {
         std::ostringstream os;
         os << "Identifier [" << (yyvsp[0].u_str) << "] not found";
         realpaver_bison_error(os.str().c_str());
         YYABORT;
      }
    }
#line 1861 "parser_bison.cpp"
    break;

  case 61: /* $@5: %empty  */
#line 453 "parser_bison.ypp"
    {
      realpaver::TermConst* cst = static_cast<realpaver::TermConst*>((yyvsp[0].u_term));
      realpaver::Interval x(cst->evalConst());
      delete cst;
      
      if (x.isEmpty())
      {
        realpaver_bison_error("Empty interval in a table constraint");
        YYABORT;
      }
      else
      {
        realpaver_itv_vector->push_back(x);
      }
    }
#line 1881 "parser_bison.cpp"
    break;

  case 65: /* table_list_val_def: itv  */
#line 478 "parser_bison.ypp"
    {
       (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 1889 "parser_bison.cpp"
    break;

  case 66: /* table_list_val_def: real  */
#line 482 "parser_bison.ypp"
    {
       (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 1897 "parser_bison.cpp"
    break;

  case 67: /* table_list_val_def: integer  */
#line 486 "parser_bison.ypp"
    {
       (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 1905 "parser_bison.cpp"
    break;

  case 68: /* const_expr: expr  */
#line 493 "parser_bison.ypp"
    {
      realpaver::TermRep* t = (yyvsp[0].u_term);

      if (t->isConstant()) (yyval.u_term) = (yyvsp[0].u_term);
      else
      {
        delete t;
        realpaver_bison_error("Expression not constant");
        YYABORT;
      }
    }
#line 1921 "parser_bison.cpp"
    break;

  case 69: /* expr: expr TK_PLUS expr  */
#line 508 "parser_bison.ypp"
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term))),
                      rhs(realpaver::Term::SharedRep((yyvsp[0].u_term))),
                      res(lhs + rhs);
      (yyval.u_term) = res.cloneRoot();
    }
#line 1932 "parser_bison.cpp"
    break;

  case 70: /* expr: expr TK_MINUS expr  */
#line 515 "parser_bison.ypp"
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term))),
                      rhs(realpaver::Term::SharedRep((yyvsp[0].u_term))),
                      res(lhs - rhs);
      (yyval.u_term) = res.cloneRoot();
    }
#line 1943 "parser_bison.cpp"
    break;

  case 71: /* expr: expr TK_MUL expr  */
#line 522 "parser_bison.ypp"
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term))),
                      rhs(realpaver::Term::SharedRep((yyvsp[0].u_term))),
                      res(lhs * rhs);
      (yyval.u_term) = res.cloneRoot();
    }
#line 1954 "parser_bison.cpp"
    break;

  case 72: /* expr: expr TK_DIV expr  */
#line 529 "parser_bison.ypp"
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term))),
                      rhs(realpaver::Term::SharedRep((yyvsp[0].u_term))),
                      res(lhs / rhs);
      (yyval.u_term) = res.cloneRoot();
    }
#line 1965 "parser_bison.cpp"
    break;

  case 73: /* expr: expr TK_CARET exponent  */
#line 536 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-2].u_term))),
                      d(realpaver::Term::SharedRep((yyvsp[0].u_term))),
                      res(pow(t, d.evalConst()));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1976 "parser_bison.cpp"
    break;

  case 74: /* expr: TK_POW TK_LPAR expr TK_COMMA exponent TK_RPAR  */
#line 543 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-3].u_term))),
                      d(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(pow(t, d.evalConst()));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1987 "parser_bison.cpp"
    break;

  case 75: /* expr: TK_MINUS expr  */
#line 550 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[0].u_term))),
                      res(-t);
      (yyval.u_term) = res.cloneRoot();
    }
#line 1997 "parser_bison.cpp"
    break;

  case 76: /* expr: TK_PLUS expr  */
#line 556 "parser_bison.ypp"
    {
       (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2005 "parser_bison.cpp"
    break;

  case 77: /* expr: TK_SQRT TK_LPAR expr TK_RPAR  */
#line 560 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(sqrt(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2015 "parser_bison.cpp"
    break;

  case 78: /* expr: TK_SQR TK_LPAR expr TK_RPAR  */
#line 566 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(sqr(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2025 "parser_bison.cpp"
    break;

  case 79: /* expr: TK_SGN TK_LPAR expr TK_RPAR  */
#line 572 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(sgn(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2035 "parser_bison.cpp"
    break;

  case 80: /* expr: TK_ABS TK_LPAR expr TK_RPAR  */
#line 578 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(abs(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2045 "parser_bison.cpp"
    break;

  case 81: /* expr: TK_PIPE expr TK_PIPE  */
#line 584 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(abs(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2055 "parser_bison.cpp"
    break;

  case 82: /* expr: TK_LOG TK_LPAR expr TK_RPAR  */
#line 590 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(log(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2065 "parser_bison.cpp"
    break;

  case 83: /* expr: TK_EXP TK_LPAR expr TK_RPAR  */
#line 596 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(exp(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2075 "parser_bison.cpp"
    break;

  case 84: /* expr: TK_COS TK_LPAR expr TK_RPAR  */
#line 602 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(cos(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2085 "parser_bison.cpp"
    break;

  case 85: /* expr: TK_SIN TK_LPAR expr TK_RPAR  */
#line 608 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(sin(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2095 "parser_bison.cpp"
    break;

  case 86: /* expr: TK_TAN TK_LPAR expr TK_RPAR  */
#line 614 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(tan(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2105 "parser_bison.cpp"
    break;

  case 87: /* expr: TK_MIN TK_LPAR expr TK_COMMA expr TK_RPAR  */
#line 620 "parser_bison.ypp"
    {
      realpaver::Term fst(realpaver::Term::SharedRep((yyvsp[-3].u_term))),
                      snd(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(MIN(fst, snd));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2116 "parser_bison.cpp"
    break;

  case 88: /* expr: TK_MAX TK_LPAR expr TK_COMMA expr TK_RPAR  */
#line 627 "parser_bison.ypp"
    {
      realpaver::Term fst(realpaver::Term::SharedRep((yyvsp[-3].u_term))),
                      snd(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(MAX(fst, snd));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2127 "parser_bison.cpp"
    break;

  case 89: /* expr: TK_LPAR expr TK_RPAR  */
#line 634 "parser_bison.ypp"
  {
    (yyval.u_term) = (yyvsp[-1].u_term);
  }
#line 2135 "parser_bison.cpp"
    break;

  case 90: /* expr: fun_call  */
#line 638 "parser_bison.ypp"
    {
      (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2143 "parser_bison.cpp"
    break;

  case 91: /* expr: ident  */
#line 642 "parser_bison.ypp"
    {
      bool found = false;

      // an argument?
      if (realpaver_fun_symbol != nullptr &&
          realpaver_fun_symbol->hasArgument((yyvsp[0].u_str)))
      {
        realpaver::Variable v = realpaver_fun_symbol->getVar((yyvsp[0].u_str));
        (yyval.u_term) = new realpaver::TermVar(v);
        found = true;
      }

      if (!found)
      {
        // a constant?
        realpaver::ConstantSymbol* cs =
          realpaver_bison_symtab->findConstant((yyvsp[0].u_str));

        if (cs != nullptr)
        {
           (yyval.u_term) = new realpaver::TermConst(cs->getValue());
           found = true;
        }
     }

      if (!found)
      {
        // a variable?
        realpaver::VariableSymbol* vs =
          realpaver_bison_symtab->findVariable((yyvsp[0].u_str));

        if (vs != nullptr)
        {
           (yyval.u_term) = new realpaver::TermVar(vs->getVar());
           found = true;
        }
      }

      if (!found)
      {
        // an alias?
        realpaver::AliasSymbol* as = realpaver_bison_symtab->findAlias((yyvsp[0].u_str));

        if (as != nullptr)
        {
           (yyval.u_term) = as->getTerm().cloneRoot();
           found = true;
        }
      }

      // symbol not found
      if (!found)
      {
        std::ostringstream os;
        os << "Identifier [" << (yyvsp[0].u_str) << "] not found";
        realpaver_bison_error(os.str().c_str());
        YYABORT;
      }
    }
#line 2207 "parser_bison.cpp"
    break;

  case 92: /* expr: integer  */
#line 702 "parser_bison.ypp"
    {
      (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2215 "parser_bison.cpp"
    break;

  case 93: /* expr: real  */
#line 706 "parser_bison.ypp"
    {
      (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2223 "parser_bison.cpp"
    break;

  case 94: /* expr: itv  */
#line 710 "parser_bison.ypp"
    {
       (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2231 "parser_bison.cpp"
    break;

  case 95: /* $@6: %empty  */
#line 717 "parser_bison.ypp"
    {
      realpaver::FunctionSymbol* fs =
          realpaver_bison_symtab->findFunction((yyvsp[-1].u_str));

      if (fs == nullptr)
      {
        std::ostringstream os;
        os << "Function [" << (yyvsp[-1].u_str) << "] not found";
        realpaver_bison_error(os.str().c_str());
        YYABORT;
      }

      realpaver_bison_symtab->pushFunctionCall(fs);
    }
#line 2250 "parser_bison.cpp"
    break;

  case 96: /* fun_call: ident TK_LPAR $@6 expr_list TK_RPAR  */
#line 732 "parser_bison.ypp"
    {
       std::pair<bool, realpaver::Term> res =
         realpaver_bison_symtab->processFunCall();

       if (res.first)
       {
         (yyval.u_term) = res.second.cloneRoot();
       }
       else
       {
         realpaver_bison_error("Function call invalid");
         YYABORT;          
       }
    }
#line 2269 "parser_bison.cpp"
    break;

  case 97: /* $@7: %empty  */
#line 750 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[0].u_term)));
      realpaver_bison_symtab->addFunctionArgument(t);
    }
#line 2278 "parser_bison.cpp"
    break;

  case 101: /* exponent: integer  */
#line 764 "parser_bison.ypp"
    {
      (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2286 "parser_bison.cpp"
    break;

  case 102: /* exponent: real  */
#line 768 "parser_bison.ypp"
    {
      (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2294 "parser_bison.cpp"
    break;

  case 103: /* integer: TK_INT  */
#line 775 "parser_bison.ypp"
    {
      try
      {
        std::string str(realpaver_bison_text);
        realpaver::Interval x(str);
        (yyval.u_term) = new realpaver::TermConst(x);
      }
      catch(realpaver::Exception& e)
      {
        realpaver_bison_error("Bad integer value");
        YYABORT;
      }       
    }
#line 2312 "parser_bison.cpp"
    break;

  case 104: /* real: TK_REAL  */
#line 792 "parser_bison.ypp"
    {
      try
      {
        std::string str(realpaver_bison_text);
        realpaver::Interval x(str);
        (yyval.u_term) = new realpaver::TermConst(x);
      }
      catch(realpaver::Exception& e)
      {
        realpaver_bison_error("Bad real number");
        YYABORT;
      }       
    }
#line 2330 "parser_bison.cpp"
    break;

  case 105: /* itv: TK_LSBR itv_left TK_COMMA itv_right TK_RSBR  */
#line 809 "parser_bison.ypp"
    {
      realpaver::Term::SharedRep lo((yyvsp[-3].u_term));
      realpaver::Term::SharedRep up((yyvsp[-1].u_term));

      realpaver::Interval x = lo->evalConst();
      realpaver::Interval y = up->evalConst();
      realpaver::Interval z(x.left(), y.right());

      if (z.isEmpty())
      {
        realpaver_bison_error("Empty interval");
        YYABORT;
      }

      (yyval.u_term) = new realpaver::TermConst(z);
    }
#line 2351 "parser_bison.cpp"
    break;

  case 106: /* itv_left: const_expr  */
#line 829 "parser_bison.ypp"
    {
      (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2359 "parser_bison.cpp"
    break;

  case 107: /* itv_left: TK_MINUS_INF  */
#line 833 "parser_bison.ypp"
    {
      // it is necessary to return a non-empty interval with -inf as left bound
      (yyval.u_term) = new realpaver::TermConst(realpaver::Interval::universe());
    }
#line 2368 "parser_bison.cpp"
    break;

  case 108: /* itv_right: const_expr  */
#line 841 "parser_bison.ypp"
    {
      (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2376 "parser_bison.cpp"
    break;

  case 109: /* itv_right: TK_PLUS_INF  */
#line 845 "parser_bison.ypp"
    {
      // it is necessary to return a non-empty interval with +inf as right bound
      (yyval.u_term) = new realpaver::TermConst(realpaver::Interval::universe());
    }
#line 2385 "parser_bison.cpp"
    break;

  case 110: /* ident: TK_IDENT  */
#line 853 "parser_bison.ypp"
   {
     strcpy((yyval.u_str), realpaver_bison_text);
   }
#line 2393 "parser_bison.cpp"
    break;


#line 2397 "parser_bison.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == REALPAVER_BISON_EMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= REALPAVER_BISON_EOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == REALPAVER_BISON_EOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = REALPAVER_BISON_EMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != REALPAVER_BISON_EMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 857 "parser_bison.ypp"


int realpaver_bison_error(const char* str)
{
  std::ostringstream os;   
  os << "l" << realpaver_bison_lineno << ": " << str;

  realpaver_parse_error = os.str();

  return 0;
}
