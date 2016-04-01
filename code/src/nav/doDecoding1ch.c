/*
 * doDecoding.c
 *
 *  Created on: May 12, 2015
 *      Author: sara
 */
#include "conf_nav.h"
#include "customDataTypes.h"
#include "initState.h"
#include "utils.h"
#include "doDecoding.h"
#include "cirShiftBuff.h"
#include "initEph.h"
#include "initNav2sch.h"
#include "initNavChannels.h"
#include "navParityChk.h"
#include "ephemeris.h"
#include "calcDateTime.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>


/* Decodes the navigation bits. */
si32 doDecoding1ch(navChannels_s * navChannels[],
		cirBuffNavBit_s * cirBuffNavBit[], eph_s * eph, nav2sch_s ** nav2sch, si32 prn) {

	fl64 timeOfSub = 0;

	/* FOR loop indexes */
	si32 ii = 0, jj = 0, kk = 0;

	/* Preamble pattern */
	si8 preamble[8] = { 1, -1, -1, -1, 1, -1, 1, 1 };
	si32 navBits[302];
	si32 bitCheck1[NUM_PRNS];
	si32 bitCheck2[NUM_PRNS];

	si32 status2 = 0;
	si32 status1 = 0;
	pi8 navBitsBin[301];
	pi8 navBitsConv[2];
	pi8 D29Star = '0';
	pi8 D30Star = '0';

	si32 subframeID = 0;
	fl64 newEphFlag = 0;
	fl64 ephFlag = 0;
	calcTimeStruct t;
	si32 ephConsistency = 0;

	/* Variable which Daehee added for better decoding */
	si32 bitCompStartIndex = 0;
	si32 bitCompFlag = 0;
	si32 tempBit = 0;
	si32 corrResult = 0;
	si32 bufIdx = 0;
	si32 tempBitBuff[302] = { 0 };
	si32 corrIdx = 0;
	si32 buffOffest = 0;

	/* Check the channel status */
	if ((navChannels[prn]->status != 'T') || (navChannels[prn]->Lock == 0)) {
		navChannels[prn]->bitSynchFlag 		= 0;
		navChannels[prn]->frameSync 		= 0;
		cirBuffNavBit[prn]->bitCount 		= 0;

		// Escape and Move to next channel.
		return EXIT_SUCCESS;
	}

	/* Check the tracking status & Lock */
	if ((navChannels[prn]->status == 'T') && (navChannels[prn]->Lock == 1)) {

		// "decode" status is set as '-1' in "checkEphStatus.c" when the ephemeris is invalid.
		if (navChannels[prn]->decode == -1){
			eph->navStatus[prn][0] = 0;
			eph->navStatus[prn][1] = 0;
			eph->navStatus[prn][2] = 0;

			eph->navTempStatus[prn][0] = 0;
			eph->navTempStatus[prn][1] = 0;
			eph->navTempStatus[prn][2] = 0;

			eph->nav[prn].IODC8bits_sf1 = 0;
			eph->nav[prn].IODE_sf2 = 0;
			eph->nav[prn].IODE_sf3 = 0;

			eph->navTemp[prn].IODC8bits_sf1 = 0;
			eph->navTemp[prn].IODE_sf2 = 0;
			eph->navTemp[prn].IODE_sf3 = 0;

			navChannels[prn]->bitSynchFlag = 0;
			cirBuffNavBit[prn]->bitCount = 0;
		}

		/* Bit synch */
		navBitSych(navChannels, prn);

		/* frame synch */
		/* Find the index to start bit compress */
		if (navChannels[prn]->bitSynchFlag == 1) {
			bitCompFlag = 0;
			// for (ii=BUF_LENGTH; ii>0; ii--){
			for (ii = 0; ii < BUF_LENGTH; ii++) {
				if (navChannels[prn]->buffSamNum[ii] == navChannels[prn]->bitSynchSamNum) {

					// if the sample number is lager than the one at bit synch point, then reset.
					if (navChannels[prn]->buffSamNum[ii] > navChannels[prn]->bitSynchSamNum) {
						// Reset the flag: bit synch & compress
						bitCompFlag = 0;
						navChannels[prn]->bitSynchFlag = 0;
						break;
					}

					/* check the bit sign consistency within 20ms */
					for (kk = ii; kk < 20-1; kk++){
						if (navChannels[prn]->buffIP[kk] * navChannels[prn]->buffIP[kk + 1] < 0) {
							// Reset the flag: bit synch & compress
							bitCompFlag = 0;
							navChannels[prn]->bitSynchFlag = 0;
							break;
						}
					}
					/* Read next NAV bit (+20) */
					bitCompStartIndex = ii + 20;
					if (bitCompStartIndex + 19 < BUF_LENGTH) {
						/* If there is enough data to compress the bits */
						bitCompFlag = 1;
					}
					/* Do not need to iterate after finding index */
					break;
				}
			}

			if (bitCompFlag == 0) {
				/* Buff does not include starting point to compress the bits.
				 * OR there is BIT Synch error.
				 * Then, set the bitSynchFlag as 0 */
				navChannels[prn]->bitSynchFlag = 0;
				cirBuffNavBit[prn]->bitCount = 0;
			}

			if (bitCompFlag == 1) {

				/* Read 20 bits and compress*/
				for (ii = bitCompStartIndex; ii + 19 < BUF_LENGTH; ii += 20) {

					/* BITs compress: Sum 20 bits and make it 1 bit  */
					tempBit = 0;
					for (jj = 0; jj < 20; jj++) {
						tempBit += navChannels[prn]->buffIP[ii + jj];
					}

					/* Change the bit range into [-1, 1] */
					if (tempBit > 0) {
						tempBit = 1;
					} else {
						tempBit = -1;
					}

					/* Update the cirBuff / Channel / Nav bit counter */
					cirBuffUpdateNavBit(cirBuffNavBit, prn, tempBit, navChannels[prn]->buffSamNum[ii]);
					navChannels[prn]->bitSynchSamNum = navChannels[prn]->buffSamNum[ii]; // Sample number
					cirBuffNavBit[prn]->bitCount++;	// NAV bit counter

					/* Find preamble when the NAV bit buffer is fully filled */
					if (cirBuffNavBit[prn]->bitCount >= 300) {

						/* Find preamble: correlation of first 2-10 bits */
						corrResult = 0;
						for (jj = 0; jj < 8; jj++) {
							/* Correlation between preamble and first nav 2-10 bits */
							bufIdx = (cirBuffNavBit[prn]->buffWriteIndex + 2 + jj) % NAVBIT_BUF_LENGTH;
							corrResult += preamble[jj] * cirBuffNavBit[prn]->buffNavBit[bufIdx];
						}

						/* Preamble check (if Value == +/-8)*/
						if (corrResult == 8 || corrResult == -8) {

							/* Read circular buffer into tempBitBuff */
							corrIdx = cirBuffNavBit[prn]->buffWriteIndex; // Just 2 bits before PREAMBLE
							buffOffest = NAVBIT_BUF_LENGTH - corrIdx;
							memcpy(&tempBitBuff[0], &cirBuffNavBit[prn]->buffNavBit[corrIdx], sizeof(si32) * (buffOffest));
							memcpy(&tempBitBuff[buffOffest], &cirBuffNavBit[prn]->buffNavBit[0], sizeof(si32) * (corrIdx));

							/*--- Check the parity of the TLM and HOW words ----------------*/
							memcpy(bitCheck1, &tempBitBuff[0], sizeof(si32) * NUM_PRNS);
							memcpy(bitCheck2, &tempBitBuff[30], sizeof(si32) * NUM_PRNS);

							navParityChk(&status1, bitCheck1);
							navParityChk(&status2, bitCheck2);

							if (status1 != 0 && status2 != 0) {

								/* Parity was OK. Now make NAV bit into 1 and 0.
									 The expression (navBits > 0) returns an array with elements set
									 to 1 if the condition is met and set to 0 if it is not met.*/

								/* reset navBitsBin each time */
								memset(navBitsBin, '\0', sizeof(pi8) * 301);
								for (kk = 0; kk < 302; kk++) {
									if (tempBitBuff[kk] > 0) {
										navBits[kk] = 1;
									} else {
										navBits[kk] = 0;
									}
									snprintf(navBitsConv, 2, "%d", navBits[kk]);
									if (kk == 0) {
										D29Star = navBitsConv[0];
									} else if (kk == 1) {
										D30Star = navBitsConv[0];
									} else {
										strncat(navBitsBin, navBitsConv, 1);
									}
								}

								/* Decode ephemerides for the decoded subframe */
								if (ephemeris(eph, &timeOfSub, &subframeID,
										prn, navBitsBin, 300, D29Star,
										D30Star) == EXIT_SUCCESS) {

									/* Save the absolute sample number for the first subframe start index
									 * "corrIdx" is just 2 nav bits before PREAMBLE */
									navChannels[prn]->firstSubFrameBits =
											cirBuffNavBit[prn]->buffSamNum[(corrIdx	+ 2) % NAVBIT_BUF_LENGTH];

									/* Update the variables */
									cirBuffNavBit[prn]->bitCount = 0;	// Reset the NAV bit counter
									navChannels[prn]->frameSync = 1;	// set the flag
									navChannels[prn]->TOW = timeOfSub;	// current TOW

									/* try to decode after following time */
									navChannels[prn]->decodeWaitCounter = navChannels[prn]->count + 6000;

									calcDateTime(&t, eph->nav[prn].weekNumber, timeOfSub + 6);

									// Save the subframe time data for scheduler
									nav2sch[prn]->prn 		= prn;
									nav2sch[prn]->status 	= 0;
									nav2sch[prn]->lastAbsSample =
											cirBuffNavBit[prn]->buffSamNum[(corrIdx	+ 2 + 299) % NAVBIT_BUF_LENGTH];

								}/* end of if statement */

								else { /* if failed to decode */
									navChannels[prn]->frameSync = 0;
								}/* end of else statement*/
							}
						}
					} /* END: Find the preamble */
				}/*END: FOR loop */
			}
		}
	}

	/* ====================== */

	/* EPHEMERIS UPDATE*/
	/*--------< Check the ephemeris change (See GPS ICD) >--------
		 The issue of ephemeris data (IODE) term shall provide the user
		 with a convenient means for detecting any change in the ephemeris
		 representation parameters. The IODE is provided in both subframes
		 2 and 3 for the purpose of comparison with the 8 LSBs of the IODC
		 term in subframe 1. Whenever these three terms do not match, a
		 data set cutover has occurred and new data must be collected.

		 check if new ephemeris is fully updated. need to wait until fully
		 decoded, otherwise mixed ephemeris parameters are used for
		 navigation solution. for this reason, ther is a ephemeris buffer
		 called "eph.navTemp". Once the navTemp fully updates, then it
		 replaces current ephemeris.*/
	newEphFlag = eph->navTempStatus[prn][0] + eph->navTempStatus[prn][1] + eph->navTempStatus[prn][2];

 	if (newEphFlag == 3) { /*Subframe 1-3 were doecoded*/
		/* check the ephemeris change based on IODC. if current and
			 previous IODC are differenct each other, then the ephemeris
			 is replaced by current one.*/

		eph->navTempStatus[prn][0] = 0;
		eph->navTempStatus[prn][1] = 0;
		eph->navTempStatus[prn][2] = 0;

		if (eph->nav[prn].IODC8bits_sf1	!= eph->navTemp[prn].IODC8bits_sf1) {
			/*check the consistency of the IODC (the first 8 bits of
				 IODC in  subframe 1) and IODE (subframe 2 & 3). */
			if ((eph->navTemp[prn].IODC8bits_sf1 == eph->navTemp[prn].IODE_sf2)
					&& (eph->navTemp[prn].IODE_sf2 == eph->navTemp[prn].IODE_sf3)) {

				/*update the ephemeris*/
				memcpy(&eph->nav[prn], &eph->navTemp[prn], sizeof(nav_s));

				/* set the new ephemeris flag to write the nav rinex
					 % file. it wil be set as "0" after the rinex update.*/
				eph->newEphFlag[prn] = 1;

				/* reset the subframe flag*/
				eph->navStatus[prn][0] = 1;
				eph->navStatus[prn][1] = 1;
				eph->navStatus[prn][2] = 1;

				printf("Ephemeris update: TOW = %0.0f,PRN = %d \n", timeOfSub, prn+1);

			}/*end of ifstatement*/

		}/*end of ifstatement*/

	}/*end of ifstatement*/

	//if subframe 1-3 are decoded, then this channel is ready to use for navigation solution.

	/* check if ephemeris is fully updated: if ephFlag == 3*/
	ephFlag = eph->navStatus[prn][0] + eph->navStatus[prn][1] + eph->navStatus[prn][2];

	/*check the IODC/IODE consistency*/
	ephConsistency =
			((eph->nav[prn].IODC8bits_sf1 == eph->nav[prn].IODE_sf2)
					&& (eph->nav[prn].IODE_sf2 == eph->nav[prn].IODE_sf3));

	if ((si32) ephFlag == 3){
		if (ephConsistency == 1) {
			/* all ephemeris in this channel are available.*/
			if (navChannels[prn]->decode == 0) {

				/* it is the first time for the full ephemeris decoding.*/
				/* set the flag to write the nav rinex file.*/
				eph->newEphFlag[prn] = 1;
				navChannels[prn]->decode = 1;

				printf("All subframes decoded: TOW = %0.0f, PRN = %d, Track Count = %llu \n",
						timeOfSub, prn + 1, navChannels[prn]->count);

			}/*end of if statement*/

		} else {
			eph->navStatus[prn][0] = 0;
			eph->navStatus[prn][1] = 0;
			eph->navStatus[prn][2] = 0;

			eph->navTempStatus[prn][0] = 0;
			eph->navTempStatus[prn][1] = 0;
			eph->navTempStatus[prn][2] = 0;

			eph->nav[prn].IODC8bits_sf1 = 0;
			eph->nav[prn].IODE_sf2 = 0;
			eph->nav[prn].IODE_sf3 = 0;

			eph->navTemp[prn].IODC8bits_sf1 = 0;
			eph->navTemp[prn].IODE_sf2 = 0;
			eph->navTemp[prn].IODE_sf3 = 0;

			navChannels[prn]->bitSynchFlag = 0;

			navChannels[prn]->decode = 0;
		}

	} else {
		/* ephemeris in this channel are not valid.*/
		navChannels[prn]->decode = 0;
	} /*end of else statement*/

	return EXIT_SUCCESS;
}/*end of function*/

