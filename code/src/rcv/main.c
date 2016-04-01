/*
 #include <cor.h>
 * main.c
 *
 *  Created on: May 7, 2015
 *      Author: sean
 */

#include "initNav2sch.h"
#include "customDataTypes.h"

#include "res.h"
#include "ctr.h"
#include "cor.h"

#include "chn_init.h"
#include "nav_init.h"

#include "apt.h"
#include "chn.h"
#include "chn_asn.h"
#include "conf.h"
#include "conf_chn.h"
#include "conf_swc.h"
#include "conf_apt.h"
#include "conf_nav.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

si32 main() {
	/// Allocate shared data structures
	res_s res;
	ctr_s ctr;
	apt_s apt;
	/// Allocate data structure for modules/threads
	cor_s cor;
	chn_s chn;

	/// Initialize shared data structures
	res_init(&res);
	ctr_init(&ctr);
	/// Initialize modules/threads
	cor_init(&cor, &res, &ctr);
	chn_init(&chn, &res, &ctr, &apt);

	navSuper_s navStr;
	nav2sch_s * nav2sch[NUM_PRNS];

	si32 chncnt = 0;

	nav_init(&navStr);
	nav2sch_init(nav2sch);	// init nav2sch structure

	cor_init(&cor, &res, &ctr );
	nav_init(&navStr);

	// While loop to run through all the data.
	while (cor.corDone == false || cor.L2CDone == false) {

		// Read the data
		cor.readData = true;
#ifdef USEL2C
		cor.readL2Data = true;
#endif
		// loop over the channels (4 at a time) 3 times before we have gotten through all the channels.
		for (chncnt = 0; chncnt < CONF_NUMCPB; chncnt++) { // process 4 channels at a time{
			chn.m = chncnt;
			// Run the software correlator for 4 channels
			cor.chnNr = chncnt;
			cor_main((void *) &cor);

			// Do acq/tracking & navigation
			chn_main((void *) &chn);

		} //4channels loop

		// set readData to be true to indicate that we need to read more data
		cor.readData = true;
#ifdef USEL2C
		cor.readL2Data = true;
#endif
		// run NAV
		nav_main(&navStr, &apt, nav2sch);

	} //while loop

	printf("End of File \n");
	res_free(&res);
	ctr_free(&ctr);
	nav_free(&navStr);
	cor_free(&cor);
	chn_free(&chn);
	nav2sch_free(nav2sch);	// free nav2sch structure

	fflush(stdout);

#ifdef USEUSB
	fclose(stdin); /* Apparently we have to close stdin or setvbuf will break */
#endif	

} //end of main
