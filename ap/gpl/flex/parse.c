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




/* Copy the first part of user declarations.  */
#line 9 "./parse.y" /* yacc.c:339  */

/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Vern Paxson.
 * 
 * The United States Government has rights in this work pursuant
 * to contract no. DE-AC03-76SF00098 between the United States
 * Department of Energy and the University of California.
 *
 * Redistribution and use in source and binary forms with or without
 * modification are permitted provided that: (1) source distributions retain
 * this entire copyright notice and comment, and (2) distributions including
 * binaries display the following acknowledgement:  ``This product includes
 * software developed by the University of California, Berkeley and its
 * contributors'' in the documentation or other materials provided with the
 * distribution and in all advertising materials mentioning features or use
 * of this software.  Neither the name of the University nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* $Header: /ramdisk/repositories/20_cvs_clean_up/2011-02-11_sj/src/router/flex/parse.y,v 1.1.1.1 2001-04-08 23:53:37 mhuang Exp $ */


/* Some versions of bison are broken in that they use alloca() but don't
 * declare it properly.  The following is the patented (just kidding!)
 * #ifdef chud to fix the problem, courtesy of Francois Pinard.
 */
#ifdef YYBISON
/* AIX requires this to be the first thing in the file.  What a piece.  */
# ifdef _AIX
 #pragma alloca
# endif
#endif

#include "flexdef.h"

/* The remainder of the alloca() cruft has to come after including flexdef.h,
 * so HAVE_ALLOCA_H is (possibly) defined.
 */
#ifdef YYBISON
# ifdef __GNUC__
#  ifndef alloca
#   define alloca __builtin_alloca
#  endif
# else
#  if HAVE_ALLOCA_H
#   include <alloca.h>
#  else
#   ifdef __hpux
void *alloca ();
#   else
#    ifdef __TURBOC__
#     include <malloc.h>
#    else
char *alloca ();
#    endif
#   endif
#  endif
# endif
#endif

/* Bletch, ^^^^ that was ugly! */


int pat, scnum, eps, headcnt, trailcnt, anyccl, lastchar, i, rulelen;
int trlcontxt, xcluflg, currccl, cclsorted, varlength, variable_trail_rule;

int *scon_stk;
int scon_stk_ptr;

static int madeany = false;  /* whether we've made the '.' character class */
int previous_continued_action;	/* whether the previous rule's action was '|' */

/* Expand a POSIX character class expression. */
#define CCL_EXPR(func) \
	{ \
	int c; \
	for ( c = 0; c < csize; ++c ) \
		if ( isascii(c) && func(c) ) \
			ccladd( currccl, c ); \
	}

/* While POSIX defines isblank(), it's not ANSI C. */
#define IS_BLANK(c) ((c) == ' ' || (c) == '\t')

/* On some over-ambitious machines, such as DEC Alpha's, the default
 * token type is "long" instead of "int"; this leads to problems with
 * declaring yylval in flexdef.h.  But so far, all the yacc's I've seen
 * wrap their definitions of YYSTYPE with "#ifndef YYSTYPE"'s, so the
 * following should ensure that the default token type is "int".
 */
#define YYSTYPE int


#line 168 "y.tab.c" /* yacc.c:339  */

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
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
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
    NUMBER = 259,
    SECTEND = 260,
    SCDECL = 261,
    XSCDECL = 262,
    NAME = 263,
    PREVCCL = 264,
    EOF_OP = 265,
    OPTION_OP = 266,
    OPT_OUTFILE = 267,
    OPT_PREFIX = 268,
    OPT_YYCLASS = 269,
    CCE_ALNUM = 270,
    CCE_ALPHA = 271,
    CCE_BLANK = 272,
    CCE_CNTRL = 273,
    CCE_DIGIT = 274,
    CCE_GRAPH = 275,
    CCE_LOWER = 276,
    CCE_PRINT = 277,
    CCE_PUNCT = 278,
    CCE_SPACE = 279,
    CCE_UPPER = 280,
    CCE_XDIGIT = 281
  };
