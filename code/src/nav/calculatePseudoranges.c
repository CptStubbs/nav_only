/*
 * calculatePseudoranges.c
 *
 *  Created on: May 13, 2015
 *      Author: sara
 */
#include <tgmath.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "calculatePseudoranges.h"

#include <conf_nav.h>
#include "customDataTypes.h"

si32 calculatePseudoranges(
/* Output */
fl64 * pseudoranges,
/* Input */
fl64 * transmitTime, fl64 rxTime, si32 * channelList, si32 channelListSize) {

	fl64 travelTime;
	si32 channelNr = 0;
	si32 number = 0;

	for (channelNr = 0; channelNr < channelListSize; channelNr++) {
		number = channelList[channelNr];/*SARA now provides PRN number instead of channel number*/
		travelTime = rxTime - transmitTime[number];

		if (travelTime > (fl64) SECOFWEEK){
			travelTime = travelTime - (fl64) SECOFWEEK;
		}
		else if(travelTime < 0){
			travelTime = travelTime + (fl64) SECOFWEEK;
		}

		pseudoranges[number] = travelTime * SPEEDOFLIGHT;

	}

	return EXIT_SUCCESS;
}
