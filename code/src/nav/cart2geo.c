/*
 * cart2geo.c
 *
 *  Created on: May 18, 2015
 *      Author: sean
 */

#include "customDataTypes.h"
#include "cart2geo.h"

#include <conf_nav.h>
#include <math.h>
#include <stdio.h>

void cart2geo(fl64 * phi, fl64 * lambda, fl64 * h, fl64 X, fl64 Y, fl64 Z,
		si32 i) {

	fl64 a[5] = { 6378388, 6378160, 6378135, 6378137, 6378137 };
	fl64 f[5] = { 1.0 / 297, 1.0 / 298.247, 1.0 / 298.26, 1.0 / 298.257222101,
			1.0 / 298.257223563 };
	fl64 localPhi = 0;
	fl64 localLambda = 0;
	fl64 localH = 0;
	fl64 ex2 = 0;
	fl64 c = 0;
	fl64 oldh = 0;
	si32 iterations = 0;
	fl64 N = 0;
	localLambda = atan2(Y, X);
	ex2 = (2 - f[i]) * f[i] / pow((1 - f[i]), 2);
	c = a[i] * sqrt(1 + ex2);
	localPhi = atan(
			Z / ((sqrt(pow(X, 2) + pow(Y, 2)) * (1 - (2 - f[i])) * f[i])));

	localH = 0.1;
	while (fabs(localH - oldh) > 1.0e-12) {
		oldh = localH;
		N = c / sqrt(1 + ex2 * pow(cos(localPhi), 2));
		localPhi = atan(
				Z
						/ ((sqrt(pow(X, 2) + pow(Y, 2))
								* (1 - (2 - f[i]) * f[i] * N / (N + localH)))));
		localH = sqrt(pow(X, 2) + pow(Y, 2)) / cos(localPhi) - N;
		iterations++;
		if (iterations > 100) {
			fprintf(stderr,
					"Failed to approximate h with desired precision h-oldh: %f.\n",
					localH - oldh);
			break;
		}
	}
	localPhi = localPhi * 180 / M_PI;
	localLambda = localLambda * 180 / M_PI;

	*phi = localPhi;
	*lambda = localLambda;
	*h = localH;
}
