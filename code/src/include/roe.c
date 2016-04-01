/*
 * roe.c
 *
 *  Created on: Nov 18, 2015
 *      Author: erin
 */
#include <stdlib.h>
#include "roe.h"
#include "conf_sch.h"
#include "customDataTypes.h"

int roe_initEmt(roe_emt_s * roeEmt){
	roeEmt->radDst = 0;
	roeEmt->radVel = 0;
	roeEmt->radAcc = 0;
	roeEmt->az = 0;
	roeEmt->el = 0;
	roeEmt->occType = UNK;
	return EXIT_SUCCESS;
}

int roe_init(roe_s * roe){
	si32 prn;

	for (prn = 0; prn <= NUMSATSGPS; prn++){
		roe_initEmt(&roe->emtGPS[prn]);
	}

	for (prn = 0; prn <= NUMSATSGLO; prn++){
		roe_initEmt(&roe->emtGLO[prn]);
	}

	for (prn = 0; prn <= NUMSATSGAL; prn++){
		roe_initEmt(&roe->emtGAL[prn]);
	}

	for (prn = 0; prn <= NUMSATSBDS; prn++){
		roe_initEmt(&roe->emtBDS[prn]);
	}
	return EXIT_SUCCESS;
}
void roe_free(roe_s * roe){
	(void) roe;
}
