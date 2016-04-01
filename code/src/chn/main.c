#include "chn.h"
#include "ctr.h"
#include <cor.h>
#include "chn_print.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "chn_init.h"

static const bool doprint = true;

// Generate a sample from a standard normal distribution using the box-muller transform
fl64 randn() {
	fl64 U1 = ((fl64) rand()) / ((fl64) RAND_MAX);	// Between 0 and 1
	fl64 U2 = ((fl64) rand()) / ((fl64) RAND_MAX);	// Between 0 and 1
	return sqrt(-2 * log(U1)) * cos(2 * M_PI * U2);	// Box-muller transform
}

#ifndef __DO_TEST_ACQUIRE
int main() {
	if (doprint == true) {
		printf("Running '_test_chn.c' as main\n");
		fflush(stdout);
	}
	chn_s chn;
	res_s res;
	ctr_s ctr;
	apt_s apt;

	si32 m, n;

	/// Initialize data structures
	res_init(&res);
	ctr_init(&ctr);
	chn_init(&chn, &res, &ctr, &apt);
	//ccr_print(&ccr,CONF_NUMCHN,doprint);
	srand((unsigned) time(NULL));	// Initialize random generator

	// Spawn chn.c thread
	pthread_t th0;
	pthread_create(&th0, NULL, chn_main, &chn);
	//nanosleep((struct timespec[]){{0, 10000000}}, NULL);	// sleep for 10 ms
	//nanosleep((struct timespec[]){{0, 10000000}}, NULL);	// sleep for 10 ms
	//nanosleep((struct timespec[]){{0, 10000000}}, NULL);	// sleep for 10 ms
	// Grab new data
	for (m = 0; m < 20; m++) {	// Loop over updates
		for (n = 0; n < CONF_NUMCHN; n++) {	// Loop over channels
			if (ctr.blks[0].chns[n].newFreq == true) {
				// We have new control parameters -
				if (doprint == true) {
					printf("prn: %02u, %+8.1f %8.1f\n", ctr.blks[0].chns[n].prn,
							ctr.blks[0].chns[n].carrFreq, ctr.blks[0].chns[n].codeFreq);
					fflush(stdout);
				}
				ctr.blks[0].chns[n].newFreq = false;
				// Update correlator dumps
				res.blks[n].chns[0].EI = (si32) (randn() * 2000.0);
				res.blks[n].chns[0].EQ = (si32) (randn() * 2000.0);
				res.blks[n].chns[0].PI = (si32) (randn() * 2000.0);
				res.blks[n].chns[0].PQ = (si32) (randn() * 2000.0);
				res.blks[n].chns[0].LI = (si32) (randn() * 2000.0);
				res.blks[n].chns[0].LQ = (si32) (randn() * 2000.0);
				// Simulate an acquired signal
				if (m > 9) {
					res.blks[n].chns[0].EI = +325000 + 1000;
					res.blks[n].chns[0].EQ = -325000 - 1000;
					res.blks[n].chns[0].PI = +20;
					res.blks[n].chns[0].PQ = -10;
					res.blks[n].chns[0].LI = +325000;
					res.blks[n].chns[0].LQ = -325000;
				}
				res.blks[n].chns[0].newCorr = true;
			}
		}
		//nanosleep((struct timespec[]){{0, 1000000}}, NULL);	// sleep for 1 ms
	}
	pthread_join(th0, NULL);
	if (doprint == true) {
		printf("Exiting '_test_chn.c'\n");
		fflush(stdout);
	}
	return EXIT_SUCCESS;
}
#endif
