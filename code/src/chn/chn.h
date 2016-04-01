#ifndef __CHN_H

#define __CHN_H

#include "customDataTypes.h"
#include "res.h"
#include "ctr.h"
#include "apt.h"
#include "conf.h"
#include "conf_chn.h"
#include "apt.h"
#include "conf.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <memory.h>
#include <math.h>


typedef struct {	/// Acquisition parameters
	/// Carrier frequency
	fl64 carrFreqAcq;
	fl32 carrFreqStart;
	fl32 carrFreqStop;
	fl32 carrFreqStep;
	/// Code frequency
	fl32 codeFreqStart;
	fl32 codeFreqStop;
	fl32 codeFreqStep;
	///	Code slew
	ui32 codeSlewStart;
	ui32 codeSlewStop;
	ui32 codeSlewStep;
	/// Detection threshold
	ui64 peakThreshold;
} chn_acq_s;

typedef struct {	/// Tracking parameters
	/// Carrier tracking
	fl32 carrTau1inv;
	fl32 carrTau2;
	fl32 carrPDI;
	fl32 carrF1;
	fl32 carrF2;
	fl32 carrBasis;
	/// Code tracking
	fl32 codeTau1inv;
	fl32 codeTau2;
	fl32 codePDI;
	fl32 codeF1;
	fl32 codeF2;
	fl32 codeBasis;

	/*--------------------*/
	fl64 tau1code;
	fl64 tau2code;
	fl64 tau1carr;
	fl64 tau2carr;


	si32 acqMode;// used to determine if we still are in acquisition mode or not
	si32 acqCodePhase;
	fl64 millisecs;	// This is used to count how many milliseconds we have processed per channel and is used for VSM calulation.
	fl64 trackCnt; // millisecond counter
	ui32 notLockedCnt;// Used to determine how many sucessive 200ms we have been not locked for
	fl64 lockTime;
	fl64 VSM;
	fl64 myFreqErr;
	si32 oldPI;
	si32 oldPQ;
	ui64 absoluteSample;
	fl32 absSamFrac;			// the fractional part of absoluteSample.
	fl64 samNum;				//sample number
	fl64 sampCheck;
	si32 Ivec[VSMINT];	// vector of 200 I, for VSM calculation
	si32 Qvec[VSMINT];	// vector of 200 Q, for VSM calculation
	fl64 CarrErrorVec[VSMINT];
	fl64 dt;
	fl64 rawDoppler;
	fl64 rawCarrPhase;


} chn_trk_s;

typedef struct {		/// Channel state parameters
	/// Carrier tracking
	fl64 carrNco;		/// Carrier NCO
	fl64 oldCarrNco;		/// Carrier NCO
	fl64 carrError;		/// Carrier tracking error
	fl64 oldCarrError;		/// Carrier tracking error
	/// Code tracking
	fl64 codeNco;		/// code NCO
	fl64 oldCodeNco;		/// code NCO
	fl64 codeError;		/// Code tracking error
	fl64 oldCodeError;		/// Code tracking error
	ui32 codeSlew;		/// Code slew
	/// Carrier lock indicator
	fl32 carrLckI_F;	/// Inphase filter value (for carrier lock indicator)
	fl32 carrLckQ_F;	/// Quadrature filter value (for carrier lock indicator)
	ui32 carrLckOpt;	/// Optimistic lock counter
	ui32 carrLckPes;	/// Pessimistic lock counter
	/// Code lock indicator
	fl32 codeLckPm;	/// Mean power estimate
	fl32 codeLckM2;	/// Square diff. from Pm
	ui32 codeLckCnt;	/// Counter (n)
	fl32 codeLckEst;	/// Last VSM estimate
	ui64 codeLckAge;	/// FPGA counter at last VSM computation
	/// False phase lock indicator
	fl32 flseLckI_S;			/// Inphase sum
	fl32 flseLckQ_S;			/// Quadrature sum
	ui32 flseCnt;					/// Counter
} chn_ste_s;

typedef enum { 	/// Conditional bits
	e_chn_cnd_found = 1,
	e_chn_cnd_done = 2,
	e_chn_cnd_visible = 4,
	e_chn_cnd_cdelck = 8,								/// True is good
	e_chn_cnd_caropt = 16,								/// True is good
	e_chn_cnd_carpes = 32,								/// True is good
	e_chn_cnd_fsephs = 64								/// True is bad
} chn_cnd_e;

typedef struct {
	ui64 active;					/// Is this prn active (launched, healthy)
	ui64 visible;										/// Is this prn visible
	ui64 assigned;					/// Have this prn been assigned to a channel
	ui8 numSats;										/// Number of satellites
	ui8 ind;							/// index of last/next prn assignment
	ui32 lastCheck;
	ui32 trackSats[32];
} chn_asn_s;

/**
 * \brief Main thread for chn
 * \param [in,out] 		param								channel data structure of type chn_s
 */
void * chn_main(void * param);

typedef struct {	/// Lock detector parameters
	/// VSM parameters
	ui32 lockVsmNumber;		/// Number of integrations per VSM dump
	fl32 lockVsmThres;		/// VSM threshold for code lock
	/// Optimistic/pessimistic carrier detector
	ui32 carrThresOpt; /// optimistic threshold for lock (T_o, 50*20ms in Kaplan)
	ui32 carrThresPes; /// pessimistic threshold for lock (T_p, 240*20ms in Kaplan)
	fl32 carrGain;				/// LPF Gain  (K1 in Kaplan, 0.0247)
	fl32 carrDivr;				/// Inphase divisor (K2 in Kaplan, 1.5)
	///False phase lock detector
	ui32 flseIntNum;			/// Number of integrations
	fl32 flsePhaseLim;		/// Limit in radians
} chn_lck_s;

typedef struct {
	chn_acq_s acq[CONF_NUMCHN];						/// Acquisition parameters
	chn_trk_s trk[CONF_NUMCHN];						/// Tracking parameters
	chn_lck_s lck[CONF_NUMCHN];		/// Code/Carrier lock detector parameters
	chn_ste_s ste[CONF_NUMCHN];						/// State variables
	chn_cnd_e cnd[CONF_NUMCHN];						///	Conditional
	chn_asn_s asn;									/// Shared assignment struct
	apt_s * apt;																				/// Amplitude, phase and time
	res_s * res;																				/// Shared structure "RESults"
	ctr_s * ctr;																				/// Shared structure "ConTRol"
	FILE * ptr_file;
	FILE *binTrkFid; 																		// Define IQ datafile info
	si32 m;																							//counter to keep track of what channel we are processing.

} chn_s;
/**
 * \brief Main thread for chn
 * \param [in,out] 		param								channel data structure of type chn_s
 */
void * chn_main(void * param);
void closeLoop(chn_ste_s *ste, ctr_cha_s *ctr, res_cha_s *res, chn_trk_s *trk,
		chn_acq_s *acq);
#endif
