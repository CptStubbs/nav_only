/*
 * adjustFormat.c
 *
 *  Created on: May 20, 2015
 *      Author: sean
 */

#include "adjustFormat.h"

#include <conf_nav.h>
#include "customDataTypes.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

si32 adjustFormat(pi8 * outStr, pi8 * inStr, ui32 length) {

	/* Two temporary strings for strrep output */
	pi8 * temp1;
	pi8 * temp2;
	pi8 * posIndexptr;
	pi8 * indexptr;
	pi8 printfBuffer[3];
	pi8 tempNum[3];
	ui32 posIndex = 0;
	ui32 index = 0;
	ui32 i = 0, j = 0;
	ui32 lenDiff = 0;
	/* Replace the + sign with a space */
	temp1 = strrep(inStr, " +", "  ");
	ERRORCHECK(temp1==NULL)
	/* Change exponential sign for E to D and reduce the number of digits after E */
	temp2 = strrep(temp1, "E+0", "D+");
	ERRORCHECK(temp2==NULL)
	free(temp1);
	temp1 = strrep(temp2, "E-0", "D-");
	ERRORCHECK(temp1==NULL)
	free(temp2);
	posIndexptr = strchr(temp1, 'D');
	/* Yay pointer math */
	posIndex = posIndexptr - temp1;
	temp2 = (pi8*) calloc(posIndex, sizeof(pi8));
	ERRORCHECK(temp2==NULL)
	for (i = posIndex; i <= posIndex; i++) { /*! TODO: WTF? */
		strncpy(temp2, temp1, i);
		if (atof(temp2) == 0) {
			indexptr = strchr(temp1, '.');
			if (indexptr != NULL) {
				index = indexptr - temp1;
				if (index != 0) {
					temp1[index] = temp1[index - 1];
					temp1[index - 1] = '.';
				}
			}
		} else {
			indexptr = strchr(temp1, '.');
			if (indexptr != NULL) {
				index = indexptr - temp1;
				temp1[index] = temp1[index - 1];
				temp1[index - 1] = '.';
				if (temp1[index + 1] == '+' && index + 3 < strlen(temp1)) {
					tempNum[0] = temp1[index + 2];
					tempNum[1] = temp1[index + 3];
					tempNum[2] = '\0';
					snprintf(printfBuffer, 3, "%02d", atoi(tempNum) + 1);
					temp1[index + 2] = printfBuffer[0];
					temp1[index + 3] = printfBuffer[1];
				} else if (index + 3 < strlen(temp1)) {
					tempNum[0] = temp1[index + 2];
					tempNum[1] = temp1[index + 3];
					tempNum[2] = '\0';
					snprintf(printfBuffer, 3, "%02d", atoi(tempNum) - 1);
					temp1[index + 2] = printfBuffer[0];
					temp1[index + 3] = printfBuffer[1];
				}
			}

		}
	}
	strncpy(outStr, temp1, length);
	if (strlen(outStr) < length) {
		lenDiff = length - strlen(outStr);
		for (i = 0; i < lenDiff; i++) {
			for (j = length - 1; j > i; j--) {
				outStr[j] = outStr[j - 1];
			}
			outStr[i] = ' ';
		}
		outStr[length] = '\0';
		outStr[length-1] = '\0';
	}
	free(temp1);
	free(temp2);
	return EXIT_SUCCESS;
}
