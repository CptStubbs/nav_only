/*
 * doNavigation.h
 *
 *  Created on: May 12, 2015
 *      Author: sean
 */

#ifndef NAV_DONAVIGATION_H_
#define NAV_DONAVIGATION_H_

#include <stdio.h>

#include "../include/initNav2sch.h"
#include "initNavChannels.h"
#include "initEph.h"
#include "customDataTypes.h"
#include "initNavSolutions.h"
#include "initState.h"


/*!
 * \brief Top level function to run all the navigation code
 * \return EXIT_SUCCESS on success. EXIT_FAILURE otherwise
 * \param navSolutions - pointer to shared navSolutions struct
 * \param state - point to current state struct
 * \param eph - pointer to eph data struct
 * \param trackResults - results from the tracking function
 * \param nav2sch - subframe information to provide for scheduler
 * \param rnxFid - FID for the LSS
 */
si32 doNavigation(
/* Output*/
navSolutions_s * navSolutions, state_s * state,
		eph_s * eph,
		/* Input */
		navChannels_s *trackResults[],
		nav2sch_s ** nav2sch, FILE *rnxFid);

#endif /* NAV_DONAVIGATION_H_ */
