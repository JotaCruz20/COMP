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
#line 1 "uccompiler.y" /* yacc.c:339  */

    /*
        João Alexandre Santos Cruz 2018288464
        André Cristóvão Ferreira da Silva  2018277921
    */

    #include <stdio.h>
    #include <stdlib.h>  
    #include <string.h>  
    int yylex(void);
    void yyerror (char * s);



    typedef struct no{
        char * id;
        char * type;
        struct no * noFilho; // cada no tem 1 filho e 1 irmao(podem ser null)
        struct no * noIrmao; // facilita, pois so temos de ter 1 pointer para o filho
    } no;                    // e o filho terá pointer para os irmaos (nao e necessario array de ponteiros)

    int errorFlag = 0;
    int counter = 0;
    extern int flag;

    char * auxType;
    no * noAuxiliar;

    no * inserirNo(char * id, char * type, no * noFilho){
        no * noAtual = (no *)malloc(sizeof(no));
        if(type!=NULL){
                noAtual->type =(char *)malloc(sizeof(char)*strlen(type));
                strcpy(noAtual->type,type);
        }
        else{
               noAtual->type=NULL; 
        }
        if(id!=NULL){
                noAtual->id =(char *)malloc(sizeof(char)*strlen(id));
                strcpy(noAtual->id,id);
        }
        else{
                noAtual->id = NULL;
        }
        noAtual->noFilho = noFilho;
        noAtual->noIrmao = NULL;
    return noAtual;
    }

    void addIrmao(no * no1, no * no2)
    {
        no1->noIrmao = no2;
    }

    void printTree(no * auxNode, int pontos) /* mudar isto */
    {
        int i=0;
        if(auxNode!=NULL){
                for(i=0;i<pontos*2;i++){
                        printf(".");
                }
                if(auxNode->id!=NULL){
                        printf("%s(%s)\n", auxNode->type,auxNode->id);
                }
                else{
                         printf("%s\n", auxNode->type);
                }
                if(auxNode->noFilho!=NULL){
                        printTree(auxNode->noFilho,pontos+1);
                }
                if(auxNode->noIrmao!=NULL){
                        printTree(auxNode->noIrmao,pontos);
                }

        }
        free(auxNode);
    }


#line 146 "y.tab.c" /* yacc.c:339  */

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
    CHRLIT = 258,
    ID = 259,
    REALLIT = 260,
    INTLIT = 261,
    CHAR = 262,
    ELSE = 263,
    WHILE = 264,
    IF = 265,
    INT = 266,
    SHORT = 267,
    DOUBLE = 268,
    RETURN = 269,
    VOID = 270,
    BAND = 271,
    BOR = 272,
    BXOR = 273,
    AND = 274,
    ASSIGN = 275,
    MUL = 276,
    COMMA = 277,
    DIV = 278,
    EQ = 279,
    GE = 280,
    GT = 281,
    LBRACE = 282,
    LE = 283,
    LPAR = 284,
    LT = 285,
    MINUS = 286,
    MOD = 287,
    NE = 288,
    NOT = 289,
    OR = 290,
    PLUS = 291,
    RBRACE = 292,
    RPAR = 293,
    SEMI = 294,
    RESERVED = 295
  };
#endif
/* Tokens.  */
#define CHRLIT 258
#define ID 259
#define REALLIT 260
#define INTLIT 261
#define CHAR 262
#define ELSE 263
#define WHILE 264
#define IF 265
#define INT 266
#define SHORT 267
#define DOUBLE 268
#define RETURN 269
#define VOID 270
#define BAND 271
#define BOR 272
#define BXOR 273
#define AND 274
#define ASSIGN 275
#define MUL 276
#define COMMA 277
#define DIV 278
#define EQ 279
#define GE 280
#define GT 281
#define LBRACE 282
#define LE 283
#define LPAR 284
#define LT 285
#define MINUS 286
#define MOD 287
#define NE 288
#define NOT 289
#define OR 290
#define PLUS 291
#define RBRACE 292
#define RPAR 293
#define SEMI 294
#define RESERVED 295

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 81 "uccompiler.y" /* yacc.c:355  */

    struct no * no;
    char * id;

