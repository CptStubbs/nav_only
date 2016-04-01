/*
 * test_togeod.c
 *
 *  Created on: May 8, 2015
 *      Author: sean
 */

#include "../../src/nav/togeod.h"
#include "../../src/include/customDataTypes.h"
#include <check.h>
#include <stdlib.h>
#include <float.h>

START_TEST(test_correct_code){
	si32 retval =0;
	fl64  dphi =0;
	fl64 dlambda=0;
	fl64 h=0;
	fl64 a=6378137;
	fl64 finv=298.257223563;
	fl64 X=-1575230.19756805;
	fl64 Y=-5832569.14854412;
	fl64 Z=5017343.20793121;
	retval = togeod(&dphi, &dlambda, &h, a, finv, X, Y, Z);
	ck_assert_int_eq(retval,EXIT_SUCCESS);
	ck_assert((dphi - 39.862224) < DBL_EPSILON );
	ck_assert((dlambda - 254.88642813) < DBL_EPSILON );
	ck_assert((h - 1483887.282211) < DBL_EPSILON);
}
END_TEST

Suite * togeod_suite(void){

	Suite *s;
	TCase *tc_core;

	s = suite_create("togeod");

	/*Null test case*/

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

	s = togeod_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
