/*
 * cart2utm.c
 *
 *  Created on: May 26, 2015
 *      Author: sean
 */
#include "cart2utm.h"

#include <conf_nav.h>
#include "customDataTypes.h"
#include "clsin.h"
#include "clksin.h"
#include "utils.h"
#include <math.h>

void cart2utm(fl64 * E, fl64 * N, fl64 * U, fl64 X, fl64 Y, fl64 Z, fl64 zone) {
	si32 i = 0;
	fl64 a = 6378388;
	fl64 f = 1 / 297;
	fl64 ex2 = (2 - f) * f / (pow(1 - f, 2));
	fl64 c = a * sqrt(1 + ex2);
	fl64 vec[3];
	fl64 alpha = .756e-6;
	fl64 R[3][3];
	fl64 trans[3] = { 89.5, 93.8, 127.6 };
	fl64 scale = 0.9999988;
	fl64 v[3];
	fl64 L = 0;
	fl64 N1 = 6395000;
	fl64 B = 0;
	fl64 oldU = 0;
	fl64 m0 = 0.0004;
	fl64 n = f / (2 - f);
	fl64 m = pow(n, 2) * (1 / 4 + n * n / 64);
	fl64 w = (a * (-n - m0 + m * (1 - m0))) / (1 + n);
	fl64 Q_n = a + w;
	fl64 E0 = 500000;
	fl64 L0 = (zone - 30) * 6 - 3;
	fl64 bg[4] =
			{ -3.37077907e-3, 4.73444769e-6, -8.29914570e-9, 1.58785330e-11 };
	fl64 gtu[4] =
			{ 8.41275991e-4, 7.67306686e-7, 1.21291230e-9, 2.48508228e-12 };
	pi8 neg_geo = 'F';
	fl64 Bg_r = 0;
	fl64 res_clensin = 0;
	fl64 Lg_r = 0;
	fl64 cos_BN = 0;
	fl64 Np = 0;
	fl64 Ep = 0;
	fl64 dN = 0, dE = 0;

	*U = 0.1;
	vec[0] = X;
	vec[1] = Y;
	vec[2] = Z - 4.5;
	R[0][0] = 1;
	R[0][1] = -alpha;
	R[0][2] = 0;
	R[1][0] = alpha;
	R[1][1] = 1;
	R[1][2] = 0;
	R[2][0] = 0;
	R[2][1] = 0;
	R[2][2] = 1;
	for (i = 0; i < 3; i++) {
		v[i] = scale * (R[i][0] * vec[0] + R[i][1] * vec[1] + R[i][2] * vec[2])
				+ trans[i];
	}
	L = atan2(v[1], v[0]);
	B = atan2(v[2] / (pow(1 - f, 2) * N1), norm(v, 2) / N1);
	while (fabs(*U - oldU) > 1.0e-4) {
		oldU = *U;
		N1 = c / sqrt(1 + ex2 * pow(cos(B), 2));
		B = atan2(v[2] / (pow(1 - f, 2) * N1 + *U), norm(v, 2) / (N1 + *U));
		*U = norm(v, 2) / cos(B) - N1;
	}

	if (B < 0) {
		neg_geo = 'T';
	}
	Bg_r = fabs(B);
	clsin(&res_clensin, bg, 4, 2 * Bg_r);
	Bg_r += res_clensin;
	L0 = L0 * M_PI / 180;
	Lg_r = L - L0;

	/*! Spherical latitude longitude to complementary spherical latitude
	 * i.e spherical N,E
	 */

	cos_BN = cos(Bg_r);
	Np = atan2(sin(Bg_r), cos(Lg_r) * cos_BN);
	Ep = atanh(sin(Lg_r) * cos_BN);

	/* Sphercal normalized N, E to ellipsoidal N,E */

	Np *= 2;
	Ep *= 2;
	clksin(&dN, &dE, gtu, 4, Np, Ep);
	Np = Np / 2;
	Ep = Ep / 2;
	Np += dN;
	Ep += dE;
	*N = Q_n * Np;
	*E = Q_n * Ep + E0;

	if (neg_geo == 'T') {
		*N = -*N + 20000000;
	}
}
