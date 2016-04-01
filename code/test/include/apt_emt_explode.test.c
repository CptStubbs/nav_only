#include "apt_emt_explode.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include "conf_log.h"
#include "log_explode.h"

#define APT_EXPLODELEN 17
#define APT_INSTANCES 3

void initTestData(apt_emt_s * apt_emt, ui32 L){
	ui32 k,l,m;
	for (l=0; l<L; l++){
		m=0;
		/// Initialize scalar fields
		#define X(type,p,name) \
			apt_emt[l].name = (type) ++m*100 + .5;
			APT_EMT_SCALARS
		#undef X
		/// Initialize vector fields
		for (k=0;k<APT_LEN;k++){
			#define X(type,p,name) \
				apt_emt[l].name[k] = (type) ++m*100 + l*50 + .5;
				APT_EMT_VECTORS
			#undef X
		}
	}
	return;
}

int writeTestData(apt_emt_s * apt_emt, ui32 K, char * fileNameBase){
	/// Clear all known fields
	char fileName[MAX_LEN_FILENAME];
	snprintf(fileName, MAX_LEN_FILENAME, "%s/%s.bin", CONF_LOG_PATH, fileNameBase);
	FILE * fd = fopen(fileName, "w");
	if(fd == NULL){
		printf("\n fopen() Error when opening '%s'! \n",fileName);
		return EXIT_FAILURE;
	}
	fwrite(apt_emt, sizeof(apt_emt_s), K, fd);
	fclose(fd);
	return EXIT_SUCCESS;
}

int readTestData(apt_emt_s * apt_emt, ui32 L, char * fileNameBase){
	ui32 ret;
	ui32 s,S;																						/// s is scalar index, S is number of vector fields
	ui32 v,V;																						/// v is vector index, V is number of vector fields

	/// Figure out the number of rows of the scalar and vector fields
	S = 0;
	#define X(a,b,c) S++;
		APT_EMT_SCALARS
	#undef X
	V = 0;
	#define X(a,b,c) V++;
		APT_EMT_VECTORS
	#undef X

	/// Open files for reading
	ret = 0;
	FILE * fdS[S];
	s = 0;
	#define X(type, format, name) \
		fopenLogs(&ret, &fdS[s++], fileNameBase, #name, #type,"r");
	APT_EMT_SCALARS
	#undef X
	FILE * fdV[V];
	v = 0;
	#define X(type, format, name) \
		fopenLogs(&ret, &fdV[v++], fileNameBase, #name, #type,"r");
	APT_EMT_VECTORS
	#undef X
	if (ret>0){
		fprintf(stderr,"Failed to open at least one log file for reading");
		return EXIT_FAILURE;
	}
	/// Read individual files
	ui32 k,l;
	for (l=0; l<L; l++){
		s=0;																						/// Initialize index for fdCtr
		#define X(type,p,name) \
			if(fread(&apt_emt[l].name,sizeof(type),1,fdS[s++]) == 0) return EXIT_FAILURE;
			APT_EMT_SCALARS																/// Write to each ccr_ctr file
		#undef X 																				/// Standard practice to undefine X
		for (k=0; k<APT_LEN; k++){
			v=0;																						/// Initialize index for fdRes
			#define X(type,p,name) \
				if(fread(&apt_emt[l].name[k],sizeof(type),1,fdV[v++]) == 0) return EXIT_FAILURE;
				APT_EMT_VECTORS																/// Write to each ccr_ctr file
			#undef X 																				/// Standard practice to undefine X
		}
	}
	/// Close all files
	for (s=0; s<S; s++)	fclose(fdS[s]);								/// ccr_ctr_xxx files
	for (v=0; v<V; v++)	fclose(fdV[v]);								/// ccr_res_xxx files
	return EXIT_SUCCESS;
}

int apt_emt_validateData(apt_emt_s * apt_emt1, apt_emt_s * apt_emt2, ui32 K){
	ck_assert(memcmp(apt_emt1, apt_emt2, K*sizeof(apt_emt_s)) == 0);
	return EXIT_SUCCESS;
}

START_TEST (fun)
{
	/// Allocate two apt_emt, one that we write to disk and one that we populate when we read from disk
	apt_emt_s apt_emt1[APT_EXPLODELEN];
	memset(&apt_emt1, 0, APT_EXPLODELEN*sizeof(apt_emt_s));						/// Make sure all is set to zero
	apt_emt_s apt_emt2[APT_EXPLODELEN];
	memset(&apt_emt2, 0, APT_EXPLODELEN*sizeof(apt_emt_s));						/// Make sure all is set to zero
	purgeLogFiles("apt_emtTest");
	initTestData(&apt_emt1[0],APT_EXPLODELEN);
	if (writeTestData(&apt_emt1[0], APT_EXPLODELEN,"apt_emtTest") == EXIT_FAILURE){
		printf("---      Failed when writing to disk in apt_emt/writeTestData\n");
		ck_assert(false);
	}
	apt_emt_explode("apt_emtTest");
	if (readTestData(&apt_emt2[0],APT_EXPLODELEN, "apt_emtTest") == EXIT_FAILURE){
		printf("---      Failed when reading from disk in apt_emt/readTestData() \n");
		ck_assert(false);
	}
	apt_emt_validateData(&apt_emt1[0], &apt_emt2[0], APT_EXPLODELEN);
	purgeLogFiles("apt_emtTest");
}
END_TEST

Suite * test_suite(void) {
    Suite * s;
    TCase * tc_core;
    s = suite_create("Test apt_emt_explode");
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
