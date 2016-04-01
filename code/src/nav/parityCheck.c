/*
 * parityCheck.c
 *
 *  Created on: May 13, 2015
 *      Author: stolen from seans old code
 *
 */

#include "parityCheck.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

si32 parityCheck(
/* Output */
si8 * word, si32 * pass,
/* Input */
si32 wordLength, si8 D29Star, si8 D30Star) {

	si8 tempProd = 0;
	si32 i = 0, j = 0;
	si8 h1[24] = { 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1,
			0, 0, 1, 0 };
	si8 h2[24] = { 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0,
			1, 0, 0, 1 };
	si8 h3[24] = { 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1,
			0, 1, 0, 0 };
	si8 h4[24] = { 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1,
			1, 0, 1, 0 };
	si8 h5[24] = { 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
			1, 1, 0, 1 };
	si8 h6[24] = { 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0,
			0, 1, 1, 1 };
	si8 * H[6];

	si8 DStarMat[6];
	si8 D[6];
	si8 temp[24];

	H[0] = h1;
	H[1] = h2;
	H[2] = h3;
	H[3] = h4;
	H[4] = h5;
	H[5] = h6;

	/*! Convert the word, D29star, D30star from '0','1' to 1, -1 */
	for (i = 0; i < wordLength; i++) {
		word[i] = 1 - 2 * (word[i] - 48);

	}

	D29Star = 1 - 2 * (D29Star - 48);
	D30Star = 1 - 2 * (D30Star - 48);
	for (i = 0; i < 24; i++) {
		word[i] = word[i] * D30Star;
	}
	DStarMat[0] = D29Star;
	DStarMat[1] = D30Star;
	DStarMat[2] = D29Star;
	DStarMat[3] = D30Star;
	DStarMat[4] = D30Star;
	DStarMat[5] = D29Star;
	for (i = 0; i < 6; i++) {
		tempProd = 1;
		for (j = 0; j < 24; j++) {
			temp[j] = H[i][j] * word[j];
			if (temp[j] != 0) {
				tempProd *= temp[j];
			}
		}
		D[i] = tempProd * DStarMat[i];
	}
	/* Convert back to the strings */
	for (i = 0; i < wordLength; i++) {
		word[i] = 48 + ((1 - word[i]) / 2);
	}
	*pass = 1;
	for (i = 0, j = 24; i < 6; i++, j++) {
		D[i] = 48 + ((1 - D[i]) / 2);
		if (D[i] != word[j]) {
			*pass = 0;
		}
	}

	return EXIT_SUCCESS;
}

