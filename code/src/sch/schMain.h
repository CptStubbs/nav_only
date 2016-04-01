/*
 * sch_main.h
 *
 *  Created on: Aug 10, 2015
 *      Author: mern
 */

#ifndef SRC_SCH_SCHMAIN_H_
	#define SRC_SCH_SCHMAIN_H_

	#include "apt.h"
	#include "kfs.h"
	#include "svi.h"
	#include "roe.h"

	typedef struct {
		apt_s * apt;
		kfs_s * kfs;
		roe_s * roe;
	} sch_s;

	void * schMain(void * param);
	void schFree(sch_s * sch);
	void schInit(sch_s * sch);

#endif /* SRC_SCH_SCHMAIN_H_ */
