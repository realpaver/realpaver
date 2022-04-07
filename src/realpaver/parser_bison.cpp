/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

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

/* Copy the first part of user declarations.  */
#line 1 "parser_bison.ypp" /* yacc.c:339  */

#include <cstring>
#include <sstream>
#include <string>
#include "realpaver/Problem.hpp"
#include "realpaver/SymbolTable.hpp"

#undef YYINITDEPTH
#define YYINITDEPTH 5000

extern char* realpaver_bison_text;
extern int realpaver_bison_lineno;
extern std::string realpaver_parse_error;
extern realpaver::Problem* realpaver_bison_problem;
extern realpaver::SymbolTable* realpaver_bison_symtab;

extern int realpaver_bison_lex(void);
int realpaver_bison_error(const char* str);

extern realpaver::FunctionSymbol* realpaver_fun_symbol;

#line 97 "parser_bison.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
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
#line 26 "parser_bison.ypp" /* yacc.c:355  */

#include "realpaver/Constraint.hpp"
#include "realpaver/Exception.hpp"

#line 140 "parser_bison.cpp" /* yacc.c:355  */

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
    TK_CARET = 280,
    TK_SQR = 281,
    TK_SQRT = 282,
    TK_LOG = 283,
    TK_EXP = 284,
    TK_TAN = 285,
    TK_COS = 286,
    TK_SIN = 287,
    TK_LPAR = 288,
    TK_RPAR = 289,
    TK_LSBR = 290,
    TK_RSBR = 291,
    TK_PIPE = 292,
    TK_COMMA = 293,
    TK_SCOLON = 294,
    TK_COLON = 295,
    TK_SGN = 296,
    TK_ABS = 297,
    TK_MIN = 298,
    TK_MAX = 299,
    TK_UMINUS = 300,
    TK_UPLUS = 301
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
#define TK_CARET 280
#define TK_SQR 281
#define TK_SQRT 282
#define TK_LOG 283
#define TK_EXP 284
#define TK_TAN 285
#define TK_COS 286
#define TK_SIN 287
#define TK_LPAR 288
#define TK_RPAR 289
#define TK_LSBR 290
#define TK_RSBR 291
#define TK_PIPE 292
#define TK_COMMA 293
#define TK_SCOLON 294
#define TK_COLON 295
#define TK_SGN 296
#define TK_ABS 297
#define TK_MIN 298
#define TK_MAX 299
#define TK_UMINUS 300
#define TK_UPLUS 301

/* Value type.  */
#if ! defined REALPAVER_BISON_STYPE && ! defined REALPAVER_BISON_STYPE_IS_DECLARED

union REALPAVER_BISON_STYPE
{
#line 32 "parser_bison.ypp" /* yacc.c:355  */

  char u_str[256];
  realpaver::TermRep* u_term;

#line 249 "parser_bison.cpp" /* yacc.c:355  */
};

typedef union REALPAVER_BISON_STYPE REALPAVER_BISON_STYPE;
# define REALPAVER_BISON_STYPE_IS_TRIVIAL 1
# define REALPAVER_BISON_STYPE_IS_DECLARED 1
#endif


extern REALPAVER_BISON_STYPE realpaver_bison_lval;

int realpaver_bison_parse (void);

#endif /* !YY_REALPAVER_BISON_PARSER_BISON_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 266 "parser_bison.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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


#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined REALPAVER_BISON_STYPE_IS_TRIVIAL && REALPAVER_BISON_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  64
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   264

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  96
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  192

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   301

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
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
      45,    46
};

#if REALPAVER_BISON_DEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    67,    67,    69,    73,    74,    75,    76,    77,    78,
      79,    83,    87,    90,    92,    96,   122,   126,   129,   131,
     135,   154,   158,   161,   163,   167,   184,   188,   191,   193,
     197,   205,   213,   224,   228,   231,   233,   237,   248,   262,
     266,   269,   271,   276,   275,   302,   305,   307,   311,   326,
     330,   333,   335,   339,   358,   373,   380,   387,   394,   401,
     408,   415,   421,   425,   431,   437,   443,   449,   455,   461,
     467,   473,   479,   485,   492,   499,   503,   507,   567,   571,
     575,   583,   582,   616,   615,   623,   625,   629,   633,   640,
     656,   672,   693,   696,   704,   712,   728
};
#endif

