/*
 * topocent.h
 *
 *  Created on: May 11, 2015
 *      Author: sean
 */

#ifndef NAV_TOPOCENT_H_
#define NAV_TOPOCENT_H_

#include "customDataTypes.h"

/*!
 * \brief Transformation of vector dx into topocentric coordinate
 *  system with origin at X.
 *  Both parameters are 3 by 1 vectors
 * \return EXIT_SUCCESS on success. EXIT_FAILURE otherwise
 * \param Az - Pointer to Azimuth from north positive clockwise, degrees
 * \param El - Pointer to elevation angle, degrees
 * \param D -  Pointer to vector length. Units like units of the input
 * \param X - vector origin coordinates (in ECEF system [X; Y; Z;])
 * \param dx - vector ([dX; dY; dZ;])
 */
si32 topocent(
/* Output */
fl64 * Az, fl64 * El, fl64 * D,
/* Input */
fl64 X[3], fl64 dx[3]);

#endif /* NAV_TOPOCENT_H_ */
