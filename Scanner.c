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
* File name: Scanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
nplp_intg line;								/* Current line number of the source code */
extern nplp_intg errorNumber;				/* Defined in platy_st.c - run-time error number */

extern nplp_intg stateType[NUM_STATES];
extern nplp_string keywordTable[KWT_SIZE];

extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern nplp_intg transitionTable[NUM_STATES][CHAR_CLASSES];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
 /* TO_DO: Follow the standard and adjust datatypes */

nplp_intg startScanner(BufferPointer psc_buf) {
	/* TO_DO: Start histogram */
	for (nplp_intg i=0; i<NUM_TOKENS;i++)
		scData.scanHistogram[i] = 0;
	/* Basic scanner initialization */
	/* in case the buffer has been read previously  */
	readerRecover(psc_buf);
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states 
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(sofia_void) {

	/* TO_DO: Follow the standard and adjust datatypes */

	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	nplp_char c;			/* input symbol */
	nplp_intg state = 0;	/* initial state of the FSM */
	nplp_intg lexStart;	/* start offset of a lexeme in the input char buffer (array) */
	nplp_intg lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/

	nplp_intg lexLength;	/* token length */
	nplp_intg i;			/* counter */
	///sofia_char newc;		// new char

	/* Starting lexeme */
	nplp_string lexeme;	/* lexeme (to check the function) */
	lexeme = (nplp_string)malloc(VID_LEN * sizeof(nplp_char));
	if (!lexeme)
		return currentToken;
	lexeme[0] = EOS_CHR;

	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = readerGetChar(sourceBuffer);

		// TO_DO: Defensive programming
		if (c < 0 || c >= NCHAR)
			return currentToken;

		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/

		/* TO_DO: All patterns that do not require accepting functions */
		switch (c) {

		/* Cases for spaces */
		case S_CHR:
		case TAB_CHR:
			break;
		case NL_CHR:
			line++;
			break;

		/* Cases for symbols */

		case P1_CHR:
			currentToken.code = P1_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case P2_CHR:
			currentToken.code = P2_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case LCUR_CHR:
			currentToken.code = LCUR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case RCUR_CHR:
			currentToken.code = RCUR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		/* Cases for END OF FILE */
		case EOS_CHR:
			currentToken.code = SEOF_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		case EOF_CHR:
			currentToken.code = SEOF_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_255;
			return currentToken;

		/* ------------------------------------------------------------------------
			Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
			Note: Part 2 must follow Part 1 to catch the illegal symbols
			-----------------------------------------------------------------------
		*/

		/* TO_DO: Adjust / check the logic for your language */

		default: // general case
			state = nextState(state, c);
			lexStart = readerGetPosRead(sourceBuffer) - 1;
			readerSetMark(sourceBuffer, lexStart);
			int pos = 0;
			while (stateType[state] == NOFS) {
				c = readerGetChar(sourceBuffer);
				state = nextState(state, c);
				pos++;
			}
			if (stateType[state] == FSWR)
				readerRetract(sourceBuffer);
			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = readerCreate((nplp_intg)lexLength + 2, 0, MODE_FIXED);
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Can not create buffer\n");
				exit(1);
			}
			readerRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
			readerAddChar(lexemeBuffer, READER_TERMINATOR);
			lexeme = readerGetContent(lexemeBuffer, 0);
			// TO_DO: Defensive programming
			if (!lexeme)
				return currentToken;
			currentToken = (*finalStateTable[state])(lexeme);
			readerRestore(lexemeBuffer);
			return currentToken;
		} // switch

	} //while

} // tokenizer


/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */
 /* TO_DO: Just change the datatypes */

