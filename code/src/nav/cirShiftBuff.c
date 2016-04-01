/*
 * trackCirShift.c
 *
 *  Created on: Jun 24, 2015
 *      Author: sean
 */

#include <conf_nav.h>
#include "cirShiftBuff.h"

#include "customDataTypes.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "utils.h"

si32 initCirBuff(cirBuff_s *cirBuff) {
/*
	ui64 buffWriteIndex;
	fl64 * buffSamNum;	// Sample number
	ui64 * buffSamNumInt;	// Sample number: integer part
	fl32 * buffSamNumFrac;	// Sample number: fractional part
	si32 * buffIP;		// In-phase prompt value
	fl64 * buffDopp;	// Doppler shift
	fl64 * buffCarr; 	// Carrier phase
	fl64 buffCNo;		// SNR
*/

	/* Buffer index where to write the data */
	cirBuff->buffWriteIndex = 0;

	/* Initialize the circular buffer */
	cirBuff->buffSamNum = (fl64 *) calloc(1, BUF_LENGTH * sizeof(fl64));
	ERRORCHECK(cirBuff->buffSamNum == NULL)
	cirBuff->buffSamNumInt = (ui64 *) calloc(1, BUF_LENGTH * sizeof(ui64));
	ERRORCHECK(cirBuff->buffSamNumInt == NULL)
	cirBuff->buffSamNumFrac = (fl32 *) calloc(1, BUF_LENGTH * sizeof(fl32));
	ERRORCHECK(cirBuff->buffSamNumFrac == NULL)
	cirBuff->buffIP = (si32 *) calloc(1, BUF_LENGTH * sizeof(si32));
	ERRORCHECK(cirBuff->buffIP == NULL)
	cirBuff->buffDopp = (fl64 *) calloc(1, BUF_LENGTH * sizeof(fl64));
	ERRORCHECK(cirBuff->buffDopp == NULL)
	cirBuff->buffCarr = (fl64 *) calloc(1, BUF_LENGTH * sizeof(fl64));
	ERRORCHECK(cirBuff->buffCarr == NULL)
	cirBuff->buffCodePhase = (fl64 *) calloc(1, BUF_LENGTH * sizeof(fl64));
	ERRORCHECK(cirBuff->buffCodePhase == NULL)

	return EXIT_SUCCESS;
}

void deleteCirBuff(cirBuff_s *cirBuff) {
	free(cirBuff->buffSamNum);
	free(cirBuff->buffSamNumInt);
	free(cirBuff->buffSamNumFrac);
	free(cirBuff->buffIP);
	free(cirBuff->buffDopp);
	free(cirBuff->buffCarr);
	free(cirBuff->buffCodePhase);
}

void buffUpdate(cirBuff_s *cirBuff[], cirBuffDataIn data) {

	// Find where to write
    ui32 writeIndex = cirBuff[data.prn]->buffWriteIndex;

    // Update the buffer
	cirBuff[data.prn]->buffSamNum[writeIndex] 		= data.absSam;
	cirBuff[data.prn]->buffSamNumInt[writeIndex] 	= data.absSamInt;
	cirBuff[data.prn]->buffSamNumFrac[writeIndex] 	= data.absSamFrac;
	cirBuff[data.prn]->buffIP[writeIndex] 			= data.IP;
	cirBuff[data.prn]->buffDopp[writeIndex] 		= data.dopp;
	cirBuff[data.prn]->buffCarr[writeIndex] 		= data.carr;
	cirBuff[data.prn]->buffCNo			 			= data.CNo;
	cirBuff[data.prn]->buffCodePhase[writeIndex] 	= data.codePhase;

	// Update the buffer index
	cirBuff[data.prn]->buffWriteIndex = (writeIndex + 1) % BUF_LENGTH;
}


