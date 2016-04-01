/*
 * checkEphStatus.c
 *
 *  Created on: May 13, 2015
 *      Author: sara
 */
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "checkEphStatus.h"

#include <conf_nav.h>
#include "initEph.h"
#include "initNavChannels.h"
#include "customDataTypes.h"

si32 checkEphStatus(
/* Output */
si32 * activeChnList, si32 * activeChnSize,
/* Input */
nav_s * nav[], fl64 rxTime, navChannels_s * navChannels[]) {

	si32 channelNr = 0;
	si32 number = 0;
	si32 cnt = 0;
	/*si32 h = 1;*/
	si32 ii = 0;
	fl64 * tempactiveChnList;
	fl64 toeGap = 0;
	fl64 towGap = 0;

	tempactiveChnList = (fl64*) malloc(*activeChnSize * sizeof(fl64)); /*! the last 1ms data in the buffer. it includes un-processed samples which*/
	ERRORCHECK(tempactiveChnList == NULL)

	/* Just check if the satellite is healty*/
	for (channelNr = 0; channelNr < *activeChnSize; channelNr++) {
		number = (si32) activeChnList[channelNr];

		toeGap = fabs(nav[number]->t_oe 		- rxTime);
		towGap = fabs(navChannels[number]->TOW  - rxTime);

		if ((nav[number]->i_0 == 0.0)
				|| (nav[number]->accuracy >= 3)
					|| (nav[number]->health != 0)) {
			navChannels[number]->decode = -1;
		}
		/*Check the time of ephemeris */
		else if ( toeGap > 7200 ) { // +/- 2 hour window
			if (toeGap > 597600 && toeGap < SECOFWEEK) {// in case of the week rollover
				tempactiveChnList[cnt] = number;
				cnt = cnt + 1;
			}
			else { // old ephemeris
				printf("Old ephemeris for PRN %d\n", number);
				navChannels[number]->decode = -1;
			}
		}
		/* check the TOW is weird or not. 300sec (5min) is user parameter.*/
		else if (towGap > 300){
			if (towGap > 604500 && towGap < SECOFWEEK) {// in case of the week rollover
				tempactiveChnList[cnt] = number;
				cnt = cnt + 1;
			}
			else { // wrong TOW, then reset the channel
				printf("Wrong TOW for PRN %d\n", number);
				navChannels[number]->decode = -1;
			}
		}
		else {
			tempactiveChnList[cnt] = number;
			cnt = cnt + 1;
		}
	}

	/*Make a list of the channels that actually are healty*/
	for (ii=0;ii<cnt;ii++){
		activeChnList[ii] = tempactiveChnList[ii];
	}
	*activeChnSize = cnt;

	free(tempactiveChnList);
	return EXIT_SUCCESS;
}
