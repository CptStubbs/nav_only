/*
 * corrCarrPhase.c
 *
 *  Created on: May 21, 2015
 *      Author: sean
 */

#include <conf.h>
#include <conf_nav.h>
#include "customDataTypes.h"
#include "initNavSolutions.h"
#include "corrCarrPhase.h"
#include <math.h>

void corrCarrPhase(navSolutions_s * navSolutions, fl64 ** carrPhase, si32 numberOfChannels) {

	fl64 diffRxTime = navSolutions->rxTime[1] - navSolutions->rxTime[0];
	fl64 diffCarrPhase = 0;
	si32 ii = 0;

	for (ii = 0; ii < numberOfChannels; ii++) {
		if (!isnan(navSolutions->channel.rawP[ii])) {
			navSolutions->channel.carrCorrCount[ii] += 1;

			/*! initialization for clock steering off */
			if (CLKSTRON == 0) {
				if (navSolutions->channel.carrCorrCount[ii] == 1) {
					navSolutions->channel.rawCarrPhase[ii] =
							navSolutions->channel.rawP[ii] / SPEEDOFLIGHT
									* L1FREQ;
					navSolutions->channel.correctedCP[ii] =
							navSolutions->channel.correctedP[ii] / SPEEDOFLIGHT
									* L1FREQ;
				} else if (navSolutions->channel.carrCorrCount[ii] > 1) {
					navSolutions->channel.rawCarrPhase[ii] += carrPhase[ii][1]
							- carrPhase[ii][0];
					diffCarrPhase = (carrPhase[ii][1] - carrPhase[ii][0])
							- navSolutions->dtRate * diffRxTime
									* L1FREQ / SPEEDOFLIGHT;
					navSolutions->channel.correctedCP[ii] += diffCarrPhase;
				}

			} else if (CLKSTRON == 1) {
				if (navSolutions->channel.carrCorrCount[ii] == 3) {
					navSolutions->channel.correctedCP[ii] =
							navSolutions->channel.rawP[ii] / SPEEDOFLIGHT
									* L1FREQ;
				} else if (navSolutions->channel.carrCorrCount[ii] > 3) {
					/*! Correct the differential carrier phase by dt-range */
					diffCarrPhase = (carrPhase[ii][1] - carrPhase[ii][0])
							- navSolutions->dtRate * diffRxTime
									* L1FREQ / SPEEDOFLIGHT;
					/*! accumulate to get the corrected carrier phase measurement */
					navSolutions->channel.correctedCP[ii] += diffCarrPhase;
				}
			}
		} else {
			navSolutions->channel.carrCorrCount[ii] = 0;
		}
	}
}
