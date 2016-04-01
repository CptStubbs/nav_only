#include "conf_chn.h"
#include "chn.h"

#include <conf_nav.h>

#include "chn_asn.h"
#include "VSM.h"
#include "PRM.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "chn_init.h"

/// ---------------------------- Tracking loop ----------------------------
void closeLoop(chn_ste_s *ste, ctr_cha_s *ctr, res_cha_s *res, chn_trk_s *trk,
		chn_acq_s *acq) {

	/*------------------------------------------------------------*/
	//tracking mode
	ste->codeError = (sqrt((fl64) pow(res->EI, 2) + (fl64) pow(res->EQ, 2))
			- sqrt((fl64) pow(res->LI, 2) + (fl64) pow(res->LQ, 2)))
			/ (sqrt((fl64) pow(res->EI, 2) + (fl64) pow(res->EQ, 2))
					+ sqrt((fl64) pow(res->LI, 2) + (fl64) pow(res->LQ, 2)));

	ste->codeNco =
			ste->oldCodeNco
					+ trk->tau2code / trk->tau1code
							* (ste->codeError - ste->oldCodeError)+ ste->codeError / trk->tau1code * PDICODE;
	ste->oldCodeError = ste->codeError;
	ste->oldCodeNco = ste->codeNco;
	ctr->codeFreq = MYPRNCODEFREQ - ste->codeNco;

	//Carrier Freq/Phase Tracking
	if (trk->acqMode < 1001) {  //freq pullin for 1 sec
		ste->carrError = 1000 / 360 * 180 / M_PI
				* atan2(
						(fl64) res->PI * trk->oldPQ
								- (fl64) res->PQ * trk->oldPI,
						(fl64) res->PI * trk->oldPI
								+ (fl64) res->PQ * trk->oldPQ);
		//1.45 is a fudge factor, not sure why it is needed or where that value comes from based on Ward chapter and eq above, tested with histograms
		//and freq offsets; changes with offset too, likely a result of the freq wrap at 500 hz offset (some noise cause true (say +100Hz) and a few false (-400Hz) results
		//really should be pi/2, the inflation factor is due to this false lock; but not sure where the pi/2 comes in
		trk->myFreqErr = trk->myFreqErr + 0.001 * 1.45 * ste->carrError; //add 1000 of them together so divide each by 1000 (or x 0.001)
		trk->oldPI = res->PI;
		trk->oldPQ = res->PQ;
		trk->acqMode++;
		if (trk->acqMode == 1000) { //incorporate that avg before moving to tracking
			//printf("Init Freq Est to FLL: %f\n", acq->carrFreqAcq);
			acq->carrFreqAcq = acq->carrFreqAcq - trk->myFreqErr;
			//printf("My Freq Est to Tracking: %f\n", acq->carrFreqAcq);
		}

	} else {  //phase tracking
		ste->carrError = atan(((fl64) (res->PQ)) / ((fl64) (res->PI)))
				/ (2 * M_PI);  //why is this not atan2 function?
		ste->carrNco = ste->oldCarrNco
				+ trk->tau2carr / trk->tau1carr
						* (ste->carrError - ste->oldCarrError)

				+ ste->carrError * PDICARR / trk->tau1carr;
		ste->oldCarrError = ste->carrError;
		ste->oldCarrNco = ste->carrNco;
		ctr->carrFreq = acq->carrFreqAcq + ste->carrNco;
	}

	ctr->startSample = ctr->startSample + ctr->blkSize;

	ctr->blkSize = (si32) (ceil(
			((fl64) (PRNCODELENGTH) - ctr->remCodePhase)
					/ (ctr->codeFreq / SAMPLINGRATE )));

	return;
}

