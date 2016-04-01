/*
 * initNav.h
 *
 *  Created on: Jul 22, 2015
 *      Author: daehee
 */

#ifndef SRC_INCLUDE_NAV_INIT_H_
#define SRC_INCLUDE_NAV_INIT_H_

#include "cirShiftBuff.h"
#include "initState.h"
#include "initEph.h"
#include "initNavSolutions.h"
#include "apt.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "stdio.h"
#include "conf.h"
#include "conf_nav.h"
#include "conf_swc.h"

#include "initNav2sch.h"

typedef struct {

	// Structure
	state_s state;
	eph_s eph;
	navSolutions_s navSolutions;
	cirBuff_s * cirBuffTrack[NUM_PRNS];
	cirBuffNavBit_s * cirBuffNavBit[NUM_PRNS];
	navChannels_s * navChannels[NUM_PRNS];

	// File ID
	FILE *kmlFid;				// FID for google earth KML file
	FILE *binRnxFid;			// FID for LSS structure log
	FILE *binAptFid;			// FID for APT structure log

	// Variables
	ui64 lastSamNum[NUM_PRNS];	// Previous sample number to check data update

	pi8 rnxFilename[256];
	pi8 aptFilename[256];

} navSuper_s;

si32 nav_init(navSuper_s * navStr);
si32 nav_main(navSuper_s * navStr, apt_s *apt_arr, nav2sch_s ** nav2sch);
void nav_free(navSuper_s * navStr);

#endif /* SRC_INCLUDE_NAV_INIT_H_ */