#line 271 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 288 "y.tab.c" /* yacc.c:358  */

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
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   830

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  107
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  188

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   295

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
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   166,   166,   169,   170,   171,   172,   179,   186,   195,
     204,   205,   208,   220,   231,   232,   235,   239,   244,   245,
     255,   256,   260,   270,   273,   274,   283,   284,   285,   286,
     287,   290,   291,   294,   295,   296,   299,   303,   309,   313,
     316,   317,   318,   321,   322,   325,   337,   339,   340,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   401,   402,   405,   406,   407
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CHRLIT", "ID", "REALLIT", "INTLIT",
  "CHAR", "ELSE", "WHILE", "IF", "INT", "SHORT", "DOUBLE", "RETURN",
  "VOID", "BAND", "BOR", "BXOR", "AND", "ASSIGN", "MUL", "COMMA", "DIV",
  "EQ", "GE", "GT", "LBRACE", "LE", "LPAR", "LT", "MINUS", "MOD", "NE",
  "NOT", "OR", "PLUS", "RBRACE", "RPAR", "SEMI", "RESERVED", "$accept",
  "Programa", "FunctionsAndDeclarations", "FunctionDefinition",
  "FunctionBody", "DeclarationsAndStatements", "FunctionDeclaration",
  "FunctionDeclarator", "ParameterList", "ParameterDeclaration",
  "Declaration", "DeclarationCD", "TypeSpec", "Declarator", "Statement",
  "Statementlbrace", "StatementError", "StatementIrmao", "StatementReturn",
  "Expr", "CommaExpr", "ExprFunctions", "ExprMath", YY_NULLPTR
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
     295
};
# endif

#define YYPACT_NINF -92

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-92)))

