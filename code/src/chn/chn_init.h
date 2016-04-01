#ifndef __CHN_INIT_H

#define __CHN_INIT_H
#include "customDataTypes.h"
#include "res.h"
#include "ctr.h"
#include "chn.h"

/**
 * \brief Initialization of the chn_s data structure
 * \param [out] 			chn									channel data structure
 * \param [in] 				ccr 								Correlation control and results (shared)
 * \param [in] 				apt 								Amplitude, phase and time\
 * \param [in] 				swc 								Software structure
 */
si32 chn_init(chn_s * chn, res_s * res, ctr_s * ctr, apt_s * apt);

/**
 * \brief Freeing any dynamically allocated data from the chn_s data structure
 * \param [out] 			chn									channel data structure
 */
si32 chn_free(chn_s * chn);
void chn_init_acq(chn_acq_s * acq);
void chn_init_trk(chn_trk_s *trk, si32 m);
void chn_init_ste(chn_ste_s *ste);
void chn_init_asn(chn_asn_s * asn, ctr_s * ctr);

#endif
