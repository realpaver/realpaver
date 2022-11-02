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
    TK_TABLE = 268,                /* TK_TABLE  */
    TK_TOL = 269,                  /* TK_TOL  */
    TK_AT = 270,                   /* TK_AT  */
    TK_EQ = 271,                   /* TK_EQ  */
    TK_EQ_EQ = 272,                /* TK_EQ_EQ  */
    TK_LE = 273,                   /* TK_LE  */
    TK_GE = 274,                   /* TK_GE  */
    TK_IN = 275,                   /* TK_IN  */
    TK_LT = 276,                   /* TK_LT  */
    TK_GT = 277,                   /* TK_GT  */
    TK_PLUS = 278,                 /* TK_PLUS  */
    TK_MINUS = 279,                /* TK_MINUS  */
    TK_MUL = 280,                  /* TK_MUL  */
    TK_DIV = 281,                  /* TK_DIV  */
    TK_POW = 282,                  /* TK_POW  */
    TK_CARET = 283,                /* TK_CARET  */
    TK_SQR = 284,                  /* TK_SQR  */
    TK_SQRT = 285,                 /* TK_SQRT  */
    TK_LOG = 286,                  /* TK_LOG  */
    TK_EXP = 287,                  /* TK_EXP  */
    TK_TAN = 288,                  /* TK_TAN  */
    TK_COS = 289,                  /* TK_COS  */
    TK_SIN = 290,                  /* TK_SIN  */
    TK_LPAR = 291,                 /* TK_LPAR  */
    TK_RPAR = 292,                 /* TK_RPAR  */
    TK_LSBR = 293,                 /* TK_LSBR  */
    TK_RSBR = 294,                 /* TK_RSBR  */
    TK_LCBR = 295,                 /* TK_LCBR  */
    TK_RCBR = 296,                 /* TK_RCBR  */
    TK_PIPE = 297,                 /* TK_PIPE  */
    TK_COMMA = 298,                /* TK_COMMA  */
    TK_SCOLON = 299,               /* TK_SCOLON  */
    TK_COLON = 300,                /* TK_COLON  */
    TK_SGN = 301,                  /* TK_SGN  */
    TK_ABS = 302,                  /* TK_ABS  */
    TK_MIN = 303,                  /* TK_MIN  */
    TK_MAX = 304,                  /* TK_MAX  */
    TK_UMINUS = 305,               /* TK_UMINUS  */
    TK_UPLUS = 306                 /* TK_UPLUS  */
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
#define TK_TABLE 268
#define TK_TOL 269
#define TK_AT 270
#define TK_EQ 271
#define TK_EQ_EQ 272
#define TK_LE 273
#define TK_GE 274
#define TK_IN 275
#define TK_LT 276
#define TK_GT 277
#define TK_PLUS 278
#define TK_MINUS 279
#define TK_MUL 280
#define TK_DIV 281
#define TK_POW 282
#define TK_CARET 283
#define TK_SQR 284
#define TK_SQRT 285
#define TK_LOG 286
#define TK_EXP 287
#define TK_TAN 288
#define TK_COS 289
#define TK_SIN 290
#define TK_LPAR 291
#define TK_RPAR 292
#define TK_LSBR 293
#define TK_RSBR 294
#define TK_LCBR 295
#define TK_RCBR 296
#define TK_PIPE 297
#define TK_COMMA 298
#define TK_SCOLON 299
#define TK_COLON 300
#define TK_SGN 301
#define TK_ABS 302
#define TK_MIN 303
#define TK_MAX 304
#define TK_UMINUS 305
#define TK_UPLUS 306

/* Value type.  */
#if ! defined REALPAVER_BISON_STYPE && ! defined REALPAVER_BISON_STYPE_IS_DECLARED
union REALPAVER_BISON_STYPE
{
#line 37 "parser_bison.ypp"

