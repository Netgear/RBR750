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
#line 31 "cfparse.y" /* yacc.c:339  */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/queue.h>
#include <sys/time.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>

#include "dhcp6.h"
#include "config.h"
#include "common.h"

extern int lineno;
extern int cfdebug;

extern void yywarn __P((char *, ...))
	__attribute__((__format__(__printf__, 1, 2)));
extern void yyerror __P((char *, ...))
	__attribute__((__format__(__printf__, 1, 2)));

#define MAKE_NAMELIST(l, n, p) do { \
	(l) = (struct cf_namelist *)malloc(sizeof(*(l))); \
	if ((l) == NULL) { \
		yywarn("can't allocate memory"); \
		if (p) cleanup_cflist(p); \
		return (-1); \
	} \
	memset((l), 0, sizeof(*(l))); \
	l->line = lineno; \
	l->name = (n); \
	l->params = (p); \
	} while (0)

#define MAKE_CFLIST(l, t, pp, pl) do { \
	(l) = (struct cf_list *)malloc(sizeof(*(l))); \
	if ((l) == NULL) { \
		yywarn("can't allocate memory"); \
		if (pp) free(pp); \
		if (pl) cleanup_cflist(pl); \
		return (-1); \
	} \
	memset((l), 0, sizeof(*(l))); \
	l->line = lineno; \
	l->type = (t); \
	l->ptr = (pp); \
	l->list = (pl); \
	l->tail = (l); \
	} while (0)

static struct cf_namelist *iflist_head, *hostlist_head, *iapdlist_head;
static struct cf_namelist *addrpoollist_head;
static struct cf_namelist *authinfolist_head, *keylist_head;
static struct cf_namelist *ianalist_head;
static struct cf_namelist *vendorclasslist_head;
static struct cf_namelist *vendorlist_head;
struct cf_list *cf_dns_list, *cf_dns_name_list, *cf_ntp_list;
struct cf_list *cf_sip_list, *cf_sip_name_list;
struct cf_list *cf_nis_list, *cf_nis_name_list;
struct cf_list *cf_nisp_list, *cf_nisp_name_list;
struct cf_list *cf_bcmcs_list, *cf_bcmcs_name_list;
long long cf_refreshtime = -1;

extern int yylex __P((void));
extern int cfswitch_buffer __P((char *));
static int add_namelist __P((struct cf_namelist *, struct cf_namelist **));
static void cleanup __P((void));
static void cleanup_namelist __P((struct cf_namelist *));
static void cleanup_cflist __P((struct cf_list *));

#line 141 "y.tab.c" /* yacc.c:339  */

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
    INTERFACE = 258,
    IFNAME = 259,
    PREFIX_INTERFACE = 260,
    SLA_ID = 261,
    SLA_LEN = 262,
    DUID_ID = 263,
    ID_ASSOC = 264,
    IA_PD = 265,
    IAID = 266,
    IA_NA = 267,
    ADDRESS = 268,
    REQUEST = 269,
    SEND = 270,
    ALLOW = 271,
    PREFERENCE = 272,
    HOST = 273,
    HOSTNAME = 274,
    DUID = 275,
    OPTION = 276,
    RAPID_COMMIT = 277,
    DNS_SERVERS = 278,
    DNS_NAME = 279,
    NTP_SERVERS = 280,
    REFRESHTIME = 281,
    SOLMAPMIN = 282,
    SOLMAXRT = 283,
    SOLUSERCLASS = 284,
    OPT_VENDOR_SPEC = 285,
    SIP_SERVERS = 286,
    SIP_NAME = 287,
    NIS_SERVERS = 288,
    NIS_NAME = 289,
    NISP_SERVERS = 290,
    NISP_NAME = 291,
    BCMCS_SERVERS = 292,
    BCMCS_NAME = 293,
    INFO_ONLY = 294,
    SCRIPT = 295,
    DELAYEDKEY = 296,
    AUTHENTICATION = 297,
    PROTOCOL = 298,
    ALGORITHM = 299,
    DELAYED = 300,
    RECONFIG = 301,
    HMACMD5 = 302,
    MONOCOUNTER = 303,
    AUTHNAME = 304,
    RDM = 305,
    KEY = 306,
    KEYINFO = 307,
    REALM = 308,
    KEYID = 309,
    SECRET = 310,
    KEYNAME = 311,
    EXPIRE = 312,
    ADDRPOOL = 313,
    POOLNAME = 314,
    RANGE = 315,
    TO = 316,
    ADDRESS_POOL = 317,
    INCLUDE = 318,
    VENDOR_CLASS = 319,
    VENDOR_CLASS_IANA = 320,
    VENDOR_CLASS_DATA = 321,
    VENDOR_SPEC = 322,
    VENDOR_OUI = 323,
    VENDOR_SN = 324,
    VENDOR_PD = 325,
    VENDOR_ORO = 326,
    VENDOR_DEVTYPE = 327,
    VENDOR_ECMESAFE = 328,
    VENDOR_DEVSN = 329,
    VENDOR_DEVHWVER = 330,
    VENDOR_DEVSWVER = 331,
    VENDOR_DEVBLVER = 332,
    VENDOR_DEVVENDEROUI = 333,
    VENDOR_DEVMODELID = 334,
    VENDOR_DEVVENDERID = 335,
    NUMBER = 336,
    SLASH = 337,
    EOS = 338,
    BCL = 339,
    ECL = 340,
    STRING = 341,
    QSTRING = 342,
    PREFIX = 343,
    INFINITY = 344,
    COMMA = 345
  };
