/*
 * togeod.c
 *
 *  Created on: May 7, 2015
 *      Author: sean
 */

#include "customDataTypes.h"
#include "togeod.h"

#include <conf_nav.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

si32 togeod(fl64 * dphi, fl64 * dlambda, fl64 * h, fl64 a, fl64 finv, fl64 X,
		fl64 Y, fl64 Z) {
	/*Variable declaration*/
	si32 maxit = 10;
	si32 i = 0;
	/*! Radians-to-degree factor */
	fl64 rtd = 180 / M_PI;
	fl64 tolsq = 1.0e-10;
	fl64 esq = 0.0;
	fl64 oneesq = 0.0;
	/*! Distance from spin axis*/
	fl64 P = 0.0;
	/*! distance from origin (0,0,0) */
	fl64 r = 0.0;
	fl64 sinphi = 0.0;
	fl64 cosphi = 0.0;
	fl64 N_phi = 0.0;
	fl64 dP = 0.0;
	fl64 dZ = 0.0;
	/* Local copies of the outputs */
	fl64 localDphi = 0.0;
	fl64 localDlambda = 0.0;
	fl64 localH = 0.0;
	/* Processing code*/
	/* compute square of eccentricity */
	if (finv < 1.0e-20) {
		esq = 0;
	} else {
		esq = (2 - 1 / finv) / finv;
	}

	oneesq = 1 - esq;
	/* first guess */
	P = sqrt(pow(X, 2) + pow(Y, 2));
	/* direct calcuation of longitude */
	if (P > 1.0e-20) {
		localDlambda = atan2(Y, X) * rtd;
	} else {
		localDlambda = 0;
	}

	if (localDlambda < 0) {
		localDlambda = localDlambda + 360;
	}

	r = sqrt(pow(P, 2) + pow(Z, 2));

	if (r > 1.0e-20) {
		sinphi = Z / r;
	} else {
		sinphi = 0;
	}

	localDphi = asin(sinphi);
	/*! Initial value of height = distance from origin minus
	 * approximate distance from origin to surface of ellipsoid
	 */
	if (r < 1.0e-20) {
		*h = 0;
		*dphi = localDphi;
		*dlambda = localDlambda;
		return EXIT_SUCCESS;
	}
	localH = r - a * (1 - sinphi * sinphi / finv);

	for (i = 0; i < maxit; i++) {
		sinphi = sin(localDphi);
		cosphi = cos(localDphi);
		/* compute radius of curvature in prime vertical direction */
		N_phi = a / sqrt(1 - esq * sinphi * sinphi);
		/* compute residuals in P and Z */
		dP = P - (N_phi + localH) * cosphi;
		dZ = Z - (N_phi * oneesq + localH) * sinphi;
		localH = localH + (sinphi * dZ + cosphi * dP);
		localDphi = localDphi + (cosphi * dZ - sinphi * dP) / (N_phi + localH);
		if (dP * dP + dZ * dZ < tolsq) {
			break;
		}
		if (i == maxit - 1) {
			fprintf(stderr,
					"Problem in TOGEOD did not converge in %d iterations\n", i);
			return EXIT_FAILURE;
		}
	}

	localDphi = localDphi * rtd;
	/*output the correct variables at the end */
	*dphi = localDphi;
	*dlambda = localDlambda;
	*h = localH;

	return EXIT_SUCCESS;
}
