/*
 * findDopCarrPhase.c
 *
 *  Created on: May 15, 2015
 *      Author: sara
 */

#include "findDopCarrPhase.h"

#include <conf_nav.h>
#include "customDataTypes.h"
#include "utils.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

si32 findDopCarrPhase(
/* Output */
fl64 * doppler, fl64 * carrPhase,
/* Input */
navChannels_s * navChannels[], fl64 sampleNum, si32 * readyChnList, si32 readyChnListSize) {

	/*Assign variables */
	fl64 indexRange[BIT_REMAINDER];
	fl64 absoluteSample[BIT_REMAINDER];
	fl64 * index_a;
	fl64 * index_b;
	fl64 x1[2];
	fl64 y1[2];
	fl64 x2[2];
	fl64 y2[2];
	fl64 index_c = 0;

	si32 inda = 0;
	si32 channelNr = 0;
	si32 prn = 0;
	si32 index_aSize = 0;
	si32 index_bSize = 0;
	si32 indexEst = 0;

	fl64 samInterval = 0;

	/*Calculate the range of the index to accelerate index search*/
	for (inda = 0; inda < BIT_REMAINDER; inda++) {
		indexRange[inda] = indexEst - BUFF_SEARCH_RANGE + inda;
	}/*end of for loop to generate indexRange vector*/

	/*Calculate the doppler and carried phase each satellite using interpolation*/
	for (channelNr = 0; channelNr < readyChnListSize; channelNr++) {

		prn = readyChnList[channelNr];/*SARA now provides PRN number instead.*/

		samInterval = round((navChannels[prn]->buffSamNum[BUF_LENGTH - 1] - sampleNum) / SAMPLINGRATE * 1000);
		indexEst = BUF_LENGTH - samInterval;


		for (inda = 0; inda < BIT_REMAINDER; inda++) {

			/*absoluteSample[inda] =  navChannels[number]->dataBuff[indexEst+inda-BUFF_SEARCH_RANGE][0];*/
			absoluteSample[inda] = navChannels[prn]->buffSamNum[indexEst + inda - BUFF_SEARCH_RANGE];
		}

		/*Find the index of the sampleNum in the tracking results*/

		findfl64(&index_a, &index_aSize, absoluteSample, BIT_REMAINDER, "<=",
				sampleNum, "last");
		findfl64(&index_b, &index_bSize, absoluteSample, BIT_REMAINDER, ">=",
				sampleNum, "first");

		if (index_a != index_b) {

			x1[0] = absoluteSample[(si32) index_a[0]];
			x1[1] = absoluteSample[(si32) index_b[0]];

			y1[0] = (fl64) index_a[0];
			y1[1] = (fl64) index_b[0];

			/* Interpolate the index */
			interp1(&index_c, x1, y1, 2, sampleNum);

			x2[0] = (fl64) index_a[0];
			x2[1] = (fl64) index_b[0];

			/* Doppler measurements */
			y2[0] = navChannels[prn]->buffDopp[(si32) x2[0]];
			y2[1] = navChannels[prn]->buffDopp[(si32) x2[1]];

			/* Find the Doppler based on the index calculated*/
			interp1(&doppler[prn], x2, y2, 2, index_c);

			/* Carrier phase measurements */
			y2[0] = navChannels[prn]->buffCarr[(si32) x2[0]];
			y2[1] = navChannels[prn]->buffCarr[(si32) x2[1]];

			/*Find the carrier phase based on the index calculated*/
			interp1(&carrPhase[prn], x2, y2, 2, index_c);

		}/*end of if statement*/
		else {
			doppler[prn] =
					navChannels[prn]->buffDopp[(si32) indexRange[(si32) index_a[0]]]; /* rawDoppler*/
			carrPhase[prn] =
					navChannels[prn]->buffCarr[(si32) indexRange[(si32) index_a[0]]];/* rawCarrPhase*/
		}

		free(index_a);
		free(index_b);

	}/*end of main for loop*/
	return EXIT_SUCCESS;
}/*end of function*/
