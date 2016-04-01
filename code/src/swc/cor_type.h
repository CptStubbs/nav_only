/**
 * @file 			cor_type.h
 * @brief 		Data structure for "CORrelation"
 * @author 		staffan
 * @date 			09-Nov-2015
**/

#include "customDataTypes.h"													/// si8, etc.
#include <stdio.h>																		/// FILE,
#include "res.h"																			/// res_s, (shared data structure "RESults")
#include "ctr.h"																			/// ctr_s, (shared data structure "ConTRol")

#ifndef SRC_SWC_COR_TYPE_H_
	#define SRC_SWC_COR_TYPE_H_

	/**
	 * @brief		C/A code tables. Not sure why implemented as a double linked list?
	 */
	typedef struct caListtag {
		struct caListtag* pNext, *pLast;									/// Pointer to next and last element in this list
		signed char nCurrentNext[2];											/// Counter for the next 2(!) elements
		signed char nCurrentLast[2];											/// Counter for the last 2(!) elements
	} cat_s;

	/**
	 * @brief		Data structure for "CORrelation"
	 */
	typedef struct {
		res_s * res;																			/// Pointer to the shared data structure "RESults"
		ctr_s * ctr;																			/// Pointer to the shared data structure "ConTRol"
		si8 sinArray[256];																/// Sine LUT
		si8 cosArray[256];																/// Cosine LUT
		FILE *ptr_file; 																	/// Pointer to data file
		FILE *l2ptr_file; 																	/// Pointer to LC2 data file
		ui64 endFile;																			/// ?
		ui64 currentFile;																	/// ?
		si32 msFlag;																			/// ?
		DATATYPE inputDataBuffer[2][NUMSAMPLEINBUFF];					/// IF data buffer
		DATATYPE l2inputDataBuffer[2][NUMSAMPLEINBUFF];					/// IF data buffer
		DATATYPE * inputData;  														/// Pointer to the input data
		DATATYPE * l2inputData;  														/// Pointer to the input data
		si8 corDone;																			/// FLag set high (?) when done (end of file?)
		si8 L2CDone;																			/// FLag set high (?) when done (end of file?)
		cat_s *cat[32];																		/// C/A code table
		cat_s *cst[32];																		/// C/S code table
		si32 myMode;																			/// ?
		si32 chnNr; 																			/// channel index
		bool readData;																		/// Indicates that data needs to be read
		bool readL2Data;																		/// Indicates that data needs to be read
	} cor_s;

#endif /* SRC_SWC_COR_TYPE_H_ */
