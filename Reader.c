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
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#include <ctype.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

 /*
 ***********************************************************
 * Function name: readerCreate
 * Purpose: Creates the buffer reader according to capacity, increment
	 factor and operational mode ('f', 'a', 'm')
 * Author: Svillen Ranev / Paulo Sousa
 * History/Versions: S22
 * Called functions: calloc(), malloc()
 * Parameters:
 *   size = initial capacity
 *   increment = increment factor
 *   mode = operational mode
 * Return value: bPointer (pointer to reader)
 * Algorithm: Allocation of memory according to inicial (default) values.
 * TODO ......................................................
 *	- Adjust datatypes for your LANGUAGE.
 *   - Use defensive programming
 *	- Check boundary conditions
 *	- Check flags.
 *************************************************************
 */

BufferPointer readerCreate(nplp_intg size, nplp_intg increment, nplp_char mode) {
	//int i;
	BufferPointer readerPointer;
	nplp_intg count = 0;

	/* TO_DO: Defensive programming */
	if (!size)
		size = READER_DEFAULT_SIZE;
	if (!increment)
		increment = READER_DEFAULT_INCREMENT;
	if (!mode)
		mode = MODE_FIXED;

	readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));
	if (!readerPointer)
		return INVALID;

	readerPointer->content = (nplp_string)malloc(size);
	/* TO_DO: Defensive programming */
	if (!readerPointer->content)
		return INVALID;

	/* TO_DO: Initialize the histogram */
	for (int i = 0; i < NCHAR; i++) {
		readerPointer->histogram[i] = 0;
	}
	/* TO_DO: Initialize errors */
	readerPointer->mode = mode;
	readerPointer->size = size;
	readerPointer->increment = increment;
	/* TO_DO: Initialize flags */
	readerPointer->flags.isEmpty = TRUE;
	readerPointer->flags.isFull = FALSE;
	readerPointer->flags.isMoved = FALSE;
	readerPointer->flags.isRead = FALSE;
	/* TO_DO: Default checksum */
	readerPointer->checksum = 0;
	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

