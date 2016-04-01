/*
 * gpsL1dr.h
 *
 *  Created on: Nov 23, 2014
 *      Author: Nagaraj
 */

#ifndef GPSL1DR_H_
#define GPSL1DR_H_

#include "customDataTypes.h"
//#include "xparameters.h"
//#include "xil_types.h"
//#include "xscugic.h"
//#include "xgpio.h"
//#include "gpio_header.h"

#define MAX_CHANNELS 12
#define NUM_CODE_TAPS 3
#define CHANNEL_ADDRESS_OFFSET (0x10*4) // the *4 is for byte addressing

//#define GPS_L1CA_BB_BASE_ADDRESS XPAR_EMC_0_S_AXI_MEM0_BASEADDR // Vivado doesn't seem to change the macro for manually entered address base values
#define GPS_L1CA_BB_BASE_ADDRESS 0x44000000

// Nominal values for MAX2769 default configuration
// 4.092MHz resultant carrier frequency
// 1.023MHz code frequency
#define NOMINAL_CARR_FREQ 0x10000000
#define NOMINAL_CODE_FREQ	0x04000000

// Define a structure for the baseband interface
#if (3 == NUM_CODE_TAPS)
typedef struct {

	ui16 prnG1;		//G1 register
	ui16 prnG2;		//G2 register

	ui32 CarNCOLoad;// Carrier NCO value = (car_nco_freq * 2^30) / nco_clk_freq

	ui32 CodeNCOLoad;// Code NCO value	= (2* code_nco_freq * 2^29) / nco_clk_freq

	ui16 CodeSlewLoad;	// Code slew in number of half chips
	ui16 Reserved0;

	ui32 CarMeas;// bits 31:10 => Carrier cycles within the meas interval, bits 9:0 => fractional carrier phase

	ui32 CodeMeas;// bits 20:10 => Number of half chips cleared at the accumulator dump, latched at the meas TIC
				  // bits 9:0 => fractional code phase
	ui16 EpochMeas;	// bits 4:0 => number of code sequences ranges from 0-19 (ideally 1ms), cleared when Epoch Register is loaded,
					// latched at measurement TIC, incremented at every accumulator dump.
					// bits 10:5 => number of bit sequences (typically 20ms)
	ui16 EpochInst;	// Same as EpochMeas but instantaneous. Latched when the register is read.

	ui16 EpochLoad;	// Epoch register to load
	ui16 Reserved1;

	si32 IEarly;// 1ms accumulator values. Actually 16 bit value sign extended
	si32 QEarly;
	si32 IPrompt;
	si32 QPrompt;
	si32 ILate;
	si32 QLate;

	ui32 Reserved2;
	ui32 Reserved3;

} GpsL1BbChn;

#elif (8 == NUM_CODE_TAPS)

typedef struct {

	ui16 prnG1;		//G1 register
	ui16 prnG2;//G2 register
	ui32 CarNCOLoad;// Carrier NCO value = (car_nco_freq * 2^30) / nco_clk_freq
	ui32 CodeNCOLoad;// Code NCO value	= (2* code_nco_freq * 2^29) / nco_clk_freq
	ui16 CodeSlewLoad;// Code slew in number of half chips
	ui16 Reserved0;
	ui32 CarMeas;// bits 31:10 => Carrier cycles within the meas interval, bits 9:0 => fractional carrier phase
	ui32 CodeMeas;// bits 20:10 => Number of half chips cleared at the accumulator dump, latched at the meas TIC
				  // bits 9:0 => fractional code phase
	ui16 EpochMeas;// bits 4:0 => number of code sequences ranges from 0-19 (ideally 1ms), cleared when Epoch Register is loaded,
				   // latched at measurement TIC, incremented at every accumulator dump.
				   // bits 10:5 => number of bit sequences (typically 20ms)
	ui16 EpochInst;// Same as EpochMeas but instantaneous. Latched when the register is read.
	ui16 EpochLoad;// Epoch register to load
	ui16 Reserved1;

	si16 I0;// 1ms accumulator values. Note that the first three taps are aligned with 3 tap design
	si16 I4;

	si16 Q0;
	si16 Q4;

	si16 I1;
	si16 I5;

	si16 Q1;
	si16 Q5;

	si16 I2;
	si16 I6;

	si16 Q2;
	si16 Q6;

	si16 I3;
	si16 I7;

	si16 Q3;
	si16 Q7;

}GpsL1BbChn;

#endif

typedef struct {
	ui32 InterruptClear; // bit 1 => Accumulator interrupt clear, bit 0 => Meas TIC interrupt clear
	ui32 AccDataReady; // bits 0:MAX_CHANNELS-1 indicate the availability of new accumulator data;
	ui32 TICRegCount; // 24 bits of measurement interval register
	ui32 AccRegCount; // 24 bits of accumulator interval register
	ui32 AccDataReadPending; // bits 0:MAX_CHANNELS-1 => set if the processor fails to read the cor values on the acc interrupt
							 // Only one miss indicated (multiple missed reads not indicated).
	ui32 Reserved0;
	ui32 Reserved1;
	ui32 Reserved2;
	ui32 Reserved3;
	ui32 Reserved4;
	ui32 Reserved5;
	ui32 Reserved6;
	ui32 Reserved7;
	ui32 Reserved8;
	ui32 Reserved9;

	ui32 HardwareID; // Hardware version of the GPSL1 Baseband. Example hardware version  = 1.00 = 100d = 0x64h
} GpsL1BbStatus;

typedef struct {
	ui32 BbSoftReset; // When write operation (any content) is performed on this register, entire Bb is reset
					  // This is in addition to the hard reset through processor GPIO.
	ui32 TICIntervalProg; // 24 bits of measurement interval referenced to  sampling clock (16.368MHz)
	ui32 AccIntervalProg; // 24 bits of accumulator interval referenced to  sampling clock (16.368MHz)

	ui32 PpsReserved0;// Reserved for PPS arming, register programming, width programming etc.
	ui32 PpsReserved1;
	ui32 PpsReserved2;

	ui32 Reserved0;
	ui32 Reserved1;
	ui32 Reserved2;
	ui32 Reserved3;
	ui32 Reserved4;
	ui32 Reserved5;
	ui32 Reserved6;
	ui32 Reserved7;
	ui32 Reserved8;
	ui32 Reserved9;

} GpsL1BbControl;

typedef struct {

	GpsL1BbChn ChannelParameter[MAX_CHANNELS];
	GpsL1BbStatus BasebandStatus;
	GpsL1BbControl BasebandControl;

} GpsL1Bb;

#endif /* GPSL1DR_H_ */