#endif
/* Tokens.  */
#define INTERFACE 258
#define IFNAME 259
#define PREFIX_INTERFACE 260
#define SLA_ID 261
#define SLA_LEN 262
#define DUID_ID 263
#define ID_ASSOC 264
#define IA_PD 265
#define IAID 266
#define IA_NA 267
#define ADDRESS 268
#define REQUEST 269
#define SEND 270
#define ALLOW 271
#define PREFERENCE 272
#define HOST 273
#define HOSTNAME 274
#define DUID 275
#define OPTION 276
#define RAPID_COMMIT 277
#define DNS_SERVERS 278
#define DNS_NAME 279
#define NTP_SERVERS 280
#define REFRESHTIME 281
#define SOLMAPMIN 282
#define SOLMAXRT 283
#define SOLUSERCLASS 284
#define OPT_VENDOR_SPEC 285
#define SIP_SERVERS 286
#define SIP_NAME 287
#define NIS_SERVERS 288
#define NIS_NAME 289
#define NISP_SERVERS 290
#define NISP_NAME 291
#define BCMCS_SERVERS 292
#define BCMCS_NAME 293
#define INFO_ONLY 294
#define SCRIPT 295
#define DELAYEDKEY 296
#define AUTHENTICATION 297
#define PROTOCOL 298
#define ALGORITHM 299
#define DELAYED 300
#define RECONFIG 301
#define HMACMD5 302
#define MONOCOUNTER 303
#define AUTHNAME 304
#define RDM 305
#define KEY 306
#define KEYINFO 307
#define REALM 308
#define KEYID 309
#define SECRET 310
#define KEYNAME 311
#define EXPIRE 312
#define ADDRPOOL 313
#define POOLNAME 314
#define RANGE 315
#define TO 316
#define ADDRESS_POOL 317
#define INCLUDE 318
#define VENDOR_CLASS 319
#define VENDOR_CLASS_IANA 320
#define VENDOR_CLASS_DATA 321
#define VENDOR_SPEC 322
#define VENDOR_OUI 323
#define VENDOR_SN 324
#define VENDOR_PD 325
#define VENDOR_ORO 326
#define VENDOR_DEVTYPE 327
#define VENDOR_ECMESAFE 328
#define VENDOR_DEVSN 329
#define VENDOR_DEVHWVER 330
#define VENDOR_DEVSWVER 331
#define VENDOR_DEVBLVER 332
#define VENDOR_DEVVENDEROUI 333
#define VENDOR_DEVMODELID 334
#define VENDOR_DEVVENDERID 335
#define NUMBER 336
#define SLASH 337
#define EOS 338
#define BCL 339
#define ECL 340
#define STRING 341
#define QSTRING 342
#define PREFIX 343
#define INFINITY 344
#define COMMA 345

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 133 "cfparse.y" /* yacc.c:355  */

	long long num;
	char* str;
	struct cf_list *list;
	struct dhcp6_prefix *prefix;
	struct dhcp6_range *range;
	struct dhcp6_poolspec *pool;

#line 370 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 387 "y.tab.c" /* yacc.c:358  */

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
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   305

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  91
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  131
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  292

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   345

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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   158,   158,   160,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   177,   189,   201,   210,   225,   234,
     243,   258,   267,   282,   291,   306,   315,   330,   359,   368,
     382,   391,   408,   420,   432,   443,   455,   467,   484,   485,
     503,   526,   527,   545,   553,   561,   569,   577,   585,   593,
     601,   610,   618,   626,   634,   645,   649,   659,   667,   675,
     683,   691,   699,   707,   715,   723,   731,   739,   747,   755,
     763,   771,   779,   787,   795,   801,   807,   816,   847,   874,
     907,   934,   967,   992,  1023,  1027,  1034,  1035,  1053,  1054,
    1065,  1075,  1076,  1094,  1102,  1113,  1114,  1132,  1143,  1144,
    1162,  1170,  1178,  1186,  1197,  1198,  1202,  1206,  1210,  1211,
    1229,  1237,  1245,  1253,  1264,  1265,  1283,  1294,  1295,  1313,
    1321,  1329,  1337,  1345,  1353,  1361,  1369,  1377,  1385,  1393,
    1401,  1409
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTERFACE", "IFNAME",
  "PREFIX_INTERFACE", "SLA_ID", "SLA_LEN", "DUID_ID", "ID_ASSOC", "IA_PD",
  "IAID", "IA_NA", "ADDRESS", "REQUEST", "SEND", "ALLOW", "PREFERENCE",
  "HOST", "HOSTNAME", "DUID", "OPTION", "RAPID_COMMIT", "DNS_SERVERS",
  "DNS_NAME", "NTP_SERVERS", "REFRESHTIME", "SOLMAPMIN", "SOLMAXRT",
  "SOLUSERCLASS", "OPT_VENDOR_SPEC", "SIP_SERVERS", "SIP_NAME",
  "NIS_SERVERS", "NIS_NAME", "NISP_SERVERS", "NISP_NAME", "BCMCS_SERVERS",
  "BCMCS_NAME", "INFO_ONLY", "SCRIPT", "DELAYEDKEY", "AUTHENTICATION",
  "PROTOCOL", "ALGORITHM", "DELAYED", "RECONFIG", "HMACMD5", "MONOCOUNTER",
  "AUTHNAME", "RDM", "KEY", "KEYINFO", "REALM", "KEYID", "SECRET",
  "KEYNAME", "EXPIRE", "ADDRPOOL", "POOLNAME", "RANGE", "TO",
  "ADDRESS_POOL", "INCLUDE", "VENDOR_CLASS", "VENDOR_CLASS_IANA",
  "VENDOR_CLASS_DATA", "VENDOR_SPEC", "VENDOR_OUI", "VENDOR_SN",
  "VENDOR_PD", "VENDOR_ORO", "VENDOR_DEVTYPE", "VENDOR_ECMESAFE",
  "VENDOR_DEVSN", "VENDOR_DEVHWVER", "VENDOR_DEVSWVER", "VENDOR_DEVBLVER",
  "VENDOR_DEVVENDEROUI", "VENDOR_DEVMODELID", "VENDOR_DEVVENDERID",
  "NUMBER", "SLASH", "EOS", "BCL", "ECL", "STRING", "QSTRING", "PREFIX",
  "INFINITY", "COMMA", "$accept", "statements", "statement",
  "interface_statement", "host_statement", "option_statement",
  "ia_statement", "authentication_statement", "key_statement",
  "include_statement", "addrpool_statement", "vendorclass_statement",
  "vendor_statement", "address_list", "address_list_ent", "declarations",
  "declaration", "dhcpoption_list", "dhcpoption", "rangeparam",
  "addressparam", "prefixparam", "poolparam", "duration", "iapdconf_list",
  "iapdconf", "prefix_interface", "ifparams", "ifparam", "ianaconf_list",
  "ianaconf", "authparam_list", "authparam", "authproto", "authalg",
  "authrdm", "keyparam_list", "keyparam", "vendorclassparam_list",
  "vendorclassparam", "vendorparam_list", "vendorparam", YY_NULLPTR
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345
};
# endif