BufferPointer readerAddChar(BufferPointer readerPointer, nplp_char ch) {
	nplp_string tempReader = INVALID;
	nplp_intg newSize = 0;
	nplp_char tempChar = ' ';

	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return ERROR;
	/* TO_DO: Reset Realocation */
	readerPointer->flags.isMoved = FALSE;
	/* TO_DO: Test the inclusion of chars */
	if (readerPointer->positions.wrte * (nplp_intg)sizeof(nplp_char) < readerPointer->size) {
		/* TO_DO: This buffer is NOT full */
		readerPointer->flags.isFull = FALSE; 
	}
	else {
		/* TO_DO: Reset Full flag */
		readerPointer->flags.isFull = TRUE;
		switch (readerPointer->mode) {
		case MODE_FIXED:
			/* TO_DO: Update the last position with Terminator */
			return INVALID;
			break;
		case MODE_ADDIT:
			/* TO_DO: Update size for Additive mode */
			newSize = readerPointer->size + readerPointer->increment;
			/* TO_DO: Defensive programming */
			if (newSize < readerPointer->size)
				return INVALID;
			break;
		case MODE_MULTI:
			/* TO_DO: Update size for Additive mode */
			newSize = readerPointer->size * readerPointer->increment;
			/* TO_DO: Defensive programming */
			if (newSize < readerPointer->size)
				return INVALID;
			break;
		default:
			return INVALID;
		}
		
			
		/* TO_DO: Reallocate */
		tempReader = (nplp_string)realloc(readerPointer->content, newSize);
		/* TO_DO: Defensive programming */
		if (!tempReader)
			return INVALID;
		readerPointer->flags.isFull = FALSE;
		readerPointer->size = newSize;
		readerPointer->content = tempReader;
		return readerPointer;
	}
	/* TO_DO: Update the flags */
	readerPointer->content[readerPointer->positions.wrte++] = ch;

	/* TO_DO: Updates histogram */
	readerPointer->histogram[(nplp_intg)ch]++;
	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_boln readerClear(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return ERROR;
	/* TO_DO: Adjust the write, mark and read to zero */
	readerPointer->positions.mark = 0;
	readerPointer->positions.wrte = 0;
	readerPointer->positions.read = 0;
	/* TO_DO: Adjust flags */
	readerPointer->flags.isMoved = FALSE;
	readerPointer->flags.isEmpty = TRUE;
	readerPointer->flags.isFull = FALSE;
	readerPointer->flags.isRead = FALSE;
	
	return TRUE;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_boln readerFree(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return FALSE;
	/* TO_DO: Free pointers */
	if (readerPointer->content) {
		free(readerPointer->content);  // Free the dynamically allocated content
	}
	return TRUE;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_boln readerIsFull(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return ERROR;
	/* TO_DO: Check flag if buffer is FUL */
	if (readerPointer->flags.isFull)
		return TRUE;
	return FALSE;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_boln readerIsEmpty(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		//return NPLP_FALSE;
		return ERROR; //returning error
	/* TO_DO: Check flag if buffer is EMP */
	if (readerPointer->flags.isEmpty)
		return TRUE;
	return FALSE;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_boln readerSetMark(BufferPointer const readerPointer, nplp_intg mark) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return ERROR;
	/* TO_DO: Adjust mark */
	if (mark < 0 || mark >= readerPointer->size) {
		return FALSE;  // Mark is out of bounds
	}
	readerPointer->positions.mark = mark;
	return TRUE;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_intg readerPrint(BufferPointer const readerPointer) {
	nplp_intg cont = 0;
	nplp_char c;
	/* TO_DO: Defensive programming (including invalid chars) */
	if (!readerPointer)
		return ERROR;

	c = readerGetChar(readerPointer);
	while (cont < readerPointer->positions.wrte) {
		cont++;
		printf("%c", c);
		c = readerGetChar(readerPointer);
	}
	return cont;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_intg readerLoad(BufferPointer readerPointer, FILE* const fileDescriptor) {
	nplp_intg size = 0;
	nplp_char c;
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return ERROR;

	while (!feof(fileDescriptor)) {
		c = (nplp_char)fgetc(fileDescriptor);
		readerPointer = readerAddChar(readerPointer, c);
		size++;
	}
	/* TO_DO: Defensive programming */
	return size;
}

/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_boln readerRecover(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return ERROR;

	/* TO_DO: Recover positions: read and mark must be zero */
	readerPointer->positions.mark = 0;
	readerPointer->positions.read = 0;
	/* TO_DO: Update flags */
	readerPointer->flags.isRead = FALSE;
	return TRUE;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_boln readerRetract(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return FALSE;

	/* TO_DO: Retract (return 1 pos read) */
	if (readerPointer->positions.read <= 0)
		return FALSE;

	/* Retract the read position by 1 */
	readerPointer->positions.read--;

	/* Update flags */
	readerPointer->flags.isRead = FALSE;  // Reset isRead flag

	return TRUE;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_boln readerRestore(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return ERROR;
	/* TO_DO: Restore positions (read to mark) */
	readerPointer->positions.read = readerPointer->positions.mark;
	return TRUE;
}



/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_char readerGetChar(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return ERROR;
	/* TO_DO: Check condition to read/wrte */
	if (readerPointer->positions.read >= readerPointer->positions.wrte)
		readerPointer->flags.isRead = TRUE;

	return readerPointer->content[readerPointer->positions.read++];
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_string readerGetContent(BufferPointer const readerPointer, nplp_intg pos) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return INVALID;

	if (pos < 0 || pos >= readerPointer->positions.wrte)
		return INVALID;
	return readerPointer->content + pos;
}



/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_intg readerGetPosRead(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return ERROR;
	/* TO_DO: Return read */
	return readerPointer->positions.read;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_intg readerGetPosWrte(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return ERROR;
	/* TO_DO: Return wrte */
	return readerPointer->positions.wrte;
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_intg readerGetPosMark(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return ERROR;
	/* TO_DO: Return mark */
	return readerPointer->positions.mark;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_intg readerGetSize(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return ERROR;
	/* TO_DO: Return size */
	return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_intg readerGetInc(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return ERROR;
	/* TO_DO: Return increment */
	return readerPointer->increment;
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_char readerGetMode(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return ERROR;
	/* TO_DO: Return mode */
	//return '\0';
	return readerPointer->mode;
}

/*
***********************************************************
* Function name: readerShowStat
* Purpose: Shows the char statistic.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_void readerPrintStat(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return;
	/* TO_DO: Updates the histogram */
	for (int i = 0; i < NCHAR; i++) {
		if (readerPointer->histogram[i] > 0) {
			/* Print the character and its frequency */
			if (i >= 32 && i <= 126) {  // Printable ASCII characters
				printf("B[%c]=%d\t", (char)i, readerPointer->histogram[i]);
			}
			else {  // Non-printable characters
				printf("B[]=%d\t", readerPointer->histogram[i]);
			}
		}
	}printf("\n");
}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
nplp_intg readerGetNumErrors(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return ERROR;
	/* TO_DO: Returns the number of errors */
	return readerPointer->numReaderErrors;
}

/*
***********************************************************
* Function name: readerCalcChecksum
* Purpose: Calculates the checksum of the reader (8 bits).
* Parameters: m
*   readerPointer = pointer to Buffer Reader
* Return value:
*	[None]
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

nplp_void readerCalcChecksum(BufferPointer readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Calculate checksum */

	//we didn't do this function because there's no specification for this function
}

/*
***********************************************************
* Function name: readerPrintFlags
* Purpose: Sets the checksum of the reader (4 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	[None]
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

nplp_boln readerPrintFlags(BufferPointer readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		printf("00\n"); 
		return FALSE; 
	}

	printf("%d%d\n",
		readerPointer->flags.isEmpty,
		readerPointer->flags.isFull);


	return TRUE;
}
