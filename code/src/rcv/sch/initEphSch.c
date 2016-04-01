/*
 * initEph.c
 *
 *  Created on: Nov 3, 2015
 *      Author: erin
 */

#include "initEphSch.h"

#include "customDataTypes.h"
#include "conf_sch.h"

void initEph(ephSch_s * eph){
	si32 prn = 0;

	for (prn = 0; prn <= NUMSATSGPS; prn++){
		initEphGPSNav(&eph->navGPS[prn]);
	}

	for (prn = 0; prn <= NUMSATSGLO; prn++){
		initEphGLONav(&eph->navGLO[prn]);
	}

	for (prn = 0; prn <= NUMSATSGAL; prn++){
		initEphGALNav(&eph->navGAL[prn]);
	}

	for (prn = 0; prn <= NUMSATSBDS; prn++){
		initEphBDSNav(&eph->navBDS[prn]);
	}

	for (prn = 0; prn <= NUMSATSQZS; prn++){
		initEphQZSNav(&eph->navQZS[prn]);
	}

	for (prn = 0; prn <= NUMSATSSBA; prn++){
		initEphSBANav(&eph->navSBA[prn]);
	}

	initTimeTransfer(&eph->gaut,&eph->gput,&eph->sbut,&eph->glut,
			&eph->gpga,&eph->glgp,&eph->qzgp,&eph->qzut,&eph->bdut);

	eph->leapSeconds = 0;
}

void initEphGPSNav(navGPS_s * navGPS){
	navGPS->a_f0 = 0;
	navGPS->a_f1 = 0;
	navGPS->a_f2 = 0;
	navGPS->IODE = 0;
	navGPS->C_rs = 0;
	navGPS->deltan = 0;
	navGPS->M_0 = 0;
	navGPS->C_uc = 0;
	navGPS->e = 0;
	navGPS->C_us = 0;
	navGPS->sqrtA = 0;
	navGPS->t_oe = 0;
	navGPS->C_ic = 0;
	navGPS->omega_0 = 0;
	navGPS->C_is = 0;
	navGPS->i_0 = 0;
	navGPS->C_rc = 0;
	navGPS->omega = 0;
	navGPS->omegaDot = 0;
	navGPS->iDot = 0;
	navGPS->codes = 0;
	navGPS->weekno = 0;
	navGPS->L2flag = 0;
	navGPS->svaccur = 0;
	navGPS->svhealth = 0;
	navGPS->T_GD = 0;
	navGPS->IODC = 0;
	navGPS->TOW = 0;
	navGPS->fitInt = 0;
}

void initEphGLONav(navGLO_s * navGLO){
	navGLO->tau_n = 0;
	navGLO->gam = 0;
	navGLO->TOD = 0;
	navGLO->xDis = 0;
	navGLO->xVel = 0;
	navGLO->xAcc = 0;
	navGLO->svhealth = 0;
	navGLO->yDis = 0;
	navGLO->yVel = 0;
	navGLO->yAcc = 0;
	navGLO->freqNum = 0;
	navGLO->zDis = 0;
	navGLO->zVel = 0;
	navGLO->zAcc = 0;
	navGLO->AOOI = 0;
	navGLO->tau_c = 0;
	navGLO->tb = 0;
}

void initEphGALNav(navGAL_s * navGAL){
	navGAL->a_f0 = 0;
	navGAL->a_f1 = 0;
	navGAL->a_f2 = 0;
	navGAL->IODE = 0;
	navGAL->C_rs = 0;
	navGAL->deltan = 0;
	navGAL->M_0 = 0;
	navGAL->C_uc = 0;
	navGAL->e = 0;
	navGAL->C_us = 0;
	navGAL->sqrtA = 0;
	navGAL->t_oe = 0;
	navGAL->C_ic = 0;
	navGAL->omega_0 = 0;
	navGAL->C_is = 0;
	navGAL->i_0 = 0;
	navGAL->C_rc = 0;
	navGAL->omega = 0;
	navGAL->omegaDot = 0;
	navGAL->iDot = 0;
	navGAL->dataSource = 0;
	navGAL->weekno = 0;
	navGAL->SISA = 0;
	navGAL->svhealth = 0;
	navGAL->BGD_E5A = 0;
	navGAL->BGD_E5B = 0;
	navGAL->TOW = 0;
}

