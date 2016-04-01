/*
 * initNav.c
 *
 *  Created on: Jul 22, 2015
 *      Author: daehee
 */
#include "conf.h"
#include "conf_nav.h"
#include "conf_swc.h"
#include "initNavChannels.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "nav_init.h"
#include <string.h>

#include "../include/cirShiftBuff.h"
#include "customDataTypes.h"

#include "initState.h"
#include "initEph.h"
#include "initNavSolutions.h"
#include "doDecoding.h"
#include "doNavigation.h"

/* Initialize NAVigation logic */
si32 nav_init(navSuper_s * navStr) {

	/* define the structure */
	si32 i = 0;		// FOR loop counter

	/*=====< Structure initialization >=====*/
	/* Channel independent structures */
	initState(&navStr->state);
	initEph(&navStr->eph);
	initNavSolution(&navStr->navSolutions);

	/* Channel dependent structures */
	for (i = 0; i < NUM_PRNS; i++) {

		/* Initialize the circular buffer */
		navStr->cirBuffTrack[i] = (cirBuff_s *) malloc(sizeof(cirBuff_s));
		ERRORCHECK(navStr->cirBuffTrack[i] == NULL)
		if (initCirBuff(navStr->cirBuffTrack[i]) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}

		navStr->cirBuffNavBit[i] = (cirBuffNavBit_s *) malloc(
				sizeof(cirBuffNavBit_s));
		ERRORCHECK(navStr->cirBuffNavBit[i] == NULL)
		if (initCirBuffNavBit(navStr->cirBuffNavBit[i]) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}

		/* Initialize the navSuperStruct * navStrtrackResults */
		navStr->navChannels[i] = (navChannels_s *) malloc(
				sizeof(navChannels_s));
		ERRORCHECK(navStr->navChannels[i] == NULL)
		if (initNavChannels(navStr->navChannels[i]) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}

		/* Initialize the NAV variables*/
		navStr->state.checkPRN[i] = 0;// Initialize the SV availability: default is '0'
		navStr->lastSamNum[i] = 0;// Previous sample number to check data update

		//Open all the log files
#if LOGRNX	//generate file name & Open it
	snprintf(navStr->rnxFilename, 256, RNXOUT, 0, 0);
	navStr->binRnxFid = fopen(navStr->rnxFilename, "wb");
	ERRORCHECK(navStr->binRnxFid == NULL)
	writeHeader(navStr->binRnxFid);
	writernxHeader(navStr->binRnxFid);
#endif

	//open output files.
#if LOGAPT	//generate file name & Open it
	snprintf(navStr->aptFilename, 256, APTOUT, 0, 0);
	navStr->binAptFid = fopen(navStr->aptFilename, "wb");
	ERRORCHECK(navStr->binAptFid == NULL)
	writeHeader(navStr->binAptFid);
	writeaptemtHeader(navStr->binAptFid);
#endif


	}

	return EXIT_SUCCESS;
}

void nav_free(navSuper_s * navStr) {
	si32 i = 0;

	deleteNavSolution(&navStr->navSolutions);

	for (i = 0; i < NUM_PRNS; i++) {
		deleteCirBuff(navStr->cirBuffTrack[i]);
		deleteCirBuffNavBit(navStr->cirBuffNavBit[i]);
		free(navStr->cirBuffTrack[i]);
		deleteNavChannels(navStr->navChannels[i]);
		free(navStr->navChannels[i]);
	}
#if LOGRNX
	fclose(navStr->binRnxFid);
#endif
#if LOGAPT
	fclose(navStr->binAptFid);
#endif

}
