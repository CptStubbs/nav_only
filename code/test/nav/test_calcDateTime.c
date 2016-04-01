/*
 * test_twosComp2dec.c
 *
 *  Created on: May 18, 2015
 *      Author: sara
 */


#include "../../src/include/calcDateTime.h"
#include "../../src/include/customDataTypes.h"
#include <check.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>

START_TEST(test_correct_code){
	calcTimeStruct calcTime;
	fl64 weekNumber = 1822;
	fl64 TOW = 504606;
	calcDateTime(&calcTime,weekNumber,TOW);
	ck_assert(calcTime.year-2014<DBL_EPSILON);
	ck_assert(calcTime.dayofYear-346<DBL_EPSILON);
	ck_assert(calcTime.month-12<DBL_EPSILON);
	ck_assert(calcTime.day-12<DBL_EPSILON);
	ck_assert(calcTime.hour-20<DBL_EPSILON);
	ck_assert(calcTime.minute-10<DBL_EPSILON);
	ck_assert(calcTime.second-6<DBL_EPSILON);

}
END_TEST

Suite * calcDateTime_suite(void){

	Suite *s;
	TCase *tc_core;

	s = suite_create("calcDateTime");

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

	s = calcDateTime_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