#define YYTABLE_NINF -3

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     815,   -32,   -92,   -92,   -92,   -92,   -92,     8,   808,   -92,
     -92,   -92,    41,   -92,   -92,   -92,   -92,   -92,   -14,    85,
      19,   -92,   161,   357,    13,   -92,   -92,    42,   -92,   -92,
     -17,   -92,   -92,   167,   226,   226,   226,   542,   -92,   -92,
      21,   -92,    44,   -92,    24,   -92,   -92,    25,    27,    26,
     293,   232,   -92,   -92,    96,   -92,    42,   -92,   379,   -92,
     -92,    49,   -92,    32,    33,   427,   -92,   -92,   -92,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   357,   -92,   -92,
     356,   161,   161,   -92,   -92,   403,   -26,   -92,   -92,   -92,
     -92,   307,    34,   450,   -92,   -92,   -92,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   -92,    56,   -92,   473,   -92,
     -92,   735,   666,   703,   626,   542,   -92,   542,   -92,   763,
     157,   157,   157,   157,    94,   -92,   763,   584,    94,   -92,
      58,   496,   519,   -92,   -92,   -92,    59,   -92,   -92,   -92,
     -92,   749,   685,   721,   647,   563,   -92,   563,   -92,   775,
     222,   222,   222,   222,   154,   -92,   775,   605,   154,   -92,
     -92,   -92,   344,   344,   -92,   105,   344,   -92
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    26,    27,    29,    30,    28,     0,     0,     3,
       4,     5,     0,    23,     1,     6,     7,     8,    31,     0,
       0,    24,     0,     0,     0,    16,     9,     0,    22,    72,
      70,    73,    71,     0,     0,     0,     0,    32,    68,    67,
       0,    18,    20,    99,    97,   100,    98,     0,     0,     0,
       0,     0,    10,    34,     0,    15,     0,    14,     0,    95,
      94,    31,    25,     0,     0,     0,   106,   107,   105,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    17,    21,
       0,     0,     0,    47,    39,     0,     0,    42,    43,    35,
      46,     0,     0,     0,    11,    13,    12,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    33,     0,   103,     0,    75,
      74,    58,    59,    60,    57,    49,    53,    50,    54,    61,
      64,    66,    63,    65,    52,    55,    62,    56,    51,    19,
       0,     0,     0,    48,    41,    44,     0,    40,    45,   102,
     101,    85,    86,    87,    84,    76,    80,    77,    81,    88,
      91,    93,    90,    92,    79,    82,    89,    83,    78,    69,
     104,    96,     0,     0,    38,    36,     0,    37
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -92,   -92,   -92,   106,   -92,   -92,   108,   -92,   -92,   -23,
      -3,   -92,   -20,    91,   -15,   -92,   -91,   -92,   -92,   201,
     -34,    71,   136
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     7,     8,     9,    26,    54,    10,    19,    40,    41,
      11,    20,    12,    21,    98,    99,   100,   101,    94,   128,
      58,    59,    60
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      66,    67,    68,    42,    56,    17,    22,    13,    14,    57,
     158,   154,    63,   155,     1,    23,    43,    44,    45,    46,
       2,    55,    47,    48,     3,     4,     5,    49,     6,    29,
      30,    31,    32,   126,    56,    29,    30,    31,    32,   106,
      50,    27,    51,    87,    34,    18,    61,    35,    89,    36,
      52,   105,    53,    90,    91,    33,    92,    34,    28,    88,
      35,    33,    36,    34,   149,    93,    35,    42,    36,    22,
     127,   129,   159,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   184,   185,    38,   179,   187,   181,     1,   155,    43,
      44,    45,    46,     2,    38,    47,    48,     3,     4,     5,
      49,     6,    24,   186,    15,    74,    16,    76,    62,     0,
      38,     0,    38,    50,    25,    51,    83,    34,     0,     0,
      35,     0,    36,   104,    38,    53,     0,     0,     0,     0,
      38,    38,    38,    38,    38,    38,    38,    38,    38,    38,
      38,    38,    38,    38,    38,    38,    38,    38,    39,     0,
       0,    38,    38,    38,    29,    30,    31,    32,    64,    39,
      29,    30,    31,    32,     0,   112,     0,   114,    74,     0,
      76,     0,     0,     0,     0,    39,   121,    39,    82,    83,
      33,     0,    34,    86,     0,    35,    33,    36,    34,    39,
       0,    35,     0,    36,     0,    39,    39,    39,    39,    39,
      39,    39,    39,    39,    39,    39,    39,    39,    39,    39,
      39,    39,    39,    37,     0,     0,    39,    39,    39,    43,
      44,    45,    46,   102,    65,    29,    30,    31,    32,     0,
       0,     0,     0,   112,     0,   114,     0,     0,     0,     0,
      95,     0,   103,   120,   121,    51,     0,    34,   124,     0,
      35,    33,    36,    34,     0,     0,    35,     0,    36,     0,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,     0,     0,
       0,     0,   151,   152,    96,     0,    43,    44,    45,    46,
       0,     0,    47,    48,     0,     0,     0,    49,   156,     0,
      43,    44,    45,    46,     0,     0,    47,    48,     0,     0,
      50,    49,    51,     0,    34,     0,     0,    35,     0,    36,
      97,     0,    53,     0,    50,     0,    51,     0,    34,     0,
       0,    35,     0,    36,   157,   156,    53,    43,    44,    45,
      46,     0,     0,    47,    48,     0,     0,   150,    49,    29,
      30,    31,    32,     0,     2,     0,     0,     0,     3,     4,
       5,    50,     6,    51,     0,    34,     0,     0,    35,     0,
      36,     0,     0,    53,     0,    33,     0,    34,     0,     0,
      35,     0,    36,     0,   127,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,     0,   118,     0,   119,
     120,   121,   122,     0,   123,   124,     0,     0,   125,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,     0,    81,    82,    83,    84,     0,    85,    86,
       0,     0,   153,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,     0,    80,     0,    81,    82,    83,
      84,     0,    85,    86,     0,   130,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,     0,    80,     0,
      81,    82,    83,    84,     0,    85,    86,     0,   160,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,     0,    81,    82,    83,    84,     0,    85,    86,
       0,   180,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,     0,    80,     0,    81,    82,    83,    84,
       0,    85,    86,     0,   182,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,     0,    80,     0,    81,
      82,    83,    84,     0,    85,    86,     0,   183,    69,    70,
      71,    72,    73,    74,     0,    76,    77,    78,    79,     0,
      80,     0,    81,    82,    83,    84,     0,    85,    86,   107,
     108,   109,   110,   111,   112,     0,   114,   115,   116,   117,
       0,   118,     0,   119,   120,   121,   122,     0,   123,   124,
      69,    70,    71,    72,     0,    74,     0,    76,    77,    78,
      79,     0,    80,     0,    81,    82,    83,    84,     0,     0,
      86,   107,   108,   109,   110,     0,   112,     0,   114,   115,
     116,   117,     0,   118,     0,   119,   120,   121,   122,     0,
       0,   124,    69,    70,    71,     0,     0,    74,     0,    76,
      77,    78,    79,     0,    80,     0,    81,    82,    83,    84,
       0,     0,    86,   107,   108,   109,     0,     0,   112,     0,
     114,   115,   116,   117,     0,   118,     0,   119,   120,   121,
     122,     0,    69,   124,    71,     0,     0,    74,     0,    76,
      77,    78,    79,     0,    80,     0,    81,    82,    83,    84,
       0,   107,    86,   109,     0,     0,   112,     0,   114,   115,
     116,   117,     0,   118,     0,   119,   120,   121,   122,    69,
       0,   124,     0,     0,    74,     0,    76,    77,    78,    79,
       0,    80,     0,    81,    82,    83,    84,   107,     0,    86,
       0,     0,   112,     0,   114,   115,   116,   117,     0,   118,
       0,   119,   120,   121,   122,     0,    74,   124,    76,    77,
      78,    79,     0,    80,     0,    81,    82,    83,    84,     0,
     112,    86,   114,   115,   116,   117,     0,   118,     0,   119,
     120,   121,   122,     0,    74,   124,    76,     0,    78,    79,
       0,    80,     0,    81,    82,    83,   112,     0,   114,    86,
     116,   117,     0,   118,     0,   119,   120,   121,    -2,     1,
       0,   124,     0,     0,     0,     2,     1,     0,     0,     3,
       4,     5,     2,     6,     0,     0,     3,     4,     5,     0,
       6
};

