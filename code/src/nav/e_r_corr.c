/*
 * e_r_corr.c
 *
 *  Created on: May 13, 2015
 *      Author: sean
 */
#include "e_r_corr.h"
#include "customDataTypes.h"
#include <math.h>

void e_r_corr(fl64 X_sat_rot[3], fl64 travelTime, fl64 X_sat[3]) {
	si32 i = 0, j = 0;
	fl64 OmegaeDot = 7.292115147e-5; /* rad/sec*/
	/* Rotation angle */
	fl64 omegaTau = OmegaeDot * travelTime;
	fl64 R3[3][3];
	R3[0][0] = cos(omegaTau);
	R3[0][1] = sin(omegaTau);
	R3[0][2] = 0;
	R3[1][0] = -sin(omegaTau);
	R3[1][1] = cos(omegaTau);
	R3[1][2] = 0;
	R3[2][0] = 0;
	R3[2][1] = 0;
	R3[2][2] = 1;

	for (i = 0; i < 3; i++) {
		X_sat_rot[i] = 0;
		for (j = 0; j < 3; j++) {
			X_sat_rot[i] += R3[i][j] * X_sat[j];
		}
	}
}
