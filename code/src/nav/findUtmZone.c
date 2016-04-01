/*
 * findUtmZone.c
 *
 *  Created on: May 18, 2015
 *      Author: sean
 */

#include <conf_nav.h>
#include "customDataTypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

si32 findUtmZone(si32 * utmZone, fl64 latitude, fl64 longitude) {

	si32 tempUtmZone = 0;
	if ((longitude > 180) || (longitude < -180)) {
		fprintf(stderr, "Longitude value exceeds limits (-180:180).");
		return EXIT_FAILURE;
	}
	if ((latitude > 84) || (latitude < -80)) {
		fprintf(stderr, "Latitude value exceeds limits (-80:84).");
		return EXIT_FAILURE;
	}

	tempUtmZone = (si32) trunc((180 + longitude) / 6) + 1;

	if (latitude > 72) {
		if ((longitude >= 0) && (longitude < 9)) {
			tempUtmZone = 31;
		} else if ((longitude >= 9) && (longitude < 21)) {
			tempUtmZone = 33;
		} else if ((longitude >= 21) && (longitude < 33)) {
			tempUtmZone = 35;
		} else if ((longitude >= 33) && (longitude < 42)) {
			tempUtmZone = 37;
		}

	} else if ((latitude >= 56) && (latitude < 64)) {
		/*! correction for zone 32 */
		if ((longitude >= 3) && (longitude < 12)) {
			tempUtmZone = 32;
		}
	}

	*utmZone = tempUtmZone;
	return EXIT_SUCCESS;
}
