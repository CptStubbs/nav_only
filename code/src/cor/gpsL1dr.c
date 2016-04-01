/*
 * gpsL1dr.c
 *
 *  Created on: Nov 23, 2014
 *      Author: Nagaraj
 */

#include "xparameters.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "scugic_header.h"
#include "xdevcfg.h"
#include "devcfg_header.h"
#include "xdmaps.h"
#include "dmaps_header.h"
#include "xemacps.h"
#include "xemacps_example.h"
#include "emacps_header.h"
#include "xgpio.h"
#include "gpio_header.h"
#include "xscutimer.h"
#include "scutimer_header.h"
#include "xscuwdt.h"
#include "scuwdt_header.h"

#include "chn.h"
#include "ccr.h"
#include "gpsL1dr.h"

//#define DBG

// Function definitions
static void sgi_handler(void *CallBackRef);
static void Measurement_InterruptHandler(void *CallBackRef);
unsigned Max2769_SPI_Config(void);
static u32 Baseband_InterruptHandler(void *CallBackRef);

// Global Variables Declaration
static s32 CorrelationValues[MAX_CHANNELS][6];
static u32 SampleCount[MAX_CHANNELS];
static u32 MeasurementValues[MAX_CHANNELS][3];
XGpio GpioOutput; /* The driver instance for GPIO Device configured as O/P */
void *CallBackRef;

static XScuGic intc;

struct chn_input_s chn_input;

//static s16 CorrelationValues[MAX_CHANNELS][6];
//static u32 MeasurementValues[MAX_CHANNELS][3];

const u16 InitG2Setting[] = { 0x3EC, 0x3D8, 0x3B0, 0x360, 0x096, 0x12C, 0x196,
		0x32C, 0x258, 0x374, 0x2E8, 0x3A0, 0x340, 0x280, 0x100, 0x200, 0x226,
		0x04C, 0x098, 0x130, 0x260, 0x0C0, 0x0CE, 0x270, 0x0E0, 0x1C0, 0x380,
		0x300, 0x056, 0x0AC, 0x158, 0x2B0, 0x160, 0x0B0, 0x316, 0x22C, 0x0B0,
		0x3C0, 0x21A, 0x0F8, 0x259, 0x257, 0x308, 0x31A, 0x18B, 0x2B1, 0x0BB,
		0x2BD, 0x21B, 0x3AD, 0x1CF, 0x2E7, 0x101, 0x2C1, 0x1B7, 0x3F8, 0x35D,
		0x1A2, 0x3A4, 0x18A, 0x1EC, 0x36D, 0x29F, 0x0D4, 0x107, 0x039, 0x0C9,
		0x11D, 0x2A0, 0x32B, 0x191, 0x082, 0x293, 0x0D1, 0x170, 0x134, 0x3F0,
		0x22A, 0x003, 0x041, 0x213, 0x2BC, 0x074, 0x334, 0x346, 0x3F4, 0x13B,
		0x0F7, 0x349, 0x295, 0x186, 0x1E8, 0x1D4, 0x05B, 0x0E5, 0x034, 0x2D3,
		0x2EB, 0x19E, 0x361, 0x345, 0x04E, 0x22E, 0x33F, 0x25F, 0x2E2, 0x2EE,
		0x27E, 0x018, 0x248, 0x1D6, 0x1C1, 0x132, 0x241, 0x15A, 0x290, 0x089,
		0x3DA, 0x0BC, 0x189, 0x215, 0x3B4, 0x164, 0x3C7, 0x1F1, 0x0BF, 0x3CE,
		0x16B, 0x055, 0x21C, 0x25A, 0x02A, 0x26F, 0x18E, 0x1C5, 0x01E, 0x381,
		0x052, 0x28C, 0x357, 0x243, 0x097, 0x14D, 0x031, 0x01B, 0x2E0, 0x2DC,
		0x2EC, 0x0D5, 0x211, 0x118, 0x217, 0x08B, 0x25E, 0x0FF, 0x388, 0x359,
		0x11B, 0x325, 0x10F, 0x1E2, 0x2EF, 0x137, 0x07B, 0x37B, 0x204, 0x2E6,
		0x3EF, 0x077, 0x0E8, 0x0A5, 0x033, 0x133, 0x0D7, 0x04A, 0x114, 0x119,
		0x3C1, 0x071, 0x20A, 0x2D2, 0x1BB, 0x2C4, 0x301, 0x0D3, 0x2B7, 0x272,
		0x05F, 0x38C, 0x174, 0x0BE, 0x123, 0x051, 0x387, 0x39F, 0x28D, 0x01A,
		0x33D, 0x3AE, 0x398, 0x185, 0x291, 0x1F2, 0x086, 0x3DF, 0x06D, 0x336,
		0x1E3, 0x35E, 0x26E

};

