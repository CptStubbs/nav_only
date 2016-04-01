/*
 * initEph.h
 *
 *  Created on: May 11, 2015
 *      Author: user
 */

#ifndef SRC_INCLUDE_INITEPH_H_
#define SRC_INCLUDE_INITEPH_H_

#include "customDataTypes.h"

typedef struct {

	/* contents in subframe 1*/
	fl64 TOW;
	fl64 weekNumber;
	fl64 accuracy;
	fl64 health;
	fl64 T_GD;
	fl64 IODC;
	fl64 IODC8bits_sf1;
	fl64 t_oc;
	fl64 a_f2;
	fl64 a_f1;
	fl64 a_f0;
	/* contents in subframe 2*/
	fl64 IODE_sf2;
	fl64 C_rs;
	fl64 deltan;
	fl64 M_0;
	fl64 C_uc;
	fl64 e;
	fl64 C_us;
	fl64 sqrtA;
	fl64 t_oe;
	/*contents in subframe 3*/
	fl64 C_ic;
	fl64 omega_0;
	fl64 C_is;
	fl64 i_0;
	fl64 C_rc;
	fl64 omega;
	fl64 omegaDot;
	fl64 IODE_sf3;
	fl64 iDot;

	fl64 * nav;
} nav_s;

typedef struct {
	/* contents in page 18, subframe 4*/
	/* Update every 6 days*/
	fl64 dataID;
	fl64 svID;
	/* Ionospheric model parameters*/
	fl64 a0;
	fl64 a1;
	fl64 a2;
	fl64 a3;
	fl64 beta0;
	fl64 beta1;
	fl64 beta2;
	fl64 beta3;
	/* UTC parameters*/
	fl64 A0;
	fl64 A1;
	fl64 deltaTls;
	fl64 t_ot;
	fl64 WNt;
	fl64 WNlsf;
	fl64 DN;
	fl64 deltaTlsf;

} almanac_s;

typedef struct {

	/* The ephemeris is updated every 2 hours
	 * No matter how short the data is, still consider there is one updates in
	 * the data
	 * Estimate the number of ephemeris records
	 */
	/*! Stored navsettings from subframe  */
	almanac_s almanac;
	nav_s nav[32];
	nav_s navTemp[32];

	/* to check if it is fully filled ([1 1 1]) or not.*/
	si32 navStatus[32][3];
	si32 navTempStatus[32][3];
	si32 newEphFlag[32];

} eph_s;

void initEph(eph_s * eph); /* have to declare channel somewhere else.*/
void initEphNav(nav_s * nav);

#endif /* SRC_INCLUDE_INITEPH_H_ */