#endif
/* Tokens.  */
#define CHAR 258
#define NUMBER 259
#define SECTEND 260
#define SCDECL 261
#define XSCDECL 262
#define NAME 263
#define PREVCCL 264
#define EOF_OP 265
#define OPTION_OP 266
#define OPT_OUTFILE 267
#define OPT_PREFIX 268
#define OPT_YYCLASS 269
#define CCE_ALNUM 270
#define CCE_ALPHA 271
#define CCE_BLANK 272
#define CCE_CNTRL 273
#define CCE_DIGIT 274
#define CCE_GRAPH 275
#define CCE_LOWER 276
#define CCE_PRINT 277
#define CCE_PUNCT 278
#define CCE_SPACE 279
#define CCE_UPPER 280
#define CCE_XDIGIT 281

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 271 "y.tab.c" /* yacc.c:358  */

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
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   110

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  26
/* YYNRULES -- Number of rules.  */
#define YYNRULES  76
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  107

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   281

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      28,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    42,     2,    36,     2,     2,     2,
      43,    44,    34,    39,    35,    47,    41,    38,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      32,    27,    33,    40,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    45,     2,    46,    31,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    29,    37,    30,     2,     2,     2,     2,
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
      25,    26
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   112,   112,   142,   149,   150,   151,   152,   156,   164,
     167,   171,   174,   177,   181,   184,   185,   188,   193,   195,
     199,   201,   203,   207,   219,   255,   279,   302,   307,   310,
     313,   331,   334,   336,   338,   342,   365,   420,   423,   466,
     484,   490,   495,   522,   530,   534,   541,   547,   553,   581,
     595,   614,   636,   654,   661,   664,   667,   678,   681,   688,
     716,   727,   735,   742,   743,   744,   745,   746,   747,   748,
     749,   750,   751,   752,   758,   761,   772
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CHAR", "NUMBER", "SECTEND", "SCDECL",
  "XSCDECL", "NAME", "PREVCCL", "EOF_OP", "OPTION_OP", "OPT_OUTFILE",
  "OPT_PREFIX", "OPT_YYCLASS", "CCE_ALNUM", "CCE_ALPHA", "CCE_BLANK",
  "CCE_CNTRL", "CCE_DIGIT", "CCE_GRAPH", "CCE_LOWER", "CCE_PRINT",
  "CCE_PUNCT", "CCE_SPACE", "CCE_UPPER", "CCE_XDIGIT", "'='", "'\\n'",
  "'{'", "'}'", "'^'", "'<'", "'>'", "'*'", "','", "'$'", "'|'", "'/'",
  "'+'", "'?'", "'.'", "'\"'", "'('", "')'", "'['", "']'", "'-'",
  "$accept", "goal", "initlex", "sect1", "sect1end", "startconddecl",
  "namelist1", "options", "optionlist", "option", "sect2", "initforrule",
  "flexrule", "scon_stk_ptr", "scon", "namelist2", "sconname", "rule",
  "re", "re2", "series", "singleton", "fullccl", "ccl", "ccl_expr",
  "string", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,    61,    10,   123,
     125,    94,    60,    62,    42,    44,    36,   124,    47,    43,
      63,    46,    34,    40,    41,    91,    93,    45
};
# endif

#define YYPACT_NINF -52

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-52)))

