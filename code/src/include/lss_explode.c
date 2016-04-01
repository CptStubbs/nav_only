/*
 * lss_explode.c
 *
 *  Created on: Oct 05, 2015
 *      Author: staffan
 */

#include <rnx.h>
#include <stdio.h>
#include <stdlib.h>
#include "lss_explode.h"
#include "log_explode.h"
#include "conf.h"
#include "conf_log.h"
#include "conf_lss.h"
#include "customMacros.h"
#include "customDataTypes.h"


/*!
 * \brief "Explode a lss.bin log file"
 * \param fileNameBase pointer to a string (like "lss")
 * \return success or failure
 *
 */
int lss_explode(char * fileNameBase){
	ui32 s,S;																						/// s is scalar index, S is number of vector fields
	ui32 v,V;																						/// v is vector index, V is number of vector fields
	ui32 k, ret;																				/// k general counter, ret return value for fopen

	/// Open input file
	char fileName[MAX_LEN_FILENAME];										/// Allocate a string for lss filename
	snprintf(fileName,MAX_LEN_FILENAME,"%s/%s.bin",CONF_LOG_PATH,fileNameBase);	/// Create filename
//	puts(fileName);																			/// print filename (debugging)
	FILE * fd = fopen(fileName, "r");										/// Open lss file
	if(fd == NULL){																			/// Make sure that the file opened correctly
		printf("\nCould not open '%s'! \n",fileName);			/// Print warning otherwise,
		return EXIT_FAILURE;															/// And exit with error
	}

	/// Figure out the number of rows of the scalar and vector fields
	S = 0;
	#define X(a,b,c) S++;
		RNX_SCALARS
	#undef X
	V = 0;
	#define X(a,b,c) V++;
		RNX_VECTORS
	#undef X

	/// Open files for writing
	ret = 0;
	FILE * fdS[S];
	s = 0;
	#define X(type, format, name) \
		fopenLogs(&ret, &fdS[s++], fileNameBase, #name, #type,"w");
		RNX_SCALARS
	#undef X
	FILE * fdV[V];
	v = 0;
	#define X(type, format, name) \
		fopenLogs(&ret, &fdV[v++], fileNameBase, #name, #type,"w");
		RNX_VECTORS
	#undef X

	if (ret>0){
		fprintf(stderr,"Failed to open at least one log file");
		return EXIT_FAILURE;
	}

	/// Read lss, write individual files
	rnx_s lss;
	while(fread(&lss,sizeof(rnx_s),1,fd) > 0){					/// While there is data to read
		if (feof(fd)){
			fprintf(stderr, "Unexpected end of file of lss.bin detected. Exiting.\n");
			return EXIT_FAILURE;
		}
		s=0;																							/// Initialize index for fdS
		#define X(type,p,name) \
			if (fwrite(&lss.name,sizeof(type),1,fdS[s++]) == 0) return EXIT_FAILURE;
			RNX_SCALARS																			/// Write to each scalar file
		#undef X 																					/// Standard practice to undefine X
		for (k=0; k<CONF_LSS_LEN;k++){
			v=0;																							/// Initialize index for fdV
			#define X(type,p,name) \
				if (fwrite(&lss.name[k],sizeof(type),1,fdV[v++]) == 0) return EXIT_FAILURE;
				RNX_VECTORS																			/// Write to vector file
			#undef X 																					/// Standard practice to undefine X
		}
	}

	/// Close all files
	fclose(fd);																					/// input
	for (s=0; s<S; s++)	fclose(fdS[s]);									/// scalar files
	for (v=0; v<V; v++)	fclose(fdV[v]);									/// vector files

	/// Return
	return EXIT_SUCCESS;																/// Exit successfully
}