#if REALPAVER_BISON_DEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TK_IDENT", "TK_INT", "TK_REAL",
  "TK_STR_VAR", "TK_STR_CONST", "TK_STR_CTR", "TK_STR_OBJ", "TK_STR_FUN",
  "TK_STR_ALIAS", "TK_STR_INT", "TK_EQ", "TK_EQ_EQ", "TK_LE", "TK_GE",
  "TK_IN", "TK_LT", "TK_GT", "TK_PLUS", "TK_MINUS", "TK_MUL", "TK_DIV",
  "TK_POW", "TK_CARET", "TK_SQR", "TK_SQRT", "TK_LOG", "TK_EXP", "TK_TAN",
  "TK_COS", "TK_SIN", "TK_LPAR", "TK_RPAR", "TK_LSBR", "TK_RSBR",
  "TK_PIPE", "TK_COMMA", "TK_SCOLON", "TK_COLON", "TK_SGN", "TK_ABS",
  "TK_MIN", "TK_MAX", "TK_UMINUS", "TK_UPLUS", "$accept", "model",
  "statement", "const_statement", "const_list", "const_tail", "const_def",
  "var_statement", "var_list", "var_tail", "var_def", "alias_statement",
  "alias_list", "alias_tail", "alias_def", "ctr_statement", "ctr_list",
  "ctr_tail", "ctr_def", "obj_statement", "obj_list", "obj_tail",
  "obj_def", "fun_statement", "fun_list", "fun_tail", "fun_def", "$@1",
  "arg_list", "arg_tail", "arg_def", "int_statement", "int_list",
  "int_tail", "int_def", "const_expr", "expr", "fun_call", "$@2",
  "expr_list", "$@3", "expr_tail", "exponent", "integer", "real", "itv",
  "domain", "ident", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301
};
# endif

