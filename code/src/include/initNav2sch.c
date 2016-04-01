/*
 * initNav2Sch.c
 *
 *  Created on: Nov 30, 2015
 *      Author: daehee
 */

#include <stdlib.h>
#include "customDataTypes.h"		/// Standard types
#include <conf_nav.h>
#include "initNavChannels.h"
#include <conf_nav.h>
#include <initNav2sch.h>
#include <math.h>

si32 nav2sch_init(nav2sch_s ** nav2sch) {

	si32 i = 0;
	for (i = 0; i < NUM_PRNS; i++) {

		/* Initialize the size */
		nav2sch[i] = (nav2sch_s *) malloc(sizeof(nav2sch_s));
	}

	for (i = 0; i < NUM_PRNS; i++) {
		nav2sch[i]->prn = 0;
		nav2sch[i]->status = 0;
		nav2sch[i]->validWeek = 0;
		nav2sch[i]->validSec = 0;
		nav2sch[i]->startWeek = 0;
		nav2sch[i]->startSec = 0;
		nav2sch[i]->startAbsSample = 0;
		nav2sch[i]->lastAbsSample = 0;
		nav2sch[i]->carr = 0;
		nav2sch[i]->dopp = 0;
		nav2sch[i]->codePhase = 0;
	}
	return EXIT_SUCCESS;
}


si32 nav2sch_free(nav2sch_s ** nav2sch){
	si32 i = 0;
	for (i = 0; i < NUM_PRNS; i++) {
		free(nav2sch[i]);
	}

	return EXIT_SUCCESS;
}


si32 findSchInfo(navChannels_s *navChannels[], nav2sch_s ** nav2sch,
		si32 * readyChnList, si32 readyChnListSize, fl64 rxTime, fl64 currSampleNum){

	si32 channelNr = 0;
	si32 prn = 0;
	si32 indexEst = 0;
	fl64 samInterval = 0;
	fl64 sampleNum = 0;
	si32 iterCount = 0;


	for (channelNr = 0; channelNr < readyChnListSize; channelNr++) {

			prn = readyChnList[channelNr];/*SARA now provides PRN number instead.*/

			// Check the status of channel to run the code or not.
			// Do not need to run this code every time, so need to check the status.
			// The status is reset in the decoding when there is new subframe.
			if (nav2sch[prn]->status == 0){

				// Last absolute sample number in the latest subframe
				sampleNum = nav2sch[prn]->lastAbsSample;
				// Find the location of subframe start
				samInterval = round((navChannels[prn]->buffSamNum[BUF_LENGTH - 1] - sampleNum) / SAMPLINGRATE * 1000);
				indexEst = BUF_LENGTH - samInterval;

				// Try to refine the exact index (index of the last sample number in subframe)
				iterCount = 0;	// reset the iteration counter
				while (1){
					iterCount++;
					if (sampleNum == navChannels[prn]->buffSamNum[indexEst]){
						break;
					}
					else if (sampleNum > navChannels[prn]->buffSamNum[indexEst]){
						indexEst++;
					}
					else if (sampleNum < navChannels[prn]->buffSamNum[indexEst]){
						indexEst--;
					}

					// Check the range of indexEst
					if (indexEst > BUF_LENGTH || indexEst < 0 || iterCount > 10){
						// index is out of range.
						break;
					}
				}

				// Starting index of next subframe is just next one from the last sample number of latest subframe
				indexEst++;

				if (indexEst <= BUF_LENGTH && indexEst > 0 && iterCount <= 10) {
					nav2sch[prn]->status = 1;
					nav2sch[prn]->carr = navChannels[prn]->buffCarr[indexEst];
					nav2sch[prn]->dopp = navChannels[prn]->buffDopp[indexEst];
					nav2sch[prn]->codePhase = navChannels[prn]->buffCodePhase[indexEst];
					nav2sch[prn]->startAbsSample = navChannels[prn]->buffSamNum[indexEst];

					// calculate the time when the subframe started. This point is the latest subframe start,
					// but this subframe is not fully populated yet.
					// Simply, next subframe start is just adding 6 seconds from this point, but need to consider doppler.
					nav2sch[prn]->startSec = rxTime + (sampleNum - currSampleNum)/SAMPLINGRATE;
					nav2sch[prn]->validSec = nav2sch[prn]->startSec + 6.0;
				}
				// if the index exceeds the length of buffer, then set it is unavailable.
				else {
					nav2sch[prn]->status = 0;
				}
			}
	}

	return EXIT_SUCCESS;
}
