/*
 * clsin.c
 *
 *  Created on: May 18, 2015
 *      Author: sean
 */

#include "clsin.h"
#include "customDataTypes.h"
#include <math.h>

void clsin(fl64 * result, fl64 * ar, si32 degree, fl64 argument) {

	fl64 cos_arg = 2 * cos(argument);
	fl64 hr2 = 0;
	fl64 hr1 = 0;
	fl64 hr = 0;
	si32 t = 0;

	for (t = degree - 1; t >= 0; t--) {
		hr2 = hr1;
		hr1 = hr;
		hr = ar[t] + cos_arg * hr1 - hr2;
	}
	*result = hr * sin(argument);

}
