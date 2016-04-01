/*
 * findDopCarrPhase.h
 *
 *  Created on: May 12, 2015
 *      Author: sara
 */

#ifndef SRC_NAV_FINDDOPCARRPHASE_H_
#define SRC_NAV_FINDDOPCARRPHASE_H_
#include "../include/initNavChannels.h"
#include "customDataTypes.h"
#include "initState.h"
/* \brief Interpolate doppler and carrier phase measurements

 Inputs:
 \params sampleNum         - sample number at the time
 \params readyChnList      - available channel list
 \params navChannels      - Data structure from Tracking

 Outputs:
 \params doppler           - doppler frequency at the sample number
 \params carrPhase         - carrier phase measurement at the sample number*/
si32 findDopCarrPhase(
/* Output */
fl64 * doppler, fl64 * carrPhase,
/* Input */
navChannels_s * navChannels[], fl64 sampleNum, si32 * readyChnList, si32 readyChnListSize);

#endif /* SRC_NAV_FINDDOPCARRPHASE_H_ */
