/*
 * initState.c
 *
 *  Created on: May 12, 2015
 *      Author: user
 */

/*!
 * togeod.c
 *
 *  Created on: May 7, 2015
 *      Author: sara
 */

#include "customDataTypes.h"
#include "initState.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

si32 initState(state_s * state) {

	/*! Index*/
	state->dataBlock = 0; /*! global index to count the number of block which processed.*/
	state->accNumSamples = 0; /*! accumulated number of samples which processed.*/

	state->numberOfChannels = 0;

	state->navCount = 0; /*!< how many position solution computed*/
	state->navLossCount = 0; /*!< how many times cannot compute the nav solution*/
	state->navFlag = 0; /*!< whether the nav solutions computed previously (1).*/
	/* if it is "0", then try to find the firstSample for navigation again*/

	/* TODO: RINEX format should be defined in later time */
	/*! for RINEX*/
	state->rinexFlag = 0; /*!< previously rinex files opened?*/
	state->rinexFileNo = 1; /*!< for rinex file name index. in this time, it fixed as "1". need to change for multiple files */

	/*! rinex year/doy info. */
	state->year = 0;
	state->doy = 0;

	state->nextTOW = 604800;// next TOW to split out the log files
	state->currTOW = 0;		// current TOW to split out the log files

	state->fileNameFlag = 0; // Status of the file name - 0: not initialized (still o week, 0 sec), 1: initialized (all set to go)

	state->trackBlockCount = 0; // count how many blocks were processed.

	return EXIT_SUCCESS;
}
