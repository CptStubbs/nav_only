/*
 * test_leastSquareVel2.c
 *
 *  Created on: May 15, 2015
 *      Author: sean
 */

#include "../../src/nav/leastSquareVel2.h"
#include "../../src/include/customDataTypes.h"
#include <check.h>
#include <stdlib.h>
#include <float.h>
#include <stdio.h>

START_TEST(test_correct_code)
	{
		si32 retval = 0;
		si32 i = 0;
		fl64 ** vel;
		fl64 vel_expected[4] = { -0.494399,-0.924021756560064,
				0.574348807836781, 22.6795528347563 };
		fl64 transmitFreq[7] = { 1575419999.99642, 1575419999.99678,
				1575420000.00645, 1575420000.00412, 1575420000.0043,
				1575419999.9991, 1575419999.99427 };
		fl64 receivedFreq[7] = { 1575416938.80863, 1575419008.93521,
				1575421445.99892, 1575419657.73788, 1575420252.4331,
				1575421444.73477, 1575422997.87483 };
		fl64 ** satPositions;
		fl64 satPositions1[7] = { 11889583.8685301, -180234.831446901,
				-16380161.1689619, -2718384.0065623, -19516438.5410116,
				14801414.6187957, -15037445.2533632 };
		fl64 satPositions2[7] = { -10307008.4677562, -23635088.0390124,
				-11396225.0198663, -15987387.2007278, -6521663.27657975,
				-20258702.5421519, -21941774.6771709 };
		fl64 satPositions3[7] = { 21316411.0927666, 12358222.9383066,
				17821018.5091568, 21509201.6011814, 16825573.8507491,
				8500609.61134778, -1234697.39820181 };
		fl64 ** satVelocity;
		fl64 satVelocity1[7] = { 1751.79469809823, 858.312280842806,
				2180.25062152798, 2613.68464390265, -1155.46344096715,
				-146.832612489051, 108.05621163503 };
		fl64 satVelocity2[7] = { 2100.82135420343, -1256.94892794268,
				-501.683892506687, -349.84626640049, -1802.22559713724,
				1114.04341530777, -287.429788276339 };
		fl64 satVelocity3[7] = { 69.8766160914404, -2569.71320395319,
				1686.83358602316, 40.2894285399185, -2029.53518886809,
				2916.51053109981, 3177.85822227668 };
		fl64 userPos[3] = { -1288084.3083213, -4720843.11694472,
				4079691.74066319 };
		si32 activeChnList[7] = { 0,1,2,3,4,5,6 };
		si32 activeChnListSize = 7;
		vel = (fl64**) malloc(4*sizeof(fl64*));
		for(i=0;i<4;i++){
			vel[i]= (fl64*) malloc(1*sizeof(fl64));
		}
		satPositions = (fl64**) malloc(3*sizeof(fl64*));
		satPositions[0] = satPositions1;
		satPositions[1] = satPositions2;
		satPositions[2] = satPositions3;

		satVelocity = (fl64**) malloc(3*sizeof(fl64*));
		satVelocity[0] = satVelocity1;
		satVelocity[1] = satVelocity2;
		satVelocity[2] = satVelocity3;

		retval = leastSquareVel2(vel, transmitFreq, receivedFreq, satPositions,
				satVelocity, userPos, activeChnList, activeChnListSize);
		ck_assert_int_eq(retval, EXIT_SUCCESS);
		for (i = 0; i < 4; i++) {
			ck_assert(vel[i][0]-vel_expected[i]<DBL_EPSILON);
		}
		for(i=0;i<4;i++){
			free(vel[i]);
		}
		free(vel);
		free(satPositions);
		free(satVelocity);
	}END_TEST

Suite * leastSquareVel2_suite(void) {

	Suite *s;
	TCase *tc_core;

	s = suite_create("leastSquareVel2");

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

	s = leastSquareVel2_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