void InitGpsL1Bb() {

	u16 channel;
	u16 prn;

	// Initialize GPIOs
	XGpio_Initialize(&GpioOutput, XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&GpioOutput, 1, 0xF);
	XGpio_SetDataDirection(&GpioOutput, 2, 0x0);
	XGpio_DiscreteWrite(&GpioOutput, 2, 0x0);

	// Setup the Interrupt Controller
	XScuGic_Config *IntcConfig;
	// Initialize all the ARM interrupts (the default ones)
	Xil_ExceptionInit();
	// Lookup the configuration for the GIC instance that we created
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	//initialize the GIC
	XScuGic_CfgInitialize(&intc, IntcConfig, IntcConfig->CpuBaseAddress);
	//connect to Exception Handler
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler) XScuGic_InterruptHandler, &intc);

	//connect to the hardware - for baseband interrupt
	XScuGic_Connect(&intc, BASEBAND_INTERRUPT_ID,
			(Xil_ExceptionHandler) Baseband_InterruptHandler,
			(void *) CallBackRef);
	// Set the priority and trigger for the baseband interrupt (edge triggered in this case)
	XScuGic_SetPriorityTriggerType(&intc, BASEBAND_INTERRUPT_ID, 0x0, 3);// Set interrupt priority of 0  -- highest

	//connect to the hardware - for measurement interrupt
	XScuGic_Connect(&intc, MEASUREMENT_INTERRUPT_ID,
			(Xil_ExceptionHandler) Measurement_InterruptHandler,
			(void *) CallBackRef);
	// Set the priority and trigger for the baseband interrupt (edge triggered in this case)
	XScuGic_SetPriorityTriggerType(&intc, MEASUREMENT_INTERRUPT_ID, 0x8, 3);// Set interrupt priority of 8  -- second highest

	/* Baseband Setup */
	// Assert and Deassert the Baseband Reset line
	XGpio_DiscreteWrite(&GpioOutput, 2, 0x0);
	XGpio_DiscreteWrite(&GpioOutput, 2, 0x1);
	// Setup the accumulation interrupt rate
	//Xil_Out32(GPS_L1CA_BB_BASE_ADDRESS+(0xF2*4), (0x7CE0C0>>1));

	// Initialize each channel
	for (channel = 0; channel < MAX_CHANNELS; channel++) {
		prn = 4;
		Xil_Out32(
				GPS_L1CA_BB_BASE_ADDRESS + (channel * CHANNEL_ADDRESS_OFFSET)
						+ 0, InitG2Setting[prn]);
		Xil_Out32(
				GPS_L1CA_BB_BASE_ADDRESS + (channel * CHANNEL_ADDRESS_OFFSET)
						+ 4, NOMINAL_CARR_FREQ);
		Xil_Out32(
				GPS_L1CA_BB_BASE_ADDRESS + (channel * CHANNEL_ADDRESS_OFFSET)
						+ 8, NOMINAL_CODE_FREQ);
		Xil_Out32(
				GPS_L1CA_BB_BASE_ADDRESS + (channel * CHANNEL_ADDRESS_OFFSET)
						+ 12, 0x0); // Code Slew
		Xil_Out32(
				GPS_L1CA_BB_BASE_ADDRESS + (channel * CHANNEL_ADDRESS_OFFSET)
						+ 28, 0x0); // Epoch Counter Load
	}

	// Initialize Accumulator interval -- 0.5ms (16.368*4 clock). Value = (65472*0.5 - 1)
	Xil_Out32(GPS_L1CA_BB_BASE_ADDRESS + (15 * CHANNEL_ADDRESS_OFFSET) + 8,
			0x7FDF); //0xFFBF = 1ms, 0x7FDF = 0.5ms

	// Initialize Measurement TIC interval -- 0.5s (16.368*4 clock). Value = (65472000*0.5 - 1)
	Xil_Out32(GPS_L1CA_BB_BASE_ADDRESS + (15 * CHANNEL_ADDRESS_OFFSET) + 4,
			0x1F382FF);

}

