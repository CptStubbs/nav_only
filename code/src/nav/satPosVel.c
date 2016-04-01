/*
 * satPosVel.c
 *
 *  Created on: May 14, 2015
 *      Author: sara
 */
#include "satPosVel.h"

#include <conf_nav.h>
#include "initEph.h"
#include "customDataTypes.h"
#include "check_t.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void satPosVel(
/* Output */
fl64 * satPositions[3],/*This are not 1x3 but rather 12x3 or something similar*/
fl64 * satVelocity[3], fl64 * satClkCorr,
/* Input */
fl64 * transmitTime, nav_s * eph[], si32 * activeChnList,
		si32 activeChnListSize) {
	si32 iii = 0;
	si32 PRN = 0;
	si32 ii = 0;

	fl64 dt = 0;
	fl64 time = 0;
	fl64 a = 0;
	fl64 tk = 0;
	fl64 n0 = 0;
	fl64 n = 0;
	fl64 M = 0;
	fl64 M_dot = 0;
	fl64 E = 0;
	fl64 dE = 0;
	fl64 E_old = 0;
	fl64 E_dot = 0;
	fl64 dtr = 0;
	fl64 nu = 0;
	fl64 nu_dot = 0;
	fl64 phi = 0, u = 0, i = 0, r = 0, u_dot = 0, r_dot = 0, i_dot = 0, xp = 0,
			yp = 0, xp_dot = 0, yp_dot = 0, Omega = 0, Omega_Dot = 0;

	/*Process each satellite =================================================*/
	for (iii = 0; iii < activeChnListSize; iii++) {
		PRN = activeChnList[iii];/*Sara Now provides PRN number instead of channel number.*/

		/*Find initial satellite clock correction --------------------------------*/
		/*--- Find time difference ---------------------------------------------*/
		check_t(&dt, (transmitTime[PRN] - eph[PRN]->t_oc));

		/*--- Calculate clock correction ---------------------------------------*/
		satClkCorr[PRN] = (eph[PRN]->a_f2 * dt + eph[PRN]->a_f1) * dt
				+ eph[PRN]->a_f0 - eph[PRN]->T_GD;

		time = transmitTime[PRN] - satClkCorr[PRN];

		/*Find satellite's position ----------------------------------------------*/

		/*Restore semi-major axis*/
		a = eph[PRN]->sqrtA * eph[PRN]->sqrtA;

		/*Time correction*/
		check_t(&tk, (time - eph[PRN]->t_oe));

		/*Initial mean motion*/
		n0 = sqrt(GM / pow(a, 3));
		/*Mean motion*/
		n = n0 + eph[PRN]->deltan;

		/*Mean anomaly*/
		M = eph[PRN]->M_0 + n * tk;
		/*Reduce mean anomaly to between 0 and 360 deg*/
		M = fmod(M + 2 * M_PI, 2 * M_PI);

		/*New Line*/
		M_dot = n;

		/*Initial guess of eccentric anomaly*/
		E = M;

		/*--- Iteratively compute eccentric anomaly ----------------------------*/
		for (ii = 0; ii < 10; ii++) {
			E_old = E;
			E = M + eph[PRN]->e * sin(E);
			dE = fmod(E - E_old, 2 * M_PI);

			if (fabs(dE) < 1.e-12) {
				/* Necessary precision is reached, exit from the loop*/
				break;
			}/*end of if statement*/
		}/*end of for loop*/

		/*Reduce eccentric anomaly to between 0 and 360 deg*/
		E = fmod(E + 2 * M_PI, 2 * M_PI);

		/* New Line*/
		E_dot = M_dot / (1.0 - eph[PRN]->e * cos(E));

		/*Compute relativistic correction term*/
		dtr = (F * eph[PRN]->e) * (eph[PRN]->sqrtA * sin(E));

		/*Calculate the true anomaly*/
		nu = atan2(sqrt(1 - pow(eph[PRN]->e, 2)) * sin(E),
				cos(E) - eph[PRN]->e);

		/*New Line*/
		nu_dot = sin(E) * E_dot * (1.0 + eph[PRN]->e * cos(nu))
				/ (sin(nu) * (1.0 - eph[PRN]->e * cos(E)));

		/*Compute angle phi*/
		phi = nu + eph[PRN]->omega;
		/*Reduce phi to between 0 and 360 deg*/
		phi = fmod(phi, 2 * M_PI);

		/*Correct argument of latitude*/
		u = phi + eph[PRN]->C_uc * cos(2 * phi) + eph[PRN]->C_us * sin(2 * phi);
		/*Correct radius*/
		r = a * (1 - eph[PRN]->e * cos(E)) + eph[PRN]->C_rc * cos(2 * phi)
				+ eph[PRN]->C_rs * sin(2 * phi);
		/*Correct inclination*/
		i = eph[PRN]->i_0 + eph[PRN]->iDot * tk + eph[PRN]->C_ic * cos(2 * phi)
				+ eph[PRN]->C_is * sin(2 * phi);

		/*New Lines*/
		u_dot = nu_dot
				+ 2.0
						* (eph[PRN]->C_us * cos(2.0 * u)
								- eph[PRN]->C_uc * sin(2.0 * u)) * nu_dot;
		r_dot = a * eph[PRN]->e * sin(E) * n / (1.0 - eph[PRN]->e * cos(E))
				+ 2.0
						* (eph[PRN]->C_rs * cos(2.0 * u)
								- eph[PRN]->C_rc * sin(2.0 * u)) * nu_dot;
		i_dot =
				eph[PRN]->iDot
						+ (eph[PRN]->C_is * cos(2.0 * u)
								- eph[PRN]->C_ic * sin(2.0 * u)) * 2.0 * nu_dot;

		xp = r * cos(u);
		yp = r * sin(u);

		xp_dot = r_dot * cos(u) - yp * u_dot;
		yp_dot = r_dot * sin(u) + xp * u_dot;

		/*Compute the angle between the ascending node and the Greenwich meridian*/
		Omega = eph[PRN]->omega_0 + (eph[PRN]->omegaDot - OMEGAE_DOT) * tk
				- OMEGAE_DOT * eph[PRN]->t_oe;
		/*Reduce to between 0 and 360 deg*/
		Omega = fmod(Omega + 2 * M_PI, 2 * M_PI);

		/* New Line*/
		Omega_Dot = (eph[PRN]->omegaDot - OMEGAE_DOT);

		/*--- Compute satellite coordinates ------------------------------------*/
		satPositions[0][PRN] = xp * cos(Omega) - yp * cos(i) * sin(Omega);
		satPositions[1][PRN] = xp * sin(Omega) + yp * cos(i) * cos(Omega);
		satPositions[2][PRN] = yp * sin(i);

		/* New Lines-- Compute satellite veloticy------------------------------*/
		satVelocity[0][PRN] = (xp_dot - yp * cos(i) * Omega_Dot) * cos(Omega)
				- (xp * Omega_Dot + yp_dot * cos(i) - yp * sin(i) * i_dot)
						* sin(Omega);
		satVelocity[1][PRN] = (xp_dot - yp * cos(i) * Omega_Dot) * sin(Omega)
				+ (xp * Omega_Dot + yp_dot * cos(i) - yp * sin(i) * i_dot)
						* cos(Omega);
		satVelocity[2][PRN] = yp_dot * sin(i) + yp * cos(i) * i_dot;

		/* Include relativistic correction in clock correction ----------------*/
		satClkCorr[PRN] = (eph[PRN]->a_f2 * dt + eph[PRN]->a_f1) * dt
				+ eph[PRN]->a_f0 - eph[PRN]->T_GD + dtr;

	}/*end of for loop*/
}/*end of the function*/
