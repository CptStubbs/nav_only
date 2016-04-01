/*
 * test_togeod.c
 *
 *  Created on: May 8, 2015
 *      Author: sara
 */

#include "../../src/nav/navParityChk.h"
#include "../../src/include/customDataTypes.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>

START_TEST(test_correct_code){
	si32 status = 0;
	si32 ndat[32] = {-1,	-1,	1,	-1,	-1,	-1,	1,	-1,	1,	1,	-1,	-1,	-1,	-1,	1,	1,	-1,	-1,	-1,	1,	1,	1,	1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1};
	navParityChk(&status,ndat);

	ck_assert(status == 1);
}
END_TEST

Suite * navParityChk_suite(void){

	Suite *s;
	TCase *tc_core;

	s = suite_create("navParityChk");

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

	s = navParityChk_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
