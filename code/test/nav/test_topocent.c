/*
 * test_topocent.c
 *
 *  Created on: May 18, 2015
 *      Author: sean
 */


/*
 * test_topocent.c
 *
 *  Created on: May 8, 2015
 *      Author: sean
 */

#include "../../src/nav/topocent.h"
#include "../../src/include/customDataTypes.h"
#include <check.h>
#include <stdlib.h>
#include <float.h>


START_TEST(test_correct_code){
	fl64 X[3]={0,0,0};
	fl64 dx[3]={11889517.4435718,-10307085.0913442,21316411.0927666};
	si32 retval=0;
	fl64 Az = 0;
	fl64 El = 0;
	fl64 D = 0;
	retval = topocent(&Az, &El, &D, X, dx);
	ck_assert_int_eq(retval,EXIT_SUCCESS);
	ck_assert((Az - 334.19492523332) < DBL_EPSILON );
	ck_assert((El - 26.663218) < DBL_EPSILON );
	ck_assert((D - 26495018.5883504) < DBL_EPSILON);
}
END_TEST

Suite * topocent_suite(void){

	Suite *s;
	TCase *tc_core;

	s = suite_create("topocent");

	/*Core test case*/
	tc_core = tcase_create("Core");


	tcase_add_test(tc_core,test_correct_code);
	suite_add_tcase(s,tc_core);

	return s;
}

si32 main(void){
	si32 number_failed =0;
	Suite *s;
	SRunner *sr;

	s = topocent_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
