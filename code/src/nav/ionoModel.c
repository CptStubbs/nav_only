/*
 * ionoModel.c
 *
 *  Created on: May 26, 2015
 *      Author: sean
 */

#include "customDataTypes.h"
#include "ionoModel.h"

#include <conf_nav.h>

#include "cart2geo.h"
#include <math.h>

void ionoModel(fl64 * iono, almanac_s almanac, fl64 az, fl64 el, fl64 TOW,
		fl64 pos[3]) {

	/*! Degrees to semi-circles */
	fl64 deg2semi = 1.0 / 180.0;
	/*! temp iono placeholder */
	fl64 tempIono = 0;
	si32 i = 0;
	si32 posZero = 0;
	fl64 phi_u = 0;
	fl64 lambda_u = 0;
	fl64 h_u = 0;
	fl64 psi = 0;
	fl64 phi_i = 0;
	fl64 lambda_i = 0;
	fl64 phi_m = 0;
	fl64 t = 0;
	fl64 FIono = 0;
	fl64 PER = 0;
	fl64 x = 0;
	fl64 AMP = 0;
	az *= deg2semi;
	el *= deg2semi;

	if (almanac.a0 == 0) {
		tempIono = 0;
	} else {
		for (i = 0; i < 3; i++) {
			if (pos[i] != 0) {
				posZero = 1;
				break;
			}
		}
		if (posZero == 0) {
			phi_u = 0;
			lambda_u = 0;
		} else {
			cart2geo(&phi_u, &lambda_u, &h_u, pos[0], pos[1], pos[2], 5);
			phi_u *= deg2semi;
			lambda_u *= deg2semi;
		}
		psi = 0.0137 / (el + 0.11) - 0.022;
		phi_i = phi_u + psi * cos(az);
		if (phi_i > 0.416) {
			phi_i = 0.416;
		}
		if (phi_i < -0.416) {
			phi_i = -0.416;
		}

		lambda_i = lambda_u + psi * sin(az) / cos(phi_i);
		phi_m = phi_i + 0.064 * cos(lambda_i - 1.617);
		t = 4.32e4 * lambda_i + TOW;
		if (t >= 86400) {
			t -= 86400;
		} else {
			if (t < 0) {
				t += 86400;
			}
		}
		FIono = 1.0 + 16.0 * pow(0.53 - el, 3);
		PER = almanac.beta0 + almanac.beta1 * phi_m
				+ almanac.beta2 * pow(phi_m, 2) + almanac.beta3 * pow(phi_m, 3);
		if (PER < 72000) {
			PER = 72000;
		}

		x = 2 * M_PI * (t - 50400) / PER;
		AMP = almanac.a0 + almanac.a1 * phi_m + almanac.a2 * pow(phi_m, 2)
				+ almanac.a3 * pow(phi_m, 3);
		if (AMP < 0) {
			AMP = 0;
		}
		if (fabs(x) < 1.57) {
			tempIono = FIono
					* (5.0e-9 + AMP * (1 - (pow(x, 2)) / 2 + pow(x, 4) / 24));
		} else {
			tempIono = FIono * (5.0e-9);
		}

	}

	*iono = tempIono;

}
