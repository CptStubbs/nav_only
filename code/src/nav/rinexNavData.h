/*
 * rinexNavData.h
 *
 *  Created on: May 12, 2015
 *      Author: sean
 */

#ifndef NAV_RINEXNAVDATA_H_
#define NAV_RINEXNAVDATA_H_

#include <stdio.h>
#include "customDataTypes.h"
#include "rinexDataStruct.h"

/*!
 * \brief Write rinex nav file
 * \return EXIT_SUCCESS on success. EXIT_FAILURE otherwise. Will perror reason
 * \param fid - file ID
 * \param rinexData - Rinex data structure
 */

si32 rinexNavDate(
/*Output */
/* Input */
FILE * fid, rinexData_s rinexData);

#endif /* NAV_RINEXNAVDATA_H_ */