#define YYPACT_NINF -197

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-197)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -197,    92,  -197,     9,    47,    17,   152,   -27,    -3,   -12,
     -17,    13,    30,  -197,  -197,  -197,  -197,  -197,  -197,  -197,
    -197,  -197,  -197,  -197,    48,    10,    18,    54,  -197,    52,
    -197,    -5,  -197,    53,  -197,    55,  -197,    56,  -197,    58,
      57,    62,    63,    15,    64,  -197,  -197,    65,  -197,    67,
    -197,  -197,   -71,    29,   -38,    69,     7,    70,    14,    71,
      25,    74,    50,    75,  -197,  -197,  -197,  -197,  -197,    94,
      -6,  -197,    -1,  -197,    -8,     3,  -197,  -197,  -197,  -197,
    -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,
      24,   -16,    11,   -36,    73,    95,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    78,  -197,   116,
      93,    93,    93,    99,   173,   120,   117,   119,   121,   122,
     123,   124,  -197,     1,   205,   128,   124,  -197,  -197,    19,
     116,   129,  -197,   130,    61,   167,   168,   131,   132,  -197,
     133,   137,   134,   135,   136,  -197,   140,   138,   141,  -197,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   153,
     154,   155,   156,  -197,    -9,   157,   160,   161,  -197,  -197,
    -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,
    -197,  -197,  -197,  -197,  -197,   186,   162,   158,   163,   164,
     166,   169,  -197,   170,   171,   182,   172,    -9,   174,  -197,
     176,   177,   178,   175,  -197,   179,   180,   181,  -197,  -197,
    -197,  -197,   183,  -197,   184,  -197,   185,   187,  -197,   188,
     189,   190,   191,  -197,  -197,   192,  -197,  -197,  -197,  -197,
    -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,
    -197,  -197,    -9,  -197,  -197,  -197,  -197,  -197,    93,  -197,
    -197,  -197,  -197,  -197,  -197,   165,  -197,    -9,  -197,   195,
    -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,
    -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,    -9,
      -4,    -9,   196,   197,   198,  -197,  -197,   199,   200,  -197,
    -197,  -197
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     3,     4,     5,     6,     7,     8,     9,
      11,    10,    12,    13,     0,     0,     0,     0,    38,     0,
      38,     0,    38,     0,    38,     0,    38,     0,    38,     0,
       0,     0,     0,     0,     0,   117,    41,     0,    86,     0,
      95,    41,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,   108,    41,    34,   114,     0,
       0,    86,     0,    95,     0,     0,    16,    40,    39,    17,
      18,    27,    19,    20,    21,    22,    23,    24,    25,    26,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   118,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    42,     0,     0,     0,     0,    87,    88,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,   109,     0,     0,     0,   115,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,     0,     0,     0,     0,    57,    63,
      64,    65,    66,    74,    75,    76,    73,    61,    62,    67,
      68,    69,    70,    71,    72,     0,     0,    55,     0,     0,
       0,     0,    45,     0,     0,     0,     0,     0,     0,    14,
       0,     0,     0,     0,    29,     0,     0,     0,    31,    15,
     104,   105,     0,   106,     0,   107,     0,     0,    32,     0,
       0,     0,     0,    33,    35,     0,    36,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
      85,    84,    78,    48,    59,    60,    58,    44,     0,    43,
      46,    50,    47,    51,    52,     0,    53,    82,    54,     0,
      49,    28,    91,    89,    30,    97,   100,   101,   102,   103,
     110,   111,   112,   113,   116,    79,    56,    77,    83,     0,
       0,    80,     0,     0,     0,    92,    81,     0,     0,    90,
      93,    94
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,
    -197,  -197,  -197,    28,  -197,   -11,  -197,  -111,   193,  -197,
     126,   118,  -197,  -196,   194,  -197,  -197,  -197,  -197,   206,
    -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,
    -197,  -197
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    52,    78,    70,   122,   186,   187,   196,
     165,   201,   198,   242,    72,   127,   128,   280,   285,    74,
     132,    90,   139,   212,   214,   216,    91,   145,    93,   149,
      69,   108
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
     188,   257,   282,   283,   124,   130,   124,   109,   110,   111,
     112,   113,    76,    24,   114,    77,   109,   110,   111,   112,
     113,    47,    40,   114,   109,   110,   111,   112,   113,    49,
     147,   114,   130,   115,   116,   117,    27,   140,   141,   142,
      75,   143,   115,   116,   117,    80,   275,    42,    77,   148,
     115,   116,   117,    41,   118,    92,   119,    25,    54,    26,
      56,   278,    58,   118,    60,   119,    62,   134,   135,   144,
      43,   118,   240,   119,   136,   137,    55,   131,    44,   120,
     241,   284,   121,   281,   125,   286,   202,   126,   133,   126,
      82,   121,     2,    77,    48,     3,   146,    84,    67,   121,
      77,     4,    50,   166,   206,   167,   210,   211,    86,   138,
       5,    77,    79,     6,    45,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,    46,    88,     7,   185,    77,   276,    51,    53,
      57,    64,    59,    61,     8,    63,    65,    66,    68,    71,
       9,    73,    81,    83,    85,    10,    11,    87,    89,    12,
     150,   163,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,    28,    29,    30,    31,   107,
     190,   191,   151,    32,    33,    34,    35,    36,    37,    38,
      39,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   164,   192,   193,   194,   199,   195,   197,   203,
     200,   204,   208,   209,   213,   218,   215,   217,   220,   223,
     219,   221,   222,   224,   226,   225,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   246,   236,   237,   238,   239,
     243,   244,   245,   255,   205,   247,   249,   250,   248,   251,
       0,   277,   252,   253,   254,   256,   207,   258,   259,   262,
     260,   261,   263,   264,   265,   123,   266,   267,   268,     0,
     269,   270,   271,   272,   273,   274,   279,   287,   288,   129,
       0,   289,   290,   291,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   189
};

