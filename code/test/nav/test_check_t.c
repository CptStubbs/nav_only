/*
 * test_togeod.c
 *
 *  Created on: May 8, 2015
 *      Author: sean
 */

#include "../../src/nav/check_t.h"
#include "../../src/include/customDataTypes.h"
#include <check.h>
#include <stdlib.h>
#include <float.h>


START_TEST(test_correct_code){
	fl64 time = 5555555;
	fl64 corrTime =0;

	check_t(&corrTime,time);/*fl64 * &corrTime,fl64 time)*/
	ck_assert((corrTime - 4950755) < DBL_EPSILON );
}
END_TEST

Suite * check_t_suite(void){

	Suite *s;
	TCase *tc_core;

	s = suite_create("check_t");

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

	s = check_t_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
