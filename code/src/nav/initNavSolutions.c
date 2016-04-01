/*
 * intNavSolution.c
 *
 *  Created on: May 19, 2015
 *      Author: sean
 */

#include <conf_nav.h>
#include "initNavSolutions.h"
#include "customDataTypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

si32 initNavSolution(navSolutions_s * navSolutions) {

	si32 i = 0;
	navSolutions->firstSample = 0; /* the sample number for the first navigation calculation */
	navSolutions->sampleNum = 0;

	/* channel dependent variables
	 Save list of satellites used for position calculation */
	navSolutions->channel.PRN = (fl64*) malloc(
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->channel.PRN == NULL)
	/* navSolutions->channel.satElev        = inf(settings.numberOfChannels, 1); */
	navSolutions->channel.CNo = (fl64*) malloc(
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->channel.CNo == NULL)

	/* These two lines help the skyPlot function. The satellites excluded do to
	 elevation mask will not "jump" to position (0,0) in the sky plot. */
	navSolutions->channel.el = (fl64*) malloc(
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->channel.el == NULL)

	navSolutions->channel.az = (fl64*) malloc(
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->channel.az == NULL)

	navSolutions->channel.doppler = (fl64*) malloc(
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->channel.doppler == NULL)

	navSolutions->channel.carrPhase = (fl64*) malloc(
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->channel.carrPhase == NULL)

	navSolutions->channel.rawCarrPhase = (fl64*) malloc(
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->channel.rawCarrPhase == NULL)

	navSolutions->channel.rawP = (fl64*) malloc(
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->channel.rawP == NULL)

	navSolutions->channel.satPositions = (fl64 **) malloc(3 * sizeof(fl64*));
	ERRORCHECK(navSolutions->channel.satPositions == NULL)

	navSolutions->channel.satPositionsOld = (fl64 **) malloc(3 * sizeof(fl64*));
	ERRORCHECK(navSolutions->channel.satPositionsOld == NULL)

	navSolutions->channel.satVelocity = (fl64 **) malloc(3 * sizeof(fl64*));
	ERRORCHECK(navSolutions->channel.satVelocity == NULL)

	for (i = 0; i < 3; i++) {
		navSolutions->channel.satPositions[i] = (fl64 *) malloc(
		NUMSATS * sizeof(fl64));
		ERRORCHECK(navSolutions->channel.satPositions[i] == NULL)

		navSolutions->channel.satPositionsOld[i] = (fl64 *) malloc(
		NUMSATS * sizeof(fl64));
		ERRORCHECK(navSolutions->channel.satPositionsOld[i] == NULL)

		navSolutions->channel.satVelocity[i] = (fl64 *) malloc(
		NUMSATS * sizeof(fl64));
		ERRORCHECK(navSolutions->channel.satVelocity[i] == NULL)

	}
	navSolutions->channel.satClkCorr = (fl64*) malloc(
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->channel.satClkCorr == NULL)

	navSolutions->channel.iono = (fl64*) malloc(
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->channel.iono == NULL)

	navSolutions->channel.tropo = (fl64*) malloc(
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->channel.tropo == NULL)

	navSolutions->channel.transmitFreq = (fl64*) malloc(
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->channel.transmitFreq == NULL)

	navSolutions->channel.receivedFreq = (fl64*) malloc(
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->channel.receivedFreq == NULL)

	navSolutions->channel.correctedDop = (fl64*) malloc(
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->channel.correctedDop == NULL)

	navSolutions->channel.correctedP = (fl64*) malloc(
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->channel.correctedP == NULL)

	navSolutions->channel.correctedCP = (fl64*) malloc(
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->channel.correctedCP == NULL)

	navSolutions->channel.geoRange = (fl64*) malloc(
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->channel.geoRange == NULL)

	navSolutions->channel.carrCorrCount = (fl64*) calloc(1,
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->channel.carrCorrCount == NULL)

	navSolutions->velSolPRNlist = (fl64*) malloc(
	NUMSATS * sizeof(fl64));
	ERRORCHECK(navSolutions->velSolPRNlist == NULL)

	navSolutions->posSolPRNlist = (si32*) malloc(
	NUMSATS * sizeof(si32));
	ERRORCHECK(navSolutions->posSolPRNlist == NULL)

	/* channel independant variables */
	navSolutions->xyzdt[0] = NAN;
	navSolutions->xyzdt[1] = NAN;
	navSolutions->xyzdt[2] = NAN;
	navSolutions->xyzdt[3] = NAN;

	navSolutions->xyzdtOld[0] = NAN;
	navSolutions->xyzdtOld[1] = NAN;
	navSolutions->xyzdtOld[2] = NAN;
	navSolutions->xyzdtOld[3] = NAN;

	navSolutions->VX = 0;
	navSolutions->VY = 0;
	navSolutions->VZ = 0;
	navSolutions->dtRate = 0;

	navSolutions->E = 0;
	navSolutions->N = 0;
	navSolutions->U = 0;

	navSolutions->latitude = 0;
	navSolutions->longitude = 0;
	navSolutions->height = 0;

	navSolutions->DOP[0] = 0;
	navSolutions->DOP[1] = 0;
	navSolutions->DOP[2] = 0;
	navSolutions->DOP[3] = 0;
	navSolutions->DOP[4] = 0;

	navSolutions->rxTime[0] = 0;
	navSolutions->rxTime[1] = 0;
	navSolutions->absoluteSample[0] = 0;
	navSolutions->absoluteSample[1] = 0;

	navSolutions->samPerSecBuff = (fl64*) malloc(
			CLKSTRNUM * sizeof(fl64));
	ERRORCHECK(navSolutions->samPerSecBuff == NULL)

	navSolutions->rawRxTime = 0;
	navSolutions->utmZone = 0;

	navSolutions->avgClock = 0;
	navSolutions->firstSampleTime = 0;

	navSolutions->weekNumber = 0;
	navSolutions->firstSampleTimeUTC = 0;
	navSolutions->leapSec = 0;

	for (i = 0; i < NUMSATS; i++) {
		navSolutions->channel.el[i] = INFINITY;
	}

	return EXIT_SUCCESS;
}
void deleteNavSolution(navSolutions_s * navSolutions) {
	si32 i = 0;

	free(navSolutions->channel.PRN);
	free(navSolutions->channel.CNo);
	free(navSolutions->channel.el);
	free(navSolutions->channel.az);
	free(navSolutions->channel.doppler);
	free(navSolutions->channel.carrPhase);
	free(navSolutions->channel.rawCarrPhase);

	free(navSolutions->channel.rawP);
	for (i = 0; i < 3; i++) {
		free(navSolutions->channel.satPositions[i]);
		free(navSolutions->channel.satPositionsOld[i]);
		free(navSolutions->channel.satVelocity[i]);
	}

	free(navSolutions->channel.satPositions);
	free(navSolutions->channel.satPositionsOld);
	free(navSolutions->channel.satVelocity);
	free(navSolutions->channel.satClkCorr);
	free(navSolutions->channel.iono);
	free(navSolutions->channel.tropo);
	free(navSolutions->channel.transmitFreq);
	free(navSolutions->channel.receivedFreq);
	free(navSolutions->channel.correctedDop);
	free(navSolutions->channel.correctedP);
	free(navSolutions->channel.correctedCP);
	free(navSolutions->channel.geoRange);
	free(navSolutions->channel.carrCorrCount);
	free(navSolutions->velSolPRNlist);
	free(navSolutions->posSolPRNlist);

	free(navSolutions->samPerSecBuff);
}