static const yytype_int16 yycheck[] =
{
     111,   197,     6,     7,     5,    13,     5,    13,    14,    15,
      16,    17,    83,     4,    20,    86,    13,    14,    15,    16,
      17,    11,    49,    20,    13,    14,    15,    16,    17,    11,
      66,    20,    13,    39,    40,    41,    19,    53,    54,    55,
      51,    57,    39,    40,    41,    83,   242,    59,    86,    85,
      39,    40,    41,    56,    60,    66,    62,    10,    30,    12,
      32,   257,    34,    60,    36,    62,    38,    43,    44,    85,
      87,    60,    81,    62,    50,    51,    81,    85,    65,    85,
      89,    85,    88,   279,    85,   281,    85,    88,    85,    88,
      83,    88,     0,    86,    84,     3,    85,    83,    83,    88,
      86,     9,    84,    10,    85,    12,    45,    46,    83,    85,
      18,    86,    83,    21,    84,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    84,    83,    42,    42,    86,   248,    84,    87,
      87,    84,    87,    87,    52,    87,    84,    84,    84,    84,
      58,    84,    83,    83,    83,    63,    64,    83,    83,    67,
      87,    83,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    23,    24,    25,    26,    85,
      81,     8,    87,    31,    32,    33,    34,    35,    36,    37,
      38,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    86,    83,    87,    86,    83,    86,    86,     4,
      86,    83,    83,    83,    47,    83,    48,    86,    81,    83,
      87,    87,    87,    83,    83,    87,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    49,    83,    83,    83,    83,
      83,    81,    81,    61,   126,    83,    83,    83,    90,    83,
      -1,    86,    83,    83,    83,    83,   130,    83,    82,    84,
      83,    83,    83,    83,    83,    71,    83,    83,    83,    -1,
      83,    83,    83,    83,    83,    83,    81,    81,    81,    73,
      -1,    83,    83,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    92,     0,     3,     9,    18,    21,    42,    52,    58,
      63,    64,    67,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,     4,    10,    12,    19,    23,    24,
      25,    26,    31,    32,    33,    34,    35,    36,    37,    38,
      49,    56,    59,    87,    65,    84,    84,    11,    84,    11,
      84,    84,   104,    87,   104,    81,   104,    87,   104,    87,
     104,    87,   104,    87,    84,    84,    84,    83,    84,   131,
     106,    84,   115,    84,   120,   106,    83,    86,   105,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
     122,   127,   106,   129,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    85,   132,    13,
      14,    15,    16,    17,    20,    39,    40,    41,    60,    62,
      85,    88,   107,   115,     5,    85,    88,   116,   117,   120,
      13,    85,   121,    85,    43,    44,    50,    51,    85,   123,
      53,    54,    55,    57,    85,   128,    85,    66,    85,   130,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    83,    86,   111,    10,    12,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    42,   108,   109,   108,   109,
      81,     8,    83,    87,    86,    86,   110,    86,   113,    83,
      86,   112,    85,     4,    83,   112,    85,   111,    83,    83,
      45,    46,   124,    47,   125,    48,   126,    86,    83,    87,
      81,    87,    87,    83,    83,    87,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      81,    89,   114,    83,    81,    81,    49,    83,    90,    83,
      83,    83,    83,    83,    83,    61,    83,   114,    83,    82,
      83,    83,    84,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,   114,   108,    86,   114,    81,
     118,   114,     6,     7,    85,   119,   114,    81,    81,    83,
      83,    83
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    91,    92,    92,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    94,    95,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    97,    97,
      97,    97,    98,    99,   100,   101,   102,   103,   104,   104,
     105,   106,   106,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   108,   108,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   110,   111,   111,
     112,   112,   113,   113,   114,   114,   115,   115,   116,   116,
     117,   118,   118,   119,   119,   120,   120,   121,   122,   122,
     123,   123,   123,   123,   124,   124,   125,   126,   127,   127,
     128,   128,   128,   128,   129,   129,   130,   131,   131,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     6,     6,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     7,     6,
       7,     6,     6,     6,     3,     6,     6,     5,     0,     2,
       1,     0,     2,     3,     3,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     3,     1,     2,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     2,     3,
       4,     5,     2,     3,     1,     1,     0,     2,     1,     3,
       6,     0,     2,     3,     3,     0,     2,     3,     0,     2,
       3,     3,     3,     3,     1,     1,     1,     1,     0,     2,
       3,     3,     3,     3,     0,     2,     3,     0,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3
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
        case 14:
#line 178 "cfparse.y" /* yacc.c:1646  */
    {
		struct cf_namelist *ifl;

		MAKE_NAMELIST(ifl, (yyvsp[-4].str), (yyvsp[-2].list));

		if (add_namelist(ifl, &iflist_head))
			return (-1);
	}
#line 1697 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 190 "cfparse.y" /* yacc.c:1646  */
    {
		struct cf_namelist *host;

		MAKE_NAMELIST(host, (yyvsp[-4].str), (yyvsp[-2].list));

		if (add_namelist(host, &hostlist_head))
			return (-1);
	}
#line 1710 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 202 "cfparse.y" /* yacc.c:1646  */
    {
			if (cf_dns_list == NULL)
				cf_dns_list = (yyvsp[-1].list);
			else {
				cf_dns_list->tail->next = (yyvsp[-1].list);
				cf_dns_list->tail = (yyvsp[-1].list)->tail;
			}
		}
#line 1723 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 211 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[-1].str), NULL);

			if (cf_dns_name_list == NULL) {
				cf_dns_name_list = l;
				cf_dns_name_list->tail = l;
				cf_dns_name_list->next = NULL;
			} else {
				cf_dns_name_list->tail->next = l;
				cf_dns_name_list->tail = l->tail;
			}
		}
#line 1742 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 226 "cfparse.y" /* yacc.c:1646  */
    {
			if (cf_ntp_list == NULL)
				cf_ntp_list = (yyvsp[-1].list);
			else {
				cf_ntp_list->tail->next = (yyvsp[-1].list);
				cf_ntp_list->tail = (yyvsp[-1].list)->tail;
			}
		}
#line 1755 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 235 "cfparse.y" /* yacc.c:1646  */
    {
			if (cf_sip_list == NULL)
				cf_sip_list = (yyvsp[-1].list);
			else {
				cf_sip_list->tail->next = (yyvsp[-1].list);
				cf_sip_list->tail = (yyvsp[-1].list)->tail;
			}
		}
#line 1768 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 244 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[-1].str), NULL);

			if (cf_sip_name_list == NULL) {
				cf_sip_name_list = l;
				cf_sip_name_list->tail = l;
				cf_sip_name_list->next = NULL;
			} else {
				cf_sip_name_list->tail->next = l;
				cf_sip_name_list->tail = l->tail;
			}
		}
#line 1787 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 259 "cfparse.y" /* yacc.c:1646  */
    {
			if (cf_nis_list == NULL)
				cf_nis_list = (yyvsp[-1].list);
			else {
				cf_nis_list->tail->next = (yyvsp[-1].list);
				cf_nis_list->tail = (yyvsp[-1].list)->tail;
			}
		}
#line 1800 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 268 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[-1].str), NULL);

			if (cf_nis_name_list == NULL) {
				cf_nis_name_list = l;
				cf_nis_name_list->tail = l;
				cf_nis_name_list->next = NULL;
			} else {
				cf_nis_name_list->tail->next = l;
				cf_nis_name_list->tail = l->tail;
			}
		}
#line 1819 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 283 "cfparse.y" /* yacc.c:1646  */
    {
			if (cf_nisp_list == NULL)
				cf_nisp_list = (yyvsp[-1].list);
			else {
				cf_nisp_list->tail->next = (yyvsp[-1].list);
				cf_nisp_list->tail = (yyvsp[-1].list)->tail;
			}
		}
#line 1832 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 292 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[-1].str), NULL);

			if (cf_nisp_name_list == NULL) {
				cf_nisp_name_list = l;
				cf_nisp_name_list->tail = l;
				cf_nisp_name_list->next = NULL;
			} else {
				cf_nisp_name_list->tail->next = l;
				cf_nisp_name_list->tail = l->tail;
			}
		}
#line 1851 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 307 "cfparse.y" /* yacc.c:1646  */
    {
			if (cf_bcmcs_list == NULL)
				cf_bcmcs_list = (yyvsp[-1].list);
			else {
				cf_bcmcs_list->tail->next = (yyvsp[-1].list);
				cf_bcmcs_list->tail = (yyvsp[-1].list)->tail;
			}
		}
#line 1864 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 316 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[-1].str), NULL);

			if (cf_bcmcs_name_list == NULL) {
				cf_bcmcs_name_list = l;
				cf_bcmcs_name_list->tail = l;
				cf_bcmcs_name_list->next = NULL;
			} else {
				cf_bcmcs_name_list->tail->next = l;
				cf_bcmcs_name_list->tail = l->tail;
			}
		}
