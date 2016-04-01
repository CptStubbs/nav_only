/*
 * mea_main.c
 *
 *  Created on: Aug 10, 2015
 *      Author: staffan
 */
#include <stdlib.h>
#include <stdio.h>
#include "mea.h"
#include <time.h>

void * mea_main(void * param) {
	mea_s * mea = (mea_s *) param;
	extern bool KEEPTHREADSALIVE;
	while (KEEPTHREADSALIVE){
		mea->value ++;
		nanosleep((struct timespec[]){{0, 1000}}, NULL);
	};
	return NULL;
}
