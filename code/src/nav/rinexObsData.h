/*
 * rinexObsData.h
 *
 *  Created on: May 12, 2015
 *      Author: sean
 */

#ifndef NAV_RINEXOBSDATA_H_
#define NAV_RINEXOBSDATA_H_

#include <stdio.h>
#include "rinexDataStruct.h"

/*!
 * \brief Write rinex obs file
 * \return EXIT_SUCCESS on success. EXIT_FAILURE otherwise. Will perror reason
 * \param fid - file ID
 * \param rinexData - Rinex data structure
 */
si32 rinexObsData(
/*Output*/
/*Input */
FILE * fid, rinexData_s rinexData);

#endif /* NAV_RINEXOBSDATA_H_ */
