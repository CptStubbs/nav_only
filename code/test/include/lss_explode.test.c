#include "lss_explode.h"
#include "log_explode.h"
#include "conf_lss.h"
#include "customDataTypes.h"
#include <check.h>
#include <rnx.h>
#include <stdlib.h>
#include <stdio.h>
#include "conf_log.h"
#include "conf_lss.h"
#include "conf.h"

#define LSS_EXPLODELEN 17
#define LSS_INSTANCES 3

void printLSS(rnx_s * lss){
	ui32 k;
	/// Print scalar values
	#define X(type,format,name) \
		printf(#name":\t"format"\t",lss->name);						/// print info
		RNX_SCALARS																				/// using scalar fields
	#undef X 																						/// undefine X
	printf("\n");
	for (k=0; k<CONF_LSS_LEN; k++){
		/// Print vector values
		#define X(type,format,name) \
			printf(#name":\t"format"\t",lss->name[k]);			/// print info
			RNX_VECTORS																			/// using vector fields
		#undef X 																					/// undefine X
		printf("\n");
	}
	printf("\n");
}

void initTestData(rnx_s * lss, ui32 L){
	ui32 k,l,m;
	for (l=0; l<L; l++){
		m=0;
		/// Initialize scalar fields
		#define X(type,p,name) \
			lss[l].name = (type) (++m*100 + l*10+.5);
			RNX_SCALARS
		#undef X
		for (k=0; k<CONF_LSS_LEN;k++){
			/// Initialize vector fields
			#define X(type,p,name) \
				lss[l].name[k] = (type) (++m*100 + l*10 + k + .5);
				RNX_VECTORS
			#undef X
		}
	}
	return;
}

int writeTestData(rnx_s * lss, ui32 K, char * fileNameBase){
	/// Clear all known fields
	char fileName[MAX_LEN_FILENAME];
	snprintf(fileName, MAX_LEN_FILENAME, "%s/%s.bin", CONF_LOG_PATH, fileNameBase);
	FILE * fd = fopen(fileName, "w");
	if(fd == NULL){
		printf("\n fopen() Error when opening '%s'! \n",fileName);
		return EXIT_FAILURE;
	}
	fwrite(lss, sizeof(rnx_s), K, fd);
	fclose(fd);
	return EXIT_SUCCESS;
}

int readTestData(rnx_s * lss, ui32 L, char * fileNameBase){
	ui32 ret;
	ui32 s,S;																						/// s is scalar index, S is number of vector fields
	ui32 v,V;																						/// v is vector index, V is number of vector fields

	/// Figure out the number of rows of the scalar and vector fields
	S = 0;
	#define X(a,b,c) S++;
		RNX_SCALARS
	#undef X
	V = 0;
	#define X(a,b,c) V++;
		RNX_VECTORS
	#undef X

	/// Open files for reading
	ret = 0;
	FILE * fdS[S];
	s = 0;
	#define X(type, format, name) \
		fopenLogs(&ret, &fdS[s++], fileNameBase, #name, #type,"r");
		RNX_SCALARS
	#undef X
	FILE * fdV[V];
	v = 0;
	#define X(type, format, name) \
		fopenLogs(&ret, &fdV[v++], fileNameBase, #name, #type,"r");
		RNX_VECTORS
	#undef X
	if (ret>0){
		fprintf(stderr,"Failed to open at least one log file for reading");
		return EXIT_FAILURE;
	}

	/// Read individual files
	ui32 l;
	for (l=0; l<L; l++){
		s=0;																							/// Initialize scalar index
		#define X(type,p,name) \
			if(fread(&lss[l].name,sizeof(type),1,fdS[s++]) == 0) return EXIT_FAILURE;
			RNX_SCALARS																			/// Read from each file
		#undef X 																					/// Standard practice to undefine X
	}
	for (l=0; l<L; l++){
		v=0;																							/// Initialize scalar index
		#define X(type,p,name) \
			if(fread(&lss[l].name,sizeof(type),CONF_LSS_LEN,fdV[v++]) == 0) return EXIT_FAILURE;
			RNX_VECTORS																			/// Read from each file
		#undef X 																					/// Standard practice to undefine X
	}
	/// Close all files
	for (s=0; s<S; s++)	fclose(fdS[s]);									/// lss_xxx files
	for (v=0; v<V; v++)	fclose(fdV[v]);									/// lss_xxx files
	return EXIT_SUCCESS;
}

int lss_validateData(rnx_s * lss1, rnx_s * lss2, ui32 K){
	ck_assert(memcmp(lss1, lss2, K*sizeof(rnx_s)) == 0);
	return EXIT_SUCCESS;
}

START_TEST (fun)
{
	/// Allocate two ccr, one that we write to disk and one that we populate when we read from disk
	rnx_s lss1[LSS_EXPLODELEN];
	memset(&lss1, 0, LSS_EXPLODELEN*sizeof(rnx_s));						/// Make sure all is set to zero
	rnx_s lss2[LSS_EXPLODELEN];
	memset(&lss2, 0, LSS_EXPLODELEN*sizeof(rnx_s));						/// Make sure all is set to zero
	purgeLogFiles("lssTest");
	initTestData(&lss1[0],LSS_EXPLODELEN);
	if (writeTestData(lss1, LSS_EXPLODELEN,"lssTest") == EXIT_FAILURE){
		printf("---      Failed when writing to disk in lss/writeTestData\n");
		ck_assert(0);
	}
	lss_explode("lssTest");
	if (readTestData(lss2,LSS_EXPLODELEN, "lssTest") == EXIT_FAILURE){
		printf("---      Failed when reading from disk in lss/readTestData() \n");
		ck_assert(0);
	}
	lss_validateData(&lss1[0], &lss2[0], LSS_EXPLODELEN);
	purgeLogFiles("lssTest");
}
END_TEST

Suite * test_suite(void) {
    Suite * s;
    TCase * tc_core;
    s = suite_create("Test lss_explode");
    tc_core = tcase_create("Core");
    tcase_set_timeout(tc_core,10);
    tcase_add_test(tc_core, fun);
    suite_add_tcase(s, tc_core);
    return s;
}

si32 main(void) {
    si32 number_failed = 0;
    Suite * s;
    SRunner * sr;
    s = test_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
