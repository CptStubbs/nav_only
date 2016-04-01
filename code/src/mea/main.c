/*
 * main.c
 *
 *  Created on: May 7, 2015
 *      Author: sean
 */
#include <stdio.h>								// Printing
#include <stdlib.h>								// EXIT
#include <time.h>									// Waiting
#include "customDataTypes.h"			// Data types
#include "mea.h"									// Data types for mea
#include "mea_init.h"							// Initialize and free
#include "mea_main.h"							// Main function
#include <pthread.h>							// Threading

bool KEEPTHREADSALIVE = true;

int main() {
	pthread_t th;
	mea_s mea;
	mea.value = 12;
	printf("This is mea/meaexe\n");
	printf("Forking thread:\n");
	pthread_create(&th, NULL, mea_main, &mea);
	printf("Waiting:\n");
	nanosleep((struct timespec[]){{1, 1000000}}, NULL);
	KEEPTHREADSALIVE = false;
	printf("Joining\n");
	pthread_join(th, NULL);
	printf("Done\n");
	return EXIT_SUCCESS;
}
