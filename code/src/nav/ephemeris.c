/*
 * ephemeris.c
 *
 *  Created on: May 18, 2015
 *      Author: sara
 */
#include "ephemeris.h"

#include <conf_nav.h>
#include "initEph.h"

#include "parityCheck.h"
#include "twosComp2dec.h"
#include "customDataTypes.h"
#include "utils.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
si32 ephemeris(
/* Output */
eph_s * eph, fl64 * TOW, si32 * subframeID,
/* Input */
si32 PRN, pi8 * bits, si32 bitsSize, pi8 D29Star, pi8 D30Star) {

	si32 tempSubFrameID = *subframeID;
	pi8 subframe[301];
	pi8 subframeTemp[4];
	pi8 subframeTemp1[30];
	pi8 subframeTemp2[18];
	pi8 subframeCheck[9];
	si32 inda = 0;
	si32 pass = 0;
	si8 word[30];
	pi8 temp1[3];
	pi8 temp2[9];
	pi8 temp3[11];
	pi8 temp4[9];
	pi8 temp5[25];
	pi8 temp6[33];
	fl64 subcheck = 0;
	fl64 dataID = 0;
	fl64 svID = 0;
	fl64 a0 = 0;
	fl64 tot = 0;

	if (bitsSize != 300) {
		*TOW = 0;
		fprintf(stderr, "The parameter BITS must contain 300 bits!");
		return EXIT_FAILURE;
	}/*for if statement*/
	memcpy(subframe, bits, 301 * sizeof(pi8));

	for (inda = 0; inda < 10; inda++) {
		memcpy(word, &subframe[30 * (inda)], 30 * sizeof(pi8));
		parityCheck(word, &pass, 30, (si8) D29Star, (si8) D30Star);
		// Need to update the subframe by word after parityCheck.
		memcpy(&subframe[30 * inda], word, 30 * sizeof(pi8));

		if (pass == 0) {
			*TOW = 0;
			fprintf(stderr, "Parity Check failed\n");
			return EXIT_FAILURE;
		}/*for if statement*/

		D29Star = subframe[(30 * (inda + 1)) - 2];
		D30Star = subframe[(30 * (inda + 1)) - 1];
	}/*for for loop*/

	/* Decode the sub-frames ==================================================*/
	memcpy(subframeTemp, &subframe[49], 3 * sizeof(pi8));
	subframeTemp[3] = '\0'; // Never forget the null bits
	tempSubFrameID = bin2dec(subframeTemp);

	if (tempSubFrameID > 5 || tempSubFrameID < 1) {
		fprintf(stderr, "Wrong subframe ID(%d), IF DATA IS NOT RELIABLE!\n",
				tempSubFrameID);
		*TOW = 0;
		return EXIT_FAILURE;
	}/*for if statement*/

	/* --- Decode sub-frame based on the sub-frames id ----------------------
	 The task is to select the necessary bits and convert them to decimal
	 numbers. For more details on sub-frame contents please refer to GPS
	 ICD (IS-GPS-200D).*/

	switch (tempSubFrameID) {
	case 1:/*--- It is subframe 1 -------------------------------------*/
		/* It contains WN, SV clock corrections, health and accuracy
		 tranmission time of message*/
		memcpy(subframeCheck, &subframe[210], sizeof(pi8) * 8);
		subframeCheck[8] = '\0';
		subcheck = bin2dec(subframeCheck);

		if (eph->nav[PRN].IODC8bits_sf1 == 0) {
			eph->navStatus[PRN][0] = 1; /* status update*/

			memcpy(subframeTemp1, &subframe[30], sizeof(pi8) * 17);
			subframeTemp1[17] = '\0';
			eph->nav[PRN].TOW = bin2dec(subframeTemp1) * 6;

			memcpy(subframeTemp1, &subframe[60], sizeof(pi8) * 10);
			subframeTemp1[10] = '\0';
			eph->nav[PRN].weekNumber = bin2dec(subframeTemp1) + 1024;

			memcpy(subframeTemp1, &subframe[72], sizeof(pi8) * 4);
			subframeTemp1[4] = '\0';
			eph->nav[PRN].accuracy = bin2dec(subframeTemp1);

			memcpy(subframeTemp1, &subframe[76], sizeof(pi8) * 6);
			subframeTemp1[6] = '\0';
			eph->nav[PRN].health = bin2dec(subframeTemp1);

			memcpy(subframeTemp1, &subframe[196], sizeof(pi8) * 8);
			subframeTemp1[8] = '\0';
			twosComp2dec(&eph->nav[PRN].T_GD, subframeTemp1, 8);
			eph->nav[PRN].T_GD *= pow(2, -31);

			memset(temp3, 0, 11 * sizeof(pi8));
			memcpy(temp1, &subframe[82], sizeof(pi8) * 2);
			temp1[2] = '\0';
			memcpy(temp2, &subframe[210], sizeof(pi8) * 8);
			temp2[8] = '\0';
			strcat(temp3, temp1);
			strcat(temp3, temp2);
			eph->nav[PRN].IODC = bin2dec(temp3);

			memcpy(subframeTemp1, &subframe[210], sizeof(pi8) * 8);
			subframeTemp1[8] = '\0';
			eph->nav[PRN].IODC8bits_sf1 = bin2dec(subframeTemp1);

			memcpy(subframeTemp1, &subframe[218], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			eph->nav[PRN].t_oc = bin2dec(subframeTemp1) * pow(2, 4);

			memcpy(subframeTemp1, &subframe[240], sizeof(pi8) * 8);
			subframeTemp1[8] = '\0';
			twosComp2dec(&eph->nav[PRN].a_f2, subframeTemp1, 8);
			eph->nav[PRN].a_f2 *= pow(2, -55);

			memcpy(subframeTemp1, &subframe[248], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			twosComp2dec(&eph->nav[PRN].a_f1, subframeTemp1, 16);
			eph->nav[PRN].a_f1 *= pow(2, (-43));

			memcpy(subframeTemp1, &subframe[270], sizeof(pi8) * 22);
			subframeTemp1[22] = '\0';

			twosComp2dec(&eph->nav[PRN].a_f0, subframeTemp1, 22);
			eph->nav[PRN].a_f0 *= pow(2, (-31));
		}/*for if statement*/
		else if (eph->nav[PRN].IODC8bits_sf1 != subcheck) {

			eph->navTempStatus[PRN][0] = 1; /* status update*/

			memcpy(subframeTemp1, &subframe[30], sizeof(pi8) * 17);
			subframeTemp1[17] = '\0';
			eph->navTemp[PRN].TOW = bin2dec(subframeTemp1) * 6;

			memcpy(subframeTemp1, &subframe[60], sizeof(pi8) * 10);
			subframeTemp1[10] = '\0';
			eph->navTemp[PRN].weekNumber = bin2dec(subframeTemp1) + 1024;

			memcpy(subframeTemp1, &subframe[72], sizeof(pi8) * 4);
			subframeTemp1[4] = '\0';
			eph->navTemp[PRN].accuracy = bin2dec(subframeTemp1);

			memcpy(subframeTemp1, &subframe[76], sizeof(pi8) * 6);
			subframeTemp1[6] = '\0';
			eph->navTemp[PRN].health = bin2dec(subframeTemp1);

			memcpy(subframeTemp1, &subframe[196], sizeof(pi8) * 8);
			subframeTemp1[8] = '\0';
			twosComp2dec(&eph->navTemp[PRN].T_GD, subframeTemp1, 8);
			eph->navTemp[PRN].T_GD *= pow(2, -31);

			memset(temp3, 0, 11 * sizeof(pi8));
			memcpy(temp1, &subframe[82], sizeof(pi8) * 2);
			temp1[2] = '\0';
			memcpy(temp2, &subframe[210], sizeof(pi8) * 8);
			temp2[8] = '\0';
			strcat(temp3, temp1);
			strcat(temp3, temp2);
			eph->navTemp[PRN].IODC = bin2dec(temp3);
			memcpy(subframeTemp1, &subframe[210], sizeof(pi8) * 8);
			subframeTemp1[8] = '\0';

			eph->navTemp[PRN].IODC8bits_sf1 = bin2dec(subframeTemp1);

			memcpy(subframeTemp1, &subframe[218], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			eph->navTemp[PRN].t_oc = bin2dec(subframeTemp1) * pow(2, 4);

			memcpy(subframeTemp1, &subframe[240], sizeof(pi8) * 8);
			subframeTemp1[8] = '\0';
			twosComp2dec(&eph->navTemp[PRN].a_f2, subframeTemp1, 8);
			eph->navTemp[PRN].a_f2 *= pow(2, -55);

			memcpy(subframeTemp1, &subframe[248], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			twosComp2dec(&eph->navTemp[PRN].a_f1, subframeTemp1, 16);
			eph->navTemp[PRN].a_f1 *= pow(2, (-43));

			memcpy(subframeTemp1, &subframe[270], sizeof(pi8) * 22);
			subframeTemp1[22] = '\0';
			twosComp2dec(&eph->navTemp[PRN].a_f0, subframeTemp1, 22);
			eph->navTemp[PRN].a_f0 *= pow(2, (-31));

		}/*for else if statement*/
		break;
	case 2:/*--- It is subframe 2 -------------------------------------
	 It contains first part of ephemeris paraTempmeters*/
		memcpy(subframeCheck, &subframe[60], sizeof(pi8) * 8);
		subframeCheck[8] = '\0';
		subcheck = bin2dec(subframeCheck);

		if (eph->nav[PRN].IODE_sf2 == 0) {
			eph->navStatus[PRN][1] = 1; /* status update*/

			memcpy(subframeTemp1, &subframe[60], sizeof(pi8) * 8);
			subframeTemp1[8] = '\0';
			eph->nav[PRN].IODE_sf2 = bin2dec(subframeTemp1);

			memcpy(subframeTemp1, &subframe[68], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			twosComp2dec(&eph->nav[PRN].C_rs, subframeTemp1, 16);
			eph->nav[PRN].C_rs *= pow(2, -5);

			memcpy(subframeTemp1, &subframe[90], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			twosComp2dec(&eph->nav[PRN].deltan, subframeTemp1, 16);
			eph->nav[PRN].deltan *= pow(2, -43) * M_PI;

			memset(temp6, 0, sizeof(pi8) * 33);
			memcpy(temp4, &subframe[106], sizeof(pi8) * 8);
			temp4[8] = '\0';
			memcpy(temp5, &subframe[120], sizeof(pi8) * 24);
			temp5[24] = '\0';
			strcat(temp6, temp4);
			strcat(temp6, temp5);
			twosComp2dec(&eph->nav[PRN].M_0, temp6, 32);
			eph->nav[PRN].M_0 *= pow(2, -31) * M_PI;

			memcpy(subframeTemp1, &subframe[150], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			twosComp2dec(&eph->nav[PRN].C_uc, subframeTemp1, 16);
			eph->nav[PRN].C_uc *= pow(2, -29);

			/*reuse temp4, 5 and 6 since the lengths are the same*/
			memset(temp6, 0, sizeof(pi8) * 33);
			memcpy(temp4, &subframe[166], sizeof(pi8) * 8);
			temp4[8] = '\0';
			memcpy(temp5, &subframe[180], sizeof(pi8) * 24);
			temp5[24] = '\0';
			strcat(temp6, temp4);
			strcat(temp6, temp5);
			temp6[32] = '\0';
			eph->nav[PRN].e = bin2dec(temp6) * pow(2, -33);

			memcpy(subframeTemp1, &subframe[210], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			twosComp2dec(&eph->nav[PRN].C_us, subframeTemp1, 16);
			eph->nav[PRN].C_us *= pow(2, -29);

			/*reuse temp4, 5 and 6 since the lengths are the same*/
			memset(temp6, 0, sizeof(pi8) * 33);
			memcpy(temp4, &subframe[226], sizeof(pi8) * 8);
			temp4[8] = '\0';
			memcpy(temp5, &subframe[240], sizeof(pi8) * 24);
			temp5[24] = '\0';
			strcat(temp6, temp4);
			strcat(temp6, temp5);
			eph->nav[PRN].sqrtA = bin2dec(temp6) * pow(2, -19);

			memcpy(subframeTemp1, &subframe[270], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			eph->nav[PRN].t_oe = bin2dec(subframeTemp1) * pow(2, 4);

		} else if (eph->nav[PRN].IODE_sf2 != subcheck) {

			eph->navTempStatus[PRN][1] = 1; /* status update*/

			memcpy(subframeTemp1, &subframe[60], sizeof(pi8) * 8);
			subframeTemp1[8] = '\0';
			eph->navTemp[PRN].IODE_sf2 = bin2dec(subframeTemp1);

			memcpy(subframeTemp1, &subframe[68], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			twosComp2dec(&eph->navTemp[PRN].C_rs, subframeTemp1, 16);
			eph->navTemp[PRN].C_rs *= pow(2, -5);

			memcpy(subframeTemp1, &subframe[90], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			twosComp2dec(&eph->navTemp[PRN].deltan, subframeTemp1, 16);
			eph->navTemp[PRN].deltan *= pow(2, -43) * M_PI;

			memset(temp6, 0, sizeof(pi8) * 33);
			memcpy(temp4, &subframe[106], sizeof(pi8) * 8);
			temp4[8] = '\0';
			memcpy(temp5, &subframe[120], sizeof(pi8) * 24);
			temp5[24] = '\0';
			strcat(temp6, temp4);
			strcat(temp6, temp5);
			twosComp2dec(&eph->navTemp[PRN].M_0, temp6, 32);
			eph->navTemp[PRN].M_0 *= pow(2, -31) * M_PI;

			memcpy(subframeTemp1, &subframe[150], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			twosComp2dec(&eph->navTemp[PRN].C_uc, subframeTemp1, 16);
			eph->navTemp[PRN].C_uc *= pow(2, -29);

			/*reuse temp4, 5 and 6 since the lengths are the same*/
			memset(temp6, 0, sizeof(pi8) * 33);
			memcpy(temp4, &subframe[166], sizeof(pi8) * 8);
			temp4[8] = '\0';
			memcpy(temp5, &subframe[180], sizeof(pi8) * 24);
			temp5[24] = '\0';
			strcat(temp6, temp4);
			strcat(temp6, temp5);
			temp6[32] = '\0';
			eph->navTemp[PRN].e = bin2dec(temp6) * pow(2, -33);

			memcpy(subframeTemp1, &subframe[210], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			twosComp2dec(&eph->navTemp[PRN].C_us, subframeTemp1, 16);
			eph->navTemp[PRN].C_us *= pow(2, -29);

			/*reuse temp4, 5 and 6 since the lengths are the same*/
			memset(temp6, 0, sizeof(pi8) * 33);
			memcpy(temp4, &subframe[226], sizeof(pi8) * 8);
			temp4[8] = '\0';
			memcpy(temp5, &subframe[240], sizeof(pi8) * 24);
			temp5[24] = '\0';
			strcat(temp6, temp4);
			strcat(temp6, temp5);
			eph->navTemp[PRN].sqrtA = bin2dec(temp6) * pow(2, -19);

			memcpy(subframeTemp1, &subframe[270], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			eph->navTemp[PRN].t_oe = bin2dec(subframeTemp1) * pow(2, 4);
		}
		break;
	case 3:/*--- It is subframe 3 -------------------------------------
	 It contains second part of ephemeris parameters*/
		memcpy(subframeCheck, &subframe[270], sizeof(pi8) * 8);
		subframeCheck[8] = '\0';
		subcheck = bin2dec(subframeCheck);

		if (eph->nav[PRN].IODE_sf3 == 0) {
			eph->navStatus[PRN][2] = 1; /* status update*/
			memcpy(subframeTemp1, &subframe[60], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			twosComp2dec(&eph->nav[PRN].C_ic, subframeTemp1, 16);
			eph->nav[PRN].C_ic *= pow(2, -29);

			/*Reuse the temp4,5,6*/
			memset(temp6, 0, sizeof(pi8) * 33);
			memcpy(temp4, &subframe[76], sizeof(pi8) * 8);
			temp4[8] = '\0';
			memcpy(temp5, &subframe[90], sizeof(pi8) * 24);
			temp5[24] = '\0';
			strcat(temp6, temp4);
			strcat(temp6, temp5);
			twosComp2dec(&eph->nav[PRN].omega_0, temp6, strlen(temp6));
			eph->nav[PRN].omega_0 *= pow(2, -31);
			eph->nav[PRN].omega_0 *= M_PI;

			memcpy(subframeTemp1, &subframe[120], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			twosComp2dec(&eph->nav[PRN].C_is, subframeTemp1, 16);
			eph->nav[PRN].C_is *= pow(2, -29);

			/*Reuse the temp4,5,6*/
			memset(temp6, 0, sizeof(pi8) * 33);
			memcpy(temp4, &subframe[136], sizeof(pi8) * 8);
			temp4[8] = '\0';
			memcpy(temp5, &subframe[150], sizeof(pi8) * 24);
			temp5[24] = '\0';
			strcat(temp6, temp4);
			strcat(temp6, temp5);
			twosComp2dec(&eph->nav[PRN].i_0, temp6, strlen(temp6));
			eph->nav[PRN].i_0 *= pow(2, -31);
			eph->nav[PRN].i_0 *= M_PI;

			memcpy(subframeTemp1, &subframe[180], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			twosComp2dec(&eph->nav[PRN].C_rc, subframeTemp1, 16);
			eph->nav[PRN].C_rc *= pow(2, -5);

			/*Reuse the temp4,5,6*/
			memset(temp6, 0, sizeof(pi8) * 33);
			memcpy(temp4, &subframe[196], sizeof(pi8) * 8);
			temp4[8] = '\0';
			memcpy(temp5, &subframe[210], sizeof(pi8) * 24);
			temp5[24] = '\0';
			strcat(temp6, temp4);
			strcat(temp6, temp5);
			twosComp2dec(&eph->nav[PRN].omega, temp6, strlen(temp6));
			eph->nav[PRN].omega *= pow(2, -31);
			eph->nav[PRN].omega *= M_PI;

			memcpy(subframeTemp1, &subframe[240], sizeof(pi8) * 24);
			subframeTemp1[24] = '\0';
			twosComp2dec(&eph->nav[PRN].omegaDot, subframeTemp1, 24);
			eph->nav[PRN].omegaDot *= pow(2, -43) * M_PI;

			memcpy(subframeTemp1, &subframe[270], sizeof(pi8) * 8);
			subframeTemp1[8] = '\0';
			eph->nav[PRN].IODE_sf3 = bin2dec(subframeTemp1);

			memcpy(subframeTemp1, &subframe[278], sizeof(pi8) * 14);
			subframeTemp1[14] = '\0';
			twosComp2dec(&eph->nav[PRN].iDot, subframeTemp1, 14);
			eph->nav[PRN].iDot *= pow(2, -43) * M_PI;
		} else if (eph->nav[PRN].IODE_sf3 != subcheck) {

			eph->navTempStatus[PRN][2] = 1; /* status update*/
			memcpy(subframeTemp1, &subframe[60], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			twosComp2dec(&eph->navTemp[PRN].C_ic, subframeTemp1, 16);
			eph->navTemp[PRN].C_ic *= pow(2, -29);

			/*Reuse the temp4,5,6*/
			memset(temp6, 0, sizeof(pi8) * 33);
			memcpy(temp4, &subframe[76], sizeof(pi8) * 8);
			temp4[8] = '\0';
			memcpy(temp5, &subframe[90], sizeof(pi8) * 24);
			temp5[24] = '\0';
			strcat(temp6, temp4);
			strcat(temp6, temp5);
			twosComp2dec(&eph->navTemp[PRN].omega_0, temp6, strlen(temp6));
			eph->navTemp[PRN].omega_0 *= pow(2, -31);
			eph->navTemp[PRN].omega_0 *= M_PI;

			memcpy(subframeTemp1, &subframe[120], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			twosComp2dec(&eph->navTemp[PRN].C_is, subframeTemp1, 16);
			eph->navTemp[PRN].C_is *= pow(2, -29);

			/*Reuse the temp4,5,6*/
			memset(temp6, 0, sizeof(pi8) * 33);
			memcpy(temp4, &subframe[136], sizeof(pi8) * 8);
			temp4[8] = '\0';
			memcpy(temp5, &subframe[150], sizeof(pi8) * 24);
			temp5[24] = '\0';
			strcat(temp6, temp4);
			strcat(temp6, temp5);
			twosComp2dec(&eph->navTemp[PRN].i_0, temp6, strlen(temp6));
			eph->navTemp[PRN].i_0 *= pow(2, -31);
			eph->navTemp[PRN].i_0 *= M_PI;

			memcpy(subframeTemp1, &subframe[180], sizeof(pi8) * 16);
			subframeTemp1[16] = '\0';
			twosComp2dec(&eph->navTemp[PRN].C_rc, subframeTemp1, 16);
			eph->navTemp[PRN].C_rc *= pow(2, -5);

			/*Reuse the temp4,5,6*/
			memset(temp6, 0, sizeof(pi8) * 33);
			memcpy(temp4, &subframe[196], sizeof(pi8) * 8);
			temp4[8] = '\0';
			memcpy(temp5, &subframe[210], sizeof(pi8) * 24);
			temp5[24] = '\0';
			strcat(temp6, temp4);
			strcat(temp6, temp5);
			twosComp2dec(&eph->navTemp[PRN].omega, temp6, strlen(temp6));
			eph->navTemp[PRN].omega *= pow(2, -31);
			eph->navTemp[PRN].omega *= M_PI;

			memcpy(subframeTemp1, &subframe[240], sizeof(pi8) * 24);
			subframeTemp1[24] = '\0';
			twosComp2dec(&eph->navTemp[PRN].omegaDot, subframeTemp1, 24);
			eph->navTemp[PRN].omegaDot *= pow(2, -43) * M_PI;

			memcpy(subframeTemp1, &subframe[270], sizeof(pi8) * 8);
			subframeTemp1[8] = '\0';
			eph->navTemp[PRN].IODE_sf3 = bin2dec(subframeTemp1);

			memcpy(subframeTemp1, &subframe[278], sizeof(pi8) * 14);
			subframeTemp1[14] = '\0';
			twosComp2dec(&eph->navTemp[PRN].iDot, subframeTemp1, 14);
			eph->navTemp[PRN].iDot *= pow(2, -43) * M_PI;

		}
		break;
	case 4:/*--- It is subframe 4 -------------------------------------
	 Almanac, ionospheric model, UTC parameters.
	 SV health (PRN: 25-32).
	 Not decoded at the moment.*/
		memcpy(subframeTemp1, &subframe[60], sizeof(pi8) * 2);
		subframeTemp1[2] = '\0';
		dataID = bin2dec(subframeTemp1);

		memcpy(subframeTemp1, &subframe[62], sizeof(pi8) * 6);
		subframeTemp1[6] = '\0';
		svID = bin2dec(subframeTemp1);

		if (svID == 56) {
			/* start decoding page 18*/
			memcpy(subframeTemp1, &subframe[68], sizeof(pi8) * 8);
			subframeTemp1[8] = '\0';
			twosComp2dec(&a0, subframeTemp1, 8);
			a0 *= pow(2, -30);
			/* Ionospheric model parameters*/
			if (eph->almanac.a0 != a0) {
				eph->almanac.dataID = dataID;
				eph->almanac.svID = svID;

				memcpy(subframeTemp1, &subframe[68], sizeof(pi8) * 8);
				subframeTemp1[8] = '\0';
				twosComp2dec(&eph->almanac.a0, subframeTemp1, 8);
				eph->almanac.a0 *= pow(2, -30);

				memcpy(subframeTemp1, &subframe[76], sizeof(pi8) * 8);
				subframeTemp1[8] = '\0';
				twosComp2dec(&eph->almanac.a1, subframeTemp1, 8);
				eph->almanac.a1 *= pow(2, -27);

				memcpy(subframeTemp1, &subframe[90], sizeof(pi8) * 8);
				subframeTemp1[8] = '\0';
				twosComp2dec(&eph->almanac.a2, subframeTemp1, 8);
				eph->almanac.a2 *= pow(2, -24);

				memcpy(subframeTemp1, &subframe[98], sizeof(pi8) * 8);
				subframeTemp1[8] = '\0';
				twosComp2dec(&eph->almanac.a3, subframeTemp1, 8);
				eph->almanac.a3 *= pow(2, -24);

				memcpy(subframeTemp1, &subframe[106], sizeof(pi8) * 8);
				subframeTemp1[8] = '\0';
				twosComp2dec(&eph->almanac.beta0, subframeTemp1, 8);
				eph->almanac.beta0 *= pow(2, 11);

				memcpy(subframeTemp1, &subframe[120], sizeof(pi8) * 8);
				subframeTemp1[8] = '\0';
				twosComp2dec(&eph->almanac.beta1, subframeTemp1, 8);
				eph->almanac.beta1 *= pow(2, 14);

				memcpy(subframeTemp1, &subframe[128], sizeof(pi8) * 8);
				subframeTemp1[8] = '\0';
				twosComp2dec(&eph->almanac.beta2, subframeTemp1, 8);
				eph->almanac.beta2 *= pow(2, 16);

				memcpy(subframeTemp1, &subframe[136], sizeof(pi8) * 8);
				subframeTemp1[8] = '\0';
				twosComp2dec(&eph->almanac.beta3, subframeTemp1, 8);
				eph->almanac.beta3 *= pow(2, 16);
			}
			memcpy(subframeTemp1, &subframe[218], sizeof(pi8) * 8);
			subframeTemp1[8] = '\0';
			tot = bin2dec(subframeTemp1) * pow(2, 12);

			/* A0 and A1 do not agree with novatel*/
			if (eph->almanac.t_ot != tot) {
				memcpy(subframeTemp1, &subframe[150], sizeof(pi8) * 24);
				subframeTemp1[24] = '\0';
				twosComp2dec(&eph->almanac.A1, subframeTemp1, 24);
				eph->almanac.A1 *= pow(2, -50);

				/*Reuse the temp4,5,6*/
				memset(temp6, 0, sizeof(pi8) * 33);
				memcpy(temp4, &subframe[210], sizeof(pi8) * 8);
				temp4[8] = '\0';
				memcpy(temp5, &subframe[180], sizeof(pi8) * 24);
				temp5[24] = '\0';
				strcat(temp6, temp5);
				strcat(temp6, temp4);
				twosComp2dec(&eph->almanac.A0, temp6, 32);
				eph->almanac.A0 *= pow(2, -30);

				memcpy(subframeTemp1, &subframe[218], sizeof(pi8) * 8);
				subframeTemp1[8] = '\0';
				eph->almanac.t_ot = bin2dec(subframeTemp1) * pow(2, 12);

				/* UTC reference week*/
				memcpy(subframeTemp1, &subframe[226], sizeof(pi8) * 8);
				subframeTemp1[8] = '\0';
				eph->almanac.WNt = bin2dec(subframeTemp1) + 256 * 5;

				memcpy(subframeTemp1, &subframe[240], sizeof(pi8) * 8);
				subframeTemp1[8] = '\0';
				twosComp2dec(&eph->almanac.deltaTls, subframeTemp1, 8);

				memcpy(subframeTemp1, &subframe[248], sizeof(pi8) * 8);
				subframeTemp1[8] = '\0';
				eph->almanac.WNlsf = bin2dec(subframeTemp1) + 256 * 5;

				/* Day number*/
				memcpy(subframeTemp1, &subframe[256], sizeof(pi8) * 8);
				subframeTemp1[8] = '\0';
				eph->almanac.DN = bin2dec(subframeTemp1);

				memcpy(subframeTemp1, &subframe[270], sizeof(pi8) * 8);
				subframeTemp1[8] = '\0';
				twosComp2dec(&eph->almanac.deltaTlsf, subframeTemp1, 8);
			}

		}

		break;
	case 5:/*--- It is subframe 5 -------------------------------------
	 SV almanac and health (PRN: 1-24).
	 Almanac reference week number and time.
	 Not decoded at the moment.*/
		break;
	default:
		break;
	}/* for swich*/

	memcpy(subframeTemp2, &subframe[30], 17 * sizeof(pi8));
	subframeTemp2[17] = '\0';
	*TOW = (fl64) bin2dec(subframeTemp2) * 6 - 6;

	*subframeID = tempSubFrameID;
	return EXIT_SUCCESS;

}/*end of function*/
