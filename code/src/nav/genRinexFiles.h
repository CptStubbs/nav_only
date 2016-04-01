/*
 * genRinexFiles.h
 *
 *  Created on: May 12, 2015
 *      Author: sean
 */

#ifndef NAV_GENRINEXFILES_H_
#define NAV_GENRINEXFILES_H_

#include "customDataTypes.h"
#include "initSettings.h"
#include "rinexDataStruct.h"
#include "initState.h"

/*!
 * \brief Generate Rinex files
 * \return EXIT_SUCCESS on success. EXIT_FAILURE otherwise. Will perror reason
 * \param state - pointer to the current state of the receiver
 * \param rinexData - Rinex data structure
 * \param settings - receiver settings
 */
si32 genRinexFiles(
/* Output */
state_s * state,
/* Input */
rinexData_s rinexData, settings_s settings,
);

#endif /* NAV_GENRINEXFILES_H_ */
