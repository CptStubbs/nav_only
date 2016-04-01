/*
 * loadLEO.h
 *
 *  Created on: Nov 11, 2015
 *      Author: erin
 */

#ifndef SRC_SCH_LOADLEO_H_
#define SRC_SCH_LOADLEO_H_

#include "customDataTypes.h"
#include "conf_sch.h"

typedef struct {
	fl64 x_m[ONEDAY];
	fl64 y_m[ONEDAY];
	fl64 z_m[ONEDAY];

	fl64 vx_m[ONEDAY];
	fl64 vy_m[ONEDAY];
	fl64 vz_m[ONEDAY];

	fl64 ax_m[ONEDAY];
	fl64 ay_m[ONEDAY];
	fl64 az_m[ONEDAY];

	fl64 clk_bias[ONEDAY];
	fl64 clk_drift[ONEDAY];

} leoOrb_s;

void loadLEO(leoOrb_s * leoOrb);
void getValues(char * filePath,leoOrb_s * leoOrb,int type);

#endif /* SRC_SCH_LOADLEO_H_ */