static const yytype_int16 yycheck[] =
{
      34,    35,    36,    23,    24,     8,    20,    39,     0,    24,
     101,    37,    29,    39,     1,    29,     3,     4,     5,     6,
       7,    24,     9,    10,    11,    12,    13,    14,    15,     3,
       4,     5,     6,     1,    54,     3,     4,     5,     6,    54,
      27,    22,    29,    22,    31,     4,     4,    34,     4,    36,
      37,    54,    39,    29,    29,    29,    29,    31,    39,    38,
      34,    29,    36,    31,    87,    39,    34,    87,    36,    20,
      38,    38,    38,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   182,   183,    22,    38,   186,    38,     1,    39,     3,
       4,     5,     6,     7,    33,     9,    10,    11,    12,    13,
      14,    15,    27,     8,     8,    21,     8,    23,    27,    -1,
      49,    -1,    51,    27,    39,    29,    32,    31,    -1,    -1,
      34,    -1,    36,    37,    63,    39,    -1,    -1,    -1,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    22,    -1,
      -1,    90,    91,    92,     3,     4,     5,     6,     1,    33,
       3,     4,     5,     6,    -1,    21,    -1,    23,    21,    -1,
      23,    -1,    -1,    -1,    -1,    49,    32,    51,    31,    32,
      29,    -1,    31,    36,    -1,    34,    29,    36,    31,    63,
      -1,    34,    -1,    36,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    22,    -1,    -1,    90,    91,    92,     3,
       4,     5,     6,     1,    33,     3,     4,     5,     6,    -1,
      -1,    -1,    -1,    21,    -1,    23,    -1,    -1,    -1,    -1,
      49,    -1,    51,    31,    32,    29,    -1,    31,    36,    -1,
      34,    29,    36,    31,    -1,    -1,    34,    -1,    36,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    -1,
      -1,    -1,    91,    92,     1,    -1,     3,     4,     5,     6,
      -1,    -1,     9,    10,    -1,    -1,    -1,    14,     1,    -1,
       3,     4,     5,     6,    -1,    -1,     9,    10,    -1,    -1,
      27,    14,    29,    -1,    31,    -1,    -1,    34,    -1,    36,
      37,    -1,    39,    -1,    27,    -1,    29,    -1,    31,    -1,
      -1,    34,    -1,    36,    37,     1,    39,     3,     4,     5,
       6,    -1,    -1,     9,    10,    -1,    -1,     1,    14,     3,
       4,     5,     6,    -1,     7,    -1,    -1,    -1,    11,    12,
      13,    27,    15,    29,    -1,    31,    -1,    -1,    34,    -1,
      36,    -1,    -1,    39,    -1,    29,    -1,    31,    -1,    -1,
      34,    -1,    36,    -1,    38,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    28,    -1,    30,
      31,    32,    33,    -1,    35,    36,    -1,    -1,    39,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    28,    -1,    30,    31,    32,    33,    -1,    35,    36,
      -1,    -1,    39,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    28,    -1,    30,    31,    32,
      33,    -1,    35,    36,    -1,    38,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    28,    -1,
      30,    31,    32,    33,    -1,    35,    36,    -1,    38,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    28,    -1,    30,    31,    32,    33,    -1,    35,    36,
      -1,    38,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    28,    -1,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    28,    -1,    30,
      31,    32,    33,    -1,    35,    36,    -1,    38,    16,    17,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,    -1,
      28,    -1,    30,    31,    32,    33,    -1,    35,    36,    16,
      17,    18,    19,    20,    21,    -1,    23,    24,    25,    26,
      -1,    28,    -1,    30,    31,    32,    33,    -1,    35,    36,
      16,    17,    18,    19,    -1,    21,    -1,    23,    24,    25,
      26,    -1,    28,    -1,    30,    31,    32,    33,    -1,    -1,
      36,    16,    17,    18,    19,    -1,    21,    -1,    23,    24,
      25,    26,    -1,    28,    -1,    30,    31,    32,    33,    -1,
      -1,    36,    16,    17,    18,    -1,    -1,    21,    -1,    23,
      24,    25,    26,    -1,    28,    -1,    30,    31,    32,    33,
      -1,    -1,    36,    16,    17,    18,    -1,    -1,    21,    -1,
      23,    24,    25,    26,    -1,    28,    -1,    30,    31,    32,
      33,    -1,    16,    36,    18,    -1,    -1,    21,    -1,    23,
      24,    25,    26,    -1,    28,    -1,    30,    31,    32,    33,
      -1,    16,    36,    18,    -1,    -1,    21,    -1,    23,    24,
      25,    26,    -1,    28,    -1,    30,    31,    32,    33,    16,
      -1,    36,    -1,    -1,    21,    -1,    23,    24,    25,    26,
      -1,    28,    -1,    30,    31,    32,    33,    16,    -1,    36,
      -1,    -1,    21,    -1,    23,    24,    25,    26,    -1,    28,
      -1,    30,    31,    32,    33,    -1,    21,    36,    23,    24,
      25,    26,    -1,    28,    -1,    30,    31,    32,    33,    -1,
      21,    36,    23,    24,    25,    26,    -1,    28,    -1,    30,
      31,    32,    33,    -1,    21,    36,    23,    -1,    25,    26,
      -1,    28,    -1,    30,    31,    32,    21,    -1,    23,    36,
      25,    26,    -1,    28,    -1,    30,    31,    32,     0,     1,
      -1,    36,    -1,    -1,    -1,     7,     1,    -1,    -1,    11,
      12,    13,     7,    15,    -1,    -1,    11,    12,    13,    -1,
      15
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     7,    11,    12,    13,    15,    42,    43,    44,
      47,    51,    53,    39,     0,    44,    47,    51,     4,    48,
      52,    54,    20,    29,    27,    39,    45,    22,    39,     3,
       4,     5,     6,    29,    31,    34,    36,    60,    62,    63,
      49,    50,    53,     3,     4,     5,     6,     9,    10,    14,
      27,    29,    37,    39,    46,    51,    53,    55,    61,    62,
      63,     4,    54,    29,     1,    60,    61,    61,    61,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      28,    30,    31,    32,    33,    35,    36,    22,    38,     4,
      29,    29,    29,    39,    59,    60,     1,    37,    55,    56,
      57,    58,     1,    60,    37,    51,    55,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    28,    30,
      31,    32,    33,    35,    36,    39,     1,    38,    60,    38,
      38,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    50,
       1,    60,    60,    39,    37,    39,     1,    37,    57,    38,
      38,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    38,
      38,    38,    38,    38,    57,    57,     8,    57
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    41,    42,    43,    43,    43,    43,    43,    43,    44,
      45,    45,    46,    46,    46,    46,    47,    48,    49,    49,
      50,    50,    51,    51,    52,    52,    53,    53,    53,    53,
      53,    54,    54,    55,    55,    55,    55,    55,    55,    55,
      56,    56,    56,    57,    57,    58,    58,    59,    59,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    62,    62,    63,    63,    63
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     2,     2,     2,     3,
       2,     3,     2,     2,     1,     1,     3,     4,     1,     3,
       1,     2,     3,     2,     1,     3,     1,     1,     1,     1,
       1,     1,     3,     2,     1,     2,     5,     7,     5,     2,
       2,     2,     1,     1,     2,     2,     1,     1,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     4,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     4,     1,     1,     1,
       1,     3,     3,     3,     4,     2,     2,     2
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
#line 166 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no)=inserirNo(NULL,"Program",(yyvsp[0].no));if (flag==2 && errorFlag==0)printTree((yyval.no),0);}
#line 1624 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 169 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = (yyvsp[0].no);}
#line 1630 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 170 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = (yyvsp[0].no);}
#line 1636 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 171 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = (yyvsp[0].no);}
#line 1642 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 172 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = (yyvsp[-1].no);
                                                        noAuxiliar = (yyvsp[-1].no);
                                                        while(noAuxiliar->noIrmao!=NULL){
                                                                noAuxiliar=noAuxiliar->noIrmao;
                                                        }
                                                        addIrmao(noAuxiliar,(yyvsp[0].no));
                                                        }
