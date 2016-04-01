/*
 * leastSquarePos.h
 *
 *  Created on: May 11, 2015
 *      Author: sean
 */

#ifndef NAV_LEASTSQUAREPOS_H_
#define NAV_LEASTSQUAREPOS_H_

#include "initEph.h"
#include "customDataTypes.h"

/*!
 * \brief Function calculates the Least Square Solution
 * \return EXIT_SUCCESS on success. EXIT_FAILURE otherwise
 * \param pos - pointer to receiver position and receiver clock error
 * (in ECEF system: [X, Y, Z,dt])
 * \param el - pointer to array of Satellites elevation angles (degrees) assumed to be allocated before call
 * \param az - pointer to array of Satellites azimuth angles (degrees) assumed to be allocated before call
 * \param iono - pointer to iono delay for each satellite
 * \param trop - pointer to tropo delay for each satellite
 * \param dop - pointer to Dilutions of Precision ([GDOP PDOP HDOP VDOP TDOP])
 * \param satpos -Satellites positions (in ECEF system: [X; Y; Z;] -
 * one column per satellite)
 * \param nmbOfSatellites - Number of satellites. Exists as size for both obs and satpos
 * \param obs - Observations - the pseudorange measurements to each satellite
 * \param almanac - ionospheric model parameters
 * \param rxTime - raw receiver time in GPS time
 * \param activeChnList - channel list
 */
si32 leastSquarePos(
/* Output */
fl64 pos[4], fl64 * el, fl64 * az, fl64 * iono, fl64 * trop, fl64 dop[5],
/* Input */
fl64 ** satpos, si32 nmbOfSatellites, fl64 * obs, almanac_s almanac,
		fl64 rxTime, si32 * activeChnList);

#endif /* NAV_LEASTSQUAREPOS_H_ */
