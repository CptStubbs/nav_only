/*
 * test_twosComp2dec.c
 *
 *  Created on: May 18, 2015
 *      Author: sara
 */


#include "../../src/nav/adjustFormat.h"
#include "../../src/include/customDataTypes.h"
#include <check.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>

START_TEST(test_correct_code){
	pi8 * inStr="0.000E+00";
	pi8 outStr[13];
	si32 retval=0;
	si32 length=12;
	retval = adjustFormat(outStr,inStr,length);
	ck_assert_int_eq(retval,EXIT_SUCCESS);
	ck_assert_int_eq(strncmp(outStr,"    .0000D+0",length-1),0);
}
END_TEST

Suite * adjustFormat_suite(void){

	Suite *s;
	TCase *tc_core;

	s = suite_create("adjustFormat");

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

	s = adjustFormat_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

