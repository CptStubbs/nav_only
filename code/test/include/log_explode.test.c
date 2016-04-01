#include "../../src/include/customDataTypes.h"
#include "../../src/include/log_explode.h"
#include <check.h>
#include <rnx.h>
#include <stdlib.h>
#include <stdio.h>
#include "conf_log.h"
#include "conf.h"
#include <unistd.h>

#define LOG_TABLE \
		X(ui16,	  "%u", 		yo1) \
		X(fl64, 	"%f",			yo2)

START_TEST (fun)
{
	ui32 ret, k, K = 0;
	#define X(a,b,c) K++;
		LOG_TABLE
	#undef X
	purgeLogFiles("logTest");
	FILE * fds[K];
	k = 0; ret = 0;
	#define X(type, format, name) \
		fopenLogs(&ret, &fds[k++], "logTest",#name,#type,"w");
		LOG_TABLE
	#undef X
	if (ret != EXIT_SUCCESS){
		fprintf(stderr,"Error - could not open one or more files");
	}
	for (k=0; k<K; k++) fclose(fds[k]);
//	purgeLogFiles("logTest");
	log_explodeMain();
}
END_TEST

Suite * test_suite(void) {
    Suite * s;
    TCase * tc_core;
    s = suite_create("Test log_explode");
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
