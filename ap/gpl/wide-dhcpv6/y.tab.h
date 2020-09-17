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
#line 133 "cfparse.y" /* yacc.c:1909  */

	long long num;
	char* str;
	struct cf_list *list;
	struct dhcp6_prefix *prefix;
	struct dhcp6_range *range;
	struct dhcp6_poolspec *pool;

#line 243 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
