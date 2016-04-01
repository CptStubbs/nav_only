/*
 * apt_explode.c
 *
 *  Created on: Oct 05, 2015
 *      Author: staffan
 */

#include <stdio.h>
#include <stdlib.h>
#include "apt.h"
#include "log_explode.h"
#include "apt_emt_explode.h"
#include "conf_log.h"


/*!
 * \brief "Explode a apt.bin log file"
 * \return success or failure
 *
 */
int apt_emt_explode(char * fileNameBase){
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
		APT_EMT_SCALARS
	#undef X
	V = 0;
	#define X(a,b,c) V++;
		APT_EMT_VECTORS
	#undef X

	/// Open files for writing
	ret = 0;
	FILE * fdS[S];
	s = 0;
	#define X(type, format, name) \
		fopenLogs(&ret, &fdS[s++], fileNameBase, #name, #type,"w");
		APT_EMT_SCALARS
	#undef X
	FILE * fdV[V];
	v = 0;
	#define X(type, format, name) \
		fopenLogs(&ret, &fdV[v++], fileNameBase, #name, #type,"w");
		APT_EMT_VECTORS
	#undef X

	if (ret>0){
		fprintf(stderr,"Failed to open at least one log file");
		return EXIT_FAILURE;
	}

	/// Read apt, write individual files
	apt_emt_s apt_emt;
	while(fread(&apt_emt,sizeof(apt_emt_s),1,fd) > 0){								/// While there is data to read
		if (feof(fd)){
			printf("Unexpected end of file of apt_emt.bin detected. Exiting.\n");
			return EXIT_FAILURE;
		}
		s=0;																						/// Initialize index for fdCtr
		#define X(type,p,name) \
			if (fwrite(&apt_emt.name,sizeof(type),1,fdS[s++]) == 0) return EXIT_FAILURE;
			APT_EMT_SCALARS																/// Write to each ccr_ctr file
		#undef X 																				/// Standard practice to undefine X
		for (k=0; k<APT_LEN; k++){												/// Loop over all apt_array values
			v=0;																						/// Initialize index for fdVec
			#define X(type,p, name) \
				if (fwrite(&apt_emt.name[k],sizeof(type),1,fdV[v++]) == 0) return EXIT_FAILURE;
				APT_EMT_VECTORS																/// Write to each ccr_ctr file
			#undef X 																				/// Standard practice to undefine X
		}
	}

	/// Close all files
	fclose(fd);																					/// ccr file
	for (s=0; s<S; s++)	fclose(fdS[s]);								/// ccr_ctr_xxx files
	for (v=0; v<V; v++)	fclose(fdV[v]);								/// ccr_res_xxx files

	return EXIT_SUCCESS;																/// Exit successfully
}
