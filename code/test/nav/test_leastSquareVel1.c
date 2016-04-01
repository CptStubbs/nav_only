/*
 * test_leastSquareVel1.c
 *
 *  Created on: May 19, 2015
 *      Author: sean
 */

/*
 * test_leastSquareVel1.c
 *
 *  Created on: May 15, 2015
 *      Author: sean
 */

#include "../../src/nav/leastSquareVel1.h"
#include "../../src/include/customDataTypes.h"
#include "../../src/include/initNavSolutions.h"
#include <check.h>
#include <stdlib.h>
#include <float.h>
#include <stdio.h>

START_TEST(test_correct_code)
	{
		si32 retval = 0;
		si32 i = 0;
		fl64 ** vel;
		fl64 vel_expected[4] = { -0.001666, -0.00677159108100256,
				0.0112052500644, 21.84106900698220243839 };
		fl64 carrPhaseDiff[7] = { 582.171189782154, 188.815591769088,
				-275.070883074778, 65.3726321780373, -48.0352268314988,
				-275.425135454102, -570.341610616282 };
		si32 activeChnList[7] = { 0,1,2,3,4,5,6 };
		si32 activeChnListSize = 7;
		navSolutions_s navSolutions;
		initNavSolution(&navSolutions);
		/* TODO: Fill these data structures */
		/* satPositionsOld */
		navSolutions.channel.satPositionsOld[0][0] = 11889583.8685301;
		navSolutions.channel.satPositionsOld[0][1] = -180234.831446901;
		navSolutions.channel.satPositionsOld[0][2] = -16380161.1689619;
		navSolutions.channel.satPositionsOld[0][3] = -2718384.0065623;
		navSolutions.channel.satPositionsOld[0][4] = -19516438.5410116;
		navSolutions.channel.satPositionsOld[0][5] = 14801414.6187957;
		navSolutions.channel.satPositionsOld[0][6] = -15037445.2533632;
		navSolutions.channel.satPositionsOld[1][0] = -10307008.4677562;
		navSolutions.channel.satPositionsOld[1][1] = -23635088.0390124;
		navSolutions.channel.satPositionsOld[1][2] = -11396225.0198663;
		navSolutions.channel.satPositionsOld[1][3] = -15987387.2007278;
		navSolutions.channel.satPositionsOld[1][4] = -6521663.27657975;
		navSolutions.channel.satPositionsOld[1][5] = -20258702.5421519;
		navSolutions.channel.satPositionsOld[1][6] = -21941774.6771709;
		navSolutions.channel.satPositionsOld[2][0] = 21316411.0927666;
		navSolutions.channel.satPositionsOld[2][1] = 12358222.9383066;
		navSolutions.channel.satPositionsOld[2][2] = 17821018.5091568;
		navSolutions.channel.satPositionsOld[2][3] = 21509201.6011814;
		navSolutions.channel.satPositionsOld[2][4] = 16825573.8507491;
		navSolutions.channel.satPositionsOld[2][5] = 8500609.61134778;
		navSolutions.channel.satPositionsOld[2][6] = -1234697.39820181;

		/* satPositions*/
		navSolutions.channel.satPositions[0][0] = 11891335.7172568;
		navSolutions.channel.satPositions[0][1] = -179376.609937144;
		navSolutions.channel.satPositions[0][2] = -16377980.8260109;
		navSolutions.channel.satPositions[0][3] = -2715770.32752303;
		navSolutions.channel.satPositions[0][4] = -19517593.9808421;
		navSolutions.channel.satPositions[0][5] = 14801267.7479571;
		navSolutions.channel.satPositions[0][6] = -15037337.0991438;
		navSolutions.channel.satPositions[1][0] = -10304907.6952046;
		navSolutions.channel.satPositions[1][1] = -23636344.8643187;
		navSolutions.channel.satPositions[1][2] = -11396726.7749187;
		navSolutions.channel.satPositions[1][3] = -15987737.1170402;
		navSolutions.channel.satPositions[1][4] = -6523465.36634381;
		navSolutions.channel.satPositions[1][5] = -20257588.3236579;
		navSolutions.channel.satPositions[1][6] = -21942061.9421135;
		navSolutions.channel.satPositions[2][0] = 21316480.7408344;
		navSolutions.channel.satPositions[2][1] = 12355653.0968673;
		navSolutions.channel.satPositions[2][2] = 17822705.1588088;
		navSolutions.channel.satPositions[2][3] = 21509241.6712939;
		navSolutions.channel.satPositions[2][4] = 16823544.1366570;
		navSolutions.channel.satPositions[2][5] = 8503526.03342716;
		navSolutions.channel.satPositions[2][6] = -1231519.52089491;

		/* xyzdtOld*/
		navSolutions.xyzdtOld[0] = -1288084.30832130;
		navSolutions.xyzdtOld[1] = -4720843.11694472;
		navSolutions.xyzdtOld[2] = 4079691.74066319;
		navSolutions.xyzdtOld[3] = -67432.8432687253;
		/*xyzdt*/
		navSolutions.xyzdt[0] = -1288083.42200892;
		navSolutions.xyzdt[1] = -4720843.71304684;
		navSolutions.xyzdt[2] = 4079690.56329526;
		navSolutions.xyzdt[3] = 19.7439436219622;

		/*rxTime*/
		navSolutions.rxTime[0] = 504620.209778135;
		navSolutions.rxTime[1] = 504621.209778069;

		vel = (fl64**) malloc(4 * sizeof(fl64*));
		for (i = 0; i < 4; i++) {
			vel[i] = (fl64*) malloc(1 * sizeof(fl64));
		}
		/* Actual code execution */
		retval = leastSquareVel1(vel, navSolutions, carrPhaseDiff,
				activeChnList, activeChnListSize);
		ck_assert_int_eq(retval, EXIT_SUCCESS);
		for (i = 0; i < 4; i++) {
			ck_assert(vel[i][0]-vel_expected[i]<DBL_EPSILON);
		}
		for (i = 0; i < 4; i++) {
			free(vel[i]);
		}
		free(vel);
		deleteNavSolution(&navSolutions);
	}END_TEST

Suite * leastSquareVel1_suite(void) {

	Suite *s;
	TCase *tc_core;

	s = suite_create("leastSquareVel1");

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

	s = leastSquareVel1_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