void bufGet(navChannels_s *navChannels[], cirBuff_s *cirBuff[],	state_s state) {

	/* Circular data = |Recent data| ... (Currently update here) ... |Old data|
	 * Tracking data = |Old data| + |Recent data|
	 * Old data: Current write index  --> End of buffer
	 * Recent data: Beginning of data --> Current index */

	si32 i, buffOffset;		// i: index for PRN array, buffOffset: buffer offset
	si32 * prn;				// PRN (actually it is "prn")
	si32 numberSize = 0;	// Number of PRNs

	/* Find the PRNs */
	findsi32(&prn, &numberSize, state.checkPRN, 32, ">", 0, "all");

	for (i = 0; i < numberSize; i++) {

		/* calculate the data length towards the end of buffer */
		buffOffset = BUF_LENGTH - cirBuff[prn[i]]->buffWriteIndex;

		/* update the sample number */
		memcpy(&navChannels[(si32) prn[i]]->buffSamNum[0],
				&cirBuff[(si32) prn[i]]->buffSamNum[cirBuff[prn[i]]->buffWriteIndex],
				sizeof(fl64) * buffOffset);
		memcpy(&navChannels[(si32) prn[i]]->buffSamNum[buffOffset],
				&cirBuff[(si32) prn[i]]->buffSamNum[0],
				sizeof(fl64) * cirBuff[prn[i]]->buffWriteIndex);

		/* update the sample number: Integer part */
		memcpy(&navChannels[(si32) prn[i]]->buffSamNumInt[0],
				&cirBuff[(si32) prn[i]]->buffSamNumInt[cirBuff[prn[i]]->buffWriteIndex],
				sizeof(ui64) * buffOffset);
		memcpy(&navChannels[(si32) prn[i]]->buffSamNumInt[buffOffset],
				&cirBuff[(si32) prn[i]]->buffSamNumInt[0],
				sizeof(ui64) * cirBuff[prn[i]]->buffWriteIndex);

		/* update the sample number: Fractional part */
		memcpy(&navChannels[(si32) prn[i]]->buffSamNumFrac[0],
				&cirBuff[(si32) prn[i]]->buffSamNumFrac[cirBuff[prn[i]]->buffWriteIndex],
				sizeof(fl32) * buffOffset);
		memcpy(&navChannels[(si32) prn[i]]->buffSamNumFrac[buffOffset],
				&cirBuff[(si32) prn[i]]->buffSamNumFrac[0],
				sizeof(fl32) * cirBuff[prn[i]]->buffWriteIndex);

		/* update the in-phase prompt */
		memcpy(&navChannels[(si32) prn[i]]->buffIP[0],
				&cirBuff[(si32) prn[i]]->buffIP[cirBuff[prn[i]]->buffWriteIndex],
				sizeof(si32) * buffOffset);
		memcpy(&navChannels[(si32) prn[i]]->buffIP[buffOffset],
				&cirBuff[(si32) prn[i]]->buffIP[0],
				sizeof(si32) * cirBuff[prn[i]]->buffWriteIndex);

		/* update the doppler */
		memcpy(&navChannels[(si32) prn[i]]->buffDopp[0],
				&cirBuff[(si32) prn[i]]->buffDopp[cirBuff[prn[i]]->buffWriteIndex],
				sizeof(fl64) * buffOffset);
		memcpy(&navChannels[(si32) prn[i]]->buffDopp[buffOffset],
				&cirBuff[(si32) prn[i]]->buffDopp[0],
				sizeof(fl64) * cirBuff[prn[i]]->buffWriteIndex);

		/* update the carrier phase */
		memcpy(&navChannels[(si32) prn[i]]->buffCarr[0],
				&cirBuff[(si32) prn[i]]->buffCarr[cirBuff[prn[i]]->buffWriteIndex],
				sizeof(fl64) * buffOffset);
		memcpy(&navChannels[(si32) prn[i]]->buffCarr[buffOffset],
				&cirBuff[(si32) prn[i]]->buffCarr[0],
				sizeof(fl64) * cirBuff[prn[i]]->buffWriteIndex);

		navChannels[(si32) prn[i]]->CNo = cirBuff[prn[i]]->buffCNo;

		/* update the code phase */
		memcpy(&navChannels[(si32) prn[i]]->buffCodePhase[0],
				&cirBuff[(si32) prn[i]]->buffCodePhase[cirBuff[prn[i]]->buffWriteIndex],
				sizeof(fl64) * buffOffset);
		memcpy(&navChannels[(si32) prn[i]]->buffCodePhase[buffOffset],
				&cirBuff[(si32) prn[i]]->buffCodePhase[0],
				sizeof(fl64) * cirBuff[prn[i]]->buffWriteIndex);

	}
}


