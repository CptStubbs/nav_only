/*
 * initEph.c
 *
 *  Created on: May 28, 2015
 *      Author: daehee
 */

#include "customDataTypes.h"
#include "initEph.h"

void initEph(eph_s * eph) {
	/*
	 % Initialize the ephemeris data structure
	 %
	 %  eph = initEph(settings)
	 %
	 %   Inputs:
	 %
	 %       settings          - receiver settings.
	 %   Outputs:
	 %       eph.nav           - navigation message
	 %       eph.almanac       - ionospheric and UTC parameters


	 % The ephemeris is updated every 2 hours
	 % No matter how short the data is, still consider there is one updates in
	 % the data
	 % Estimate the number of ephemeris records
	 */

	si32 prn = 0;

	/* contents in page 18, subframe 4, Update every 6 days */
	eph->almanac.dataID = 0;
	eph->almanac.svID = 0;

	/* Ionospheric model parameters */
	eph->almanac.a0 = 0;
	eph->almanac.a1 = 0;
	eph->almanac.a2 = 0;
	eph->almanac.a3 = 0;
	eph->almanac.beta0 = 0;
	eph->almanac.beta1 = 0;
	eph->almanac.beta2 = 0;
	eph->almanac.beta3 = 0;

	/* UTC parameters */
	eph->almanac.A0 = 0;
	eph->almanac.A1 = 0;
	eph->almanac.deltaTls = 0;
	eph->almanac.t_ot = 0;
	eph->almanac.WNt = 0;
	eph->almanac.WNlsf = 0;
	eph->almanac.DN = 0;
	eph->almanac.deltaTlsf = 0;

	for (prn = 0; prn < 32; prn++) {
		initEphNav(&eph->nav[prn]);
		/* "eph.navTemp" is to cope with the change of ephemeris. need to save the
		 new ephemeris data into "eph.navTemp". once it is fully filled with new
		 data, then update the "eph.nav"*/
		initEphNav(&eph->navTemp[prn]);

		eph->navStatus[prn][0] = 0;
		eph->navStatus[prn][1] = 0;
		eph->navStatus[prn][2] = 0;

		eph->navTempStatus[prn][0] = 0;
		eph->navTempStatus[prn][1] = 0;
		eph->navTempStatus[prn][2] = 0;

		eph->newEphFlag[prn] = 0;
	}
}

void initEphNav(nav_s * nav) {
	/* contents in subframe 1 */
	nav->TOW = 0;
	nav->weekNumber = 0;
	nav->accuracy = 0;
	nav->health = 0;
	nav->T_GD = 0;
	nav->IODC = 0;
	nav->IODC8bits_sf1 = 0;
	nav->t_oc = 0;
	nav->a_f2 = 0;
	nav->a_f1 = 0;
	nav->a_f0 = 0;

	/* contents in subframe 2 */
	nav->IODE_sf2 = 0;
	nav->C_rs = 0;
	nav->deltan = 0;
	nav->M_0 = 0;
	nav->C_uc = 0;
	nav->e = 0;
	nav->C_us = 0;
	nav->sqrtA = 0;
	nav->t_oe = 0;

	/* contents in subframe 3 */
	nav->C_ic = 0;
	nav->omega_0 = 0;
	nav->C_is = 0;
	nav->i_0 = 0;
	nav->C_rc = 0;
	nav->omega = 0;
	nav->omegaDot = 0;
	nav->IODE_sf3 = 0;
	nav->iDot = 0;

	/* FOR TEMP NAV */
	/* contents in subframe 1 */
	nav->TOW = 0;
	nav->weekNumber = 0;
	nav->accuracy = 0;
	nav->health = 0;
	nav->T_GD = 0;
	nav->IODC = 0;
	nav->IODC8bits_sf1 = 0;
	nav->t_oc = 0;
	nav->a_f2 = 0;
	nav->a_f1 = 0;
	nav->a_f0 = 0;

	/* contents in subframe 2 */
	nav->IODE_sf2 = 0;
	nav->C_rs = 0;
	nav->deltan = 0;
	nav->M_0 = 0;
	nav->C_uc = 0;
	nav->e = 0;
	nav->C_us = 0;
	nav->sqrtA = 0;
	nav->t_oe = 0;

	/* contents in subframe 3 */
	nav->C_ic = 0;
	nav->omega_0 = 0;
	nav->C_is = 0;
	nav->i_0 = 0;
	nav->C_rc = 0;
	nav->omega = 0;
	nav->omegaDot = 0;
	nav->IODE_sf3 = 0;
	nav->iDot = 0;
}