nplp_intg nextState(nplp_intg state, nplp_char c) {
	nplp_intg col;
	nplp_intg next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != FS);
	if (DEBUG)
		if (next == FS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */
/* TO_DO: Use your column configuration */

/* Adjust the logic to return next column in TT */
/*    [A-z],[0-9],    _,     ,~     , \n   , #   ,'    ,    ( ,     ",  )  ,
	   L(0), D(1), U(2), S(3),DEF(4), NL(5), H(6),SQ(7), P1(8),  Q(9),  P2(10) */

nplp_intg nextClass(nplp_char c) {
	if (isalpha(c)) return 0;    // L (Letters [A-z])
	if (isdigit(c)) return 1;    // D (Digits [0-9])
	if (c == U_CHR) return 2;  // U (_)
	if (c == S_CHR) return 3;  // U (_)
	if (c == TIL_CHR) return 4;  // DEF (~)
	if (c == NL_CHR) return 5;  // NL (\n)
	if (c == H_CHR) return 6;  // H (#)
	if (c == SGQ_CHR) return 7;  // SQ (')
	if (c == P1_CHR) return 8;  // P1 ( ( )
	if (c == DBQ_CHR) return 9;  // Q (")
	if (c == P2_CHR) return 10; // P2 ()) 
	return 4; // Default to DEF (other characters)
}

/*
 ************************************************************
 * Acceptance State Function SINGLE COM
 *		Function responsible to identify SINGLE COM (comments).
 ***********************************************************
 */
 /* TO_DO: Adjust the function for IL */

Token funcCMT(nplp_string lexeme) {
	Token currentToken = { 0 };
	// Since the comment ends at \n, the newline is handled in tokenizer (line++ on NWL_CHR)
	// We don't need to process the lexeme for newlines here.
	currentToken.code = SCMT_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}

/*
 ************************************************************
 * Acceptance State Function MULTI COM
 *		Function responsible to identify MULTI COM (comments).
 ***********************************************************
 */
Token funcMLC(nplp_string lexeme) {
	Token currentToken = { 0 };
	nplp_intg i = 0, len = (nplp_intg)strlen(lexeme);

	// Count newlines to update the line number
	for (i = 0; i < len; i++) {
		if (lexeme[i] == NL_CHR) {
			line++;
		}
	}

	currentToken.code = MCMT_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}

 /*
  ************************************************************
  * Acceptance State Function IL
  *		Function responsible to identify IL (integer literals).
  * - It is necessary respect the limit (ex: 2-byte integer in C).
  * - In the case of larger lexemes, error shoul be returned.
  * - Only first ERR_LEN characters are accepted and eventually,
  *   additional three dots (...) should be put in the output.
  ***********************************************************
  */
  /* TO_DO: Adjust the function for IL */

Token funcIL(nplp_string lexeme) {
	Token currentToken = { 0 };
	nplp_long tlong;
	if (lexeme[0] != EOS_CHR && strlen(lexeme) > NUM_LEN) {
		currentToken = (*finalStateTable[ESNR])(lexeme);
	}
	else {
		tlong = atol(lexeme);
		if (tlong >= 0 && tlong <= SHRT_MAX) {
			currentToken.code = D_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.intValue = (nplp_intg)tlong;
		}
		else {
			currentToken = (*finalStateTable[ESNR])(lexeme);
		}
	}
	return currentToken;
}


/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for ID */

Token funcID(nplp_string lexeme) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);
	nplp_char firstch = lexeme[0];
	nplp_intg isID = FALSE;
	switch (firstch) {
	case TIL_CHR:
		currentToken.code = DEF_T;
		scData.scanHistogram[currentToken.code]++;
		isID = TRUE;
		break;
	default:
		currentToken = funcKEY(lexeme);
		break;

	}
	if (isID == TRUE) {
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;
	}

	return currentToken;
}

/*
 ************************************************************
 * Acceptance State Function VAR
 *		Function responsible to identify VAR (variables).
 ***********************************************************
 */
Token funcVAR(nplp_string lexeme) {
	Token currentToken = { 0 };

	// Copy the lexeme into the token's idLexeme attribute
	strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
	currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;

	currentToken.code = VAR_T;
	scData.scanHistogram[currentToken.code]++;

	return currentToken;
}

/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table 
 *   (stringLiteralTable). You need to include the literals in 
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */
/* TO_DO: Adjust the function for SL */

Token funcSL(nplp_string lexeme) {
	Token currentToken = { 0 };
	nplp_intg i = 0, len = (nplp_intg)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == NL_CHR)
			line++;
		if (!readerAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RTE_T;
			scData.scanHistogram[currentToken.code]++;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	if (!readerAddChar(stringLiteralTable, EOS_CHR)) {
		currentToken.code = RTE_T;
		scData.scanHistogram[currentToken.code]++;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	currentToken.code = L_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Keywords */


Token funcKEY(nplp_string lexeme) {

	Token currentToken = { 0 };
	nplp_intg kwindex = -1, j = 0;
	nplp_intg len = (nplp_intg)strlen(lexeme);
	///lexeme[len - 1] = EOS_CHR;
	for (j = 0; j < KWT_SIZE; j++)
		if (!strcmp(lexeme, &keywordTable[j][0]))
			kwindex = j;

	if (kwindex != -1) {
		currentToken.code = K_T;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.codeType = kwindex;
	}
	else {
		currentToken = funcVAR(lexeme);
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Errors */

Token funcErr(nplp_string lexeme) {
	Token currentToken = { 0 };
	nplp_intg i = 0, len = (nplp_intg)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = EOS_CHR;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == NL_CHR)
			line++;
	currentToken.code = ERR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

nplp_void printToken(Token t) {
	extern nplp_string keywordTable[]; /* link to keyword table in */
	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case DEF_T:
		printf("DEF_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case VAR_T:
		printf("VAR_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case L_T:
		printf("L_T\t\t%d\t ", (nplp_intg)t.attribute.codeType);
		printf("%s\n", readerGetContent(stringLiteralTable, (nplp_intg)t.attribute.codeType));
		break;
	case D_T:
		printf("D_T\t\t%d\n", t.attribute.intValue);
		break;
	case P1_T:
		printf("P1_T\n");
		break;
	case P2_T:
		printf("P2_T\n");
		break;
	case LCUR_T:
		printf("LCUR_T\n");
		break;
	case RCUR_T:
		printf("RCUR_T\n");
		break;
	case K_T:
		printf("K_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case SCMT_T:
		printf("SCMT_T\n");
		break;
	case MCMT_T:
		printf("MCMT_T\n");
		break;

	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

/*
 ************************************************************
 * The function prints statistics of tokens
 * Param:
 *	- Scanner data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
nplp_void printScannerData(ScannerData scData) {
	/* Print Scanner statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_TOKENS; cont++) {
		if (scData.scanHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", tokenStrTable[cont], "]=", scData.scanHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}

/*
TO_DO: (If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/