void bufGet1ch(navChannels_s *navChannels[], cirBuff_s *cirBuff[], si32 prn) {

	/* Circular data = |Recent data| ... (Currently update here) ... |Old data|
	 * Tracking data = |Old data| + |Recent data|
	 * Old data: Current write index  --> End of buffer
	 * Recent data: Beginning of data --> Current index */

	si32 buffOffset;		// buffer offset

	/* calculate the data length towards the end of buffer */
	buffOffset = BUF_LENGTH - cirBuff[prn]->buffWriteIndex;

	/* update the sample number */
	memcpy(&navChannels[(si32) prn]->buffSamNum[0],
			&cirBuff[(si32) prn]->buffSamNum[cirBuff[prn]->buffWriteIndex],
			sizeof(fl64) * buffOffset);
	memcpy(&navChannels[(si32) prn]->buffSamNum[buffOffset],
			&cirBuff[(si32) prn]->buffSamNum[0],
			sizeof(fl64) * cirBuff[prn]->buffWriteIndex);

	/* update the sample number: Integer part */
	memcpy(&navChannels[(si32) prn]->buffSamNumInt[0],
			&cirBuff[(si32) prn]->buffSamNumInt[cirBuff[prn]->buffWriteIndex],
			sizeof(ui64) * buffOffset);
	memcpy(&navChannels[(si32) prn]->buffSamNumInt[buffOffset],
			&cirBuff[(si32) prn]->buffSamNumInt[0],
			sizeof(ui64) * cirBuff[prn]->buffWriteIndex);

	/* update the sample number: Fractional part */
	memcpy(&navChannels[(si32) prn]->buffSamNumFrac[0],
			&cirBuff[(si32) prn]->buffSamNumFrac[cirBuff[prn]->buffWriteIndex],
			sizeof(fl32) * buffOffset);
	memcpy(&navChannels[(si32) prn]->buffSamNumFrac[buffOffset],
			&cirBuff[(si32) prn]->buffSamNumFrac[0],
			sizeof(fl32) * cirBuff[prn]->buffWriteIndex);

	/* update the in-phase prompt */
	memcpy(&navChannels[(si32) prn]->buffIP[0],
			&cirBuff[(si32) prn]->buffIP[cirBuff[prn]->buffWriteIndex],
			sizeof(si32) * buffOffset);
	memcpy(&navChannels[(si32) prn]->buffIP[buffOffset],
			&cirBuff[(si32) prn]->buffIP[0],
			sizeof(si32) * cirBuff[prn]->buffWriteIndex);

	/* update the doppler */
	memcpy(&navChannels[(si32) prn]->buffDopp[0],
			&cirBuff[(si32) prn]->buffDopp[cirBuff[prn]->buffWriteIndex],
			sizeof(fl64) * buffOffset);
	memcpy(&navChannels[(si32) prn]->buffDopp[buffOffset],
			&cirBuff[(si32) prn]->buffDopp[0],
			sizeof(fl64) * cirBuff[prn]->buffWriteIndex);

	/* update the carrier phase */
	memcpy(&navChannels[(si32) prn]->buffCarr[0],
			&cirBuff[(si32) prn]->buffCarr[cirBuff[prn]->buffWriteIndex],
			sizeof(fl64) * buffOffset);
	memcpy(&navChannels[(si32) prn]->buffCarr[buffOffset],
			&cirBuff[(si32) prn]->buffCarr[0],
			sizeof(fl64) * cirBuff[prn]->buffWriteIndex);

	navChannels[(si32) prn]->CNo = cirBuff[(si32) prn]->buffCNo;

	/* update the code phase */
	memcpy(&navChannels[(si32) prn]->buffCodePhase[0],
			&cirBuff[(si32) prn]->buffCodePhase[cirBuff[prn]->buffWriteIndex],
			sizeof(fl64) * buffOffset);
	memcpy(&navChannels[(si32) prn]->buffCodePhase[buffOffset],
			&cirBuff[(si32) prn]->buffCodePhase[0],
			sizeof(fl64) * cirBuff[prn]->buffWriteIndex);

}

/* FOR NAV bit*/
si32 initCirBuffNavBit(cirBuffNavBit_s *cirBuffNavBit) {

	/* Buffer index where to write the data */
	cirBuffNavBit->buffWriteIndex = 0;	// Write index
	cirBuffNavBit->bitCount = 0;	//NAV bit counter

	/* Initialize the circular buffer */
	cirBuffNavBit->buffNavBit = (si32 *) calloc(1,
			NAVBIT_BUF_LENGTH * sizeof(si32));
	ERRORCHECK(cirBuffNavBit->buffNavBit == NULL)
	cirBuffNavBit->buffSamNum = (fl64 *) calloc(1,
			NAVBIT_BUF_LENGTH * sizeof(fl64));
	ERRORCHECK(cirBuffNavBit->buffSamNum == NULL)

	return EXIT_SUCCESS;
}

void deleteCirBuffNavBit(cirBuffNavBit_s *cirBuffNavBit) {
	free(cirBuffNavBit->buffNavBit);
	free(cirBuffNavBit->buffSamNum);
}

void cirBuffUpdateNavBit(cirBuffNavBit_s *cirBuff[], si32 ch, si32 bit,
		fl64 samNum) {

	/* Update the buffer */
	/* TODO: None of these should be floats */
	cirBuff[ch]->buffNavBit[cirBuff[ch]->buffWriteIndex] = bit;
	cirBuff[ch]->buffSamNum[cirBuff[ch]->buffWriteIndex] = samNum;

	/* Update the buffer index */
	cirBuff[ch]->buffWriteIndex = (cirBuff[ch]->buffWriteIndex + 1)
			% NAVBIT_BUF_LENGTH;
}
