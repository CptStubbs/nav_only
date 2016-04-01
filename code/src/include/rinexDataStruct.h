/*
 * rinexDataStruct.h
 *
 *  Created on: May 12, 2015
 *      Author: sean
 */

#ifndef INCLUDE_RINEXDATASTRUCT_H_
#define INCLUDE_RINEXDATASTRUCT_H_

#include "initEph.h"
#include "initNavSolutions.h"

typedef struct {
	/*! Time information @{ */
	fl64 firstRxTime;
	fl64 lastRxTime;
	fl64 rxTime;
	/*! @} */

	/*! GPS week Number */
	fl64 weekNumber;

	/*! Almanac */
	almanac_s almanac;

	/*! Reference ECEF Coordinates @{ */
	fl64 X;
	fl64 Y;
	fl64 Z;
	/*! @} */

	/*! PRN */
	si32 PRN;

	/*! Eph nav data */
	nav_s eph;

	/*! raw measurements (clock bias and drift remain in the measurements ) @{ */
	fl64 pseudorange;
	fl64 carrierPhase;
	fl64 doppler;
	/*! @} */

	/*! CNo information */
	fl64 CNo;
	/*! if it is "1" then write the nav rinex file */
	si32 newEphFlag;

} rinexData_s;

#endif /* INCLUDE_RINEXDATASTRUCT_H_ */
