/*
 * log_explode.h
 *
 *  Created on: Oct 7, 2015
 *      Author: staffan
 */

#include <stdio.h>
#include <customDataTypes.h>

#ifndef SRC_INCLUDE_LOG_EXPLODE_H_
	#define SRC_INCLUDE_LOG_EXPLODE_H_


/**
	 * \brief Explodes all log files in LOG_PATH
	 * \return																EXIT_SUCCESS or EXIT_FAILURE
	**/
	int log_explodeMain();

	/**
	 * \brief Opens a log file named LOG_PATH/fileNameBase_name.type for read or write
	 * \param [out] 			ret 								return value - adds 0 if the file opened correctly
	 * \param	[out]				fd									file descriptor
	 * \param [in]				fileNameBase 				(typically the same as the log)
	 * \param [in]				name 								name of field
	 * \param [in]				type 								name of type
	 * \param [in]				mode 								read "r" or write "w"
	**/
	void fopenLogs(ui32 * ret, FILE ** fd, char * fileNameBase, char * name, char * type, char * mode);

	/**
	 * \brief Deletes log files in LOG_PATH/"fileNameBase"*
	 * \param [in]				fileNameBase 				(typically the same as the log)
	 * \return																EXIT_SUCCESS or EXIT_FAILURE
	**/
	int purgeLogFiles(char * fileNameBase);

#endif /* SRC_INCLUDE_LOG_EXPLODE_H_ */
