/*
 * conf_nav.h
 *
 *  Created on: May 11, 2015
 *      Author: gnss
 */
#include "conf.h"

#ifndef SRC_CONF_CONF_NAV_H_
	#define SRC_CONF_CONF_NAV_H_
			extern int make_ISO_compiler_happy;
	// Define the expected size of the bits array. Subject to change as development continues
	#define BITSSIZE 			300

	// Included for cross compatibility
	#ifndef M_PI
		#define M_PI 				3.14159265358979323846
	#endif

	#ifndef OMEGAE_DOT
		#define OMEGAE_DOT 			7.2921151467e-5 // Earth rotation rate, [rad/s]
	#endif

	#ifndef GM
		#define GM 					3.986005e14 	// Earth's universal gravitational parameter, [m^3/s^2]
	#endif

	#ifndef F
		#define F 					-4.442807633e-10 // Constant, [sec/(meter)^(1/2)]
	#endif

	// For NAV decoding
	#define BUF_LENGTH			6220		// Buffer size for tracking results for navigation solution
	#define NAVBIT_BUF_LENGTH	302			// Buffer size for the navigation bit decoding
	#define NUMSATS 			32			//  MAX number of sats
	#define NUM_PRNS   			32			// Number of PRN list
	#define BUFF_SEARCH_RANGE	21			// Search range for transmit time
	#define BIT_REMAINDER 		41			// Search boundary for transmit time
	#define BITSYNCHLENGTH		1400		// How many bits to check for bit synch
	#define LONGLENGTH 			9			// Maximum length of a long int

	// For NAV solution
	#define NAVRATE 			1.0			// Position update rate
	#define NAVSAMSTEP 			SAMPLINGRATE/NAVRATE	// Number of samples in 1 NAV step
	#define MASKANGLE 			10.0		// Cut off angle for satellite
	#define NAVRSTSEC			30.0		// Navigation reset if there is insufficient #SVs for NAVRSTSEC (unit: seconds)

	#define SECOFWEEK 			604800		// seconds of week for week rollover

	#define TRANMITOFFSET 		68.802		// Initial base offset for travel time

	#define CLKSTRON 			1			// Clock steering on (1) / off (0)
	#define CLKSTRNUM 			10			// Number of NAV steps for clock steering
	#define SPEEDOFLIGHT 		299792458	// Speed of light

	#define IONOCORR 			1			// Iono correction on (1) / off (0)
	#define TROPCORR 			1			// Tropo correction on (1) / off (0)

	#define VELSOL 				2			// Velocity solution on (1 & 2) / off (0)
	#define LEAPSECONDS 		16			// Leap seconds

	// RINEX settings
	#define RUNBY 				"GNSS Lab,UCB"

	// This function prints the last received error messaged and the current line
	#define ERRORCHECK(a)\
			if (a)\
			{\
				perror("Error Message: ");\
				fprintf(stderr, "Error at %s line %d\n", __FILE__, __LINE__);\
				return EXIT_FAILURE;\
			}

#endif /* SRC_CONF_CONF_NAV_H_ */

