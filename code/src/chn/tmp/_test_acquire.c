#include "acquire.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "svi.h"
#include "chn.h"

#ifdef __DO_TEST_ACQUIRE
	int main(){
		// Initialize svi
		struct svi_s svi;	
		svi_init(&svi);
		// Spawn chn.c thread
		pthread_t th0;
		pthread_create(&th0, NULL, chn, &svi);
		// Initialize acquire_inp_s
		struct acquire_inp_s ainp; 
		ainp.prn 		= 1;
		ainp.fs 		= 4.0e6;
		ainp.freqlo = -10.0e3;
		ainp.freqhi = +10.0e3;
		// Declare acquire_out_s
		struct acquire_out_s aout;
		// call acquire
		if (acquire(&svi,&ainp,&aout)==EXIT_FAILURE){
			return EXIT_FAILURE;
		};
		 /**  Print results*/
		printf("Acquisition results:\n");
		if (aout.found){
			printf("found:      yes\n");	/*!< Detailed description after the member */
			printf("codephase:  %f\n",aout.codephase);
			printf("codefreq:   %f\n",aout.codefreq);
			printf("carrphase:  %f\n",aout.carrphase);
			printf("carrfreq:   %f\n",aout.carrfreq);	
		} else {
			printf("found:      no\n");
		};
		// Join the spawned chn thread
		printf("Waiting for thread to finish.\n");
		fflush(stdout);
		pthread_join(th0, NULL);
		return EXIT_SUCCESS;
	}  
#endif