/*
 * rinexObsHeader.h
 *
 *  Created on: May 12, 2015
 *      Author: sean
 */

#ifndef NAV_RINEXOBSHEADER_H_
#define NAV_RINEXOBSHEADER_H_

#include <stdio.h>
#include "rinexDataStruct.h"
#include "initSettings.h"

/*!
 * \brief Write rinex obs file header
 * \return EXIT_SUCCESS on success. EXIT_FAILURE otherwise. Will perror reason
 * \param fid - file ID
 * \param rinexData - Rinex data structure
 * \param settings - receiver settings
 */
si32 rinexObsHeader(
/* Output */
/* Input */
FILE * fid, rinexData_s rinexData, settings_s settings);

#endif /* NAV_RINEXOBSHEADER_H_ */
