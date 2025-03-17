/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S24)  |"
# ECHO "=---------------------------------------="
# ECHO "    n   n   pppp    l       pppp         "
# ECHO "    nn  n   p   p   l       p   p        "
# ECHO "    n n n   pppp    l       pppp         "
# ECHO "    n  nn   p       l       p            "
# ECHO "    n   n   p       llll    p            "
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define the number of tokens */
#define NUM_TOKENS 15

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	DEF_T,		/*  1: Method name identifier token (start: ~) */
	VAR_T,      /*  2: Variable identifier token */
	D_T,		/*  3: Integer literal token */
	L_T,		/*  4: String literal token */
	P1_T,		/*  5: Left parenthesis token */
	P2_T,		/*  6: Right parenthesis token */
	LCUR_T,		/*  7: Left brace token */
	RCUR_T,		/*  8: Right brace token */
	K_T,		/*  9: Keyword token */
	RTE_T,		/* 11 Run-time error token */
	SEOF_T,		/* 12:Source end-of-file token */
	SCMT_T,		/* 13: Comment token */
	MCMT_T		/* 14: Multi Comment token */
};

/* TO_DO: Define the list of keywords */
static nplp_string tokenStrTable[NUM_TOKENS] = {
	"ERR_T",	/*  0: Error token */
	"DEF_T",	/*  1: Method name identifier token (start: ~) */
	"VID_T",	/*  2: Variable identifier token */
	"D_T",		/*  3: Integer literal token */
	"L_T",		/*  4: String literal token */
	"P1_T",		/*  5: Left parenthesis token */
	"P2_T",		/*  6: Right parenthesis token */
	"LCUR_T",	/*  7: Left brace token */
	"RCUR_T",	/*  8: Right brace token */
	"K_T",		/*  9: Keyword token */
	"RTE_T",	/* 11 Run-time error token */
	"SEOF_T",	/* 12:Source end-of-file token */
	"SCMT_T",	/* 13: Comment token */
	"MCMT_T"	/* 14: Multi Comment token */
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	nplp_intg codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	nplp_intg intValue;				/* integer literal attribute (value) */
	nplp_intg keywordIndex;			/* keyword index in the keyword table */
	nplp_intg contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	nplp_real floatValue;				/* floating-point literal attribute (value) */
	nplp_char idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	nplp_char errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	nplp_byte flags;			/* Flags information */
	union {
		nplp_intg intValue;				/* Integer value */
		nplp_real floatValue;			/* Float value */
		nplp_string stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	nplp_intg code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	nplp_intg scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* TO_DO: Define lexeme FIXED classes */
/* EOF definitions */
#define EOS_CHR '\0'	// CH00
#define EOF_CHR 0xFF	// CH01
#define U_CHR '_'		// CH02
#define H_CHR '#'		// CH05
#define TAB_CHR '\t'	// CH06
#define S_CHR ' '		// CH07
#define NL_CHR '\n'	// CH08
#define P1_CHR '('		// CH10
#define P2_CHR ')'		// CH11
#define LCUR_CHR '{'		// CH12
#define RCUR_CHR '}'		// CH13
#define TIL_CHR '~'
#define SGQ_CHR '\''
#define DBQ_CHR '\"'



/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */


/* TO_DO: Error states and illegal state */
#define ESNR	98		/* Error state with no retract */
#define ESWR	99		/* Error state with retract */
#define FS		100		/* Illegal state */

 /* TO_DO: State transition table definition */
#define NUM_STATES		18
#define CHAR_CLASSES	11


/* TO_DO: Transition table - type of states defined in separate table 
static nplp_intg transitionTable[NUM_STATES][CHAR_CLASSES] = {
/*    [A-z],[0-9],    _,     ,~     , \n   , #   ,'    ,   ( ,   ",  )  ,
	   L(0), D(1), U(2), S(3),DEF(4), NL(5), H(6),SQ(7),   P1,   Q,  P2 *//*
	{     9,   11, ESNR, ESNR,    13,  ESNR,	 1,    3, ESNR,  16,  ESNR},	// S0: NOAS
	{     1,    1,    1,    1,	   1,     2,     1,    1,    1,   1,    1 },	// S1: NOAS
	{    FS,   FS,   FS,   FS,    FS,    FS,	FS,   FS,   FS,  FS,    FS},	// S2: ASNR (SLC)
	{  ESNR, ESNR, ESNR, ESNR,  ESNR,  ESNR,  ESNR,    4, ESNR,ESNR,  ESNR},	// S3: NOAS (KEY)
	{  ESNR, ESNR, ESNR, ESNR,  ESNR,  ESNR,  ESNR,    5, ESNR,ESNR,  ESNR},	// S4: NOAS
	{     5,    5,    5,    5,     5,     5,     5,    6,    5,   5,     5},	// S5: NOAS 
	{     5,    5,    5,    5,     5,     5,     5,    7,    5,   5,     5},	// S6: NOAS
	{     5,    5,    5,    5,     5,     5,     5,    8,    5,   5,     5},	// S7: NOAS (COM)
	{    FS,   FS,   FS,   FS,    FS,    FS,	FS,   FS,   FS,  FS,    FS},	// S8: ASNR (ES)
	{     9,    9,    9,   10,    10,    10,    10,   10,   10,  10,    10},    // S9: NOAS 
	{    FS,   FS,   FS,   FS,    FS,    FS,	FS,   FS,   FS,  FS,    FS},    // S10: ASNR (ER)
	{    12,   11,   12,   12,    12,    12,    12,   12,   12,  12,    12},    // S11: NOAS 
	{    FS,   FS,   FS,   FS,    FS,    FS,	FS,   FS,   FS,  FS,    FS},    // S12: ASNR (ER)
	{  ESNR, ESNR, ESNR,   14,  ESNR,  ESNR,  ESNR, ESNR, ESNR,ESNR,  ESNR},    // S13: NOAS (ER)
	{    14,   14,   14, ESNR,  ESNR,  ESNR,  ESNR, ESNR,   15,ESNR,  ESNR},    // S14: NOAS (ER)
	{    FS,   FS,   FS,   FS,    FS,    FS,	FS,   FS,   FS,  FS,    FS},    // S15: ASNR (ER)
	{    16,   16,   16,   16,    16,    16,    16,   16,   16,  17,    16},    // S16 NOAS 
	{    FS,   FS,   FS,   FS,    FS,    FS,	FS,   FS,   FS,  FS,    FS},    // S17: ASNR (ER)
};*/
static nplp_intg transitionTable[NUM_STATES][CHAR_CLASSES] = {
	/*    [A-z],[0-9],    _,     ,~     , \n   , #   ,'    ,     ( ,     ",    )  ,
		   L(0), D(1), U(2), S(3),DEF(4), NL(5), H(6),SQ(7),  P1(8),  Q(9),  P2(10) */
		{     9,   11, ESNR, ESNR,    13,  ESNR,	1,    3,   ESNR,    16,  ESNR},	// S0: NOAS
		{     1,    1,    1,    1,	   1,     2,    1,    1,      1,     1,    1 },	// S1: NOAS
		{    FS,   FS,   FS,   FS,    FS,    FS,   FS,   FS,     FS,    FS,    FS},	// S2: ASNR (SLC)
		{  ESNR, ESNR, ESNR, ESNR,  ESNR,  ESNR, ESNR,    4,   ESNR,  ESNR,  ESNR},	// S3: NOAS (KEY)
		{  ESNR, ESNR, ESNR, ESNR,  ESNR,  ESNR, ESNR,    5,   ESNR,  ESNR,  ESNR},	// S4: NOAS
		{     5,    5,    5,    5,     5,     5,    5,    6,      5,     5,     5},	// S5: NOAS 
		{     5,    5,    5,    5,     5,     5,    5,    7,      5,     5,     5},	// S6: NOAS
		{     5,    5,    5,    5,     5,     5,    5,    8,      5,     5,     5},	// S7: NOAS (COM)
		{    FS,   FS,   FS,   FS,    FS,    FS,   FS,   FS,     FS,    FS,    FS},	// S8: ASNR (MLC)
		{     9,    9,    9,   10,    10,    10,   10,   10,     10,    10,    10},    // S9: NOAS 
		{    FS,   FS,   FS,   FS,    FS,    FS,   FS,   FS,     FS,    FS,    FS},    // S10: ASNR (VAR)
		{    12,   11,   12,   12,    12,    12,   12,   12,     12,    12,    12},    // S11: NOAS 
		{    FS,   FS,   FS,   FS,    FS,    FS,   FS,   FS,     FS,    FS,    FS},    // S12: ASNR (INTEGER)
		{  ESNR, ESNR, ESNR,   14,  ESNR,  ESNR, ESNR, ESNR,   ESNR,  ESNR,  ESNR},    // S13: NOAS (ER)
		{    14,   14,   14,   15,    15,    15,   15,   15,     15,    15,    15},    // S14: NOAS (ER)
		{    FS,   FS,   FS,   FS,    FS,    FS,   FS,   FS,     FS,    FS,    FS},    // S15: ASNR (MTHID)
		{    16,   16,   16,   16,    16,    16,   16,   16,     16,    17,    16},    // S16 NOAS 
		{    FS,   FS,   FS,   FS,    FS,    FS,   FS,   FS,     FS,    FS,    FS},    // S17: ASNR (ER)
};
/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static nplp_intg stateType[NUM_STATES] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	FSNR, /* 02 (SLC) - Single Line Comment*/
	NOFS, /* 03 */
	NOFS, /* 04 */
	NOFS, /* 05 */
	NOFS, /* 06 */
	NOFS, /* 07 */
	FSNR, /* 08 (MLC) - Multi Line Comment */
	NOFS, /* 09 */
	FSWR, /* 10 Variable */
	NOFS, /* 11 */
	FSNR, /* 12 Integer */
	NOFS, /* 13 */
	NOFS, /* 14 */
	FSWR, /* 15 (MID) - Method Indication */
	NOFS, /* 16 */
	FSNR  /* 17 String literal */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
nplp_intg			startScanner(BufferPointer psc_buf);
static nplp_intg	nextClass(nplp_char c);					/* character class function */
static nplp_intg	nextState(sofia_intg, sofia_char);		/* state machine function */
nplp_void			printScannerData(ScannerData scData);
Token				tokenizer(sofia_void);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(nplp_string lexeme);

/* Declare accepting states functions */
Token funcSL	(nplp_string lexeme);
Token funcIL	(nplp_string lexeme);
Token funcID	(nplp_string lexeme);
Token funcCMT   (nplp_string lexeme);
Token funcKEY	(nplp_string lexeme);
Token funcErr	(nplp_string lexeme);
Token funcMLC   (nplp_string lexeme);
Token funcVAR(nplp_string lexeme);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,     /* 00 */
	NULL,     /* 01 */
	funcCMT,  /* 02 (MNID_T or KW_T) */
	NULL,     /* 03 */
	NULL,     /* 04 */
	NULL,     /* 05 */
	NULL,     /* 06 */
	NULL,	  /* 07  */
	funcMLC,     /* 08 (MCMT_T) */
	NULL,     /* 09 */
	funcKEY,   /* 10 (ID) */
	NULL,     /* 11 */
	funcIL,   /* 12 (INL_T) */
	NULL,     /* 13 */
	NULL,     /* 14 */
	funcID,   /* 15 (MNID_T) */
	NULL,     /* 16 */
	funcSL    /* 17 (STR_T) */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 8

/* TO_DO: Define the list of keywords */

static nplp_string keywordTable[KWT_SIZE] = {
	"int",		/* KW00 */
	"string",	/* KW01 */
	"if",		/* KW02 */
	"then",		/* KW03 */
	"else",		/* KW04 */
	"while",	/* KW05 */
	"do",		/* KW06 */
	"return"	/* KW07 */
};


/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT TAB_CHR  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	nplp_char indentationCharType;
	nplp_intg indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
nplp_intg numScannerErrors;

/* Scanner data */
ScannerData scData;

#endif
