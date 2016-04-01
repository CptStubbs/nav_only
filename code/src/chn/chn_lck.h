#ifndef __CHN_LCK_H
	#define __CHN_LCK_H
	#include "chn.h"

	/**
	 * \brief Code lock detector using VSM - online implementation
	 * \param [in] 				pi 									prompt inphase
	 * \param [in] 				pq 									prompt quadrature
	 * \param [in]				lck 								lock parameters TODO move to conf
	 * \param [in] 				trk 								tracking parameters (derived from BW)
	 * \param [in,out]		ste 								state variables
	 * \param [in,out]		cnd 								conditional (locked or not)
	 */
	void codeLockDetector(
			si32 pi,
			si32 pq,
			chn_lck_s * lck,
			chn_trk_s * trk,
			chn_ste_s * ste,
			chn_cnd_e * cnd
			);

	/**
	 * \brief Carrier lock detector using optimistic/pessimistic detector from Kaplan
	 * \param [in]				pi									prompt inphase
	 * \param [in] 				pq 									prompt quadrature
	 * \param [in]				lck 								lock parameters TODO move to conf
	 * \param [in] 				trk 								tracking parameters (derived from BW)
	 * \param [in,out]		ste 								state variables
	 * \param [in,out]		cnd 								conditional (locked or not)
	 */
	void carrLockDetector(
			si32 pi,
			si32 pq,
			chn_lck_s * lck,
			chn_trk_s * trk,
			chn_ste_s * ste,
			chn_cnd_e * cnd
			);

	/**
	 * \brief False phase lock detector from Kaplan
	 * \param [in]				pi									prompt inphase
	 * \param [in] 				pq 									prompt quadrature
	 * \param [in]				lck 								lock parameters TODO move to conf
	 * \param [in] 				trk 								tracking parameters (derived from BW)
	 * \param [in,out]		ste 								state variables
	 * \param [in,out]		cnd 								conditional (locked or not)
	 */
	void falsePhaseLockDetector(
			si32 pi,
			si32 pq,
			chn_lck_s * lck,
			chn_trk_s * trk,
			chn_ste_s * ste,
			chn_cnd_e * cnd
			);
#endif
