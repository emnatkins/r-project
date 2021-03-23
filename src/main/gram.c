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


/*
 *  R : A Computer Language for Statistical Data Analysis
 *  Copyright (C) 1995, 1996, 1997  Robert Gentleman and Ross Ihaka
 *  Copyright (C) 1997--2020  The R Core Team
 *  Copyright (C) 2009--2011  Romain Francois
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, a copy is available at
 *  https://www.R-project.org/Licenses/
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define R_USE_SIGNALS 1
#include "IOStuff.h"		/*-> Defn.h */
#include "Fileio.h"
#include "Parse.h"
#include <R_ext/Print.h>

#if !defined(__STDC_ISO_10646__) && (defined(__APPLE__) || defined(__FreeBSD__) || defined(__sun))
/* This may not be 100% true (see the comment in rlocale.h),
   but it seems true in normal locales.
 */
# define __STDC_ISO_10646__
#endif

/* #define YYDEBUG 1 */
#define YYERROR_VERBOSE 1
#define PARSE_ERROR_SIZE 256	    /* Parse error messages saved here */
#define PARSE_CONTEXT_SIZE 256	    /* Recent parse context kept in a circular buffer */

static Rboolean busy = FALSE;
static SEXP R_NullSymbol = NULL;

static int identifier ;
static void incrementId(void);
static void initData(void);
static void initId(void);
static void record_( int, int, int, int, int, int, char* ) ;

static void yyerror(const char *);
static int yylex();
int yyparse(void);

static FILE *fp_parse;
static int (*ptr_getc)(void);

static int	SavedToken;
static SEXP	SavedLval;

#define yyconst const

typedef struct yyltype
{
  int first_line;
  int first_column;
  int first_byte;

  int last_line;
  int last_column;
  int last_byte;
  
  int first_parsed;
  int last_parsed;
  
  int id;
} yyltype;


#define INIT_DATA_COUNT 16384    	/* init parser data to this size */
#define MAX_DATA_COUNT   65536		/* release it at the end if it is this size or larger*/

#define DATA_COUNT  (length( PS_DATA ) / DATA_ROWS)
#define ID_COUNT    ((length( PS_IDS ) / 2) - 1)

static void finalizeData( ) ;
static void growData( ) ;
static void growID( int ) ;

#define DATA_ROWS 8

#define _FIRST_PARSED( i ) INTEGER( PS_DATA )[ DATA_ROWS*(i)     ]
#define _FIRST_COLUMN( i ) INTEGER( PS_DATA )[ DATA_ROWS*(i) + 1 ]
#define _LAST_PARSED( i )  INTEGER( PS_DATA )[ DATA_ROWS*(i) + 2 ]
#define _LAST_COLUMN( i )  INTEGER( PS_DATA )[ DATA_ROWS*(i) + 3 ]
#define _TERMINAL( i )     INTEGER( PS_DATA )[ DATA_ROWS*(i) + 4 ]
#define _TOKEN( i )        INTEGER( PS_DATA )[ DATA_ROWS*(i) + 5 ]
#define _ID( i )           INTEGER( PS_DATA )[ DATA_ROWS*(i) + 6 ]
#define _PARENT(i)         INTEGER( PS_DATA )[ DATA_ROWS*(i) + 7 ]

#define ID_ID( i )      INTEGER(PS_IDS)[ 2*(i) ]
#define ID_PARENT( i )  INTEGER(PS_IDS)[ 2*(i) + 1 ]

static void modif_token( yyltype*, int ) ;
static void recordParents( int, yyltype*, int) ;

static int _current_token ;

/**
 * Records the current non-terminal token expression and gives it an id
 *
 * @param loc the location of the expression
 */   
static void setId(yyltype loc){
    record_( 
	    (loc).first_parsed, (loc).first_column, (loc).last_parsed, (loc).last_column, 
	    _current_token, (loc).id, 0 ) ;
}

# define YYLTYPE yyltype
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do	{ 								\
	if (N){								\
	    (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	    (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	    (Current).first_byte   = YYRHSLOC (Rhs, 1).first_byte;	\
	    (Current).last_line    = YYRHSLOC (Rhs, N).last_line;	\
	    (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	    (Current).last_byte    = YYRHSLOC (Rhs, N).last_byte;	\
	    (Current).first_parsed = YYRHSLOC (Rhs, 1).first_parsed;    \
	    (Current).last_parsed  = YYRHSLOC (Rhs, N).last_parsed;	\
	    incrementId( ) ; 						\
	    (Current).id = identifier ; 				\
	    _current_token = yyr1[yyn] ; 				\
	    if (ParseState.keepSrcRefs && ParseState.keepParseData) {	\
	        yyltype childs[N];					\
	        int ii = 0; 						\
	        for(ii=0; ii<N; ii++){					\
		      childs[ii] = YYRHSLOC (Rhs, (ii+1) ) ; 		\
	        } 							\
	        recordParents( identifier, childs, N) ; 		\
	    }								\
	} else	{							\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_parsed   = (Current).last_parsed   =		\
	    YYRHSLOC (Rhs, 0).last_parsed;				\
	  (Current).first_column = YYRHSLOC (Rhs, 0).last_column;	\
	  (Current).last_column = (Current).first_column - 1;		\
	  (Current).first_byte = YYRHSLOC (Rhs, 0).last_byte;		\
	  (Current).last_byte = (Current).first_byte - 1;		\
	  (Current).id = NA_INTEGER;                                    \
	} 								\
    } while (0)

		
# define YY_LOCATION_PRINT(File,Loc)					\
 fprintf ( File, "%d.%d.%d-%d.%d.%d (%d)",				\
 	(Loc).first_line, (Loc).first_column,	(Loc).first_byte, 	\
 	(Loc).last_line,  (Loc).last_column, 	(Loc).last_byte, 	\
	(Loc).id )

/* Useful defines so editors don't get confused ... */

#define LBRACE	'{'
#define RBRACE	'}'

/* Functions used in the parsing process */

static void	CheckFormalArgs(SEXP, SEXP, YYLTYPE *);
static SEXP	FirstArg(SEXP, SEXP); /* create list with one element */
static void 	GrowList(SEXP, SEXP); /* add element to list end */

static void	SetSingleSrcRef(SEXP);
static void	AppendToSrcRefs(SEXP);
static void	PrependToSrcRefs(SEXP);
static SEXP	SrcRefsToVectorList();

static void	IfPush(void);
static int	KeywordLookup(const char *);
static SEXP	NewList(void);
static void	NextArg(SEXP, SEXP, SEXP); /* add named element to list end */
static SEXP	TagArg(SEXP, SEXP, YYLTYPE *);
static int 	processLineDirective();

static SEXP R_PipeBindSymbol = NULL;

/* These routines allocate constants */

static SEXP	mkComplex(const char *);
SEXP		mkFalse(void);
static SEXP     mkFloat(const char *);
static SEXP 	mkInt(const char *); 
static SEXP	mkNA(void);
SEXP		mkTrue(void);

/* Internal lexer / parser state variables */

static int	EatLines = 0;
static int	GenerateCode = 0;
static int	EndOfFile = 0;
static int	xxgetc();
static int	xxungetc(int);
static int	xxcharcount, xxcharsave;
static int	xxlinesave, xxbytesave, xxcolsave, xxparsesave;

static SrcRefState ParseState;

#define PS_SET_SRCREFS(x)   SET_VECTOR_ELT(ParseState.sexps, 0, (x))
#define PS_SET_SRCFILE(x)   SET_VECTOR_ELT(ParseState.sexps, 1, (x))
#define PS_SET_ORIGINAL(x)  SET_VECTOR_ELT(ParseState.sexps, 2, (x))

/* direct pointer to data is kept for performance of finalizeData() */
#define PS_SET_DATA(x)      do {                \
    SEXP __x__ = (x);                           \
    SET_VECTOR_ELT(ParseState.sexps, 3, __x__); \
    ParseState.data = __x__;                    \
} while(0);

#define PS_SET_TEXT(x)      SET_VECTOR_ELT(ParseState.sexps, 4, (x))
#define PS_SET_IDS(x)       SET_VECTOR_ELT(ParseState.sexps, 5, (x))
#define PS_SET_SVS(x)       SET_VECTOR_ELT(ParseState.sexps, 6, (x))

#define PS_SRCREFS          VECTOR_ELT(ParseState.sexps, 0)
#define PS_SRCFILE          VECTOR_ELT(ParseState.sexps, 1)
#define PS_ORIGINAL         VECTOR_ELT(ParseState.sexps, 2)
#define PS_DATA             ParseState.data
#define PS_TEXT             VECTOR_ELT(ParseState.sexps, 4)
#define PS_IDS              VECTOR_ELT(ParseState.sexps, 5)
#define PS_SVS              VECTOR_ELT(ParseState.sexps, 6)

/* Memory protection in the parser

   The generated code of the parser keeps semantic values (SEXPs) on its
   semantic values stack. Values are added to the stack during shift and
   reduce operations and are removed during reduce operations or error
   handling. Values are created by the lexer before they are added to the
   stack. Values are also held in a local SEXP variable once removed from
   the stack but still needed. The stack is automatically expanded on demand.

   For memory protection, it would be natural to have that stack on the R heap
   and to use PROTECT/UNPROTECT to protect values in local SEXP variables.
   Unfortunately, bison does not seem to be customizable enough to allow this.

   Hence, semantic values, when created by the lexer or reduce operations, are
   placed on parser state precious multi-set via PRESERVE_SV. They are removed
   from the multi-set in reduce operations using RELEASE_SV, because by design
   of the bison parsers such values are subsequently removed from the stack.
   They are also automatically removed when the parsing finishes, including
   parser error (also on R error, via the context on-end action).

   Previously semantic values were protected via PROTECT/UNPROTECT_PTR with
   similar semantics but using protect stack shared with PROTECT/UNPROTECT.
   Using a separate precious multi-set is safe even with interleaving of the
   two protection schemes.
*/

#define INIT_SVS()     PS_SET_SVS(R_NewPreciousMSet(200))
#define PRESERVE_SV(x) R_PreserveInMSet((x), PS_SVS)
#define RELEASE_SV(x)  R_ReleaseFromMSet((x), PS_SVS)
#define CLEAR_SVS()    R_ReleaseMSet(PS_SVS, 500)

/* Memory leak

   yyparse(), as generated by bison, allocates extra space for the parser
   stack using malloc(). Unfortunately this means that there is a memory
   leak in case of an R error (long-jump). In principle, we could define
   yyoverflow() to relocate the parser stacks for bison and allocate say on
   the R heap, but yyoverflow() is undocumented and somewhat complicated
   (we would have to replicate some macros from the generated parser here).
   The same problem exists at least in the Rd and LaTeX parsers in tools.
*/

#include <rlocale.h>
#ifdef HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

static int mbcs_get_next(int c, wchar_t *wc)
{
    int i, res, clen = 1; char s[9];
    mbstate_t mb_st;

    s[0] = (char) c;
    /* This assumes (probably OK) that all MBCS embed ASCII as single-byte
       lead bytes, including control chars */
    if((unsigned int) c < 0x80) {
	*wc = (wchar_t) c;
	return 1;
    }
    if(utf8locale) {
	clen = utf8clen((char) c);
	for(i = 1; i < clen; i++) {
	    c = xxgetc();
	    if(c == R_EOF) error(_("EOF whilst reading MBCS char at line %d"), ParseState.xxlineno);
	    s[i] = (char) c;
	}
	s[clen] ='\0'; /* x86 Solaris requires this */
	res = (int) mbrtowc(wc, s, clen, NULL);
	if(res == -1) error(_("invalid multibyte character in parser at line %d"), ParseState.xxlineno);
    } else {
	/* This is not necessarily correct for stateful MBCS */
	while(clen <= MB_CUR_MAX) {
	    mbs_init(&mb_st);
	    res = (int) mbrtowc(wc, s, clen, &mb_st);
	    if(res >= 0) break;
	    if(res == -1)
		error(_("invalid multibyte character in parser at line %d"), ParseState.xxlineno);
	    /* so res == -2 */
	    c = xxgetc();
	    if(c == R_EOF) error(_("EOF whilst reading MBCS char at line %d"), ParseState.xxlineno);
	    s[clen++] = (char) c;
	} /* we've tried enough, so must be complete or invalid by now */
    }
    for(i = clen - 1; i > 0; i--) xxungetc(s[i]);
    return clen;
}

/* Soon to be defunct entry points */

void		R_SetInput(int);
int		R_fgetc(FILE*);

/* Routines used to build the parse tree */

static SEXP	xxnullformal(void);
static SEXP	xxfirstformal0(SEXP);
static SEXP	xxfirstformal1(SEXP, SEXP);
static SEXP	xxaddformal0(SEXP, SEXP, YYLTYPE *);
static SEXP	xxaddformal1(SEXP, SEXP, SEXP, YYLTYPE *);
static SEXP	xxexprlist0();
static SEXP	xxexprlist1(SEXP, YYLTYPE *);
static SEXP	xxexprlist2(SEXP, SEXP, YYLTYPE *);
static SEXP	xxsub0(void);
static SEXP	xxsub1(SEXP, YYLTYPE *);
static SEXP	xxsymsub0(SEXP, YYLTYPE *);
static SEXP	xxsymsub1(SEXP, SEXP, YYLTYPE *);
static SEXP	xxnullsub0(YYLTYPE *);
static SEXP	xxnullsub1(SEXP, YYLTYPE *);
static SEXP	xxsublist1(SEXP);
static SEXP	xxsublist2(SEXP, SEXP);
static SEXP	xxcond(SEXP);
static SEXP	xxifcond(SEXP);
static SEXP	xxif(SEXP, SEXP, SEXP);
static SEXP	xxifelse(SEXP, SEXP, SEXP, SEXP);
static SEXP	xxforcond(SEXP, SEXP);
static SEXP	xxfor(SEXP, SEXP, SEXP);
static SEXP	xxwhile(SEXP, SEXP, SEXP);
static SEXP	xxrepeat(SEXP, SEXP);
static SEXP	xxnxtbrk(SEXP);
static SEXP	xxfuncall(SEXP, SEXP);
static SEXP	xxdefun(SEXP, SEXP, SEXP, YYLTYPE *);
static SEXP	xxpipe(SEXP, SEXP);
static SEXP	xxpipebind(SEXP, SEXP, SEXP);
static SEXP	xxunary(SEXP, SEXP);
static SEXP	xxbinary(SEXP, SEXP, SEXP);
static SEXP	xxparen(SEXP, SEXP);
static SEXP	xxsubscript(SEXP, SEXP, SEXP);
static SEXP	xxexprlist(SEXP, YYLTYPE *, SEXP);
static int	xxvalue(SEXP, int, YYLTYPE *);

#define YYSTYPE		SEXP




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
    END_OF_INPUT = 258,
    ERROR = 259,
    STR_CONST = 260,
    NUM_CONST = 261,
    NULL_CONST = 262,
    SYMBOL = 263,
    FUNCTION = 264,
    INCOMPLETE_STRING = 265,
    LEFT_ASSIGN = 266,
    EQ_ASSIGN = 267,
    RIGHT_ASSIGN = 268,
    LBB = 269,
    FOR = 270,
    IN = 271,
    IF = 272,
    ELSE = 273,
    WHILE = 274,
    NEXT = 275,
    BREAK = 276,
    REPEAT = 277,
    GT = 278,
    GE = 279,
    LT = 280,
    LE = 281,
    EQ = 282,
    NE = 283,
    AND = 284,
    OR = 285,
    AND2 = 286,
    OR2 = 287,
    NS_GET = 288,
    NS_GET_INT = 289,
    COMMENT = 290,
    LINE_DIRECTIVE = 291,
    SYMBOL_FORMALS = 292,
    EQ_FORMALS = 293,
    EQ_SUB = 294,
    SYMBOL_SUB = 295,
    SYMBOL_FUNCTION_CALL = 296,
    SYMBOL_PACKAGE = 297,
    SLOT = 298,
    PIPE = 299,
    PIPEBIND = 300,
    LOW = 301,
    TILDE = 302,
    UNOT = 303,
    NOT = 304,
    SPECIAL = 305,
    UMINUS = 306,
    UPLUS = 307
  };
#endif
/* Tokens.  */
#define END_OF_INPUT 258
#define ERROR 259
#define STR_CONST 260
#define NUM_CONST 261
#define NULL_CONST 262
#define SYMBOL 263
#define FUNCTION 264
#define INCOMPLETE_STRING 265
#define LEFT_ASSIGN 266
#define EQ_ASSIGN 267
#define RIGHT_ASSIGN 268
#define LBB 269
#define FOR 270
#define IN 271
#define IF 272
#define ELSE 273
#define WHILE 274
#define NEXT 275
#define BREAK 276
#define REPEAT 277
#define GT 278
#define GE 279
#define LT 280
#define LE 281
#define EQ 282
#define NE 283
#define AND 284
#define OR 285
#define AND2 286
#define OR2 287
#define NS_GET 288
#define NS_GET_INT 289
#define COMMENT 290
#define LINE_DIRECTIVE 291
#define SYMBOL_FORMALS 292
#define EQ_FORMALS 293
#define EQ_SUB 294
#define SYMBOL_SUB 295
#define SYMBOL_FUNCTION_CALL 296
#define SYMBOL_PACKAGE 297
#define SLOT 298
#define PIPE 299
#define PIPEBIND 300
#define LOW 301
#define TILDE 302
#define UNOT 303
#define NOT 304
#define SPECIAL 305
#define UMINUS 306
#define UPLUS 307

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);



