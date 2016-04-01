/**
 * This function helps testing the shared apt data structure.
 * Basically, it writes dummy values to the apt struct such that we can test nav.c or chn.c
 *
 * As it is designed to run in its own thread,the input is a void pointer that is cast to
 * the type we want.
 *
 * The function is rather simple. Every K/N second a new entry is written by apt_writer()
 * K = 1m*APT_LEN, N=CONF_NUMCHN. It is cleared by apt_reader().
**/

#include "customDataTypes.h"
#include "apt.h"
#include "conf.h"
#include "conf_apt.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#define D_PI 3.14

fl64 randn(){	// Random value from a normal distribution
	fl64 U1 = ( (fl64) rand()) / ( (fl64) RAND_MAX);	// Between 0 and 1
	fl64 U2 = ( (fl64) rand()) / ( (fl64) RAND_MAX);	// Between 0 and 1
	return sqrt(-2*log(U1))*cos(2*D_PI*U2);						// Box-muller transform
}

void *apt_writer(void *param){
	apt_s * apt = (apt_s *) param;
	extern bool KEEPTHREADSALIVE;
	ui32 m,n;

	/// Find out how long we should sleep for
	fl64 waitTime = 0.001 * (fl64) APT_LEN / CONF_NUMCHN;			/// fl64 value
	ui32 secs = floor(waitTime);															/// integer seconds
	ui32 nsec = (ui32) ((waitTime - (fl64) secs)*1000000000);	/// integer nanoseconds
	while (KEEPTHREADSALIVE){
		nanosleep((struct timespec[]){{secs, nsec}}, NULL);
		/// Update message
		n = apt->indWrite;
		pthread_mutex_lock(&apt->emt[n].lock); 	/// Lock mutex
		apt->emt[n].id = 0x0101;								/// GPS, PRN 1

		apt->emt[n].len = APT_LEN;							/// Fully populated
		for (m=0; m<APT_LEN; m++){
			apt->emt[n].P_I[m] = (si32) m;				/// Inphase prompt
			//apt->emt[n].smp[m] = (ui64) m;				/// Integer sample
			//apt->emt[n].frc[m] = (ui32) m;				/// Fractional sample
		}
		apt->emt[n].newApt = true;							/// indicate new message
		pthread_mutex_unlock(&apt->emt[n].lock);/// Unlock mutex
		apt->indWrite = (n + 1) % APT_NUM;			/// Increase the write index
	}
	return NULL;
}

void *apt_reader(void *param){
	apt_s * apt = (apt_s *) param;
	extern bool KEEPTHREADSALIVE;
	ui32 n;

	while (KEEPTHREADSALIVE){
		n = apt->indRead;
		pthread_mutex_lock(&apt->emt[n].lock); 	/// Lock mutex
		if (apt->emt[n].newApt == true){				/// We have a new message
			///printf("received msg %u\n",n);						/// Print simple response
			apt->emt[n].newApt = false;						/// Clear apt message
		}
		apt->indRead = (n + 1) % APT_NUM;
		pthread_mutex_unlock(&apt->emt[n].lock);/// Unlock mutex
	}
	return NULL;
}
