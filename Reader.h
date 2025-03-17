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
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'f', /* Fixed mode (constant size) */
	MODE_ADDIT = 'a', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'm'  /* Multiplicative mode (constant increment to be multiplied) */
};


/* Constants about controls (not need to change) */
#define READER_TERMINATOR	'\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */

#define NCHAR				128			/* Chars from 0 to 127 */

#define CHARSEOF			(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct flag {
	nplp_boln isEmpty;			/* indicates if the buffer is empty */
	nplp_boln isFull;			/* indicates if the buffer is full */
	nplp_boln isRead;			/* indicates if the buffer was completely read */
	nplp_boln isMoved;			/* indicates if the buffer memory was changed */
} Flag;

/* Offset declaration */
typedef struct position {
	nplp_intg wrte;			/* the offset to the add chars (in chars) */
	nplp_intg read;			/* the offset to the get a char position (in chars) */
	nplp_intg mark;			/* the offset to the mark position (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	nplp_string		content;			/* pointer to the beginning of character array (character buffer) */
	nplp_intg		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	nplp_intg		increment;			/* character array increment factor */
	nplp_char		mode;				/* operational mode indicator */
	Flag			flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		positions;			/* Offset / position field */
	nplp_intg		histogram[NCHAR];	/* Statistics of chars */
	nplp_intg		numReaderErrors;	/* Number of errors from Reader */
	nplp_byte		checksum;
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer	readerCreate		(nplp_intg, nplp_intg, nplp_char);
BufferPointer	readerAddChar		(BufferPointer const, nplp_char);
nplp_boln		readerClear		    (BufferPointer const);
nplp_boln		readerFree		    (BufferPointer const);
nplp_boln		readerIsFull		(BufferPointer const);
nplp_boln		readerIsEmpty		(BufferPointer const);
nplp_boln		readerSetMark		(BufferPointer const, nplp_intg);
nplp_intg		readerPrint		    (BufferPointer const);
nplp_intg		readerLoad			(BufferPointer const, FILE* const);
nplp_boln		readerRecover		(BufferPointer const);
nplp_boln		readerRetract		(BufferPointer const);
nplp_boln		readerRestore		(BufferPointer const);
nplp_void		readerCalcChecksum	(BufferPointer const);
nplp_boln		readerPrintFlags	(BufferPointer const);
nplp_void		readerPrintStat     (BufferPointer const);
/* Getters */
nplp_char		readerGetChar		(BufferPointer const);
nplp_string	readerGetContent	(BufferPointer const, nplp_intg);
nplp_intg		readerGetPosRead	(BufferPointer const);
nplp_intg		readerGetPosWrte	(BufferPointer const);
nplp_intg		readerGetPosMark	(BufferPointer const);
nplp_intg		readerGetSize		(BufferPointer const);
nplp_intg		readerGetInc		(BufferPointer const);
nplp_char		readerGetMode		(BufferPointer const);
nplp_intg		readerGetNumErrors	(BufferPointer const);

#endif
