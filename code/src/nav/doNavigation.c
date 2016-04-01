/*
 * doNavigation.c
 *
 *  Created on: May 28, 2015
 *      Author: sean
 */

#include "customDataTypes.h"
#include "initNavSolutions.h"
#include "initState.h"
#include "initEph.h"
#include "utils.h"
#include "doNavigation.h"

#include <conf_nav.h>
#include <initNav2sch.h>
#include <initNavChannels.h>
#include "findEleAzi.h"
#include "findTransTime.h"
#include "checkEphStatus.h"
#include "findDopCarrPhase.h"
#include "calculatePseudoranges.h"
#include "satPosVel.h"
#include "leastSquarePos.h"
#include "leastSquareVel1.h"
#include "leastSquareVel2.h"
#include "corrCarrPhase.h"
#include "conf.h"
#include <math.h>
#include <nav_init.h>
#include <rnx.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



si32 doNavigation(navSolutions_s * navSolutions, state_s * state, eph_s * eph,
		navChannels_s *navChannels[], nav2sch_s ** nav2sch, FILE * rnxFid) {

	si32 channelSize = state->numberOfChannels;
	si32 i = 0, j = 0;
	fl64 sampleNum = navSolutions->sampleNum;
	si32 * activeChnList;
	si32 * readyChnList;
	si32 activeChnSize = 0;
	si32 readyChnSize = 0;
	si32 tempActiveChnSize = 0;
	si32 number1 = 0;
	fl64 rxTime = navSolutions->rxTime[1];
	fl64 ** carrPhaseMat;
	fl64 ** satClkCorrMat;
	fl64 ** ionoMat;
	fl64 ** tropoMat;
	fl64 ** satPositions;
	fl64 ** satVelocity;
	fl64 ** velXYZdtRate;
	fl64 * satClkCorr;
	fl64 * transmitTime;
	fl64 * tempobs;
	fl64 * carrPhaseDiff;
	fl64 * deltaErrors;
	fl64 * cpCompDiff;
	fl64 * deltaTropo;
	fl64 * deltaIono;
	si32 * number;
	fl64 userPos[3];
	fl64 transmitTimeMax = -HUGE_VALF;
	fl64 deltaTime = 0;
	fl64 deltaSample = 0;
	fl64 samplePerSec = 0;
	fl64 samplePerSecMean = 0;
	fl64 timeUpdate = 0;
	fl64 sampleUpdate = 0;
	fl64 af1 = 0;
	si32 numberSize;
	nav_s * navArray[NUM_PRNS];
	rnx_s rnx;

	// check the available PRNs.
	findsi32(&number, &numberSize, state->checkPRN, NUM_PRNS, ">", 0, "all");

	// Check the NAV LOSS COUNTER and reset the flag
	if (state->navLossCount > (fl64) NAVRSTSEC / (fl64) NAVRATE) {

		//printf("\nRESET the NAV: NAV was not running properly for more than %d seconds.\n\n", (si32) NAVRSTSEC);

		if (state->navFlag == 1){
			state->navFlag = 0;
			state->navLossCount = 0;

			//printf("\nRESET the NAV: NAV was not running properly for more than %d seconds.\n\n", (si32) NAVRSTSEC);
		}
	}

	// Basically increase the NAV LOSS COUNTER, and it will be reset at the end of this code.
	// If it failed to reach at the end of code (fail to get the NAV solutions), then it keeps increasing.
	state->navLossCount++;

	activeChnList = (si32 *) calloc(1, sizeof(si32) * channelSize);
	ERRORCHECK(activeChnList==NULL)

	// Check the number of satellites
	for (i = 0; i < channelSize; i++) {
		if (navChannels[(si32) number[i]]->decode == 1
				&& navChannels[(si32) number[i]]->Lock == 1) {
			activeChnList[activeChnSize] = number[i];
			activeChnSize++;
		}
		if (navChannels[(si32) number[i]]->status == 'A') {
			navSolutions->channel.el[(si32) number[i]] = INFINITY;
		}
	}
	// update the time step
	if (activeChnSize == 0) {
		if (state->navFlag == 1){
			state->navCount++;
			//state->navLossCount++;
			navSolutions->sampleNum += NAVSAMSTEP;

			fprintf(stderr, "No available SVs.\n");
		}

		free(activeChnList); // release the memory
		return EXIT_FAILURE;
	}


	/* Malloc Section */
	satClkCorr = (fl64 *) malloc(sizeof(fl64) * NUMSATS);
	ERRORCHECK(satClkCorr==NULL)
	tempobs = (fl64 *) malloc(sizeof(fl64) * NUMSATS);
	ERRORCHECK(tempobs==NULL)
	carrPhaseMat = (fl64 **) malloc(sizeof(fl64 *) * NUMSATS);
	ERRORCHECK(carrPhaseMat==NULL)
	carrPhaseDiff = (fl64 *) malloc(sizeof(fl64) * NUMSATS);
	ERRORCHECK(carrPhaseDiff==NULL)
	deltaErrors = (fl64 *) malloc(sizeof(fl64) * NUMSATS);
	ERRORCHECK(deltaErrors==NULL)
	cpCompDiff = (fl64 *) malloc(sizeof(fl64) * NUMSATS);
	ERRORCHECK(cpCompDiff==NULL)
	deltaTropo = (fl64 *) malloc(sizeof(fl64) * NUMSATS);
	ERRORCHECK(deltaTropo==NULL)
	deltaIono = (fl64 *) malloc(sizeof(fl64) * NUMSATS);
	ERRORCHECK(deltaIono==NULL)
	satClkCorrMat = (fl64 **) malloc(sizeof(fl64 *) * NUMSATS);
	ERRORCHECK(satClkCorrMat==NULL)
	ionoMat = (fl64 **) malloc(sizeof(fl64 *) * NUMSATS);
	ERRORCHECK(ionoMat==NULL)
	tropoMat = (fl64 **) malloc(sizeof(fl64 *) * NUMSATS);
	ERRORCHECK(tropoMat==NULL)

	for (i = 0; i < channelSize; i++) {
		carrPhaseMat[i] = (fl64 *) malloc(sizeof(fl64) * 2);
		ERRORCHECK(carrPhaseMat[i]==NULL)
		satClkCorrMat[i] = (fl64 *) malloc(sizeof(fl64) * 2);
		ERRORCHECK(satClkCorrMat[i]==NULL)
		ionoMat[i] = (fl64 *) malloc(sizeof(fl64) * 2);
		ERRORCHECK(ionoMat[i]==NULL)
		tropoMat[i] = (fl64 *) malloc(sizeof(fl64) * 2);
		ERRORCHECK(tropoMat[i]==NULL)

	}
	satPositions = (fl64 **) malloc(sizeof(fl64 *) * 3);
	ERRORCHECK(satPositions==NULL)
	satVelocity = (fl64 **) malloc(sizeof(fl64 *) * 3);
	ERRORCHECK(satVelocity==NULL)
	for (i = 0; i < 3; i++) {
		satPositions[i] = (fl64 *) malloc(sizeof(fl64) * NUMSATS);
		ERRORCHECK(satPositions[i]==NULL);
		satVelocity[i] = (fl64 *) malloc(sizeof(fl64) * NUMSATS);
		ERRORCHECK(satVelocity[i]==NULL);
	}

	velXYZdtRate = (fl64 **) malloc(sizeof(fl64 *) * 4);
	ERRORCHECK(velXYZdtRate==NULL)
	for (i = 0; i < 4; i++) {
		velXYZdtRate[i] = (fl64 *) malloc(sizeof(fl64) * 1);
	}


	activeChnList = (si32 *) realloc(activeChnList,	sizeof(si32) * activeChnSize);
	ERRORCHECK(activeChnList==NULL)
	readyChnSize = activeChnSize;
	readyChnList = (si32 *) malloc(sizeof(si32) * readyChnSize);
	ERRORCHECK(readyChnList==NULL)
	transmitTime = (fl64 *) malloc(sizeof(fl64) * NUMSATS);
	ERRORCHECK(transmitTime==NULL)
	memcpy(readyChnList, activeChnList, sizeof(si32) * activeChnSize);


	// Initialization
	if (state->navFlag == 0) {
		navSolutions->firstSample = state->accNumSamples;
		state->navFlag = 1;
		state->navCount = 0;

		//printf("\nNAV initialized.\n\n");
	}

	/* Main Loop */
	while (1) {
		/* Update the navigation counter */
		state->navCount++;

		/* Set the variables */
		rxTime = navSolutions->rxTime[1];

		for (i = 0; i < channelSize; i++) {
			carrPhaseMat[i][0] = navSolutions->channel.carrPhase[(si32) number[i]];
			satClkCorrMat[i][0] = navSolutions->channel.satClkCorr[(si32) number[i]];
			ionoMat[i][0] = navSolutions->channel.iono[(si32) number[i]];
			tropoMat[i][0] = navSolutions->channel.tropo[(si32) number[i]];
		}
		memcpy(navSolutions->xyzdtOld, navSolutions->xyzdt, 4 * sizeof(fl64));
		for (i = 0; i < 3; i++) {
			memcpy(navSolutions->channel.satPositionsOld[i],
					navSolutions->channel.satPositions[i],
					sizeof(fl64) * channelSize);
		}

		/*! Calculate the current sample number, the transmitting time of the
		 * satellite and raw receiver time with or without clock steering
		 */
		if (state->navCount <= 2) {
			sampleNum = navSolutions->firstSample
					+ (state->navCount - 1) * NAVSAMSTEP;
			/* check the sample index is within buffer */
			if (state->accNumSamples < sampleNum) {
				state->navCount--;
				break;
			}
			if (findTransTime(transmitTime, sampleNum, activeChnSize,
					navChannels, state) == EXIT_FAILURE) {
				fprintf(stderr, "Failed to find transmit time \n");
				break; //return EXIT_FAILURE;
			}

			for (i = 0; i < activeChnSize; i++) {
				if (transmitTimeMax < transmitTime[activeChnList[i]]) {
					transmitTimeMax = transmitTime[activeChnList[i]];
				}
			}
			switch (state->navCount) {
			case 1:
				/* Estimate the first receiver time */
				rxTime = transmitTimeMax + TRANMITOFFSET / 1000;
				navSolutions->weekNumber =	eph->nav[activeChnList[0]].weekNumber;

				break;
			case 2:
				/* Propagate the receiver time by the navSolution rate */
				rxTime = rxTime + 1.0 / NAVRATE;
				break;
			}
			/* Set the samples per second buffer */
			/* Poorman's circ shift  */
			memmove(navSolutions->samPerSecBuff,
					&navSolutions->samPerSecBuff[1],
					sizeof(fl64) * (CLKSTRNUM - 1));
			navSolutions->samPerSecBuff[CLKSTRNUM - 1] =
					SAMPLINGRATE;

		} else {
			/* The delta corrected receiver time between two adjacent epochs */
			deltaTime = navSolutions->rxTime[1] - navSolutions->rxTime[0];

			if (deltaTime < 0){
				// To deal with the week rollover
				deltaTime = deltaTime + (fl64) SECOFWEEK;
			}

			/* The delta absolute sample between two adjacent epochs */
			deltaSample = navSolutions->absoluteSample[1]
					- navSolutions->absoluteSample[0];

			/* Set the samples per second buffer */
			/* Poorman's circ shift  */
			memmove(navSolutions->samPerSecBuff,
					&navSolutions->samPerSecBuff[1],
					sizeof(fl64) * (CLKSTRNUM - 1));
			navSolutions->samPerSecBuff[CLKSTRNUM - 1] = deltaSample
					/ deltaTime;
			/* Clock steering starts from the third epoch */
			if (state->navCount == 3) {
				if (CLKSTRON == 1) {
					/* Find the receiver time which is in the multiple of the updated epoch */
					rxTime = navSolutions->rxTime[1]
							- fmod(navSolutions->rxTime[1],	1.0 / NAVRATE) + 1.0 / NAVRATE;
				} else {
					rxTime += 1.0 / NAVRATE;
				}
			} else {
				rxTime += 1.0 / NAVRATE;
			}

			/*  Find the averaged samples per second */
			if (state->navCount <= CLKSTRNUM + 2) {
				samplePerSec = navSolutions->samPerSecBuff[CLKSTRNUM - 1];
			} else {
				samplePerSecMean = 0;
				for (i = 0; i < CLKSTRNUM; i++) {
					samplePerSecMean += navSolutions->samPerSecBuff[i];
				}
				samplePerSecMean = samplePerSecMean / CLKSTRNUM;
				samplePerSec = samplePerSecMean;
			}
			/* Propagate the sample number */
			if (CLKSTRON == 1) {
				/* Calculate the propagation value for the sample number */
				timeUpdate = rxTime - navSolutions->rxTime[1];
				sampleUpdate = samplePerSec * timeUpdate;
			} else {
				sampleUpdate = NAVSAMSTEP;
			}

			sampleNum += sampleUpdate;

			/* Check the sample index is within buffer */
			if (state->accNumSamples < sampleNum) {
				sampleNum -= sampleUpdate;
				state->navCount--;
				break;
			}

			if (findTransTime(transmitTime, sampleNum, readyChnSize,
					navChannels, state) == EXIT_FAILURE) {
				fprintf(stderr, "Failed to find transmit time \n");
				break; //return EXIT_FAILURE;
			}
		}

		// Check the week rollover in Rx time //
		if (rxTime > (fl64) SECOFWEEK){
			rxTime = rxTime - (fl64) SECOFWEEK;
			navSolutions->weekNumber ++;
		}

		// Update the rxTime buffer
		navSolutions->rxTime[0] = navSolutions->rxTime[1];
		navSolutions->rxTime[1] = rxTime;
		navSolutions->rawRxTime = rxTime;// Record the raw receiver time

		// Update the absolute sample number at a circular buffer //
		navSolutions->sampleNum = sampleNum;
		navSolutions->absoluteSample[0] = navSolutions->absoluteSample[1];
		navSolutions->absoluteSample[1] = sampleNum;

		// Find pseudoranges
		if (calculatePseudoranges(navSolutions->channel.rawP, transmitTime,
				rxTime, activeChnList, activeChnSize) == EXIT_FAILURE) {
			printf("Failed to calculate pseudorange \n");
			break; //return EXIT_FAILURE;
		}

		// Find Carrier phase & Doppler
		findDopCarrPhase(navSolutions->channel.doppler, navSolutions->channel.carrPhase,
				navChannels, sampleNum, activeChnList, activeChnSize);

		// Write the code for scheduler info
		findSchInfo(navChannels, nav2sch, activeChnList, activeChnSize, rxTime, sampleNum);

		// Find satellite positions and clock corrections
		for (i = 0; i < NUM_PRNS; i++) {
			navArray[i] = &eph->nav[i];
		}
		satPosVel(satPositions, satVelocity, satClkCorr, transmitTime, navArray,
				activeChnList, activeChnSize);

		// Log the rnx structure: only channel data (except position solution)
		/* If #satellites < 4, then the code escapes the NAV without ANY LOGGING.
		 * Therefore the channel measurements should be logged here,
		 * otherwise we do not have any rnx data under insufficient satellite condition.
		 */
		for (i = 0; i < CONF_NUMCHN ; i++){
			if (i < activeChnSize){
				number1 = (si32) activeChnList[i];
				rnx.validPRN[i] = true;
				rnx.prn[i] 		= (ui32) number1 + 1;
				rnx.pr1[i] 		= navSolutions->channel.rawP[number1];
				rnx.cp1[i] 		= navSolutions->channel.carrPhase[number1];
				rnx.dp1[i] 		= navSolutions->channel.doppler[number1];
				rnx.cno1[i] 	= navChannels[number1]->CNo;
				rnx.pr2[i] 		= 0;
				rnx.cp2[i] 		= 0;
				rnx.dp2[i] 		= 0;
				rnx.cno2[i] 	= 0;
				rnx.Xsv[i]		= satPositions[0][number1];
				rnx.Ysv[i]		= satPositions[1][number1];
				rnx.Zsv[i]		= satPositions[2][number1];
				rnx.svClk[i]	= satClkCorr[number1];
			}
			else
			{
				rnx.validPRN[i] = false;
				rnx.prn[i] 		= 0;
			}
		}


		/*! Check the health and recode number of ephemeris
		 * if the satellite is unhealthy then it is removed from the satellite
		 * list. Besides based on the transmit time of the satellites. the
		 * following satellite orbits calculation should use the most recent ephemeris
		 */

		if (checkEphStatus(activeChnList, &activeChnSize, navArray,
				rxTime, navChannels) == EXIT_FAILURE) {
			fprintf(stderr, "Failed to check ephemeris \n");
			break; //return EXIT_FAILURE;
		}

		// Calculate the elevation angle based on previous position to check the mask angle.
		// In here, nav count 10 and dt 1000 are arbitrary number.
		// Only check the elevation angle when the previous position solution was valid (dt <1000).
		if (state->navCount > 10 && fabs(navSolutions->xyzdt[3]) < 1000){
			if (findEleAzi(navSolutions->xyzdt, navSolutions->channel.el,
					navSolutions->channel.az, satPositions, activeChnSize,
					activeChnList) == EXIT_FAILURE) {
				fprintf(stderr,
						"Failed to get the elevation angle \n");
				// Reset the dt, then next time skip this IF STATEMENT.
				navSolutions->xyzdt[3] = 10000;

				break; //return EXIT_FAILURE;
			}
		}

		// Record the satellites position velocity and clocks corrections,
		// And check the elevation angle
		tempActiveChnSize = 0;
		for (i = 0; i < activeChnSize; i++) {
			number1 = activeChnList[i];

			// Save the satellite clock correction
			navSolutions->channel.satClkCorr[number1] = satClkCorr[number1];
			satClkCorrMat[i][1] = satClkCorr[number1];

			// Save the satellite position & velocity
			for (j = 0; j < 3; j++) {
				navSolutions->channel.satPositions[j][number1] = satPositions[j][number1];
				navSolutions->channel.satVelocity[j][number1]  = satVelocity[j][number1];
			}

			// Determine the actual satellite transmitted frequency and measured estimate of the received signal frequency
			af1 = eph->nav[number1].a_f1;
			navSolutions->channel.transmitFreq[number1] = L1FREQ * (1 + af1);
			navSolutions->channel.receivedFreq[number1] = L1FREQ + navSolutions->channel.doppler[number1];

			// Check the elevation angle
			if (fabs(navSolutions->xyzdt[3]) < 1000){
				if (navSolutions->channel.el[number1] > (fl64) MASKANGLE) {
					// The size of new active channel list is always equal or smaller than current one.
					// And there is no inverse of order. So we can use below allocation process without define another variable.
					activeChnList[tempActiveChnSize] = activeChnList[i];
					tempActiveChnSize++;
				}
			}
		}
		// Only update the ACTIVE CHANNEL LIST when the position solution was valid (dt <1000).
		if (fabs(navSolutions->xyzdt[3]) < 1000){
			activeChnSize = tempActiveChnSize;
			activeChnList = (si32 *) realloc(activeChnList, activeChnSize * sizeof(si32));
			ERRORCHECK(activeChnList == NULL)
		}

		// Find receiver position and velocity
		// 3D receiver position/velocity can be found only if signals from more than 3 satellites are available
		if (activeChnSize > 3) {
			for (i = 0; i < activeChnSize; i++) {
				number1 = (si32) activeChnList[i];
				tempobs[number1] = navSolutions->channel.rawP[number1] + satClkCorr[number1] * SPEEDOFLIGHT;
			}

			/* ---< calculate receiver position >---- */
			/* This is a huge function all. Abandon all hope ye who hope to debug it */
			if (leastSquarePos(navSolutions->xyzdt, navSolutions->channel.el,
					navSolutions->channel.az, navSolutions->channel.iono,
					navSolutions->channel.tropo, navSolutions->DOP,
					satPositions, activeChnSize, tempobs, eph->almanac, rxTime,
					activeChnList) == EXIT_FAILURE) {
				navSolutions->rxTime[1] = rxTime;
				fprintf(stderr, "Failed to get position solution \n");
				break; //return EXIT_FAILURE;
			}
			/* save the iono/tropo data for velocity calculation */
			for (i = 0; i < channelSize; i++) {
				number1 = (si32) number[i];
				ionoMat[i][1] = navSolutions->channel.iono[number1];
				tropoMat[i][1] = navSolutions->channel.tropo[number1];
			}

			/* Compute the corrected receiver time */
			navSolutions->rxTime[1] = rxTime - navSolutions->xyzdt[3] / SPEEDOFLIGHT;


			/* ---< Calculate receiver velocity >---- */
			switch (VELSOL) {
			case 1:
				if (state->navCount == 1) {
					for (i = 0; i < 3; i++) {
						userPos[i] = navSolutions->xyzdt[i];
					}
					if (leastSquareVel2(velXYZdtRate,
							navSolutions->channel.transmitFreq,
							navSolutions->channel.receivedFreq, satPositions,
							satVelocity, userPos, activeChnList, activeChnSize) == EXIT_FAILURE) {

						fprintf(stderr,
								"Failed to get velocity solution at nav count = 1 \n");
						break; //return EXIT_FAILURE;
					}
				} else {
					/* The delta corrected receiver time between two adjacent epochs */
					deltaTime = navSolutions->rxTime[1]	- navSolutions->rxTime[0];
					if (deltaTime < 0){
						// To deal with the week rollover
						deltaTime = deltaTime + (fl64) SECOFWEEK;
					}

					for (i = 0; i < channelSize; i++) {
						/* Find the differential carrier phase between two adjacent epochs */
						carrPhaseDiff[i] = carrPhaseMat[i][1] - carrPhaseMat[i][0];
						/* Find the change rate of the carrier phase measurement */
						carrPhaseDiff[i] = carrPhaseDiff[i] / deltaTime;
						/*Record the delta errors (satellite clock drift) */
						deltaErrors[i] = satClkCorrMat[i][1] - satClkCorrMat[i][0];
						/* Find the error rate */
						deltaErrors[i] = deltaErrors[i] / deltaTime;
						/* Calculate the compensated SD carrier phase */
						cpCompDiff[i] = carrPhaseDiff[i] + deltaErrors[i];
						/* Calculate the differential tropo error */
						deltaTropo[i] = tropoMat[i][1] - tropoMat[i][0];
						/*  Find the tropo change rate */
						deltaTropo[i] = deltaTropo[i] / deltaTime;
						/* Calculate the differential iono error */
						deltaIono[i] = ionoMat[i][1] - ionoMat[i][0];
						/*Find the iono change rate */
						deltaIono[i] = deltaIono[i] / deltaTime;
						/* Correct the iono and tropo change rate from the
						 * differential carrier phase
						 */
						cpCompDiff[i] = cpCompDiff[i] - deltaTropo[i] + deltaIono[i];
					}
					if (leastSquareVel1(velXYZdtRate, *navSolutions, cpCompDiff,
							activeChnList, activeChnSize) == EXIT_FAILURE) {

						fprintf(stderr,
								"Failed to get velocity solution (case 1) \n");
						break; //return EXIT_FAILURE;
					}
				}
				break;
			case 2:
				for (i = 0; i < 3; i++) {
					userPos[i] = navSolutions->xyzdt[i];
				}
				if (leastSquareVel2(velXYZdtRate,
						navSolutions->channel.transmitFreq,
						navSolutions->channel.receivedFreq, satPositions,
						satVelocity, userPos, activeChnList, activeChnSize) == EXIT_FAILURE) {

					fprintf(stderr,
							"Failed to get velocity solution (case 2) \n");
					break; //return EXIT_FAILURE;
				}
				break;
			default:
				fprintf(stderr,
						"Error: Settings.velSOl should be either 1 or 2");
				break; //return EXIT_FAILURE;
			}

			navSolutions->VX = velXYZdtRate[0][0];
			navSolutions->VY = velXYZdtRate[1][0];
			navSolutions->VZ = velXYZdtRate[2][0];
			navSolutions->dtRate = velXYZdtRate[3][0];
			/* ---<velocity calculation end > --- */


			/* correct the doppler from receiver clock drift */
			for (i = 0; i < channelSize; i++) {
				number1 = (si32) number[i];
				navSolutions->channel.correctedDop[number1] =
						navSolutions->channel.receivedFreq[number1]
								* (1 + navSolutions->dtRate / SPEEDOFLIGHT)
								- navSolutions->channel.transmitFreq[number1];
			}
			// Correct pseudorange measurements for receiver clocks errors
			for (i = 0; i < activeChnSize; i++) {
				number1 = activeChnList[i];
				navSolutions->channel.correctedP[number1] =
						navSolutions->channel.rawP[number1]
								- navSolutions->xyzdt[3];
			}
		}

		else {	// Insufficient satellites

			// Before escape NAV, need to log the rnx data
			rnx.newKal 		= false;
			rnx.newMea 		= false;
			rnx.validPos 	= false;
			rnx.rtow		= navSolutions->rawRxTime;
			rnx.week		= (ui32) navSolutions->weekNumber;

#if LOGRNX
				fwrite(&rnx, sizeof(rnx), 1, rnxFid);
#endif



			if (state->navCount == 1){
				state->navFlag = 0;
			}
			fprintf(stderr, "Insufficient satellites for position solution. \n");

			break; //return EXIT_FAILURE;

		}

		corrCarrPhase(navSolutions, carrPhaseMat, channelSize);

		// Leap Second
		navSolutions->leapSec = LEAPSECONDS;

		// Reset the NAV LOSS COUNT
		state->navLossCount = 0;

		/* display current status */
		for (i = 0; i < NUM_PRNS; i++) {
			eph->newEphFlag[i] = 0;
		}
		printf("\nGPS Week = %0.0f, Sec = %0.3f, #NAV = %d, #SVs = %d \n",
				navSolutions->weekNumber, rxTime, state->navCount, activeChnSize);
		printf("Position (x,y,z,dt): %0.3f, %0.3f, %0.3f, %0.3f \n",
				navSolutions->xyzdt[0], navSolutions->xyzdt[1],
				navSolutions->xyzdt[2], navSolutions->xyzdt[3]);

		// Save the data into rnx
		rnx.newKal 		= false;
		rnx.newMea 		= true;
		rnx.validPos 	= true;
		rnx.rtow		= navSolutions->rawRxTime;
		rnx.week		= (ui32) navSolutions->weekNumber;
		rnx.numSat 		= (ui32) activeChnSize;
		rnx.X			= navSolutions->xyzdt[0];
		rnx.Y			= navSolutions->xyzdt[1];
		rnx.Z			= navSolutions->xyzdt[2];
		rnx.dt			= navSolutions->xyzdt[3];
		rnx.Vx			= navSolutions->VX;
		rnx.Vy			= navSolutions->VY;
		rnx.Vz			= navSolutions->VZ;
		rnx.dtRate		= navSolutions->dtRate;
		rnx.HDOP 		= navSolutions->DOP[2];
		rnx.VDOP		= navSolutions->DOP[3];
		rnx.TDOP		= navSolutions->DOP[4];
		rnx.smp			= sampleNum;
		rnx.frc			= sampleNum;
		rnx.valid1		= 1;
		rnx.valid2		= 0;


#if LOGRNX
				fwrite(&rnx, sizeof(rnx), 1, rnxFid);
#endif

	}

	//navSolutions->sampleNum = sampleNum;

	/* Free section */
	for (i = 0; i < channelSize; i++) {
		free(carrPhaseMat[i]);
		free(satClkCorrMat[i]);
		free(ionoMat[i]);
		free(tropoMat[i]);

	}

	for (i = 0; i < 3; i++) {
		free(satPositions[i]);
		free(satVelocity[i]);
	}

	for (i = 0; i < 4; i++) {
		free(velXYZdtRate[i]);
	}

	free(velXYZdtRate);
	free(carrPhaseMat);
	free(satClkCorrMat);
	free(ionoMat);
	free(tropoMat);
	free(activeChnList);
	free(readyChnList);
	free(transmitTime);
	free(tempobs);
	free(satPositions);
	free(satVelocity);
	free(satClkCorr);
	free(carrPhaseDiff);
	free(deltaErrors);
	free(cpCompDiff);
	free(deltaTropo);
	free(deltaIono);
	free(number);
	return EXIT_SUCCESS;
}
