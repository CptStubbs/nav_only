/*
 * main.c
 *
 *  Created on: May 21, 2015
 *      Author: Nagaraj
 */

#include <stdio.h>
#include "xil_cache.h"

// Local includes
#include "gpsL1dr.h"
#include "chn.h"
#include "ccr.h"

int main() {

//   static XEmacPs ps7_ethernet_0;
//   static XScuTimer ps7_scutimer_0;
//   static XScuWdt ps7_scuwdt_0;

	ui32 RfConfigSuccess;

	Xil_ICacheEnable();
	Xil_DCacheEnable();
	print("---Entering main---\n\r");

	RfConfigSuccess = Max2769_SPI_Config();
	if (RfConfigSuccess == 1) {
		print("RF Config PASSED\r\n");
	}

	// Initialize the GPS L1 Baseband
	InitGpsL1Bb();

	// Initialize channel structures
	chn_init();

	// Enable interrupts
	EnablePlatformInterrupts();

	// Work on the interrupts - forever
	while (1) {

	}

	print("---Exiting main---\n\r");
	Xil_DCacheDisable();
	Xil_ICacheDisable();
	return 0;
}

