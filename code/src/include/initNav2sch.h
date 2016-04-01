/*
 * initSchTime.h
 *
 *  Created on: Nov 25, 2015
 *      Author: daehee
 */

#ifndef SRC_INCLUDE_INITNAV2SCH_H_
#define SRC_INCLUDE_INITNAV2SCH_H_

#include "customDataTypes.h"		/// Standard types
#include "initNavChannels.h"

typedef struct {

	// Time information when the new subframe starts
	si32 prn;				// PRN number
	si32 status;			// 0: Not valid, 1: Valid
	si32 validWeek;		// Will be valid till "validWeek" (week number)
	fl64 validSec;		// Will be valid till "validSec" (second number)
	si32 startWeek;		// Week number when the new subframe started
	fl64 startSec;		// Seconds
	fl64 startAbsSample;	// Absolute sample number
	fl64 lastAbsSample;
	fl64 carr;			// Carrier phase
	fl64 dopp;			// Doppler shift
	fl64 codePhase;		// Code phase

} nav2sch_s;

si32 nav2sch_init(nav2sch_s ** nav2sch);
si32 nav2sch_free(nav2sch_s ** nav2sch);
si32 findSchInfo(navChannels_s *navChannels[], nav2sch_s ** nav2sch,
		si32 * readyChnList, si32 readyChnListSize, fl64 rxTime, fl64 sampleNum);

#endif /* SRC_INCLUDE_INITNAV2SCH_H_ */