#define YYPACT_NINF -95

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-95)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     252,    17,    17,    10,   -32,    17,    17,    17,    22,   -12,
     -95,   -95,   -95,   -95,   -95,   -95,   -95,   -95,   -95,    -9,
      53,   -95,    -6,    31,   -95,   -95,    10,    10,    13,    26,
      28,    30,    32,    38,    51,    52,    10,    10,    10,    58,
      70,    72,    73,   -95,    12,   232,   -95,   -95,   -95,   -95,
      74,    10,    10,   -95,    71,   -95,    75,    82,   -95,    84,
      99,   -95,    90,   -95,   -95,   252,    17,   -95,    10,    10,
      81,   -95,    17,   -95,    10,   109,   109,    10,    10,    10,
      10,    10,    10,    10,    10,   110,   105,    -4,   104,    10,
      10,    10,    10,    10,   -95,    10,    10,    10,    10,    10,
      10,    10,    78,   -95,    -4,    -4,   -32,   -95,    17,   -95,
     -95,    17,   -95,    10,    17,   -95,   -95,   -95,   -95,   -95,
     -95,   -95,   -95,    35,   117,   133,   148,   154,   169,   175,
     185,   -95,    10,   -95,   191,   201,    79,    98,   -95,    -4,
      -4,    -4,     3,     3,   109,   109,   -95,   -95,   -95,    10,
     -95,   -95,    17,   -95,    -4,   -95,    78,   -95,   -95,   -95,
     -95,   -95,   -95,   -95,   111,   -95,   -95,    10,    10,    -4,
     114,   115,   107,   -95,   116,   -95,   207,   217,   119,   -95,
     146,    17,   -95,   -95,   -95,   -95,    10,   -95,    10,   -95,
     -95,    -4
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     9,    10,    96,    16,    18,
      92,    11,    13,     0,    89,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    26,    28,     0,    76,    78,    79,    80,
      77,     0,     0,    33,    35,    39,    41,     0,    21,    23,
       0,    49,    51,    53,     1,     2,     0,    17,     0,     0,
       0,    20,     0,    12,     0,    62,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,    27,     0,     0,     0,     0,     0,
       0,     0,     0,    81,    37,    38,     0,    34,     0,    40,
      43,     0,    22,     0,     0,    50,     3,    19,    93,    94,
      95,    14,    15,     0,     0,     0,     0,     0,     0,     0,
       0,    75,     0,    67,     0,     0,     0,     0,    29,    30,
      31,    32,    55,    56,    57,    58,    59,    87,    88,     0,
      36,    42,     0,    24,    25,    52,     0,    64,    63,    68,
      69,    72,    70,    71,     0,    65,    66,     0,     0,    83,
       0,     0,    46,    48,     0,    91,     0,     0,    85,    82,
       0,     0,    45,    60,    73,    74,     0,    84,     0,    47,
      86,    44
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -95,    95,   -95,   -95,    89,   -95,   -95,   -95,    96,   -95,
     -95,   -95,    55,   -95,   -95,   -95,    85,   -95,   -95,   -95,
      57,   -95,   -95,   -95,    64,   -95,   -95,   -95,    -1,   -95,
     -95,   -95,    87,   -95,   -95,   -65,    -3,   -95,   -95,    -2,
     -95,   -95,    37,   -94,   -92,   129,   -95,     0
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     8,     9,    10,    21,    73,    22,    11,    18,    67,
      19,    12,    58,   112,    59,    13,    43,    94,    44,    14,
      53,   107,    54,    15,    55,   109,    56,   152,   171,   182,
     172,    16,    61,   115,    62,    86,    87,    46,   149,   170,
     178,   187,   146,    47,    48,    49,    71,    50
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      45,    20,    23,   118,   119,    57,    60,    63,   147,   122,
     148,    51,    52,    17,    24,    25,    98,    99,   100,   101,
      17,   102,    64,    75,    76,   100,   101,    65,   102,    66,
      26,    27,    72,    85,    28,    88,    29,    30,    31,    32,
      33,    34,    35,    36,    74,    37,    77,    38,   104,   105,
      93,    39,    40,    41,    42,    98,    99,   100,   101,    78,
     102,    79,   147,    80,   148,    81,    20,   164,    68,    69,
      70,    82,    23,   156,   123,   124,   125,   126,   127,   128,
     129,   130,    24,    25,    83,    84,   134,   135,   136,   137,
      45,    89,   139,   140,   141,   142,   143,   144,   145,    98,
      99,   100,   101,    90,   102,    91,    92,   103,    57,   106,
     154,    60,   113,   108,    63,   110,    37,   167,    98,    99,
     100,   101,   111,   102,    98,    99,   100,   101,   114,   102,
      98,    99,   100,   101,   102,   102,   168,    98,    99,   100,
     101,   133,   102,   132,   131,   181,   169,   175,   179,   180,
     183,   157,   173,    98,    99,   100,   101,   186,   102,   188,
     116,   121,   117,   150,   176,   177,   153,   158,    98,    99,
     100,   101,   151,   102,    98,    99,   100,   101,   138,   102,
     189,   173,   159,   169,   190,   191,     0,     0,   160,    98,
      99,   100,   101,   174,   102,    98,    99,   100,   101,   120,
     102,   155,     0,   161,     0,    98,    99,   100,   101,   162,
     102,    98,    99,   100,   101,     0,   102,     0,     0,   163,
       0,    98,    99,   100,   101,   165,   102,    98,    99,   100,
     101,     0,   102,     0,     0,   166,     0,    98,    99,   100,
     101,   184,   102,     0,     0,     0,    95,    96,    97,     0,
       0,   185,    98,    99,   100,   101,     0,   102,     1,     2,
       3,     4,     5,     6,     7
};

