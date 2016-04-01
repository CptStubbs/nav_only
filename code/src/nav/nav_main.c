/*
 * runNav.c
 *
 *  Created on: Jul 22, 2015
 *      Author: daehee
 */

#include "conf_nav.h"
#include "initNav2sch.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "nav_init.h"
#include <string.h>

#include "customDataTypes.h"
#include "doDecoding.h"
#include "doNavigation.h"

#include "apt.h"
#include "conf_apt.h"
#include "conf.h"

#include "cart2geo.h"
#include "conf_swc.h"


si32 nav_main(navSuper_s * navStr, apt_s * apt, nav2sch_s ** nav2sch) {

	// =====< Define variables >=====
	si32 prn 			= 0;		// PRN number
	fl64 samNum 		= 0;		// Sample number
	si32 i = 0, j = 0;				// FOR loop counters
	si32 readySatCount 	= 0;		// count tracked & decoded channels to detect satellites changes
	fl64 tempSamNum 	= 0;		// Variable for the accumulated sample number

	cirBuffDataIn cirDataIn;		// Structure of the circular buffer input

	// update the block counter which is increasing 1 by 1 after 1 block processing of acq/track
	navStr->state.trackBlockCount++;

	// Display the millisecond counter
	if (navStr->state.trackBlockCount % 11111 == 0) {
		printf("Processed %.3f seconds\n",
				(fl64) navStr->state.trackBlockCount * NUMSAMPLEINBUFF / SAMPLINGRATE);
	}

	// run Nav at a specific interval
	//if(navStr->state.trackBlockCount % (APT_LEN + 1) == 0){
	if(navStr->state.trackBlockCount % (APT_LEN + 1) == 0 || NAVDEBUG){

// Change the file name every day based on GPS time ----------------------

		// Below code is for ROUND because internal ROUND function does not work properly...
		navStr->state.currTOW = (si32) navStr->navSolutions.rawRxTime
				+ (si32) ( (navStr->navSolutions.rawRxTime - (si32)navStr->navSolutions.rawRxTime)*2 );
		if (navStr->state.currTOW >= 604800){
			navStr->state.currTOW = navStr->state.currTOW - 604800;
		}

		// Initialize "nextTOW" to split out the file
		if (navStr->state.fileNameFlag == 0){
			if (navStr->state.navCount != 0){

				navStr->state.nextTOW = 0;
				while (1){
					navStr->state.nextTOW = navStr->state.nextTOW + 86400;
					if (navStr->state.nextTOW > navStr->state.currTOW){
						break;
					}
				}
				// Initialize the TOW & Week number
				navStr->state.nextTOW = navStr->state.nextTOW % 604800;
				if (navStr->state.nextTOW == 0){
					navStr->state.nextWeek = navStr->navSolutions.weekNumber + 1;
				}
				else{
					navStr->state.nextWeek = navStr->navSolutions.weekNumber;
				}
				navStr->state.fileNameFlag = 1;
			}
		}

		// Check the GPS time
		if (navStr->state.nextTOW <= navStr->state.currTOW) {

			if ( navStr->state.nextTOW != 0 || (navStr->state.nextTOW == 0 && navStr->state.currTOW < 518400) ){
#if LOGAPT
				// close current FID & Open new FID
				fclose(navStr->binAptFid);
				snprintf(navStr->aptFilename, 256, APTOUT,
						navStr->state.nextWeek, navStr->state.nextTOW);
				navStr->binAptFid = fopen(navStr->aptFilename, "wb");
				ERRORCHECK(navStr->binAptFid == NULL)
				writeHeader(navStr->binAptFid);
#endif

#if LOGRNX
				// close current FID & Open new FID
				fclose(navStr->binRnxFid);
				snprintf(navStr->rnxFilename, 256, RNXOUT,
						navStr->state.nextWeek, navStr->state.nextTOW);
				navStr->binRnxFid = fopen(navStr->rnxFilename, "wb");
				ERRORCHECK(navStr->binRnxFid == NULL)
				writeHeader(navStr->binRnxFid);
				writernxHeader(navStr->binRnxFid);
#endif
				//update the next reference TOW & Week
				navStr->state.nextTOW = (navStr->state.nextTOW + 86400) % 604800;
				if (navStr->state.nextTOW == 0){
					navStr->state.nextWeek = navStr->navSolutions.weekNumber + 1;
				}
				else{
					navStr->state.nextWeek = navStr->navSolutions.weekNumber;
				}
			}
		}
//----------------------------------------------------------------------------------------------


#if LOGAPT	// Write APT
		fwrite(&apt->emt, sizeof(apt->emt), 1, navStr->binAptFid);
#endif

		// =====< APT structure reading >=====
		for (i = 0; i < APT_NUM; i++) {

			// Check the length of APT. If it is not 0, then there are new data.!
			if (apt->emt[i].len != 0){

				// Read the PRN number
				prn = apt->emt[i].prn & 0xFF;

				// =====< Data buffer update >=====
				for (j = 0; j < apt->emt[i].len; j++) {

					// This is where we should add the fractional part on absSamFrac.
					samNum = (fl64) apt->emt[i].absoluteSample[j] + (fl64) apt->emt[i].absSamFrac[j];

					// save the previous buffer index
					navStr->lastSamNum[prn] = samNum;

					// save variables into the structure for circular buffer input
					cirDataIn.prn 		= prn;
					cirDataIn.absSam 	= samNum;
					cirDataIn.absSamInt = apt->emt[i].absoluteSample[j];
					cirDataIn.absSamFrac= apt->emt[i].absSamFrac[j];
					cirDataIn.IP		= apt->emt[i].P_I[j];
					cirDataIn.dopp 		= apt->emt[i].dopp[j];
					cirDataIn.carr 		= apt->emt[i].carr[j];
					cirDataIn.CNo 		= apt->emt[i].VSM[j];
					cirDataIn.codePhase = 0;

					// update the channel
					navStr->navChannels[prn]->count ++;
					navStr->navChannels[prn]->LockTime = apt->emt[i].lockTime[j];
					navStr->navChannels[prn]->PRN = prn;
					//navStr->navChannels[prn]->PRN = prn;

					// Check the Lock time
					if (apt->emt[i].lockTime[j] > 0){
						navStr->navChannels[prn]->status = 'T';
						navStr->navChannels[prn]->Lock = 1;
					}
					else{ // If the lock lost, then reset the status.

						cirDataIn.IP = 0;

						navStr->navChannels[prn]->status 	= 'A';
						navStr->navChannels[prn]->Lock 		= 0;
						navStr->navChannels[prn]->count 	= 0;
						navStr->navChannels[prn]->LockTime 	= 0;
						navStr->navChannels[prn]->decodeWaitCounter = 6000;
					}

					// circular buffer update
					buffUpdate(navStr->cirBuffTrack, cirDataIn);

				}
				// =====< CORE Logic #1: Decoding >=====
				// Check the time if there is enough samples for decoding
				if (navStr->navChannels[prn]->count >= (ui64) navStr->navChannels[prn]->decodeWaitCounter
						&& navStr->navChannels[prn]->Lock == 1) {

					// Read 1 channel buffer data
					bufGet1ch(navStr->navChannels, navStr->cirBuffTrack, prn);

					// Decoding only 1 channel
					if (doDecoding1ch(navStr->navChannels, navStr->cirBuffNavBit,
							&navStr->eph, nav2sch, prn) == EXIT_FAILURE) {
						fprintf(stderr,	"Decoding returned EXIT_FAILURE. There was some error. \n");
					}
				}

				// if newApt flag was 1 and finished reading it, then set the flag as 0.
				apt->emt[i].newApt = false;

				// Count the absolute sample number: That is the maximum value among all channels.
				// In here, the number '50' is related with the variable 'BUFF_SEARCH_RANGE' for the function 'findTransmitTime.c".
				// Remember the number 50 causes time delay in navigation solution.
				tempSamNum = (fl64) samNum - (fl64) (50 * SAMPLESPERCODE);
				if (navStr->state.accNumSamples	< tempSamNum) {
					navStr->state.accNumSamples = tempSamNum;
				}

			} // if (apt->emt[i].newApt == true)

//#if LOGAPT	// Write APT
//			fwrite(&apt->emt, sizeof(apt->emt), 1, navStr->binAptFid);
//#endif
			apt->emt[i].len = 0;

		}


		// Count the available satellites
		readySatCount = 0;
		for (i = 0; i < NUM_PRNS; i++) {
			// Available satellites
			if (navStr->lastSamNum[i] >= navStr->state.accNumSamples){

				// Available + Tracked + frameSych + decoded
				if (navStr->navChannels[i]->status == 'T'
						&& navStr->navChannels[i]->frameSync == 1
						&& navStr->navChannels[i]->decode == 1) {

					navStr->state.checkPRN[i] = 1;
					readySatCount++;
				}
				else{ // regard it as "NOT AVAILABLE"
					navStr->state.checkPRN[i] = 0;
				}
			} else {
				// If the sample number did not update properly, then we regard it as "NOT AVAILABLE"
				navStr->state.checkPRN[i] = 0;
				navStr->navChannels[i]->bitSynchFlag = 0;
				navStr->navChannels[i]->frameSync = 0;
			}
		}


		// Update the number of channels
		navStr->state.numberOfChannels = readySatCount;

		//=====< CORE Logic #2: Navigation >=====

		// Check the time if there is enough samples for navigation
		if (navStr->state.accNumSamples
				> navStr->navSolutions.firstSample
				+ (navStr->state.navCount) * NAVSAMSTEP) {
			// Read buffer data
			bufGet(navStr->navChannels, navStr->cirBuffTrack, navStr->state);


			// Navigation
			if (doNavigation(&navStr->navSolutions,
					&navStr->state, &navStr->eph, navStr->navChannels, nav2sch, navStr->binRnxFid) == EXIT_FAILURE) {
				//fprintf(stderr,"NAV EXIT FAILURE. \n");
			}

			// Write the KML file for Google Earth plot
#if LOGKML
			// convert the coordinate system from WGS84 to LLH
			cart2geo(&navStr->navSolutions.latitude, &navStr->navSolutions.longitude, &navStr->navSolutions.height,
					navStr->navSolutions.xyzdt[0], navStr->navSolutions.xyzdt[1], navStr->navSolutions.xyzdt[2],
					4);

			// Write the KML file
			navStr->kmlFid = fopen(KMLOUT, "w+");
			fprintf(navStr->kmlFid, "<?xml version=\"1.0\" encoding=\"UTF-8\"?><kml xmlns=\"http://www.opengis.net/kml/2.2\"><Document><Placemark><name>Rx Pos</name><LookAt>");
			fprintf(navStr->kmlFid, "<longitude> %f </longitude><latitude> %f </latitude><altitude>0</altitude><range>5000.0</range><gx:altitudeMode>relativeToSeaFloor</gx:altitudeMode></LookAt><Point><gx:drawOrder>1</gx:drawOrder>",
					navStr->navSolutions.longitude, navStr->navSolutions.latitude);
			fprintf(navStr->kmlFid, "<coordinates>%f,%f,%f</coordinates></Point></Placemark></Document></kml>",
					navStr->navSolutions.longitude, navStr->navSolutions.latitude, navStr->navSolutions.height);
			fclose(navStr->kmlFid);
#endif

		}

	}

	// =====< End CORE Logic: Decoding & Navigation >=====

	return EXIT_SUCCESS;
}