/* Copy the second part of user declarations.  */



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
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  47
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   550

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  74
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  13
/* YYNRULES -- Number of rules.  */
#define YYNRULES  93
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  173

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   307

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      65,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    70,     2,     2,    61,     2,     2,     2,
      63,    69,    54,    52,    73,    53,     2,    55,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    57,    66,
       2,     2,     2,    46,    62,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    64,    71,    72,    60,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    67,     2,    68,    48,     2,     2,     2,
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
      45,    47,    49,    50,    51,    56,    58,    59
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   415,   415,   416,   417,   418,   419,   422,   423,   424,
     427,   428,   431,   432,   433,   434,   436,   437,   439,   440,
     441,   442,   443,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,   469,   470,   471,   472,
     473,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   490,   491,   495,
     498,   501,   505,   506,   507,   508,   509,   510,   513,   514,
     517,   518,   519,   520,   521,   522,   523,   524,   527,   528,
     529,   530,   531,   535
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "END_OF_INPUT", "ERROR", "STR_CONST",
  "NUM_CONST", "NULL_CONST", "SYMBOL", "FUNCTION", "INCOMPLETE_STRING",
  "LEFT_ASSIGN", "EQ_ASSIGN", "RIGHT_ASSIGN", "LBB", "FOR", "IN", "IF",
  "ELSE", "WHILE", "NEXT", "BREAK", "REPEAT", "GT", "GE", "LT", "LE", "EQ",
  "NE", "AND", "OR", "AND2", "OR2", "NS_GET", "NS_GET_INT", "COMMENT",
  "LINE_DIRECTIVE", "SYMBOL_FORMALS", "EQ_FORMALS", "EQ_SUB", "SYMBOL_SUB",
  "SYMBOL_FUNCTION_CALL", "SYMBOL_PACKAGE", "SLOT", "PIPE", "PIPEBIND",
  "'?'", "LOW", "'~'", "TILDE", "UNOT", "NOT", "'+'", "'-'", "'*'", "'/'",
  "SPECIAL", "':'", "UMINUS", "UPLUS", "'^'", "'$'", "'@'", "'('", "'['",
  "'\\n'", "';'", "'{'", "'}'", "')'", "'!'", "'\\\\'", "']'", "','",
  "$accept", "prog", "expr_or_assign_or_help", "expr_or_help", "expr",
  "cond", "ifcond", "forcond", "exprlist", "sublist", "sub", "formlist",
  "cr", YY_NULLPTR
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
     295,   296,   297,   298,   299,   300,    63,   301,   126,   302,
     303,   304,    43,    45,    42,    47,   305,    58,   306,   307,
      94,    36,    64,    40,    91,    10,    59,   123,   125,    41,
      33,    92,    93,    44
};
# endif

#define YYPACT_NINF -127

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-127)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     149,  -127,  -127,    55,  -127,  -127,    80,   -47,   -41,   -39,
     -37,  -127,  -127,   216,   216,   216,   216,   216,   216,  -127,
     216,   216,   -29,    36,    20,   287,     6,    89,    93,    94,
      68,    74,   216,   216,   216,   216,   216,    72,    72,   375,
     123,   123,    18,    23,    12,   442,    68,  -127,   216,   216,
    -127,  -127,   216,   216,   236,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,    95,   100,   236,   236,  -127,
    -127,  -127,  -127,  -127,  -127,  -127,  -127,    78,   -56,    88,
      72,   -42,   287,    -3,   -38,    72,  -127,   216,   216,  -127,
     -50,    72,    72,   287,   331,    -5,    97,    58,    60,    40,
    -127,   486,   486,   486,   486,   486,   486,   442,   419,   442,
     419,    79,    11,   375,   118,   118,   264,   264,    79,   123,
     123,  -127,  -127,  -127,  -127,    48,    49,   216,  -127,   115,
     216,   216,  -127,   216,  -127,    23,    23,  -127,   216,   216,
     216,    56,    54,  -127,  -127,    60,   216,   117,   -36,  -127,
      72,   216,    60,    60,    60,  -127,   236,    72,   216,  -127,
      72,  -127,    60
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     6,     2,    13,    12,    14,    15,     0,     0,     0,
       0,    67,    68,     0,     0,     0,     0,     0,     0,     3,
      72,     0,     0,     0,     0,     7,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,    52,    22,    21,
      19,    18,     0,    73,     0,    20,    88,     1,     0,     0,
       4,     5,     0,     0,    80,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    80,    80,    58,
      57,    62,    61,    56,    55,    60,    59,    89,     0,     0,
      50,     0,    10,    48,     0,    51,    17,    77,    75,    16,
       0,     8,     9,    43,    44,    13,    14,    15,    81,    93,
      78,    36,    35,    31,    32,    33,    34,    37,    38,    39,
      40,    41,    42,    30,    24,    25,    26,    27,    29,    23,
      28,    64,    63,    66,    65,    93,    93,     0,    93,     0,
       0,     0,    70,     0,    69,    76,    74,    93,    84,    86,
      82,     0,     0,    47,    54,    90,     0,    91,     0,    11,
      49,     0,    85,    87,    83,    53,    80,    45,     0,    71,
      46,    79,    92
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -127,  -127,    47,   -30,   -15,  -127,  -127,  -127,  -127,    38,
     -35,    84,  -126
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    23,    24,   108,    25,    36,    34,    32,    44,   109,
     110,    88,   152
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      39,    40,    41,    91,   141,    94,    45,   148,   141,    48,
     141,    79,   156,   138,    80,   143,    30,   139,    92,   147,
      92,   161,    31,   139,    33,    54,    35,   142,    26,    27,
      48,   144,    48,   169,    46,    48,    47,   103,   104,    92,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      37,    38,    92,    92,    49,    42,    49,    43,    73,    49,
     150,    74,    75,    76,    77,    78,    87,    97,    98,    90,
      99,    93,    89,    95,    48,    50,    51,    96,    26,    27,
     137,    28,    29,    54,    81,   101,   102,    82,    83,    85,
     131,    84,    86,   132,   140,   133,   141,   155,   134,   149,
     158,   159,   151,    28,    29,   135,   136,   153,   162,   163,
     164,   154,    92,   157,    66,    92,    92,   166,   165,   168,
     100,   171,    54,    92,    92,    92,    73,    54,   172,    74,
      75,    76,    77,    78,   145,   146,     0,     0,     0,     0,
       1,    92,     2,    92,     3,     4,     5,     6,     7,     0,
       0,     0,    65,    66,     8,     0,     9,     0,    10,    11,
      12,    13,    70,    71,    72,    73,     0,     0,    74,    75,
      76,    77,    78,    74,    75,    76,    77,    78,     0,     0,
     160,     0,     0,     0,     0,    14,     0,    15,     0,     0,
       0,    16,    17,   167,     0,     0,     0,     0,   170,     0,
       0,     0,    18,     0,    19,     0,    20,     0,     0,    21,
      22,     3,     4,     5,     6,     7,     0,     0,     0,     0,
       0,     8,     0,     9,     0,    10,    11,    12,    13,     0,
       0,   105,     4,   106,   107,     7,     0,     0,     0,     0,
       0,     8,     0,     9,     0,    10,    11,    12,    13,     0,
       0,     0,    14,     0,    15,     0,     0,     0,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    18,
       0,     0,    14,    20,    15,     0,    21,    22,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,    52,    18,
      53,    54,     0,    20,     0,     0,    21,    22,    65,    66,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      72,    73,     0,     0,    74,    75,    76,    77,    78,     0,
       0,    65,    66,     0,     0,    67,     0,     0,     0,    68,
      69,    70,    71,    72,    73,    54,     0,    74,    75,    76,
      77,    78,     0,     0,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,     0,     0,    67,
       0,     0,     0,    68,    69,    70,    71,    72,    73,    54,
       0,    74,    75,    76,    77,    78,     0,     0,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,    73,    54,     0,    74,    75,    76,    77,    78,
       0,     0,    55,    56,    57,    58,    59,    60,    61,     0,
      63,     0,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,    65,    66,    55,    56,    57,    58,    59,
      60,    68,    69,    70,    71,    72,    73,     0,     0,    74,
      75,    76,    77,    78,     0,     0,    65,    66,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,    73,
      54,     0,    74,    75,    76,    77,    78,     0,     0,    -1,
      -1,    -1,    -1,    -1,    -1,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,    73,     0,     0,    74,    75,    76,    77,
      78
};