#line 1883 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 331 "cfparse.y" /* yacc.c:1646  */
    {
			if (cf_refreshtime == -1) {
				cf_refreshtime = (yyvsp[-1].num);
				if (cf_refreshtime < -1 ||
				    cf_refreshtime > 0xffffffff) {
					/*
					 * refresh time should not be negative
					 * according to the lex definition,
					 * but check it for safety.
					 */
					yyerror("refresh time is out of range");
				}
				if (cf_refreshtime < DHCP6_IRT_MINIMUM) {
					/*
					 * the value MUST NOT be smaller than
					 * IRT_MINIMUM.
					 */
					yyerror("refresh time is too small "
					    "(must not be smaller than %d)",
					    DHCP6_IRT_MINIMUM);
				}
			} else {
				yywarn("multiple refresh times (ignored)");
			}
		}
#line 1913 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 360 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_namelist *iapd;

			MAKE_NAMELIST(iapd, (yyvsp[-4].str), (yyvsp[-2].list));

			if (add_namelist(iapd, &iapdlist_head))
				return (-1);
		}
#line 1926 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 369 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_namelist *iapd;
			char *zero;

			if ((zero = strdup("0")) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			MAKE_NAMELIST(iapd, zero, (yyvsp[-2].list));

			if (add_namelist(iapd, &iapdlist_head))
				return (-1);
		}
#line 1944 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 383 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_namelist *iana;

			MAKE_NAMELIST(iana, (yyvsp[-4].str), (yyvsp[-2].list));

			if (add_namelist(iana, &ianalist_head))
				return (-1);
		}
#line 1957 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 392 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_namelist *iana;
			char *zero;

			if ((zero = strdup("0")) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			MAKE_NAMELIST(iana, zero, (yyvsp[-2].list));

			if (add_namelist(iana, &ianalist_head))
				return (-1);
		}
#line 1975 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 409 "cfparse.y" /* yacc.c:1646  */
    {
		struct cf_namelist *authinfo;

		MAKE_NAMELIST(authinfo, (yyvsp[-4].str), (yyvsp[-2].list));

		if (add_namelist(authinfo, &authinfolist_head))
			return (-1);
	}
#line 1988 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 421 "cfparse.y" /* yacc.c:1646  */
    {
		struct cf_namelist *key;

		MAKE_NAMELIST(key, (yyvsp[-4].str), (yyvsp[-2].list));

		if (add_namelist(key, &keylist_head))
			return (-1);
	}
#line 2001 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 433 "cfparse.y" /* yacc.c:1646  */
    {
		if (cfswitch_buffer((yyvsp[-1].str))) {
			free((yyvsp[-1].str));
			return (-1);
		}
		free((yyvsp[-1].str));
	}
#line 2013 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 444 "cfparse.y" /* yacc.c:1646  */
    {
		struct cf_namelist *pool;

		MAKE_NAMELIST(pool, (yyvsp[-4].str), (yyvsp[-2].list));

		if (add_namelist(pool, &addrpoollist_head))
			return (-1);
	}
#line 2026 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 456 "cfparse.y" /* yacc.c:1646  */
    {
		struct cf_namelist *vendor_class;

		MAKE_NAMELIST(vendor_class, (yyvsp[-4].num), (yyvsp[-2].list));

		if (add_namelist(vendor_class, &vendorclasslist_head))
			return (-1);
	}
#line 2039 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 468 "cfparse.y" /* yacc.c:1646  */
    {
		struct cf_namelist *vendor;
		char *zero;

		if ((zero = strdup("0")) == NULL) {
			yywarn("can't allocate memory");
			return (-1);
		}
		MAKE_NAMELIST(vendor, zero, (yyvsp[-2].list));

		if (add_namelist(vendor, &vendorlist_head))
			return (-1);
	}
#line 2057 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 484 "cfparse.y" /* yacc.c:1646  */
    { (yyval.list) = NULL; }
#line 2063 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 486 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *head;

			if ((head = (yyvsp[-1].list)) == NULL) {
				(yyvsp[0].list)->next = NULL;
				(yyvsp[0].list)->tail = (yyvsp[0].list);
				head = (yyvsp[0].list);
			} else {
				head->tail->next = (yyvsp[0].list);
				head->tail = (yyvsp[0].list)->tail;
			}

			(yyval.list) = head;
		}
#line 2082 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 504 "cfparse.y" /* yacc.c:1646  */
    {
		struct cf_list *l;
		struct in6_addr a0, *a;

		if (inet_pton(AF_INET6, (yyvsp[0].str), &a0) != 1) {
			yywarn("invalid IPv6 address: %s", (yyvsp[0].str));
			free((yyvsp[0].str));
			return (-1);
		}
		if ((a = malloc(sizeof(*a))) == NULL) {
			yywarn("can't allocate memory");
			return (-1);
		}
		*a = a0;

		MAKE_CFLIST(l, CFLISTENT_GENERIC, a, NULL);

		(yyval.list) = l;
	}
#line 2106 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 526 "cfparse.y" /* yacc.c:1646  */
    { (yyval.list) = NULL; }
#line 2112 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 528 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *head;

			if ((head = (yyvsp[-1].list)) == NULL) {
				(yyvsp[0].list)->next = NULL;
				(yyvsp[0].list)->tail = (yyvsp[0].list);
				head = (yyvsp[0].list);
			} else {
				head->tail->next = (yyvsp[0].list);
				head->tail = (yyvsp[0].list)->tail;
			}

			(yyval.list) = head;
		}
#line 2131 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 546 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_SEND, NULL, (yyvsp[-1].list));

			(yyval.list) = l;
		}
#line 2143 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 554 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_REQUEST, NULL, (yyvsp[-1].list));

			(yyval.list) = l;
		}
#line 2155 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 562 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_INFO_ONLY, NULL, NULL);
			/* no value */
			(yyval.list) = l;
		}
#line 2167 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 570 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_ALLOW, NULL, (yyvsp[-1].list));

			(yyval.list) = l;
		}
#line 2179 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 578 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_DUID, (yyvsp[-1].str), NULL);

			(yyval.list) = l;
		}
#line 2191 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 586 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_ADDRESS, (yyvsp[-1].prefix),NULL);

			(yyval.list) = l;
		}
#line 2203 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 594 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_PREFIX, (yyvsp[-1].prefix), NULL);

			(yyval.list) = l;
		}
#line 2215 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 602 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_PREFERENCE, NULL, NULL);
			l->num = (yyvsp[-1].num);

			(yyval.list) = l;
		}
#line 2228 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 611 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_SCRIPT, (yyvsp[-1].str), NULL);

			(yyval.list) = l;
		}
#line 2240 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 619 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_DELAYEDKEY, (yyvsp[-1].str), NULL);

			(yyval.list) = l;
		}
#line 2252 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 627 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_RANGE, (yyvsp[-1].range), NULL);

			(yyval.list) = l;
		}
