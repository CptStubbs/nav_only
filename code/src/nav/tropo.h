/*
 * tropo.h
 *
 *  Created on: May 11, 2015
 *      Author: sean
 */

#ifndef NAV_TROPO_H_
#define NAV_TROPO_H_

#include "customDataTypes.h"

/*!
 * \brief Calculation of tropospheric correction.
 * 		  The range correction ddr in m is to be subtracted from
 * 		  pseudo-ranges and carrier phases.
 * \param ddr - pointer to range correction (meters)
 * \param sinel - sin of elevation angle of the satellite
 * \param hsta - height of the station in km
 * \param p - atmospheric pressure in mb at height hp
 * \param tkel - surface temperature in degrees Kelvin at height htkel
 * \param hum - humidity in % at height hhum
 * \param hp - height of pressure measurement in km
 * \param htkel - height of temperature measurement in km
 * \param hhum - height of humidity measurement in km
 */
void tropo(
/* Output */
fl64 * ddr,
/* Input */
fl64 sinel, fl64 hsta, fl64 p, fl64 tkel, fl64 hum, fl64 hp, fl64 htkel,
		fl64 hhum);

#endif /* NAV_TROPO_H_ */
