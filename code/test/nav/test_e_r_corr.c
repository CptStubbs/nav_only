/*
 * test_e_r_corr.c
 *
 *  Created on: May 13, 2015
 *      Author: sean
 */



#include "../../src/nav/e_r_corr.h"
#include "../../src/include/customDataTypes.h"
#include <check.h>
#include <stdlib.h>
#include <float.h>
#include <stdio.h>

START_TEST(test_correct_code){
	fl64 Rot_X[3]={0,0,0};
	fl64 travelTime= 0.0883778690268132;
	fl64 X[3]={ 11889583.8685301,-10307008.4677562,21316411.0927666};
	e_r_corr(Rot_X,travelTime,X);
	ck_assert((Rot_X[0] - 11889517.4435718) < DBL_EPSILON );
	ck_assert((Rot_X[1] - (-10307085.0913442)) < DBL_EPSILON );
	ck_assert((Rot_X[2] - 21316411.0927666) < DBL_EPSILON);
}
END_TEST

Suite * e_r_corr_suite(void){

	Suite *s;
	TCase *tc_core;

	s = suite_create("e_r_corr");

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

	s = e_r_corr_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