#line 1654 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 179 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = (yyvsp[-1].no);
                                                        noAuxiliar = (yyvsp[-1].no);
                                                        while(noAuxiliar->noIrmao!=NULL){
                                                                noAuxiliar=noAuxiliar->noIrmao;
                                                        }
                                                        addIrmao(noAuxiliar,(yyvsp[0].no));
                                                        }
#line 1666 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 186 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = (yyvsp[-1].no);
                                                        noAuxiliar = (yyvsp[-1].no);
                                                        while(noAuxiliar->noIrmao!=NULL){
                                                                noAuxiliar=noAuxiliar->noIrmao;
                                                        }
                                                        addIrmao(noAuxiliar,(yyvsp[0].no));
                                                        }
#line 1678 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 195 "uccompiler.y" /* yacc.c:1646  */
    { (yyval.no) = inserirNo(NULL,"FuncDefinition",(yyvsp[-2].no));
                                                                addIrmao((yyvsp[-2].no),(yyvsp[-1].no));
                                                                noAuxiliar = (yyvsp[-1].no);
                                                                while (noAuxiliar->noIrmao!=NULL)
                                                                        noAuxiliar = noAuxiliar->noIrmao;
                                                                noAuxiliar->noIrmao = (yyvsp[0].no);
                                                                }
