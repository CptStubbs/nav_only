/*
 * calcLoopCoef.h
 *
 *  Created on: May 13, 2015
 *      Author: sara
 */

#ifndef SRC_NAV_CALCLOOPCOEF_H_
#define SRC_NAV_CALCLOOPCOEF_H_
#include "customDataTypes.h"
/*! \brief Function finds loop coefficients. The coefficients are used then in PLL-s
 and DLL-s.

 [tau1, tau2] = calcLoopCoef(LBW, zeta, k)

 Inputs:
 LBW           - Loop noise bandwidth
 zeta          - Damping ratio
 k             - Loop gain

 Outputs:
 tau1, tau2   - Loop filter coefficients*/

void calcLoopCoef(
/* Output */
fl64 * tau1, fl64 * tau2,
/* Input */
fl64 LBW, fl64 zeta, fl64 k);
#endif /* SRC_NAV_CALCLOOPCOEF_H_ */