#line 2264 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 635 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_ADDRESSPOOL, (yyvsp[-1].pool), NULL);

			(yyval.list) = l;
		}
#line 2276 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 646 "cfparse.y" /* yacc.c:1646  */
    {
			(yyval.list) = (yyvsp[0].list);
		}
#line 2284 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 650 "cfparse.y" /* yacc.c:1646  */
    {
			(yyvsp[-2].list)->next = (yyvsp[0].list);
			(yyvsp[-2].list)->tail = (yyvsp[0].list)->tail;

			(yyval.list) = (yyvsp[-2].list);
		}
#line 2295 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 660 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_RAPID_COMMIT, NULL, NULL);
			/* no value */
			(yyval.list) = l;
		}
#line 2307 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 668 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_AUTHINFO, NULL, NULL);
			l->ptr = (yyvsp[0].str);
			(yyval.list) = l;
		}
#line 2319 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 676 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_IA_PD, NULL, NULL);
			l->num = (yyvsp[0].num);
			(yyval.list) = l;
		}
#line 2331 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 684 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_IA_NA, NULL, NULL);
			l->num = (yyvsp[0].num);
			(yyval.list) = l;
		}
#line 2343 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 692 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_SIP, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
#line 2355 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 700 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_SIPNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
#line 2367 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 708 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_DNS, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
#line 2379 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 716 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_DNSNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
#line 2391 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 724 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NTP, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
#line 2403 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 732 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_REFRESHTIME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
#line 2415 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 740 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NIS, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
#line 2427 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 748 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NISNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
#line 2439 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 756 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NISP, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
#line 2451 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 764 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NISPNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
#line 2463 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 772 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_BCMCS, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
#line 2475 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 780 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_BCMCSNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
#line 2487 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 788 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_VENDORSPEC, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
#line 2499 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 796 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;
			MAKE_CFLIST(l, DHCPOPT_SOLMAPMIN, NULL, NULL);
			(yyval.list) = l;
		}
#line 2509 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 802 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;
			MAKE_CFLIST(l, DHCPOPT_SOLMAXRT, NULL, NULL);
			(yyval.list) = l;
		}
#line 2519 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 808 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;
			MAKE_CFLIST(l, DHCPOPT_USERCLASS, NULL, NULL);
			(yyval.list) = l;
		}
#line 2529 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 817 "cfparse.y" /* yacc.c:1646  */
    {
			struct dhcp6_range range0, *range;

			memset(&range0, 0, sizeof(range0));
			if (inet_pton(AF_INET6, (yyvsp[-2].str), &range0.min) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[-2].str));
				free((yyvsp[-2].str));
				free((yyvsp[0].str));
				return (-1);
			}
			if (inet_pton(AF_INET6, (yyvsp[0].str), &range0.max) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[0].str));
				free((yyvsp[-2].str));
				free((yyvsp[0].str));
				return (-1);
			}
			free((yyvsp[-2].str));
			free((yyvsp[0].str));

			if ((range = malloc(sizeof(*range))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*range = range0;

			(yyval.range) = range;
		}
#line 2561 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 848 "cfparse.y" /* yacc.c:1646  */
    {
			struct dhcp6_prefix pconf0, *pconf;

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[-1].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[-1].str));
				free((yyvsp[-1].str));
				return (-1);
			}
			free((yyvsp[-1].str));
			/* validate other parameters later */
			pconf0.plen = 128; /* XXX this field is ignored */
			if ((yyvsp[0].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[0].num);
			pconf0.vltime = pconf0.pltime;

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
#line 2592 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 875 "cfparse.y" /* yacc.c:1646  */
    {
			struct dhcp6_prefix pconf0, *pconf;

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[-2].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[-2].str));
				free((yyvsp[-2].str));
				return (-1);
			}
			free((yyvsp[-2].str));
			/* validate other parameters later */
			pconf0.plen = 128; /* XXX */
			if ((yyvsp[-1].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[-1].num);
			if ((yyvsp[0].num) < 0)
				pconf0.vltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.vltime = (u_int32_t)(yyvsp[0].num);

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
#line 2626 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 908 "cfparse.y" /* yacc.c:1646  */
    {
			struct dhcp6_prefix pconf0, *pconf;

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[-3].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[-3].str));
				free((yyvsp[-3].str));
				return (-1);
			}
			free((yyvsp[-3].str));
			/* validate other parameters later */
			pconf0.plen = (yyvsp[-1].num);
			if ((yyvsp[0].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[0].num);
			pconf0.vltime = pconf0.pltime;

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
#line 2657 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 935 "cfparse.y" /* yacc.c:1646  */
    {
			struct dhcp6_prefix pconf0, *pconf;

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[-4].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[-4].str));
				free((yyvsp[-4].str));
				return (-1);
			}
			free((yyvsp[-4].str));
			/* validate other parameters later */
			pconf0.plen = (yyvsp[-2].num);
			if ((yyvsp[-1].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[-1].num);
			if ((yyvsp[0].num) < 0)
				pconf0.vltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.vltime = (u_int32_t)(yyvsp[0].num);

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
#line 2691 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 968 "cfparse.y" /* yacc.c:1646  */
    {
			struct dhcp6_poolspec* pool;

			if ((pool = malloc(sizeof(*pool))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[-1].str));
				return (-1);
			}
			if ((pool->name = strdup((yyvsp[-1].str))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[-1].str));
				return (-1);
			}
			free((yyvsp[-1].str));

			/* validate other parameters later */
			if ((yyvsp[0].num) < 0)
				pool->pltime = DHCP6_DURATION_INFINITE;
			else
				pool->pltime = (u_int32_t)(yyvsp[0].num);
			pool->vltime = pool->pltime;

			(yyval.pool) = pool;
		}
#line 2720 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 993 "cfparse.y" /* yacc.c:1646  */
    {
			struct dhcp6_poolspec* pool;

			if ((pool = malloc(sizeof(*pool))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[-2].str));
				return (-1);
			}
			if ((pool->name = strdup((yyvsp[-2].str))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[-2].str));
				return (-1);
			}
			free((yyvsp[-2].str));

			/* validate other parameters later */
			if ((yyvsp[-1].num) < 0)
				pool->pltime = DHCP6_DURATION_INFINITE;
			else
				pool->pltime = (u_int32_t)(yyvsp[-1].num);
			if ((yyvsp[0].num) < 0)
				pool->vltime = DHCP6_DURATION_INFINITE;
			else
				pool->vltime = (u_int32_t)(yyvsp[0].num);

			(yyval.pool) = pool;
		}
#line 2752 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 1024 "cfparse.y" /* yacc.c:1646  */
    {
			(yyval.num) = -1;
		}
#line 2760 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 1028 "cfparse.y" /* yacc.c:1646  */
    {
			(yyval.num) = (yyvsp[0].num);
		}
#line 2768 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 1034 "cfparse.y" /* yacc.c:1646  */
    { (yyval.list) = NULL; }
