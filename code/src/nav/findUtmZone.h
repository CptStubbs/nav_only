/*
 * findUtmZone.h
 *
 *  Created on: May 12, 2015
 *      Author: sean
 */

#ifndef NAV_FINDUTMZONE_H_
#define NAV_FINDUTMZONE_H_

#include "customDataTypes.h"

/*!
 * \brief Function finds the UTM zone number for given longitude and latitude.
 * The longitude value must be between -180 (180 degree West) and 180 (180
 * degree East) degree. The latitude must be within -80 (80 degree South) and
 * 84 (84 degree North).
 * \return EXIT_SUCCESS on success. EXIT_FAILURE otherwise
 * \param utmZone - pointer to the UTM zone number
 * \param latitude - latitude of the point in decimal degrees
 * \param longitude - longitude of the point in decimal degrees
 */

si32 findUtmZone(
/* Output */
si32 * utmZone,
/* Input */
fl64 latitude, fl64 longitude);

#endif /* NAV_FINDUTMZONE_H_ */
