/*
 * leastSquareVel2.c
 *
 *  Created on: May 13, 2015
 *      Author: sean
 */

#include "leastSquareVel2.h"

#include <conf_nav.h>
#include "customDataTypes.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "utils.h"

si32 leastSquareVel2(fl64 ** vel, fl64 * transmitFreq, fl64 * receivedFreq,
		fl64 ** satPositions, fl64 ** satVelocity, fl64 userPos[3],
		si32 * activeChnList, si32 activeChnListSize) {

	/* Declarations */
	fl64 ** losVector;
	fl64 ** H;
	fl64 ** Ht;
	fl64 ** HtxH;
	fl64 ** HtxH_Inv;
	fl64 ** HtxH_InvxHt;
	si32 i = 0, channelNr = 0;
	si32 number = 0;
	si32 HColLength = 4;
	fl64 vecNorm = 0;
	fl64 tempVec[3];
	fl64 tempLosVector[3];
	fl64 tempSatVelocity[3];
	fl64 ** y;
	/* Code */
	/*! Setup y */
	y = (fl64 **) malloc(activeChnListSize * sizeof(fl64 *));
	ERRORCHECK(y == NULL)
	for (i = 0; i < activeChnListSize; i++) {
		y[i] = (fl64 *) malloc(1 * sizeof(fl64));
		ERRORCHECK(y[i] == NULL)

	}
	/*! Setup losVector */
	losVector = (fl64 **) malloc(3 * sizeof(fl64 *));
	ERRORCHECK(losVector == NULL)
	for (i = 0; i < 3; i++) {
		losVector[i] = (fl64 *) malloc(activeChnListSize * sizeof(fl64));
		ERRORCHECK(losVector[i] == NULL)
	}
	/*! Setup H */
	H = (fl64 **) malloc(activeChnListSize * sizeof(fl64 *));
	ERRORCHECK(H == NULL)
	for (i = 0; i < activeChnListSize; i++) {
		H[i] = (fl64 *) malloc(HColLength * sizeof(fl64));
		ERRORCHECK(H[i] == NULL)

	}
	/*! Setup Ht */
	Ht = (fl64 **) malloc(HColLength * sizeof(fl64 *));
	ERRORCHECK(Ht == NULL)
	for (i = 0; i < HColLength; i++) {
		Ht[i] = (fl64 *) malloc(activeChnListSize * sizeof(fl64));
		ERRORCHECK(Ht[i] == NULL)
	}
	/*! Setup HtH */
	HtxH = (fl64 **) malloc(HColLength * sizeof(fl64 *));
	ERRORCHECK(HtxH == NULL)
	for (i = 0; i < HColLength; i++) {
		HtxH[i] = (fl64 *) malloc(HColLength * sizeof(fl64));
		ERRORCHECK(HtxH[i] == NULL)
	}

	/*! Setup HtH inverse */
	HtxH_Inv = (fl64 **) malloc(HColLength * sizeof(fl64 *));
	ERRORCHECK(HtxH_Inv == NULL)
	for (i = 0; i < HColLength; i++) {
		HtxH_Inv[i] = (fl64 *) malloc(HColLength * sizeof(fl64));
		ERRORCHECK(HtxH_Inv[i] == NULL)
	}
	/*! Setup HtH inverse ldivide Ht */
	HtxH_InvxHt = (fl64 **) malloc(HColLength * sizeof(fl64 *));
	ERRORCHECK(HtxH_InvxHt == NULL)
	for (i = 0; i < HColLength; i++) {
		HtxH_InvxHt[i] = (fl64 *) malloc(activeChnListSize * sizeof(fl64));
		ERRORCHECK(HtxH_InvxHt[i] == NULL)
	}

	/* Actual code */
	for (i = 0; i < 3; i++) {
		for (channelNr = 0; channelNr < activeChnListSize; channelNr++) {
			number = activeChnList[channelNr];
			tempVec[0] = satPositions[0][number] - userPos[0];
			tempVec[1] = satPositions[1][number] - userPos[1];
			tempVec[2] = satPositions[2][number] - userPos[2];
			vecNorm = norm(tempVec, 3);
			losVector[i][channelNr] = (satPositions[i][number] - userPos[i])
					/ vecNorm;
		}
	}
	for (channelNr = 0; channelNr < activeChnListSize; channelNr++) {
		number = activeChnList[channelNr];
		/* Confirm that this is correct ... */
		for (i = 0; i < 3; i++) {
			tempLosVector[i] = losVector[i][channelNr];
			tempSatVelocity[i] = satVelocity[i][number];
		}
		y[channelNr][0] = (SPEEDOFLIGHT
				* (receivedFreq[number] - transmitFreq[number])
				/ transmitFreq[number])
				+ dot(tempSatVelocity, tempLosVector, 3);
	}
	/*! Line 33 of the matlab */
	for (i = 0; i < 4; i++) {
		for (channelNr = 0; channelNr < activeChnListSize; channelNr++) {
			number = activeChnList[channelNr];
			if (i == 3) {
				Ht[i][channelNr] = -receivedFreq[number] / transmitFreq[number];
				H[channelNr][i] = Ht[i][channelNr];
			} else {
				H[channelNr][i] = losVector[i][channelNr];
				Ht[i][channelNr] = losVector[i][channelNr];
			}
		}
	}
	if (matMult(HtxH, Ht, HColLength, activeChnListSize, H, activeChnListSize,
			HColLength) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	if (matInverse(HtxH_Inv, HtxH, HColLength) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	if (matMult(HtxH_InvxHt, HtxH_Inv, HColLength, HColLength, Ht, HColLength,
			activeChnListSize) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	if (matMult(vel, HtxH_InvxHt, HColLength, activeChnListSize, y,
			activeChnListSize, 1) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	for (i = 0; i < 3; i++) {
		free(losVector[i]);
	}
	for (i = 0; i < HColLength; i++) {
		free(HtxH_InvxHt[i]);
		free(HtxH_Inv[i]);
		free(HtxH[i]);
		free(Ht[i]);
	}
	for (i = 0; i < activeChnListSize; i++) {
		free(H[i]);
		free(y[i]);
	}
	free(HtxH_InvxHt);
	free(HtxH_Inv);
	free(HtxH);
	free(Ht);
	free(H);
	free(losVector);
	free(y);
	return EXIT_SUCCESS;
}
