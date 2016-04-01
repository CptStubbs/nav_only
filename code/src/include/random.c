/**
 * This file provides random number functionality (including randn)
 */

#include <conf_nav.h>
#include <math.h>														/// sqrt, log, cos, M_PI
#include <stdlib.h>													/// srand, rand
#include "random.h"

void rand_init(ui32 seed){
	srand(seed);
}

fl64 rand_fl64(){
	return ((fl64) rand())/((fl64) RAND_MAX);	/// Between 0 and 1
}

fl32 rand_fl32(){
	return ((fl32) rand())/((fl32) RAND_MAX);	/// Between 0 and 1
}

fl64 randn_fl64(){
	fl64 a = rand_fl64();										/// Uniform [0,1]
	fl64 b = rand_fl64();										/// Uniform [0,1]
	return sqrt(-2*log(a))*cos(2*M_PI*b);		/// Box-muller transform
}

fl32 randn_fl32(){

	fl32 a = rand_fl32();										/// Uniform [0,1]
	fl32 b = rand_fl32();										/// Uniform [0,1]
	return sqrt(-2*log(a))*cos(2*M_PI*b);		/// Box-muller transform
}
