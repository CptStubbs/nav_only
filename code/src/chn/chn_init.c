#include "chn_init.h"
#include "calcLoopCoef.h"
#include "customDataTypes.h"
#include "stdlib.h"
#include "math.h"
#include "conf.h"														/// INTTIME
#include "conf_chn.h"												/// Multiple parameters
#include "conf_swc.h"
#include "stdio.h"
#include "chn_asn.h"

/// Calculate loop coefficients
void calcLoopCoef(fl64 lbw, fl64 zeta, fl64 k, fl64 * tau1, fl64 * tau2) {
	fl64 Wn = lbw * 8 * zeta / (4 * zeta * zeta + 1);
	*tau1 = k / (Wn * Wn);
	*tau2 = 2.0 * zeta / Wn;
	return;
}

/// Initialize acquisition parameters
void chn_init_acq(chn_acq_s * acq) {

		acq->carrFreqAcq = CARRFREQ;

}
/// Initialize tracking parameters
void chn_init_trk(chn_trk_s *trk, si32 m) {
	fl64 carrFreq[PRNLISTLENGTH] = MYPRNCARRFREQ;
	/*Not sure these other ones are necessary*/
	trk->carrBasis = (fl32) CARRFREQ;
	trk->codeTau1inv = (fl32) 14.317007; //	1/0.069847, [tau1,tau2]=calcLoopCoef(2.0Hz,0.7,1.0)
	trk->codeTau2 = (fl32) 0.370000; // 							[tau1,tau2]=calcLoopCoef(2.0Hz,0.7,1.0)
	trk->codePDI = (fl32) 0.001;
	trk->codeF1 = trk->codeTau2 * trk->codeTau1inv;
	trk->codeF2 = trk->codePDI * trk->codeTau1inv;
	trk->codeBasis = (fl32) 1.023e6;
	trk->sampCheck = 0;
	calcLoopCoef(DLLNOISEBW, DLLDAMPINGRATIO, 1, &trk->tau1code,
			&trk->tau2code);	//calculate the code coefficients
	calcLoopCoef(PLLNOISEBW, PLLDAMPINGRATIO, 0.25, &trk->tau1carr,
			&trk->tau2carr);	//calculate the code coefficients
	trk->carrBasis = (fl32) carrFreq[m];
	trk->codeBasis = (fl32) CODEFREQ;
	trk->codePDI = (fl32) PDICODE;
	trk->carrPDI = (fl32) PDICARR;
	trk->acqCodePhase = 0; //codePhase[m];
	trk->acqMode = MODE; //go directly into tracking
	trk->millisecs = 0; // millisecond counter
	trk->trackCnt = 0; // millisecond counter
	trk->notLockedCnt = 0;
	trk->lockTime = -200;//ms
	trk->VSM = 0;
	trk->myFreqErr = 0;
	trk->oldPI = 0;
	trk->oldPQ = 0;
	trk->absoluteSample =0;
	trk->dt =0;
	trk->rawDoppler = 0;
	trk->rawCarrPhase = 0;

}
//Need to remove this
/// Initialize lock indicator parameters
void chn_init_lck(chn_lck_s * lck) {
	lck->lockVsmNumber = (ui32) 300;
	lck->lockVsmThres = (fl32) pow((fl32) 10, (fl32) (30.0 / 10.0));/// 30dB <-> 1000 ...
	lck->carrThresOpt = (ui32) 1000;
	lck->carrThresPes = (ui32) 4800;
	lck->carrGain = (fl32) 0.0247;
	lck->carrDivr = (fl32) 1.5;
	lck->flseIntNum = (ui32) (((fl64) CHN_LCK_FALSEPHASEINTTIME)
			/ ((fl64) INTTIME)); /// Integration limit
	lck->flsePhaseLim = (fl32) CHN_LCK_FALSEPHASETHRESHOLD * M_PI / 180.0;
}
/// Initialize state variables (set all to zero)
void chn_init_ste(chn_ste_s *ste) {
	ste->carrNco = (fl64) 0.0;
	ste->oldCarrNco = (fl64) 0.0;
	ste->carrError = (fl64) 0.0;
	ste->oldCarrError = (fl64) 0.0;
	ste->codeNco = (fl64) 0.0;
	ste->oldCodeNco = (fl64) 0.0;
	ste->carrError = (fl64) 0.0;
	ste->oldCarrError = (fl64) 0.0;
}
/// Initialize assignment variables

void chn_init_asn(chn_asn_s * asn, ctr_s * ctr) {
	si32 inda = 0;
	asn->active = CHN_ASN_ACTIVE; /// All 32 prns are active (should be changed to true value)
	asn->visible = CHN_ASN_VISIBLE;	/// All 32 prns are visible (until we know better)
	//asn->assigned = 0x0;											/// Prns that are tracked by a channel
	asn->numSats = 32;/// Maximum number of satellites in the constellation (32 for GPS etc.)
	asn->ind = 0;									/// Last prn to be assigned
	asn->lastCheck = 0;
	for (inda = 0; inda < 32; inda++) {
		if (inda < CONF_NUMCHN) {
			asn->trackSats[inda] = 0;

			ctr->blks[0].chns[inda].prn = asn->lastCheck; //assign prn 0 to the first channel
			asn->lastCheck = asn->lastCheck + 1; //increase lastCheck
			asn->lastCheck = asn->lastCheck % 32; // make sure it only goes between 0 and 31.
		} else {
			asn->trackSats[inda] = 1;
		}
	}
	asn->lastCheck = asn->lastCheck - 1; //decrease lastCheck
	asn->lastCheck = asn->lastCheck % 32; // make sure it only goes between 0 and 31.
	return;
}

/// Initialize chn fields
si32 chn_init(chn_s * chn, res_s * res, ctr_s * ctr, apt_s * apt) {
	ui32 m;

	for (m = 0; m < CONF_NUMCHN; m++) {
		chn_init_acq(&chn->acq[m]);
		chn_init_trk(&chn->trk[m], m);
		chn_init_lck(&chn->lck[m]);
		chn_init_ste(&chn->ste[m]);
		chn->cnd[m] = 0;
	}

	chn->res = res;
	chn->ctr = ctr;
	chn_init_asn(&chn->asn, chn->ctr); // has it's own initialization.
	/* Init apt*/
	apt->indRead = 0;
	apt->indWrite = 0;
	for (m = 0; m < APT_NUM; m++) {
		memset(&apt->emt[m], '\0', sizeof(apt_emt_s));
	}
	chn->apt = apt;
	chn->m = 0;

#if LOGCOR
	chn->binTrkFid = fopen(DATAFILEOUT, "wb");
	ERRORCHECK(chn->binTrkFid == NULL);
	writeHeader(chn->binTrkFid);
#endif


	return EXIT_SUCCESS;
}

si32 chn_free(chn_s * chn) {
	(void) chn;									/// Suppress compiler warning

#if LOGCOR
	fclose(chn->binTrkFid);
#endif


	return EXIT_SUCCESS;
}
