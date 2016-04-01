/*
 * clksin.h
 *
 *  Created on: May 12, 2015
 *      Author: sean
 */

#ifndef NAV_CLKSIN_H_
#define NAV_CLKSIN_H_

#include "customDataTypes.h"

/*!
 * \brief Clenshaw summation of sinus with complex argument
 * \param re - pointer to the real result
 * \param im - pointer to the imaginary result
 * \param ar - sinus arg
 * \param degree - starting degree for the summation
 * \param arg_real - real argument
 * \param arg_imag - imaginary argument
 */
void clksin(
/* Output */
fl64 * re, fl64 * im,
/* Input */
fl64 * ar, si32 degree, fl64 arg_real, fl64 arg_imag);

#endif /* NAV_CLKSIN_H_ */
