/*
 * cirShift.h
 *
 *  Created on: May 13, 2015
 *      Author: Daehee
 */

#ifndef TRACKCIRSHIFT_H_
#define TRACKCIRSHIFT_H_

#include "conf_nav.h"
#include "customDataTypes.h"
#include "initNavChannels.h"
#include "initState.h"
#include "utils.h"

typedef struct {

	ui64 buffWriteIndex;	// Wite index
	si32 bitCount;			// NAV bit counter
	si32 * buffNavBit;	// compressed bits (20 I_P bits --> 1 NAV bit), [-1, 1]
	fl64 * buffSamNum;

} cirBuffNavBit_s;


typedef struct {
	ui64 buffWriteIndex;
	fl64 * buffSamNum;	// Sample number
	ui64 * buffSamNumInt;	// Sample number: integer part
	fl32 * buffSamNumFrac;	// Sample number: fractional part
	si32 * buffIP;		// In-phase prompt value
	fl64 * buffDopp;	// Doppler shift
	fl64 * buffCarr; 	// Carrier phase
	fl64 buffCNo;		// SNR
	fl64 * buffCodePhase;
} cirBuff_s;


typedef struct {
	si32 prn;			// PRN number
	fl64 absSam;		// Sample number
	ui64 absSamInt;		// Sample number: integer part
	fl32 absSamFrac;	// Sample number: fractional part
	si32 IP;			// Inphase prompt
	fl64 dopp;			// Doppler shift
	fl64 carr;			// Carrier phase
	fl64 CNo;			// SNR
	fl64 codePhase;		// Code phase
} cirBuffDataIn;


si32 initCirBuff(cirBuff_s *myCirShift);
void deleteCirBuff(cirBuff_s *myCirShift);
void buffUpdate(cirBuff_s *myCirBuff[], cirBuffDataIn data);
void bufGet(navChannels_s *myTrack[], cirBuff_s *cirBuff[], state_s state);
void bufGet1ch(navChannels_s *navChannels[], cirBuff_s *cirBuff[], si32 prn);

si32 initCirBuffNavBit(cirBuffNavBit_s *cirBuff);
void deleteCirBuffNavBit(cirBuffNavBit_s *cirBuff);
void cirBuffUpdateNavBit(cirBuffNavBit_s *cirBuff[], si32 ch, si32 bit,
		fl64 samNum);

#endif /* TRACKCIRSHIFT_H_ */
