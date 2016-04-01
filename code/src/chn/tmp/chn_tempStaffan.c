#include "chn.h"
#include "ccr.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
//#include "gpsL1dr.h"

#define FLOAT2INT 8388608.0/127875.0
#define CARRFREQSTEP	1000.0*FLOAT2INT

#define CARRFREQSTART 4.092e6*FLOAT2INT - 10.0*CARRFREQSTEP+1.0
#define CARRFREQSTOP 	4.092e6*FLOAT2INT + 10.0*CARRFREQSTEP+100.0

void *chn(void *param){
	struct ccr_s * ccr = (struct ccr_s *) param;	// Cast the input to a ptr to struct ccr_s
	si32 m;
	// Noise power estimate
	ui64 noiseLevel = 325000;
	//printf("Noise level: %llu\n",noiseLevel);
	ui64 ep, pp, lp;
	// carrier frequency 
	ui32 carrFreqStart = (ui32) CARRFREQSTART;
	ui32 carrFreqStop  = (ui32) CARRFREQSTOP;
	ui32 carrFreqStep  = (ui32) CARRFREQSTEP;
	// codeslew
	ui32 codeSlewStart = 0;
	ui32 codeSlewStop  = 2045;
	ui32 codeSlewStep  = 3;
	
	// Tracking parameters 
	static const fl64 codeTau1 = 0.069846938775510;	// [tau1, tau2] = calcLoopCoef(2.0Hz, 0.7, 1.0)
  static const fl64 codeTau2 = 0.370000000000000;	// [tau1, tau2] = calcLoopCoef(2.0Hz, 0.7, 1.0)
	static const fl64 codePDI = 0.001;
	static const fl64 codeFreqBasis = 1.023e6;
	// Tracking variables
	fl64 codeNcoErrorNew;
	fl64 codeNcoErrorOld[CCR_MAX_CHANNELS];
	fl64 codeNco[CCR_MAX_CHANNELS];
	
	bool acquired[CCR_MAX_CHANNELS];
	for (m=0;m<CCR_MAX_CHANNELS;m++){
		codeNcoErrorOld[m] = 0.0;
		codeNco[m] = 0.0;
		acquired[m] = false;
	}
	
	// Set up each channel, sleep 1us between them
	for (m=0;m<CCR_MAX_CHANNELS;m++){
		ccr->ctr[m].prn = 1;//m+1;
		ccr->ctr[m].newPrn = true;
		ccr->ctr[m].carrFreq = carrFreqStart + m*carrFreqStep;
		ccr->ctr[m].codeFreq = (ui32) (FLOAT2INT * 1.023e6);
		ccr->ctr[m].codeSlew = codeSlewStart;
		ccr->ctr[m].new = true;
		nanosleep((struct timespec[]){{0, 1000}}, NULL);	// sleep for 1 us
	};
	nanosleep((struct timespec[]){{0, 1000}}, NULL);	// sleep for 1 us
	// Ok, now all channels should be up and running, monitor updates
	while (true){
		for (m=0;m<CCR_MAX_CHANNELS;m++){
			// Check if channel have been updated
			if (ccr->res[m].new == true){
				// Grab results and compute power 
				ep = (ui64) sqrt((ccr->res[m].EI*ccr->res[m].EI + ccr->res[m].EQ*ccr->res[m].EQ));	// early power
				pp = (ui64) sqrt((ccr->res[m].PI*ccr->res[m].PI + ccr->res[m].PQ*ccr->res[m].PQ)); // prompt power
				lp = (ui64) sqrt((ccr->res[m].LI*ccr->res[m].LI + ccr->res[m].LQ*ccr->res[m].LQ)); // late power
				ccr->res[m].new = false;
				//printf("powers: %u %u %u,\n",ep,pp,lp);
				// Use power to update noise level estimate
				noiseLevel = noiseLevel-(noiseLevel>>7)-(noiseLevel>>7)-(noiseLevel>>7) + (ep) + (pp) + (lp);
				//printf("Noise level: %8llu (%8llu %8llu %8llu)\n",noiseLevel,ep,pp,lp);
				//fflush(stdout);
				if ( (ep<<7)>(4*noiseLevel) || (pp<<7)>(4*noiseLevel) || (lp<<7)>(4*noiseLevel) ) {
					acquired[m] = true;
					printf("Acquired on ch %d\n",m);
				};
				// Acquisition or tracking?
				if (acquired[m]==false){
					// ----------------- Acquisition on this channel ------------------ 
					// Update codeslew -- (1/1)
					ccr->ctr[m].codeSlew = ccr->ctr[m].codeSlew + codeSlewStep;
					if (ccr->ctr[m].codeSlew>codeSlewStop){
						// codeslew out of range, set to codeSlewStart and update carrFreq -- (1/600)
						ccr->ctr[m].codeSlew = codeSlewStart;
						ccr->ctr[m].carrFreq = ccr->ctr[m].carrFreq + carrFreqStep;
						if (ccr->ctr[m].carrFreq>carrFreqStop){	
							// carrFreq out of range, set to carrFreqStart and update prn -- (1/12600) CHANGE - STICK TO JUST ONE PRN
							ccr->ctr[m].carrFreq = carrFreqStart;
							//ccr->ctr[m].prn = ((ccr->ctr[m].prn-1+CCR_MAX_CHANNELS) % 32) + 1;
							//ccr->ctr[m].newPrn = true;
						}
					}				
				} else {
					// ----------------- Tracking on this channel ------------------ 
					// only code tracking initially
					// Code discriminator (early minus late power)
					codeNcoErrorNew = ( (fl64) ep - (fl64) lp) / ( (fl64) ep + (fl64) lp);
					// 1st order NCO
					codeNco[m] = codeNco[m] + (codeTau2/codeTau1)*(codeNcoErrorNew-codeNcoErrorOld[m]) + (codePDI/codeTau1)*codeNcoErrorNew;
					// remember old code NCO value
					codeNcoErrorOld[m] = codeNcoErrorNew;
					ccr->ctr[m].codeFreq = (ui32) (codeFreqBasis - codeNco[m])*FLOAT2INT;
				}
				// Flag the updated values as new 
				ccr->ctr[m].new = true;
			}	
		}
		nanosleep((struct timespec[]){{0, 1000}}, NULL);	// sleep for 1 us
	}
	printf("Exiting chn.c\n");
	return NULL;
}

