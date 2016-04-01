/*
 * initTrackResults.c
 *
 *  Created on: May 28, 2015
 *      Author: daehee
 */

#include <conf_nav.h>
#include "initNavChannels.h"

#include "customDataTypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

si32 initNavChannels(navChannels_s * navChannels) {

	navChannels->PRN = 0; /* PRN number of the tracked satellite */
	navChannels->status = 'A'; /* Mode/status of the tracking channel: acquire */
	navChannels->Lock = 0; /* Signal is locked (1) or not (0) */

	navChannels->bitSynchFlag = 0; /* 0: no, 1: bit synch */
	navChannels->bitSynchSamNum = 0; /* sample number at bit synch */

	navChannels->frameSync = 0; /* 0: no preamble, 1: found the preamble & verified the 6sec gap */
	navChannels->firstSubFrameBits = 0; /* bit index of first preamble */

	navChannels->TOW = 0; /* TOW for current channel */
	navChannels->decode = 0; /* whether ephemeris fully decoded or not */


	/*! Channel status */
	navChannels->count = 0;
	navChannels->samples = 0;
	//navChannels->PRN = -1;

	/*! time for the phase lock and the threshold */
	navChannels->LockTime = 0;

	/*! C/No */
	navChannels->CNo = 0;
	//navChannels->VSMIndex = 0;

	navChannels->buffSamNum = (fl64 *) malloc( BUF_LENGTH * sizeof(fl64));
	ERRORCHECK(navChannels->buffSamNum == NULL)
	navChannels->buffSamNumInt = (ui64 *) malloc( BUF_LENGTH * sizeof(ui64));
	ERRORCHECK(navChannels->buffSamNumInt == NULL)
	navChannels->buffSamNumFrac = (fl32 *) malloc( BUF_LENGTH * sizeof(fl32));
	ERRORCHECK(navChannels->buffSamNumFrac == NULL)
	navChannels->buffIP = (si32 *) malloc( BUF_LENGTH * sizeof(si32));
	ERRORCHECK(navChannels->buffIP == NULL)
	navChannels->buffDopp = (fl64 *) malloc( BUF_LENGTH * sizeof(fl64));
	ERRORCHECK(navChannels->buffDopp == NULL)
	navChannels->buffCarr = (fl64 *) malloc( BUF_LENGTH * sizeof(fl64));
	ERRORCHECK(navChannels->buffCarr == NULL)
	navChannels->buffCodePhase = (fl64 *) malloc( BUF_LENGTH * sizeof(fl64));
	ERRORCHECK(navChannels->buffCodePhase == NULL)

	navChannels->decodeWaitCounter = BUF_LENGTH;

	return EXIT_SUCCESS;
}

void deleteNavChannels(navChannels_s * navChannels) {
	free(navChannels->buffSamNum);
	free(navChannels->buffSamNumInt);
	free(navChannels->buffSamNumFrac);
	free(navChannels->buffIP);
	free(navChannels->buffDopp);
	free(navChannels->buffCarr);
	free(navChannels->buffCodePhase);
}
