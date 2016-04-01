/*
 * ephemeris.h
 *
 *  Created on: May 13, 2015
 *      Author: sara
 */

#ifndef SRC_NAV_EPHEMERIS_H_
#define SRC_NAV_EPHEMERIS_H_
#include "initEph.h"
#include "customDataTypes.h"
/*! \brief Function decodes ephemerides and TOW from the given bit stream. The stream
 (array) in the parameter BITS must contain 1500 bits. The first element in
 the array must be the first bit of a subframe. The subframe ID of the
 first subframe in the array is not important.


 eph = ephemeris(bits, D30Star)

 Inputs:
 \param PRN         - current PRN to scan for information on
 \param bits 		- bits of the navigation messages (5 subframes).
 Type is character array and it must contain only
 characters '0' or '1'.
 \param bitsSize    - length of the bits array
 \param D29Star     - The 29th bit of the previous nav-word. Refer to the
 GPS interface control document ICD (IS-GPS-200D) for
 more details on the parity checking. Parameter type is
 char. It must contain only characters '0' or '1'.
 \param D30Star     - The 30th (last) bit of the previous nav-word. Refer to
 the GPS interface control document ICD (IS-GPS-200D) for
 more details on the parity checking. Parameter type is
 char. It must contain only characters '0' or '1'.
 Outputs:
 \param eph         - SV ephemeris
 \param TOW         - Time Of Week (TOW) of the sub-frame in the bit stream (in seconds)
 \param subframeID - pointer to the current subframe for analysis
 */
si32 ephemeris(
/* Output */
eph_s * eph, fl64 * TOW, si32 * subframeID,
/* Input */
si32 PRN, pi8 * bits, si32 bitsSize, pi8 D29Star, pi8 D30Star);
#endif /* SRC_NAV_EPHEMERIS_H_ */