// Everything below is the real fsm, we will add that back in later
// si32 chk(void * param){
	// return EXIT_SUCCESS;
// }
// si32 acq(void * param){
	// return EXIT_SUCCESS;
// }
// si32 trk(void * param){
	// return EXIT_SUCCESS;
// }

// void *chn(void *param){
	// struct svi_s * svi = (struct svi_s *) param;	// Cast the input to a ptr to struct svi_s
	// ui32 k;
	// /// Quick Behaviour: 
	// /// 1. Initialize fsm

	// struct chn_nfo_s chn_nfo;
	// /// Declare the FSM state variable (only one for now)
	// fsm_s fsm;
	// fsm.state = 0;									// Current State	(Initialize to IDLE)
	// fsm.flags = 0;										// Condition flag
	// fsm.numS = 3;										// Number of states
	// fsm.numT = 7;										// Number of transitions

	// /// Set mask values
	// enum cnd_e {chn_true=0, chn_visible=1, chn_found=2, chn_done=4, chn_lock=8};
	
	// struct fsms_s S[fsm.numS];			// Declare array of states
	// k = 0;
	// S[k].fun = chk;		S[k].arg = &chn_nfo; 		k++;
	// S[k].fun = acq;		S[k].arg = &chn_nfo; 		k++;
	// S[k].fun = trk;		S[k].arg = &chn_nfo; 		k++;
	// fsm.S = S;
	// struct fsmt_s T[fsm.numT];			// Declare array of transitions
	// k = 0;
	// T[k].src = 0;	T[k].dst = 1;	T[k].cnd = chn_visible;						k++;
	// T[k].src = 0;	T[k].dst = 0;	T[k].cnd = chn_true;							k++;
	// T[k].src = 1;	T[k].dst = 2;	T[k].cnd = chn_found;							k++;
	// T[k].src = 1;	T[k].dst = 0;	T[k].cnd = chn_done;							k++;
	// T[k].src = 1;	T[k].dst = 1;	T[k].cnd = chn_true;							k++;
	// T[k].src = 2;	T[k].dst = 2;	T[k].cnd = chn_lock+chn_visible;	k++;
	// T[k].src = 2;	T[k].dst = 0;	T[k].cnd = chn_true;							k++;
	// fsm.T = T;

	// /// 2. Run through fsm until done (visible goes from true to false)
	// bool oldvis = false;
	// bool newvis = false;
	// fsm.flags = 0;
	// for (k=0;k<10;k++){
		// printf("state: %d\n",fsm.state);
		// if (fsm_update(&fsm)==EXIT_FAILURE){
			// printf("EXIT_FAILURE\n");
			// return;
		// };
		// // update newvis and oldvis
		// // Check if we are done (visible from true to false)
		// if ((oldvis == true) & (newvis == false)){
			// printf("EXIT_SUCCESS\n");
			// return;
		// };
	// };
	
	// printf("loop ended ... \n");
	// return EXIT_SUCCESS;
	// /// Final Behaviour: 
	// /// 0. loop through fsm's forever
// }
