/*
 * initEph.h
 *
 *  Created on: Oct 30, 2015
 *      Author: erin
 */

#ifndef SRC_SCH_INITEPH_H_
#define SRC_SCH_INITEPH_H_

#include "customDataTypes.h"
#include "conf_sch.h"

typedef struct {
	fl64 a_f0;
	fl64 a_f1;
	fl64 a_f2;
	ui32 IODE;
	fl64 C_rs;
	fl64 deltan;
	fl64 M_0;
	fl64 C_uc;
	fl64 e;
	fl64 C_us;
	fl64 sqrtA;
	fl32 t_oe;
	fl64 C_ic;
	fl64 omega_0;
	fl64 C_is;
	fl64 i_0;
	fl64 C_rc;
	fl64 omega;
	fl64 omegaDot;
	fl64 iDot;
	ui32 codes;
	ui32 weekno;
	ui32 L2flag;
	ui32 svaccur;
	ui32 svhealth;
	fl64 T_GD;
	ui32 IODC;
	fl32 TOW;
	ui32 fitInt;
} navGPS_s;

typedef struct {
	fl64 tau_n;
	fl64 gam;
	fl32 TOD;
	fl64 xDis;
	fl64 xVel;
	fl64 xAcc;
	ui32 svhealth;
	fl64 yDis;
	fl64 yVel;
	fl64 yAcc;
	ui32 freqNum;
	fl64 zDis;
	fl64 zVel;
	fl64 zAcc;
	ui32 AOOI;
	fl64 tau_c;
	fl32 tb;
} navGLO_s;

typedef struct {
	fl64 a_f0;
	fl64 a_f1;
	fl64 a_f2;
	ui32 IODE;
	fl64 C_rs;
	fl64 deltan;
	fl64 M_0;
	fl64 C_uc;
	fl64 e;
	fl64 C_us;
	fl64 sqrtA;
	fl32 t_oe;
	fl64 C_ic;
	fl64 omega_0;
	fl64 C_is;
	fl64 i_0;
	fl64 C_rc;
	fl64 omega;
	fl64 omegaDot;
	fl64 iDot;
	ui32 dataSource;
	ui32 weekno;
	ui32 SISA;
	ui32 svhealth;
	fl64 BGD_E5A;
	fl64 BGD_E5B;
	fl32 TOW;
} navGAL_s;

typedef struct {
	fl64 a_f0;
	fl64 a_f1;
	fl64 a_f2;
	ui32 IODE;
	fl64 C_rs;
	fl64 deltan;
	fl64 M_0;
	fl64 C_uc;
	fl64 e;
	fl64 C_us;
	fl64 sqrtA;
	fl32 t_oe;
	fl64 C_ic;
	fl64 omega_0;
	fl64 C_is;
	fl64 i_0;
	fl64 C_rc;
	fl64 omega;
	fl64 omegaDot;
	fl64 iDot;
	ui32 weekno;
	ui32 svaccur;
	fl64 satH1;
	fl64 tgd1;
	fl64 tgd2;
	fl32 TOW;
	ui32 IODC;
} navBDS_s;

typedef struct {
	fl64 a_f0;
	fl64 a_f1;
	fl64 a_f2;
	ui32 IODE;
	fl64 C_rs;
	fl64 deltan;
	fl64 M_0;
	fl64 C_uc;
	fl64 e;
	fl64 C_us;
	fl64 sqrtA;
	fl32 t_oe;
	fl64 C_ic;
	fl64 omega_0;
	fl64 C_is;
	fl64 i_0;
	fl64 C_rc;
	fl64 omega;
	fl64 omegaDot;
	fl64 iDot;
	ui32 codes;
	ui32 weekno;
	ui32 svaccur;
	ui32 svhealth;
	fl64 T_GD;
	ui32 IODC;
	fl32 TOW;
} navQZS_s;

typedef struct {
	fl64 aGf0;
	fl64 aGf1;
	fl32 TOW;
	fl64 xDis;
	fl64 xVel;
	fl64 xAcc;
	ui32 svhealth;
	fl64 yDis;
	fl64 yVel;
	fl64 yAcc;
	ui32 URA;
	fl64 zDis;
	fl64 zVel;
	fl64 zAcc;
	ui32 IODN;
} navSBA_s;

typedef struct {
	fl64 a0;
	fl64 a1;
	si32 T;
	si32 W1;
	si32 W2;
} gaut_s;

typedef struct {
	fl64 a0;
	fl64 a1;
	si32 T;
	si32 W1;
	si32 W2;
} gput_s;

typedef struct {
	fl64 a0;
	fl64 a1;
	si32 T;
	si32 W1;
	si32 W2;
} sbut_s;

typedef struct {
	fl64 tauC;
	fl64 a1;
	si32 T;
	si32 W1;
	si32 W2;
} glut_s;

typedef struct {
	fl64 A0G;
	fl64 A1G;
	si32 T;
	si32 W1;
	si32 W2;
} gpga_s;

typedef struct {
	fl64 tauGPS;
	fl64 a1;
	si32 T;
	si32 W1;
	si32 W2;
} glgp_s;

typedef struct {
	fl64 a0;
	fl64 a1;
	si32 T;
	si32 W1;
	si32 W2;
} qzgp_s;

typedef struct {
	fl64 a0;
	fl64 a1;
	si32 T;
	si32 W1;
	si32 W2;
} qzut_s;

typedef struct {
	fl64 A0_UTC;
	fl64 A1_UTC;
	si32 T;
	si32 W1;
	si32 W2;
} bdut_s;

typedef struct {
	navGPS_s navGPS[NUMSATSGPS];
	navGLO_s navGLO[NUMSATSGLO];
	navGAL_s navGAL[NUMSATSGAL];
	navBDS_s navBDS[NUMSATSBDS];
	navQZS_s navQZS[NUMSATSQZS];
	navSBA_s navSBA[NUMSATSSBA];

	gaut_s gaut;
	gput_s gput;
	sbut_s sbut;
	glut_s glut;
	gpga_s gpga;
	glgp_s glgp;
	qzgp_s qzgp;
	qzut_s qzut;
	bdut_s bdut;

	si32 leapSeconds;

} ephSch_s;

void initEph(ephSch_s * eph);
void initEphGPSNav(navGPS_s * navGPS);
void initEphGLONav(navGLO_s * navGLO);
void initEphGALNav(navGAL_s * navGAL);
void initEphBDSNav(navBDS_s * navBDS);
void initEphQZSNav(navQZS_s * navQZS);
void initEphSBANav(navSBA_s * navSBA);
void initTimeTransfer(gaut_s * gaut, gput_s * gput, sbut_s * sbut, glut_s * glut,
		gpga_s * gpga, glgp_s * glgp, qzgp_s * qzgp, qzut_s * qzut, bdut_s * bdut);

#endif /* SRC_SCH_INITEPH_H_ */
