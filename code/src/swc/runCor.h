/*
 * runCor.h
 *
 *  Created on: Jul 22, 2015
 *      Author: user
 */

#ifndef SRC_SWC_RUNCOR_H_

#define SRC_SWC_RUNCOR_H_

#include "softFrontEnd.h"
#include "conf_chn.h"
#include "cor.h"
#include "customDataTypes.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <conf_nav.h>
/*! \brief This function contains everything that soft correlator does. It gets inputs from ccr and gives outputs to the ccr.

 Inputs:
 \param ccr_s	  - pointer to the input and output struct ccr*/
si32 runCor(res_s * ctr, ctr_s * res, si32 chnNr, si8* sinArray, si8* cosArray,
		cat_s* pStart[32], cor_s * cor);

#endif /* SRC_SWC_RUNCOR_H_ */
