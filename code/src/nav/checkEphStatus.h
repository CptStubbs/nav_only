/*
 * checkEphStatus.h
 *
 *  Created on: May 12, 2015
 *      Author: sara
 */

#ifndef SRC_NAV_CHECKEPHSTATUS_H_
#define SRC_NAV_CHECKEPHSTATUS_H_
#include "initEph.h"
#include "initNavChannels.h"
/*! \brief Check the status of ephemeris (update status and health status)

 Inputs:
 \param nav               	- ephemeris nav data
 \param rxTime				- Receiver time
 \param navChannels			- channel status
 Outputs:
 \param activeChnList     	- list of available channels
 \param activeChnSize		- number of channels
*/

si32 checkEphStatus(
/* Output */
si32 * activeChnList, si32 * activeChnSize,
/* Input */
nav_s * nav[], fl64 rxTime, navChannels_s * navChannels[]);
#endif /* SRC_NAV_CHECKEPHSTATUS_H_ */
