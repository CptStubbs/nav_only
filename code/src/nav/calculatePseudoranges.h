/*
 * calculatePseudoranges.h
 *
 *  Created on: May 12, 2015
 *      Author: sara
 */

#ifndef SRC_NAV_CALCULATEPSEUDORANGES_H_
#define SRC_NAV_CALCULATEPSEUDORANGES_H_
#include "customDataTypes.h"
/* \brief calculatePseudoranges finds relative pseudoranges for all satellites
 listed in CHANNELLIST at the specified millisecond of the processed
 signal. The pseudoranges contain unknown receiver clock offset. It can be
 found by the least squares position search procedure.

 Inputs:
 \params transmitTime    - transmitting time all satellites on the list
 \params rxTime          - receiver time
 \params channelList     - list of channels to be processed
 \params settings        - receiver settings

 Outputs:
 \params pseudoranges    - relative pseudoranges to the satellites.(NUST BE FREED ELSEWHERE!!)*/

si32 calculatePseudoranges(
/* Output */
fl64 * pseudoranges,
/* Input */
fl64 * transmitTime, fl64 rxTime, si32 * channelList, si32 channelListSize);

#endif /* SRC_NAV_CALCULATEPSEUDORANGES_H_ */
