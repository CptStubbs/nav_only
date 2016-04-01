/*
 * rinexNavHeader.c
 *
 *  Created on: May 21, 2015
 *      Author: sean
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "customDataTypes.h"
#include "rinexNavHeader.h"

#include <conf_nav.h>

#include "adjustFormat.h"

si32 rinexNavHeader(FILE * fid, almanac_s almanac) {

	fl64 version = 2.10;
	pi8 * fileType = "NAV";
	pi8 * space = " ";
	pi8 * headerLabel = "RINEX VERSION / TYPE";
	pi8 * headerStr;
	pi8 * pgm = "GNSS SDR-C Version";
	pi8 * runBy = RUNBY;
	pi8 atmp[13];
	pi8 Atmp[20];
	pi8 a0[13];
	pi8 a1[13];
	pi8 a2[13];
	pi8 a3[13];
	pi8 B0[13];
	pi8 B1[13];
	pi8 B2[13];
	pi8 B3[13];
	pi8 A0[20];
	pi8 A1[20];
	fl64 T = 0;
	fl64 W = 0;
	fl64 leapSec = 0;
	time_t curtime;
	char *date;
	si32 headerStrLen = 3 + strlen(fileType) + strlen(space) * 2
			+ strlen(headerLabel) + strlen("\n");
	/* Actual Code */
	headerStr = (pi8*) malloc((headerStrLen + 1) * sizeof(pi8));
	ERRORCHECK(headerStr==NULL)
	snprintf(headerStr, headerStrLen + 1, "%3f%s%s%s%s\n", version, space,
			fileType, space, headerLabel);

	headerLabel = "PGM / RUN BY / DATE";
	time(&curtime);
	date = ctime(&curtime);
	headerStrLen += strlen(pgm) + strlen(runBy) + strlen(date)
			+ strlen(headerLabel) + strlen("\n");
	headerStr = realloc(headerStr, (headerStrLen + 1) * sizeof(pi8));
	ERRORCHECK(headerStr==NULL)
	snprintf(headerStr, headerStrLen + 1, "%s%s%s%s%s\n", headerStr, pgm, runBy,
			date, headerLabel);
	headerLabel = "ION ALPHA";
	snprintf(atmp, 13, "%12.3e", almanac.a0);
	adjustFormat(a0, atmp, 12);
	snprintf(atmp, 13, "%12.3e", almanac.a1);
	adjustFormat(a1, atmp, 12);
	snprintf(atmp, 13, "%12.3e", almanac.a2);
	adjustFormat(a2, atmp, 12);
	snprintf(atmp, 13, "%12.3e", almanac.a3);
	adjustFormat(a3, atmp, 12);
	headerStrLen += strlen(space) + strlen(a0) + strlen(a1) + strlen(a2)
			+ strlen(a3) + strlen(space) + strlen(headerLabel) + strlen("\n");
	headerStr = realloc(headerStr, (headerStrLen + 1) * sizeof(pi8));
	ERRORCHECK(headerStr==NULL)
	snprintf(headerStr, headerStrLen + 1, "%s%s%s%s%s%s%s%s\n", headerStr,
			space, a0, a1, a2, a3, space, headerLabel);

	headerLabel = "ION BETA";
	snprintf(atmp, 13, "%12.3e", almanac.beta0);
	adjustFormat(B0, atmp, 12);
	snprintf(atmp, 13, "%12.3e", almanac.beta1);
	adjustFormat(B1, atmp, 12);
	snprintf(atmp, 13, "%12.3e", almanac.beta2);
	adjustFormat(B2, atmp, 12);
	snprintf(atmp, 13, "%12.3e", almanac.beta3);
	adjustFormat(B3, atmp, 12);
	headerStrLen += strlen(space) + strlen(B0) + strlen(B1) + strlen(B2)
			+ strlen(B3) + strlen(space) + strlen(headerLabel) + strlen("\n");
	headerStr = realloc(headerStr, (headerStrLen + 1) * sizeof(pi8));
	ERRORCHECK(headerStr==NULL)
	snprintf(headerStr, headerStrLen + 1, "%s%s%s%s%s%s%s%s\n", headerStr,
			space, B0, B1, B2, B3, space, headerLabel);

	headerLabel = "DELTA-UTC: A0,A1,T,W";
	snprintf(Atmp, 20, "%19.11e", almanac.A0);
	adjustFormat(A0, Atmp, 19);
	snprintf(Atmp, 20, "%19.11e", almanac.A1);
	adjustFormat(A1, Atmp, 19);
	T = almanac.t_ot;
	W = almanac.WNt;
	headerStrLen += strlen(space) + strlen(A0) + strlen(A1) + 9 + 9
			+ strlen(space) + strlen(headerLabel) + strlen("\n");
	headerStr = realloc(headerStr, (headerStrLen + 1) * sizeof(pi8));
	ERRORCHECK(headerStr==NULL)
	snprintf(headerStr, headerStrLen + 1, "%s%s%s%s%9f%9f%s%s\n", headerStr,
			space, A0, A1, T, W, space, headerLabel);

	headerLabel = "LEAP SECONDS";
	leapSec = almanac.deltaTls;
	headerStrLen += 6 + strlen(space) + strlen(headerLabel) + strlen("\n");
	headerStr = realloc(headerStr, (headerStrLen + 1) * sizeof(pi8));
	ERRORCHECK(headerStr==NULL)
	snprintf(headerStr, headerStrLen + 1, "%s%6f%s%s\n", headerStr, leapSec,
			space, headerLabel);

	headerLabel = "END OF HEADER";
	headerStrLen += strlen(space) + strlen(headerLabel) + strlen("\n");
	headerStr = realloc(headerStr, (headerStrLen + 1) * sizeof(pi8));
	ERRORCHECK(headerStr==NULL)
	snprintf(headerStr, headerStrLen + 1, "%s%s%s\n", headerStr, space,
			headerLabel);
	fprintf(fid, "%s", headerStr);
	free(headerStr);
	return EXIT_SUCCESS;
}
