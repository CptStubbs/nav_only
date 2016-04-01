/**
 * @file 			cor_free.h
 * @brief 		Clean up data structure for "CORrelation"
 * @author 		staffan
 * @date 			16-Dec-2015
**/

#include "cor_type.h"																	/// cor_s,

#ifndef SRC_SWC_COR_FREE_H_
	#define SRC_SWC_COR_FREE_H_

	/**
	 * @brief							Initialize the data structure for "CORrelation"
	 * @param[inout]			cor 		Pointer to cor_s
	 * @return						0 if successful
	 */
	int cor_free(cor_s * cor);

#endif /* SRC_SWC_COR_FREE_H_ */
