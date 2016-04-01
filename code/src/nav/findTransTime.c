/*
 * findTransTime.c
 *
 *  Created on: May 15, 2015
 *      Author: sara
 */
#include "customDataTypes.h"

#include "initState.h"
#include "initNavChannels.h"
#include "findTransTime.h"

#include <conf.h>
#include <conf_nav.h>
#include "initEph.h"

#include "check_t.h"
#include "initState.h"
#include "utils.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

si32 findTransTime(fl64 * transmitTime, fl64 sampleNum, si32 readyChnListSize,
		navChannels_s * navChannels[], state_s *state) {

	/*Assign variables */
	fl64 absoluteSample[BIT_REMAINDER];
	fl64 TOW = 0;
	fl64 * index_a;
	fl64 * index_b;
	si32 * number;
	si32 prn = 0;
	fl64 msRange[2];

	fl64 sampleRange[2];
	fl64 sampleSec = 0;
	fl64 gpsSecRange[2];

	si32 inda = 0;
	si32 channelNr = 0;
	si32 index_aSize = 0;
	si32 index_bSize = 0;
	si32 indexEst = 0;
	si32 numberSize = 0;
	fl64 samInterval = 0;

	// find out the available PRNs
	findsi32(&number, &numberSize, state->checkPRN, 32, ">", 0, "all");

	for (channelNr = 0; channelNr < readyChnListSize; channelNr++) {

		// assign the PRN
		prn = (si32) number[channelNr];

		/*Calculate the range of the index to accelerate index search*/
		samInterval = round( ((navChannels[prn]->buffSamNum[BUF_LENGTH - 1] - sampleNum) / SAMPLINGRATE )* 1000 );
		indexEst = BUF_LENGTH - samInterval;

		// actually this WHILE loop is not necessary under error-free absolute sample number condition,
		// but in this version there exists in-continuous sample number sometimes.
		// Need to check it.
		while (1){

			if (fabs(navChannels[prn]->buffSamNum[indexEst] - sampleNum) < (fl64) 2 * SAMPLESPERCODE){
				break;
			}
			samInterval = round( (navChannels[prn]->buffSamNum[indexEst] - sampleNum)
							/ SAMPLINGRATE * 1000 );
			indexEst = indexEst - samInterval;
		}

		for (inda = 0; inda < BIT_REMAINDER; inda++) {
			absoluteSample[inda] = navChannels[prn]->buffSamNum[indexEst + inda - BUFF_SEARCH_RANGE];
		}
		TOW = navChannels[prn]->TOW;

		/*Find the index of the sampleNum in the tracking results*/
		findfl64(&index_a, &index_aSize, absoluteSample, BIT_REMAINDER, "<=",
				sampleNum, "last");
		findfl64(&index_b, &index_bSize, absoluteSample, BIT_REMAINDER, ">=",
				sampleNum, "first");


		if (index_a == NULL) {
			fprintf(stderr, "Failed to find index A in findTransTime\n");
			return EXIT_FAILURE;
		}
		if (index_b == NULL) {
			fprintf(stderr, "Failed to find index B in findTransTime\n");

			fprintf(stderr, "\nChannel = %d \n", channelNr);
			fprintf(stderr, "Start & End index: %d ... %d \n",
					indexEst - BUFF_SEARCH_RANGE,
					indexEst + inda - BUFF_SEARCH_RANGE);
			fprintf(stderr, "Estimated buffer start index = %0.0f \n",
					index_a[0]);
			fprintf(stderr,
					"Estimated buffer index for current index buff: %d \n",
					indexEst);

			return EXIT_FAILURE;
		}

		msRange[0] = (absoluteSample[(si32) index_a[0]]) / SAMPLINGRATE * 1000;
		msRange[1] = (absoluteSample[(si32) index_b[0]]) / SAMPLINGRATE * 1000;

		if (index_a[0] != index_b[0]) {

			sampleRange[0] = absoluteSample[(si32) index_a[0]];
			sampleRange[1] = absoluteSample[(si32) index_b[0]];

			interp1(&sampleSec, sampleRange, msRange, 2, sampleNum);

			gpsSecRange[0] = TOW + round( (absoluteSample[(si32) index_a[0]] - navChannels[prn]->firstSubFrameBits)
											/ SAMPLINGRATE * 1000)	* 0.001;

			gpsSecRange[1] = TOW + round( (absoluteSample[(si32) index_b[0]] - navChannels[prn]->firstSubFrameBits)
											/ SAMPLINGRATE * 1000) * 0.001;

			/*Find the transmitting time based on the index calculated*/
			interp1(&transmitTime[prn], msRange, gpsSecRange, 2, sampleSec);

		}/*end of if-statement*/

		else {
			transmitTime[prn] = TOW - round( (navChannels[prn]->firstSubFrameBits - absoluteSample[(si32) index_a[0]])
											/ SAMPLINGRATE * 1000) * 0.001;
		}
		free(index_a);
		free(index_b);
	}/*End of the main for loop*/
	free(number);
	return EXIT_SUCCESS;
}

