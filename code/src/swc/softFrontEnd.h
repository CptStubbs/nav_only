/*
 * softFrontEnd.h
 *
 *  Created on: Jul 22, 2015
 *      Author: Sara
 */

#ifndef SRC_SWC_SOFTFRONTEND_H_
#define SRC_SWC_SOFTFRONTEND_H_

#include "conf_chn.h"
#include "conf.h"
#include "customDataTypes.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conf_nav.h>



/*! \brief This function is called to act as a software front-end.
 * It reads data from either a file or a USB3 interface, stores it in a
 * buffer and sends it to main.
 */
si32 softFrontEnd /* Output */
(DATATYPE* buffer, /*Input */
si32 bufLength,FILE * ptr_file);

/*! \brief This is a subfunction of the main front end, dedicated for file reading
 Inputs:
 \param ptr_file	  - pointer to the file
 \param buflength	  - how long is 1 ms
 Outputs:
 \param buffer   - 3 ms buffer of input data
 */

si32 softFrontEndFile(/* Output */
DATATYPE * buffer,  /* INPUT */
si32 bufLength, FILE * ptr_file);


/*! \brief This is a subfunction of the main front end, dedicated for standard in reading
 Inputs:
 \param buflength	  - how long is 1 ms
 Outputs:
 \param buffer   - 3 ms buffer of input data
 */ 

si32 softFrontEndStdin(/* Output */
DATATYPE * buffer,  /* INPUT */
si32 bufLength);

#endif /* SRC_SWC_SOFTFRONTEND_H_ */
