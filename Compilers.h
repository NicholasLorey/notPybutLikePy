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
* File name: Compilers.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: Sep 01 2024
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainReader(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/* TO_DO: Adjust your language (cannot be "Sofia") */

/* TO_DO: Language name */
#define STR_LANGNAME	"notPybutLikePy"

/* TO_DO: Logical constants - adapt for your language */
#define TRUE		1
#define FALSE		0
#define ERROR		(-1)	/* General error message */

#define INVALID	NULL

/*
------------------------------------------------------------
Data types definitions
NOTE: Some types may not be directly used by your language,
		but they can be necessary for conversions.
------------------------------------------------------------
*/


/* TO_DO: Define your typedefs */
typedef char			nplp_char;
typedef char*			nplp_string;
typedef int				nplp_intg;
typedef float			nplp_real;
typedef void			nplp_void;

typedef char			nplp_boln;
typedef unsigned char	nplp_byte;

typedef long			nplp_long;
typedef double			nplp_doub;

/*
------------------------------------------------------------
Programs:
1: Reader - invokes MainReader code
2: Scanner - invokes MainScanner code
3: Parser - invokes MainParser code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_READER  = '1',
	PGM_SCANNER = '2',
	PGM_PARSER  = '3'
};

/*
------------------------------------------------------------
Main functions signatures
(Code will be updated during next assignments)
------------------------------------------------------------
*/
nplp_intg mainReader(nplp_intg argc, nplp_string* argv);
/* TO_DO: Remove comment in next assignments */

nplp_intg mainScanner(nplp_intg argc, nplp_string* argv);
//nplp_intg mainParser(nplp_intg argc, nplp_string* argv);


nplp_void printLogo();

#endif
