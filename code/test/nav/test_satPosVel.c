/*
 * test_togeod.c
 *
 *  Created on: May 8, 2015
 *      Author: sara
 */

#include "../../src/nav/satPosVel.h"
#include "../../src/include/customDataTypes.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>

START_TEST(test_correct_code)
	{
		/*si8 * wordtmp = "100010110000110001111000000000";
		 si8 word[31];
		 si32 pass = 0;
		 si32 wordLength = 30;
		 si8 D29Star = '0';
		 si8 D30Star = '0';
		 si8 * wordtrue = "100010110000110001111000000000";
		 si32 passTrue = 1;
		 strncpy(word, wordtmp, 31);
		 satPosVel(satPositions[3],satVelocity[3],satClkCorr,transmitTime,transmitTimeSize,prnList[32], navStruct * eph[],activeChnList, activeChnListSize)
		 (fl64 * satPositions[3],fl64 * satVelocity[3], fl64 * satClkCorr,fl64 * transmitTime, si32 transmitTimeSize, fl64 prnList[32], navStruct * eph[],fl64 * activeChnList, si32 activeChnListSize)
		 ck_assert(passTrue == pass); */
	}END_TEST

Suite * satPosVel_suite(void) {

	Suite *s;
	TCase *tc_core;

	s = suite_create("satPosVel");

	/*Null test case*/

	/*Core test case*/
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_correct_code);
	suite_add_tcase(s, tc_core);

	return s;
}

si32 main(void) {
	si32 number_failed = 0;
	Suite *s;
	SRunner *sr;

	s = satPosVel_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