u32 EnablePlatformInterrupts() {
	u32 status;

	status = XScuGic_Connect(&intc, SW_INT_ID,
			(Xil_ExceptionHandler) sgi_handler, (void *) CallBackRef);
	if (status != XST_SUCCESS) {
		print("error setting SGI");
		return XST_FAILURE;
	}

	// Enable the software interrupt
	XScuGic_Enable(&intc, SW_INT_ID);
	//Enable Baseband interrupt
	XScuGic_Enable(&intc, BASEBAND_INTERRUPT_ID);
	//Enable Measurement interrupt
	XScuGic_Enable(&intc, MEASUREMENT_INTERRUPT_ID);

	// Enable all the interrupts of ARM
	Xil_ExceptionEnable();

	return status;

}

u32 Baseband_InterruptHandler(void *CallBackRef) {
	u32 newdata;

	u32 status;
	u32 accum_count;
	u32 hardware_id;
	u8 channel;

#ifdef DBG
	u32 test_int = 0;
	u32 ie;
	u32 ip;
	u32 il;
	u32 qe;
	u32 qp;
	u32 ql;
#endif
	s32 n;

	fl32 carrval;
	fl32 codeval;

	//???? *intc_Baseband_LOCAL = (????? *) CallBackRef; -- Xilinx doesn't define how to handle callbackref for custom PL module generated interrupts

	//print("We are in the interrupt handler!!\n\r");

	hardware_id = Xil_In32(GPS_L1CA_BB_BASE_ADDRESS + (0xEF * 4));
	if (hardware_id != 0x64) {
		//printf("Something wrong with the Baseband Memory access\n\r");
		hardware_id += 0x64;
	}

	status = Xil_In32(GPS_L1CA_BB_BASE_ADDRESS + (0xE0 * 4));//read the status to acknowledge the interrupt so that the baseband can clear the interrupt
	newdata = Xil_In32(GPS_L1CA_BB_BASE_ADDRESS + (0xE1 * 4));
	accum_count = Xil_In32(GPS_L1CA_BB_BASE_ADDRESS + (0xE3 * 4));

#ifdef DBG
	if(newdata & 0x1)
	{
		ie = Xil_In32(GPS_L1CA_BB_BASE_ADDRESS+(0x08*4));
		ip = Xil_In32(GPS_L1CA_BB_BASE_ADDRESS+(0x09*4));
		il = Xil_In32(GPS_L1CA_BB_BASE_ADDRESS+(0x0A*4));
		qe = Xil_In32(GPS_L1CA_BB_BASE_ADDRESS+(0x0B*4));
		qp = Xil_In32(GPS_L1CA_BB_BASE_ADDRESS+(0x0C*4));
		ql = Xil_In32(GPS_L1CA_BB_BASE_ADDRESS+(0x0D*4));

	}

	test_int = 1;
#endif

	for (channel = 0; channel < MAX_CHANNELS; channel++) {

		if ((newdata >> channel) & 0x1) {
			CorrelationValues[channel][0] = Xil_In32(
					GPS_L1CA_BB_BASE_ADDRESS
							+ (channel * CHANNEL_ADDRESS_OFFSET) + 32);
			CorrelationValues[channel][1] = Xil_In32(
					GPS_L1CA_BB_BASE_ADDRESS
							+ (channel * CHANNEL_ADDRESS_OFFSET) + 36);
			CorrelationValues[channel][2] = Xil_In32(
					GPS_L1CA_BB_BASE_ADDRESS
							+ (channel * CHANNEL_ADDRESS_OFFSET) + 40);
			CorrelationValues[channel][3] = Xil_In32(
					GPS_L1CA_BB_BASE_ADDRESS
							+ (channel * CHANNEL_ADDRESS_OFFSET) + 44);
			CorrelationValues[channel][4] = Xil_In32(
					GPS_L1CA_BB_BASE_ADDRESS
							+ (channel * CHANNEL_ADDRESS_OFFSET) + 48);
			CorrelationValues[channel][5] = Xil_In32(
					GPS_L1CA_BB_BASE_ADDRESS
							+ (channel * CHANNEL_ADDRESS_OFFSET) + 52);
			SampleCount[channel] = Xil_In32(
					GPS_L1CA_BB_BASE_ADDRESS
							+ (channel * CHANNEL_ADDRESS_OFFSET) + 24);
			//ccr.res[channel].new = (bool) ((newdata>>channel) & 0x1);
		}
	}

	//printf("Running '_test_chn.c' as main\n");

	//srand((unsigned) time(NULL));	// Initialize random generator

	// Spawn chn.c thread
	//pthread_t th0;
	//pthread_create(&th0, NULL, chn, &ccr);
	// NCS Instead of spawning a thread, generate a software interrupt

//		Status = XScuGic_SoftwareIntr(&intc,SW_INT_ID, XSCUGIC_SPI_CPU0_MASK);
//		if (Status != XST_SUCCESS) {
//				//print("error triggering SGI");
//				return XST_FAILURE;
//		}
	//print("\n\r setting up SW Interrupts\n\r");

	//nanosleep((struct timespec[]){{0, 10000000}}, NULL);	// sleep for 10 ms
	//nanosleep((struct timespec[]){{0, 10000000}}, NULL);	// sleep for 10 ms
	//nanosleep((struct timespec[]){{0, 10000000}}, NULL);	// sleep for 10 ms

	// Grab new data
	for (n = 0; n < MAX_CHANNELS; n++) {
		//if (ccr.ctr[n].new == true){
		// We have new control parameters -
		//printf("prn: %02d, %+08.1f %08u\n",ccr.ctr[n].prn, ((fl64) ccr.ctr[n].carrFreq)/(8388608.0/127875.0)-4.092e6, ccr.ctr[n].codeSlew);
		//fflush(stdout);
		// printf("M=%4d\n",m);
		// fflush(stdout);
		//ccr.ctr[n].new = false;
		//fflush(stdout);
		// Update correlator dumps
		chn_input.ccr->res[n].EI = (si32) CorrelationValues[n][0];
		chn_input.ccr->res[n].EQ = (si32) CorrelationValues[n][1];
		chn_input.ccr->res[n].PI = (si32) CorrelationValues[n][2];
		chn_input.ccr->res[n].PQ = (si32) CorrelationValues[n][3];
		chn_input.ccr->res[n].LI = (si32) CorrelationValues[n][4];
		chn_input.ccr->res[n].LQ = (si32) CorrelationValues[n][5];
		chn_input.ccr->res[n].smp = (ui32) SampleCount[n];
		chn_input.ccr->res[n].newCorr = (bool) (((newdata >> n) & 0x1) && true);
		//}
	}

	chn_main(&chn_input);

	// Writing ccr control values back to FPGA
	for (channel = 0; channel < MAX_CHANNELS; channel++) {
		if (chn_input.ccr->ctr[channel].newFreq == true) {
//				if (chn_input.ccr->ctr[channel].newPrn){
//					Xil_Out32(GPS_L1CA_BB_BASE_ADDRESS+(channel*CHANNEL_ADDRESS_OFFSET)+0, InitG2Setting[(u16) (chn_input.ccr->ctr[channel].prn-1)]);
//				}
			carrval = chn_input.ccr->ctr[channel].carrFreq * (fl32) FLOAT2INT;
			codeval = chn_input.ccr->ctr[channel].codeFreq * (fl32) FLOAT2INT;
			Xil_Out32(
					GPS_L1CA_BB_BASE_ADDRESS
							+ (channel * CHANNEL_ADDRESS_OFFSET) + 4,
					(u32) carrval);
			Xil_Out32(
					GPS_L1CA_BB_BASE_ADDRESS
							+ (channel * CHANNEL_ADDRESS_OFFSET) + 8,
					(u32) codeval);
			chn_input.ccr->ctr[channel].newFreq = false;
		}
		if (chn_input.ccr->ctr[channel].newSlew == true) {
			Xil_Out32(
					GPS_L1CA_BB_BASE_ADDRESS
							+ (channel * CHANNEL_ADDRESS_OFFSET) + 12,
					chn_input.ccr->ctr[channel].codeSlew); // Code Slew
			chn_input.ccr->ctr[channel].newSlew = false;
		}

	}

	//pthread_join(th0, NULL);
	return EXIT_SUCCESS;

}

void Measurement_InterruptHandler(void *CallBackRef) {
	u32 status;
	u16 channel;

	status = Xil_In32(GPS_L1CA_BB_BASE_ADDRESS + (0xE0 * 4));

	for (channel = 0; channel < MAX_CHANNELS; channel++) {

		MeasurementValues[channel][0] = Xil_In32(
				GPS_L1CA_BB_BASE_ADDRESS + (channel * CHANNEL_ADDRESS_OFFSET)
						+ 16);
		MeasurementValues[channel][1] = Xil_In32(
				GPS_L1CA_BB_BASE_ADDRESS + (channel * CHANNEL_ADDRESS_OFFSET)
						+ 20);
		MeasurementValues[channel][2] = Xil_In32(
				GPS_L1CA_BB_BASE_ADDRESS + (channel * CHANNEL_ADDRESS_OFFSET)
						+ 24);

	}

}

void sgi_handler(void *CallBackRef) {
	u32 INT;

	//BaseAddress = (u32)CallbackRef;
	INT = Xil_In32(0xF8F0010C);
	Xil_Out32(0xF8F0010C, INT);
	//print("SGI****");
}

