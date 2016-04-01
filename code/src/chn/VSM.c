/*
 * VSM.c
 *
 *  Created on: Aug 24, 2015
 *      Author: user
 */
#include "VSM.h"
#include <stdlib.h>
#include <complex.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "conf.h"
#include "conf_chn.h"
fl64 VSM(si32 * Is, si32 * Qs, fl64 T) {
	fl64 VSM;
	fl64 Zsum = 0;
	fl64comp Var = 0;
	fl64comp VarSum = 0;
	fl64comp Zm,Zv, Pav, Nv;
	fl64 Z[VSMINT];
	ui32 inda = 0;


	for (inda = 0; inda < VSMINT; inda++) {
		Z[inda] = (fl64) pow(Is[inda], 2) + (fl64) pow(Qs[inda], 2);
		Zsum += Z[inda];
	}
	// get the mean of Z
	Zm = Zsum / VSMINT;
	//Get the variance of Z
	for (inda = 0; inda < VSMINT; inda++) {;
		Var = pow((Z[inda]-Zm),2);
		VarSum +=Var;
	}
	Zv = VarSum/VSMINT;

	Pav = (fl64comp) csqrt(pow(Zm,2)-Zv);

	Nv = 0.5*(Zm-Pav);

	VSM = (fl64) 10*log10(cabs((1/T)*Pav/(2*Nv)));
	return VSM;

}
