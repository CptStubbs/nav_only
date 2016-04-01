#include "xparameters.h"
#include "xil_io.h"

#define MAX2769SPI_BASEADDR XPAR_AXILITE_MMPL_0_S00_AXI_BASEADDR

unsigned Max2769_SPI_Config() {

	u32 test;

	u32 dbg1;

#define SPI_BUSY 0x01
#define SPI_IDLEN 0x02
#define SPI_SHDN 0x04
#define SPI_PLLLD 0x08

#define SPI_DATA_REG 0x00
#define SPI_CONFIG_REG 0x04

//	Xil_Out32(MAX2769SPI_BASEADDR+SPI_CONFIG_REG, (SPI_IDLEN | SPI_SHDN)); //enable fe

//test = 0xA2953330; //lna1, fcen 5.07mhz, 4.2MHz FBW
	test = 0xA2953230; //lna1, fcen 5.07mhz, 2.5MHz FBW
	//test = 0xA2954C30; // lna 1
	//test = 0xA2936630; // lna 2
	Xil_Out32(MAX2769SPI_BASEADDR + SPI_DATA_REG, test);

	dbg1 = Xil_In32(MAX2769SPI_BASEADDR + 0);

	while (Xil_In32(MAX2769SPI_BASEADDR + SPI_CONFIG_REG) & SPI_BUSY)
		;

	//test = 0x05502AC1; //select analog mode, independent iq
	test = 0x055028C1; //select digital mode, independent iq
	//test = 0x0550A8C1; //select digital mode, locked iq
	Xil_Out32(MAX2769SPI_BASEADDR + SPI_DATA_REG, test);
	while (Xil_In32(MAX2769SPI_BASEADDR + SPI_CONFIG_REG) & SPI_BUSY)
		;

	test = 0xEAFE1DC2;
	Xil_Out32(MAX2769SPI_BASEADDR + SPI_DATA_REG, test);
	while (Xil_In32(MAX2769SPI_BASEADDR + SPI_CONFIG_REG) & SPI_BUSY)
		;

	//test = 0x9EC00003; //clk*1
	test = 0x9EC00003; //clk*2
	Xil_Out32(MAX2769SPI_BASEADDR + SPI_DATA_REG, test);
	while (Xil_In32(MAX2769SPI_BASEADDR + SPI_CONFIG_REG) & SPI_BUSY)
		;

	test = 0x013A0084;
	Xil_Out32(MAX2769SPI_BASEADDR + SPI_DATA_REG, test);
	while (Xil_In32(MAX2769SPI_BASEADDR + SPI_CONFIG_REG) & SPI_BUSY)
		;

	test = 0x0AC08705; //fractional division ratio (0.42MHz) -> 1570.42MHz LO
	Xil_Out32(MAX2769SPI_BASEADDR + SPI_DATA_REG, test);
	while (Xil_In32(MAX2769SPI_BASEADDR + SPI_CONFIG_REG) & SPI_BUSY)
		;

	test = 0x80000006;
	Xil_Out32(MAX2769SPI_BASEADDR + SPI_DATA_REG, test);
	while (Xil_In32(MAX2769SPI_BASEADDR + SPI_CONFIG_REG) & SPI_BUSY)
		;

	test = 0x10061B27;
	Xil_Out32(MAX2769SPI_BASEADDR + SPI_DATA_REG, test);
	while (Xil_In32(MAX2769SPI_BASEADDR + SPI_CONFIG_REG) & SPI_BUSY)
		;

	test = 0x1E0F4018;
	Xil_Out32(MAX2769SPI_BASEADDR + SPI_DATA_REG, test);
	while (Xil_In32(MAX2769SPI_BASEADDR + SPI_CONFIG_REG) & SPI_BUSY)
		;

	test = 0x14C04029;
	Xil_Out32(MAX2769SPI_BASEADDR + SPI_DATA_REG, test);
	while (Xil_In32(MAX2769SPI_BASEADDR + SPI_CONFIG_REG) & SPI_BUSY)
		;

	Xil_Out32(MAX2769SPI_BASEADDR + SPI_CONFIG_REG, (SPI_IDLEN | SPI_SHDN)); //enable fe

	dbg1 = Xil_In32(MAX2769SPI_BASEADDR + SPI_CONFIG_REG);
	dbg1 = dbg1 & SPI_PLLLD;

	// Wait for PLL to lock
	while (!(Xil_In32(MAX2769SPI_BASEADDR + SPI_CONFIG_REG) & (u32) SPI_PLLLD))
		;

	return 1;

}
