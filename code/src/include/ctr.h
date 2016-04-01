/**
 * @file 			ctr.h
 * @brief 		Data structure and init/free functions for "ConTRol"
 * @author 		staffan
 * @date 			09-Nov-2015
**/

#include "customDataTypes.h"													/// Data types
#include "conf.h"																			/// NUMCPB, NUMBLK

#ifndef SRC_INCLUDE_COR_H_
	#define SRC_INCLUDE_COR_H_

	/// Define control values: prn numbers, frequencies etc.
	/// Implemented as an X-macro so we can iterate/generate header automatically
	#define CTR_FIELDS \
		X(ui32, 	"%u", 		prn) \
		X(bool, 	"%d",			newPrn) \
		X(ui32, 	"%u",			blkSize) \
		X(fl64, 	"%f",			carrFreq) \
		X(fl64, 	"%f",			codeFreq) \
		X(bool, 	"%d",			newFreq) \
		X(fl64, 	"%f",			remCarrPhase) \
		X(fl64, 	"%f",			remCodePhase) \
		X(ui32, 	"%u",			startSample)\
		X(ui32, 	"%u",			startSampleChn)\
		X(ui32, 	"%u",			howMuchLeft)// how much data should be left to use if the buffer is not enough
	/// TODO - newPrn should be moved to thr
	/// TODO - newFreq should be moved to thr
	/// TODO - howMuchLeft should be removed
	/// TODO - renaming fields

	/**
	 * @brief							One channel in "ConTRol", implemented using the X-macro above
	 */
	typedef struct {
		/// Standard fields
		#define X(type,format,name) type name;
			CTR_FIELDS
		#undef X
			/// Temporary fields - TODO - remove these fields
			DATATYPE *inputData;			// Input data
	} ctr_cha_s;

	/**
	 * @brief							One block in "ConTRol" is an array of channels
	**/
	typedef struct {
		ctr_cha_s 					chns[CONF_NUMCPB];
	} ctr_blk_s;

	/**
	 * @brief							Top level of "ConTRol" is an array of blocks that is an array of channels
	 */
	typedef struct {
		ctr_blk_s 					blks[CONF_NUMBLK];
	} ctr_s;

	/**
	 * @brief							Initialize the shared data structure "ConTRol"
	 * @param[inout]			ctr 		Pointer to ctr_s
	 * @return						0 if successful
	 */
	int ctr_init(ctr_s * ctr);

	/**
	 * @brief							Clean up the shared data structure "ConTRol"
	 * @param[inout]			ctr 		Pointer to ctr_s
	 * @return						0 if successful
	 */
	int ctr_free(ctr_s * ctr);

#endif /* SRC_INCLUDE_COR_H_ */