static const yytype_int16 yycheck[] =
{
       3,     1,     2,    68,    69,     5,     6,     7,   102,    74,
     102,    43,    44,     3,     4,     5,    20,    21,    22,    23,
       3,    25,     0,    26,    27,    22,    23,    39,    25,    38,
      20,    21,    38,    36,    24,    38,    26,    27,    28,    29,
      30,    31,    32,    33,    13,    35,    33,    37,    51,    52,
      38,    41,    42,    43,    44,    20,    21,    22,    23,    33,
      25,    33,   156,    33,   156,    33,    66,   132,    15,    16,
      17,    33,    72,    38,    77,    78,    79,    80,    81,    82,
      83,    84,     4,     5,    33,    33,    89,    90,    91,    92,
      93,    33,    95,    96,    97,    98,    99,   100,   101,    20,
      21,    22,    23,    33,    25,    33,    33,    33,   108,    38,
     113,   111,    13,    38,   114,    33,    35,    38,    20,    21,
      22,    23,    38,    25,    20,    21,    22,    23,    38,    25,
      20,    21,    22,    23,    25,    25,    38,    20,    21,    22,
      23,    37,    25,    38,    34,    38,   149,    36,    34,    34,
      34,    34,   152,    20,    21,    22,    23,    38,    25,    13,
      65,    72,    66,   106,   167,   168,   111,    34,    20,    21,
      22,    23,   108,    25,    20,    21,    22,    23,    93,    25,
     181,   181,    34,   186,   186,   188,    -1,    -1,    34,    20,
      21,    22,    23,   156,    25,    20,    21,    22,    23,    70,
      25,   114,    -1,    34,    -1,    20,    21,    22,    23,    34,
      25,    20,    21,    22,    23,    -1,    25,    -1,    -1,    34,
      -1,    20,    21,    22,    23,    34,    25,    20,    21,    22,
      23,    -1,    25,    -1,    -1,    34,    -1,    20,    21,    22,
      23,    34,    25,    -1,    -1,    -1,    14,    15,    16,    -1,
      -1,    34,    20,    21,    22,    23,    -1,    25,     6,     7,
       8,     9,    10,    11,    12
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     6,     7,     8,     9,    10,    11,    12,    48,    49,
      50,    54,    58,    62,    66,    70,    78,     3,    55,    57,
      94,    51,    53,    94,     4,     5,    20,    21,    24,    26,
      27,    28,    29,    30,    31,    32,    33,    35,    37,    41,
      42,    43,    44,    63,    65,    83,    84,    90,    91,    92,
      94,    43,    44,    67,    69,    71,    73,    94,    59,    61,
      94,    79,    81,    94,     0,    39,    38,    56,    15,    16,
      17,    93,    38,    52,    13,    83,    83,    33,    33,    33,
      33,    33,    33,    33,    33,    83,    82,    83,    83,    33,
      33,    33,    33,    38,    64,    14,    15,    16,    20,    21,
      22,    23,    25,    33,    83,    83,    38,    68,    38,    72,
      33,    38,    60,    13,    38,    80,    48,    55,    82,    82,
      92,    51,    82,    83,    83,    83,    83,    83,    83,    83,
      83,    34,    38,    37,    83,    83,    83,    83,    63,    83,
      83,    83,    83,    83,    83,    83,    89,    90,    91,    85,
      67,    71,    74,    59,    83,    79,    38,    34,    34,    34,
      34,    34,    34,    34,    82,    34,    34,    38,    38,    83,
      86,    75,    77,    94,    89,    36,    83,    83,    87,    34,
      34,    38,    76,    34,    34,    34,    38,    88,    13,    75,
      86,    83
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    47,    48,    48,    49,    49,    49,    49,    49,    49,
      49,    50,    51,    52,    52,    53,    54,    55,    56,    56,
      57,    58,    59,    60,    60,    61,    62,    63,    64,    64,
      65,    65,    65,    66,    67,    68,    68,    69,    69,    70,
      71,    72,    72,    74,    73,    75,    76,    76,    77,    78,
      79,    80,    80,    81,    82,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    85,    84,    87,    86,    88,    88,    89,    89,    90,
      91,    92,    93,    93,    93,    93,    94
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     0,     2,     3,     2,     2,     0,     2,
       2,     2,     2,     0,     2,     3,     2,     2,     0,     2,
       3,     3,     3,     2,     2,     0,     2,     2,     2,     2,
       2,     0,     2,     0,     7,     2,     0,     2,     1,     2,
       2,     0,     2,     1,     1,     3,     3,     3,     3,     3,
       6,     2,     2,     4,     4,     4,     4,     3,     4,     4,
       4,     4,     4,     6,     6,     3,     1,     1,     1,     1,
       1,     0,     5,     0,     3,     0,     2,     1,     1,     1,
       1,     5,     0,     2,     2,     2,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
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
        case 15:
#line 97 "parser_bison.ypp" /* yacc.c:1646  */
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
#line 1520 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 136 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term::SharedRep e((yyvsp[0].u_term));
      realpaver::Interval x = e->evalConst();

      if (realpaver_bison_symtab->hasSymbol((yyvsp[-1].u_str)))
      {
        std::ostringstream os;
        os << "Symbol [" << (yyvsp[-1].u_str) << "] already used";
        realpaver_bison_error(os.str().c_str());
        YYABORT;         
      }

      realpaver::Variable v = realpaver_bison_problem->addRealVar(x, (yyvsp[-1].u_str));
      realpaver_bison_symtab->insertVariable((yyvsp[-1].u_str), v);
    }
