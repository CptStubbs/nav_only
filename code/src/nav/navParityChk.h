/*
 * navParityChk.h
 *
 *  Created on: May 13, 2015
 *      Author: user
 */

#ifndef SRC_NAV_NAVPARITYCHK_H_
#define SRC_NAV_NAVPARITYCHK_H_
#include "initEph.h"
#include "customDataTypes.h"
/*! \brief This function is called to compute and status the parity bits on GPS word.
 Based on the flowchart in Figure 2-10 in the 2nd Edition of the GPS-SPS
 Signal Spec.

 Inputs:
 \param ndat        - an array (1x32) of 32 bits represent a GPS navigation
 word which is 30 bits plus two previous bits used in
 the parity calculation (-2 -1 0 1 2 ... 28 29)
 Outputs:
 \param status      - the test value which equals EITHER +1 or -1 if parity
 PASSED or 0 if parity fails.  The +1 means bits #1-24
 of the current word have the correct polarity, while -1
 means the bits #1-24 of the current word must be
 inverted.*/
void navParityChk(
/* Output */
si32 * status,
/* Input */
si32 * ndat);
#endif /* SRC_NAV_NAVPARITYCHK_H_ */
