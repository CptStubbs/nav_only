/*
 * leastSquarePos.c
 *
 *  Created on: May 27, 2015
 *      Author: sean
 */

#include "customDataTypes.h"
#include "leastSquarePos.h"

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

si32 leastSquarePos(fl64 pos[4], fl64 * el, fl64 * az, fl64 * iono, fl64 * trop,
		fl64 dop[5], fl64 ** satpos, si32 nmbOfSatellites, fl64 * obs,
		almanac_s almanac, fl64 rxTime,	si32 * activeChnList) {
	si32 i = 0;

	fl64 dtr = M_PI / 180;
	fl64 ** X;
	fl64 ** x;
	fl64 ** A;
	fl64 ** At;
	fl64 ** AtxA;
	fl64 ** AtxAinv;
	fl64 ** AtxAinvxAt;
	fl64 ** Q;
	fl64 ** omc;
	fl64 delta = 1;
	si32 iter = 1;
	si32 num = 0;
	fl64 tolerance = 0.001; //1.0e-8;
	fl64 rho2 = 0;
	fl64 travelTime = 0;
	fl64 Rot_X[3];
	fl64 tempX[3];
	fl64 tempPos[3];
	fl64 tempRot_X[3];
	fl64 xtemp[4]; /* Inverting this one because x X are both variables */
	fl64 D = 0;

	for (i = 0; i < 4; i++) {
		if (isnan(pos[i])) {
			memset(pos, 0, sizeof(fl64) * 4);
			break;
		}
	}

	/* All the mallocs needed */
	A = (fl64 **) malloc(sizeof(fl64*) * nmbOfSatellites);
	ERRORCHECK(A==NULL)
	omc = (fl64 **) malloc(sizeof(fl64*) * nmbOfSatellites);
	ERRORCHECK(omc==NULL)
	for (i = 0; i < nmbOfSatellites; i++) {
		A[i] = (fl64 *) malloc(sizeof(fl64) * 4);
		ERRORCHECK(A[i]==NULL)
		omc[i] = (fl64 *) malloc(sizeof(fl64) * 1);
		ERRORCHECK(omc[i]==NULL)
	}
	At = (fl64 **) malloc(sizeof(fl64*) * 4);
	ERRORCHECK(At==NULL)
	AtxA = (fl64 **) malloc(sizeof(fl64*) * 4);
	ERRORCHECK(AtxA==NULL)
	AtxAinv = (fl64 **) malloc(sizeof(fl64*) * 4);
	ERRORCHECK(AtxAinv==NULL)
	AtxAinvxAt = (fl64 **) malloc(sizeof(fl64*) * 4);
	ERRORCHECK(AtxAinvxAt==NULL)
	x = (fl64 **) malloc(sizeof(fl64*) * 4);
	ERRORCHECK(x==NULL)
	Q = (fl64 **) malloc(sizeof(fl64*) * 4);
	ERRORCHECK(Q==NULL)

	for (i = 0; i < 4; i++) {
		At[i] = (fl64 *) malloc(sizeof(fl64) * nmbOfSatellites);
		ERRORCHECK(At[i]==NULL)
		AtxA[i] = (fl64 *) malloc(sizeof(fl64) * 4);
		ERRORCHECK(AtxA[i]==NULL)
		AtxAinv[i] = (fl64 *) malloc(sizeof(fl64) * 4);
		ERRORCHECK(AtxAinv[i]==NULL)
		AtxAinvxAt[i] = (fl64 *) malloc(sizeof(fl64) * nmbOfSatellites);
		ERRORCHECK(AtxAinvxAt[i]==NULL)
		x[i] = (fl64 *) malloc(sizeof(fl64) * 1);
		ERRORCHECK(x[i]==NULL)
		Q[i] = (fl64 *) malloc(sizeof(fl64) * 4);
		ERRORCHECK(Q[i]==NULL)

	}
	X = (fl64 **) malloc(sizeof(fl64*) * 3);
	ERRORCHECK(X==NULL)
	for (i = 0; i < 3; i++) {
		X[i] = (fl64 *) malloc(sizeof(fl64) * NUMSATS);
		ERRORCHECK(X[i] ==NULL)
		memcpy(X[i], satpos[i], sizeof(fl64) * NUMSATS);
	}

	/* Iteratively find receiver position */
	while (delta > tolerance) {
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

			//
			if (topocent(&az[num], &el[num], &D, tempPos, tempRot_X) == EXIT_FAILURE) {
				return EXIT_FAILURE;
			}
			//Tropo error
			if (TROPCORR == 1) {
				tropo(&trop[num], sin(el[num] * dtr), 0.0, 1013.0, 293.0, 50.0,	0.0, 0.0, 0.0);
			} else {
				trop[num] = 0;
			}
			//Iono error
			if (IONOCORR == 1) {
				ionoModel(&iono[num], almanac, az[num], el[num], rxTime, pos);
				iono[num] *= SPEEDOFLIGHT;
			} else {
				iono[num] = 0;
			}
			omc[i][0] = obs[num] - norm(tempRot_X, 3) - pos[3] - trop[num] - iono[num];
			A[i][0] = (-(tempRot_X[0])) / sqrt(rho2);
			A[i][1] = -(tempRot_X[1]) / sqrt(rho2);
			A[i][2] = -(tempRot_X[2]) / sqrt(rho2);
			A[i][3] = 1;
			At[0][i] = A[i][0];
			At[1][i] = A[i][1];
			At[2][i] = A[i][2];
			At[3][i] = A[i][3];

		}

		/* We are calculating x using the atxa inverse * At * omc */
		if (matMult(AtxA, At, 4, nmbOfSatellites, A, nmbOfSatellites,
				4) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}
		if (matInverse(AtxAinv, AtxA, 4) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}
		if (matMult(AtxAinvxAt, AtxAinv, 4, 4, At, 4,
				nmbOfSatellites)==EXIT_FAILURE) {
			return EXIT_FAILURE;
		}
		if (matMult(x, AtxAinvxAt, 4, nmbOfSatellites, omc, nmbOfSatellites,
				1) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}
		for (i = 0; i < 4; i++) {
			pos[i] += x[i][0];
			xtemp[i] = x[i][0];
		}
		delta = norm(xtemp, 4);
		if (iter > 100) {
			fprintf(stderr,
					"The least square failed to reach the determined precision. \n");
			return EXIT_FAILURE;
		}
		iter++;

	}

	/* Calculate dilution of Precision */
	if (matMult(AtxA, At, 4, nmbOfSatellites, A, nmbOfSatellites,
			4) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	if (matInverse(Q, AtxA, 4) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	dop[0] = sqrt(Q[0][0] + Q[1][1] + Q[2][2] + Q[3][3]);
	dop[1] = sqrt(Q[0][0] + Q[1][1] + Q[2][2]);
	dop[2] = sqrt(Q[0][0] + Q[1][1]);
	dop[3] = sqrt(Q[2][2]);
	dop[4] = sqrt(Q[3][3]);

	for (i = 0; i < 3; i++) {
		free(X[i]);
	}

	for (i = 0; i < 4; i++) {
		free(At[i]);
		free(AtxA[i]);
		free(AtxAinv[i]);
		free(AtxAinvxAt[i]);
		free(x[i]);
		free(Q[i]);
	}
	for (i = 0; i < nmbOfSatellites; i++) {
		free(A[i]);
		free(omc[i]);
	}
	free(A);
	free(omc);
	free(At);
	free(AtxA);
	free(AtxAinv);
	free(AtxAinvxAt);
	free(Q);
	free(X);
	free(x);
	return EXIT_SUCCESS;

}