  char u_str[256];
  realpaver::TermRep* u_term;
  realpaver::Tolerance* u_tol;

#line 282 "parser_bison.cpp"

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
  YYSYMBOL_TK_TABLE = 13,                  /* TK_TABLE  */
  YYSYMBOL_TK_TOL = 14,                    /* TK_TOL  */
  YYSYMBOL_TK_AT = 15,                     /* TK_AT  */
  YYSYMBOL_TK_EQ = 16,                     /* TK_EQ  */
  YYSYMBOL_TK_EQ_EQ = 17,                  /* TK_EQ_EQ  */
  YYSYMBOL_TK_LE = 18,                     /* TK_LE  */
  YYSYMBOL_TK_GE = 19,                     /* TK_GE  */
  YYSYMBOL_TK_IN = 20,                     /* TK_IN  */
  YYSYMBOL_TK_LT = 21,                     /* TK_LT  */
  YYSYMBOL_TK_GT = 22,                     /* TK_GT  */
  YYSYMBOL_TK_PLUS = 23,                   /* TK_PLUS  */
  YYSYMBOL_TK_MINUS = 24,                  /* TK_MINUS  */
  YYSYMBOL_TK_MUL = 25,                    /* TK_MUL  */
  YYSYMBOL_TK_DIV = 26,                    /* TK_DIV  */
  YYSYMBOL_TK_POW = 27,                    /* TK_POW  */
  YYSYMBOL_TK_CARET = 28,                  /* TK_CARET  */
  YYSYMBOL_TK_SQR = 29,                    /* TK_SQR  */
  YYSYMBOL_TK_SQRT = 30,                   /* TK_SQRT  */
  YYSYMBOL_TK_LOG = 31,                    /* TK_LOG  */
  YYSYMBOL_TK_EXP = 32,                    /* TK_EXP  */
  YYSYMBOL_TK_TAN = 33,                    /* TK_TAN  */
  YYSYMBOL_TK_COS = 34,                    /* TK_COS  */
  YYSYMBOL_TK_SIN = 35,                    /* TK_SIN  */
  YYSYMBOL_TK_LPAR = 36,                   /* TK_LPAR  */
  YYSYMBOL_TK_RPAR = 37,                   /* TK_RPAR  */
  YYSYMBOL_TK_LSBR = 38,                   /* TK_LSBR  */
  YYSYMBOL_TK_RSBR = 39,                   /* TK_RSBR  */
  YYSYMBOL_TK_LCBR = 40,                   /* TK_LCBR  */
  YYSYMBOL_TK_RCBR = 41,                   /* TK_RCBR  */
  YYSYMBOL_TK_PIPE = 42,                   /* TK_PIPE  */
  YYSYMBOL_TK_COMMA = 43,                  /* TK_COMMA  */
  YYSYMBOL_TK_SCOLON = 44,                 /* TK_SCOLON  */
  YYSYMBOL_TK_COLON = 45,                  /* TK_COLON  */
  YYSYMBOL_TK_SGN = 46,                    /* TK_SGN  */
  YYSYMBOL_TK_ABS = 47,                    /* TK_ABS  */
  YYSYMBOL_TK_MIN = 48,                    /* TK_MIN  */
  YYSYMBOL_TK_MAX = 49,                    /* TK_MAX  */
  YYSYMBOL_TK_UMINUS = 50,                 /* TK_UMINUS  */
  YYSYMBOL_TK_UPLUS = 51,                  /* TK_UPLUS  */
  YYSYMBOL_YYACCEPT = 52,                  /* $accept  */
  YYSYMBOL_model = 53,                     /* model  */
  YYSYMBOL_statement = 54,                 /* statement  */
  YYSYMBOL_const_statement = 55,           /* const_statement  */
  YYSYMBOL_const_list = 56,                /* const_list  */
  YYSYMBOL_const_tail = 57,                /* const_tail  */
  YYSYMBOL_const_def = 58,                 /* const_def  */
  YYSYMBOL_var_statement = 59,             /* var_statement  */
  YYSYMBOL_var_list = 60,                  /* var_list  */
  YYSYMBOL_var_tail = 61,                  /* var_tail  */
  YYSYMBOL_var_def = 62,                   /* var_def  */
  YYSYMBOL_var_tol = 63,                   /* var_tol  */
  YYSYMBOL_alias_statement = 64,           /* alias_statement  */
  YYSYMBOL_alias_list = 65,                /* alias_list  */
  YYSYMBOL_alias_tail = 66,                /* alias_tail  */
  YYSYMBOL_alias_def = 67,                 /* alias_def  */
  YYSYMBOL_ctr_statement = 68,             /* ctr_statement  */
  YYSYMBOL_ctr_list = 69,                  /* ctr_list  */
  YYSYMBOL_ctr_tail = 70,                  /* ctr_tail  */
  YYSYMBOL_ctr_def = 71,                   /* ctr_def  */
  YYSYMBOL_72_1 = 72,                      /* $@1  */
  YYSYMBOL_obj_statement = 73,             /* obj_statement  */
  YYSYMBOL_obj_list = 74,                  /* obj_list  */
  YYSYMBOL_obj_tail = 75,                  /* obj_tail  */
  YYSYMBOL_obj_def = 76,                   /* obj_def  */
  YYSYMBOL_fun_statement = 77,             /* fun_statement  */
  YYSYMBOL_fun_list = 78,                  /* fun_list  */
  YYSYMBOL_fun_tail = 79,                  /* fun_tail  */
  YYSYMBOL_fun_def = 80,                   /* fun_def  */
  YYSYMBOL_81_2 = 81,                      /* $@2  */
  YYSYMBOL_arg_list = 82,                  /* arg_list  */
  YYSYMBOL_arg_tail = 83,                  /* arg_tail  */
  YYSYMBOL_arg_def = 84,                   /* arg_def  */
  YYSYMBOL_int_statement = 85,             /* int_statement  */
  YYSYMBOL_int_list = 86,                  /* int_list  */
  YYSYMBOL_int_tail = 87,                  /* int_tail  */
  YYSYMBOL_int_def = 88,                   /* int_def  */
  YYSYMBOL_table_list_var = 89,            /* table_list_var  */
  YYSYMBOL_table_list_var_tail = 90,       /* table_list_var_tail  */
  YYSYMBOL_table_list_var_def = 91,        /* table_list_var_def  */
  YYSYMBOL_table_list_val = 92,            /* table_list_val  */
  YYSYMBOL_93_3 = 93,                      /* $@3  */
  YYSYMBOL_table_list_val_tail = 94,       /* table_list_val_tail  */
  YYSYMBOL_table_list_val_def = 95,        /* table_list_val_def  */
  YYSYMBOL_const_expr = 96,                /* const_expr  */
  YYSYMBOL_expr = 97,                      /* expr  */
  YYSYMBOL_fun_call = 98,                  /* fun_call  */
  YYSYMBOL_99_4 = 99,                      /* $@4  */
  YYSYMBOL_expr_list = 100,                /* expr_list  */
  YYSYMBOL_101_5 = 101,                    /* $@5  */
  YYSYMBOL_expr_tail = 102,                /* expr_tail  */
  YYSYMBOL_exponent = 103,                 /* exponent  */
  YYSYMBOL_integer = 104,                  /* integer  */
  YYSYMBOL_real = 105,                     /* real  */
  YYSYMBOL_itv = 106,                      /* itv  */
  YYSYMBOL_domain = 107,                   /* domain  */
  YYSYMBOL_ident = 108                     /* ident  */
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
#define YYFINAL  65
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   313

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  111
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  219

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   306


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
      45,    46,    47,    48,    49,    50,    51
};