#line 2774 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 1036 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *head;

			if ((head = (yyvsp[-1].list)) == NULL) {
				(yyvsp[0].list)->next = NULL;
				(yyvsp[0].list)->tail = (yyvsp[0].list);
				head = (yyvsp[0].list);
			} else {
				head->tail->next = (yyvsp[0].list);
				head->tail = (yyvsp[0].list)->tail;
			}

			(yyval.list) = head;
		}
#line 2793 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 1053 "cfparse.y" /* yacc.c:1646  */
    { (yyval.list) = (yyvsp[0].list); }
#line 2799 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 1055 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IACONF_PREFIX, (yyvsp[-1].prefix), NULL);

			(yyval.list) = l;
		}
#line 2811 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 1066 "cfparse.y" /* yacc.c:1646  */
    {
		struct cf_list *ifl;

		MAKE_CFLIST(ifl, IACONF_PIF, (yyvsp[-4].str), (yyvsp[-2].list));
		(yyval.list) = ifl;
	}
#line 2822 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 1075 "cfparse.y" /* yacc.c:1646  */
    { (yyval.list) = NULL; }
#line 2828 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 1077 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *head;

			if ((head = (yyvsp[-1].list)) == NULL) {
				(yyvsp[0].list)->next = NULL;
				(yyvsp[0].list)->tail = (yyvsp[0].list);
				head = (yyvsp[0].list);
			} else {
				head->tail->next = (yyvsp[0].list);
				head->tail = (yyvsp[0].list)->tail;
			}

			(yyval.list) = head;
		}
#line 2847 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 1095 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IFPARAM_SLA_ID, NULL, NULL);
			l->num = (yyvsp[-1].num);
			(yyval.list) = l;
		}
#line 2859 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 1103 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IFPARAM_SLA_LEN, NULL, NULL);
			l->num = (yyvsp[-1].num);
			(yyval.list) = l;
		}
#line 2871 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 1113 "cfparse.y" /* yacc.c:1646  */
    { (yyval.list) = NULL; }
#line 2877 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 1115 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *head;

			if ((head = (yyvsp[-1].list)) == NULL) {
				(yyvsp[0].list)->next = NULL;
				(yyvsp[0].list)->tail = (yyvsp[0].list);
				head = (yyvsp[0].list);
			} else {
				head->tail->next = (yyvsp[0].list);
				head->tail = (yyvsp[0].list)->tail;
			}

			(yyval.list) = head;
		}
#line 2896 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 1133 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IACONF_ADDR, (yyvsp[-1].prefix), NULL);

			(yyval.list) = l;
		}
#line 2908 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 1143 "cfparse.y" /* yacc.c:1646  */
    { (yyval.list) = NULL; }
#line 2914 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 1145 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *head;

			if ((head = (yyvsp[-1].list)) == NULL) {
				(yyvsp[0].list)->next = NULL;
				(yyvsp[0].list)->tail = (yyvsp[0].list);
				head = (yyvsp[0].list);
			} else {
				head->tail->next = (yyvsp[0].list);
				head->tail = (yyvsp[0].list)->tail;
			}

			(yyval.list) = head;
		}
#line 2933 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 1163 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_PROTO, NULL, NULL);
			l->num = (yyvsp[-1].num);
			(yyval.list) = l;
		}
#line 2945 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 1171 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_ALG, NULL, NULL);
			l->num = (yyvsp[-1].num);
			(yyval.list) = l;
		}
#line 2957 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 1179 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_RDM, NULL, NULL);
			l->num = (yyvsp[-1].num);
			(yyval.list) = l;
		}
#line 2969 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 1187 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_KEY, NULL, NULL);
			l->ptr = (yyvsp[-1].str);
			(yyval.list) = l;
		}
#line 2981 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 1197 "cfparse.y" /* yacc.c:1646  */
    { (yyval.num) = DHCP6_AUTHPROTO_DELAYED; }
#line 2987 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 1198 "cfparse.y" /* yacc.c:1646  */
    { (yyval.num) = DHCP6_AUTHPROTO_RECONFIG; }
#line 2993 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 1202 "cfparse.y" /* yacc.c:1646  */
    { (yyval.num) = DHCP6_AUTHALG_HMACMD5; }
#line 2999 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 1206 "cfparse.y" /* yacc.c:1646  */
    { (yyval.num) = DHCP6_AUTHRDM_MONOCOUNTER; }
#line 3005 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 1210 "cfparse.y" /* yacc.c:1646  */
    { (yyval.list) = NULL; }
#line 3011 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 1212 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *head;

			if ((head = (yyvsp[-1].list)) == NULL) {
				(yyvsp[0].list)->next = NULL;
				(yyvsp[0].list)->tail = (yyvsp[0].list);
				head = (yyvsp[0].list);
			} else {
				head->tail->next = (yyvsp[0].list);
				head->tail = (yyvsp[0].list)->tail;
			}

			(yyval.list) = head;
		}
#line 3030 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 1230 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_REALM, NULL, NULL);
			l->ptr = (yyvsp[-1].str);
			(yyval.list) = l;
		}
#line 3042 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 1238 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_KEYID, NULL, NULL);
			l->num = (yyvsp[-1].num);
			(yyval.list) = l;
		}
#line 3054 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 1246 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_SECRET, NULL, NULL);
			l->ptr = (yyvsp[-1].str);
			(yyval.list) = l;
		}
#line 3066 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 1254 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_EXPIRE, NULL, NULL);
			l->ptr = (yyvsp[-1].str);
			(yyval.list) = l;
		}
#line 3078 "y.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 1264 "cfparse.y" /* yacc.c:1646  */
    { (yyval.list) = NULL; }
#line 3084 "y.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 1266 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *head;

			if ((head = (yyvsp[-1].list)) == NULL) {
				(yyvsp[0].list)->next = NULL;
				(yyvsp[0].list)->tail = (yyvsp[0].list);
				head = (yyvsp[0].list);
			} else {
				head->tail->next = (yyvsp[0].list);
				head->tail = (yyvsp[0].list)->tail;
			}

			(yyval.list) = head;
		}
#line 3103 "y.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 1284 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, VENDORCLASSPARAM_VENDOR_CLASS_DATA, NULL, NULL);
			l->ptr = (yyvsp[-1].str);
			(yyval.list) = l;
		}
#line 3115 "y.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 1294 "cfparse.y" /* yacc.c:1646  */
    { (yyval.list) = NULL; }
#line 3121 "y.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 1296 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *head;

			if ((head = (yyvsp[-1].list)) == NULL) {
				(yyvsp[0].list)->next = NULL;
				(yyvsp[0].list)->tail = (yyvsp[0].list);
				head = (yyvsp[0].list);
			} else {
				head->tail->next = (yyvsp[0].list);
				head->tail = (yyvsp[0].list)->tail;
			}

			(yyval.list) = head;
		}