#define YYTABLE_NINF -24

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -52,     1,    78,   -52,   -52,    81,   -52,   -52,   -52,   -52,
     -52,     6,   -52,    -2,     5,   -52,   -52,    20,   -14,     9,
      16,   -52,    39,   -52,    31,   -52,    51,    74,    82,    61,
      30,   -52,    -1,   -52,   -52,   -52,   -52,   -52,   -52,    58,
     -52,    44,   -52,   -52,   -52,   -52,    24,   -52,   -52,    24,
      66,    70,   -52,    25,    24,    24,    41,   -52,   -52,    91,
     -52,   -52,    26,    27,   -52,     0,   -52,   -52,    24,   -52,
      59,    41,    96,   -52,   -52,   -52,   -52,   -52,   -52,   -52,
      32,    54,   -52,   -52,   -52,   -52,   -52,   -52,   -52,   -52,
     -52,   -52,   -52,   -52,   -52,   -52,    24,   -52,     4,   -52,
      99,   -52,     2,   -52,    73,   -52,   -52
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     1,     7,     0,     8,     9,    10,    16,
      22,     0,     5,    14,    31,    13,    12,     4,     0,     0,
       0,    15,    28,     2,    23,    11,     0,     0,     0,     0,
       0,    22,     0,    17,    18,    19,    30,    34,    35,     0,
      33,    31,    27,    56,    53,    26,     0,    51,    76,     0,
      62,     0,    25,    39,     0,    41,    44,    52,    29,     0,
      21,    24,     0,     0,    62,     0,    20,    38,     0,    42,
      36,    43,     0,    45,    46,    47,    32,    75,    54,    55,
       0,    60,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    57,    61,    40,    37,     0,    58,
       0,    50,     0,    59,     0,    49,    48
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -52,   -52,   -52,   -52,   -52,   -52,   -52,   -52,   -52,   -52,
      75,    80,   -52,   -52,   -52,   -52,    46,    62,    23,   -52,
      42,   -51,   -52,    43,   -52,   -52
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,     5,    10,    11,    17,    12,    13,    21,
      14,    23,    51,    30,    24,    39,    40,    52,    53,    54,
      55,    56,    57,    65,    95,    62
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      42,     3,    43,    81,    71,   -23,   104,    15,    44,    45,
      18,    19,    20,    26,    16,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    43,    25,    77,
      46,    37,   105,    44,   101,    81,    27,    22,    38,   102,
      47,    48,    49,    28,    50,    71,    94,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    33,
      31,    67,    68,    69,    68,    47,    48,    49,    78,    50,
      72,    79,    63,    29,    60,    73,    22,    70,    99,     4,
      74,    75,    34,    -6,    -6,    -6,     6,     7,     8,    -6,
      35,    58,     9,    59,    36,    97,    68,    64,    66,    38,
      98,   100,   103,   106,    32,    76,    41,    80,    61,     0,
      96
};