/***************************************************************
 *                     BIT SYCHRONIZATION
 *
 * Navigation bit synchronization (Find the NAV bit start point)
 *
 * Input: navChannels	- channel structure & tracking results structure
 *        prn			- structure index, it is "real prn"
 *
 * Return: status of function
 ****************************************************************/
si32 navBitSych(navChannels_s * navChannels[], si32 prn) {

	si32 sumIP = 0;
	si32 maxIPsum = 0;
	si32 bitStartIndex = 0;
	si32 ii = 0;
	si32 jj = 0;

	/* Bit synch */
	if (navChannels[prn]->bitSynchFlag == 0
			&& navChannels[prn]->count >= BITSYNCHLENGTH) {

		/* Initialize the variables for FOR loop iteration */
		maxIPsum = 0;

		/* Find the bit (20ms) starting point */
		for (ii = 0; ii < (si32) BITSYNCHLENGTH - 20; ii++) {
			sumIP = 0;

			/* Check the bit flip such as 0-->1 or 1-->0 */
			if (navChannels[prn]->buffIP[ii - 1]
					* navChannels[prn]->buffIP[ii] < 0) {

				/* Check the bit sign change */
				for (jj = 0; jj < 20; jj++) {
					/* Sum of 20 raw bits*/
					sumIP += navChannels[prn]->buffIP[ii + jj];
				}

				/* Find out the maximum value */
				sumIP = fabs(sumIP);
				if (sumIP > maxIPsum) {
					/* Update the index and maximum value */
					bitStartIndex = ii;
					maxIPsum = sumIP;
				}
			}
		}

		if (maxIPsum > 0) {
			/* Update the results including:
			 *  1) bit index, 2) sample number, 3) 1ms track block counter, 4) synch flag */
			navChannels[prn]->bitSynchSamNum = navChannels[prn]->buffSamNum[bitStartIndex];
			navChannels[prn]->bitSynchFlag = 1;
		} else {
			/* Not found the maximum value */
			navChannels[prn]->bitSynchFlag = 0;
			return EXIT_FAILURE;
		}
	}
	if (navChannels[prn]->bitSynchFlag == 0) {
		/* Not bit synchronized */
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

