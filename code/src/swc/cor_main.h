/**
 * @file 			cor_main.h
 * @brief 		Main function for the "SoftWare Correlator"
 * @author 		sean
 * @date 			23-Jul-2015
**/

#ifndef SRC_SWC_COR_MAIN_H_
	#define SRC_SWC_COR_MAIN_H_
	#include "cor_type.h"

	/**
	 * @brief							Main function for the "SoftWare Correlator"
	 * @param[inout]			cor 			Pointer to cor_s cast to a void pointed (required by pthread)
	 * @return											Void pointer (required by pthread)
	 */
	void * cor_main(void *  cor);

#endif /* SRC_SWC_COR_MAIN_H_ */
