/*
 * cart2utm.h
 *
 *  Created on: May 12, 2015
 *      Author: sean
 */

#ifndef NAV_CART2UTM_H_
#define NAV_CART2UTM_H_

#include "customDataTypes.h"
/*!
 * \brief Transformation of (X,Y,Z) to (N,E,U) in UTM, zone 'zone'
 * \param E - pointer to UTM coordinate Easting
 * \param N - pointer to UTM coordinate Northing
 * \param U - pointer to UTM coordinate UPing
 * \param X - Cartesian coordinate X. Coordinates are referenced
 * with respect to the International Terrestrial Reference
 * Frame 1996 (ITRF96)
 * \param Y - Cartesian coordinate Y. Coordinates are referenced
 * with respect to the International Terrestrial Reference
 * Frame 1996 (ITRF96)
 * \param Z - Cartesian coordinate Z. Coordinates are referenced
 * with respect to the International Terrestrial Reference
 * Frame 1996 (ITRF96)
 * \param zone - UTM zone of the given position
 */

void cart2utm(
/* Output */
fl64 * E, fl64 * N, fl64 * U,
/* Input */
fl64 X, fl64 Y, fl64 Z, fl64 zone);

#endif /* NAV_CART2UTM_H_ */
