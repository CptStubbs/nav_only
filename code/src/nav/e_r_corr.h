/*
 * e_r_corr.h
 *
 *  Created on: May 11, 2015
 *      Author: sean
 */

#ifndef NAV_E_R_CORR_H_
#define NAV_E_R_CORR_H_

#include "customDataTypes.h"

/*!
 * \brief Returns rotated satellite ECEF coordinates due to Earth
 * rotation during signal travel time
 * \param X_sat_rot - pointer to rotated satellite's coordinates (ECEF)
 * \param travelTime - signal travel time
 * \param X_sat - satellite's ECEF coordinates
 */
void e_r_corr(
/* Output */
fl64 X_sat_rot[3],
/* Input */
fl64 travelTime, fl64 X_sat[3]);

#endif /* NAV_E_R_CORR_H_ */
