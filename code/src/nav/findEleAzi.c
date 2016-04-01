/*
 * leastSquarePos.c
 *
 *  Created on: May 27, 2015
 *      Author: sean
 */

#include "customDataTypes.h"
#include "findEleAzi.h"

#include <conf_nav.h>
#include "initEph.h"
#include "e_r_corr.h"
#include "topocent.h"
#include "tropo.h"
#include "utils.h"
#include "ionoModel.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

si32 findEleAzi(fl64 pos[4], fl64 * el, fl64 * az, fl64 ** satpos, si32 nmbOfSatellites, si32 * activeChnList) {

	si32 i = 0;
	fl64 ** X;
	si32 num = 0;
	fl64 rho2 = 0;
	fl64 travelTime = 0;
	fl64 Rot_X[3];
	fl64 tempX[3];
	fl64 tempPos[3];
	fl64 tempRot_X[3];
	fl64 D = 0;

	for (i = 0; i < 4; i++) {
		if (isnan(pos[i])) {
			memset(pos, 0, sizeof(fl64) * 4);
			break;
		}
	}

	X = (fl64 **) malloc(sizeof(fl64*) * 3);
	ERRORCHECK(X==NULL)
	for (i = 0; i < 3; i++) {
		X[i] = (fl64 *) malloc(sizeof(fl64) * NUMSATS);
		ERRORCHECK(X[i] ==NULL)
		memcpy(X[i], satpos[i], sizeof(fl64) * NUMSATS);
	}

	/* Iteratively find receiver position */
	for (i = 0; i < nmbOfSatellites; i++) {
		num = activeChnList[i];

		/* Update Equations */
		rho2 = pow(X[0][num] - pos[0], 2) + pow(X[1][num] - pos[1], 2) + pow(X[2][num] - pos[2], 2);
		travelTime = sqrt(rho2) / SPEEDOFLIGHT;

		/* Correct satellite position (do to earth rotation )*/
		tempX[0] = X[0][num];
		tempX[1] = X[1][num];
		tempX[2] = X[2][num];
		e_r_corr(Rot_X, travelTime, tempX);

		/* Find the elevation angle of the satellite */
		tempPos[0] = pos[0];
		tempPos[1] = pos[1];
		tempPos[2] = pos[2];
		tempRot_X[0] = Rot_X[0] - pos[0];
		tempRot_X[1] = Rot_X[1] - pos[1];
		tempRot_X[2] = Rot_X[2] - pos[2];

		if (topocent(&az[num], &el[num], &D, tempPos, tempRot_X) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}
	}

	for (i = 0; i < 3; i++) {
		free(X[i]);
	}
	free(X);

	return EXIT_SUCCESS;

}

