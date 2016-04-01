/*
 * topocent.c
 *
 *  Created on: May 18, 2015
 *      Author: sean
 */

#include <conf_nav.h>
#include "customDataTypes.h"
#include "togeod.h"
#include <stdlib.h>
#include <math.h>

si32 topocent(fl64 * Az, fl64 * El, fl64 * D, fl64 X[3], fl64 dx[3]) {
	fl64 dtr = M_PI / 180;
	fl64 phi = 0;
	fl64 lambda = 0;
	fl64 h = 0;
	fl64 f[3][3];
	fl64 local_vector[3];
	fl64 E = 0;
	fl64 N = 0;
	fl64 U = 0;
	fl64 hor_dis = 0;
	fl64 cl = 0;
	fl64 sl = 0;
	fl64 cb = 0;
	fl64 sb = 0;
	if (togeod(&phi, &lambda, &h, 6378137, 298.257223563, X[0], X[1],
			X[2]) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	cl = cos(lambda * dtr);
	sl = sin(lambda * dtr);
	cb = cos(phi * dtr);
	sb = sin(phi * dtr);

	/*! Automatically transpose f */
	f[0][0] = -sl;
	f[1][0] = -sb * cl;
	f[2][0] = cb * cl;
	f[0][1] = cl;
	f[1][1] = -sb * sl;
	f[2][1] = cb * sl;
	f[0][2] = 0;
	f[1][2] = cb;
	f[2][2] = sb;

	local_vector[0] = f[0][0] * dx[0] + f[0][1] * dx[1] + f[0][2] * dx[2];
	local_vector[1] = f[1][0] * dx[0] + f[1][1] * dx[1] + f[1][2] * dx[2];
	local_vector[2] = f[2][0] * dx[0] + f[2][1] * dx[1] + f[2][2] * dx[2];

	E = local_vector[0];
	N = local_vector[1];
	U = local_vector[2];

	hor_dis = sqrt(pow(E, 2) + pow(N, 2));

	if (hor_dis < 1.0e-20) {
		*Az = 0;
		*El = 90;
	} else {
		*Az = atan2(E, N) / dtr;

		*El = atan2(U, hor_dis) / dtr;
	}

	if (*Az < 0) {
		*Az += 360;
	}

	*D = sqrt(pow(dx[0], 2) + pow(dx[1], 2) + pow(dx[2], 2));

	return EXIT_SUCCESS;
}
