/*
 * initNavSolutios.h
 *
 *  Created on: May 11, 2015
 *      Author: user
 */

#ifndef SRC_INCLUDE_INITNAVSOLUTIOS_H_
#define SRC_INCLUDE_INITNAVSOLUTIOS_H_

#include "customDataTypes.h"

typedef struct {
	/* Save list of satellites used for position calculation*/
	fl64 * PRN;
	fl64 * el;
	fl64 * az;
	fl64 * CNo;
	fl64 * doppler;
	fl64 * rawCarrPhase;
	fl64 * carrPhase;
	fl64 * rawP;
	fl64 ** satPositions;
	fl64 ** satPositionsOld;
	fl64 ** satVelocity;
	fl64 * satClkCorr;

	fl64 * iono;
	fl64 * tropo;

	fl64 * transmitFreq;
	fl64 * receivedFreq;

	fl64 * correctedDop;
	fl64 * correctedP;
	fl64 * correctedCP;
	fl64 * geoRange;

	fl64 * carrCorrCount;
} navSolChannelStruct;

typedef struct {

	/*channel dependent variables are in the navChannelStruct*/
	navSolChannelStruct channel;

	fl64 firstSample;   /* the sample number for the first navigation calculation*/
	fl64 sampleNum;

	/* channel independent variables*/
	fl64 * velSolPRNlist;
	si32 * posSolPRNlist;


	fl64  xyzdt[4];
	fl64  xyzdtOld[4];

	fl64 VX;
	fl64 VY;
	fl64 VZ;
	fl64 dtRate;
	/* Not necessary since we are making this design for a LEO S/C*/
	fl64 E;
	fl64 N;
	fl64 U;

	fl64 latitude;
	fl64 longitude;
	fl64 height;

    /* parameters for second phase of tracking */
 	fl64 DOP[5];

	fl64 rxTime[2];
	fl64 absoluteSample[2];
	fl64 * samPerSecBuff;
	fl64 rawRxTime;
	fl64 utmZone;

	fl64 avgClock;
	fl64 firstSampleTime;

	fl64 weekNumber;
	fl64 firstSampleTimeUTC;
	fl64 leapSec;

} navSolutions_s;

si32 initNavSolution(navSolutions_s * navSolution); /* have to declare channel somewhere else.*/
void deleteNavSolution(navSolutions_s * navSolution);

#endif /* SRC_INCLUDE_INITNAVSOLUTIOS_H_ */