static const yytype_int16 yycheck[] =
{
      15,    16,    17,    33,    46,    35,    21,    12,    46,    12,
      46,     5,   138,    69,     8,    18,    63,    73,    33,    69,
      35,   147,    63,    73,    63,    14,    63,    69,    33,    34,
      12,    69,    12,    69,    63,    12,     0,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      13,    14,    77,    78,    46,    18,    46,    20,    57,    46,
      12,    60,    61,    62,    63,    64,     8,    65,    66,    32,
      68,    34,     8,    36,    12,    65,    66,    69,    33,    34,
      12,    33,    34,    14,     5,    48,    49,     8,     5,     5,
       5,     8,     8,     8,    16,     5,    46,   137,     8,    12,
     140,   141,    72,    33,    34,    77,    78,    69,   148,   149,
     150,    72,   137,     8,    45,   140,   141,    73,    72,    12,
      46,   166,    14,   148,   149,   150,    57,    14,   168,    60,
      61,    62,    63,    64,    97,    98,    -1,    -1,    -1,    -1,
       1,   166,     3,   168,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    44,    45,    15,    -1,    17,    -1,    19,    20,
      21,    22,    54,    55,    56,    57,    -1,    -1,    60,    61,
      62,    63,    64,    60,    61,    62,    63,    64,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    46,    -1,    48,    -1,    -1,
      -1,    52,    53,   156,    -1,    -1,    -1,    -1,   161,    -1,
      -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,    70,
      71,     5,     6,     7,     8,     9,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    17,    -1,    19,    20,    21,    22,    -1,
      -1,     5,     6,     7,     8,     9,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    17,    -1,    19,    20,    21,    22,    -1,
      -1,    -1,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    63,
      -1,    -1,    46,    67,    48,    -1,    70,    71,    52,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,    63,
      13,    14,    -1,    67,    -1,    -1,    70,    71,    44,    45,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      56,    57,    -1,    -1,    60,    61,    62,    63,    64,    -1,
      -1,    44,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      53,    54,    55,    56,    57,    14,    -1,    60,    61,    62,
      63,    64,    -1,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    53,    54,    55,    56,    57,    14,
      -1,    60,    61,    62,    63,    64,    -1,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    -1,    -1,    -1,    52,    53,    54,
      55,    56,    57,    14,    -1,    60,    61,    62,    63,    64,
      -1,    -1,    23,    24,    25,    26,    27,    28,    29,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    44,    45,    23,    24,    25,    26,    27,
      28,    52,    53,    54,    55,    56,    57,    -1,    -1,    60,
      61,    62,    63,    64,    -1,    -1,    44,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    52,    53,    54,    55,    56,    57,
      14,    -1,    60,    61,    62,    63,    64,    -1,    -1,    23,
      24,    25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    -1,    -1,    -1,    52,    53,
      54,    55,    56,    57,    -1,    -1,    60,    61,    62,    63,
      64
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     5,     6,     7,     8,     9,    15,    17,
      19,    20,    21,    22,    46,    48,    52,    53,    63,    65,
      67,    70,    71,    75,    76,    78,    33,    34,    33,    34,
      63,    63,    81,    63,    80,    63,    79,    76,    76,    78,
      78,    78,    76,    76,    82,    78,    63,     0,    12,    46,
      65,    66,    11,    13,    14,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    44,    45,    48,    52,    53,
      54,    55,    56,    57,    60,    61,    62,    63,    64,     5,
       8,     5,     8,     5,     8,     5,     8,     8,    85,     8,
      76,    77,    78,    76,    77,    76,    69,    65,    66,    68,
      85,    76,    76,    78,    78,     5,     7,     8,    77,    83,
      84,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,     5,     8,     5,     8,    83,    83,    12,    69,    73,
      16,    46,    69,    18,    69,    76,    76,    69,    12,    12,
      12,    72,    86,    69,    72,    77,    86,     8,    77,    77,
      76,    86,    77,    77,    77,    72,    73,    76,    12,    69,
      76,    84,    77
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    74,    75,    75,    75,    75,    75,    76,    76,    76,
      77,    77,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    79,
      80,    81,    82,    82,    82,    82,    82,    82,    83,    83,
      84,    84,    84,    84,    84,    84,    84,    84,    85,    85,
      85,    85,    85,    86
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     1,     1,     3,     3,
       1,     3,     1,     1,     1,     1,     3,     3,     2,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     6,     6,     4,     3,     5,
       3,     3,     2,     5,     4,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     3,
       3,     5,     0,     1,     3,     2,     3,     2,     1,     4,
       0,     1,     2,     3,     2,     3,     2,     3,     0,     1,
       3,     3,     5,     0
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


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
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
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
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
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
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
       'yyls': related to locations.

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

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
  *++yylsp = yylloc;
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

    { YYACCEPT; }

    break;

  case 3:

    { yyresult = xxvalue(NULL,2,NULL);	goto yyreturn; }

    break;

  case 4:

    { yyresult = xxvalue((yyvsp[-1]),3,&(yylsp[-1]));	goto yyreturn; }

    break;

  case 5:

    { yyresult = xxvalue((yyvsp[-1]),4,&(yylsp[-1]));	goto yyreturn; }

    break;

  case 6:

    { YYABORT; }

    break;

  case 7:

    { (yyval) = (yyvsp[0]); }

    break;

  case 8:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0])); setId((yyloc)); }

    break;

  case 9:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0])); setId((yyloc)); }

    break;

  case 10:

    { (yyval) = (yyvsp[0]); }

    break;

  case 11:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0])); setId((yyloc)); }

    break;

  case 12:

    { (yyval) = (yyvsp[0]);	setId((yyloc)); }

    break;

  case 13:

    { (yyval) = (yyvsp[0]);	setId((yyloc)); }

    break;

  case 14:

    { (yyval) = (yyvsp[0]);	setId((yyloc)); }

    break;

  case 15:

    { (yyval) = (yyvsp[0]);	setId((yyloc)); }

    break;

  case 16:

    { (yyval) = xxexprlist((yyvsp[-2]),&(yylsp[-2]),(yyvsp[-1])); setId((yyloc)); }

    break;

  case 17:

    { (yyval) = xxparen((yyvsp[-2]),(yyvsp[-1]));	setId((yyloc)); }

    break;

  case 18:

    { (yyval) = xxunary((yyvsp[-1]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 19:

    { (yyval) = xxunary((yyvsp[-1]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 20:

    { (yyval) = xxunary((yyvsp[-1]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 21:

    { (yyval) = xxunary((yyvsp[-1]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 22:

    { (yyval) = xxunary((yyvsp[-1]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 23:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 24:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 25:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 26:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 27:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 28:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 29:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 30:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 31:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 32:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 33:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 34:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 35:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 36:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 37:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 38:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 39:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 40:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 41:

    { (yyval) = xxpipe((yyvsp[-2]),(yyvsp[0]));  setId((yyloc)); }

    break;

  case 42:

    { (yyval) = xxpipebind((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 43:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 44:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[0]),(yyvsp[-2]));	setId((yyloc)); }

    break;

  case 45:

    { (yyval) = xxdefun((yyvsp[-5]),(yyvsp[-3]),(yyvsp[0]),&(yyloc)); 	setId((yyloc)); }

    break;

  case 46:

    { (yyval) = xxdefun(R_FunctionSymbol,(yyvsp[-3]),(yyvsp[0]),&(yyloc)); 	setId((yyloc)); }

    break;

  case 47:

    { (yyval) = xxfuncall((yyvsp[-3]),(yyvsp[-1]));  setId((yyloc)); modif_token( &(yylsp[-3]), SYMBOL_FUNCTION_CALL ) ; }

    break;

  case 48:

    { (yyval) = xxif((yyvsp[-2]),(yyvsp[-1]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 49:

    { (yyval) = xxifelse((yyvsp[-4]),(yyvsp[-3]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 50:

    { (yyval) = xxfor((yyvsp[-2]),(yyvsp[-1]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 51:

    { (yyval) = xxwhile((yyvsp[-2]),(yyvsp[-1]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 52:

    { (yyval) = xxrepeat((yyvsp[-1]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 53:

    { (yyval) = xxsubscript((yyvsp[-4]),(yyvsp[-3]),(yyvsp[-2]));	setId((yyloc)); }

    break;

  case 54:

    { (yyval) = xxsubscript((yyvsp[-3]),(yyvsp[-2]),(yyvsp[-1]));	setId((yyloc)); }

    break;

  case 55:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));      setId((yyloc)); modif_token( &(yylsp[-2]), SYMBOL_PACKAGE ) ; }

    break;

  case 56:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));      setId((yyloc)); modif_token( &(yylsp[-2]), SYMBOL_PACKAGE ) ; }

    break;

  case 57:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 58:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 59:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));      setId((yyloc)); modif_token( &(yylsp[-2]), SYMBOL_PACKAGE ) ;}

    break;

  case 60:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));      setId((yyloc)); modif_token( &(yylsp[-2]), SYMBOL_PACKAGE ) ;}

    break;

  case 61:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 62:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 63:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 64:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 65:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));      setId((yyloc)); modif_token( &(yylsp[0]), SLOT ) ; }

    break;

  case 66:

    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId((yyloc)); }

    break;

  case 67:

    { (yyval) = xxnxtbrk((yyvsp[0]));	setId((yyloc)); }

    break;

  case 68:

    { (yyval) = xxnxtbrk((yyvsp[0]));	setId((yyloc)); }

    break;

  case 69:

    { (yyval) = xxcond((yyvsp[-1]));   }

    break;

  case 70:

    { (yyval) = xxifcond((yyvsp[-1])); }

    break;

  case 71:

    { (yyval) = xxforcond((yyvsp[-3]),(yyvsp[-1]));	setId((yyloc)); }

    break;

  case 72:

    { (yyval) = xxexprlist0();	setId((yyloc)); }

    break;

  case 73:

    { (yyval) = xxexprlist1((yyvsp[0]), &(yylsp[0])); }

    break;

  case 74:

    { (yyval) = xxexprlist2((yyvsp[-2]), (yyvsp[0]), &(yylsp[0])); }

    break;

  case 75:

    { (yyval) = (yyvsp[-1]);		setId((yyloc)); }

    break;

  case 76:

    { (yyval) = xxexprlist2((yyvsp[-2]), (yyvsp[0]), &(yylsp[0])); }

    break;

  case 77:

    { (yyval) = (yyvsp[-1]);}

    break;

  case 78:

    { (yyval) = xxsublist1((yyvsp[0]));	  }

    break;

  case 79:

    { (yyval) = xxsublist2((yyvsp[-3]),(yyvsp[0])); }

    break;

  case 80:

    { (yyval) = xxsub0();	 }

    break;

  case 81:

    { (yyval) = xxsub1((yyvsp[0]), &(yylsp[0]));  }

    break;

  case 82:

    { (yyval) = xxsymsub0((yyvsp[-1]), &(yylsp[-1])); 	modif_token( &(yylsp[0]), EQ_SUB ) ; modif_token( &(yylsp[-1]), SYMBOL_SUB ) ; }

    break;

  case 83:

    { (yyval) = xxsymsub1((yyvsp[-2]),(yyvsp[0]), &(yylsp[-2])); 	modif_token( &(yylsp[-1]), EQ_SUB ) ; modif_token( &(yylsp[-2]), SYMBOL_SUB ) ; }

    break;

  case 84:

    { (yyval) = xxsymsub0((yyvsp[-1]), &(yylsp[-1])); 	modif_token( &(yylsp[0]), EQ_SUB ) ; }

    break;

  case 85:

    { (yyval) = xxsymsub1((yyvsp[-2]),(yyvsp[0]), &(yylsp[-2])); 	modif_token( &(yylsp[-1]), EQ_SUB ) ; }

    break;

  case 86:

    { (yyval) = xxnullsub0(&(yylsp[-1])); 	modif_token( &(yylsp[0]), EQ_SUB ) ; }

    break;

  case 87:

    { (yyval) = xxnullsub1((yyvsp[0]), &(yylsp[-2])); 	modif_token( &(yylsp[-1]), EQ_SUB ) ; }

    break;

  case 88:

    { (yyval) = xxnullformal(); }

    break;

  case 89:

    { (yyval) = xxfirstformal0((yyvsp[0])); 	modif_token( &(yylsp[0]), SYMBOL_FORMALS ) ; }

    break;

  case 90:

    { (yyval) = xxfirstformal1((yyvsp[-2]),(yyvsp[0])); 	modif_token( &(yylsp[-2]), SYMBOL_FORMALS ) ; modif_token( &(yylsp[-1]), EQ_FORMALS ) ; }

    break;

  case 91:

    { (yyval) = xxaddformal0((yyvsp[-2]),(yyvsp[0]), &(yylsp[0]));   modif_token( &(yylsp[0]), SYMBOL_FORMALS ) ; }

    break;

  case 92:

    { (yyval) = xxaddformal1((yyvsp[-4]),(yyvsp[-2]),(yyvsp[0]),&(yylsp[-2])); modif_token( &(yylsp[-2]), SYMBOL_FORMALS ) ; modif_token( &(yylsp[-1]), EQ_FORMALS ) ;}

    break;

  case 93:

    { EatLines = 1; }

    break;



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
  *++yylsp = yyloc;

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

  yyerror_range[1] = yylloc;

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
                      yytoken, &yylval, &yylloc);
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

  yyerror_range[1] = yylsp[1-yylen];
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp);
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




/*----------------------------------------------------------------------------*/

static int (*ptr_getc)(void);

/* Private pushback, since file ungetc only guarantees one byte.
   We need up to one MBCS-worth */
#define DECLARE_YYTEXT_BUFP(bp) char *bp = yytext ;
#define YYTEXT_PUSH(c, bp) do { \
    if ((bp) - yytext >= sizeof(yytext) - 1){ \
		error(_("input buffer overflow at line %d"), ParseState.xxlineno); \
	} \
    *(bp)++ = ((char)c);			\
} while(0) ;

#define PUSHBACK_BUFSIZE 16
static int pushback[PUSHBACK_BUFSIZE];
static unsigned int npush = 0;

static int prevpos = 0;
static int prevlines[PUSHBACK_BUFSIZE];
static int prevcols[PUSHBACK_BUFSIZE];
static int prevbytes[PUSHBACK_BUFSIZE];
static int prevparse[PUSHBACK_BUFSIZE];

static int xxgetc(void)
{
    int c;

    if(npush) c = pushback[--npush]; else  c = ptr_getc();

    prevpos = (prevpos + 1) % PUSHBACK_BUFSIZE;
    prevbytes[prevpos] = ParseState.xxbyteno;
    prevlines[prevpos] = ParseState.xxlineno;  
    prevparse[prevpos] = ParseState.xxparseno;
    prevcols[prevpos] = ParseState.xxcolno;
    	
    if (c == EOF) {
	EndOfFile = 1;
	return R_EOF;
    }
    R_ParseContextLast = (R_ParseContextLast + 1) % PARSE_CONTEXT_SIZE;
    R_ParseContext[R_ParseContextLast] = (char) c;

    if (c == '\n') {
	ParseState.xxlineno += 1;
	ParseState.xxcolno = 0;
    	ParseState.xxbyteno = 0;
    	ParseState.xxparseno += 1;
    } else {
        /* We only advance the column for the 1st byte in UTF-8, so handle later bytes specially */
	if (!known_to_be_utf8 || (unsigned char)c < 0x80 || 0xC0 <= (unsigned char)c)
            ParseState.xxcolno++;
    	ParseState.xxbyteno++;
    }

    if (c == '\t') ParseState.xxcolno = ((ParseState.xxcolno + 7) & ~7);
    
    R_ParseContextLine = ParseState.xxlineno;    

    xxcharcount++;
    return c;
}

static int xxungetc(int c)
{
    /* this assumes that c was the result of xxgetc; if not, some edits will be needed */
    ParseState.xxlineno = prevlines[prevpos];
    ParseState.xxbyteno = prevbytes[prevpos];
    ParseState.xxcolno  = prevcols[prevpos];
    ParseState.xxparseno = prevparse[prevpos];
    
    prevpos = (prevpos + PUSHBACK_BUFSIZE - 1) % PUSHBACK_BUFSIZE;

    R_ParseContextLine = ParseState.xxlineno;

    xxcharcount--;
    R_ParseContext[R_ParseContextLast] = '\0';
    /* precaution as to how % is implemented for < 0 numbers */
    R_ParseContextLast = (R_ParseContextLast + PARSE_CONTEXT_SIZE -1) % PARSE_CONTEXT_SIZE;
    if(npush >= PUSHBACK_BUFSIZE) return EOF;
    pushback[npush++] = c;
    return c;
}

/* Only used from finish_mbcs_in_parse_context. */
static int add_mbcs_byte_to_parse_context()
{
    int c;

    if (EndOfFile)
	error(_("invalid multibyte character in parser at line %d"),
	      ParseState.xxlineno);
    if(npush)
	c = pushback[--npush];
    else
	c = ptr_getc();
    if (c == EOF) 
	error(_("invalid multibyte character in parser at line %d"),
	      ParseState.xxlineno);
    
    R_ParseContextLast = (R_ParseContextLast + 1) % PARSE_CONTEXT_SIZE;
    R_ParseContext[R_ParseContextLast] = (char) c;
    return c;
}

/* On error, the parse context may end inside a multi-byte character. Add
   the missing bytes to the context to so that it contains full characters. */
static void finish_mbcs_in_parse_context()
{
    int i, c, nbytes = 0, first;
    Rboolean mbcs = FALSE;

    /* find the first byte of the context */
    for(i = R_ParseContextLast;
        R_ParseContext[i];
        i = (i + PARSE_CONTEXT_SIZE - 1) % PARSE_CONTEXT_SIZE) {

	nbytes++;
	if (nbytes == PARSE_CONTEXT_SIZE)
	    break;
    }
    if (!nbytes)
	return;
    if (!R_ParseContext[i])
	first = (i + 1) % PARSE_CONTEXT_SIZE;
    else
	/* The beginning of the context has been overwritten and for a general
	   encoding there is not way to recover it. It is possible for UTF-8,
	   though. */
	return;

    /* decode multi-byte characters */
    for(i = 0; i < nbytes; i++) {
	c = R_ParseContext[(first + i) % PARSE_CONTEXT_SIZE];
	if ((unsigned int)c < 0x80) continue; /* ASCII */

	if (utf8locale) {
	    /* UTF-8 could be handled more efficiently, searching from the end
	       of the string */
	    i += utf8clen((char) c) - 1;
	    if (i >= nbytes) {
		while (i >= nbytes) {
		    add_mbcs_byte_to_parse_context();
		    nbytes++;
		}
		return;
	    }
	} else
	    mbcs = TRUE;
    }
    if (!mbcs)
	return;

    /* copy the context to a linear buffer */
    char buf[nbytes + MB_CUR_MAX];

    for(i = 0; i < nbytes; i++)
	buf[i] = R_ParseContext[(first + i) % PARSE_CONTEXT_SIZE];

    for(i = 0; i < nbytes; i++) {
	wchar_t wc;
	int res;
	mbstate_t mb_st;
	
	mbs_init(&mb_st);
	res = (int) mbrtowc(&wc, buf + i, nbytes - i, &mb_st);
	while (res == -2 && nbytes < sizeof(buf)) {
	    /* This is not necessarily correct for stateful MBCS */
	    buf[nbytes++] = (char) add_mbcs_byte_to_parse_context();
	    mbs_init(&mb_st);
	    res = (int) mbrtowc(&wc, buf + i, nbytes - i, &mb_st);
	}	   
	if (res == -1)
	    error(_("invalid multibyte character in parser at line %d"),
		  ParseState.xxlineno);
	i += res - 1;
    }
}

/*
 * Increments/inits the token/grouping counter
 */
static void incrementId(void){
	identifier++; 
}

static void initId(void){
	identifier = 0 ;
}

static SEXP makeSrcref(YYLTYPE *lloc, SEXP srcfile)
{
    SEXP val;

    PROTECT(val = allocVector(INTSXP, 8));
    INTEGER(val)[0] = lloc->first_line;
    INTEGER(val)[1] = lloc->first_byte;
    INTEGER(val)[2] = lloc->last_line;
    INTEGER(val)[3] = lloc->last_byte;
    INTEGER(val)[4] = lloc->first_column;
    INTEGER(val)[5] = lloc->last_column;
    INTEGER(val)[6] = lloc->first_parsed;
    INTEGER(val)[7] = lloc->last_parsed;
    setAttrib(val, R_SrcfileSymbol, srcfile);
    setAttrib(val, R_ClassSymbol, mkString("srcref"));
    UNPROTECT(1); /* val */
    return val;
}

static void attachSrcrefs(SEXP val)
{
    SEXP srval;

    PROTECT(srval = SrcRefsToVectorList());
    
    setAttrib(val, R_SrcrefSymbol, srval);
    setAttrib(val, R_SrcfileSymbol, PS_SRCFILE);
    {
	YYLTYPE wholeFile;
	wholeFile.first_line = 1;
	wholeFile.first_byte = 0;
	wholeFile.first_column = 0;
	wholeFile.last_line = ParseState.xxlineno;
	wholeFile.last_byte = ParseState.xxbyteno;
	wholeFile.last_column = ParseState.xxcolno;
	wholeFile.first_parsed = 1;
	wholeFile.last_parsed = ParseState.xxparseno;
	setAttrib(val, R_WholeSrcrefSymbol, makeSrcref(&wholeFile, PS_SRCFILE));
    }
    PS_SET_SRCREFS(R_NilValue);
    ParseState.didAttach = TRUE;
    UNPROTECT(1); /* srval */
}

static int xxvalue(SEXP v, int k, YYLTYPE *lloc)
{
    if (k > 2) {
	if (ParseState.keepSrcRefs) {
	    SEXP s = PROTECT(makeSrcref(lloc, PS_SRCFILE));
	    AppendToSrcRefs(s);
	    UNPROTECT(1); /* s */
	}
	RELEASE_SV(v);
    }
    R_CurrentExpr = v;
    return k;
}

static SEXP xxnullformal()
{
    SEXP ans;
    PRESERVE_SV(ans = R_NilValue);
    return ans;
}

static SEXP xxfirstformal0(SEXP sym)
{
    SEXP ans;
    if (GenerateCode)
	PRESERVE_SV(ans = FirstArg(R_MissingArg, sym));
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(sym);
    return ans;
}

static SEXP xxfirstformal1(SEXP sym, SEXP expr)
{
    SEXP ans;
    if (GenerateCode)
	PRESERVE_SV(ans = FirstArg(expr, sym));
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(expr);
    RELEASE_SV(sym);
    return ans;
}

static SEXP xxaddformal0(SEXP formlist, SEXP sym, YYLTYPE *lloc)
{
    SEXP ans;
    if (GenerateCode) {
	CheckFormalArgs(formlist, sym, lloc);
	NextArg(formlist, R_MissingArg, sym);
	ans = formlist;
    } else {
	RELEASE_SV(formlist);
	PRESERVE_SV(ans = R_NilValue);
    }
    RELEASE_SV(sym);
    return ans;
}

static SEXP xxaddformal1(SEXP formlist, SEXP sym, SEXP expr, YYLTYPE *lloc)
{
    SEXP ans;
    if (GenerateCode) {
	CheckFormalArgs(formlist, sym, lloc);
	NextArg(formlist, expr, sym);
	ans = formlist;
    } else {
	RELEASE_SV(formlist);
	PRESERVE_SV(ans = R_NilValue);
    }
    RELEASE_SV(expr);
    RELEASE_SV(sym);
    return ans;
}

static SEXP xxexprlist0(void)
{
    SEXP ans;
    if (GenerateCode) {
	PRESERVE_SV(ans = NewList());
	if (ParseState.keepSrcRefs) {
	    setAttrib(ans, R_SrcrefSymbol, PS_SRCREFS);
	    PS_SET_SRCREFS(R_NilValue);
	}
    }
    else
	PRESERVE_SV(ans = R_NilValue);
    return ans;
}

static SEXP xxexprlist1(SEXP expr, YYLTYPE *lloc)
{
    SEXP ans;
    if (GenerateCode) {
	PRESERVE_SV(ans = NewList());
	if (ParseState.keepSrcRefs) {
	    setAttrib(ans, R_SrcrefSymbol, PS_SRCREFS);
	    SEXP s = PROTECT(makeSrcref(lloc, PS_SRCFILE));
	    SetSingleSrcRef(s);
	    UNPROTECT(1); /* s */
	}
	GrowList(ans, expr);
    }
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(expr);
    return ans;
}

static SEXP xxexprlist2(SEXP exprlist, SEXP expr, YYLTYPE *lloc)
{
    SEXP ans;
    if (GenerateCode) {
	if (ParseState.keepSrcRefs) {
	    SEXP s = PROTECT(makeSrcref(lloc, PS_SRCFILE));
	    AppendToSrcRefs(s);
	    UNPROTECT(1); /* s */
	}
	GrowList(exprlist, expr);
	ans = exprlist;
    } else {
	RELEASE_SV(exprlist);
	PRESERVE_SV(ans = R_NilValue);
    }
    RELEASE_SV(expr);
    return ans;
}

static SEXP xxsub0(void)
{
    SEXP ans;
    if (GenerateCode)
	PRESERVE_SV(ans = lang2(R_MissingArg,R_NilValue));
    else
	PRESERVE_SV(ans = R_NilValue);
    return ans;
}

static SEXP xxsub1(SEXP expr, YYLTYPE *lloc)
{
    SEXP ans;
    if (GenerateCode)
	PRESERVE_SV(ans = TagArg(expr, R_NilValue, lloc));
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(expr);
    return ans;
}

static SEXP xxsymsub0(SEXP sym, YYLTYPE *lloc)
{
    SEXP ans;
    if (GenerateCode)
	PRESERVE_SV(ans = TagArg(R_MissingArg, sym, lloc));
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(sym);
    return ans;
}

static SEXP xxsymsub1(SEXP sym, SEXP expr, YYLTYPE *lloc)
{
    SEXP ans;
    if (GenerateCode)
	PRESERVE_SV(ans = TagArg(expr, sym, lloc));
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(expr);
    RELEASE_SV(sym);
    return ans;
}

static SEXP xxnullsub0(YYLTYPE *lloc)
{
    SEXP ans;
    if (GenerateCode)
	PRESERVE_SV(ans = TagArg(R_MissingArg, R_NullSymbol, lloc));
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(R_NilValue);
    return ans;
}

static SEXP xxnullsub1(SEXP expr, YYLTYPE *lloc)
{
    SEXP ans;
    if (GenerateCode)
	PRESERVE_SV(ans = TagArg(expr, R_NullSymbol, lloc));
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(R_NilValue);
    RELEASE_SV(expr);
    return ans;
}


static SEXP xxsublist1(SEXP sub)
{
    SEXP ans;
    if (GenerateCode)
	PRESERVE_SV(ans = FirstArg(CAR(sub),CADR(sub)));
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(sub);
    return ans;
}

static SEXP xxsublist2(SEXP sublist, SEXP sub)
{
    SEXP ans;
    if (GenerateCode) {
	NextArg(sublist, CAR(sub), CADR(sub));
	ans = sublist;
    } else {
	RELEASE_SV(sublist);
	PRESERVE_SV(ans = R_NilValue);
    }
    RELEASE_SV(sub);
    return ans;
}

static SEXP xxcond(SEXP expr)
{
    EatLines = 1;
    return expr;
}

static SEXP xxifcond(SEXP expr)
{
    EatLines = 1;
    return expr;
}

static SEXP xxif(SEXP ifsym, SEXP cond, SEXP expr)
{
    SEXP ans;
    if (GenerateCode)
	PRESERVE_SV(ans = lang3(ifsym, cond, expr));
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(expr);
    RELEASE_SV(cond);
    return ans;
}

static SEXP xxifelse(SEXP ifsym, SEXP cond, SEXP ifexpr, SEXP elseexpr)
{
    SEXP ans;
    if (GenerateCode)
	PRESERVE_SV(ans = lang4(ifsym, cond, ifexpr, elseexpr));
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(elseexpr);
    RELEASE_SV(ifexpr);
    RELEASE_SV(cond);
    return ans;
}

static SEXP xxforcond(SEXP sym, SEXP expr)
{
    SEXP ans;
    EatLines = 1;
    if (GenerateCode)
	PRESERVE_SV(ans = LCONS(sym, expr));
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(expr);
    RELEASE_SV(sym);
    return ans;
}

static SEXP xxfor(SEXP forsym, SEXP forcond, SEXP body)
{
    SEXP ans;
    if (GenerateCode)
	PRESERVE_SV(ans = lang4(forsym, CAR(forcond), CDR(forcond), body));
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(body);
    RELEASE_SV(forcond);
    return ans;
}

static SEXP xxwhile(SEXP whilesym, SEXP cond, SEXP body)
{
    SEXP ans;
    if (GenerateCode)
	PRESERVE_SV(ans = lang3(whilesym, cond, body));
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(body);
    RELEASE_SV(cond);
    return ans;
}

static SEXP xxrepeat(SEXP repeatsym, SEXP body)
{
    SEXP ans;
    if (GenerateCode)
	PRESERVE_SV(ans = lang2(repeatsym, body));
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(body);
    return ans;
}

static SEXP xxnxtbrk(SEXP keyword)
{
    if (GenerateCode)
	PRESERVE_SV(keyword = lang1(keyword));
    else
	PRESERVE_SV(keyword = R_NilValue);
    return keyword;
}

static SEXP xxfuncall(SEXP expr, SEXP args)
{
    SEXP ans, sav_expr = expr;
    if (GenerateCode) {
	if (isString(expr))
	    expr = installTrChar(STRING_ELT(expr, 0));
	PROTECT(expr);
	if (length(CDR(args)) == 1 && CADR(args) == R_MissingArg && TAG(CDR(args)) == R_NilValue )
	    ans = lang1(expr);
	else
	    ans = LCONS(expr, CDR(args));
	UNPROTECT(1); /* expr */
	PRESERVE_SV(ans);
    } else
	PRESERVE_SV(ans = R_NilValue);

    RELEASE_SV(args);
    RELEASE_SV(sav_expr);
    return ans;
}

static SEXP mkChar2(const char *name)
{
    cetype_t enc = CE_NATIVE;

    if(known_to_be_latin1) enc = CE_LATIN1;
    else if(known_to_be_utf8) enc = CE_UTF8;

    return mkCharLenCE(name, (int) strlen(name), enc);
}

static SEXP mkString2(const char *s, size_t len, Rboolean escaped)
{
    SEXP t;
    cetype_t enc = CE_NATIVE;

    if(known_to_be_latin1) enc = CE_LATIN1;
    else if(!escaped && known_to_be_utf8) enc = CE_UTF8;

    PROTECT(t = allocVector(STRSXP, 1));
    SET_STRING_ELT(t, 0, mkCharLenCE(s, (int) len, enc));
    UNPROTECT(1); /* t */
    return t;
}

static SEXP xxdefun(SEXP fname, SEXP formals, SEXP body, YYLTYPE *lloc)
{
    SEXP ans, srcref;

    if (GenerateCode) {
    	if (ParseState.keepSrcRefs) {
	    srcref = makeSrcref(lloc, PS_SRCFILE);
    	    ParseState.didAttach = TRUE;
    	} else
    	    srcref = R_NilValue;
	PRESERVE_SV(ans = lang4(fname, CDR(formals), body, srcref));
    } else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(body);
    RELEASE_SV(formals);
    return ans;
}

static SEXP xxunary(SEXP op, SEXP arg)
{
    SEXP ans;
    if (GenerateCode)
	PRESERVE_SV(ans = lang2(op, arg));
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(arg);
    return ans;
}

static SEXP xxbinary(SEXP n1, SEXP n2, SEXP n3)
{
    SEXP ans;
    if (GenerateCode)
	PRESERVE_SV(ans = lang3(n1, n2, n3));
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(n2);
    RELEASE_SV(n3);
    return ans;
}

static SEXP findPlaceholderCell(SEXP, SEXP);

static void check_rhs(SEXP rhs)
{
    if (TYPEOF(rhs) != LANGSXP)
	error(_("The pipe operator requires a function call as RHS"));

    /* rule out syntactically special functions */
    /* the IS_SPECIAL_SYMBOL bit is set in names.c */
    SEXP fun = CAR(rhs);
    if (TYPEOF(fun) == SYMSXP && IS_SPECIAL_SYMBOL(fun))
	error("function '%s' not supported in RHS call of a pipe",
	      CHAR(PRINTNAME(fun)));
}

static SEXP xxpipe(SEXP lhs, SEXP rhs)
{
    SEXP ans;
    if (GenerateCode) {
	/* allow x => log(x) on RHS */
	if (TYPEOF(rhs) == LANGSXP && CAR(rhs) == R_PipeBindSymbol) {
	    SEXP var = CADR(rhs);
	    SEXP expr = CADDR(rhs);
	    check_rhs(expr);
	    SEXP phcell = findPlaceholderCell(var, expr);
	    if (phcell == NULL)
		error(_("no placeholder found on RHS"));
	    SETCAR(phcell, lhs);
	    return expr;
	}

	check_rhs(rhs);
	
        SEXP fun = CAR(rhs);
        SEXP args = CDR(rhs);
	PRESERVE_SV(ans = lcons(fun, lcons(lhs, args)));
    }
    else {
	PRESERVE_SV(ans = R_NilValue);
    }
    RELEASE_SV(lhs);
    RELEASE_SV(rhs);
    return ans;
}

static SEXP xxpipebind(SEXP fn, SEXP lhs, SEXP rhs)
{
    static int use_pipebind = 0;
    if (use_pipebind != 1) {
	char *lookup = getenv("_R_USE_PIPEBIND_");
	use_pipebind = ((lookup != NULL) && StringTrue(lookup)) ? 1 : 0;
    }

    if (use_pipebind)
	return xxbinary(fn, lhs, rhs);
    else
	error("'=>' is disabled; set '_R_USE_PIPEBIND_' envvar to a true value to enable it");
}

static SEXP xxparen(SEXP n1, SEXP n2)
{
    SEXP ans;
    if (GenerateCode)
	PRESERVE_SV(ans = lang2(n1, n2));
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(n2);
    return ans;
}


/* This should probably use CONS rather than LCONS, but
   it shouldn't matter and we would rather not meddle
   See PR#7055 */

static SEXP xxsubscript(SEXP a1, SEXP a2, SEXP a3)
{
    SEXP ans;
    if (GenerateCode)
	PRESERVE_SV(ans = LCONS(a2, CONS(a1, CDR(a3))));
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(a3);
    RELEASE_SV(a1);
    return ans;
}

static SEXP xxexprlist(SEXP a1, YYLTYPE *lloc, SEXP a2)
{
    SEXP ans;
    SEXP prevSrcrefs;

    EatLines = 0;
    if (GenerateCode) {
	SET_TYPEOF(a2, LANGSXP);
	SETCAR(a2, a1);
	if (ParseState.keepSrcRefs) {
	    PROTECT(prevSrcrefs = getAttrib(a2, R_SrcrefSymbol));
	    SEXP s = PROTECT(makeSrcref(lloc, PS_SRCFILE));
	    PrependToSrcRefs(s);
	    attachSrcrefs(a2);
	    UNPROTECT(2); /* prevSrcrefs, s */
#ifndef SWITCH_TO_REFCNT
	    /* SrcRefs got NAMED by being an attribute, preventively
	       getAttrib(), but it has not in fact been referenced. Set NAMED
	       to 0 to avoid overhead in further setAttrib calls due to cycle
	       detection. */
	    SET_NAMED(prevSrcrefs, 0);
#endif
	    PS_SET_SRCREFS(prevSrcrefs);
	}
	PRESERVE_SV(ans = a2);
    }
    else
	PRESERVE_SV(ans = R_NilValue);
    RELEASE_SV(a2);
    return ans;
}

/*--------------------------------------------------------------------------*/

static SEXP TagArg(SEXP arg, SEXP tag, YYLTYPE *lloc)
{
    switch (TYPEOF(tag)) {
    case STRSXP:
	tag = installTrChar(STRING_ELT(tag, 0));
    case NILSXP:
    case SYMSXP:
	return lang2(arg, tag);
    default:
	error(_("incorrect tag type at line %d"), lloc->first_line); return R_NilValue/* -Wall */;
    }
}


/* Stretchy List Structures : Lists are created and grown using a special */
/* dotted pair.  The CAR of the list points to the last cons-cell in the */
/* list and the CDR points to the first.  The list can be extracted from */
/* the pair by taking its CDR, while the CAR gives fast access to the end */
/* of the list. */

/* These functions must be called with arguments protected */

/* Create a stretchy-list dotted pair */
static SEXP NewList(void)
{
    SEXP s = CONS(R_NilValue, R_NilValue);
    SETCAR(s, s);
    return s;
}

/* Add a new element at the end of a stretchy list */
static void GrowList(SEXP l, SEXP s)
{
    SEXP tmp;
    tmp = CONS(s, R_NilValue);
    SETCDR(CAR(l), tmp);
    SETCAR(l, tmp);
}

/* Create a stretchy list with a single named element */
static SEXP FirstArg(SEXP s, SEXP tag)
{
    SEXP tmp;
    PROTECT(tmp = NewList());
    GrowList(tmp, s);
    SET_TAG(CAR(tmp), tag);
    UNPROTECT(1); /* tmp */
    return tmp;
}

/* Add named element to the end of a stretchy list */
static void NextArg(SEXP l, SEXP s, SEXP tag)
{
    GrowList(l, s);
    SET_TAG(CAR(l), tag);
}

/* SrcRefs (PS_SRCREFS) are represented as R_NilValue (empty) or by 
   a stretchy list (which includes another representation for empty)
   for fast append operation. */

static void SetSingleSrcRef(SEXP r)
{
    SEXP l;

    PROTECT(l = NewList());
    GrowList(l, r);
    PS_SET_SRCREFS(l);
    UNPROTECT(1); /* l */
}

static void AppendToSrcRefs(SEXP r)
{
    SEXP l = PS_SRCREFS;
    if (l == R_NilValue)
	SetSingleSrcRef(r);
    else
	GrowList(l, r);
}

static void PrependToSrcRefs(SEXP r)
{
    SEXP l = PS_SRCREFS;
    if (l == R_NilValue)
	SetSingleSrcRef(r);
    else if (CDR(l) == R_NilValue)
	/* adding to empty stretchy list */
	GrowList(l, r);
    else {
	SEXP tmp = CONS(r, CDR(l));
	SETCDR(l, tmp);
    }
}

static SEXP SrcRefsToVectorList() {
    SEXP l = PS_SRCREFS;
    if (l == R_NilValue)
	return PairToVectorList(l);
    else
	return PairToVectorList(CDR(l));
}

/*--------------------------------------------------------------------------*/

/*
 *  Parsing Entry Points:
 *
 *  The Following entry points provide language parsing facilities.
 *  Note that there are separate entry points for parsing IoBuffers
 *  (i.e. interactve use), files and R character strings.
 *
 *  The entry points provide the same functionality, they just
 *  set things up in slightly different ways.
 *
 *  The following routines parse a single expression:
 *
 *
 *	SEXP R_Parse1File(FILE *fp, int gencode, ParseStatus *status, Rboolean first)
 *   (used for R_ReplFile in main.c)
 *
 *	SEXP R_Parse1Buffer(IoBuffer *buffer, int gencode, ParseStatus *status, Rboolean first)
 *   (used for ReplIteration and R_ReplDLLdo1 in main.c)
 *
 *  The success of the parse is indicated as folllows:
 *
 *
 *	status = PARSE_NULL       - there was no statement to parse
 *		 PARSE_OK	  - complete statement
 *		 PARSE_INCOMPLETE - incomplete statement
 *		 PARSE_ERROR      - syntax error
 *		 PARSE_EOF	  - end of file
 *
 *
 *  The following routines parse several expressions and return
 *  their values in a single expression vector.
 *
 *	SEXP R_ParseFile(FILE *fp, int n, ParseStatus *status, SEXP srcfile)
 *    (used for do_edit in file edit.c)
 *
 *	SEXP R_ParseVector(SEXP *text, int n, ParseStatus *status, SEXP srcfile)
 *    (public, and used by parse(text=) in file source.c)
 *
 *	SEXP R_ParseBuffer(IoBuffer *buffer, int n, ParseStatus *status, SEXP prompt, SEXP srcfile)
 *    (used by parse(file="") in file source.c)
 *
 *      SEXP R_ParseConn(Rconnection con, int n, ParseStatus *status, SEXP srcfile)
 *    (used by parse(file=) in file source.c)
 *
 *  Here, status is 1 for a successful parse and 0 if parsing failed
 *  for some reason.
 */

#define CONTEXTSTACK_SIZE 50
static int	SavedToken;
static SEXP	SavedLval;
static char	contextstack[CONTEXTSTACK_SIZE], *contextp;

static void PutSrcRefState(SrcRefState *state);
static void UseSrcRefState(SrcRefState *state);

/* This is called once when R starts up. */
attribute_hidden
void InitParser(void)
{
    ParseState.sexps = allocVector(VECSXP, 7); /* initialized to R_NilValue */
    ParseState.data = R_NilValue;
    INIT_SVS();
    R_PreserveObject(ParseState.sexps); /* never released in an R session */
    R_NullSymbol = install("NULL");
    R_PipeBindSymbol = install("=>");
}

static void FinalizeSrcRefStateOnError(void *dummy)
{
    R_FinalizeSrcRefState();
}

/* This is called each time a new parse sequence begins */
attribute_hidden
void R_InitSrcRefState(RCNTXT* cptr)
{
    if (busy) {
    	SrcRefState *prev = malloc(sizeof(SrcRefState));
	if (prev == NULL)
	    error(_("allocation of source reference state failed"));
    	PutSrcRefState(prev);
	ParseState.prevState = prev;
	ParseState.sexps = allocVector(VECSXP, 7);
	ParseState.data = R_NilValue;
	INIT_SVS();
	R_PreserveObject(ParseState.sexps);
	/* ParseState.sexps released in R_FinalizeSrcRefState */
    } else
	/* re-use data, text, ids arrays */
        ParseState.prevState = NULL;
    /* set up context _after_ PutSrcRefState */
    begincontext(cptr, CTXT_CCODE, R_NilValue, R_BaseEnv, R_BaseEnv,
                 R_NilValue, R_NilValue);
    cptr->cend = &FinalizeSrcRefStateOnError;
    cptr->cenddata = NULL;
    ParseState.keepSrcRefs = FALSE;
    ParseState.keepParseData = TRUE;
    ParseState.didAttach = FALSE;
    PS_SET_SRCFILE(R_NilValue);
    PS_SET_ORIGINAL(R_NilValue);
    ParseState.data_count = 0;
    ParseState.xxlineno = 1;
    ParseState.xxcolno = 0;
    ParseState.xxbyteno = 0;
    ParseState.xxparseno = 1;
    busy = TRUE;
}

attribute_hidden
void R_FinalizeSrcRefState(void)
{
    PS_SET_SRCFILE(R_NilValue);
    PS_SET_ORIGINAL(R_NilValue);
    CLEAR_SVS();

    /* Free the data, text and ids if we are restoring a previous state,
       or if they have grown too large */
    if (PS_DATA != R_NilValue) {
    	if (ParseState.prevState || DATA_COUNT > MAX_DATA_COUNT) {
	    PS_SET_DATA(R_NilValue);
	    PS_SET_TEXT(R_NilValue);
	} else /* Remove all the strings from the text vector so they don't take up memory, and clean up data */
	    for (int i=0; i < ParseState.data_count; i++) {
		SET_STRING_ELT(PS_TEXT, i, R_BlankString);
		_PARENT(i) = 0;
	    }
    } 
    if (PS_IDS != R_NilValue) {
	if (ParseState.prevState || ID_COUNT > MAX_DATA_COUNT) {
	    PS_SET_IDS(R_NilValue);
        } else {/* Remove the parent records */
            if (identifier > ID_COUNT) identifier = ID_COUNT;
            for (int i=0; i < identifier; i++) {
		ID_ID(i) = 0;
	        ID_PARENT(i) = 0;
	    }
	}
    }
    ParseState.data_count = NA_INTEGER;
    if (ParseState.prevState) {
	R_ReleaseObject(ParseState.sexps);
        SrcRefState *prev = ParseState.prevState;
    	UseSrcRefState(prev);
    	free(prev);
    } else
        busy = FALSE;
}

static void UseSrcRefState(SrcRefState *state)
{
    ParseState.keepSrcRefs = state->keepSrcRefs;
    ParseState.keepParseData = state->keepParseData;
    ParseState.sexps = state->sexps;
    ParseState.data = state->data;
    ParseState.data_count = state->data_count;
    ParseState.xxlineno = state->xxlineno;
    ParseState.xxcolno = state->xxcolno;
    ParseState.xxbyteno = state->xxbyteno;
    ParseState.xxparseno = state->xxparseno;
    ParseState.prevState = state->prevState;
    busy = TRUE;
}

static void PutSrcRefState(SrcRefState *state)
{
    state->keepSrcRefs = ParseState.keepSrcRefs;
    state->keepParseData = ParseState.keepParseData;
    state->sexps = ParseState.sexps;
    state->data = ParseState.data;
    state->data_count = ParseState.data_count;
    state->xxlineno = ParseState.xxlineno;
    state->xxcolno = ParseState.xxcolno;
    state->xxbyteno = ParseState.xxbyteno;
    state->xxparseno = ParseState.xxparseno;
    state->prevState = ParseState.prevState;
}

static void ParseInit(void)
{
    contextp = contextstack;
    *contextp = ' ';
    SavedToken = 0;
    SavedLval = R_NilValue;
    EatLines = 0;
    EndOfFile = 0;
    xxcharcount = 0;
    npush = 0;
}

static void initData(void)
{
    ParseState.data_count = 0 ;
}


static void ParseContextInit(void)
{
    R_ParseContextLast = 0;
    R_ParseContext[0] = '\0';
    
    /* starts the identifier counter*/
    initId();
    initData();
}

static SEXP R_Parse1(ParseStatus *status)
{
    switch(yyparse()) {
    case 0:                     /* End of file */
	*status = PARSE_EOF;
	if (EndOfFile == 2) *status = PARSE_INCOMPLETE;
	break;
    case 1:                     /* Syntax error / incomplete */
	*status = PARSE_ERROR;
	if (EndOfFile) *status = PARSE_INCOMPLETE;
	break;
    case 2:                     /* Empty Line */
	*status = PARSE_NULL;
	break;
    case 3:                     /* Valid expr '\n' terminated */
    case 4:                     /* Valid expr ';' terminated */
	*status = PARSE_OK;
	break;
    }
    return R_CurrentExpr;
}

static FILE *fp_parse;

static int file_getc(void)
{
    return R_fgetc(fp_parse);
}

/* used in main.c */
attribute_hidden
SEXP R_Parse1File(FILE *fp, int gencode, ParseStatus *status)
{
    ParseInit();
    ParseContextInit();
    GenerateCode = gencode;
    fp_parse = fp;
    ptr_getc = file_getc;
    R_Parse1(status);
    CLEAR_SVS();
    return R_CurrentExpr;
}

static IoBuffer *iob;

static int buffer_getc(void)
{
    return R_IoBufferGetc(iob);
}

/* Used only in main.c */
attribute_hidden
SEXP R_Parse1Buffer(IoBuffer *buffer, int gencode, ParseStatus *status)
{
    Rboolean keepSource = FALSE; 
    RCNTXT cntxt;

    R_InitSrcRefState(&cntxt);
    if (gencode) {
    	keepSource = asLogical(GetOption1(install("keep.source")));
    	if (keepSource) {
    	    ParseState.keepSrcRefs = TRUE;
	    ParseState.keepParseData =
		asLogical(GetOption1(install("keep.parse.data")));
	    PS_SET_SRCFILE(NewEnvironment(R_NilValue, R_NilValue, R_EmptyEnv));
	    PS_SET_ORIGINAL(PS_SRCFILE);
	    PS_SET_SRCREFS(R_NilValue);
	}
    }
    ParseInit();
    ParseContextInit();
    GenerateCode = gencode;
    iob = buffer;
    ptr_getc = buffer_getc;
    R_Parse1(status);
    if (gencode && keepSource) {
    	if (ParseState.didAttach) {
   	    int buflen = R_IoBufferReadOffset(buffer);
   	    char buf[buflen+1];
   	    SEXP class;
   	    R_IoBufferReadReset(buffer);
   	    for (int i=0; i<buflen; i++)
   	    	buf[i] = (char) R_IoBufferGetc(buffer);

   	    buf[buflen] = 0;
	    SEXP s_filename = install("filename");
	    defineVar(s_filename, ScalarString(mkChar("")), PS_ORIGINAL);
	    SEXP s_lines = install("lines");
	    defineVar(s_lines, ScalarString(mkChar2(buf)), PS_ORIGINAL);
    	    PROTECT(class = allocVector(STRSXP, 2));
            SET_STRING_ELT(class, 0, mkChar("srcfilecopy"));
            SET_STRING_ELT(class, 1, mkChar("srcfile"));
	    setAttrib(PS_ORIGINAL, R_ClassSymbol, class);
	    UNPROTECT(1); /* class */
	}
    }
    PROTECT(R_CurrentExpr);
    endcontext(&cntxt);
    R_FinalizeSrcRefState();
    UNPROTECT(1); /* R_CurrentExpr */
    return R_CurrentExpr;
}

static TextBuffer *txtb;

static int text_getc(void)
{
    return R_TextBufferGetc(txtb);
}

static SEXP R_Parse(int n, ParseStatus *status, SEXP srcfile)
{
    int i;
    SEXP t, rval;
    RCNTXT cntxt;

    R_InitSrcRefState(&cntxt);
    ParseContextInit();

    PS_SET_SRCFILE(srcfile);
    PS_SET_ORIGINAL(srcfile);
    
    if (isEnvironment(srcfile)) {
    	ParseState.keepSrcRefs = TRUE;
	ParseState.keepParseData =
	    asLogical(GetOption1(install("keep.parse.data")));
	PS_SET_SRCREFS(R_NilValue);
    }
    
    PROTECT(t = NewList());
    for(i = 0; ; ) {
	if(n >= 0 && i >= n) break;
	ParseInit();
	rval = R_Parse1(status);
	switch(*status) {
	case PARSE_NULL:
	    break;
	case PARSE_OK:
	    PROTECT(rval);
	    GrowList(t, rval);
	    UNPROTECT(1); /* rval */
	    i++;
	    break;
	case PARSE_INCOMPLETE:
	case PARSE_ERROR:
	    UNPROTECT(1); /* t */
	    if (ParseState.keepSrcRefs && ParseState.keepParseData)
	        finalizeData();
	    endcontext(&cntxt);
	    R_FinalizeSrcRefState();	    
	    return R_NilValue;
	    break;
	case PARSE_EOF:
	    goto finish;
	    break;
	}
    }

finish:

    t = CDR(t);
    PROTECT(rval = allocVector(EXPRSXP, length(t)));
    for (n = 0 ; n < LENGTH(rval) ; n++, t = CDR(t))
	SET_VECTOR_ELT(rval, n, CAR(t));
    if (ParseState.keepSrcRefs) {
	if (ParseState.keepParseData)
	    finalizeData();
	attachSrcrefs(rval);
    }
    UNPROTECT(2); /* t, rval */
    PROTECT(rval);
    endcontext(&cntxt);
    R_FinalizeSrcRefState();
    UNPROTECT(1); /* rval */
    *status = PARSE_OK;
    return rval;
}

/* used in edit.c */
attribute_hidden
SEXP R_ParseFile(FILE *fp, int n, ParseStatus *status, SEXP srcfile)
{
    GenerateCode = 1;
    fp_parse = fp;
    ptr_getc = file_getc;
    return R_Parse(n, status, srcfile);
}

#include "Rconnections.h"
static Rconnection con_parse;

/* need to handle incomplete last line */
static int con_getc(void)
{
    int c;
    static int last=-1000;

    c = Rconn_fgetc(con_parse);
    if (c == EOF && last != '\n') c = '\n';
    return (last = c);
}

/* used in source.c */
attribute_hidden
SEXP R_ParseConn(Rconnection con, int n, ParseStatus *status, SEXP srcfile)
{
    GenerateCode = 1;
    con_parse = con;
    ptr_getc = con_getc;
    return R_Parse(n, status, srcfile);
}

/* This one is public, and used in source.c */
SEXP R_ParseVector(SEXP text, int n, ParseStatus *status, SEXP srcfile)
{
    SEXP rval;
    TextBuffer textb;
    R_TextBufferInit(&textb, text);
    txtb = &textb;
    GenerateCode = 1;
    ptr_getc = text_getc;
    rval = R_Parse(n, status, srcfile);
    R_TextBufferFree(&textb);
    return rval;
}

static const char *Prompt(SEXP prompt, int type)
{
    if(type == 1) {
	if(length(prompt) <= 0) {
	    return CHAR(STRING_ELT(GetOption1(install("prompt")), 0));
	}
	else
	    return CHAR(STRING_ELT(prompt, 0));
    }
    else {
	return CHAR(STRING_ELT(GetOption1(install("continue")), 0));
    }
}

/* used in source.c */
attribute_hidden
SEXP R_ParseBuffer(IoBuffer *buffer, int n, ParseStatus *status, SEXP prompt, 
		   SEXP srcfile)
{
    SEXP rval, t;
    char *bufp, buf[CONSOLE_BUFFER_SIZE];
    int c, i, prompt_type = 1;
    RCNTXT cntxt;

    R_IoBufferWriteReset(buffer);
    buf[0] = '\0';
    bufp = buf;
    R_InitSrcRefState(&cntxt);
    ParseContextInit();
    
    GenerateCode = 1;
    iob = buffer;
    ptr_getc = buffer_getc;

    PS_SET_SRCFILE(srcfile);
    PS_SET_ORIGINAL(srcfile);
    
    if (isEnvironment(srcfile)) {
    	ParseState.keepSrcRefs = TRUE;
	ParseState.keepParseData =
	    asLogical(GetOption1(install("keep.parse.data")));
	PS_SET_SRCREFS(R_NilValue);
    }
    
    PROTECT(t = NewList());
    for(i = 0; ; ) {
	if(n >= 0 && i >= n) break;
	if (!*bufp) {
	    if(R_ReadConsole((char *) Prompt(prompt, prompt_type),
			     (unsigned char *)buf, CONSOLE_BUFFER_SIZE, 1) == 0)
		goto finish;
	    bufp = buf;
	}
	while ((c = *bufp++)) {
	    R_IoBufferPutc(c, buffer);
	    if (c == ';' || c == '\n') break;
	}

	/* Was a call to R_Parse1Buffer, but we don't want to reset
	   xxlineno and xxcolno */
	ParseInit();
	/* Not calling ParseContextInit() as it resets parse data, and
	   to be consistent with R_Parse */
	R_Parse1(status);
	rval = R_CurrentExpr;

	switch(*status) {
	case PARSE_NULL:
	    break;
	case PARSE_OK:
	    PROTECT(rval);
	    GrowList(t, rval);
	    UNPROTECT(1); /* rval */
	    i++;
	    break;
	case PARSE_INCOMPLETE:
	case PARSE_ERROR:
	    UNPROTECT(1); /* t */
	    R_IoBufferWriteReset(buffer);
	    endcontext(&cntxt);
	    R_FinalizeSrcRefState();
	    return R_NilValue;
	    break;
	case PARSE_EOF:
	    goto finish;
	    break;
	}
    }
finish:
    R_IoBufferWriteReset(buffer);
    t = CDR(t);
    PROTECT(rval = allocVector(EXPRSXP, length(t)));
    for (n = 0 ; n < LENGTH(rval) ; n++, t = CDR(t))
	SET_VECTOR_ELT(rval, n, CAR(t));
    if (ParseState.keepSrcRefs) {
	if (ParseState.keepParseData)
	    finalizeData();
	attachSrcrefs(rval);
    }
    UNPROTECT(2); /* t, rval */
    PROTECT(rval);
    endcontext(&cntxt);
    R_FinalizeSrcRefState();
    UNPROTECT(1); /* rval */
    *status = PARSE_OK;
    return rval;
}


/*----------------------------------------------------------------------------
 *
 *  The Lexical Analyzer:
 *
 *  Basic lexical analysis is performed by the following
 *  routines.  Input is read a line at a time, and, if the
 *  program is in batch mode, each input line is echoed to
 *  standard output after it is read.
 *
 *  The function yylex() scans the input, breaking it into
 *  tokens which are then passed to the parser.  The lexical
 *  analyser maintains a symbol table (in a very messy fashion).
 *
 *  The fact that if statements need to parse differently
 *  depending on whether the statement is being interpreted or
 *  part of the body of a function causes the need for ifpop
 *  and IfPush.  When an if statement is encountered an 'i' is
 *  pushed on a stack (provided there are parentheses active).
 *  At later points this 'i' needs to be popped off of the if
 *  stack.
 *
 */

static void IfPush(void)
{
    if (*contextp==LBRACE ||
	*contextp=='['    ||
	*contextp=='('    ||
	*contextp == 'i') {
	if(contextp - contextstack >= CONTEXTSTACK_SIZE)
	    error(_("contextstack overflow"));
	*++contextp = 'i';
    }

}

static void ifpop(void)
{
    if (*contextp=='i')
	*contextp-- = 0;
}

/* This is only called following ., so we only care if it is
   an ANSI digit or not */
static int typeofnext(void)
{
    int k, c;

    c = xxgetc();
    if (isdigit(c)) k = 1; else k = 2;
    xxungetc(c);
    return k;
}

static int nextchar(int expect)
{
    int c = xxgetc();
    if (c == expect)
	return 1;
    else
	xxungetc(c);
    return 0;
}

/* Special Symbols */
/* Syntactic Keywords + Symbolic Constants */

struct {
    char *name;
    int token;
}
static keywords[] = {
    { "NULL",	    NULL_CONST },
    { "NA",	    NUM_CONST  },
    { "TRUE",	    NUM_CONST  },
    { "FALSE",	    NUM_CONST  },
    { "Inf",	    NUM_CONST  },
    { "NaN",	    NUM_CONST  },
    { "NA_integer_", NUM_CONST  },
    { "NA_real_",    NUM_CONST  },
    { "NA_character_", NUM_CONST  },
    { "NA_complex_", NUM_CONST  },
    { "function",   FUNCTION   },
    { "while",	    WHILE      },
    { "repeat",	    REPEAT     },
    { "for",	    FOR	       },
    { "if",	    IF	       },
    { "in",	    IN	       },
    { "else",	    ELSE       },
    { "next",	    NEXT       },
    { "break",	    BREAK      },
    { "...",	    SYMBOL     },
    { 0,	    0	       }
};

/* KeywordLookup has side effects, it sets yylval */

static int KeywordLookup(const char *s)
{
    int i;
    for (i = 0; keywords[i].name; i++) {
	if (strcmp(keywords[i].name, s) == 0) {
	    switch (keywords[i].token) {
	    case NULL_CONST:
		PRESERVE_SV(yylval = R_NilValue);
		break;
	    case NUM_CONST:
		if(GenerateCode) {
		    switch(i) {
		    case 1:
			PRESERVE_SV(yylval = mkNA());
			break;
		    case 2:
			PRESERVE_SV(yylval = mkTrue());
			break;
		    case 3:
			PRESERVE_SV(yylval = mkFalse());
			break;
		    case 4:
			PRESERVE_SV(yylval = allocVector(REALSXP, 1));
			REAL(yylval)[0] = R_PosInf;
			break;
		    case 5:
			PRESERVE_SV(yylval = allocVector(REALSXP, 1));
			REAL(yylval)[0] = R_NaN;
			break;
		    case 6:
			PRESERVE_SV(yylval = allocVector(INTSXP, 1));
			INTEGER(yylval)[0] = NA_INTEGER;
			break;
		    case 7:
			PRESERVE_SV(yylval = allocVector(REALSXP, 1));
			REAL(yylval)[0] = NA_REAL;
			break;
		    case 8:
			PRESERVE_SV(yylval = allocVector(STRSXP, 1));
			SET_STRING_ELT(yylval, 0, NA_STRING);
			break;
		    case 9:
			PRESERVE_SV(yylval = allocVector(CPLXSXP, 1));
			COMPLEX(yylval)[0].r = COMPLEX(yylval)[0].i = NA_REAL;
			break;
		    }
		} else
		    PRESERVE_SV(yylval = R_NilValue);
		break;
	    case FUNCTION:
	    case WHILE:
	    case REPEAT:
	    case FOR:
	    case IF:
	    case NEXT:
	    case BREAK:
		yylval = install(s);
		break;
	    case IN:
	    case ELSE:
		break;
	    case SYMBOL:
		PRESERVE_SV(yylval = install(s));
		break;
	    }
	    return keywords[i].token;
	}
    }
    return 0;
}

static SEXP mkFloat(const char *s)
{
    return ScalarReal(R_atof(s));
}

static SEXP mkInt(const char *s)
{
    double f = R_atof(s);  /* or R_strtol? */
    return ScalarInteger((int) f);
}

static SEXP mkComplex(const char *s)
{
    SEXP t = R_NilValue;
    double f;
    f = R_atof(s); /* FIXME: make certain the value is legitimate. */
    t = allocVector(CPLXSXP, 1);
    COMPLEX(t)[0].r = 0;
    COMPLEX(t)[0].i = f;
    return t;
}

static SEXP mkNA(void)
{
    SEXP t = allocVector(LGLSXP, 1);
    LOGICAL(t)[0] = NA_LOGICAL;
    return t;
}

attribute_hidden
SEXP mkTrue(void)
{
    SEXP s = allocVector(LGLSXP, 1);
    LOGICAL(s)[0] = 1;
    return s;
}

SEXP mkFalse(void)
{
    SEXP s = allocVector(LGLSXP, 1);
    LOGICAL(s)[0] = 0;
    return s;
}

static void yyerror(const char *s)
{
    static const char *const yytname_translations[] =
    {
    /* the left column are strings coming from bison, the right
       column are translations for users.
       The first YYENGLISH from the right column are English to be translated,
       the rest are to be copied literally.  The #if 0 block below allows xgettext
       to see these.
    */
#define YYENGLISH 8
	"$undefined",	"input",
	"END_OF_INPUT",	"end of input",
	"ERROR",	"input",
	"STR_CONST",	"string constant",
	"NUM_CONST",	"numeric constant",
	"SYMBOL",	"symbol",
	"LEFT_ASSIGN",	"assignment",
	"'\\n'",	"end of line",
	"NULL_CONST",	"'NULL'",
	"FUNCTION",	"'function'",
	"EQ_ASSIGN",	"'='",
	"RIGHT_ASSIGN",	"'->'",
	"LBB",		"'[['",
	"FOR",		"'for'",
	"IN",		"'in'",
	"IF",		"'if'",
	"ELSE",		"'else'",
	"WHILE",	"'while'",
	"NEXT",		"'next'",
	"BREAK",	"'break'",
	"REPEAT",	"'repeat'",
	"GT",		"'>'",
	"GE",		"'>='",
	"LT",		"'<'",
	"LE",		"'<='",
	"EQ",		"'=='",
	"NE",		"'!='",
	"AND",		"'&'",
	"OR",		"'|'",
	"AND2",		"'&&'",
	"OR2",		"'||'",
	"NS_GET",	"'::'",
	"NS_GET_INT",	"':::'",
	"PIPE",         "'|>'",
	"PIPEBIND",     "'=>'",
	0
    };
    static char const yyunexpected[] = "syntax error, unexpected ";
    static char const yyexpecting[] = ", expecting ";
    char *expecting;
    
    if (!EndOfFile)
	/* On EndOfFile, there are no more bytes to add, but trying to do
	   so may have non-trivial performance overhead and this can be
	   reached also in non-error situations, e.g. from repl.
	*/
	finish_mbcs_in_parse_context();

    R_ParseError     = yylloc.first_line;
    R_ParseErrorCol  = yylloc.first_column;
    R_ParseErrorFile = PS_SRCFILE;

    if (!strncmp(s, yyunexpected, sizeof yyunexpected -1)) {
	int i;
	/* Edit the error message */
	expecting = strstr(s + sizeof yyunexpected -1, yyexpecting);
	if (expecting) *expecting = '\0';
	for (i = 0; yytname_translations[i]; i += 2) {
	    if (!strcmp(s + sizeof yyunexpected - 1, yytname_translations[i])) {
                switch(i/2)
                {
                case 0:
                        snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE, _("unexpected input"));
                                break;
                case 1:
                        snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE, _("unexpected end of input"));
                                break;
                case 2:
                        snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE, _("unexpected input"));
                                break;
                case 3:
                        snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE, _("unexpected string constant"));
                                break;
                case 4:
                        snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE, _("unexpected numeric constant"));
                                break;
                case 5:
                        snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE, _("unexpected symbol"));
                                break;
                case 6:
                        snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE, _("unexpected assignment"));
                                break;
                case 7:
                        snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE, _("unexpected end of line"));
                                break;
                default:
                  snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE, _("unexpected %s"),
                           yytname_translations[i+1]);
                                break;
                }
                
		return;
	    }
	}
	snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE - 1, _("unexpected %s"),
                 s + sizeof yyunexpected - 1);
    } else {
	strncpy(R_ParseErrorMsg, s, PARSE_ERROR_SIZE - 1);
        R_ParseErrorMsg[PARSE_ERROR_SIZE - 1] = '\0';
    }
}

static void CheckFormalArgs(SEXP formlist, SEXP _new, YYLTYPE *lloc)
{
    while (formlist != R_NilValue) {
	if (TAG(formlist) == _new) {
	    error(_("repeated formal argument '%s' on line %d"), EncodeChar(PRINTNAME(_new)),
								 lloc->first_line);
	}
	formlist = CDR(formlist);
    }
}

/* This is used as the buffer for NumericValue, SpecialValue and
   SymbolValue.  None of these could conceivably need 8192 bytes.

   It has not been used as the buffer for input character strings
   since Oct 2007 (released as 2.7.0), and for comments since 2.8.0
 */
static char yytext[MAXELTSIZE];

static int SkipSpace(void)
{
    int c;

#if defined(USE_RI18N_FNS) // includes Win32
    static wctype_t blankwct = 0;
    if (!blankwct)
	blankwct = Ri18n_wctype("blank");
#endif

#ifdef Win32
    if(!mbcslocale) { /* 0xa0 is NBSP in all 8-bit Windows locales */
	while ((c = xxgetc()) == ' ' || c == '\t' || c == '\f' ||
	       (unsigned int) c == 0xa0) ;
	return c;
    } else {
	int i, clen;
	wchar_t wc;
	while (1) {
	    c = xxgetc();
	    if (c == ' ' || c == '\t' || c == '\f') continue;
	    if (c == '\n' || c == R_EOF) break;
	    if ((unsigned int) c < 0x80) break;
	    clen = mbcs_get_next(c, &wc);  /* always 2 */
	    if(! Ri18n_iswctype(wc, blankwct) ) break;
	    for(i = 1; i < clen; i++) c = xxgetc();
	}
	return c;
    }
#endif
#if defined(__STDC_ISO_10646__)
    if(mbcslocale) { /* wctype functions need Unicode wchar_t */
	int i, clen;
	wchar_t wc;
	while (1) {
	    c = xxgetc();
	    if (c == ' ' || c == '\t' || c == '\f') continue;
	    if (c == '\n' || c == R_EOF) break;
	    if ((unsigned int) c < 0x80) break;
	    clen = mbcs_get_next(c, &wc);
#if defined(USE_RI18N_FNS)
	    if(! Ri18n_iswctype(wc, blankwct) ) break;
#else
	    if(! iswblank(wc) ) break;
#endif
	    for(i = 1; i < clen; i++) c = xxgetc();
	}
    } else
#endif
	// does not support non-ASCII spaces, unlike Windows
	while ((c = xxgetc()) == ' ' || c == '\t' || c == '\f') ;
    return c;
}

/* Note that with interactive use, EOF cannot occur inside */
/* a comment.  However, semicolons inside comments make it */
/* appear that this does happen.  For this reason we use the */
/* special assignment EndOfFile=2 to indicate that this is */
/* going on.  This is detected and dealt with in Parse1Buffer. */

static int SkipComment(void)
{
    int c='#', i;
    
    /* locations before the # character was read */
    int _first_column = ParseState.xxcolno ;
    int _first_parsed = ParseState.xxparseno ;
    int type = COMMENT ;

    Rboolean maybeLine = (ParseState.xxcolno == 1);
    Rboolean doSave;

    DECLARE_YYTEXT_BUFP(yyp);
    
    if (maybeLine) {
    	char lineDirective[] = "#line";
    	YYTEXT_PUSH(c, yyp);
    	for (i=1; i<5; i++) {
    	    c = xxgetc();
  	    if (c != (int)(lineDirective[i])) {
  	    	maybeLine = FALSE;
  	    	break;
  	    }
            YYTEXT_PUSH(c, yyp);
  	}
  	if (maybeLine)     
	    c = processLineDirective(&type);
    }
    // we want to track down the character
    // __before__ the new line character
    int _last_column  = ParseState.xxcolno ;
    int _last_parsed  = ParseState.xxparseno ;
    
    if (c == '\n') {
        _last_column = prevcols[prevpos];
        _last_parsed = prevparse[prevpos];
    }
    
    doSave = !maybeLine;
    
    while (c != '\n' && c != R_EOF) {
        // Comments can be any length; we only record the ones that fit in yytext.
        if (doSave) {
            YYTEXT_PUSH(c, yyp);
            doSave = (yyp - yytext) < sizeof(yytext) - 2;
        }
 	_last_column = ParseState.xxcolno ;
	_last_parsed = ParseState.xxparseno ;
	c = xxgetc();
    }
    if (c == R_EOF) EndOfFile = 2;
    incrementId( ) ;
    YYTEXT_PUSH('\0', yyp);
    record_( _first_parsed, _first_column, _last_parsed, _last_column,
	     type, identifier, doSave ? yytext : 0 ) ;
    return c;
}

static int NumericValue(int c)
{
    int seendot = (c == '.');
    int seenexp = 0;
    int last = c;
    int nd = 0;
    int asNumeric = 0;
    int count = 1; /* The number of characters seen */

    DECLARE_YYTEXT_BUFP(yyp);
    YYTEXT_PUSH(c, yyp);
    /* We don't care about other than ASCII digits */
    while (isdigit(c = xxgetc()) || c == '.' || c == 'e' || c == 'E'
	   || c == 'x' || c == 'X' || c == 'L')
    {
	count++;
	if (c == 'L') /* must be at the end.  Won't allow 1Le3 (at present). */
	{   YYTEXT_PUSH(c, yyp);
	    break;
	}
	
	if (c == 'x' || c == 'X') {
	    if (count > 2 || last != '0') break;  /* 0x must be first */
	    YYTEXT_PUSH(c, yyp);
	    while(isdigit(c = xxgetc()) || ('a' <= c && c <= 'f') ||
		  ('A' <= c && c <= 'F') || c == '.') {
		if (c == '.') {
		    if (seendot) return ERROR;
		    seendot = 1;
		}
		YYTEXT_PUSH(c, yyp);
		nd++;
	    }
	    if (nd == 0) return ERROR;
	    if (c == 'p' || c == 'P') {
	        seenexp = 1;
		YYTEXT_PUSH(c, yyp);
		c = xxgetc();
		if (!isdigit(c) && c != '+' && c != '-') return ERROR;
		if (c == '+' || c == '-') {
		    YYTEXT_PUSH(c, yyp);
		    c = xxgetc();
		}
		for(nd = 0; isdigit(c); c = xxgetc(), nd++)
		    YYTEXT_PUSH(c, yyp);
		if (nd == 0) return ERROR;
	    }
            if (seendot && !seenexp) return ERROR;
	    break;
	}
	if (c == 'E' || c == 'e') {
	    if (seenexp)
		break;
	    seenexp = 1;
	    seendot = seendot == 1 ? seendot : 2;
	    YYTEXT_PUSH(c, yyp);
	    c = xxgetc();
	    if (!isdigit(c) && c != '+' && c != '-') return ERROR;
	    if (c == '+' || c == '-') {
		YYTEXT_PUSH(c, yyp);
		c = xxgetc();
		if (!isdigit(c)) return ERROR;
	    }
	}
	if (c == '.') {
	    if (seendot)
		break;
	    seendot = 1;
	}
	YYTEXT_PUSH(c, yyp);
	last = c;
    }
    
    if(c == 'i')
	YYTEXT_PUSH(c, yyp); /* for getParseData */
	
    YYTEXT_PUSH('\0', yyp);    
    /* Make certain that things are okay. */
    if(c == 'L') {
	double a = R_atof(yytext);
	int b = (int) a;
	/* We are asked to create an integer via the L, so we check that the
	   double and int values are the same. If not, this is a problem and we
	   will not lose information and so use the numeric value.
	*/
	if(a != (double) b) {
	    if(GenerateCode) {
		if(seendot == 1 && seenexp == 0)
		    warning(_("integer literal %s contains decimal; using numeric value"), yytext);
		else {
		    /* hide the L for the warning message */
		    warning(_("non-integer value %s qualified with L; using numeric value"), yytext);
		}
	    }
	    asNumeric = 1;
	    seenexp = 1;
	}
    }

    if(c == 'i') {
	yylval = GenerateCode ? mkComplex(yytext) : R_NilValue;
    } else if(c == 'L' && asNumeric == 0) {
	if(GenerateCode && seendot == 1 && seenexp == 0)
	    warning(_("integer literal %s contains unnecessary decimal point"), yytext);
	yylval = GenerateCode ? mkInt(yytext) : R_NilValue;
#if 0  /* do this to make 123 integer not double */
    } else if(!(seendot || seenexp)) {
	if(c != 'L') xxungetc(c);
	if (GenerateCode) {
	    double a = R_atof(yytext);
	    int b = (int) a;
	    yylval = (a != (double) b) ? mkFloat(yytext) : mkInt(yytext);
	} else yylval = R_NilValue;
#endif
    } else {
	if(c != 'L')
	    xxungetc(c);
	yylval = GenerateCode ? mkFloat(yytext) : R_NilValue;
    }

    PRESERVE_SV(yylval);
    return NUM_CONST;
}

/* Strings may contain the standard ANSI escapes and octal */
/* specifications of the form \o, \oo or \ooo, where 'o' */
/* is an octal digit. */

/* The buffer is reallocated on the R heap if needed; not by malloc */
/* to avoid memory leak in case of R error (long jump) */
#define STEXT_PUSH(c) do {                  \
	size_t nc = bp - stext;             \
	if (nc >= nstext - 1) {             \
	    char *old = stext;              \
	    SEXP st1;		            \
	    nstext *= 2;                    \
	    PROTECT(st1 = allocVector(RAWSXP, nstext)); \
	    stext = (char *)RAW(st1);       \
	    memmove(stext, old, nc);        \
	    REPROTECT(st1, sti);	    \
	    UNPROTECT(1); /* st1 */         \
	    bp = stext+nc; }		    \
	*bp++ = ((char) c);		    \
} while(0)


