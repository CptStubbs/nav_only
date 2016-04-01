/**
 * @file 			res.h
 * @brief 		Data structure and init/free functions for "RESults"
 * @author 		staffan
 * @date 			09-Nov-2015
 **/

#include "customDataTypes.h"													/// si32, bool
#include "conf.h"																			/// NUMCPB, NUMBLK

#ifndef SRC_INCLUDE_RES_H_
#define SRC_INCLUDE_RES_H_

/// Define correlator values: Early/Prompt/Late,Inphase/Quadrature
/// Implemented as an X-macro so we can iterate/generate header automatically
#define RES_FIELDS \
		X(si32, 	EI) \
		X(si32, 	EQ) \
		X(si32, 	PI) \
		X(si32, 	PQ) \
		X(si32, 	LI) \
		X(si32, 	LQ) \
		X(si32, 	L2EI) \
		X(si32, 	L2EQ) \
		X(si32, 	L2PI) \
		X(si32, 	L2PQ) \
		X(si32, 	L2LI) \
		X(si32, 	L2LQ) \
		X(bool, 	newCorr)
/// TODO - newCorr should be moved to thr

/**
 * @brief							One channel in "RESults", implemented using the X-macro above
 */
typedef struct {
#define X(type,name) type name;
	RES_FIELDS
#undef X
} res_cha_s;

/**
 * @brief							One block in "RESults" is an array of channels
 */
typedef struct {
	res_cha_s chns[CONF_NUMCPB];
} res_blk_s;

/**
 * @brief							Top level of "RESults" is an array of blocks that is an array of channels
 */
typedef struct {
	res_blk_s blks[CONF_NUMBLK];
} res_s;

/**
 * @brief							Initialize the shared data structure "RESults"
 * @param[inout]			res 		Pointer to res_s
 * @return						0 if successful
 */
int res_init(res_s * res);

/**
 * @brief							Clean up the shared data structure "RESults"
 * @param[inout]			res 		Pointer to res_s
 * @return						0 if successful
 */
int res_free(res_s * res);

#endif /* SRC_INCLUDE_RES_H_ */

