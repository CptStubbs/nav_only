/*
 * initState.h
 *
 *  Created on: May 11, 2015
 *      Author: user
 */

#ifndef SRC_INCLUDE_INITSTATE_H_
#define SRC_INCLUDE_INITSTATE_H_

#include "customDataTypes.h"

/*! Create structure for the State*/
typedef struct {
	/* Index*/
	si32 dataBlock; /* global index to count the number of block which processed.*/
	fl64 accNumSamples; /* accumulated number of samples which processed.*/

	si32 numberOfChannels;

	/* for navigation*/
	si32 navCount; /* how many position solution computed*/
	si32 navLossCount; /* how many times cannot compute the nav solution*/
	si32 navFlag; /* whether the nav solutions computed previously (1)*/

	/* TODO: RINEX format should be defined in later time */
	/* for RINEX */
	si32 rinexFlag; /* previously rinex files opened?*/
	si32 rinexFileNo; /* for rinex file name index. in this time, it fixed as "1". need to change for multiple files */

	/* rinex year/doy info. */
	si32 year;
	si32 doy;

	si32 checkPRN[32];

	si32 nextTOW;	// next TOW to split out the log files
	si32 nextWeek;
	si32 currTOW;	// current TOW to split out the log files

	si32 fileNameFlag; 	// Status of the file name - 0: not initialized (still o week, 0 sec), 1: initialized (all set to go)

	ui64 trackBlockCount; 	// count how many blocks were processed.

} state_s;

si32 initState(state_s * state); /* have to declare channel somewhere else.*/

#endif /* SRC_INCLUDE_INITSTATE_H_ */
