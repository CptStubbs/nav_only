/*
 * leastSquareVel1.c
 *
 *  Created on: May 18, 2015
 *      Author: sean
 */

#include "utils.h"
#include "customDataTypes.h"
#include "leastSquareVel1.h"

#include <conf_nav.h>
#include <stdio.h>
#include <stdlib.h>

si32 leastSquareVel1(fl64 ** vel, navSolutions_s navSolutions,
		fl64 * carrPhaseDiff, si32 * activeChnList, si32 activeChnListSize) {

	fl64 ** losVectorCurr;
	fl64 ** losVectorLast;
	fl64 * svDopp;
	fl64 * deltaGeo;
	/*! Satellite and user position for the previous epoch; */
	fl64 ** satPosLast;
	fl64 * userPosLast;
	/*! Satellite and user position for the current epoch */
	fl64 ** satPosCurr;
	fl64 * userPosCurr;
	fl64 deltaTime = 0;
	fl64 posCurrNorm = 0;
	fl64 posLastNorm = 0;
	si32 i = 0, channelNr = 0;
	si32 number = 0;
	/*! All of the h vectors */
	fl64 ** H;
	fl64 ** Ht;
	fl64 ** HtxH;
	fl64 ** HtxH_Inv;
	fl64 ** HtxH_InvxHt;
	fl64 ** y;
	/*! Placeholder vectors */
	fl64 tempForNorm[3];
	fl64 losCurrTempDot[3];
	fl64 losLastTempDot[3];
	fl64 satCurrTempDot[3];
	fl64 satLastTempDot[3];

	si32 HColLength = 4;

	/*! Setup svdopp and deltageo*/
	svDopp = (fl64 *) malloc(activeChnListSize * sizeof(fl64));
	ERRORCHECK(svDopp == NULL)
	deltaGeo = (fl64 *) malloc(activeChnListSize * sizeof(fl64));
	ERRORCHECK(deltaGeo == NULL)

	/*! Setup userPos last and current */
	userPosLast = (fl64 *) malloc(3 * sizeof(fl64));
	ERRORCHECK(userPosLast == NULL)
	userPosCurr = (fl64 *) malloc(3 * sizeof(fl64));
	ERRORCHECK(userPosCurr == NULL)

	/*! Setup satPos Last and current */
	satPosLast = (fl64 **) malloc(3 * sizeof(fl64*));
	ERRORCHECK(satPosLast== NULL)
	for (i = 0; i < 3; i++) {
		satPosLast[i] = malloc(activeChnListSize * sizeof(fl64));
		ERRORCHECK(satPosLast[i]== NULL)
	}
	satPosCurr = (fl64 **) malloc(3 * sizeof(fl64*));
	ERRORCHECK(satPosCurr== NULL)
	for (i = 0; i < 3; i++) {
		satPosCurr[i] = malloc(activeChnListSize * sizeof(fl64));
		ERRORCHECK(satPosCurr[i]== NULL)
	}

	/*! Setup y*/
	y = (fl64 **) malloc(activeChnListSize * sizeof(fl64 *));
	ERRORCHECK(y == NULL)
	for (i = 0; i < activeChnListSize; i++) {
		y[i] = (fl64 *) malloc(1 * sizeof(fl64));
		ERRORCHECK(y[i] == NULL)

	}
	/*! Setup losVector */
	losVectorCurr = (fl64 **) malloc(3 * sizeof(fl64 *));
	ERRORCHECK(losVectorCurr == NULL)
	for (i = 0; i < 3; i++) {
		losVectorCurr[i] = (fl64 *) malloc(activeChnListSize * sizeof(fl64));
		ERRORCHECK(losVectorCurr[i] == NULL)
	}
	losVectorLast = (fl64 **) malloc(3 * sizeof(fl64 *));
	ERRORCHECK(losVectorLast == NULL)
	for (i = 0; i < 3; i++) {
		losVectorLast[i] = (fl64 *) malloc(activeChnListSize * sizeof(fl64));
		ERRORCHECK(losVectorLast[i] == NULL)
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
	for (i = 0; i < HColLength; i++) {
		HtxH_InvxHt[i] = (fl64 *) malloc(activeChnListSize * sizeof(fl64));
		ERRORCHECK(HtxH_InvxHt[i] == NULL)
	}

	for (channelNr = 0; channelNr < activeChnListSize; channelNr++) {
		number = activeChnList[channelNr];
		satPosLast[0][channelNr] =
				navSolutions.channel.satPositionsOld[0][number];
		satPosLast[1][channelNr] =
				navSolutions.channel.satPositionsOld[1][number];
		satPosLast[2][channelNr] =
				navSolutions.channel.satPositionsOld[2][number];

		satPosCurr[0][channelNr] = navSolutions.channel.satPositions[0][number];
		satPosCurr[1][channelNr] = navSolutions.channel.satPositions[1][number];
		satPosCurr[2][channelNr] = navSolutions.channel.satPositions[2][number];

	}
	for (i = 0; i < 3; i++) {
		userPosLast[i] = navSolutions.xyzdtOld[i];
		userPosCurr[i] = navSolutions.xyzdt[i];
	}
	/*! The delta corrected receiver time between two adjacent epochs */
	deltaTime = navSolutions.rxTime[1] - navSolutions.rxTime[0];

	for (channelNr = 0; channelNr < activeChnListSize; channelNr++) {
		tempForNorm[0] = satPosCurr[0][channelNr] - userPosCurr[0];
		tempForNorm[1] = satPosCurr[1][channelNr] - userPosCurr[1];
		tempForNorm[2] = satPosCurr[2][channelNr] - userPosCurr[2];
		posCurrNorm = norm(tempForNorm, 3);
		tempForNorm[0] = satPosLast[0][channelNr] - userPosLast[0];
		tempForNorm[1] = satPosLast[1][channelNr] - userPosLast[1];
		tempForNorm[2] = satPosLast[2][channelNr] - userPosLast[2];
		posLastNorm = norm(tempForNorm, 3);

		for (i = 0; i < 3; i++) {
			losVectorCurr[i][channelNr] = (satPosCurr[i][channelNr]
					- userPosCurr[i]) / posCurrNorm;
			losVectorLast[i][channelNr] = (satPosLast[i][channelNr]
					- userPosLast[i]) / posLastNorm;
			losCurrTempDot[i] = losVectorCurr[i][channelNr];
			losLastTempDot[i] = losVectorLast[i][channelNr];
			satCurrTempDot[i] = satPosCurr[i][channelNr];
			satLastTempDot[i] = satPosLast[i][channelNr];

		}

		svDopp[channelNr] = dot(losCurrTempDot, satCurrTempDot, 3)
				- dot(losLastTempDot, satLastTempDot, 3);
		svDopp[channelNr] = svDopp[channelNr] / deltaTime;
		deltaGeo[channelNr] = dot(losCurrTempDot, userPosLast, 3)
				- dot(losLastTempDot, userPosLast, 3);
		deltaGeo[channelNr] = deltaGeo[channelNr] / deltaTime;
		y[channelNr][0] = carrPhaseDiff[channelNr] - svDopp[channelNr]
				+ deltaGeo[channelNr];
		/*Fill in the H and Ht arrays */
		H[channelNr][0] = -losVectorCurr[0][channelNr];
		H[channelNr][1] = -losVectorCurr[1][channelNr];
		H[channelNr][2] = -losVectorCurr[2][channelNr];
		H[channelNr][3] = 1;
		Ht[0][channelNr] = -losVectorCurr[0][channelNr];
		Ht[1][channelNr] = -losVectorCurr[1][channelNr];
		Ht[2][channelNr] = -losVectorCurr[2][channelNr];
		Ht[3][channelNr] = 1;

	}

	/* The fun bit */
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
		free(losVectorCurr[i]);
		free(losVectorLast[i]);
		free(satPosLast[i]);
		free(satPosCurr[i]);

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
	free(losVectorCurr);
	free(losVectorLast);
	free(y);
	free(satPosLast);
	free(satPosCurr);
	free(userPosCurr);
	free(userPosLast);
	free(deltaGeo);
	free(svDopp);

	return EXIT_SUCCESS;
}
