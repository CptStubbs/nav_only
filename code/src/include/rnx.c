/*
 * rnx.c
 *
 *  Created on: Mar 15, 2016
 *      Author: sean
 */
#include "rnx.h"
#include "customDataTypes.h"
#include "customMacros.h"
#include <stdio.h>

void writernxHeader(FILE * fid) {
	/** So apparently C has automatic string concationation with the preprocessor */
	pi8 scalarNames[] =
#define X(type,format,name) #name "\t"
			RNX_SCALARS;
#undef X

	pi8 vectorNames[] =
#define X(type,format,name) #name "\t"
			RNX_VECTORS;
#undef X
	pi8 scalarSizes[] =
#define X(type,format,name) format "\t"
			RNX_SCALARS;
#undef X

	pi8 vectorSizes[] =
#define X(type,format,name) xstr(CONF_NUMCHN) "*" format "\t"
			RNX_VECTORS;
#undef X



	fprintf(fid, "%s%s\r\n", scalarNames, vectorNames);
	fprintf(fid, "%s%s\r\n", scalarSizes, vectorSizes);
}