#line 1540 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 168 "parser_bison.ypp" /* yacc.c:1646  */
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
#line 1558 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 198 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term)));
      realpaver::Term rhs(realpaver::Term::SharedRep((yyvsp[0].u_term)));

      realpaver::Constraint c(lhs == rhs);
      realpaver_bison_problem->addCtr(c);
    }
#line 1570 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 206 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term)));
      realpaver::Term rhs(realpaver::Term::SharedRep((yyvsp[0].u_term)));

      realpaver::Constraint c(lhs <= rhs);
      realpaver_bison_problem->addCtr(c);
    }
#line 1582 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 214 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term)));
      realpaver::Term rhs(realpaver::Term::SharedRep((yyvsp[0].u_term)));

      realpaver::Constraint c(lhs >= rhs);
      realpaver_bison_problem->addCtr(c);
    }
#line 1594 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 238 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term f(realpaver::Term::SharedRep((yyvsp[0].u_term)));
      if (f.isConstant())
      {
        realpaver_bison_error("Constant objective function");
        YYABORT;
      }

      realpaver_bison_problem->addObjective(MIN(f));
    }
#line 1609 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 249 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term f(realpaver::Term::SharedRep((yyvsp[0].u_term)));
      if (f.isConstant())
      {
        realpaver_bison_error("Constant objective function");
        YYABORT;
      }

      realpaver_bison_problem->addObjective(MAX(f));       
    }
#line 1624 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 276 "parser_bison.ypp" /* yacc.c:1646  */
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
#line 1640 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 288 "parser_bison.ypp" /* yacc.c:1646  */
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
#line 1656 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 312 "parser_bison.ypp" /* yacc.c:1646  */
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
#line 1672 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 340 "parser_bison.ypp" /* yacc.c:1646  */
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
#line 1692 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 359 "parser_bison.ypp" /* yacc.c:1646  */
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
#line 1708 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 374 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term))),
                      rhs(realpaver::Term::SharedRep((yyvsp[0].u_term))),
                      res(lhs + rhs);
      (yyval.u_term) = res.cloneRoot();
    }
#line 1719 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 381 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term))),
                      rhs(realpaver::Term::SharedRep((yyvsp[0].u_term))),
                      res(lhs - rhs);
      (yyval.u_term) = res.cloneRoot();
    }
#line 1730 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 388 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term))),
                      rhs(realpaver::Term::SharedRep((yyvsp[0].u_term))),
                      res(lhs * rhs);
      (yyval.u_term) = res.cloneRoot();
    }
#line 1741 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 395 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term lhs(realpaver::Term::SharedRep((yyvsp[-2].u_term))),
                      rhs(realpaver::Term::SharedRep((yyvsp[0].u_term))),
                      res(lhs / rhs);
      (yyval.u_term) = res.cloneRoot();
    }
#line 1752 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 402 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-2].u_term))),
                      d(realpaver::Term::SharedRep((yyvsp[0].u_term))),
                      res(pow(t, d.evalConst()));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1763 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 409 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-3].u_term))),
                      d(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(pow(t, d.evalConst()));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1774 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 416 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[0].u_term))),
                      res(-t);
      (yyval.u_term) = res.cloneRoot();
    }
