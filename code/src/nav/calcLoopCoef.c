/*
 * calcLoopCoef.c
 *
 *  Created on: May 13, 2015
 *      Author: sara
 */

#include <tgmath.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "calcLoopCoef.h"

#include <conf_nav.h>
#include "customDataTypes.h"

void calcLoopCoef(
/* Output */
fl64 * tau1, fl64 * tau2,
/* Input */
fl64 LBW, fl64 zeta, fl64 k) {

	fl64 Wn;

	/* Solve natural frequency */
	Wn = LBW * 8 * zeta / (4 * pow(zeta, 2) + 1);

	/* solve for t1 and t2 */
	*tau1 = k / (Wn * Wn);
	*tau2 = (2.0 * zeta) / Wn;

}