#line 3140 "y.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 1314 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, VENDORPARAM_VENDOR_OUI, NULL, NULL);
			l->ptr = (yyvsp[-1].str);
			(yyval.list) = l;
		}
#line 3152 "y.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 1322 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, VENDORPARAM_VENDOR_SN, NULL, NULL);
			l->ptr = (yyvsp[-1].str);
			(yyval.list) = l;
		}
#line 3164 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 1330 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, VENDORPARAM_VENDOR_PD, NULL, NULL);
			l->ptr = (yyvsp[-1].str);
			(yyval.list) = l;
		}
#line 3176 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 1338 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, VENDORPARAM_VENDOR_ORO, NULL, NULL);
			l->ptr = (yyvsp[-1].str);
			(yyval.list) = l;
		}
#line 3188 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 1346 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, VENDORPARAM_VENDOR_DEVTYPE, NULL, NULL);
			l->ptr = (yyvsp[-1].str);
			(yyval.list) = l;
		}
#line 3200 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 1354 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, VENDORPARAM_VENDOR_ECMESAFE, NULL, NULL);
			l->ptr = (yyvsp[-1].str);
			(yyval.list) = l;
		}
#line 3212 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 1362 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, VENDORPARAM_VENDOR_DEVSN, NULL, NULL);
			l->ptr = (yyvsp[-1].str);
			(yyval.list) = l;
		}
#line 3224 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 1370 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, VENDORPARAM_VENDOR_DEVHWVER, NULL, NULL);
			l->ptr = (yyvsp[-1].str);
			(yyval.list) = l;
		}
#line 3236 "y.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 1378 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, VENDORPARAM_VENDOR_DEVSWVER, NULL, NULL);
			l->ptr = (yyvsp[-1].str);
			(yyval.list) = l;
		}
#line 3248 "y.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 1386 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, VENDORPARAM_VENDOR_DEVBLVER, NULL, NULL);
			l->ptr = (yyvsp[-1].str);
			(yyval.list) = l;
		}
#line 3260 "y.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 1394 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, VENDORPARAM_VENDOR_DEVVENDEROUI, NULL, NULL);
			l->ptr = (yyvsp[-1].str);
			(yyval.list) = l;
		}
#line 3272 "y.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 1402 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, VENDORPARAM_VENDOR_DEVMODELID, NULL, NULL);
			l->ptr = (yyvsp[-1].str);
			(yyval.list) = l;
		}
#line 3284 "y.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 1410 "cfparse.y" /* yacc.c:1646  */
    {
			struct cf_list *l;

			MAKE_CFLIST(l, VENDORPARAM_VENDOR_DEVVENDERID, NULL, NULL);
			l->ptr = (yyvsp[-1].str);
			(yyval.list) = l;
		}
#line 3296 "y.tab.c" /* yacc.c:1646  */
    break;


#line 3300 "y.tab.c" /* yacc.c:1646  */
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
#line 1418 "cfparse.y" /* yacc.c:1906  */

/* supplement routines for configuration */
static int
add_namelist(new, headp)
	struct cf_namelist *new, **headp;
{
	struct cf_namelist *n;

	/* check for duplicated configuration */
	for (n = *headp; n; n = n->next) {
		if (strcmp(n->name, new->name) == 0) {
			yywarn("duplicated name: %s (ignored)",
			       new->name);
			cleanup_namelist(new);
			return (0);
		}
	}

	new->next = *headp;
	*headp = new;

	return (0);
}

/* free temporary resources */
static void
cleanup()
{
	cleanup_namelist(iflist_head);
	iflist_head = NULL;
	cleanup_namelist(hostlist_head);
	hostlist_head = NULL;
	cleanup_namelist(iapdlist_head);
	iapdlist_head = NULL;
	cleanup_namelist(ianalist_head);
	ianalist_head = NULL;
	cleanup_namelist(authinfolist_head);
	authinfolist_head = NULL;
	cleanup_namelist(keylist_head);
	keylist_head = NULL;
	cleanup_namelist(addrpoollist_head);
	addrpoollist_head = NULL;
	cleanup_namelist(vendorclasslist_head);
	vendorclasslist_head = NULL;
	cleanup_namelist(vendorlist_head);
	vendorlist_head = NULL;

	cleanup_cflist(cf_sip_list);
	cf_sip_list = NULL;
	cleanup_cflist(cf_sip_name_list);
	cf_sip_name_list = NULL;
	cleanup_cflist(cf_dns_list);
	cf_dns_list = NULL;
	cleanup_cflist(cf_dns_name_list);
	cf_dns_name_list = NULL;
	cleanup_cflist(cf_ntp_list);
	cf_ntp_list = NULL;
	cleanup_cflist(cf_nis_list);
	cf_nis_list = NULL;
	cleanup_cflist(cf_nis_name_list);
	cf_nis_name_list = NULL;
	cleanup_cflist(cf_nisp_list);
	cf_nisp_list = NULL;
	cleanup_cflist(cf_nisp_name_list);
	cf_nisp_name_list = NULL;
	cleanup_cflist(cf_bcmcs_list);
	cf_bcmcs_list = NULL;
	cleanup_cflist(cf_bcmcs_name_list);
	cf_bcmcs_name_list = NULL;
}

static void
cleanup_namelist(head)
	struct cf_namelist *head;
{
	struct cf_namelist *ifp, *ifp_next;

	for (ifp = head; ifp; ifp = ifp_next) {
		ifp_next = ifp->next;
		cleanup_cflist(ifp->params);
		free(ifp->name);
		free(ifp);
	}
}

static void
cleanup_cflist(p)
	struct cf_list *p;
{
	struct cf_list *n;

	if (p == NULL)
		return;

	n = p->next;
	if (p->type == DECL_ADDRESSPOOL) {
		free(((struct dhcp6_poolspec *)p->ptr)->name);
	}
	if (p->ptr)
		free(p->ptr);
	if (p->list)
		cleanup_cflist(p->list);
	free(p);

	cleanup_cflist(n);
}

#define config_fail() \
	do { cleanup(); configure_cleanup(); return (-1); } while(0)

int
cf_post_config()
{
	if (configure_keys(keylist_head))
		config_fail();

	if (configure_authinfo(authinfolist_head))
		config_fail();

	if (configure_ia(iapdlist_head, IATYPE_PD))
		config_fail();

	if (configure_ia(ianalist_head, IATYPE_NA))
		config_fail();

	if (configure_pool(addrpoollist_head))
		config_fail();

	if (configure_interface(iflist_head))
		config_fail();

	if (configure_host(hostlist_head))
		config_fail();

	if (configure_vendor_class(vendorclasslist_head))
		config_fail();

	if (configure_vendor(vendorlist_head))
		config_fail();

	if (configure_global_option())
		config_fail();

	configure_commit();
	cleanup();
	return (0);
}
#undef config_fail

void
cf_init()
{
	iflist_head = NULL;
}
