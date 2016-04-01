/*
 * calUTC.h
 *
 *  Created on: May 12, 2015
 *      Author: sean
 */

#ifndef NAV_CALUTC_H_
#define NAV_CALUTC_H_

#include "initEph.h"
#include "customDataTypes.h"

/*!
 * \brief Calculate UTC time from GPS time based on broadcast almanac
 * \param utcTime - pointer to UTC time of week
 * \param almanac - UTC parameters
 * \param gpsTime - GPS time of week
 * \param WN - GPS week number
 */

void calUTC(
/* Output */
fl64 * utcTime,
/* Input */
almanac_s almanac, fl64 gpsTime, fl64 WN);

#endif /* NAV_CALUTC_H_ */
