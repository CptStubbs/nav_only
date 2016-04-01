/*
 * sch_main.c
 *
 *  Created on: Aug 10, 2015
 *      Author: staffan
 */
#include "schMain.h"																	/// Corresponding header
#include <stdlib.h>

#include "initEphSch.h"
#include "satState.h"

void schInit(sch_s * sch){
	ephSch_s eph;
	satState_s satGNSS;

	initEph(&eph);
	initSatState(&satGNSS);
	roe_init(sch->roe);

	return;
}

void * schMain(void * param) {
	(void) param;
	return NULL;
}

void schFree(sch_s * sch){
	(void) sch;
	return;
}

