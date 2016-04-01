/*
 * findTransmitTime.h
 *
 *  Created on: May 12, 2015
 *      Author: sara
 */

#ifndef SRC_NAV_FINDTRANSTIME_H_
#define SRC_NAV_FINDTRANSTIME_H_
#include "../include/initNavChannels.h"
#include "initState.h"
#include "customDataTypes.h"
/*!
 \brief findTransTime finds the transmitting time of each satellite at a specified
 sample number using the interpolation

 Inputs:
 \param sampleNum     - absolute sample number from the tracking loop
 \param readyChnListSize -size of the readyChnList array.
 \param trackResults  - output from the tracking function
 \param state  		  - hnnel independent variables
 Outputs:
 \param transmitTime  - transmitting time all ready satellites*/
si32 findTransTime(
/* Output */
fl64 * transmitTime,
/* Input */
fl64 sampleNum, si32 readyChnListSize, navChannels_s * trackResults[], state_s *state);
#endif /* SRC_NAV_FINDTRANSTIME_H_ */
