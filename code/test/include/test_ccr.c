#include "../../src/chn/chn.h"
#include "../../src/chn/chn_asn.h"
#include "../../src/include/customDataTypes.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

bool ccr_writer_alive = true;

// This should return [1:32, 1:32, ...]
START_TEST (simple)
{
    /*ccr_s ccr;
    ccr_init(&ccr, (fl32) 4.092e6, (fl32) 1.023e6);

    /// Spawn thread that writes ccr.res and ccr.ctr
    pthread_t th0, th1;
    pthread_create(&th0, NULL, ccr_res_writer, &ccr);
    pthread_create(&th1, NULL, ccr_ctr_writer, &ccr);

    nanosleep((struct timespec[]){{0, 100000000}}, NULL);	/// Sleep for 100ms
    printf("  E: %5d %5d\n",ccr.res[CONF_NUMCHN-1].EI,ccr.res[CONF_NUMCHN-1].EQ);
    printf("  P: %5d %5d\n",ccr.res[CONF_NUMCHN-1].PI,ccr.res[CONF_NUMCHN-1].PQ);
    printf("  L: %5d %5d\n",ccr.res[CONF_NUMCHN-1].LI,ccr.res[CONF_NUMCHN-1].LQ);
    printf("  F: %8.0f %8.0f\n",ccr.ctr[CONF_NUMCHN-1].carrFreq,ccr.ctr[CONF_NUMCHN-1].codeFreq);

    printf("  Terminating threads\n");
    ccr_writer_alive = false;
    nanosleep((struct timespec[]){{0, 1000}}, NULL);	/// Sleep for 1us
    //pthread_join(th0, NULL);
    //pthread_join(th1, NULL);
    printf(" Error in CHN");
    printf("  Threads terminated\n");
    */
}
END_TEST

Suite * test_suite(void) {
    Suite * s;
    TCase * tc_core;
    s = suite_create("Test ccr_dummyTesting");
    tc_core = tcase_create("Core");
    tcase_set_timeout(tc_core,10);
    tcase_add_test(tc_core, simple);
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
