/*
 * twosComp2dec.h
 *
 *  Created on: May 13, 2015
 *      Author: sara
 */

#ifndef SRC_NAV_TWOSCOMP2DEC_H_
#define SRC_NAV_TWOSCOMP2DEC_H_
#include "customDataTypes.h"

/* \brief  Converts a two's-complement binary number
 BINNUMBER (in Matlab it is a string type), represented as a row vector of
 zeros and ones, to an integer.

 Inputs:
 \params binaryNumber      - two's-complement binary number

 Outputs:
 \params intNumber         - integer number*/
si32 twosComp2dec(
/* Output */
fl64 * intNumber,
/* Input */
pi8 * binaryNumber, si32 binaryNumberSize);
#endif /* SRC_NAV_TWOSCOMP2DEC_H_ */