void * chn_main(void * param) {
	/// Cast input pointer to the correct type, extract pointers to structs of interest
	chn_s * chn = param;
	/// Allocate variables
	si32 chnm;
	si32 m;
	ui32 cnt;
	fl64 trigArg;
	ui32 startCnt;
	fl64 myCorrAcqE, myCorrAcqP, myCorrAcqL, myCorrAcq;
	//fl64 carrFreq[PRNLISTLENGTH] = MYPRNCARRFREQ;
	/*reformat structures right here*/
	ctr_cha_s * ctr;
	res_cha_s * res;
	apt_s * apt;
	chn_acq_s * acq;
	chn_trk_s * trk;
	chn_ste_s * ste;
	chn_asn_s * asn;
	ctr = chn->ctr;
	res = chn->res;
	apt = chn->apt;
	acq = chn->acq;
	trk = chn->trk;
	ste = chn->ste;
	asn = &chn->asn;
	startCnt = 0;


	// Initialize assignments
	for (chnm = 0; chnm < CONF_NUMBLK; chnm++) { //CCR_MAX_CHANNELS

		//Keep track of what channel we are processing
		m = chn->m * 4 + chnm;
		// Process a block of ~10 ms each
		while (ctr[m].startSample < NUMSAMPLEINBUFF) { //keep running correlator through code periods in first  <= but (NUM-1) indexing
			startCnt++;
			if (startCnt > 1) {
				printf("Warning! StartSample cnt >1!!\n");
			}

			trk[m].millisecs++; //increment millisecond counter.

			/* UPDATE REMCARRPHASE (the others are updated in chn) */
			trigArg = ((fl64) ctr[m].blkSize) * 2.0 * M_PI
					* (ctr[m].carrFreq / SAMPLINGRATE ) + ctr[m].remCarrPhase;
			ctr[m].remCarrPhase = fmod(trigArg, 2.0 * M_PI);
			//same thing here, setting the exact (based on double precision floats) rem code phase and will use that for the RX
			ctr[m].remCodePhase = (fl64) ctr[m].blkSize
					* (ctr[m].codeFreq / SAMPLINGRATE ) + ctr[m].remCodePhase
					- ((fl64) (PRNCODELENGTH));

			//Save the integer part and the fractional part of the absolute sample number.
			trk[m].absoluteSample = (ui64) (trk[m].samNum);
			trk[m].absSamFrac = (fl32) -(ctr[m].remCodePhase
					/ (ctr[m].codeFreq / SAMPLINGRATE ));

			trk[m].dt = ctr[m].remCodePhase / ctr[m].codeFreq; //calculate time
			if (CARRPHASEINVERSION == 1) {
				trk[m].rawDoppler = CARRFREQ - ctr[m].carrFreq; //doppler in Hz
			} else {
				trk[m].rawDoppler = ctr[m].carrFreq - CARRFREQ; //doppler in Hz
			}
			if (trk[m].millisecs == 1) {
				trk[m].rawCarrPhase = 0;
			} else {
				trk[m].rawCarrPhase = trk[m].rawCarrPhase
						- trk[m].rawDoppler
								* (ctr[m].blkSize / SAMPLINGRATE - trk[m].dt);
			}

			/* Print out to check soft corrrelator*/
#if LOGCOR
			fwrite(&(m), sizeof(si32), 1, chn->binTrkFid);
			fwrite(&(ctr[m].prn), sizeof(ui32), 1, chn->binTrkFid);
			fwrite(&(ctr[m].codeFreq), sizeof(fl64), 1, chn->binTrkFid);
			fwrite(&(ctr[m].carrFreq), sizeof(fl64), 1, chn->binTrkFid);
			fwrite(&(res[m].EI), sizeof(si32), 1, chn->binTrkFid);
			fwrite(&(res[m].EQ), sizeof(si32), 1, chn->binTrkFid);
			fwrite(&(res[m].PI), sizeof(si32), 1, chn->binTrkFid);
			fwrite(&(res[m].PQ), sizeof(si32), 1, chn->binTrkFid);
			fwrite(&(res[m].LI), sizeof(si32), 1, chn->binTrkFid);
			fwrite(&(res[m].LQ), sizeof(si32), 1, chn->binTrkFid);
			fwrite(&(res[m].L2EI), sizeof(si32), 1, chn->binTrkFid);
			fwrite(&(res[m].L2EQ), sizeof(si32), 1, chn->binTrkFid);
			fwrite(&(res[m].L2PI), sizeof(si32), 1, chn->binTrkFid);
			fwrite(&(res[m].L2PQ), sizeof(si32), 1, chn->binTrkFid);
			fwrite(&(res[m].L2LI), sizeof(si32), 1, chn->binTrkFid);
			fwrite(&(res[m].L2LQ), sizeof(si32), 1, chn->binTrkFid);
			fwrite(&(ctr[m].remCodePhase), sizeof(fl64), 1, chn->binTrkFid);
			fwrite(&(ctr[m].remCarrPhase), sizeof(fl64), 1, chn->binTrkFid);
			fwrite(&(ctr[m].startSample), sizeof(ui32), 1, chn->binTrkFid);
			fwrite(&(ctr[m].blkSize), sizeof(ui32), 1, chn->binTrkFid);
			fwrite(&(trk[m].absoluteSample), sizeof(ui64), 1, chn->binTrkFid);
			fwrite(&(trk[m].absSamFrac), sizeof(fl32), 1,chn->binTrkFid);
			fwrite(&(trk[m].VSM), sizeof(fl64), 1, chn->binTrkFid);
			fwrite(&(trk[m].lockTime), sizeof(fl64), 1, chn->binTrkFid);

#endif


			/// update the apt structure to give to nav (this structure gets handed over to nav after each 10 ms block)
			apt->emt[m].prn = 0x0100 | ctr[m].prn; //0x0100 + something weird there should only be PRN right?
			apt->emt[m].P_I[apt->emt[m].len] = res[m].PI;
#ifdef USEL2C
			apt->emt[m].L2P_I[apt->emt[m].len] = res[m].L2PI;
#endif
			//apt->emt[m].smp[apt->emt[m].len] = (ui64) trk[m].samNum;
			apt->emt[m].absoluteSample[apt->emt[m].len] = trk[m].absoluteSample;
			apt->emt[m].absSamFrac[apt->emt[m].len] = trk[m].absSamFrac;
			apt->emt[m].carr[apt->emt[m].len] = trk[m].rawCarrPhase;
			apt->emt[m].dopp[apt->emt[m].len] = trk[m].rawDoppler;
			apt->emt[m].VSM[apt->emt[m].len] = trk[m].VSM;
			apt->emt[m].lockTime[apt->emt[m].len] = trk[m].lockTime;
			apt->emt[m].len++;

			/* ------------------------------------Here starts Acquisition ----------------------------------------------*/
			if (trk[m].acqMode < 1) {  // acq mode

				trk[m].lockTime = -200;// WE are in acq mode
				//avg across early, late, prompt as 1msec acc are noisy, and not taking sqrt to save some CPU
				myCorrAcqE = pow(((fl64) (res[m].EI)), 2.0)
						+ pow(((fl64) (res[m].EQ)), 2.0);
				myCorrAcqP = pow(((fl64) (res[m].PI)), 2.0)
						+ pow(((fl64) (res[m].PQ)), 2.0);
				myCorrAcqL = pow(((fl64) (res[m].LI)), 2.0)
						+ pow(((fl64) (res[m].LQ)), 2.0);
				myCorrAcq = myCorrAcqE + myCorrAcqP + myCorrAcqL;
				//set threshold based on data observed, probably can reduce with mechanism to confirm phase lock and toss out if not
				if (myCorrAcq > ACQTHRESHOLD) { //signal detected, start fine resolution
					trk[m].acqMode = 1;  // goto refine freq
					trk[m].lockTime = -200;// WE are in acq mode
					asn->trackSats[ctr[m].prn] = 0; //Set this to 0 again to indicate we acquired it.
					trk[m].trackCnt = 0;
					printf("  --> PRN %d, acquired on channel %d at %lf mins\n",
							ctr[m].prn + 1, m,
							(fl64) trk[m].absoluteSample / SAMPLINGRATE / 60);

					ctr[m].blkSize = (si32) (NUMSAMPLE1MSEC);
					ctr[m].remCodePhase = 0.0; //just set this remCodePhase to 0
					acq[m].carrFreqAcq = ctr[m].carrFreq; //should also set code freq here too proportional to the acq freq
					trk[m].oldPI = res[m].PI;
					trk[m].oldPQ = res[m].PQ;
					if ((myCorrAcqP >= myCorrAcqE)
							&& (myCorrAcqP >= myCorrAcqL)) { //prompt is strongest
						ctr[m].startSample = ctr[m].startSample
								+ ctr[m].blkSize;
						trk[m].samNum = (fl64) trk[m].samNum + ctr[m].blkSize;
					} else if ((myCorrAcqE >= myCorrAcqP)
							&& (myCorrAcqE >= myCorrAcqL)) { //early is strongest
						ctr[m].startSample = ctr[m].startSample + ctr[m].blkSize
								+ 3; //ODD-shift back to the early code place
						trk[m].samNum = (fl64) trk[m].samNum + ctr[m].blkSize
								+ 3; //ODD-shift back to the early code place
					} else {
						ctr[m].startSample = ctr[m].startSample + ctr[m].blkSize
								- 3; //ODD-shift ahead to the late code place
						trk[m].samNum = (fl64) trk[m].samNum + ctr[m].blkSize
								- 3; //ODD-shift ahead to the late code place
					}
				}

				else {  //not locked, step through acq steps
					if (trk[m].acqCodePhase >= (si32) NUMSAMPLE1MSEC) { //through all code phase, step next freq bin
						if (ctr[m].carrFreq >= (CARRFREQ + CARRDOPPSEARCH)) { //through all freq bins too

							asn->trackSats[ctr[m].prn] = 1; //give back prn to the list since we could not find it.
							chn_asn_update(asn);
							ctr[m].prn = asn->lastCheck;
							/*/printf("  >> Assigned a new PRN! Channel %d is PRN %u\n",
							 m, ctr[m].prn + 1);*/
							ctr[m].newPrn = true;
							//just reset all, here PRN should be changed
							trk[m].acqCodePhase = 0;
							ctr[m].carrFreq =
							CARRFREQ - CARRDOPPSEARCH;
							ctr[m].startSample = ctr[m].startSample
									+ ctr[m].blkSize;
							trk[m].samNum = (fl64) trk[m].samNum
									+ ctr[m].blkSize;

							ctr[m].blkSize = (si32) (NUMSAMPLE1MSEC);
							ctr[m].remCodePhase = 0.0; //just set this remCodePhase to 0
							ctr[m].remCarrPhase = 0.0;

						} else {  //just step to next freq bin
							trk[m].acqCodePhase = 0;
							ctr[m].carrFreq = ctr[m].carrFreq + CARRSTEPACQ;
							ctr[m].startSample = ctr[m].startSample
									+ ctr[m].blkSize;
							trk[m].samNum = (fl64) trk[m].samNum
									+ ctr[m].blkSize;

							ctr[m].blkSize = (si32) (NUMSAMPLE1MSEC);
							ctr[m].remCodePhase = 0.0; //just set this remCodePhase to 0
							ctr[m].remCarrPhase = 0.0;

						}
					} else { //step to next code phase, skip ahead 1.5 chips since scanning all e,p,l - hard coded here
						trk[m].acqCodePhase = trk[m].acqCodePhase
								+ SAMPLESIN1_5CHIPS;

						ctr[m].startSample = ctr[m].startSample + ctr[m].blkSize
								+ SAMPLESIN1_5CHIPS;
						trk[m].samNum = (fl64) trk[m].samNum + ctr[m].blkSize
								+ SAMPLESIN1_5CHIPS;
						ctr[m].blkSize = (si32) (NUMSAMPLE1MSEC);
						ctr[m].remCodePhase = 0.0; //just set this remCodePhase to 0
						ctr[m].remCarrPhase = 0.0;

					}
				}

			} else { // ----------------------------- Here starts Tracking -----------------------------
				trk[m].samNum = (fl64) trk[m].samNum + ctr[m].blkSize;
				closeLoop(&ste[m], &ctr[m],

				//Close the loops (it is its own function and at the top of this file.)
						&res[m], &trk[m], &acq[m]);

				//condition for throwing out PRN into acquisition again.c
				trk[m].trackCnt++;
				cnt = (ui32) trk[m].trackCnt % (ui32) (VSMINT + 1);

				//make a 200 long vector of the saved Pi and Pq for the VSM calculation.
				trk[m].Ivec[(ui32) cnt] = res[m].PI;
				trk[m].Qvec[(ui32) cnt] = res[m].PQ;
				trk[m].CarrErrorVec[(ui32) cnt] = ste[m].carrError;
				//calculate VSM every 200ms, but ony once we have gone past the FLL
				if ((cnt == VSMINT) && (trk[m].acqMode > 1000)) {
					trk[m].VSM = VSM(trk[m].Ivec, trk[m].Qvec, 0.001);
					if ((trk[m].VSM > VSMTHRESH)
							&& (abs(trk[m].CarrErrorVec[VSMINT - 6]) < CARRERRTHRESH)
							&& (abs(trk[m].CarrErrorVec[VSMINT - 5]) < CARRERRTHRESH)
							&& (abs(trk[m].CarrErrorVec[VSMINT - 4]) < CARRERRTHRESH)
							&& (abs(trk[m].CarrErrorVec[VSMINT - 3]) < CARRERRTHRESH)
							&& (abs(trk[m].CarrErrorVec[VSMINT - 2]) < CARRERRTHRESH)
							&& (abs(trk[m].CarrErrorVec[VSMINT-1]) < CARRERRTHRESH)) { //channel is good, tracking works!
						trk[m].notLockedCnt = 0;
					} else {	//channel is lost
						trk[m].notLockedCnt++;	//increment notLocked
					}

				}

				// If we have not been locked for the past second throw out measurement. Don't throw out anything until we have past FLL
				if (trk[m].notLockedCnt >= 5) {
					trk[m].acqMode = 0;
					trk[m].notLockedCnt = 0;
					trk[m].trackCnt = 0;
					trk[m].lockTime = 0;
					printf("  <-- PRN %d, dropped from channel %d at %f mins\n",
							ctr[m].prn + 1, m,
							(fl64) trk[m].absoluteSample / SAMPLINGRATE / 60);
					// Daehee added below line
					ctr[m].newPrn = true;
					trk[m].millisecs = 0;
					chn_init_acq(&acq[m]);
					chn_init_trk(&trk[m], m);
					chn_init_ste(&ste[m]);
					chn->cnd[m] = 0;
					//assign a new prn number for this channel!
					asn->trackSats[ctr[m].prn] = 1; //give back prn to the list since we could not find it.
					chn_asn_update(asn);
					ctr[m].prn = asn->lastCheck;
					ctr[m].newPrn = true;
					//just reset all, here PRN should be changed
					trk[m].acqCodePhase = 0;
					ctr[m].carrFreq = CARRFREQ - CARRDOPPSEARCH;
					ctr[m].remCodePhase = 0.0; //just set this remCodePhase to 0
					ctr[m].remCarrPhase = 0.0;

					//Set chn to acquisition again, and if there is nothing then it will change prn.
				} else if ((trk[m].trackCnt > VSMINT * 5)&& (trk[m].trackCnt <= VSMINT * 10) ) {
					trk[m].lockTime = 0;
				}else if ((trk[m].trackCnt < VSMINT * 5)&& (trk[m].acqMode > 1)) {
					trk[m].lockTime = -100;// WE are in FLL mode
				}else {
					trk[m].lockTime = trk[m].lockTime + 1;
				}
			}
			//------------------------------------------------------------------------------

		}  //dataleft loop
		   //StartSample describes in what sample to start in the buffer.
		ctr[m].startSample = ctr[m].startSample - NUMSAMPLEINBUFF; //I think this should take care of boundary issues, not fully tested
		startCnt = 0;
	}/*End of number of channels loop*/


	return NULL;
}