#if REALPAVER_BISON_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    77,    77,    78,    82,    83,    84,    85,    86,    87,
      88,    92,    96,   100,   101,   105,   131,   135,   139,   140,
     144,   169,   173,   188,   192,   196,   197,   201,   218,   222,
     226,   227,   231,   239,   247,   256,   255,   288,   292,   296,
     297,   301,   312,   326,   330,   334,   335,   340,   339,   366,
     370,   371,   375,   390,   394,   398,   399,   403,   422,   426,
     427,   431,   452,   451,   471,   472,   476,   480,   484,   491,
     506,   513,   520,   527,   534,   541,   548,   554,   558,   564,
     570,   576,   582,   588,   594,   600,   606,   612,   618,   625,
     632,   636,   640,   700,   704,   708,   716,   715,   749,   748,
     757,   758,   762,   766,   773,   790,   807,   828,   831,   839,
     847,   863
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
  "TK_STR_FUN", "TK_STR_ALIAS", "TK_STR_INT", "TK_TABLE", "TK_TOL",
  "TK_AT", "TK_EQ", "TK_EQ_EQ", "TK_LE", "TK_GE", "TK_IN", "TK_LT",
  "TK_GT", "TK_PLUS", "TK_MINUS", "TK_MUL", "TK_DIV", "TK_POW", "TK_CARET",
  "TK_SQR", "TK_SQRT", "TK_LOG", "TK_EXP", "TK_TAN", "TK_COS", "TK_SIN",
  "TK_LPAR", "TK_RPAR", "TK_LSBR", "TK_RSBR", "TK_LCBR", "TK_RCBR",
  "TK_PIPE", "TK_COMMA", "TK_SCOLON", "TK_COLON", "TK_SGN", "TK_ABS",
  "TK_MIN", "TK_MAX", "TK_UMINUS", "TK_UPLUS", "$accept", "model",
  "statement", "const_statement", "const_list", "const_tail", "const_def",
  "var_statement", "var_list", "var_tail", "var_def", "var_tol",
  "alias_statement", "alias_list", "alias_tail", "alias_def",
  "ctr_statement", "ctr_list", "ctr_tail", "ctr_def", "$@1",
  "obj_statement", "obj_list", "obj_tail", "obj_def", "fun_statement",
  "fun_list", "fun_tail", "fun_def", "$@2", "arg_list", "arg_tail",
  "arg_def", "int_statement", "int_list", "int_tail", "int_def",
  "table_list_var", "table_list_var_tail", "table_list_var_def",
  "table_list_val", "$@3", "table_list_val_tail", "table_list_val_def",
  "const_expr", "expr", "fun_call", "$@4", "expr_list", "$@5", "expr_tail",
  "exponent", "integer", "real", "itv", "domain", "ident", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-94)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     295,     1,     1,    98,   -35,     1,     1,     1,    10,   -29,
     -94,   -94,   -94,   -94,   -94,   -94,   -94,   -94,   -94,   -13,
      21,   -94,    -6,    31,   -94,   -94,   -94,   176,   176,     9,
      12,    15,    20,    22,    23,    28,    30,   176,   176,   176,
      32,    33,    49,    50,   -94,    44,   272,   -94,   -94,   -94,
     -94,    57,   176,   176,   -94,    65,   -94,    74,    83,   -94,
      77,    59,   -94,    92,   -94,   -94,   295,     1,   -94,   176,
     176,    88,   122,     1,   -94,   176,   102,   113,   113,   176,
     176,   176,   176,   176,   176,   176,   176,    37,    99,   285,
     135,   176,   176,   176,   176,    98,   -94,   176,   176,   176,
     176,   176,   176,   176,    39,   -94,   285,   285,   -35,   -94,
       1,   -94,   -94,     1,   -94,   176,     1,   -94,   -94,   -94,
     -94,   -94,   -94,   129,   -94,   -94,   -94,   109,    -5,    81,
     141,   160,   203,   209,   219,   225,   -94,   176,   -94,   235,
     241,     3,    29,   -94,   285,   285,   285,     7,     7,   113,
     113,   -94,   -94,   -94,   176,   -94,   -94,     1,   -94,   285,
     -94,   -94,     1,    39,   -94,   -94,   -94,   -94,   -94,   -94,
     -94,   111,   -94,   -94,   176,   176,   285,   115,   116,   112,
     -94,   127,   130,   -94,   117,   -94,   251,   257,   131,   -94,
     140,     1,   -94,   132,     1,   -94,   -94,   -94,   -94,   176,
     -94,   176,   -94,   136,   -94,   -94,   285,     4,   146,   -94,
     -94,   -94,   -94,   133,   139,   -94,     4,   -94,   -94
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     9,    10,   111,    16,    18,
     107,    11,    13,     0,   104,   105,    35,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    28,    30,     0,    91,    93,    94,
      95,    92,     0,     0,    37,    39,    43,    45,     0,    23,
      25,     0,    53,    55,    57,     1,     2,     0,    17,     0,
       0,     0,    21,     0,    12,     0,     0,    77,    76,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
       0,     0,     0,     0,     0,     0,    29,     0,     0,     0,
       0,     0,     0,     0,     0,    96,    41,    42,     0,    38,
       0,    44,    47,     0,    24,     0,     0,    54,     3,    19,
     108,   109,   110,     0,    20,    14,    15,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,     0,    82,     0,
       0,     0,     0,    31,    32,    33,    34,    70,    71,    72,
      73,    74,   102,   103,     0,    40,    46,     0,    26,    27,
      56,    22,     0,     0,    79,    78,    83,    84,    87,    85,
      86,     0,    80,    81,     0,     0,    98,     0,     0,    50,
      52,     0,    59,    61,     0,   106,     0,     0,   100,    97,
       0,     0,    49,     0,     0,    58,    75,    88,    89,     0,
      99,     0,    51,     0,    60,   101,    48,     0,     0,    62,
      68,    67,    66,     0,    64,    36,     0,    63,    65
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -94,   123,   -94,   -94,   119,   -94,   -94,   -94,   126,   -94,
     -94,   -94,   -94,    82,   -94,   -94,   -94,    95,   -94,   -94,
     -94,   -94,    93,   -94,   -94,   -94,    94,   -94,   -94,   -94,
      11,   -94,   -94,   -94,    97,   -94,   -94,    25,   -94,   -94,
      -1,   -94,   -94,   -94,   -53,    -3,   -94,   -94,    17,   -94,
     -94,    54,   -93,   -92,   -68,   -94,     0
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     8,     9,    10,    21,    74,    22,    11,    18,    68,
      19,   124,    12,    59,   114,    60,    13,    44,    96,    45,
      76,    14,    54,   109,    55,    15,    56,   111,    57,   157,
     178,   192,   179,    16,    62,   117,    63,   181,   195,   182,
     208,   214,   217,   209,    88,    89,    47,   154,   177,   188,
     200,   151,    48,    49,    50,    72,    51
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      46,    20,    23,   122,    17,    58,    61,    64,    24,    25,
      65,   152,   153,    52,    53,    66,   120,   121,   100,   101,
     102,   103,   126,   104,    77,    78,   100,   101,   102,   103,
      67,   104,   102,   103,    87,   104,    90,    73,   163,    69,
      70,    71,    38,    24,    25,    79,   174,    75,    80,   106,
     107,    81,   100,   101,   102,   103,    82,   104,    83,    84,
     100,   101,   102,   103,    85,   104,    86,    20,    91,    92,
     152,   153,   175,    23,   136,   115,   128,   129,   130,   131,
     132,   133,   134,   135,   171,    93,    94,    95,   139,   140,
     141,   142,    46,   105,   144,   145,   146,   147,   148,   149,
     150,    17,    24,    25,   100,   101,   102,   103,   108,   104,
      58,    26,   159,    61,   210,   211,    64,   110,   164,   112,
     113,    27,    28,   210,   211,    29,    38,    30,    31,    32,
      33,    34,    35,    36,    37,   116,    38,   123,   127,   212,
      39,   104,   137,   161,    40,    41,    42,    43,   212,   162,
     185,   176,   189,   190,   196,   191,   201,   180,   100,   101,
     102,   103,   183,   104,   100,   101,   102,   103,   193,   104,
     215,   186,   187,   194,   199,   203,   207,   138,   165,    17,
      24,    25,   216,   100,   101,   102,   103,   213,   104,   118,
     143,   180,   125,   119,   183,   158,   176,   166,   206,    27,
      28,   155,   202,    29,   156,    30,    31,    32,    33,    34,
      35,    36,    37,   160,    38,   218,   205,   184,    39,   204,
       0,     0,    40,    41,    42,    43,   100,   101,   102,   103,
       0,   104,   100,   101,   102,   103,     0,   104,     0,     0,
     167,     0,   100,   101,   102,   103,   168,   104,   100,   101,
     102,   103,     0,   104,     0,     0,   169,     0,   100,   101,
     102,   103,   170,   104,   100,   101,   102,   103,     0,   104,
       0,     0,   172,     0,   100,   101,   102,   103,   173,   104,
     100,   101,   102,   103,     0,   104,     0,     0,   197,    97,
      98,    99,     0,     0,   198,   100,   101,   102,   103,     0,
     104,     1,     2,     3,     4,     5,     6,     7,   100,   101,
     102,   103,     0,   104
};

