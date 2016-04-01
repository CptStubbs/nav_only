/*
 * initNavChannels.h
 *
 *  Created on: May 11, 2015
 *      Author: user
 */

#ifndef SRC_INCLUDE_INITNAVCHANNELS_H_
#define SRC_INCLUDE_INITNAVCHANNELS_H_

#include "customDataTypes.h"

typedef struct {


	si32 PRN; /*!<PRN number of the tracked satellite*/
	pi8 status; /*!<Mode/status of the tracking channel: acquire */
	si32 Lock; /*!<Signal is locked (1) or not (0)*/

	/* For nav bit decoding */
	si32 bitSynchFlag; /*!<0: no, 1: bit synch */
	fl64 bitSynchSamNum; /*!< sample number at bit synch */
	si32 frameSync; /*!<0: no preamble, 1: found the preamble & verified the 6sec gap*/
	fl64 firstSubFrameBits; /*!<bit index of first preamble*/

	fl64 TOW; /*!<TOW for current channel*/
	si32 decode; /*!<whether ephemeris fully decoded or not*/


	/* Channel status*/
	ui64 count;
	fl64 samples;

	/* time for the phase lock and the threshold*/
	ui64 LockTime;

	/* C/No*/
	fl64 CNo;
	//fl64 VSMIndex;

	/*  track results*/
	fl64 ** dataBuff;
	ui32 dataBuffSize;
	ui64 decodeWaitCounter;

	/* New tracking buffer*/
	fl64 * buffSamNum;
	ui64 * buffSamNumInt;
	fl32 * buffSamNumFrac;
	si32 * buffIP;
	fl64 * buffDopp;
	fl64 * buffCarr;
	//fl64 * buffCNo;
	fl64 * buffCodePhase;

} navChannels_s;

si32 initNavChannels(navChannels_s * navChannels);
void deleteNavChannels(navChannels_s * navChannels);

#endif /* SRC_INCLUDE_INITNAVCHANNELS_H_ */
