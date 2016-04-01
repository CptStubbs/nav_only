/*
 * satPosVel.h
 *
 *  Created on: May 12, 2015
 *      Author: sara
 */

#ifndef SRC_NAV_SATPOSVEL_H_
#define SRC_NAV_SATPOSVEL_H_

#include "initEph.h"
#include "customDataTypes.h"

/*!
 * brief SATPOS and Velocity Calculation of X,Y,Z satellites coordinates at TRANSMITTIME
 for given ephemeris EPH. Coordinates are calculated for each satellite in the
 list PRNLIST.

 Inputs:
 \param transmitTime  - transmission time for all satellites
 \param eph           - ephemeridies of satellites
 \param activeChnList - Active channel list
 \param activeChnListSize - size of active channels

 Outputs:
 \param satPositions  - positions of satellites (in ECEF system [X; Y; Z;])
 \param satVelocity   - velocity of satellites (in ECEF system [VX; VY; VZ;])
 \param satClkCorr    - correction of satellites clocks*/

void satPosVel(
/* Output */
fl64 * satPositions[3], fl64 * satVelocity[3], fl64 * satClkCorr,
/* Input */
fl64 * transmitTime, nav_s * eph[], si32 * activeChnList,
		si32 activeChnListSize);

#endif /* SRC_NAV_SATPOSVEL_H_ */
