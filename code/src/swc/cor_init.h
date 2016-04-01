/**
 * @file 			cor_init.h
 * @brief 		Initializes the swc data structure
 * @author 		sean
 * @date 			23-Jul-2015
**/

#include "cor_type.h"																	/// cor_s,
#include "res.h"																			/// res_s,
#include "ctr.h"																			/// ctr_s,

#ifndef SRC_SWC_COR_INIT_H_
	#define SRC_SWC_INITCOR_H_

	/**
	 * @brief							Initialize the data structure for "CORrelation"
	 * @param[inout]			cor 		Pointer to cor_s
	 * @param[in]					res			Pointer to res_s
	 * @param[in]					ctr			Pointer to ctr_s
	 * @return						0 if successful
	 */
	int cor_init(cor_s * cor, res_s * res, ctr_s * ctr);

#endif /* SRC_SWC_COR_INIT_H_ */
