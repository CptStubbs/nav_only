/*
 * clksin.c
 *
 *  Created on: May 18, 2015
 *      Author: sean
 */

#include <math.h>
#include "clksin.h"
#include "customDataTypes.h"

void clksin(fl64 * re, fl64 * im, fl64 * ar, si32 degree, fl64 arg_real,
		fl64 arg_imag) {

	fl64 hr2 = 0;
	fl64 hr1 = 0;
	fl64 hr = 0;
	fl64 hi2 = 0;
	fl64 hi1 = 0;
	fl64 hi = 0;
	fl64 z = 0;
	si32 t = 0;
	fl64 sin_arg_r = sin(arg_real);
	fl64 cos_arg_r = cos(arg_real);
	fl64 sinh_arg_i = sinh(arg_imag);
	fl64 cosh_arg_i = cosh(arg_imag);

	fl64 r = 2 * cos_arg_r * cosh_arg_i;
	fl64 i = -2 * sin_arg_r * sinh_arg_i;

	for (t = degree - 1; t >= 0; t--) {
		hr2 = hr1;
		hr1 = hr;
		hi2 = hi1;
		hi1 = -hi;
		z = ar[t] + r * hr1 - i * hi - hr2;
		hi = i * hr1 + r * hi1 - hi2;
		hr = z;
	}

	r = sin_arg_r * cosh_arg_i;
	i = cos_arg_r * sinh_arg_i;

	*re = r * hr - i * hi;
	*im = r * hi + i * hr;

}
