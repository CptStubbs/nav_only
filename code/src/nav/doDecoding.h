/*
 * doDecoding.h
 *
 *  Created on: May 11, 2015
 *      Author: user
 */

#ifndef SRC_NAV_DODECODING_H_
#define SRC_NAV_DODECODING_H_

#include <nav_init.h>
#include "../include/initNav2sch.h"
#include "../include/initNavChannels.h"
#include "initEph.h"
#include "customDataTypes.h"
#include "initState.h"
#include "cirShiftBuff.h"


/*
 Decodes the navigation bits.

 */

si32 navBitSych(
		/* Output */
		navChannels_s * trackResults[],
		/* Input */
		si32 PRN);

si32 doDecoding1ch(
		/* Output */
		navChannels_s * trackResults[],
		cirBuffNavBit_s * cirBuffNavBit[], eph_s * eph, nav2sch_s ** nav2sch,
		/* Input */
		si32 PRN);

#endif /* SRC_NAV_DODECODING_H_ */
