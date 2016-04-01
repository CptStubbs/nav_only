/*
 * check_t.h
 *
 *  Created on: May 12, 2015
 *      Author: sara
 */

#ifndef SRC_NAV_CHECK_T_H_
#define SRC_NAV_CHECK_T_H_
#include "customDataTypes.h"

/*!
 * \brief CHECK_T accounting for beginning or end of week crossover.

 corrTime = check_t(time);

 Inputs:
 \param time        - time in seconds

 Outputs:
 \param corrTime    - corrected time (seconds)*/

void check_t(
/* Output */
fl64 * corrTime,
/* Input */
fl64 time);

#endif /* SRC_NAV_CHECK_T_H_ */
