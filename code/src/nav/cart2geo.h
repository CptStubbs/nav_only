/*
 * cart2geo.h
 *
 *  Created on: May 12, 2015
 *      Author: sean
 */

#ifndef NAV_CART2GEO_H_
#define NAV_CART2GEO_H_

#include "customDataTypes.h"

/*!
 * \brief Conversion of Cartesian coordinates (X,Y,Z) to geographical
 * coordinates (phi, lambda, h) on a selected reference ellipsoid.
 * \param phi - pointer to the phi coordinate
 * \param lambda - pointer to the lambda coordinate
 * \param h - pointer to the h coordinate
 * \param X - X coordinate
 * \param Y - Y coordinate
 * \param Z - Z coordinate
 * \param i - Choices i of Reference Ellipsoid for Geographical Coordinates
 * 1. International Ellipsoid 1924
 * 2. International Ellipsoid 1967
 * 3. World Geodetic System 1972
 * 4. Geodetic Reference System 1980
 * 5. World Geodetic System 1984
 */

void cart2geo(
/* Output */
fl64 * phi, fl64 * lambda, fl64 * h,
/* Input */
fl64 X, fl64 Y, fl64 Z, si32 i

);

#endif /* NAV_CART2GEO_H_ */