/* The idea here is that if a string contains \u escapes that are not
   valid in the current locale, we should switch to UTF-8 for that
   string.  Needs Unicode wide-char support or out substitutes.

   Defining __STDC_ISO_10646__ is done by the OS (or not) in wchar.t.
   Some (e.g. macOS, Solaris, FreeBSD) have Unicode wchar_t but do not
   define it: we override macOS and FreeBSD earlier in this file.
*/

#if defined(Win32) || defined(__STDC_ISO_10646__)
typedef wchar_t ucs_t;
# define mbcs_get_next2 mbcs_get_next
#else
typedef unsigned int ucs_t;
# define WC_NOT_UNICODE
// which is used to select our mbtoucs rather than system mbrtowc
static int mbcs_get_next2(int c, ucs_t *wc)
{
    int i, res, clen = 1; char s[9];

    s[0] = c;
    /* This assumes (probably OK) that all MBCS embed ASCII as single-byte
       lead bytes, including control chars */
    if((unsigned int) c < 0x80) {
	*wc = (wchar_t) c;
	return 1;
    }
    if(utf8locale) {
	clen = utf8clen(c);
	for(i = 1; i < clen; i++) {
	    c = xxgetc();
	    if(c == R_EOF) error(_("EOF whilst reading MBCS char at line %d"), ParseState.xxlineno);
	    s[i] = (char) c;
	}
	s[clen] ='\0'; /* x86 Solaris requires this */
	res = mbtoucs(wc, s, clen);
	if(res == -1) error(_("invalid multibyte character in parser at line %d"), ParseState.xxlineno);
    } else {
	/* This is not necessarily correct for stateful MBCS */
	while(clen <= MB_CUR_MAX) {
	    res = mbtoucs(wc, s, clen);
	    if(res >= 0) break;
	    if(res == -1)
		error(_("invalid multibyte character in parser at line %d"), ParseState.xxlineno);
	    /* so res == -2 */
	    c = xxgetc();
	    if(c == R_EOF) error(_("EOF whilst reading MBCS char at line %d"), ParseState.xxlineno);
	    s[clen++] = c;
	} /* we've tried enough, so must be complete or invalid by now */
    }
    for(i = clen - 1; i > 0; i--) xxungetc(s[i]);
    return clen;
}
#endif

