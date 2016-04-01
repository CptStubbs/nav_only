/*
 * leastSquareVel2.h
 *
 *  Created on: May 12, 2015
 *      Author: sean
 */

#ifndef NAV_LEASTSQUAREVEL2_H_
#define NAV_LEASTSQUAREVEL2_H_

#include "customDataTypes.h"

/*!
 * \brief velocity solution 2 using the measured doppler measurement
 * \param vel pointer to 4x1 array containing ecef vx,vy,vz and dtRate
 * \param transmitFreq - transmit frequency for each channel
 * \param receivedFreq - received frequency for each channel
 * \param satPositions - satellite position
 * \param satVelocity - satellite velocity
 * \param userPos - user position (ECEF XYZ)
 * \param activeChnList - list of active channel list
 * \param activeChnListSize - size of active Chn list
 */
si32 leastSquareVel2(
/* Output */
fl64 ** vel,
/* Input */
fl64 * transmitFreq, fl64 * receivedFreq, fl64 ** satPositions,
		fl64 ** satVelocity, fl64 userPos[3], si32 * activeChnList,
		si32 activeChnListSize);

#endif /* NAV_LEASTSQUAREVEL2_H_ */
