/*
 * clsin.h
 *
 *  Created on: May 12, 2015
 *      Author: sean
 */

#ifndef NAV_CLSIN_H_
#define NAV_CLSIN_H_

#include "customDataTypes.h"

/*!
 * \brief Clenshaw summation of sinus of argument.
 * \param result - pointer to the result value
 * \param ar - sinus argument
 * \param degree - initial starting angle
 * \param argument - argument to the function
 */

void clsin(
/* Output */
fl64 * result,
/* Input */
fl64 * ar, si32 degree, fl64 argument);

#endif /* NAV_CLSIN_H_ */