static const yytype_int16 yycheck[] =
{
       3,     1,     2,    71,     3,     5,     6,     7,     4,     5,
       0,   104,   104,    48,    49,    44,    69,    70,    23,    24,
      25,    26,    75,    28,    27,    28,    23,    24,    25,    26,
      43,    28,    25,    26,    37,    28,    39,    43,    43,    18,
      19,    20,    38,     4,     5,    36,    43,    16,    36,    52,
      53,    36,    23,    24,    25,    26,    36,    28,    36,    36,
      23,    24,    25,    26,    36,    28,    36,    67,    36,    36,
     163,   163,    43,    73,    37,    16,    79,    80,    81,    82,
      83,    84,    85,    86,   137,    36,    36,    43,    91,    92,
      93,    94,    95,    36,    97,    98,    99,   100,   101,   102,
     103,     3,     4,     5,    23,    24,    25,    26,    43,    28,
     110,    13,   115,   113,   207,   207,   116,    43,    37,    36,
      43,    23,    24,   216,   216,    27,    38,    29,    30,    31,
      32,    33,    34,    35,    36,    43,    38,    15,    36,   207,
      42,    28,    43,    14,    46,    47,    48,    49,   216,    40,
      39,   154,    37,    37,    37,    43,    16,   157,    23,    24,
      25,    26,   162,    28,    23,    24,    25,    26,    41,    28,
      37,   174,   175,    43,    43,    43,    40,    42,    37,     3,
       4,     5,    43,    23,    24,    25,    26,    41,    28,    66,
      95,   191,    73,    67,   194,   113,   199,    37,   201,    23,
      24,   108,   191,    27,   110,    29,    30,    31,    32,    33,
      34,    35,    36,   116,    38,   216,   199,   163,    42,   194,
      -1,    -1,    46,    47,    48,    49,    23,    24,    25,    26,
      -1,    28,    23,    24,    25,    26,    -1,    28,    -1,    -1,
      37,    -1,    23,    24,    25,    26,    37,    28,    23,    24,
      25,    26,    -1,    28,    -1,    -1,    37,    -1,    23,    24,
      25,    26,    37,    28,    23,    24,    25,    26,    -1,    28,
      -1,    -1,    37,    -1,    23,    24,    25,    26,    37,    28,
      23,    24,    25,    26,    -1,    28,    -1,    -1,    37,    17,
      18,    19,    -1,    -1,    37,    23,    24,    25,    26,    -1,
      28,     6,     7,     8,     9,    10,    11,    12,    23,    24,
      25,    26,    -1,    28
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     6,     7,     8,     9,    10,    11,    12,    53,    54,
      55,    59,    64,    68,    73,    77,    85,     3,    60,    62,
     108,    56,    58,   108,     4,     5,    13,    23,    24,    27,
      29,    30,    31,    32,    33,    34,    35,    36,    38,    42,
      46,    47,    48,    49,    69,    71,    97,    98,   104,   105,
     106,   108,    48,    49,    74,    76,    78,    80,   108,    65,
      67,   108,    86,    88,   108,     0,    44,    43,    61,    18,
      19,    20,   107,    43,    57,    16,    72,    97,    97,    36,
      36,    36,    36,    36,    36,    36,    36,    97,    96,    97,
      97,    36,    36,    36,    36,    43,    70,    17,    18,    19,
      23,    24,    25,    26,    28,    36,    97,    97,    43,    75,
      43,    79,    36,    43,    66,    16,    43,    87,    53,    60,
      96,    96,   106,    15,    63,    56,    96,    36,    97,    97,
      97,    97,    97,    97,    97,    97,    37,    43,    42,    97,
      97,    97,    97,    69,    97,    97,    97,    97,    97,    97,
      97,   103,   104,   105,    99,    74,    78,    81,    65,    97,
      86,    14,    40,    43,    37,    37,    37,    37,    37,    37,
      37,    96,    37,    37,    43,    43,    97,   100,    82,    84,
     108,    89,    91,   108,   103,    39,    97,    97,   101,    37,
      37,    43,    83,    41,    43,    90,    37,    37,    37,    43,
     102,    16,    82,    43,    89,   100,    97,    40,    92,    95,
     104,   105,   106,    41,    93,    37,    43,    94,    92
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    52,    53,    53,    54,    54,    54,    54,    54,    54,
      54,    55,    56,    57,    57,    58,    59,    60,    61,    61,
      62,    63,    63,    64,    65,    66,    66,    67,    68,    69,
      70,    70,    71,    71,    71,    72,    71,    73,    74,    75,
      75,    76,    76,    77,    78,    79,    79,    81,    80,    82,
      83,    83,    84,    85,    86,    87,    87,    88,    89,    90,
      90,    91,    93,    92,    94,    94,    95,    95,    95,    96,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    99,    98,   101,   100,
     102,   102,   103,   103,   104,   105,   106,   107,   107,   107,
     107,   108
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     0,     2,     3,     2,     2,     0,     2,
       3,     0,     2,     2,     2,     0,     2,     3,     2,     2,
       0,     2,     3,     3,     3,     0,    11,     2,     2,     0,
       2,     2,     2,     2,     2,     0,     2,     0,     7,     2,
       0,     2,     1,     2,     2,     0,     2,     1,     2,     0,
       2,     1,     0,     3,     0,     2,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     6,     2,     2,     4,     4,
       4,     4,     3,     4,     4,     4,     4,     4,     6,     6,
       3,     1,     1,     1,     1,     1,     0,     5,     0,     3,
       0,     2,     1,     1,     1,     1,     5,     0,     2,     2,
       2,     1
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
  case 15: /* const_def: ident TK_EQ const_expr  */
#line 106 "parser_bison.ypp"
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
#line 1567 "parser_bison.cpp"
    break;

  case 20: /* var_def: ident domain var_tol  */
#line 145 "parser_bison.ypp"
    {
      realpaver::Term::SharedRep e((yyvsp[-1].u_term));
      realpaver::Interval x = e->evalConst();

      if (realpaver_bison_symtab->hasSymbol((yyvsp[-2].u_str)))
      {
        std::ostringstream os;
        os << "Symbol [" << (yyvsp[-2].u_str) << "] already used";
        realpaver_bison_error(os.str().c_str());
        YYABORT;         
      }

      realpaver::Variable v = realpaver_bison_problem->addRealVar(x, (yyvsp[-2].u_str));

      realpaver::Tolerance* tol = (yyvsp[0].u_tol);
      v.setTolerance(*tol);
      delete tol;

      realpaver_bison_symtab->insertVariable((yyvsp[-2].u_str), v);
    }
#line 1592 "parser_bison.cpp"
    break;

  case 21: /* var_tol: %empty  */
#line 169 "parser_bison.ypp"
    {
      realpaver::Tolerance tol = realpaver_bison_param->getTolParam("XTOL");
      (yyval.u_tol) = new realpaver::Tolerance(tol);
    }
#line 1601 "parser_bison.cpp"
    break;

  case 22: /* var_tol: TK_AT TK_TOL  */
#line 174 "parser_bison.ypp"
    {
      try
      {
        (yyval.u_tol) = new realpaver::Tolerance(realpaver_bison_text);
      }
      catch(realpaver::Exception e)
      {
        realpaver_bison_error("Bad tolerance");
        YYABORT;
      }
    }
#line 1617 "parser_bison.cpp"
    break;

  case 27: /* alias_def: ident TK_EQ expr  */
#line 202 "parser_bison.ypp"
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
#line 1635 "parser_bison.cpp"
    break;

  case 32: /* ctr_def: expr TK_EQ_EQ expr  */
#line 232 "parser_bison.ypp"
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term)));
      realpaver::Term rhs(realpaver::Term::SharedRep((yyvsp[0].u_term)));

      realpaver::Constraint c(lhs == rhs);
      realpaver_bison_problem->addCtr(c);
    }
