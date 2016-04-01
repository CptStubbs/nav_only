/*
 * test_twosComps2dec.c
 *
 *  Created on: May 28, 2015
 *      Author: user
 */


/*
 * test_togeod.c
 *
 *  Created on: May 8, 2015
 *      Author: sara
 */

#include "../../src/nav/twosComp2dec.h"
#include "../../src/include/customDataTypes.h"
#include <check.h>
#include <stdlib.h>
#include <float.h>
#include <stdio.h>


START_TEST(test_correct_code){
	pi8 binaryNumber[9] = "11101100";
	si32 binaryNumberSize = 8;
	fl64 intNumber = 0;
	twosComp2dec(&intNumber,binaryNumber,binaryNumberSize);
	ck_assert((intNumber +20) < DBL_EPSILON );
}
END_TEST

Suite * twosComp2dec_suite(void){

	Suite *s;
	TCase *tc_core;

	s = suite_create("twosComp2dec");

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

	s = twosComp2dec_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
