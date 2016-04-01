/*
 * calcLoopCoef.c
 *
 *  Created on: Aug 11, 2015
 *      Author: user
 */
#include "customDataTypes.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <memory.h>

void calcLoopCoef(fl64 LBW, fl64 zeta, fl64 k, fl64* coef1, fl64* coef2) {
	fl64 Wn;
	Wn = LBW * 8 * zeta / (4 * pow(zeta, (si32) 2) + 1);
	*coef1 = k / (Wn * Wn);
	*coef2 = 2 * zeta / Wn;
}