static const yytype_int8 yycheck[] =
{
       1,     0,     3,     3,    55,     0,     4,     1,     9,    10,
      12,    13,    14,    27,     8,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     3,     8,     3,
      31,     1,    30,     9,    30,     3,    27,    32,     8,    35,
      41,    42,    43,    27,    45,    96,    46,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     8,
      29,    36,    37,    38,    37,    41,    42,    43,    42,    45,
      29,    44,    49,    34,    30,    34,    32,    54,    46,     1,
      39,    40,     8,     5,     6,     7,     5,     6,     7,    11,
       8,    33,    11,    35,    33,    36,    37,    31,    28,     8,
       4,    47,     3,    30,    24,    59,    31,    64,    46,    -1,
      68
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    49,    50,     0,     1,    51,     5,     6,     7,    11,
      52,    53,    55,    56,    58,     1,     8,    54,    12,    13,
      14,    57,    32,    59,    62,     8,    27,    27,    27,    34,
      61,    29,    59,     8,     8,     8,    33,     1,     8,    63,
      64,    58,     1,     3,     9,    10,    31,    41,    42,    43,
      45,    60,    65,    66,    67,    68,    69,    70,    33,    35,
      30,    65,    73,    66,    31,    71,    28,    36,    37,    38,
      66,    69,    29,    34,    39,    40,    64,     3,    42,    44,
      71,     3,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    46,    72,    68,    36,     4,    46,
      47,    30,    35,     3,     4,    30,    30
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    48,    49,    50,    51,    51,    51,    51,    52,    53,
      53,    54,    54,    54,    55,    56,    56,    57,    57,    57,
      58,    58,    58,    59,    60,    60,    60,    60,    61,    62,
      62,    62,    63,    63,    63,    64,    65,    65,    65,    65,
      66,    66,    67,    68,    68,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    70,    70,    71,
      71,    71,    71,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    73,    73
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     5,     0,     3,     2,     0,     1,     1,     1,
       1,     2,     1,     1,     2,     2,     0,     3,     3,     3,
       5,     5,     0,     0,     2,     1,     1,     1,     0,     4,
       3,     0,     3,     1,     1,     1,     2,     3,     2,     1,
       3,     1,     2,     2,     1,     2,     2,     2,     6,     5,
       4,     1,     1,     1,     3,     3,     1,     3,     4,     4,
       2,     2,     0,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     0
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
#if YYDEBUG

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
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


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
        case 2:
#line 113 "./parse.y" /* yacc.c:1646  */
    { /* add default rule */
			int def_rule;

			pat = cclinit();
			cclnegate( pat );

			def_rule = mkstate( -pat );

			/* Remember the number of the default rule so we
			 * don't generate "can't match" warnings for it.
			 */
			default_rule = num_rules;

			finish_rule( def_rule, false, 0, 0 );

			for ( i = 1; i <= lastsc; ++i )
				scset[i] = mkbranch( scset[i], def_rule );

			if ( spprdflt )
				add_action(
				"YY_FATAL_ERROR( \"flex scanner jammed\" )" );
			else
				add_action( "ECHO" );

			add_action( ";\n\tYY_BREAK\n" );
			}
#line 1455 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 142 "./parse.y" /* yacc.c:1646  */
    { /* initialize for processing rules */

			/* Create default DFA start condition. */
			scinstal( "INITIAL", false );
			}
#line 1465 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 153 "./parse.y" /* yacc.c:1646  */
    { synerr( "unknown error processing section 1" ); }
#line 1471 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 157 "./parse.y" /* yacc.c:1646  */
    {
			check_options();
			scon_stk = allocate_integer_array( lastsc + 1 );
			scon_stk_ptr = 0;
			}
#line 1481 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 165 "./parse.y" /* yacc.c:1646  */
    { xcluflg = false; }
#line 1487 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 168 "./parse.y" /* yacc.c:1646  */
    { xcluflg = true; }
#line 1493 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 172 "./parse.y" /* yacc.c:1646  */
    { scinstal( nmstr, xcluflg ); }
#line 1499 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 175 "./parse.y" /* yacc.c:1646  */
    { scinstal( nmstr, xcluflg ); }
#line 1505 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 178 "./parse.y" /* yacc.c:1646  */
    { synerr( "bad start condition list" ); }
#line 1511 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 189 "./parse.y" /* yacc.c:1646  */
    {
			outfilename = copy_string( nmstr );
			did_outfilename = 1;
			}
#line 1520 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 194 "./parse.y" /* yacc.c:1646  */
    { prefix = copy_string( nmstr ); }
#line 1526 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 196 "./parse.y" /* yacc.c:1646  */
    { yyclass = copy_string( nmstr ); }
#line 1532 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 200 "./parse.y" /* yacc.c:1646  */
    { scon_stk_ptr = (yyvsp[-3]); }
#line 1538 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 202 "./parse.y" /* yacc.c:1646  */
    { scon_stk_ptr = (yyvsp[-3]); }
#line 1544 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 207 "./parse.y" /* yacc.c:1646  */
    {
			/* Initialize for a parse of one rule. */
			trlcontxt = variable_trail_rule = varlength = false;
			trailcnt = headcnt = rulelen = 0;
			current_state_type = STATE_NORMAL;
			previous_continued_action = continued_action;
			in_rule = true;

			new_rule();
			}
#line 1559 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 220 "./parse.y" /* yacc.c:1646  */
    {
			pat = (yyvsp[0]);
			finish_rule( pat, variable_trail_rule,
				headcnt, trailcnt );

			if ( scon_stk_ptr > 0 )
				{
				for ( i = 1; i <= scon_stk_ptr; ++i )
					scbol[scon_stk[i]] =
						mkbranch( scbol[scon_stk[i]],
								pat );
				}

			else
				{
				/* Add to all non-exclusive start conditions,
				 * including the default (0) start condition.
				 */

				for ( i = 1; i <= lastsc; ++i )
					if ( ! scxclu[i] )
						scbol[i] = mkbranch( scbol[i],
									pat );
				}

			if ( ! bol_needed )
				{
				bol_needed = true;

				if ( performance_report > 1 )
					pinpoint_message(
			"'^' operator results in sub-optimal performance" );
				}
			}
#line 1598 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 256 "./parse.y" /* yacc.c:1646  */
    {
			pat = (yyvsp[0]);
			finish_rule( pat, variable_trail_rule,
				headcnt, trailcnt );

			if ( scon_stk_ptr > 0 )
				{
				for ( i = 1; i <= scon_stk_ptr; ++i )
					scset[scon_stk[i]] =
						mkbranch( scset[scon_stk[i]],
								pat );
				}

			else
				{
				for ( i = 1; i <= lastsc; ++i )
					if ( ! scxclu[i] )
						scset[i] =
							mkbranch( scset[i],
								pat );
				}
			}
#line 1625 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 280 "./parse.y" /* yacc.c:1646  */
    {
			if ( scon_stk_ptr > 0 )
				build_eof_action();
	
			else
				{
				/* This EOF applies to all start conditions
				 * which don't already have EOF actions.
				 */
				for ( i = 1; i <= lastsc; ++i )
					if ( ! sceof[i] )
						scon_stk[++scon_stk_ptr] = i;

				if ( scon_stk_ptr == 0 )
					warn(
			"all start conditions already have <<EOF>> rules" );

				else
					build_eof_action();
				}
			}
#line 1651 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 303 "./parse.y" /* yacc.c:1646  */
    { synerr( "unrecognized rule" ); }
#line 1657 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 307 "./parse.y" /* yacc.c:1646  */
    { (yyval) = scon_stk_ptr; }
#line 1663 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 311 "./parse.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-2]); }
#line 1669 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 314 "./parse.y" /* yacc.c:1646  */
    {
			(yyval) = scon_stk_ptr;

			for ( i = 1; i <= lastsc; ++i )
				{
				int j;

				for ( j = 1; j <= scon_stk_ptr; ++j )
					if ( scon_stk[j] == i )
						break;

				if ( j > scon_stk_ptr )
					scon_stk[++scon_stk_ptr] = i;
				}
			}
#line 1689 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 331 "./parse.y" /* yacc.c:1646  */
    { (yyval) = scon_stk_ptr; }
#line 1695 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 339 "./parse.y" /* yacc.c:1646  */
    { synerr( "bad start condition list" ); }
#line 1701 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 343 "./parse.y" /* yacc.c:1646  */
    {
			if ( (scnum = sclookup( nmstr )) == 0 )
				format_pinpoint_message(
					"undeclared start condition %s",
					nmstr );
			else
				{
				for ( i = 1; i <= scon_stk_ptr; ++i )
					if ( scon_stk[i] == scnum )
						{
						format_warn(
							"<%s> specified twice",
							scname[scnum] );
						break;
						}

				if ( i > scon_stk_ptr )
					scon_stk[++scon_stk_ptr] = scnum;
				}
			}
#line 1726 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 366 "./parse.y" /* yacc.c:1646  */
    {
			if ( transchar[lastst[(yyvsp[0])]] != SYM_EPSILON )
				/* Provide final transition \now/ so it
				 * will be marked as a trailing context
				 * state.
				 */
				(yyvsp[0]) = link_machines( (yyvsp[0]),
						mkstate( SYM_EPSILON ) );

			mark_beginning_as_normal( (yyvsp[0]) );
			current_state_type = STATE_NORMAL;

			if ( previous_continued_action )
				{
				/* We need to treat this as variable trailing
				 * context so that the backup does not happen
				 * in the action but before the action switch
				 * statement.  If the backup happens in the
				 * action, then the rules "falling into" this
				 * one's action will *also* do the backup,
				 * erroneously.
				 */
				if ( ! varlength || headcnt != 0 )
					warn(
		"trailing context made variable due to preceding '|' action" );

				/* Mark as variable. */
				varlength = true;
				headcnt = 0;
				}

			if ( lex_compat || (varlength && headcnt == 0) )
				{ /* variable trailing context rule */
				/* Mark the first part of the rule as the
				 * accepting "head" part of a trailing
				 * context rule.
				 *
				 * By the way, we didn't do this at the
				 * beginning of this production because back
				 * then current_state_type was set up for a
				 * trail rule, and add_accept() can create
				 * a new state ...
				 */
				add_accept( (yyvsp[-1]),
					num_rules | YY_TRAILING_HEAD_MASK );
				variable_trail_rule = true;
				}
			
			else
				trailcnt = rulelen;

			(yyval) = link_machines( (yyvsp[-1]), (yyvsp[0]) );
			}
#line 1784 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 421 "./parse.y" /* yacc.c:1646  */
    { synerr( "trailing context used twice" ); }
#line 1790 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 424 "./parse.y" /* yacc.c:1646  */
    {
			headcnt = 0;
			trailcnt = 1;
			rulelen = 1;
			varlength = false;

			current_state_type = STATE_TRAILING_CONTEXT;

			if ( trlcontxt )
				{
				synerr( "trailing context used twice" );
				(yyval) = mkstate( SYM_EPSILON );
				}

			else if ( previous_continued_action )
				{
				/* See the comment in the rule for "re2 re"
				 * above.
				 */
				warn(
		"trailing context made variable due to preceding '|' action" );

				varlength = true;
				}

			if ( lex_compat || varlength )
				{
				/* Again, see the comment in the rule for
				 * "re2 re" above.
				 */
				add_accept( (yyvsp[-1]),
					num_rules | YY_TRAILING_HEAD_MASK );
				variable_trail_rule = true;
				}

			trlcontxt = true;

			eps = mkstate( SYM_EPSILON );
			(yyval) = link_machines( (yyvsp[-1]),
				link_machines( eps, mkstate( '\n' ) ) );
			}
#line 1836 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 467 "./parse.y" /* yacc.c:1646  */
    {
			(yyval) = (yyvsp[0]);

			if ( trlcontxt )
				{
				if ( lex_compat || (varlength && headcnt == 0) )
					/* Both head and trail are
					 * variable-length.
					 */
					variable_trail_rule = true;
				else
					trailcnt = rulelen;
				}
			}
#line 1855 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 485 "./parse.y" /* yacc.c:1646  */
    {
			varlength = true;
			(yyval) = mkor( (yyvsp[-2]), (yyvsp[0]) );
			}
#line 1864 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 491 "./parse.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1870 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 496 "./parse.y" /* yacc.c:1646  */
    {
			/* This rule is written separately so the
			 * reduction will occur before the trailing
			 * series is parsed.
			 */

			if ( trlcontxt )
				synerr( "trailing context used twice" );
			else
				trlcontxt = true;

			if ( varlength )
				/* We hope the trailing context is
				 * fixed-length.
				 */
				varlength = false;
			else
				headcnt = rulelen;

			rulelen = 0;

			current_state_type = STATE_TRAILING_CONTEXT;
			(yyval) = (yyvsp[-1]);
			}
#line 1899 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 523 "./parse.y" /* yacc.c:1646  */
    {
			/* This is where concatenation of adjacent patterns
			 * gets done.
			 */
			(yyval) = link_machines( (yyvsp[-1]), (yyvsp[0]) );
			}
#line 1910 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 531 "./parse.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1916 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 535 "./parse.y" /* yacc.c:1646  */
    {
			varlength = true;

			(yyval) = mkclos( (yyvsp[-1]) );
			}
#line 1926 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 542 "./parse.y" /* yacc.c:1646  */
    {
			varlength = true;
			(yyval) = mkposcl( (yyvsp[-1]) );
			}
#line 1935 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 548 "./parse.y" /* yacc.c:1646  */
    {
			varlength = true;
			(yyval) = mkopt( (yyvsp[-1]) );
			}
#line 1944 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 554 "./parse.y" /* yacc.c:1646  */
    {
			varlength = true;

			if ( (yyvsp[-3]) > (yyvsp[-1]) || (yyvsp[-3]) < 0 )
				{
				synerr( "bad iteration values" );
				(yyval) = (yyvsp[-5]);
				}
			else
				{
				if ( (yyvsp[-3]) == 0 )
					{
					if ( (yyvsp[-1]) <= 0 )
						{
						synerr(
						"bad iteration values" );
						(yyval) = (yyvsp[-5]);
						}
					else
						(yyval) = mkopt(
							mkrep( (yyvsp[-5]), 1, (yyvsp[-1]) ) );
					}
				else
					(yyval) = mkrep( (yyvsp[-5]), (yyvsp[-3]), (yyvsp[-1]) );
				}
			}
#line 1975 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 582 "./parse.y" /* yacc.c:1646  */
    {
			varlength = true;

			if ( (yyvsp[-2]) <= 0 )
				{
				synerr( "iteration value must be positive" );
				(yyval) = (yyvsp[-4]);
				}

			else
				(yyval) = mkrep( (yyvsp[-4]), (yyvsp[-2]), INFINITY );
			}
#line 1992 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 596 "./parse.y" /* yacc.c:1646  */
    {
			/* The singleton could be something like "(foo)",
			 * in which case we have no idea what its length
			 * is, so we punt here.
			 */
			varlength = true;

			if ( (yyvsp[-1]) <= 0 )
				{
				synerr( "iteration value must be positive" );
				(yyval) = (yyvsp[-3]);
				}

			else
				(yyval) = link_machines( (yyvsp[-3]),
						copysingl( (yyvsp[-3]), (yyvsp[-1]) - 1 ) );
			}
#line 2014 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 615 "./parse.y" /* yacc.c:1646  */
    {
			if ( ! madeany )
				{
				/* Create the '.' character class. */
				anyccl = cclinit();
				ccladd( anyccl, '\n' );
				cclnegate( anyccl );

				if ( useecs )
					mkeccl( ccltbl + cclmap[anyccl],
						ccllen[anyccl], nextecm,
						ecgroup, csize, csize );

				madeany = true;
				}

			++rulelen;

			(yyval) = mkstate( -anyccl );
			}
#line 2039 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 637 "./parse.y" /* yacc.c:1646  */
    {
			if ( ! cclsorted )
				/* Sort characters for fast searching.  We
				 * use a shell sort since this list could
				 * be large.
				 */
				cshell( ccltbl + cclmap[(yyvsp[0])], ccllen[(yyvsp[0])], true );

			if ( useecs )
				mkeccl( ccltbl + cclmap[(yyvsp[0])], ccllen[(yyvsp[0])],
					nextecm, ecgroup, csize, csize );

			++rulelen;

			(yyval) = mkstate( -(yyvsp[0]) );
			}
#line 2060 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 655 "./parse.y" /* yacc.c:1646  */
    {
			++rulelen;

			(yyval) = mkstate( -(yyvsp[0]) );
			}
#line 2070 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 662 "./parse.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 2076 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 665 "./parse.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 2082 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 668 "./parse.y" /* yacc.c:1646  */
    {
			++rulelen;

			if ( caseins && (yyvsp[0]) >= 'A' && (yyvsp[0]) <= 'Z' )
				(yyvsp[0]) = clower( (yyvsp[0]) );

			(yyval) = mkstate( (yyvsp[0]) );
			}
#line 2095 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 679 "./parse.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 2101 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 682 "./parse.y" /* yacc.c:1646  */
    {
			cclnegate( (yyvsp[-1]) );
			(yyval) = (yyvsp[-1]);
			}
#line 2110 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 689 "./parse.y" /* yacc.c:1646  */
    {
			if ( caseins )
				{
				if ( (yyvsp[-2]) >= 'A' && (yyvsp[-2]) <= 'Z' )
					(yyvsp[-2]) = clower( (yyvsp[-2]) );
				if ( (yyvsp[0]) >= 'A' && (yyvsp[0]) <= 'Z' )
					(yyvsp[0]) = clower( (yyvsp[0]) );
				}

			if ( (yyvsp[-2]) > (yyvsp[0]) )
				synerr( "negative range in character class" );

			else
				{
				for ( i = (yyvsp[-2]); i <= (yyvsp[0]); ++i )
					ccladd( (yyvsp[-3]), i );

				/* Keep track if this ccl is staying in
				 * alphabetical order.
				 */
				cclsorted = cclsorted && ((yyvsp[-2]) > lastchar);
				lastchar = (yyvsp[0]);
				}

			(yyval) = (yyvsp[-3]);
			}
#line 2141 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 717 "./parse.y" /* yacc.c:1646  */
    {
			if ( caseins && (yyvsp[0]) >= 'A' && (yyvsp[0]) <= 'Z' )
				(yyvsp[0]) = clower( (yyvsp[0]) );

			ccladd( (yyvsp[-1]), (yyvsp[0]) );
			cclsorted = cclsorted && ((yyvsp[0]) > lastchar);
			lastchar = (yyvsp[0]);
			(yyval) = (yyvsp[-1]);
			}
#line 2155 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 728 "./parse.y" /* yacc.c:1646  */
    {
			/* Too hard to properly maintain cclsorted. */
			cclsorted = false;
			(yyval) = (yyvsp[-1]);
			}
#line 2165 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 735 "./parse.y" /* yacc.c:1646  */
    {
			cclsorted = true;
			lastchar = 0;
			currccl = (yyval) = cclinit();
			}
#line 2175 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 742 "./parse.y" /* yacc.c:1646  */
    { CCL_EXPR(isalnum) }
#line 2181 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 743 "./parse.y" /* yacc.c:1646  */
    { CCL_EXPR(isalpha) }
#line 2187 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 744 "./parse.y" /* yacc.c:1646  */
    { CCL_EXPR(IS_BLANK) }
#line 2193 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 745 "./parse.y" /* yacc.c:1646  */
    { CCL_EXPR(iscntrl) }
#line 2199 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 746 "./parse.y" /* yacc.c:1646  */
    { CCL_EXPR(isdigit) }
#line 2205 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 747 "./parse.y" /* yacc.c:1646  */
    { CCL_EXPR(isgraph) }
#line 2211 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 748 "./parse.y" /* yacc.c:1646  */
    { CCL_EXPR(islower) }
#line 2217 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 749 "./parse.y" /* yacc.c:1646  */
    { CCL_EXPR(isprint) }
#line 2223 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 750 "./parse.y" /* yacc.c:1646  */
    { CCL_EXPR(ispunct) }
#line 2229 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 751 "./parse.y" /* yacc.c:1646  */
    { CCL_EXPR(isspace) }
#line 2235 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 752 "./parse.y" /* yacc.c:1646  */
    {
				if ( caseins )
					CCL_EXPR(islower)
				else
					CCL_EXPR(isupper)
				}
#line 2246 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 758 "./parse.y" /* yacc.c:1646  */
    { CCL_EXPR(isxdigit) }
#line 2252 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 762 "./parse.y" /* yacc.c:1646  */
    {
			if ( caseins && (yyvsp[0]) >= 'A' && (yyvsp[0]) <= 'Z' )
				(yyvsp[0]) = clower( (yyvsp[0]) );

			++rulelen;

			(yyval) = link_machines( (yyvsp[-1]), mkstate( (yyvsp[0]) ) );
			}
#line 2265 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 772 "./parse.y" /* yacc.c:1646  */
    { (yyval) = mkstate( SYM_EPSILON ); }
#line 2271 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2275 "y.tab.c" /* yacc.c:1646  */
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
#line 775 "./parse.y" /* yacc.c:1906  */



/* build_eof_action - build the "<<EOF>>" action for the active start
 *                    conditions
 */

void build_eof_action()
	{
	register int i;
	char action_text[MAXLINE];

	for ( i = 1; i <= scon_stk_ptr; ++i )
		{
		if ( sceof[scon_stk[i]] )
			format_pinpoint_message(
				"multiple <<EOF>> rules for start condition %s",
				scname[scon_stk[i]] );

		else
			{
			sceof[scon_stk[i]] = true;
			sprintf( action_text, "case YY_STATE_EOF(%s):\n",
				scname[scon_stk[i]] );
			add_action( action_text );
			}
		}

	line_directive_out( (FILE *) 0, 1 );

	/* This isn't a normal rule after all - don't count it as
	 * such, so we don't have any holes in the rule numbering
	 * (which make generating "rule can never match" warnings
	 * more difficult.
	 */
	--num_rules;
	++num_eof_rules;
	}


/* format_synerr - write out formatted syntax error */

void format_synerr( msg, arg )
char msg[], arg[];
	{
	char errmsg[MAXLINE];

	(void) sprintf( errmsg, msg, arg );
	synerr( errmsg );
	}


/* synerr - report a syntax error */

void synerr( str )
char str[];
	{
	syntaxerror = true;
	pinpoint_message( str );
	}


/* format_warn - write out formatted warning */

void format_warn( msg, arg )
char msg[], arg[];
	{
	char warn_msg[MAXLINE];

	(void) sprintf( warn_msg, msg, arg );
	warn( warn_msg );
	}


/* warn - report a warning, unless -w was given */

void warn( str )
char str[];
	{
	line_warning( str, linenum );
	}

/* format_pinpoint_message - write out a message formatted with one string,
 *			     pinpointing its location
 */

void format_pinpoint_message( msg, arg )
char msg[], arg[];
	{
	char errmsg[MAXLINE];

	(void) sprintf( errmsg, msg, arg );
	pinpoint_message( errmsg );
	}


/* pinpoint_message - write out a message, pinpointing its location */

void pinpoint_message( str )
char str[];
	{
	line_pinpoint( str, linenum );
	}


/* line_warning - report a warning at a given line, unless -w was given */

void line_warning( str, line )
char str[];
int line;
	{
	char warning[MAXLINE];

	if ( ! nowarn )
		{
		sprintf( warning, "warning, %s", str );
		line_pinpoint( warning, line );
		}
	}


/* line_pinpoint - write out a message, pinpointing it at the given line */

void line_pinpoint( str, line )
char str[];
int line;
	{
	fprintf( stderr, "\"%s\", line %d: %s\n", infilename, line, str );
	}


/* yyerror - eat up an error message from the parser;
 *	     currently, messages are ignore
 */

void yyerror( msg )
char msg[];
	{
	}
