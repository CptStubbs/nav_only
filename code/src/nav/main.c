/*
 * main.c
 *
 *  Created on: Oct 21, 2015
 *      Author: Daehee
 */
#include "conf_nav.h"
#include "initNav2sch.h"
#include <stdio.h>

#include "apt.h"
#include "conf.h"
#include "conf_chn.h"
#include "conf_swc.h"
#include "customDataTypes.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "nav_init.h"


si32 main() {
	// Cast input pointer to the correct type, extract pointers to structs of interest
	apt_s apt;
	navSuper_s navStr;
	nav2sch_s * nav2sch[NUM_PRNS];

	si32 ij = 0;
	pi8 aptFilename[256], rnxFilename[256];
	FILE *binAptFid;

	si32 week = 0, tow = 0;

	// Select the right index for the APT file


	nav_init(&navStr); 		// Init nav
	nav2sch_init(nav2sch);	// init nav2sch structure

#if LOGRNX
	fclose(navStr.binRnxFid);	// it is already open within "nav_init"
	snprintf(rnxFilename, 256, RNXOUT, week, tow);	//generate file name
	navStr.binRnxFid = fopen(rnxFilename, "wb");
	ERRORCHECK(navStr.binRnxFid == NULL)
	writeHeader(navStr.binRnxFid);
	writernxHeader(navStr.binRnxFid);

#endif

	//generate APT file name & open it
	snprintf(aptFilename, 256, APTOUT, week, tow);	//generate file name: Month/Day/Year
	binAptFid = fopen(aptFilename, "rb");
	ERRORCHECK(binAptFid == NULL)
	//fseek(binAptFid, aptOffset, SEEK_SET); /*This line moot by loop below?*/
	pi8 hdrEnd[] = "*END*"; /*Dirty fix, but this string is in the last line of the hdr*/
	pi8 line[128];

	if(NAVDEBUG == 1){
		while(1){ /*This loop will skip  APT header for nav-debugging */
			fgets(line,sizeof(line),binAptFid);
			if(strstr(line,hdrEnd)){ /*if hdrEnd is read in current line*/
				break; /*End of header reached */
			}
		}
	}


	// While loop to run through all the data.
	while (1) {

		// counter update
		ij++;

		if (fread(&apt.emt, sizeof(apt.emt), 1, binAptFid) == 0){

			// close current FID
			fclose(binAptFid);
			printf("Epoch Counter = %d \n", ij);

			tow = tow + 86400;
			if (tow >= 604800){
				week++;
				tow = tow % 604800;
			}

			//Open new APT file
			snprintf(aptFilename, 256, APTOUT, week, tow);
			binAptFid = fopen(aptFilename, "rb");
			if (binAptFid == NULL){
				break;
			}
		}

		nav_main(&navStr, &apt, nav2sch);

	}

	printf("End of File \n");

	nav_free(&navStr);
	nav2sch_free(nav2sch);

	fflush(stdout);
}
