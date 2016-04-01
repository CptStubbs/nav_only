#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#include "ccr.h"

// Wipe carrier function
int wipeCarrier4_04_08(ui32 * carrPhase, ui32 carrFreq, ci8 * inp, ci8 * out){
	// 4 bits long LUT, 4 bits input, 8 bits output
	ui8 cps;	//carrPhaseShifted
	carrPhase = carrPhase + carrFreq;
	cps = (ui8) (carrPhase >> 28);	// 4 bits left
	// from "generateLUTcode.m"
	cps = (ui8)(carrPhase >> 28);
	const static pi8 SL[16] = {+00,+06,+11,+14,+15,+14,+11,+06,+00,-06,-11,-14,-15,-14,-11,-06};
	const static pi8 CL[16] = {+15,+14,+11,+06,+00,-06,-11,-14,-15,-14,-11,-06,+00,+06,+11,+14};
	// Generate output
	out.r = inp.r*CL[cps] - inp.i*SL[cps];
	out.i = inp.i*CL[cps] + inp.r*SL[cps];
};

// Correlate function
void *correlate(void *param){
	struct ccr_s *ccr  = (struct ccr_s *) param;	// Cast param to the correct type
	int m, n, m1, m2, started; 
	
	struct a2f_s a2f;
	struct f2a_s f2a;
	
	si32 I[8];
	si32 Q[8];
	
	ci8 data;
	ci8 bb;
		
	n = 0;
	started = false;
	
	for (m=0;m<100000;m++){		// Let m be the "cycle" counter
		if ((m % 10000)==0){
			printf("%5d\n",m);
		}
		if ( (!started) && (ccr->a2f[n].new) ){	// We have a new entry in the input queue
			a2f = ccr->a2f[n];	// copy input to our local copy
			m1 	= a2f.c_begin;
			m2 	= a2f.c_end;
			
			I[0] = 0;
			Q[0] = 0;
			ccr->a2f[n].new = false;
			ccr->c_a2f = m;
			started = true;
		}; 
		if ( (started) && (m>=m1) ){	// Process data
			// Get data
			data.r = +7;
			data.i = -3;
			// Wipe carrier
			wipeCarr4_04_08(&a2f.carrPhase,&a2f.carrFreq,&data,&outI,&outQ);
			// Wipe code
			switch (a2f.ctrFlags>>24) {
				case 0:	// GPS L1CA
					
				
			}
			
			I[0] = I[0] + 1;
			Q[0] = Q[0] - 1;
		};
		if ( (started) && (m==m2) ){ 	// Stop processing data
			started = false;
			memcpy(ccr->f2a[0].I,I,sizeof(I));
			memcpy(ccr->f2a[0].Q,Q,sizeof(Q));
			ccr->f2a[0].new = true;
			ccr->c_f2a = m;
		};
	};
	return NULL;
};
 
int main(){
	// Initialize communication data structure
	si32 m,n;
	ui32 len = 4;
	struct a2f_s a2f[len];
	for (m=0;m<len;m++){
		a2f[m].new = false;
		a2f[m].c_begin = 0;
		a2f[m].c_end = 0;
	}
	struct f2a_s f2a[len];	
	for (m=0;m<len;m++){
		f2a[m].new = false;
		f2a[m].I[0] = 0;
		f2a[m].Q[0] = 0;
	}
	struct ccr_s ccr; 
	ccr.c_a2f = 0;
	ccr.c_f2a = 0;
	ccr.len = len;		// Length of buffer
	ccr.a2f = a2f;
	ccr.f2a = f2a;

	pthread_t thread0;
	// Fork thread 
	n = pthread_create(&thread0, NULL, correlate, &ccr);
	if (n != EXIT_SUCCESS){
		printf("This failed miserably\n");
		return EXIT_FAILURE;
	}; 
	// Set up first comm channel
	ccr.a2f[0].c_begin 	= 100;
	ccr.a2f[0].c_end 	= 199;
	ccr.a2f[0].new 		= true;
	
	n = 0;
	for (m=0;m<1000000;m++){
		if (ccr.f2a[n].new){		// check for a new output (result)
			// Print the result
			printf("m=%d, n=%d, I=%d, Q=%d, a2f=%llu\n",m,n,ccr.f2a[n].I[0],ccr.f2a[n].Q[0],ccr.c_a2f);
			ccr.f2a[n].new = false;
			// Update the input 
			ccr.a2f[n].c_begin 	= ccr.a2f[n].c_begin + 1000;
			ccr.a2f[n].c_end 	= ccr.a2f[n].c_end 	+ 1000;
			ccr.a2f[n].new = true;
			//// Check the next entry in the queue next time
			
		};
	};

	pthread_join(thread0,NULL);
	
	return EXIT_SUCCESS;
};


