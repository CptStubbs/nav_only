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
#ifndef __APT_DUMMYTESTING_H
	#define __APT_DUMMYTESTING_H

	#include "apt.h"

	void *apt_writer(void *);
	void *apt_reader(void *);

	#endif
