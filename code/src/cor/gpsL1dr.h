/*
 * gpsL1dr.h
 *
 *  Created on: Nov 23, 2014
 *      Author: Nagaraj
 */

#ifndef GPSL1DR_H_
#define GPSL1DR_H_

#include "xparameters.h"
#include "xil_types.h"
#include "xscugic.h"
#include "xgpio.h"
#include "gpio_header.h"
#include "chn.h"
#include "ccr.h"

#define MAX_CHANNELS 12
#define NUM_CODE_TAPS 3
#define CHANNEL_ADDRESS_OFFSET (0x10*4) // the *4 is for byte addressing

#define GPS_L1CA_BB_BASE_ADDRESS XPAR_EMC_0_S_AXI_MEM0_BASEADDR // Vivado doesn't seem to change the macro for manually entered address base values

// Nominal values for MAX2769 default configuration
// 4.092MHz resultant carrier frequency
// 1.023MHz code frequency
// 16.368MHz is the sampling frequency
// divide by 4 is because baseband is effectively clocked at 4*16.368MHz
#define NOMINAL_CARR_FREQ 	(0x10000000 >>2)
#define NOMINAL_CODE_FREQ	(0x04000000 >>2)


// Define a structure for the baseband interface
#if (3 == NUM_CODE_TAPS)
typedef struct {

	u16 prnG2;		//G2 register
	u16 prnG1;		//G1 register

	u32 CarNCOLoad;		// Carrier NCO value = (car_nco_freq * 2^30) / nco_clk_freq

	u32 CodeNCOLoad;	// Code NCO value	= (2* code_nco_freq * 2^29) / nco_clk_freq

	u16 CodeSlewLoad;	// Code slew in number of half chips
	u16 Reserved0;

	u32 CarMeas;	// bits 31:10 => Carrier cycles within the meas interval, bits 9:0 => fractional carrier phase

	u32 CodeMeas;	// bits 20:10 => Number of half chips cleared at the accumulator dump, latched at the meas TIC
					// bits 9:0 => fractional code phase
	u16 EpochMeas;	// bits 4:0 => number of code sequences ranges from 0-19 (ideally 1ms), cleared when Epoch Register is loaded,
					// latched at measurement TIC, incremented at every accumulator dump.
					// bits 10:5 => number of bit sequences (typically 20ms)
	u16 EpochInst;	// Same as EpochMeas but instantaneous. Latched when the register is read.

	u16 EpochLoad;	// Epoch register to load
	u16 Reserved1;

	s32 IEarly;			// 1ms accumulator values. Actually 16 bit value sign extended
	s32 QEarly;
	s32 IPrompt;
	s32 QPrompt;
	s32 ILate;
	s32 QLate;

	u32 Reserved2;
	u32 Reserved3;

} GpsL1BbChn;

#elif (8 == NUM_CODE_TAPS)

typedef struct {

	u16 prnG1;		//G1 register
	u16 prnG2;		//G2 register
	u32 CarNCOLoad;		// Carrier NCO value = (car_nco_freq * 2^30) / nco_clk_freq
	u32 CodeNCOLoad;	// Code NCO value	= (2* code_nco_freq * 2^29) / nco_clk_freq
	u16 CodeSlewLoad;	// Code slew in number of half chips
	u16 Reserved0;
	u32 CarMeas;	// bits 31:10 => Carrier cycles within the meas interval, bits 9:0 => fractional carrier phase
	u32 CodeMeas;	// bits 20:10 => Number of half chips cleared at the accumulator dump, latched at the meas TIC
					// bits 9:0 => fractional code phase
	u16 EpochMeas;	// bits 4:0 => number of code sequences ranges from 0-19 (ideally 1ms), cleared when Epoch Register is loaded,
					// latched at measurement TIC, incremented at every accumulator dump.
					// bits 10:5 => number of bit sequences (typically 20ms)
	u16 EpochInst;	// Same as EpochMeas but instantaneous. Latched when the register is read.
	u16 EpochLoad;	// Epoch register to load
	u16 Reserved1;

	s16 I0;			// 1ms accumulator values. Note that the first three taps are aligned with 3 tap design
	s16 I4;

	s16 Q0;
	s16 Q4;

	s16 I1;
	s16 I5;

	s16 Q1;
	s16 Q5;

	s16 I2;
	s16 I6;

	s16 Q2;
	s16 Q6;

	s16 I3;
	s16 I7;

	s16 Q3;
	s16 Q7;

} GpsL1BbChn;

#endif

typedef struct {
	u32 InterruptClear; // bit 1 => Accumulator interrupt clear, bit 0 => Meas TIC interrupt clear
	u32 AccDataReady; // bits 0:MAX_CHANNELS-1 indicate the availability of new accumulator data;
	u32 TICRegCount; // 24 bits of measurement interval register
	u32 AccRegCount; // 24 bits of accumulator interval register
	u32 AccDataReadPending; // bits 0:MAX_CHANNELS-1 => set if the processor fails to read the cor values on the acc interrupt
							// Only one miss indicated (multiple missed reads not indicated).
	u32 Reserved0;
	u32 Reserved1;
	u32 Reserved2;
	u32 Reserved3;
	u32 Reserved4;
	u32 Reserved5;
	u32 Reserved6;
	u32 Reserved7;
	u32 Reserved8;
	u32 Reserved9;

	u32 HardwareID; // Hardware version of the GPSL1 Baseband. Example hardware version  = 1.00 = 100d = 0x64h
} GpsL1BbStatus;

typedef struct {
	u32 BbSoftReset; // When write operation (any content) is performed on this register, entire Bb is reset
					 // This is in addition to the hard reset through processor GPIO.
	u32 TICIntervalProg; // 24 bits of measurement interval referenced to  sampling clock (16.368MHz)
	u32 AccIntervalProg; // 24 bits of accumulator interval referenced to  sampling clock (16.368MHz)

	u32 PpsReserved0;	// Reserved for PPS arming, register programming, width programming etc.
	u32 PpsReserved1;
	u32 PpsReserved2;

	u32 Reserved0;
	u32 Reserved1;
	u32 Reserved2;
	u32 Reserved3;
	u32 Reserved4;
	u32 Reserved5;
	u32 Reserved6;
	u32 Reserved7;
	u32 Reserved8;
	u32 Reserved9;

} GpsL1BbControl;

typedef struct {

	GpsL1BbChn 		ChannelParameter[MAX_CHANNELS];
	GpsL1BbStatus 	BasebandStatus;
	GpsL1BbControl	BasebandControl;

} GpsL1Bb;


#define FLOAT2INT (1073.741824e6/65.472e6)

//define interrupt related macros
#define INTC_DEVICE_ID 				XPAR_SCUGIC_0_DEVICE_ID
#define BASEBAND_INTERRUPT_ID  		XPAR_FABRIC_BASEBAND_WRAPPER_0_IRQ_INTR
#define MEASUREMENT_INTERRUPT_ID	XPAR_FABRIC_BASEBAND_WRAPPER_0_TIC_INTR
#define SW_INT_ID 					0


extern void InitGpsL1Bb(void);
extern unsigned Max2769_SPI_Config(void);
extern u32 EnablePlatformInterrupts(void);

#endif /* GPSL1DR_H_ */