#line 1690 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 204 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,"FuncBody",NULL);}
#line 1696 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 205 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,"FuncBody",(yyvsp[-1].no));}
#line 1702 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 208 "uccompiler.y" /* yacc.c:1646  */
    {if((yyvsp[-1].no)!=NULL){
                                                                        (yyval.no)=(yyvsp[-1].no);
                                                                        if ((yyvsp[0].no)!=NULL){
                                                                                noAuxiliar = (yyval.no);
                                                                                while (noAuxiliar->noIrmao!=NULL)
                                                                                        noAuxiliar = noAuxiliar->noIrmao;
                                                                                noAuxiliar->noIrmao = (yyvsp[0].no);
                                                                        }
                                                                        }else{
                                                                                (yyval.no)=(yyvsp[0].no);
                                                                        }
                                                                }
#line 1719 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 220 "uccompiler.y" /* yacc.c:1646  */
    {if((yyvsp[-1].no)!=NULL){
                                                                        (yyval.no)=(yyvsp[-1].no);
                                                                        if ((yyvsp[0].no)!=NULL){
                                                                                noAuxiliar = (yyval.no);
                                                                                while (noAuxiliar->noIrmao!=NULL)
                                                                                        noAuxiliar = noAuxiliar->noIrmao;
                                                                                noAuxiliar->noIrmao = (yyvsp[0].no);
                                                                        }
                                                                }else{
                                                                        (yyval.no)=(yyvsp[0].no);
                                                                }}
#line 1735 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 231 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no)=(yyvsp[0].no);}
#line 1741 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 232 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no)=(yyvsp[0].no);}
#line 1747 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 235 "uccompiler.y" /* yacc.c:1646  */
    { addIrmao((yyvsp[-2].no),(yyvsp[-1].no)); 
                                                        (yyval.no) = inserirNo(NULL,"FuncDeclaration",(yyvsp[-2].no));}
#line 1754 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 239 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no)=inserirNo((yyvsp[-3].id),"Id",NULL);
                                                addIrmao((yyval.no),inserirNo(NULL,"ParamList",(yyvsp[-1].no)));
                                                }
#line 1762 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 244 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = (yyvsp[0].no);}
#line 1768 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 245 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = (yyvsp[-2].no);
                                                        noAuxiliar = (yyvsp[-2].no);
                                                        while(noAuxiliar->noIrmao!=NULL){
                                                                noAuxiliar=noAuxiliar->noIrmao;
                                                        }
                                                        addIrmao(noAuxiliar,(yyvsp[0].no));
                                                                }
#line 1780 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 255 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,"ParamDeclaration",(yyvsp[0].no));}
#line 1786 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 256 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-1].no),inserirNo((yyvsp[0].id),"Id",NULL));
                                (yyval.no) = inserirNo(NULL,"ParamDeclaration",(yyvsp[-1].no));}
#line 1793 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 260 "uccompiler.y" /* yacc.c:1646  */
    {      no * noDeclarate;
                                                /*addIrmao($1,$2);*/
                                                noAuxiliar=(yyvsp[-1].no);
                                                while(noAuxiliar!=NULL){
                                                        noDeclarate=noAuxiliar->noFilho;
                                                        noAuxiliar->noFilho=inserirNo((yyvsp[-2].no)->id,(yyvsp[-2].no)->type,(yyvsp[-2].no)->noFilho);
                                                        addIrmao(noAuxiliar->noFilho,noDeclarate);
                                                        noAuxiliar=noAuxiliar->noIrmao;
                                                }
                                                (yyval.no) = (yyvsp[-1].no);}
