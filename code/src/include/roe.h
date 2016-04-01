/*
 * roe.h
 *
 *  Created on: Nov 18, 2015
 *      Author: erin
 *      This header file defines the information contained in a Radio Occultation Event (ROE)
 */

#ifndef SRC_INCLUDE_ROE_H_
	#define SRC_INCLUDE_ROE_H_
	#include "customDataTypes.h"			// Standard types
	#include <pthread.h>							// mutex locks
	#include "conf_sch.h"

enum roe_cat_e {
	POD,
	FCL,
	FOC,
	FOL,
	AOL,
	ACO,
	ACL,
	NIV,
	UNK
};

typedef struct{
	fl64 radDst;
	fl64 radVel;
	fl64 radAcc;
	fl64 az;
	fl64 el;
	enum roe_cat_e occType;
} roe_emt_s;

typedef struct{
	roe_emt_s emtGPS[NUMSATSGPS];
	roe_emt_s emtGLO[NUMSATSGLO];
	roe_emt_s emtGAL[NUMSATSGAL];
	roe_emt_s emtBDS[NUMSATSBDS];
} roe_s;

int roe_init(roe_s * roe);
void roe_free(roe_s * roe);

#endif /* SRC_INCLUDE_ROE_H_ */
