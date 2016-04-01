/*
 * PRM.c
 *
 *  Created on: Aug 26, 2015
 *      Author: user
 */
#include "PRM.h"
#include <stdlib.h>
#include <complex.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "conf.h"
#include "conf_chn.h"

//! WARNING DO NOT ATTEMPT TO USE THIS FUNCTION!!! IT WILL NOT WORK PROPERY UNLESS A VERY LARGE AMOUNT OF P_I DATA IS USED>
fl64 PRM(si32 * Is, si32 * Qs, fl64 T) {
	fl64 PRM;
	fl64 Zsum = 0;
	fl64 Isum[10];
	fl64 Qsum[10];
	fl64 II = 0;
	fl64 QQ = 0;
	fl64comp MuNP = 0;
	fl64comp WBP[10];
	fl64comp NBP[10];
	fl64comp NP[10];
	ui32 inda = 0;
	ui32 M = 20;
	ui32 cnt = 0;
	//sum the numbers 20 at a time. Wide band power
	for (inda = 1; inda < (VSMINT + 1); inda++) {
		Zsum += (fl64) pow(Is[inda - 1], 2) + (fl64) pow(Qs[inda - 1], 2);
		II += (fl64) (Is[inda]);
		QQ += (fl64) (Qs[inda]);
		//printf("%d; %d %d,sum %f\n",inda-1,Is[inda-1],Qs[inda-1], Zsum);
		if ((inda % 20 == 0)) { //
			//printf("Cnt = %d;inda= %d\n",(cnt),inda);
			WBP[cnt] = Zsum;			//this sum is not correct!!!
			Isum[cnt] = II;
			Qsum[cnt] = QQ;

			Zsum = 0; //reset the sum
			II = 0;
			QQ = 0;
			cnt++;
		}
	}

	for (inda = 0; inda < 10; inda++) {
		if (WBP[inda] != 0) {
			NBP[inda] = pow(Isum[inda], 2) + pow(Qsum[inda], 2);
			NP[inda] = NBP[inda] / WBP[inda];
			MuNP += NP[inda];
		} else {
			PRM = 0;
			return PRM;
		}
	}
	MuNP = MuNP / 10;
	PRM = (fl64) 10*log10(cabs((1/T)*(MuNP-1)/(M-MuNP)));
	return PRM;
}