#define WTEXT_PUSH(c) do { if(wcnt < 10000) wcs[wcnt++] = c; } while(0)

static SEXP mkStringUTF8(const ucs_t *wcs, int cnt)
{
    SEXP t;
    int nb;

/* NB: cnt includes the terminator */
#ifdef Win32
    nb = cnt*4; /* UCS-2/UTF-16 so max 4 bytes per wchar_t */
#else
    nb = cnt*6;
#endif
    R_CheckStack2(nb);
    char s[nb];
    memset(s, 0, nb); /* safety */
    // This used to differentiate WC_NOT_UNICODE but not needed
    wcstoutf8(s, (const wchar_t *)wcs, sizeof(s));
    PROTECT(t = allocVector(STRSXP, 1));
    SET_STRING_ELT(t, 0, mkCharCE(s, CE_UTF8));
    UNPROTECT(1); /* t */
    return t;
}

#define CTEXT_PUSH(c) do { \
	if (ct - currtext >= 1000) { \
	    memmove(currtext, currtext+100, 901); memmove(currtext, "... ", 4); ct -= 100; \
	    currtext_truncated = TRUE; \
	} \
	*ct++ = ((char) c);  \
} while(0)
#define CTEXT_POP() ct--


/* forSymbol is true when parsing backticked symbols */
static int StringValue(int c, Rboolean forSymbol)
{
    int quote = c;
    char currtext[1010], *ct = currtext;
    char st0[MAXELTSIZE];
    unsigned int nstext = MAXELTSIZE;
    char *stext = st0, *bp = st0;
    PROTECT_INDEX sti;
    int wcnt = 0;
    ucs_t wcs[10001];
    Rboolean oct_or_hex = FALSE, use_wcs = FALSE, currtext_truncated = FALSE;

    PROTECT_WITH_INDEX(R_NilValue, &sti);
    CTEXT_PUSH(c);
    while ((c = xxgetc()) != R_EOF && c != quote) {
	CTEXT_PUSH(c);
	if (c == '\n') {
	    xxungetc(c); CTEXT_POP();
	    /* Fix suggested by Mark Bravington to allow multiline strings
	     * by pretending we've seen a backslash. Was:
	     * return ERROR;
	     */
	    c = '\\';
	}
	if (c == '\\') {
	    c = xxgetc(); CTEXT_PUSH(c);
	    if ('0' <= c && c <= '7') {
		int octal = c - '0';
		if ('0' <= (c = xxgetc()) && c <= '7') {
		    CTEXT_PUSH(c);
		    octal = 8 * octal + c - '0';
		    if ('0' <= (c = xxgetc()) && c <= '7') {
			CTEXT_PUSH(c);
			octal = 8 * octal + c - '0';
		    } else {
			xxungetc(c);
			CTEXT_POP();
		    }
		} else {
		    xxungetc(c);
		    CTEXT_POP();
		}
		if (!octal)
		    error(_("nul character not allowed (line %d)"), ParseState.xxlineno);
		c = octal;
		oct_or_hex = TRUE;
	    }
	    else if(c == 'x') {
		int val = 0; int i, ext;
		for(i = 0; i < 2; i++) {
		    c = xxgetc(); CTEXT_PUSH(c);
		    if(c >= '0' && c <= '9') ext = c - '0';
		    else if (c >= 'A' && c <= 'F') ext = c - 'A' + 10;
		    else if (c >= 'a' && c <= 'f') ext = c - 'a' + 10;
		    else {
			xxungetc(c);
			CTEXT_POP();
			if (i == 0) { /* was just \x */
			    *ct = '\0';
			    errorcall(R_NilValue, _("'\\x' used without hex digits in character string starting \"%s\""), currtext);
			}
			break;
		    }
		    val = 16*val + ext;
		}
		if (!val)
		    error(_("nul character not allowed (line %d)"), ParseState.xxlineno);
		c = val;
		oct_or_hex = TRUE;
	    }
	    else if(c == 'u') {
		unsigned int val = 0; int i, ext; 
		Rboolean delim = FALSE;

		if(forSymbol) 
		    error(_("\\uxxxx sequences not supported inside backticks (line %d)"), ParseState.xxlineno);
		if((c = xxgetc()) == '{') {
		    delim = TRUE;
		    CTEXT_PUSH(c);
		} else xxungetc(c);
		for(i = 0; i < 4; i++) {
		    c = xxgetc(); CTEXT_PUSH(c);
		    if(c >= '0' && c <= '9') ext = c - '0';
		    else if (c >= 'A' && c <= 'F') ext = c - 'A' + 10;
		    else if (c >= 'a' && c <= 'f') ext = c - 'a' + 10;
		    else {
			xxungetc(c);
			CTEXT_POP();
			if (i == 0) { /* was just \u */
			    *ct = '\0';
			    errorcall(R_NilValue, _("'\\u' used without hex digits in character string starting \"%s\""), currtext);
			}
			break;
		    }
		    val = 16*val + ext;
		}
		if(delim) {
		    if((c = xxgetc()) != '}')
			error(_("invalid \\u{xxxx} sequence (line %d)"),
			      ParseState.xxlineno);
		    else CTEXT_PUSH(c);
		}
		if (!val)
		    error(_("nul character not allowed (line %d)"), ParseState.xxlineno);
		WTEXT_PUSH(val); /* this assumes wchar_t is Unicode */
		use_wcs = TRUE;
		continue;
	    }
	    else if(c == 'U') {
		unsigned int val = 0; int i, ext;
		Rboolean delim = FALSE;
		if(forSymbol) 
		    error(_("\\Uxxxxxxxx sequences not supported inside backticks (line %d)"), ParseState.xxlineno);
		if((c = xxgetc()) == '{') {
		    delim = TRUE;
		    CTEXT_PUSH(c);
		} else xxungetc(c);
		for(i = 0; i < 8; i++) {
		    c = xxgetc(); CTEXT_PUSH(c);
		    if(c >= '0' && c <= '9') ext = c - '0';
		    else if (c >= 'A' && c <= 'F') ext = c - 'A' + 10;
		    else if (c >= 'a' && c <= 'f') ext = c - 'a' + 10;
		    else {
			xxungetc(c);
			CTEXT_POP();
			if (i == 0) { /* was just \U */
			    *ct = '\0';
			    errorcall(R_NilValue, _("'\\U' used without hex digits in character string starting \"%s\""), currtext);
			}
			break;
		    }
		    val = 16*val + ext;
		}
		if(delim) {
		    if((c = xxgetc()) != '}')
			error(_("invalid \\U{xxxxxxxx} sequence (line %d)"),
			      ParseState.xxlineno);
		    else CTEXT_PUSH(c);
		}
		if (!val)
		    error(_("nul character not allowed (line %d)"),
			  ParseState.xxlineno);
		if (val > 0x10FFFF) {
		    if(delim)
			error(_("invalid \\U{xxxxxxxx} value %6x (line %d)"),
			      val, ParseState.xxlineno);
		    else
			error(_("invalid \\Uxxxxxxxx value %6x (line %d)"),
			      val, ParseState.xxlineno);
		}
#ifdef Win32
		if (0x010000 <= val && val <= 0x10FFFF) {   /* Need surrogate pair in Windows */
		    val = val - 0x010000;
		    WTEXT_PUSH( 0xD800 | (val >> 10) );
		    val = 0xDC00 | (val & 0x03FF);
		}
#endif
		WTEXT_PUSH(val);
		use_wcs = TRUE;
		continue;
	    }
	    else {
		switch (c) {
		case 'a':
		    c = '\a';
		    break;
		case 'b':
		    c = '\b';
		    break;
		case 'f':
		    c = '\f';
		    break;
		case 'n':
		    c = '\n';
		    break;
		case 'r':
		    c = '\r';
		    break;
		case 't':
		    c = '\t';
		    break;
		case 'v':
		    c = '\v';
		    break;
		case '\\':
		    c = '\\';
		    break;
		case '"':
		case '\'':
		case '`':
		case ' ':
		case '\n':
		    break;
		default:
		    *ct = '\0';
		    errorcall(R_NilValue, _("'\\%c' is an unrecognized escape in character string starting \"%s\""), c, currtext);
		}
	    }
	} else if(mbcslocale) {
	    int i, clen;
	    ucs_t wc;
	    clen = mbcs_get_next2(c, &wc);
	    WTEXT_PUSH(wc);
	    ParseState.xxbyteno += clen-1;
	    
	    for(i = 0; i < clen - 1; i++){
		STEXT_PUSH(c);
		c = xxgetc();
		if (c == R_EOF) break;
		CTEXT_PUSH(c);
		if (c == '\n') {
		    xxungetc(c); CTEXT_POP();
		    c = '\\';
		}
	    }
	    if (c == R_EOF) break;
	    STEXT_PUSH(c);
	    continue;
	}
	STEXT_PUSH(c);
	if ((unsigned int) c < 0x80) WTEXT_PUSH(c);
	else { /* have an 8-bit char in the current encoding */
#ifdef WC_NOT_UNICODE
	    ucs_t wc;
	    char s[2] = " ";
	    s[0] = (char) c;
	    mbtoucs(&wc, s, 2);
#else
	    wchar_t wc;
	    char s[2] = " ";
	    s[0] = (char) c;
	    mbrtowc(&wc, s, 2, NULL);
#endif
	    WTEXT_PUSH(wc);
	}
    }
    STEXT_PUSH('\0');
    WTEXT_PUSH(0);
    yytext[0] = '\0';
    if (c == R_EOF) {
	PRESERVE_SV(yylval = R_NilValue);
	UNPROTECT(1); /* release stext */
    	return INCOMPLETE_STRING;
    } else {
    	CTEXT_PUSH(c);
    	CTEXT_PUSH('\0');
    }
    if (!currtext_truncated)
    	strcpy(yytext, currtext);
    else if (forSymbol || !use_wcs) {
        size_t total = strlen(stext);
        snprintf(yytext, MAXELTSIZE, "[%u chars quoted with '%c']", (unsigned int)total, quote);
    } else 
        snprintf(yytext, MAXELTSIZE, "[%d wide chars quoted with '%c']", wcnt, quote);
    if(forSymbol) {
	PRESERVE_SV(yylval = install(stext));
	UNPROTECT(1); /* release stext */
	return SYMBOL;
    } else {
	if(use_wcs) {
	    if(oct_or_hex)
		error(_("mixing Unicode and octal/hex escapes in a string is not allowed"));
	    if(wcnt < 10000)
		PRESERVE_SV(yylval = mkStringUTF8(wcs, wcnt)); /* include terminator */
	    else
		error(_("string at line %d containing Unicode escapes not in this locale\nis too long (max 10000 chars)"), ParseState.xxlineno);
	} else
	    PRESERVE_SV(yylval = mkString2(stext,  bp - stext - 1, oct_or_hex));
	UNPROTECT(1); /* release stext */
	return STR_CONST;
    }
}

