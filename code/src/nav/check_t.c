/*
 * check_t.c
 *
 *  Created on: May 13, 2015
 *      Author: sara
 */

#include <tgmath.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "check_t.h"

#include <conf_nav.h>
#include "customDataTypes.h"

void check_t(
/* Output */
fl64 * corrTime,
/* Input */
fl64 time) {

	fl64 half_week = 0;

	half_week = 302400; /* seconds*/

	*corrTime = time;

	if (time > half_week) {
		*corrTime = time - 2 * half_week;
	} else if (time < -half_week) {
		*corrTime = time + 2 * half_week;
	}

}