#line 1808 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 270 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,NULL,NULL);errorFlag=1;}
#line 1814 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 273 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no)=inserirNo(NULL,"Declaration",(yyvsp[0].no));}
#line 1820 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 274 "uccompiler.y" /* yacc.c:1646  */
    { (yyval.no) = (yyvsp[-2].no);
                                                noAuxiliar = (yyval.no);
                                                while(noAuxiliar->noIrmao!=NULL){
                                                        noAuxiliar=noAuxiliar->noIrmao;
                                                }
                                                addIrmao(noAuxiliar,inserirNo(NULL,"Declaration",(yyvsp[0].no)));
                                                }
#line 1832 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 283 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,"Char",NULL);}
#line 1838 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 284 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,"Int",NULL);}
#line 1844 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 285 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,"Void",NULL);}
#line 1850 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 286 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,"Short",NULL);}
#line 1856 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 287 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,"Double",NULL);}
#line 1862 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 290 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo((yyvsp[0].id),"Id",NULL);}
#line 1868 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 291 "uccompiler.y" /* yacc.c:1646  */
    { (yyval.no) = inserirNo((yyvsp[-2].id),"Id",NULL); addIrmao((yyval.no),(yyvsp[0].no));}
#line 1874 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 294 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = (yyvsp[-1].no);}
#line 1880 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 295 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = NULL;}
#line 1886 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 296 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = (yyvsp[0].no);
                                   if ((yyvsp[0].no)!=NULL && (yyvsp[0].no)->noIrmao!=NULL){ (yyval.no) = inserirNo(NULL,"StatList",(yyvsp[0].no)); }
                                }
#line 1894 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 299 "uccompiler.y" /* yacc.c:1646  */
    { if((yyvsp[0].no)==NULL) (yyvsp[0].no)=inserirNo(NULL,"Null",NULL);
                                             addIrmao((yyvsp[-2].no),(yyvsp[0].no));
                                             addIrmao((yyvsp[0].no),inserirNo(NULL,"Null",NULL));
                                             (yyval.no) = inserirNo(NULL,"If",(yyvsp[-2].no));  }
#line 1903 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 303 "uccompiler.y" /* yacc.c:1646  */
    {if((yyvsp[-2].no)==NULL) (yyvsp[-2].no)=inserirNo(NULL,"Null",NULL);
                                                                if((yyvsp[0].no)==NULL) (yyvsp[0].no)=inserirNo(NULL,"Null",NULL);
                                                                addIrmao((yyvsp[-4].no),(yyvsp[-2].no));
                                                                addIrmao((yyvsp[-2].no),(yyvsp[0].no));
                                                                (yyval.no) = inserirNo(NULL,"If",(yyvsp[-4].no));
                                                                ;}
#line 1914 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 309 "uccompiler.y" /* yacc.c:1646  */
    { if((yyvsp[0].no)==NULL) (yyvsp[0].no)=inserirNo(NULL,"Null",NULL);
                                                addIrmao((yyvsp[-2].no),(yyvsp[0].no));                                                
                                                (yyval.no) = inserirNo(NULL,"While",(yyvsp[-2].no));
                                                }
#line 1923 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 313 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no)=(yyvsp[0].no);}
#line 1929 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 316 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no)=(yyvsp[-1].no);}
#line 1935 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 317 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,NULL,NULL);errorFlag=1;}
#line 1941 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 318 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = NULL;}
#line 1947 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 321 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no)=(yyvsp[0].no);}
#line 1953 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 322 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,NULL,NULL);errorFlag=1;}
#line 1959 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 325 "uccompiler.y" /* yacc.c:1646  */
    {
                                                if((yyvsp[-1].no)!=NULL){
                                                        (yyval.no) = (yyvsp[-1].no);
                                                        if ((yyvsp[0].no)!=NULL){
                                                                noAuxiliar = (yyvsp[-1].no);
                                                                while (noAuxiliar->noIrmao!=NULL)
                                                                        noAuxiliar = noAuxiliar->noIrmao;
                                                                noAuxiliar->noIrmao = (yyvsp[0].no);
                                                        }
                                                    }else
                                                        (yyval.no) = (yyvsp[0].no);
                                                    }