static int RawStringValue(int c0, int c)
{
    int quote = c;
    int delim = ')';
    char currtext[1010], *ct = currtext;
    char st0[MAXELTSIZE];
    unsigned int nstext = MAXELTSIZE;
    char *stext = st0, *bp = st0;
    PROTECT_INDEX sti;
    int wcnt = 0;
    ucs_t wcs[10001];
    Rboolean oct_or_hex = FALSE, use_wcs = FALSE, currtext_truncated = FALSE;

    CTEXT_PUSH(c0); /* 'r' or 'R' */
    CTEXT_PUSH(c);  /* opening quote */

    /* count dashes between the opening quote and opening delimiter */
    int ndash = 0;
    while (nextchar('-')) { CTEXT_PUSH('-'); ndash++; }

    c = xxgetc();
    CTEXT_PUSH(c);
    switch(c) {
    case '(': delim = ')'; break;
    case '[': delim = ']'; break;
    case '{': delim = '}'; break;
    case '|': delim = '|'; break;
    default:
	error(_("malformed raw string literal at line %d"),
	      ParseState.xxlineno);
    }

    PROTECT_WITH_INDEX(R_NilValue, &sti);
    while ((c = xxgetc()) != R_EOF) {
	if (c == delim) {
	    /* count the dashes after the closing delimiter */
	    int nd = 0;
	    while (nd < ndash && nextchar('-')) nd++;
	    
	    if (nd == ndash && nextchar(quote))
		/* right number of dashes, right quote: were done! */
		break;
	    else {
		/* not done: emit closing delimiter, dashes, and continue */
		CTEXT_PUSH(delim);
		STEXT_PUSH(delim);
		WTEXT_PUSH(delim);
		for (int i = 0; i < nd; i++) {
		    CTEXT_PUSH('-');
		    STEXT_PUSH('-');
		    WTEXT_PUSH('-');
		}
		continue;
	    }
	}
	CTEXT_PUSH(c);
	if(mbcslocale) {
	    int i, clen;
	    ucs_t wc;
	    clen = mbcs_get_next2(c, &wc);
	    WTEXT_PUSH(wc);
	    ParseState.xxbyteno += clen-1;
	    
	    for(i = 0; i < clen - 1; i++){
		STEXT_PUSH(c);
		c = xxgetc();
		if (c == R_EOF) break;
		CTEXT_PUSH(c);
	    }
	    if (c == R_EOF) break;
	    STEXT_PUSH(c);
	    continue;
	}
	STEXT_PUSH(c);
	if ((unsigned int) c < 0x80) WTEXT_PUSH(c);
	else { /* have an 8-bit char in the current encoding */
#ifdef WC_NOT_UNICODE
	    ucs_t wc;
	    char s[2] = " ";
	    s[0] = (char) c;
	    mbtoucs(&wc, s, 2);
#else
	    wchar_t wc;
	    char s[2] = " ";
	    s[0] = (char) c;
	    mbrtowc(&wc, s, 2, NULL);
#endif
	    WTEXT_PUSH(wc);
	}
    }
    STEXT_PUSH('\0');
    WTEXT_PUSH(0);
    yytext[0] = '\0';
    if (c == R_EOF) {
	PRESERVE_SV(yylval = R_NilValue);
	UNPROTECT(1); /* release stext */
    	return INCOMPLETE_STRING;
    } else {
	/* record delim, dashes, and quote, and terminate string */
	CTEXT_PUSH(delim);
	for (int i = 0; i < ndash; i++)
	    CTEXT_PUSH('-');
	CTEXT_PUSH(quote);
    	CTEXT_PUSH('\0');
    }
    if (!currtext_truncated)
    	strcpy(yytext, currtext);
    else if (!use_wcs) {
        size_t total = strlen(stext);
        snprintf(yytext, MAXELTSIZE, "[%u chars quoted with '%c']", (unsigned int)total, quote);
    } else 
        snprintf(yytext, MAXELTSIZE, "[%d wide chars quoted with '%c']", wcnt, quote);
    if(use_wcs) {
	if(oct_or_hex)
	    error(_("mixing Unicode and octal/hex escapes in a string is not allowed"));
	if(wcnt < 10000)
	    PRESERVE_SV(yylval = mkStringUTF8(wcs, wcnt)); /* include terminator */
	else
	    error(_("string at line %d containing Unicode escapes not in this locale\nis too long (max 10000 chars)"), ParseState.xxlineno);
    } else
	PRESERVE_SV(yylval = mkString2(stext,  bp - stext - 1, oct_or_hex));
    UNPROTECT(1); /* release stext */
    return STR_CONST;
}