void initEphBDSNav(navBDS_s * navBDS){
	navBDS->a_f0 = 0;
	navBDS->a_f1 = 0;
	navBDS->a_f2 = 0;
	navBDS->IODE = 0;
	navBDS->C_rs = 0;
	navBDS->deltan = 0;
	navBDS->M_0 = 0;
	navBDS->C_uc = 0;
	navBDS->e = 0;
	navBDS->C_us = 0;
	navBDS->sqrtA = 0;
	navBDS->t_oe = 0;
	navBDS->C_ic = 0;
	navBDS->omega_0 = 0;
	navBDS->C_is = 0;
	navBDS->i_0 = 0;
	navBDS->C_rc = 0;
	navBDS->omega = 0;
	navBDS->omegaDot = 0;
	navBDS->iDot = 0;
	navBDS->weekno = 0;
	navBDS->svaccur = 0;
	navBDS->satH1 = 0;
	navBDS->tgd1 = 0;
	navBDS->tgd2 = 0;
	navBDS->TOW = 0;
	navBDS->IODC = 0;
}

void initEphQZSNav(navQZS_s * navQZS){
	navQZS->a_f0 = 0;
	navQZS->a_f1 = 0;
	navQZS->a_f2 = 0;
	navQZS->IODE = 0;
	navQZS->C_rs = 0;
	navQZS->deltan = 0;
	navQZS->M_0 = 0;
	navQZS->C_uc = 0;
	navQZS->e = 0;
	navQZS->C_us = 0;
	navQZS->sqrtA = 0;
	navQZS->t_oe = 0;
	navQZS->C_ic = 0;
	navQZS->omega_0 = 0;
	navQZS->C_is = 0;
	navQZS->i_0 = 0;
	navQZS->C_rc = 0;
	navQZS->omega = 0;
	navQZS->omegaDot = 0;
	navQZS->iDot = 0;
	navQZS->codes = 0;
	navQZS->weekno = 0;
	navQZS->svaccur = 0;
	navQZS->svhealth = 0;
	navQZS->T_GD = 0;
	navQZS->IODC = 0;
	navQZS->TOW = 0;
}

void initEphSBANav(navSBA_s * navSBA){
	navSBA->aGf0 = 0;
	navSBA->aGf1 = 0;
	navSBA->TOW = 0;
	navSBA->xDis = 0;
	navSBA->xVel = 0;
	navSBA->xAcc = 0;
	navSBA->svhealth = 0;
	navSBA->yDis = 0;
	navSBA->yVel = 0;
	navSBA->yAcc = 0;
	navSBA->URA = 0;
	navSBA->zDis = 0;
	navSBA->zVel = 0;
	navSBA->zAcc = 0;
	navSBA->IODN = 0;
}

void initTimeTransfer(gaut_s * gaut, gput_s * gput, sbut_s * sbut, glut_s * glut,
		gpga_s * gpga, glgp_s * glgp, qzgp_s * qzgp, qzut_s * qzut, bdut_s * bdut) {
	gaut->a0 = 0;
	gaut->a1 = 0;
	gaut->T = 0;
	gaut->W1 = 0;
	gaut->W2 = 0;

	gput->a0 = 0;
	gput->a1 = 0;
	gput->T = 0;
	gput->W1 = 0;
	gput->W2 = 0;

	sbut->a0 = 0;
	sbut->a1 = 0;
	sbut->T = 0;
	sbut->W1 = 0;
	sbut->W2 = 0;

	glut->tauC = 0;
	glut->a1 = 0;
	glut->T = 0;
	glut->W1 = 0;
	glut->W2 = 0;

	gpga->A0G = 0;
	gpga->A1G = 0;
	gpga->T = 0;
	gpga->W1 = 0;
	gpga->W2 = 0;

	glgp->tauGPS = 0;
	glgp->a1 = 0;
	glgp->T = 0;
	glgp->W1 = 0;
	glgp->W2 = 0;

	qzgp->a0 = 0;
	qzgp->a1 = 0;
	qzgp->T = 0;
	qzgp->W1 = 0;
	qzgp->W2 = 0;

	qzut->a0 = 0;
	qzut->a1 = 0;
	qzut->T = 0;
	qzut->W1 = 0;
	qzut->W2 = 0;

	bdut->A0_UTC = 0;
	bdut->A1_UTC = 0;
	bdut->T = 0;
	bdut->W1 = 0;
	bdut->W2 = 0;
}
