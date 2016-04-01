/*
 * twosComp2dec.c
 *
 *  Created on: May 18, 2015
 *      Author: sara
 */
#include "customDataTypes.h"
#include "twosComp2dec.h"

#include <conf_nav.h>
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

si32 twosComp2dec(
/* Output */
fl64 * intNumber,
/* Input */
pi8 * binaryNumber, si32 binaryNumberSize) {

	si32 inda = 0;
	si32 indaPos = 0;
	si32 rem = 0;
	si64 n = 0;
	pi8 * tempBuffer;
	si32 tempNumberSize = binaryNumberSize;
	/* 2^32 = 10^9 */
	if (binaryNumberSize > LONGLENGTH) {
		inda = 0;
		while (tempNumberSize > 0) {
			if (tempNumberSize > LONGLENGTH) {
				tempBuffer = malloc((LONGLENGTH + 1) * sizeof(pi8));
				ERRORCHECK(tempBuffer == NULL)
				memcpy(tempBuffer, &binaryNumber[tempNumberSize - LONGLENGTH],
				LONGLENGTH * sizeof(pi8));
				tempBuffer[LONGLENGTH] = '\0';
				tempNumberSize -= LONGLENGTH;
			} else {
				tempBuffer = malloc((tempNumberSize + 1) * sizeof(pi8));
				ERRORCHECK(tempBuffer == NULL)
				memcpy(tempBuffer, binaryNumber, tempNumberSize * sizeof(pi8));
				tempBuffer[tempNumberSize] = '\0';
				tempNumberSize = 0;
				/* Only when the first digit is one does inda get calculated wrong */
			}
			n = atol(tempBuffer);
			/*Convert binary to decimal, tested and should work */
			while (n != 0) {
				rem = n % 10;/*TODO FIX so that it takes leading zeros!!!!*/
				n /= 10;
				*intNumber += rem * pow(2, inda);
				inda++;
			}
			/* If the last digit of tempBuffer is 0 then we need to increment it */
			indaPos = 0;
			while (tempBuffer[indaPos] == '0') {
				inda++;
				indaPos++;
			}

			free(tempBuffer);
		}
		if (binaryNumber[0] == '1') {
			*intNumber = *intNumber - pow(2, binaryNumberSize);
		}
	} else {
		n = atol(binaryNumber);
		inda = 0;
		/*Convert binary to decimal, tested and should work */
		while (n != 0) {
			rem = n % 10;/*TODO FIX so that it takes leading zeros!!!!*/
			n /= 10;
			*intNumber += rem * pow(2, inda);
			++inda;
		}/*end while loop*/

		if (binaryNumber[0] == '1') {
			*intNumber = *intNumber - pow(2, binaryNumberSize);
		}
	}
	return EXIT_SUCCESS;

}/*end function*/
