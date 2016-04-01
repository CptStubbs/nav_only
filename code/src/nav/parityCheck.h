/*
 * parityCheck.h
 *
 *  Created on: May 13, 2015
 *      Author: sara
 */

#ifndef SRC_NAV_PARITYCHECK_H_
#define SRC_NAV_PARITYCHECK_H_
#include "customDataTypes.h"
/*! \brief Checks the parity of the supplied 30bit word.If the parity check is not
 passed, the program will be quit with error messages shown
 The last two parity bits of the previous word is used for the calculation.
 A note on the procedure is supplied by the GPS standard positioning
 service signal specification.
 Inputs:
 \param word        - an array with 30 bit long word from the navigation
 message (a character array, must contain only '0' or
 '1').
 \param D29Star     - the 29th bit of the previous word (char type).
 \param D30Star     - the 30th bit of the previous word (char type).

 Outputs:
 \param word        - word with corrected polarity of the data bits
 (character array).
 \param wordLength
 \param pass        - flag value to check if the parity check passes*/
si32 parityCheck(
/* Output */
si8 * word, si32 * pass,
/* Input */
si32 wordLength, si8 D29Star, si8 D30Star);
#endif /* SRC_NAV_PARITYCHECK_H_ */
