/*
 * rinexNavHeader.h
 *
 *  Created on: May 12, 2015
 *      Author: sean
 */

#ifndef NAV_RINEXNAVHEADER_H_
#define NAV_RINEXNAVHEADER_H_

#include <stdio.h>

#include "initEph.h"

/*!
 * \brief Write rinex nav file header
 * \return EXIT_SUCCESS on success. EXIT_FAILURE otherwise. Will perror reason
 * \param fid - file ID
 * \param almanac - Iono and UTC parameters
 */

si32 rinexNavHeader(
/* Output */
/* Input */
FILE * fid, almanac_s almanac);

#endif /* NAV_RINEXNAVHEADER_H_ */
