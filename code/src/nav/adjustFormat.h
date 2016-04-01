/*
 * adjustFormat.h
 *
 *  Created on: May 19, 2015
 *      Author: sean
 */

#ifndef NAV_ADJUSTFORMAT_H_
#define NAV_ADJUSTFORMAT_H_

#include "customDataTypes.h"

/*!
 * \brief Adjust the dataformat in rinex files
 * \return EXIT_SUCCESS or FAILURE
 * \param outStr - pointer to output data stream
 * \param inStr - pointer to input data stream
 * \param length - length of the output stream
 */
si32 adjustFormat(
/* Output */
pi8 * outStr,
/* Input */
pi8 * inStr, ui32 length);

#endif /* NAV_ADJUSTFORMAT_H_ */
