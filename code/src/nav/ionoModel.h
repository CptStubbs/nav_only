/*
 * ionoModel.h
 *
 *  Created on: May 11, 2015
 *      Author: sean
 */

#ifndef IONOMODEL_H_
#define IONOMODEL_H_

#include "initEph.h"
#include "customDataTypes.h"

/*!
 * \brief Calculation of an Ionospheric range correction for the GPS L1 frequency
 * from the parameters broadcasted in the GPS Navigation Message
 * (Klobuchar model).
 * \param iono - pointer to ionospheric error of the satellite (seconds)
 * \param almanac - ionospheric correction parameters from almanac of the satellites
 * \param az - Geodetic azimuth of the satellite (degrees)
 * \param el - Elevation angle of the satellite (degrees)
 * \param TOW - GPS time for the receiver
 * \param pos - ECEF coordinate for the receiver
 */
void ionoModel(
/* Output */
fl64 * iono,
/* Input */
almanac_s almanac, fl64 az, fl64 el, fl64 TOW, fl64 pos[3]);

#endif /* IONOMODEL_H_ */
