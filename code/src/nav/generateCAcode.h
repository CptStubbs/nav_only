/*
 * generateCAcode.h
 *
 *  Created on: May 13, 2015
 *      Author: sara
 */

#ifndef SRC_NAV_GENERATECACODE_H_
#define SRC_NAV_GENERATECACODE_H_
#include "customDataTypes.h"
/*! \brief Function to generate any of the 1-37 (GPS) C/A codes, 120-158 (GPS-SBAS)
 C/A codes, or GLONASS code

 CAcode = generateCAcode(PRN)

 Inputs:
 \param PRN         - sat id number (GLONASS = 0; PRN = 1-37,120-158)

 Outputs:
 \param CAcode      - a vector containing the desired C/A code sequence
 (chips).*/
void generateCAcode(
/* Output */
fl64 * CAcode,
/* Input */
si32 PRN);
#endif /* SRC_NAV_GENERATECACODE_H_ */