static int SpecialValue(int c)
{
    DECLARE_YYTEXT_BUFP(yyp);
    YYTEXT_PUSH(c, yyp);
    while ((c = xxgetc()) != R_EOF && c != '%') {
	if (c == '\n') {
	    xxungetc(c);
	    return ERROR;
	}
	YYTEXT_PUSH(c, yyp);
    }
    if (c == '%')
	YYTEXT_PUSH(c, yyp);
    YYTEXT_PUSH('\0', yyp);
    yylval = install(yytext);
    return SPECIAL;
}

/* return 1 if name is a valid name 0 otherwise */
attribute_hidden
int isValidName(const char *name)
{
    const char *p = name;
    int i;

    if(mbcslocale) {
	/* the only way to establish which chars are alpha etc is to
	   use the wchar variants */
	size_t n = strlen(name), used;
	wchar_t wc;
	used = Mbrtowc(&wc, p, n, NULL); p += used; n -= used;
	if(used == 0) return 0;
	if (wc != L'.' && !iswalpha(wc) ) return 0;
	if (wc == L'.') {
	    /* We don't care about other than ASCII digits */
	    if(isdigit(0xff & (int)*p)) return 0;
	    /* Mbrtowc(&wc, p, n, NULL); if(iswdigit(wc)) return 0; */
	}
	while((used = Mbrtowc(&wc, p, n, NULL))) {
	    if (!(iswalnum(wc) || wc == L'.' || wc == L'_')) break;
	    p += used; n -= used;
	}
	if (*p != '\0') return 0;
    } else {
	int c = 0xff & *p++;
	if (c != '.' && !isalpha(c) ) return 0;
	if (c == '.' && isdigit(0xff & (int)*p)) return 0;
	while ( c = 0xff & *p++, (isalnum(c) || c == '.' || c == '_') ) ;
	if (c != '\0') return 0;
    }

    if (strcmp(name, "...") == 0) return 1;

    for (i = 0; keywords[i].name != NULL; i++)
	if (strcmp(keywords[i].name, name) == 0) return 0;

    return 1;
}


static int SymbolValue(int c)
{
    int kw;
    DECLARE_YYTEXT_BUFP(yyp);
    if(mbcslocale) {
	// FIXME potentially need R_wchar_t with UTF-8 Windows.
	wchar_t wc; int i, clen;
	clen = mbcs_get_next(c, &wc);
	while(1) {
	    /* at this point we have seen one char, so push its bytes
	       and get one more */
	    for(i = 0; i < clen; i++) {
		YYTEXT_PUSH(c, yyp);
		c = xxgetc();
	    }
	    if(c == R_EOF) break;
	    if(c == '.' || c == '_') {
		clen = 1;
		continue;
	    }
	    clen = mbcs_get_next(c, &wc);
	    if(!iswalnum(wc)) break;
	}
    } else
	do {
	    YYTEXT_PUSH(c, yyp);
	} while ((c = xxgetc()) != R_EOF &&
		 (isalnum(c) || c == '.' || c == '_'));
    xxungetc(c);
    YYTEXT_PUSH('\0', yyp);
    if ((kw = KeywordLookup(yytext))) 
	return kw;
    
    PRESERVE_SV(yylval = install(yytext));
    return SYMBOL;
}

static void setParseFilename(SEXP newname) {
    SEXP class;
    
    if (isEnvironment(PS_SRCFILE)) {
	SEXP oldname = findVar(install("filename"), PS_SRCFILE);
    	if (isString(oldname) && length(oldname) > 0 &&
    	    strcmp(CHAR(STRING_ELT(oldname, 0)),
    	           CHAR(STRING_ELT(newname, 0))) == 0) return;
	PS_SET_SRCFILE(NewEnvironment(R_NilValue, R_NilValue, R_EmptyEnv));
	defineVar(install("filename"), newname, PS_SRCFILE);
	defineVar(install("original"), PS_ORIGINAL, PS_SRCFILE);

	PROTECT(class = allocVector(STRSXP, 2));
	SET_STRING_ELT(class, 0, mkChar("srcfilealias"));
	SET_STRING_ELT(class, 1, mkChar("srcfile"));
	setAttrib(PS_SRCFILE, R_ClassSymbol, class);
	UNPROTECT(1); /* class */
    } else 
	PS_SET_SRCFILE(duplicate(newname));
    RELEASE_SV(newname);
}

static int processLineDirective(int *type)
{
    int c, tok, linenumber;
    c = SkipSpace();
    if (!isdigit(c)) return(c);
    tok = NumericValue(c);
    linenumber = atoi(yytext);
    c = SkipSpace();
    if (c == '"') 
	tok = StringValue(c, FALSE);
    else
    	xxungetc(c);
    if (tok == STR_CONST) 
	setParseFilename(yylval);
    while ((c = xxgetc()) != '\n' && c != R_EOF) /* skip */ ;
    ParseState.xxlineno = linenumber;
    *type = LINE_DIRECTIVE;
    /* we don't change xxparseno here:  it counts parsed lines, not official lines */
    R_ParseContext[R_ParseContextLast] = '\0';  /* Context report shouldn't show the directive */
    return(c);
}

/* Get the R symbol, and set yytext at the same time */
static SEXP install_and_save(char * text)
{
    strcpy(yytext, text);
    return install(text);
}

/* Get an R symbol, and set different yytext.  Used for translation of -> to <-. ->> to <<- */
static SEXP install_and_save2(char * text, char * savetext)
{
    strcpy(yytext, savetext);
    return install(text);
}


/* Split the input stream into tokens. */
/* This is the lowest of the parsing levels. */

static int token(void)
{
    int c;
    wchar_t wc;

    if (SavedToken) {
	c = SavedToken;
	yylval = SavedLval;
	SavedLval = R_NilValue;
	SavedToken = 0;
	yylloc.first_line = xxlinesave;
	yylloc.first_column = xxcolsave;
	yylloc.first_byte = xxbytesave;
	yylloc.first_parsed = xxparsesave;
	return c;
    }
    xxcharsave = xxcharcount; /* want to be able to go back one token */

    c = SkipSpace();
    if (c == '#') c = SkipComment();

    yylloc.first_line = ParseState.xxlineno;
    yylloc.first_column = ParseState.xxcolno;
    yylloc.first_byte = ParseState.xxbyteno;
    yylloc.first_parsed = ParseState.xxparseno;

    if (c == R_EOF) return END_OF_INPUT;

    /* Either digits or symbols can start with a "." */
    /* so we need to decide which it is and jump to  */
    /* the correct spot. */

    if (c == '.' && typeofnext() >= 2) goto symbol;

    /* literal numbers */

    if (c == '.') return NumericValue(c);
    /* We don't care about other than ASCII digits */
    if (isdigit(c)) return NumericValue(c);

    /* raw string literal */

    if (c == 'r' || c == 'R') {
	if (nextchar('"'))
	    return RawStringValue(c, '"');
	else if (nextchar('\''))
	    return RawStringValue(c, '\'');
    }

    /* literal strings */

    if (c == '\"' || c == '\'')
	return StringValue(c, FALSE);

    /* special functions */

    if (c == '%')
	return SpecialValue(c);

    /* functions, constants and variables */

    if (c == '`')
	return StringValue(c, TRUE);
 symbol:

    if (c == '.') return SymbolValue(c);
    if(mbcslocale) {
	// FIXME potentially need R_wchar_t with UTF-8 Windows.
	mbcs_get_next(c, &wc);
	if (iswalpha(wc)) return SymbolValue(c);
    } else
	if (isalpha(c)) return SymbolValue(c);

    /* compound tokens */

    switch (c) {
    case '<':
	if (nextchar('=')) {
	    yylval = install_and_save("<=");
	    return LE;
	}
	if (nextchar('-')) {
	    yylval = install_and_save("<-");
	    return LEFT_ASSIGN;
	}
	if (nextchar('<')) {
	    if (nextchar('-')) {
		yylval = install_and_save("<<-");
		return LEFT_ASSIGN;
	    }
	    else
		return ERROR;
	}
	yylval = install_and_save("<");
	return LT;
    case '-':
	if (nextchar('>')) {
	    if (nextchar('>')) {
		yylval = install_and_save2("<<-", "->>");
		return RIGHT_ASSIGN;
	    }
	    else {
		yylval = install_and_save2("<-", "->");
		return RIGHT_ASSIGN;
	    }
	}
	yylval = install_and_save("-");
	return '-';
    case '>':
	if (nextchar('=')) {
	    yylval = install_and_save(">=");
	    return GE;
	}
	yylval = install_and_save(">");
	return GT;
    case '!':
	if (nextchar('=')) {
	    yylval = install_and_save("!=");
	    return NE;
	}
	yylval = install_and_save("!");
	return '!';
    case '=':
	if (nextchar('=')) {
	    yylval = install_and_save("==");
	    return EQ;
	}
	else if (nextchar('>')) {
	    yylval = install_and_save("=>");
	    return PIPEBIND;
	}		 
	yylval = install_and_save("=");
	return EQ_ASSIGN;
    case ':':
	if (nextchar(':')) {
	    if (nextchar(':')) {
		yylval = install_and_save(":::");
		return NS_GET_INT;
	    }
	    else {
		yylval = install_and_save("::");
		return NS_GET;
	    }
	}
	if (nextchar('=')) {
	    yylval = install_and_save(":=");
	    return LEFT_ASSIGN;
	}
	yylval = install_and_save(":");
	return ':';
    case '&':
	if (nextchar('&')) {
	    yylval = install_and_save("&&");
	    return AND2;
	}
	yylval = install_and_save("&");
	return AND;
    case '|':
	if (nextchar('|')) {
	    yylval = install_and_save("||");
	    return OR2;
	}
	else if (nextchar('>')) {
	    yylval = install_and_save("|>");
	    return PIPE;
	}
	yylval = install_and_save("|");
	return OR;
    case LBRACE:
	yylval = install_and_save("{");
	return c;
    case RBRACE:
        strcpy(yytext, "}");
	return c;
    case '(':
	yylval = install_and_save("(");
	return c;
    case ')':
        strcpy(yytext, ")");
	return c;
    case '[':
	if (nextchar('[')) {
	    yylval = install_and_save("[[");
	    return LBB;
	}
	yylval = install_and_save("[");
	return c;
    case ']':
        strcpy(yytext, "]");
	return c;
    case '?':
	yylval = install_and_save("?");
	return c;
    case '*':
	/* Replace ** by ^.  This has been here since 1998, but is
	   undocumented (at least in the obvious places).  It is in
	   the index of the Blue Book with a reference to p. 431, the
	   help for 'Deprecated'.  S-PLUS 6.2 still allowed this, so
	   presumably it was for compatibility with S. */
	if (nextchar('*')) {
	    yylval = install_and_save2("^", "**");
	    return '^';
	} else
	    yylval = install_and_save("*");
	return c;
    case '+':
    case '/':
    case '^':
    case '~':
    case '$':
    case '@':
    case '\\':
	yytext[0] = (char) c;
	yytext[1] = '\0';
	yylval = install(yytext);
	return c;
    default:
        yytext[0] = (char) c;
        yytext[1] = '\0';
	return c;
    }
}

/**
 * Sets the first elements of the yyloc structure with current 
 * information
 */
static void setfirstloc(void)
{
    yylloc.first_line   = ParseState.xxlineno;
    yylloc.first_column = ParseState.xxcolno;
    yylloc.first_byte   = ParseState.xxbyteno;
    yylloc.first_parsed = ParseState.xxparseno;
}

static void setlastloc(void)
{
    yylloc.last_line = ParseState.xxlineno;
    yylloc.last_column = ParseState.xxcolno;
    yylloc.last_byte = ParseState.xxbyteno;
    yylloc.last_parsed = ParseState.xxparseno;
}

/**
 * Wrap around the token function. Returns the same result
 * but increments the identifier, after a call to token_, 
 * the identifier variable contains the id of the token
 * just returned
 *
 * @return the same as token
 */

static int token_(void){
    // capture the position before retrieving the token
    setfirstloc( ) ;

    // get the token
    int res = token( ) ;

    // capture the position after
    int _last_col  = ParseState.xxcolno ;
    int _last_parsed = ParseState.xxparseno ;

    _current_token = res ;
    incrementId( ) ;
    yylloc.id = identifier ;

    // record the position
    if( res != '\n' && res != END_OF_INPUT)
	record_( yylloc.first_parsed, yylloc.first_column, 
	         _last_parsed, _last_col,
		res, identifier, yytext );

    return res; 
}


