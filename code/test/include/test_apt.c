
#include "../../src/include/apt_dummyTesting.h"
#include "../../src/include/customDataTypes.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

bool KEEPTHREADSALIVE = true;

// This should return [1:32, 1:32, ...]
START_TEST (apt_test0)
{
	ui32 m;
	/// Allocate and initialize apt_arr
	apt_s apt;
	apt.indWrite = 0;
	apt.indRead = 0;
	for (m=0; m<APT_NUM; m++){
		pthread_mutex_init(&apt.emt[m].lock, NULL);
		apt.emt[m].newApt = false;
	}

	/// Spawn thread that writes and reads apt_arr
	pthread_t th0, th1;
	pthread_create(&th0, NULL, apt_writer, &apt);
	pthread_create(&th1, NULL, apt_reader, &apt);

	nanosleep((struct timespec[]){{0, 100000000}}, NULL);	/// Sleep for 100ms

//	printf("  Terminating threads\n");
	KEEPTHREADSALIVE = false;
	nanosleep((struct timespec[]){{0, 1000}}, NULL);	/// Sleep for 1us
	pthread_join(th0, NULL);
	pthread_join(th1, NULL);
//	printf("  Threads terminated\n");
	ck_assert(true);
}
END_TEST

Suite * test_suite(void) {
	Suite * s;
	TCase * tc_core;
	s = suite_create("Test apt_dummyTesting");
	tc_core = tcase_create("Core");
	tcase_add_test(tc_core, apt_test0);
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