#line 1647 "parser_bison.cpp"
    break;

  case 33: /* ctr_def: expr TK_LE expr  */
#line 240 "parser_bison.ypp"
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term)));
      realpaver::Term rhs(realpaver::Term::SharedRep((yyvsp[0].u_term)));

      realpaver::Constraint c(lhs <= rhs);
      realpaver_bison_problem->addCtr(c);
    }
#line 1659 "parser_bison.cpp"
    break;

  case 34: /* ctr_def: expr TK_GE expr  */
#line 248 "parser_bison.ypp"
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term)));
      realpaver::Term rhs(realpaver::Term::SharedRep((yyvsp[0].u_term)));

      realpaver::Constraint c(lhs >= rhs);
      realpaver_bison_problem->addCtr(c);
    }
#line 1671 "parser_bison.cpp"
    break;

  case 35: /* $@1: %empty  */
#line 256 "parser_bison.ypp"
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
#line 1687 "parser_bison.cpp"
    break;

  case 36: /* ctr_def: TK_TABLE $@1 TK_LPAR TK_LCBR table_list_var TK_RCBR TK_COMMA TK_LCBR table_list_val TK_RCBR TK_RPAR  */
#line 269 "parser_bison.ypp"
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
#line 1708 "parser_bison.cpp"
    break;

  case 41: /* obj_def: TK_MIN expr  */
