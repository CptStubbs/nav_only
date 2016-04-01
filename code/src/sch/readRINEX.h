/*
 * readRINEX.h
 *
 *  Created on: Nov 5, 2015
 *      Author: erin
 */

#ifndef SRC_SCH_READRINEX_H_
	#define SRC_SCH_READRINEX_H_

	#include <stdlib.h>
	#include <stdio.h>
	#include "conf_sch.h"
#include "initEphSch.h"

	void readRINEX(ephSch_s * eph,fl64 t);
	void getTimeTransfer(FILE * fileID,ephSch_s * eph);
	void parseGPSEntry(FILE * fileID,ephSch_s * eph,fl64 t,int satID,
			double a_f0,double a_f1,double a_f2);
	void parseGLOEntry(FILE * fileID,ephSch_s * eph,fl64 t,int satID,
			double tau_n,double gam,double TOD);
	void parseGALEntry(FILE * fileID,ephSch_s * eph,fl64 t,int satID,
			double a_f0,double a_f1,double a_f2);
	void parseBDSEntry(FILE * fileID,ephSch_s * eph,fl64 t,int satID,
			double a_f0,double a_f1,double a_f2);
	void parseQZSEntry(FILE * fileID,ephSch_s * eph,fl64 t,int satID,
			double a_f0,double a_f1,double a_f2);
	void parseSBAEntry(FILE * fileID,ephSch_s * eph,fl64 t,int satID,
			double aGf0,double aGf1,double TOW);
	int checkEphEntry(fl64 currTime, float t_oe, char gnss);

#endif /* SRC_SCH_READRINEX_H_ */
