#include "acquire.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#define PI (3.141592653589793)

si32 acquire(struct svi_s * svi, struct acquire_inp_s * inp, struct acquire_out_s * out){
	// Temp behavior: randomly perform acq
	srand (time(NULL));
	si32 r = rand();	// between 0 and RAND_MAX
	out->peak = ((fl32) r) / ((fl32) RAND_MAX);	// between 0.0 and 1.0
	printf("Peak %4.2f\n",out->peak);
	out->found = out->peak>0.5;
	if (out->found){
		out->codephase = (((fl64) rand()) / RAND_MAX)*1e6;
		out->codefreq  = 1.023e6 + (((fl64) rand()) / RAND_MAX)*2 - 1;
		out->carrphase = (((fl64) rand()) / RAND_MAX)*PI;
		out->carrfreq  = (((fl64) rand()) / RAND_MAX)*20000 - 10000;		
	};
	
	/// Final behavior:
	/// 1. Modify svi_s according to acquire_inp_s
	/// 2. Wait until chn is finished with acquisition
	sleep(1);
	/// 3. Update acquire_out_s and exit

	return EXIT_SUCCESS;
}