#line 302 "parser_bison.ypp"
    {
      realpaver::Term f(realpaver::Term::SharedRep((yyvsp[0].u_term)));
      if (f.isConstant())
      {
        realpaver_bison_error("Constant objective function");
        YYABORT;
      }

      realpaver_bison_problem->addObjective(MIN(f));
    }
#line 1723 "parser_bison.cpp"
    break;

  case 42: /* obj_def: TK_MAX expr  */
#line 313 "parser_bison.ypp"
    {
      realpaver::Term f(realpaver::Term::SharedRep((yyvsp[0].u_term)));
      if (f.isConstant())
      {
        realpaver_bison_error("Constant objective function");
        YYABORT;
      }

      realpaver_bison_problem->addObjective(MAX(f));       
    }
#line 1738 "parser_bison.cpp"
    break;

  case 47: /* $@2: %empty  */
#line 340 "parser_bison.ypp"
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
#line 1754 "parser_bison.cpp"
    break;

  case 48: /* fun_def: ident TK_LPAR $@2 arg_list TK_RPAR TK_EQ expr  */
#line 352 "parser_bison.ypp"
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
#line 1770 "parser_bison.cpp"
    break;

  case 52: /* arg_def: ident  */
#line 376 "parser_bison.ypp"
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
#line 1786 "parser_bison.cpp"
    break;

  case 57: /* int_def: ident  */
#line 404 "parser_bison.ypp"
    {
      realpaver::VariableSymbol* vs = realpaver_bison_symtab->findVariable((yyvsp[0].u_str));

      if (vs != nullptr)
      {
         vs->getVar().setDiscrete();
      }
      else
      {
        std::ostringstream os;
        os << "Variable [" << (yyvsp[0].u_str) << "] not found";
        realpaver_bison_error(os.str().c_str());
        YYABORT;
      }
    }
