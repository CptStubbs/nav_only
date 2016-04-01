/*
 * conf_sch.h
 *
 *  Created on: Nov 4, 2015
 *      Author: erin
 */
#include "conf.h"

#ifndef SRC_CONF_CONF_SCH_H_
	#define SRC_CONF_CONF_SCH_H_

	#define SPEEDOFLIGHT 	299792458	//Speed of light [m/s]
	#define FL1 					1575.42e6 //Frequency of L1 signal [Hz]
	#define FL2 					1227.60e6 //Frequency of L2 signal [Hz]
	#define FL5 					1176.45e6 //Frequency of L5 signal [Hz]

	// Constants necessary for the GPS satellite state calculations
	#define RADEGPS  			6378136.3 // Semi-major axis of Earth [m]
	#define OMEGAEDOTGPS	7.2921151467e-5 // WGS84 defined rotation rate of Earth [rad/s]
	#define GMGPS 				3.986005e14 //Earth gravitational parameter [m^3/s^2]
	#define FGPS 					-4.442807633e-10 //Flattening constant [s/(m^(1/2))]
	#define NUMSATSGPS  	32 	// Number of GPS satellites
	#define EPHUPDATEGPS 	7200 // Ephemeris update rate [s]

	// Constants necessary for the GLONASS satellite state calculations
	#define RADEGLO 			6378136.0 // Semi-major axis of Earth [m]
	#define OMEGAEDOTGLO	7.292115e-5 //Rotation rate of Earth [rad/s]
	#define GMGLO 				3.986004418e14 //Earth gravitational parameter [m^3/s^2]
	#define J02						1.082657e-3 //Second zonal harmonic of the geopotential
	#define SIDDAYGLO			0.93851851806 //Sidereal day
	#define NUMSATSGLO 		24 	// Number of GLONASS satellites
	#define EPHUPDATEGLO	1800 // Ephemeris update rate [s]
	#define PZ90TOWGS84 	1.6e-6 // Rotation from PZ-90 to WGS84

	// Constants necessary for the Galileo satellite state calculations
	#define OMEGAEDOTGAL 	7.2921151467e-5  // WGS84 defined rotation rate of Earth [rad/s]
	#define GMGAL 				3.986004418e14 //Earth gravitational parameter [m^3/s^2]
	#define FGAL 					-4.442807309e-10 //Flattening constant [s/(m^(1/2))]
	#define NUMSATSGAL		30 // Number of Galileo satellites
	#define EPHUPDATEGAL	600 // Ephemeris update rate [s]

	// Constants necessary for the BeiDou satellite state calculations
	#define OMEGAEDOTBDS 	7.2921150e-5 //Rotation rate of Earth [rad/s]
	#define GMBDS 				3.986004418e14 //Earth gravitational parameter [m^3/s^2]
	#define FBDS 					-4.44280730904398e-10 //Flattening constant [s/(m^(1/2))]
	#define LEAPSECONDS2006	14 //Number of leap seconds in 2006
	#define NUMSATSBDS		35	// Number of BeiDou satellites
	#define EPHUPDATEBDS 	3600 // Ephemeris update rate [s]

	// Constants necessary for the QZSS satellite state calculations
	#define NUMSATSQZS 		1 	// Number of QZSS satellites
	#define EPHUPDATEQZS	900 // Ephemeris update rate [s]

	// Constants necessary for the SBAS satellite state calculations
	#define NUMSATSSBA 		38	// Number of SBAS satellites
	#define EPHUPDATESBA 	600 // Ephmeris update rate [s]

	//Conversions
	#define M2KM					1e-3	// [km]
	#define KM2M					1e3  // [m]
	#define ONEDAY				86400 // [s]
	#define ONEWEEK 			604800 // [s]

	//Numerical limitations
	#define EPSILON 			1e-12

	//Occultation settings
	#define OLMAX					50e3 	// Open loop region upper bound [m]
	#define OLMIN					-250e3 // Open loop region lower bound [m]
	#define OLCL					60e3 // Transition altitude between open and closed loop tracking[m]
	#define FOV						55	// Occultation antenna field of view from boresight [degrees]

#endif /* SRC_CONF_CONF_SCH_H_ */
