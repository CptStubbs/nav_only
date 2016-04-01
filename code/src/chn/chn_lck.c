/**
 * This file contains the functions related to code and
 * carrier lock. The code lock detector is a simple C/No
 * using the VSM, and the carrier lock detector is from
 * Kaplan & Hegarty. We also have a false phase lock indicator.
 *
 **/

#include "chn_lck.h"
#include "customDataTypes.h"								/// Data types
#include "math.h"														/// sqrt
#include <stdlib.h>													/// abs
#include <stdio.h>
#include "conf_chn.h"

/// Code lock detector (C/No using VSM)
void codeLockDetector(si32 pi, si32 pq, chn_lck_s * lck, chn_trk_s *trk,
		chn_ste_s * ste, chn_cnd_e *cnd) {

	fl32 tmp, delta, Pv, Pc, Pe;
	tmp = (((fl32) pi) * ((fl32) pi) + ((fl32) pq) * ((fl32) pq)); /// P
	delta = tmp - ste->codeLckPm;												/// x
	ste->codeLckCnt++;																	/// Increase counter
	ste->codeLckPm += delta / ((fl32) ste->codeLckCnt);	/// Update Pm estimate
	ste->codeLckM2 += delta * (tmp - ste->codeLckPm);		/// Update M2 estimate

	if (ste->codeLckCnt >= lck->lockVsmNumber) {				/// If we should update C/No
		Pv = ste->codeLckM2 / (ste->codeLckCnt - 1);			/// Variance of P
		tmp = ste->codeLckPm * ste->codeLckPm;						/// Pm^2
		Pc = sqrt(fabs(tmp - Pv));												/// Carrier power
		tmp = Pc / (ste->codeLckPm - Pc);									/// SNR estimate (linear form)
		Pe = (1.0 / trk->codePDI) * fabs(tmp);						/// C/No estimate (linear form)

		ste->codeLckEst = Pe;															/// Update C/No estimate
		ste->codeLckAge++;																/// Change to actual cycle number
		ste->codeLckPm = (fl32) 0.0;											/// Reset Pm estimate
		ste->codeLckM2 = (fl32) 0.0;											/// Reset M2 estimate
		ste->codeLckCnt = (fl32) 0.0;											/// Reset counter

		if (Pe > lck->lockVsmThres) {											/// We have code lock if above threshold
			*cnd = *cnd | e_chn_cnd_cdelck;									/// Set code lock to true
		} else {
			*cnd = *cnd & ~e_chn_cnd_cdelck;								/// Set code lock to false
		}
	}
	return;
}

/// ---------------------------- Carrier lock detector ----------------------------
void carrLockDetector(si32 pi, si32 pq, chn_lck_s *lck, chn_trk_s *trk,
		chn_ste_s *ste, chn_cnd_e *cnd) {
	(void) trk;					/// Suppress compiler warning, trk not used yet
	fl32 afi, afq;
	afi = fabs((fl32) pi);
	afq = fabs((fl32) pq);
	/// Filter inputs
	ste->carrLckI_F = lck->carrGain * (afi)
			+ (1 - lck->carrGain) * ste->carrLckI_F;
	ste->carrLckQ_F = lck->carrGain * (afq)
			+ (1 - lck->carrGain) * ste->carrLckQ_F;
	///
	if ((ste->carrLckI_F / lck->carrDivr) > ste->carrLckQ_F) {	/// If I>>Q
		*cnd = *cnd | e_chn_cnd_caropt;			/// Set optimistic lock to true
		ste->carrLckOpt = 0;					/// Set optimistic counter to 0
		if (ste->carrLckPes > lck->carrThresPes) {/// If pessimistic counter > threshold
			*cnd = *cnd | e_chn_cnd_carpes;		/// Set pessimistic lock true
		} else {						/// If pessimistic counter !> threshold
			ste->carrLckPes++;					/// Increase pessimistic counter
		}
	} else {													/// if !(I>>Q)
		*cnd = *cnd & ~e_chn_cnd_carpes;		/// Set pessimistic lock false
		ste->carrLckPes = 0;					/// Set pessimistic counter to 0
		if (ste->carrLckOpt > lck->carrThresOpt) {/// If optimistic counter > threshold
			*cnd = *cnd & ~e_chn_cnd_caropt;	/// Set optimistic lock false
		} else {						/// If optimistic counter !> threshold
			ste->carrLckOpt++;					/// Increase optimistic counter
		}
	}
}

void falsePhaseLockDetector(si32 pi, si32 pq, chn_lck_s *lck, chn_trk_s *trk, chn_ste_s *ste, chn_cnd_e *cnd){
	(void) trk;																/// Suppress compiler warning, trk not used yet
	/// Add pi and pq to the sums
	ste->flseLckI_S += (fl32) pi;
	ste->flseLckQ_S += (fl32) pq;
	if (ste->flseCnt >= lck->flseIntNum){
		/// Compute four quadrant arctangent
		fl64 res = atan2(ste->flseLckQ_S, ste->flseLckI_S);
//		printf("angle: %f (%f)\n",res,lck->flsePhaseLim);
		/// Check if above limit
		if (fabs(res) > lck->flsePhaseLim){
			*cnd = *cnd | e_chn_cnd_fsephs;				/// Set false phase lock true
		} else {
			*cnd = *cnd & ~e_chn_cnd_fsephs;			/// Set false phase lock false
		}
		/// Reset all values
		ste->flseLckI_S = 0.0;
		ste->flseLckQ_S = 0.0;
		ste->flseCnt = 0;
	}
	ste->flseCnt += 1;
}