#line 1806 "parser_bison.cpp"
    break;

  case 61: /* table_list_var_def: ident  */
#line 432 "parser_bison.ypp"
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
#line 1827 "parser_bison.cpp"
    break;

  case 62: /* $@3: %empty  */
#line 452 "parser_bison.ypp"
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
#line 1847 "parser_bison.cpp"
    break;

  case 66: /* table_list_val_def: itv  */
#line 477 "parser_bison.ypp"
    {
       (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 1855 "parser_bison.cpp"
    break;

  case 67: /* table_list_val_def: real  */
#line 481 "parser_bison.ypp"
    {
       (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 1863 "parser_bison.cpp"
    break;

  case 68: /* table_list_val_def: integer  */
#line 485 "parser_bison.ypp"
    {
       (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 1871 "parser_bison.cpp"
    break;

  case 69: /* const_expr: expr  */
#line 492 "parser_bison.ypp"
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
#line 1887 "parser_bison.cpp"
    break;

  case 70: /* expr: expr TK_PLUS expr  */
#line 507 "parser_bison.ypp"
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term))),
                      rhs(realpaver::Term::SharedRep((yyvsp[0].u_term))),
                      res(lhs + rhs);
      (yyval.u_term) = res.cloneRoot();
    }
#line 1898 "parser_bison.cpp"
    break;

  case 71: /* expr: expr TK_MINUS expr  */
#line 514 "parser_bison.ypp"
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term))),
                      rhs(realpaver::Term::SharedRep((yyvsp[0].u_term))),
                      res(lhs - rhs);
      (yyval.u_term) = res.cloneRoot();
    }
#line 1909 "parser_bison.cpp"
    break;

  case 72: /* expr: expr TK_MUL expr  */
#line 521 "parser_bison.ypp"
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term))),
                      rhs(realpaver::Term::SharedRep((yyvsp[0].u_term))),
                      res(lhs * rhs);
      (yyval.u_term) = res.cloneRoot();
    }
#line 1920 "parser_bison.cpp"
    break;

  case 73: /* expr: expr TK_DIV expr  */
#line 528 "parser_bison.ypp"
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term))),
                      rhs(realpaver::Term::SharedRep((yyvsp[0].u_term))),
                      res(lhs / rhs);
      (yyval.u_term) = res.cloneRoot();
    }
#line 1931 "parser_bison.cpp"
    break;

  case 74: /* expr: expr TK_CARET exponent  */
#line 535 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-2].u_term))),
                      d(realpaver::Term::SharedRep((yyvsp[0].u_term))),
                      res(pow(t, d.evalConst()));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1942 "parser_bison.cpp"
    break;

  case 75: /* expr: TK_POW TK_LPAR expr TK_COMMA exponent TK_RPAR  */
#line 542 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-3].u_term))),
                      d(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(pow(t, d.evalConst()));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1953 "parser_bison.cpp"
    break;

  case 76: /* expr: TK_MINUS expr  */
#line 549 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[0].u_term))),
                      res(-t);
      (yyval.u_term) = res.cloneRoot();
    }
#line 1963 "parser_bison.cpp"
    break;

  case 77: /* expr: TK_PLUS expr  */
