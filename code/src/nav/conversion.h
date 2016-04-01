/*
 * conversion.h
 *
 *  Created on: May 12, 2015
 *      Author: sean
 */

#ifndef NAV_CONVERSION_H_
#define NAV_CONVERSION_H_

#include "initEph.h"
#include "customDataTypes.h"
#include "initNavSolutions.h"
#include "initSettings.h"
#include "rinexDataStruct.h"

/*!
 * \brief Construct the rinex data structure
 * Currently only the satellite that is visible for the whole period will be
 * wrote into the rinex file
 * \return EXIT_SUCCESS on success. EXIT_FAILURE otherwise
 * \param rinexData - pointer to Rinex data structure
 * \param eph - pointer to ephemeris (new ephemeris flag may be updated within this function)
 * \param navSolutions - Data structure from navSolution
 * \param settings - receiver settings
 * \param activeChnList - valid channel list
 */
si32 conversion(
/* Output */
rinexData_s * rinexData, eph_s * eph,
/* Input */
navSolutions_s navSolutions, settings_s settings, fl64 * activeChnList);

#endif /* NAV_CONVERSION_H_ */