static int yylex(void)
{
    int tok;

 again:

    tok = token_();

    /* Newlines must be handled in a context */
    /* sensitive way.  The following block of */
    /* deals directly with newlines in the */
    /* body of "if" statements. */

    if (tok == '\n') {

	if (EatLines || *contextp == '[' || *contextp == '(')
	    goto again;

	/* The essence of this is that in the body of */
	/* an "if", any newline must be checked to */
	/* see if it is followed by an "else". */
	/* such newlines are discarded. */

	if (*contextp == 'i') {

	    /* Find the next non-newline token */

	    while(tok == '\n')
		tok = token_();

	    /* If we encounter "}", ")" or "]" then */
	    /* we know that all immediately preceding */
	    /* "if" bodies have been terminated. */
	    /* The corresponding "i" values are */
	    /* popped off the context stack. */

	    if (tok == RBRACE || tok == ')' || tok == ']' ) {
		while (*contextp == 'i')
		    ifpop();
		*contextp-- = 0;
		setlastloc();
		return tok;
	    }

	    /* When a "," is encountered, it terminates */
	    /* just the immediately preceding "if" body */
	    /* so we pop just a single "i" of the */
	    /* context stack. */

	    if (tok == ',') {
		ifpop();
		setlastloc();
		return tok;
	    }

	    /* Tricky! If we find an "else" we must */
	    /* ignore the preceding newline.  Any other */
	    /* token means that we must return the newline */
	    /* to terminate the "if" and "push back" that */
	    /* token so that we will obtain it on the next */
	    /* call to token.  In either case sensitivity */
	    /* is lost, so we pop the "i" from the context */
	    /* stack. */

	    if(tok == ELSE) {
		EatLines = 1;
		ifpop();
		setlastloc();
		return ELSE;
	    }
	    else {
		ifpop();
		SavedToken = tok;
		xxlinesave = yylloc.first_line;
		xxcolsave  = yylloc.first_column;
		xxbytesave = yylloc.first_byte;
		xxparsesave = yylloc.first_parsed;
		SavedLval = yylval;
		setlastloc();
		if (ParseState.keepSrcRefs && ParseState.keepParseData &&
		    yytext[0])

		    /* unrecord the pushed back token if not null */
		    ParseState.data_count--;
		return '\n';
	    }
	}
	else {
	    setlastloc();
	    return '\n';
	}
    }

    /* Additional context sensitivities */

    switch(tok) {

	/* Any newlines immediately following the */
	/* the following tokens are discarded. The */
	/* expressions are clearly incomplete. */

    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
    case LT:
    case LE:
    case GE:
    case GT:
    case EQ:
    case NE:
    case OR:
    case AND:
    case OR2:
    case AND2:
    case PIPE:
    case PIPEBIND:
    case SPECIAL:
    case FUNCTION:
    case WHILE:
    case REPEAT:
    case FOR:
    case IN:
    case '?':
    case '!':
    case '=':
    case ':':
    case '~':
    case '$':
    case '@':
    case LEFT_ASSIGN:
    case RIGHT_ASSIGN:
    case EQ_ASSIGN:
	EatLines = 1;
	break;

	/* Push any "if" statements found and */
	/* discard any immediately following newlines. */

    case IF:
	IfPush();
	EatLines = 1;
	break;

	/* Terminate any immediately preceding "if" */
	/* statements and discard any immediately */
	/* following newlines. */

    case ELSE:
	ifpop();
	EatLines = 1;
	break;

	/* These tokens terminate any immediately */
	/* preceding "if" statements. */

    case ';':
    case ',':
	ifpop();
	break;

	/* Any newlines following these tokens can */
	/* indicate the end of an expression. */

    case SYMBOL:
    case STR_CONST:
    case NUM_CONST:
    case NULL_CONST:
    case NEXT:
    case BREAK:
	EatLines = 0;
	break;

	/* Handle brackets, braces and parentheses */

    case LBB:
	if(contextp - contextstack >= CONTEXTSTACK_SIZE - 1)
	    error(_("contextstack overflow at line %d"), ParseState.xxlineno);
	*++contextp = '[';
	*++contextp = '[';
	break;

    case '[':
	if(contextp - contextstack >= CONTEXTSTACK_SIZE)
	    error(_("contextstack overflow at line %d"), ParseState.xxlineno);
	*++contextp = (char) tok;
	break;

    case LBRACE:
	if(contextp - contextstack >= CONTEXTSTACK_SIZE)
	    error(_("contextstack overflow at line %d"), ParseState.xxlineno);
	*++contextp = (char) tok;
	EatLines = 1;
	break;

    case '(':
	if(contextp - contextstack >= CONTEXTSTACK_SIZE)
	    error(_("contextstack overflow at line %d"), ParseState.xxlineno);
	*++contextp = (char) tok;
	break;

    case ']':
	while (*contextp == 'i')
	    ifpop();
	*contextp-- = 0;
	EatLines = 0;
	break;

    case RBRACE:
	while (*contextp == 'i')
	    ifpop();
	*contextp-- = 0;
	break;

    case ')':
	while (*contextp == 'i')
	    ifpop();
	*contextp-- = 0;
	EatLines = 0;
	break;

    }
    setlastloc();
    return tok;
}
/**
 * Records location information about a symbol. The information is
 * used to fill the data 
 * 
 */
static void record_( int first_parsed, int first_column, int last_parsed, int last_column,
	int token, int id, char* text_in ){
	
	if (!ParseState.keepSrcRefs || !ParseState.keepParseData
	    || id == NA_INTEGER) return;
	
	// don't care about zero sized things
	if( !yytext[0] ) return ;
	
	if (ParseState.data_count == DATA_COUNT)
	    growData();
	
	_FIRST_COLUMN( ParseState.data_count ) = first_column; 
	_FIRST_PARSED( ParseState.data_count ) = first_parsed;
	_LAST_COLUMN( ParseState.data_count )  = last_column;  
	_LAST_PARSED( ParseState.data_count )  = last_parsed; 
	_TOKEN( ParseState.data_count )        = token;        
	_ID( ParseState.data_count )           = id ;          
	_PARENT(ParseState.data_count)         = 0 ; 
	if ( text_in )
	    SET_STRING_ELT(PS_TEXT, ParseState.data_count, mkChar2(text_in));
	else
	    SET_STRING_ELT(PS_TEXT, ParseState.data_count, mkChar(""));
	
	if( id > ID_COUNT )
	    growID(id) ;

	ID_ID( id ) = ParseState.data_count ; 
	
	ParseState.data_count++ ;
}

/**
 * records parent as the parent of all its childs. This grows the 
 * parents list with a new vector. The first element of the new 
 * vector is the parent id, and other elements are childs id
 *
 * @param parent id of the parent expression
 * @param childs array of location information for all child symbols
 * @param nchilds number of childs
 */
static void recordParents( int parent, yyltype * childs, int nchilds){
	
	if( parent > ID_COUNT ){
		growID(parent) ;
	}
	
	/* some of the childs might be an empty token (like cr)
	   which we do not want to track */
	int ii;    /* loop index */
	yyltype loc ;
	for( ii=0; ii<nchilds; ii++){
		loc = childs[ii] ;
		if( loc.id == NA_INTEGER || (loc.first_line == loc.last_line && loc.first_byte > loc.last_byte) )
			continue ;
		/*  This shouldn't happen... */
		if (loc.id < 0 || loc.id > identifier) {
		    error(_("internal parser error at line %d"),  ParseState.xxlineno);
		}
		ID_PARENT( loc.id ) = parent;
	}
	
}

/**
 * The token pointed by the location has the wrong token type, 
 * This updates the type
 *
 * @param loc location information for the token to track
 */ 
static void modif_token( yyltype* loc, int tok ){
	
	int id = loc->id ;
	
	if (!ParseState.keepSrcRefs || !ParseState.keepParseData
	    || id < 0 || id > ID_COUNT) return;
	    
	if( tok == SYMBOL_FUNCTION_CALL ){
		// looking for first child of id
		int j = ID_ID( id ) ;
		int parent = id ;
		
		if (j < 0 || j > ID_COUNT)
	            return;
	            
		while( ID_PARENT( _ID(j) ) != parent ){
		    j-- ; 
		    if (j < 0)
	        	return;
		}
			
		if( _TOKEN(j) == SYMBOL ){
		    _TOKEN(j) = SYMBOL_FUNCTION_CALL ;
		}
		
	} else{
		_TOKEN( ID_ID(id) ) = tok ;
	}
	
}

/* this local version of lengthgets() always copies and doesn't fill with NA */
static SEXP lengthgets2(SEXP x, int len) {
    SEXP result;
    PROTECT(result = allocVector( TYPEOF(x), len ));
    
    len = (len < length(x)) ? len : length(x);
    switch(TYPEOF(x)) {
    	case INTSXP: 
    	    for (int i = 0; i < len; i++)
    	    	INTEGER(result)[i] = INTEGER(x)[i];
	    for (int i = len; i < length(result); i++)
		INTEGER(result)[i] = 0;
    	    break;
    	case STRSXP:
    	    for (int i = 0; i < len; i++)
    	    	SET_STRING_ELT(result, i, STRING_ELT(x, i));
    	    break;
    	default:
	    UNIMPLEMENTED_TYPE("lengthgets2", x);
    }
    UNPROTECT(1); /* result */
    return result;
}

static void finalizeData( ){
	
    int nloc = ParseState.data_count ;

    int i, j, id ;
    int parent ;

    /* store parents in the data */
    for( i=0; i<nloc; i++){
	id = _ID(i);
	parent = ID_PARENT( id ) ;
	while( parent != 0 && ID_ID(parent) == 0 )
	    parent = ID_PARENT( parent ) ;
	_PARENT(i) = parent ;

#define FD_FAST_UPDATE_PARENTS
#ifdef FD_FAST_UPDATE_PARENTS
	/*
	   With long generated expressions, updating the parents can take
	   a lot of time due to long chains of nodes not represented in the
	   parse data. To reduce the overhead somewhat, we create shortcuts
	   in the IDS array to point directly to the parent that is in the
	   parse data.
	*/
	int data_parent = parent;
	parent = ID_PARENT( id ) ;
	while( parent != data_parent ){
	    ID_PARENT( id ) = data_parent; /* set shortcut */
	    id = parent;
	    parent = ID_PARENT( parent );
	}
#endif
    }

    /* attach comments to closest enclosing symbol */
    /* not updating ID_PARENT anymore */

#define FD_FAST_ASSIGN_COMMENTS
#ifdef FD_FAST_ASSIGN_COMMENTS
    /*
       All terminals (tokens) are ordered by start and end location, including
       the comments, in the data.

       All non-terminals, including to be found parents of the comments, are
       ordered by their end location. When they have the same end location
       in the code, they are ordered by their decreasing start location
       (children before parents).

       All terminals and non-terminals are also before their parents (if any),
       so a comment is also befor its parent in the data.

       Consequently: the first non-terminal after a comment that encloses the
       comment is its (immediate) parent. The original algorithm for every
       comment linearly searches for the first enclosing non-terminal and
       returns it, but it has quadratic complexity and dominates the whole
       parsing for long inputs (used when FD_FAST_ASSIGN_COMMENTS is not
       defined).

       This algorithm uses the parental information available on nodes that
       follow the comments. That information has been filled by the parser
       during reductions (but not for comments, because those are not in the
       grammar). A node following a comment is either the parent of the
       comment, or some of its parents are, or is an orphan.

       Note that a non-terminal may end before a terminal (e.g. comment) in the
       code but be after the terminal in the data (due to look-ahead). It seems
       that the parent of the comment has to be within parents of the
       non-terminal as well, but I am not sure how to prove it, so the algorithm
       just skips non-terminals preceding the comment in the code (so is not
       strictly linear).
      */

    for(i = nloc-1; i >= 0; i--) {
	if (_TOKEN(i) == COMMENT) {
	    int orphan = 1;
	    int istartl = _FIRST_PARSED(i);
	    int istartc = _FIRST_COLUMN(i);

	    /* look for first node j that does not end before the comment i */
	    for(j = i + 1; j < nloc && _LAST_PARSED(j) <= istartl; j++);

	    if (j < nloc) {
		for(;;) {
		    int jstartl = _FIRST_PARSED(j);
		    int jstartc = _FIRST_COLUMN(j);

		    if (jstartl < istartl || (jstartl == istartl
		                              && jstartc <= istartc)) {
			/* j starts before or at the comment */
			_PARENT(i) = _ID(j);
			orphan = 0;
			break;
		    }
		    /* find parent of j */
		    int jparent = _PARENT(j);
		    if (jparent == 0)
			break; /* orphan */
		    j = ID_ID(jparent);
		}
	    }
	    if (orphan)
		_PARENT(i) = 0;
	}
    }
#else
    /* the original algorithm, which is slow for large inputs */

    int comment_line, comment_first_col;
    int this_first_parsed, this_last_parsed, this_first_col ;
    int orphan ;

    for( i=0; i<nloc; i++){
	if( _TOKEN(i) == COMMENT ){
	    comment_line = _FIRST_PARSED( i ) ;
	    comment_first_col = _FIRST_COLUMN( i ) ;

	    orphan = 1 ;
	    for( j=i+1; j<nloc; j++){
		this_first_parsed = _FIRST_PARSED( j ) ;
		this_first_col = _FIRST_COLUMN( j ) ;
		this_last_parsed  = _LAST_PARSED( j ) ;

		/* the comment needs to start after the current symbol */
		if( comment_line < this_first_parsed ) continue ;
		if( (comment_line == this_first_parsed) & (comment_first_col < this_first_col) ) continue ;

		/* the current symbol must finish after the comment */
		if( this_last_parsed <= comment_line ) continue ; 

		/* we have a match, record the parent and stop looking */
		_PARENT(i) = _ID(j);
		orphan = 0;
		break ;
	    }
	    if(orphan){
		_PARENT(i) = 0 ;
	    }
	}
    }
#endif


    /* now rework the parents of comments, we try to attach 
    comments that are not already attached (parent=0) to the next
    enclosing top-level expression */ 

    for( i=0; i<nloc; i++){
	int token = _TOKEN(i); 
	if( token == COMMENT && _PARENT(i) == 0 ){
	    for( j=i; j<nloc; j++){
		int token_j = _TOKEN(j); 
		if( token_j == COMMENT ) continue ;
		if( _PARENT(j) != 0 ) continue ;
		_PARENT(i) = - _ID(j) ;
		break ;
	    }
	}
    }

    /* attach the token names as an attribute so we don't need to switch to a dataframe, and decide on terminals */
    SEXP tokens;
    PROTECT(tokens = allocVector( STRSXP, nloc ) );
    for (int i=0; i<nloc; i++) {
        int token = _TOKEN(i);
        int xlat = yytranslate[token];
        if (xlat == 2) /* "unknown" */
            xlat = token;
        if (xlat < YYNTOKENS + YYNNTS)
    	    SET_STRING_ELT(tokens, i, mkChar(yytname[xlat]));
    	else { /* we have a token which doesn't have a name, e.g. an illegal character as in PR#15518 */
    	    char name[2];
    	    name[0] = (char) xlat;
    	    name[1] = 0;
    	    SET_STRING_ELT(tokens, i, mkChar(name));
    	}
    	_TERMINAL(i) = xlat < YYNTOKENS;
    }
    SEXP dims, newdata, newtext;
    if (nloc) {
	PROTECT( newdata = lengthgets2(PS_DATA, nloc * DATA_ROWS));
	PROTECT( newtext = lengthgets2(PS_TEXT, nloc));
    } else {
	PROTECT( newdata = allocVector( INTSXP, 0));
	PROTECT( newtext = allocVector( STRSXP, 0));
    }
    PROTECT( dims = allocVector( INTSXP, 2 ) ) ;
    INTEGER(dims)[0] = DATA_ROWS ;
    INTEGER(dims)[1] = nloc ;
    setAttrib( newdata, install( "dim" ), dims ) ;
    setAttrib( newdata, install("tokens"), tokens );
    setAttrib( newdata, install("text"), newtext );
    
    setAttrib(newdata, R_ClassSymbol, mkString("parseData"));
    
    /* Put it into the srcfile environment */
    if (isEnvironment(PS_SRCFILE))
	defineVar(install("parseData"), newdata, PS_SRCFILE);
    UNPROTECT(4); /* tokens, newdata, newtext, dims */
}

/**
 * Grows the data
 */
static void growData(){
	
    int new_data_count;	
    if (PS_DATA == R_NilValue) {
        new_data_count = INIT_DATA_COUNT;
	PS_SET_DATA(allocVector(INTSXP, 0));
	PS_SET_TEXT(allocVector(STRSXP, 0));
    } else
        new_data_count = 2*DATA_COUNT;
	
    PS_SET_DATA(lengthgets2(PS_DATA, new_data_count * DATA_ROWS));
    PS_SET_TEXT(lengthgets2(PS_TEXT, new_data_count));
}

/**
 * Grows the ids vector so that ID_ID(target) can be called
 */
static void growID( int target ){
	
    int new_count;
    if (PS_IDS == R_NilValue) {
        new_count = INIT_DATA_COUNT/2 - 1;
        PS_SET_IDS(allocVector(INTSXP, 0));
    } else
    	new_count = ID_COUNT;
    	
    while (target > new_count)
    	new_count = 2*new_count + 1;
    	
    if (new_count <= ID_COUNT)
    	return;
    
    int new_size = (1 + new_count)*2;
    PS_SET_IDS(lengthgets2(PS_IDS, new_size));
}

static int checkForPlaceholder(SEXP placeholder, SEXP arg)
{
    if (arg == placeholder)
	return TRUE;
    else if (TYPEOF(arg) == LANGSXP)
	for (SEXP cur = arg; cur != R_NilValue; cur = CDR(cur))
	    if (checkForPlaceholder(placeholder, CAR(cur)))
		return TRUE;
    return FALSE;
}

static void NORET signal_ph_error(SEXP rhs, SEXP ph) {
    errorcall(rhs, _("pipe placeholder must only appear as a top-level "
		     "argument in the RHS call"));
}
    
static SEXP findPlaceholderCell(SEXP placeholder, SEXP rhs)
{
    SEXP phcell = NULL;
    int count = 0;
    if (checkForPlaceholder(placeholder, CAR(rhs)))
	signal_ph_error(rhs, placeholder);
    for (SEXP a = CDR(rhs); a != R_NilValue; a = CDR(a))
	if (CAR(a) == placeholder) {
	    if (phcell == NULL)
		phcell = a;
	    count++;
	}
	else if (checkForPlaceholder(placeholder, CAR(a)))
	    signal_ph_error(rhs, placeholder);
    if (count > 1)
	errorcall(rhs, _("pipe placeholder may only appear once"));
    return phcell;
}
