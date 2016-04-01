/*
 * leastSquareVel1.h
 *
 *  Created on: May 11, 2015
 *      Author: sean
 */

#ifndef NAV_LEASTSQUAREVEL1_H_
#define NAV_LEASTSQUAREVEL1_H_

#include "customDataTypes.h"
#include "initNavSolutions.h"

/*!
 * \brief Velocity solution 1, using the differential carrier phase measurement
 * \param vel - ecef vx, vy, vz and dtRate
 * \param navSolutions - structure from postNavigation
 * \param carrPhaseDiff - differential carrier phase
 * \param activeChnList - list of active channel list
 * \param activeChnListLength - number of channels
 */
si32 leastSquareVel1(
/* Output */
fl64 ** vel,
/* Input */
navSolutions_s navSolutions, fl64 * carrPhaseDiff, si32 * activeChnList,
		si32 activeChnListLength);

#endif /* NAV_LEASTSQUAREVEL1_H_ */
