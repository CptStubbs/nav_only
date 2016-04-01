/*
 * satState.c
 *
 *  Created on: Nov 19, 2015
 *      Author: erin
 */
#include <stdlib.h>
#include "satState.h"
#include "customDataTypes.h"
#include "conf_sch.h"

int initSatEmt(satEmt_s * satEmt){
	satEmt->pos[0] = 0;
	satEmt->pos[1] = 0;
	satEmt->pos[2] = 0;

	satEmt->vel[0] = 0;
	satEmt->vel[1] = 0;
	satEmt->vel[2] = 0;

	satEmt->acc[0] = 0;
	satEmt->acc[1] = 0;
	satEmt->acc[2] = 0;

	satEmt->clk = 0;

	return EXIT_SUCCESS;
}

int initSatState(satState_s * satGNSS){
	si32 prn;

	for (prn = 0; prn <= NUMSATSGPS; prn++){
		initSatEmt(&satGNSS->satGPS[prn]);
	}

	for (prn = 0; prn <= NUMSATSGLO; prn++){
		initSatEmt(&satGNSS->satGLO[prn]);
	}

	for (prn = 0; prn <= NUMSATSGAL; prn++){
		initSatEmt(&satGNSS->satGAL[prn]);
	}

	for (prn = 0; prn <= NUMSATSBDS; prn++){
		initSatEmt(&satGNSS->satBDS[prn]);
	}

	return EXIT_SUCCESS;
}


void freeSatState(satState_s * satGNSS){
	(void) satGNSS;
}