#line 1976 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 337 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no)=(yyvsp[0].no);}
#line 1982 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 339 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,"Return",inserirNo(NULL,"Null",NULL));}
#line 1988 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 340 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,"Return",(yyvsp[-1].no));}
#line 1994 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 343 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Store",(yyvsp[-2].no));}
#line 2000 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 344 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no)=(yyvsp[-2].no);}
#line 2006 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 345 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Add",(yyvsp[-2].no));}
#line 2012 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 346 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Sub",(yyvsp[-2].no));}
#line 2018 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 347 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Mul",(yyvsp[-2].no));}
#line 2024 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 348 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Div",(yyvsp[-2].no));}
#line 2030 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 349 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Mod",(yyvsp[-2].no));}
#line 2036 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 350 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Or",(yyvsp[-2].no));}
#line 2042 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 351 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"And",(yyvsp[-2].no));}
#line 2048 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 352 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"BitWiseAnd",(yyvsp[-2].no));}
#line 2054 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 353 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"BitWiseOr",(yyvsp[-2].no));}
#line 2060 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 354 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"BitWiseXor",(yyvsp[-2].no));}
#line 2066 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 355 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Eq",(yyvsp[-2].no));}
#line 2072 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 356 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Ne",(yyvsp[-2].no));}
#line 2078 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 357 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Le",(yyvsp[-2].no));}
#line 2084 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 358 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Ge",(yyvsp[-2].no));}
#line 2090 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 359 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Lt",(yyvsp[-2].no));}
#line 2096 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 360 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Gt",(yyvsp[-2].no));}
#line 2102 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 362 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,"Call",(yyvsp[0].no));}
#line 2108 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 363 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,NULL,NULL);errorFlag=1;}
#line 2114 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 364 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no)=inserirNo((yyvsp[0].id),"Id",NULL);}
#line 2120 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 365 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no)=inserirNo((yyvsp[0].id),"IntLit",NULL);}
#line 2126 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 366 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no)=inserirNo((yyvsp[0].id),"ChrLit",NULL);}
#line 2132 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 367 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no)=inserirNo((yyvsp[0].id),"RealLit",NULL);}
#line 2138 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 368 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = (yyvsp[-1].no);}
#line 2144 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 369 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,NULL,NULL);errorFlag=1;}
#line 2150 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 372 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Store",(yyvsp[-2].no));}
#line 2156 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 373 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no)) ;(yyval.no) = inserirNo(NULL,"Comma",(yyvsp[-2].no));}
#line 2162 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 374 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Add",(yyvsp[-2].no));}
#line 2168 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 375 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Sub",(yyvsp[-2].no));}
#line 2174 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 376 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Mul",(yyvsp[-2].no));}
#line 2180 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 377 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Div",(yyvsp[-2].no));}
#line 2186 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 378 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Mod",(yyvsp[-2].no));}
#line 2192 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 379 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Or",(yyvsp[-2].no));}
#line 2198 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 380 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"And",(yyvsp[-2].no));}
#line 2204 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 381 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"BitWiseAnd",(yyvsp[-2].no));}
#line 2210 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 382 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"BitWiseOr",(yyvsp[-2].no));}
#line 2216 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 383 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"BitWiseXor",(yyvsp[-2].no));}
#line 2222 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 384 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Eq",(yyvsp[-2].no));}
#line 2228 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 385 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Ne",(yyvsp[-2].no));}
#line 2234 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 386 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Le",(yyvsp[-2].no));}
#line 2240 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 387 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Ge",(yyvsp[-2].no));}
#line 2246 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 388 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Lt",(yyvsp[-2].no));}
#line 2252 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 389 "uccompiler.y" /* yacc.c:1646  */
    {addIrmao((yyvsp[-2].no),(yyvsp[0].no));(yyval.no) = inserirNo(NULL,"Gt",(yyvsp[-2].no));}
#line 2258 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 391 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,"Call",(yyvsp[0].no));}
#line 2264 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 392 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,NULL,NULL);errorFlag=1;}
#line 2270 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 393 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no)=inserirNo((yyvsp[0].id),"Id",NULL);}
#line 2276 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 394 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no)=inserirNo((yyvsp[0].id),"IntLit",NULL);}
#line 2282 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 395 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no)=inserirNo((yyvsp[0].id),"ChrLit",NULL);}
#line 2288 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 396 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no)=inserirNo((yyvsp[0].id),"RealLit",NULL);}
#line 2294 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 397 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = (yyvsp[-1].no);}
#line 2300 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 398 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,NULL,NULL);errorFlag=1;}
#line 2306 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 401 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo((yyvsp[-2].id),"Id",NULL);}
#line 2312 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 402 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo((yyvsp[-3].id),"Id",NULL);
                                        addIrmao((yyval.no),(yyvsp[-1].no));}
#line 2319 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 405 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,"Plus",(yyvsp[0].no));}
#line 2325 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 406 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,"Minus",(yyvsp[0].no));}
#line 2331 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 407 "uccompiler.y" /* yacc.c:1646  */
    {(yyval.no) = inserirNo(NULL,"Not",(yyvsp[0].no));}
#line 2337 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2341 "y.tab.c" /* yacc.c:1646  */
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