#line 555 "parser_bison.ypp"
    {
       (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 1971 "parser_bison.cpp"
    break;

  case 78: /* expr: TK_SQRT TK_LPAR expr TK_RPAR  */
#line 559 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(sqrt(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1981 "parser_bison.cpp"
    break;

  case 79: /* expr: TK_SQR TK_LPAR expr TK_RPAR  */
#line 565 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(sqr(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1991 "parser_bison.cpp"
    break;

  case 80: /* expr: TK_SGN TK_LPAR expr TK_RPAR  */
#line 571 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(sgn(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2001 "parser_bison.cpp"
    break;

  case 81: /* expr: TK_ABS TK_LPAR expr TK_RPAR  */
#line 577 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(abs(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2011 "parser_bison.cpp"
    break;

  case 82: /* expr: TK_PIPE expr TK_PIPE  */
#line 583 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(abs(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2021 "parser_bison.cpp"
    break;

  case 83: /* expr: TK_LOG TK_LPAR expr TK_RPAR  */
#line 589 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(log(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2031 "parser_bison.cpp"
    break;

  case 84: /* expr: TK_EXP TK_LPAR expr TK_RPAR  */
#line 595 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(exp(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2041 "parser_bison.cpp"
    break;

  case 85: /* expr: TK_COS TK_LPAR expr TK_RPAR  */
#line 601 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(cos(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2051 "parser_bison.cpp"
    break;

  case 86: /* expr: TK_SIN TK_LPAR expr TK_RPAR  */
#line 607 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(sin(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2061 "parser_bison.cpp"
    break;

  case 87: /* expr: TK_TAN TK_LPAR expr TK_RPAR  */
#line 613 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(tan(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2071 "parser_bison.cpp"
    break;

  case 88: /* expr: TK_MIN TK_LPAR expr TK_COMMA expr TK_RPAR  */
#line 619 "parser_bison.ypp"
    {
      realpaver::Term fst(realpaver::Term::SharedRep((yyvsp[-3].u_term))),
                      snd(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(MIN(fst, snd));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2082 "parser_bison.cpp"
    break;

  case 89: /* expr: TK_MAX TK_LPAR expr TK_COMMA expr TK_RPAR  */
#line 626 "parser_bison.ypp"
    {
      realpaver::Term fst(realpaver::Term::SharedRep((yyvsp[-3].u_term))),
                      snd(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(MAX(fst, snd));
      (yyval.u_term) = res.cloneRoot();
    }
#line 2093 "parser_bison.cpp"
    break;

  case 90: /* expr: TK_LPAR expr TK_RPAR  */
#line 633 "parser_bison.ypp"
  {
    (yyval.u_term) = (yyvsp[-1].u_term);
  }
#line 2101 "parser_bison.cpp"
    break;

  case 91: /* expr: fun_call  */
#line 637 "parser_bison.ypp"
    {
      (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2109 "parser_bison.cpp"
    break;

  case 92: /* expr: ident  */
#line 641 "parser_bison.ypp"
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
#line 2173 "parser_bison.cpp"
    break;

  case 93: /* expr: integer  */
#line 701 "parser_bison.ypp"
    {
      (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2181 "parser_bison.cpp"
    break;

  case 94: /* expr: real  */
#line 705 "parser_bison.ypp"
    {
      (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2189 "parser_bison.cpp"
    break;

  case 95: /* expr: itv  */
#line 709 "parser_bison.ypp"
    {
       (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2197 "parser_bison.cpp"
    break;

  case 96: /* $@4: %empty  */
#line 716 "parser_bison.ypp"
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
#line 2216 "parser_bison.cpp"
    break;

  case 97: /* fun_call: ident TK_LPAR $@4 expr_list TK_RPAR  */
#line 731 "parser_bison.ypp"
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
#line 2235 "parser_bison.cpp"
    break;

  case 98: /* $@5: %empty  */
#line 749 "parser_bison.ypp"
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[0].u_term)));
      realpaver_bison_symtab->addFunctionArgument(t);
    }
#line 2244 "parser_bison.cpp"
    break;

  case 102: /* exponent: integer  */
#line 763 "parser_bison.ypp"
    {
      (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2252 "parser_bison.cpp"
    break;

  case 103: /* exponent: real  */
#line 767 "parser_bison.ypp"
    {
      (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2260 "parser_bison.cpp"
    break;

  case 104: /* integer: TK_INT  */
#line 774 "parser_bison.ypp"
    {
      try
      {
        std::string str(realpaver_bison_text);
        realpaver::Interval x(str);
        (yyval.u_term) = new realpaver::TermConst(x);
      }
      catch(realpaver::Exception e)
      {
        realpaver_bison_error("Bad integer value");
        YYABORT;
      }       
    }
#line 2278 "parser_bison.cpp"
    break;

  case 105: /* real: TK_REAL  */
#line 791 "parser_bison.ypp"
    {
      try
      {
        std::string str(realpaver_bison_text);
        realpaver::Interval x(str);
        (yyval.u_term) = new realpaver::TermConst(x);
      }
      catch(realpaver::Exception e)
      {
        realpaver_bison_error("Bad real number");
        YYABORT;
      }       
    }
#line 2296 "parser_bison.cpp"
    break;

  case 106: /* itv: TK_LSBR const_expr TK_COMMA const_expr TK_RSBR  */
#line 808 "parser_bison.ypp"
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
#line 2317 "parser_bison.cpp"
    break;

  case 107: /* domain: %empty  */
#line 828 "parser_bison.ypp"
    {
      (yyval.u_term) = new realpaver::TermConst(realpaver::Interval::universe());
    }
#line 2325 "parser_bison.cpp"
    break;

  case 108: /* domain: TK_LE const_expr  */
#line 832 "parser_bison.ypp"
    {
      realpaver::Term::SharedRep e((yyvsp[0].u_term));
      realpaver::Interval up = e->evalConst();
      realpaver::Interval domain = realpaver::Interval::lessThan(up.right());

      (yyval.u_term) = new realpaver::TermConst(domain);
    }
#line 2337 "parser_bison.cpp"
    break;

  case 109: /* domain: TK_GE const_expr  */
#line 840 "parser_bison.ypp"
    {
      realpaver::Term::SharedRep e((yyvsp[0].u_term));
      realpaver::Interval lo = e->evalConst();
      realpaver::Interval domain = realpaver::Interval::moreThan(lo.left());

      (yyval.u_term) = new realpaver::TermConst(domain);
    }
#line 2349 "parser_bison.cpp"
    break;

  case 110: /* domain: TK_IN itv  */
#line 848 "parser_bison.ypp"
    {
      realpaver::Term::SharedRep e((yyvsp[0].u_term));
      realpaver::Interval domain = e->evalConst();

      if (domain.isEmpty())
      {
         realpaver_bison_error("Empty domain");
         YYABORT;
      }

      (yyval.u_term) = new realpaver::TermConst(domain);
    }
#line 2366 "parser_bison.cpp"
    break;

  case 111: /* ident: TK_IDENT  */
#line 864 "parser_bison.ypp"
   {
     strcpy((yyval.u_str), realpaver_bison_text);
   }
#line 2374 "parser_bison.cpp"
    break;


#line 2378 "parser_bison.cpp"

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

#line 868 "parser_bison.ypp"


int realpaver_bison_error(const char* str)
{
  std::ostringstream os;   
  os << "l" << realpaver_bison_lineno << ": " << str;

  realpaver_parse_error = os.str();

  return 0;
}
