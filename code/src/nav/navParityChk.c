/*
 * navParityChk.c
 *
 *  Created on: May 18, 2015
 *      Author: sara
 */

#include "navParityChk.h"

#include <conf_nav.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void navParityChk(
/* Output */
si32 * status,
/* Input */
si32 * ndat) {

	si32 inda;
	si32 indb;
	si32 sumParity;
	si32 sumNdat;
	si32 parity[6];
	/* In order to accomplish the exclusive or operation using multiplication
	 this program represents a '0' with a '-1' and a '1' with a '1' so that
	 the exclusive or table holds true for common data operations

	 a	b	xor 			a	b	product
	 --------------          -----------------
	 0	0	 1			   -1  -1	   1
	 0	1	 0			   -1   1	  -1
	 1	0	 0			    1  -1	  -1
	 1	1	 1			    1   1	   1*/

	/*--- Check if the data bits must be inverted ------------------------------*/
	if (ndat[1] != 1) {
		for (inda = 2; inda < 26; inda++) {
			ndat[inda] = -ndat[inda];
		}/*end of for loop*/
	}/*end of if statement*/

	/*--- Calculate 6 parity bits ----------------------------------------------
	 The elements of the ndat array correspond to the bits showed in the table
	 20-XIV (ICD-200C document) in the following way:
	 The first element in the ndat is the D29* bit and the second - D30*.
	 The elements 3 - 26 are bits d1-d24 in the table.
	 The elements 27 - 32 in the ndat array are the received bits D25-D30.
	 The array "parity" contains the computed D25-D30 (parity) bits.*/

	parity[0] = ndat[1 - 1] * ndat[3 - 1] * ndat[4 - 1] * ndat[5 - 1]
			* ndat[7 - 1] * ndat[8 - 1] * ndat[12 - 1] * ndat[13 - 1]
			* ndat[14 - 1] * ndat[15 - 1] * ndat[16 - 1] * ndat[19 - 1]
			* ndat[20 - 1] * ndat[22 - 1] * ndat[25 - 1];

	parity[1] = ndat[2 - 1] * ndat[4 - 1] * ndat[5 - 1] * ndat[6 - 1]
			* ndat[8 - 1] * ndat[9 - 1] * ndat[13 - 1] * ndat[14 - 1]
			* ndat[15 - 1] * ndat[16 - 1] * ndat[17 - 1] * ndat[20 - 1]
			* ndat[21 - 1] * ndat[23 - 1] * ndat[26 - 1];

	parity[2] = ndat[1 - 1] * ndat[3 - 1] * ndat[5 - 1] * ndat[6 - 1]
			* ndat[7 - 1] * ndat[9 - 1] * ndat[10 - 1] * ndat[14 - 1]
			* ndat[15 - 1] * ndat[16 - 1] * ndat[17 - 1] * ndat[18 - 1]
			* ndat[21 - 1] * ndat[22 - 1] * ndat[24 - 1];

	parity[3] = ndat[2 - 1] * ndat[4 - 1] * ndat[6 - 1] * ndat[7 - 1]
			* ndat[8 - 1] * ndat[10 - 1] * ndat[11 - 1] * ndat[15 - 1]
			* ndat[16 - 1] * ndat[17 - 1] * ndat[18 - 1] * ndat[19 - 1]
			* ndat[22 - 1] * ndat[23 - 1] * ndat[25 - 1];

	parity[4] = ndat[2 - 1] * ndat[3 - 1] * ndat[5 - 1] * ndat[7 - 1]
			* ndat[8 - 1] * ndat[9 - 1] * ndat[11 - 1] * ndat[12 - 1]
			* ndat[16 - 1] * ndat[17 - 1] * ndat[18 - 1] * ndat[19 - 1]
			* ndat[20 - 1] * ndat[23 - 1] * ndat[24 - 1] * ndat[26 - 1];

	parity[5] = ndat[1 - 1] * ndat[5 - 1] * ndat[7 - 1] * ndat[8 - 1]
			* ndat[10 - 1] * ndat[11 - 1] * ndat[12 - 1] * ndat[13 - 1]
			* ndat[15 - 1] * ndat[17 - 1] * ndat[21 - 1] * ndat[24 - 1]
			* ndat[25 - 1] * ndat[26 - 1];

	/*--- Compare if the received parity is equal the calculated parity --------*/
	sumParity = 0;
	sumNdat = 0;
	for (indb = 0; indb < 6; indb++) {
		sumParity += parity[indb];
		sumNdat += ndat[26 + indb];

	}

	if (sumParity == sumNdat) {
		/* Parity is OK. Function output is -1 or 1 depending if the data bits
		 must be inverted or not. The "ndat(2)" is D30* bit - the last  bit of
		 previous subframe.*/
		*status = -1 * ((si32) ndat[1]);
	} else {
		/*Parity Failure*/
		//printf("Nav Parity Failure! \n");
		*status = 0;
	}

}/*end of function*/
