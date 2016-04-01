/*
 * corrCarrPhase.h
 *
 *  Created on: May 12, 2015
 *      Author: sean
 */

#ifndef NAV_CORRCARRPHASE_H_
#define NAV_CORRCARRPHASE_H_

#include "customDataTypes.h"
#include "initNavSolutions.h"

/*!
 * \brief Function adjust and correct carrier phase measurements for each satellite
 * \param navSolutions - contains measured pseudoranges, receiver
 * clock error, receiver coordinates in several
 * coordinate systems (at least ECEF and UCM)
 * \param carrPhase - carrier phase measurements
 * \param numberOfChannels 	- number of channels
 */

void corrCarrPhase(
/* Output*/
navSolutions_s * navSolutions,
/* Input */
fl64 ** carrPhase, si32 numberOfChannels);

#endif /* NAV_CORRCARRPHASE_H_ */
