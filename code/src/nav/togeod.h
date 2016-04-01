/*
 * togeod.h
 *
 *  Created on: May 7, 2015
 *      Author: sean
 */

#ifndef SRC_NAV_TOGEOD_H_
#define SRC_NAV_TOGEOD_H_

#include "customDataTypes.h"

/*!
 * \brief Subroutine to calculate geodetic coordinates latitude, longitude,
 * height given Cartesian coordinates X,Y,Z, and reference ellipsoid
 * values semi-major axis (a) and the inverse of flattening (finv).
 * \return EXIT_SUCCESS on success. EXIT_FAILURE otherwise
 * \param dphi - pointer to the latitude results
 * \param dlambda - pointer to the longitude results
 * \param h - height above reference ellipisoid
 * \param a - semi-major axis of the reference ellipsoid
 * \param finv - inverse of flattening of the reference ellipsoid
 * \param X - Cartesian coordinate
 * \param Y - Cartesian Coordinates
 * \param Z - Cartesian coordinate
 *
 * The units of linear parameters X,Y,Z,a must all agree (m,km,mi,ft,..etc)
 * The output units of angular quantities will be in decimal degrees
 * 15.5 degrees not 15 deg 30 min). The output units of h will be the
 */
si32 togeod(
/* Output */
fl64 * dphi, fl64 * dlambda, fl64 * h,
/* Input */
fl64 a, fl64 finv, fl64 X, fl64 Y, fl64 Z);

#endif /* SRC_NAV_TOGEOD_H_ */
