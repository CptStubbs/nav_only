/**
 * @file 			ctr.c
 * @author 		staffan
 * @date 			09-Nov-2015
**/

#include "ctr.h"																			/// Corresponding header
#include "conf.h"																			/// CONF_NUMBLK, CONF_NUMCPB
#include "conf_chn.h"																	/// MYxxx, CODEFREQ, CARRFREQ
#include <stdlib.h>																		/// Return values
#include "math.h"																			/// ceil(),

void ctr_chn_init(ctr_cha_s * ctr, si32 m) {
	int prnList[PRNLISTLENGTH] = MYPRN;									/// Array of PRN numbers
	int codePhase[PRNLISTLENGTH] = MYPRNCODEPHASE;			/// Array of code phases
	double carrFreq[PRNLISTLENGTH] = MYPRNCARRFREQ;			/// Array of carrier frequencies

	if ( MODE) { 																				/// Start in tracking mode
		ctr->prn = prnList[m] - 1; 												/// Set to define value for satellite identifier (-1 cause of array indexing)
		ctr->codeFreq = CODEFREQ;  												/// Set to define value
		ctr->carrFreq = carrFreq[m];  										/// Set to define value
		ctr->startSample = codePhase[m] - 1; 							/// Set to define value (-1 cause of array indexing), either acq or tracking
		ctr->remCodePhase = 0.0;  												/// Initial value of "left over" is 0
		ctr->remCarrPhase = 0.0;  												/// Initial value of "left over" is 0
		ctr->blkSize = (int) (ceil(
				((double) (PRNCODELENGTH)) / (CODEFREQ / SAMPLINGRATE ))); // set # of samples in the first block
	} else { 																						/// Start in acquisition mode
		ctr->prn = 0; 																		/// Set to define value for satellite identifier (-1 cause of array indexing)
		ctr->newPrn = false;
		ctr->codeFreq = CODEFREQ;  												/// Set to define value
		ctr->carrFreq = CARRFREQ - CARRDOPPSEARCH; 				///	Set to define value
		ctr->newFreq = false;
		ctr->startSample = 0; 														/// Set to define value (-1 cause of array indexing), either acq or tracking
		ctr->remCodePhase = 0.0;  												/// Initial value of "left over" is 0
		ctr->remCarrPhase = 0.0;  												/// Initial value of "left over" is 0
		ctr->blkSize = (int) (ceil(
				((double) (PRNCODELENGTH)) / (CODEFREQ / SAMPLINGRATE ))); // set # of samples in the first block
	}
}


int ctr_init(ctr_s * ctr){
	int m,n;																						/// Block and channel counter
	int k = 0;																					/// Running channel counter
	for (m=0; m<CONF_NUMBLK; m++){											/// Loop over blocks
		for (n=0; n<CONF_NUMCPB; n++){										/// Loop over channels
			ctr_chn_init(&ctr->blks[m].chns[n],k);					/// Initialize one channel
			k++;																						/// Increase channel counter
		}	/// for (n= ...)
	} /// for (m= ...)
	return EXIT_SUCCESS;																/// Always successful
}

int ctr_free(ctr_s * ctr){
	(void) ctr;																					/// Cast to suppress compiler warning
	return EXIT_SUCCESS;																/// Always successful
}

