/*
 * satState.h
 *
 *  Created on: Nov 19, 2015
 *      Author: erin
 */

#ifndef SRC_SCH_SATSTATE_H_
#define SRC_SCH_SATSTATE_H_

#include "customDataTypes.h"
#include "conf_sch.h"

typedef struct {
	fl64 pos[3];
	fl64 vel[3];
	fl64 acc[3];
	fl64 clk;
} satEmt_s;

typedef struct {
	satEmt_s satGPS[NUMSATSGPS];
	satEmt_s satGLO[NUMSATSGLO];
	satEmt_s satGAL[NUMSATSGAL];
	satEmt_s satBDS[NUMSATSBDS];
} satState_s;

int initSatState(satState_s * satGNSS);
void freeSatState(satState_s * satGNSS);

#endif /* SRC_SCH_SATSTATE_H_ */