#line 1784 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 422 "parser_bison.ypp" /* yacc.c:1646  */
    {
       (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 1792 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 426 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(sqrt(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1802 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 432 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(sqr(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1812 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 438 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(sgn(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1822 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 444 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(abs(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1832 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 450 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(abs(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1842 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 456 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(log(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1852 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 462 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(exp(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1862 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 468 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(cos(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1872 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 474 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(sin(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1882 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 480 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(tan(t));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1892 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 486 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term fst(realpaver::Term::SharedRep((yyvsp[-3].u_term))),
                      snd(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(MIN(fst, snd));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1903 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 493 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term fst(realpaver::Term::SharedRep((yyvsp[-3].u_term))),
                      snd(realpaver::Term::SharedRep((yyvsp[-1].u_term))),
                      res(MAX(fst, snd));
      (yyval.u_term) = res.cloneRoot();
    }
#line 1914 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 500 "parser_bison.ypp" /* yacc.c:1646  */
    {
    (yyval.u_term) = (yyvsp[-1].u_term);
  }
#line 1922 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 504 "parser_bison.ypp" /* yacc.c:1646  */
    {
      (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 1930 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 508 "parser_bison.ypp" /* yacc.c:1646  */
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
#line 1994 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 568 "parser_bison.ypp" /* yacc.c:1646  */
    {
      (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2002 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 572 "parser_bison.ypp" /* yacc.c:1646  */
    {
      (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2010 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 576 "parser_bison.ypp" /* yacc.c:1646  */
    {
       (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2018 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 583 "parser_bison.ypp" /* yacc.c:1646  */
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
#line 2037 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 598 "parser_bison.ypp" /* yacc.c:1646  */
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
#line 2056 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 616 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term t(realpaver::Term::SharedRep((yyvsp[0].u_term)));
      realpaver_bison_symtab->addFunctionArgument(t);
    }
#line 2065 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 630 "parser_bison.ypp" /* yacc.c:1646  */
    {
      (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2073 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 634 "parser_bison.ypp" /* yacc.c:1646  */
    {
      (yyval.u_term) = (yyvsp[0].u_term);
    }
#line 2081 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 641 "parser_bison.ypp" /* yacc.c:1646  */
    {
      try
      {
        realpaver::Interval x(realpaver_bison_text);
        (yyval.u_term) = new realpaver::TermConst(x);
      }
      catch(realpaver::Exception e)
      {
        realpaver_bison_error("Bad integer value");
        YYABORT;
      }       
    }
#line 2098 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 657 "parser_bison.ypp" /* yacc.c:1646  */
    {
      try
      {
        realpaver::Interval x(realpaver_bison_text);
        (yyval.u_term) = new realpaver::TermConst(x);
      }
      catch(realpaver::Exception e)
      {
        realpaver_bison_error("Bad real number");
        YYABORT;
      }       
    }
#line 2115 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 673 "parser_bison.ypp" /* yacc.c:1646  */
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
#line 2136 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 693 "parser_bison.ypp" /* yacc.c:1646  */
    {
      (yyval.u_term) = new realpaver::TermConst(realpaver::Interval::universe());
    }
#line 2144 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 697 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term::SharedRep e((yyvsp[0].u_term));
      realpaver::Interval up = e->evalConst();
      realpaver::Interval domain = realpaver::Interval::lessThan(up.right());

      (yyval.u_term) = new realpaver::TermConst(domain);
    }
#line 2156 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 705 "parser_bison.ypp" /* yacc.c:1646  */
    {
      realpaver::Term::SharedRep e((yyvsp[0].u_term));
      realpaver::Interval lo = e->evalConst();
      realpaver::Interval domain = realpaver::Interval::moreThan(lo.left());

      (yyval.u_term) = new realpaver::TermConst(domain);
    }
#line 2168 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 713 "parser_bison.ypp" /* yacc.c:1646  */
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
#line 2185 "parser_bison.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 729 "parser_bison.ypp" /* yacc.c:1646  */
    {
     strcpy((yyval.u_str), realpaver_bison_text);
   }
#line 2193 "parser_bison.cpp" /* yacc.c:1646  */
    break;


#line 2197 "parser_bison.cpp" /* yacc.c:1646  */
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
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
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 733 "parser_bison.ypp" /* yacc.c:1906  */


int realpaver_bison_error(const char* str)
{
  std::ostringstream os;   
  os << "l" << realpaver_bison_lineno << ": " << str;

  realpaver_parse_error = os.str();

  return 0;
}
