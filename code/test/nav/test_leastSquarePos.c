/*
 * test_leastSquarePos.c
 *
 *  Created on: May 28, 2015
 *      Author: sean
 */

/*
 * test_leastSquareVel2.c
 *
 *  Created on: May 15, 2015
 *      Author: sean
 */

#include "../../src/nav/leastSquarePos.h"
#include "../../src/include/customDataTypes.h"
#include "../../src/include/initEph.h"
#include <check.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdio.h>

START_TEST(test_correct_code)
	{
		si32 retval = 0;
		si32 i = 0;
		si32 nmbOfSatellites = 7;
		fl64 pos[4] = { NAN, NAN, NAN, NAN };
		fl64 el[7] ={0,0,0,0,0,0,0};
		fl64 elSol[7] = { 34.0737827160265, 67.3054010543388, 51.5154505335219,
				72.3064086176064, 36.2652853126411, 29.0754216257933,
				32.2046730661638 };
		fl64 az[7] ={0,0,0,0,0,0,0};
		fl64 azSol[7] = { 49.84223057891144037512, 130.70486253163412015965, 286.65959477760912932354,
				14.5252820061485, 288.004065304747, 100.19754244593701741906,
				207.079537476701 };
		fl64 iono[7] ={0,0,0,0,0,0,0};
		fl64 ionoSol[7] = { 0, 0, 0, 0, 0, 0, 0 };
		fl64 trop[7] ={0,0,0,0,0,0,0};
		fl64 tropSol[7] = { 0, 0, 0, 0, 0, 0, 0 };
		fl64 dop[5] ={0,0,0,0,0};
		fl64 dopSol[5] = { 2.90848339648789, 2.46613429544748, 1.99831848761206,
				1.44517873816704, 1.54190054947247 };
		fl64 posSol[4] = { -1288084.02344772, -4720842.55463995,
				4079691.42906543286517262459, -67434.11619599783443845809};
		fl64 obs[7] = { 22337037.8427271, 20608878.3072235, 21407111.9477515,
				20735654.674826, 22247900.5353277, 22732570.199603,
				22600792.0131866 };
		almanac_s almanac;
		almanac.a0=0;
		si32 list[6] = {13, 17, 20, 21, 13, 16};
		fl64 rxTime = 504620.219542447;

		fl64 ** satPos;
		satPos = (fl64 **) malloc(sizeof(fl64 *) * 3);
		for (i = 0; i < 3; i++) {
			satPos[i] = (fl64 *) malloc(sizeof(fl64) * nmbOfSatellites);
		}

		satPos[0][0] = 11889601.367614;
		satPos[0][1] = -180226.257567648;
		satPos[0][2] = -16380139.3898692;
		satPos[0][3] = -2718357.89782956;
		satPos[0][4] = -19516450.0832209;
		satPos[0][5] = 14801413.1520431;
		satPos[0][6] = -15037444.1739514;
		satPos[1][0] = -10306987.4821754;
		satPos[1][1] = -23635100.5949658;
		satPos[1][2] = -11396230.0313227;
		satPos[1][3] = -15987390.6954344;
		satPos[1][4] = -6521681.27944586;
		satPos[1][5] = -20258691.4136682;
		satPos[1][6] = -21941777.5483668;
		satPos[2][0] = 21316411.7907573;
		satPos[2][1] = 12358197.2688094;
		satPos[2][2] = 17821035.3593523;
		satPos[2][3] = 21509202.0036204;
		satPos[2][4] = 16825553.5771995;
		satPos[2][5] = 8500638.74511287;
		satPos[2][6] = -1234665.65373016;

		retval = leastSquarePos(pos, el, az, iono, trop, dop, satPos,
				nmbOfSatellites, obs, almanac, rxTime,list);
		ck_assert_int_eq(retval, EXIT_SUCCESS);
		for (i = 0; i < 7; i++) {
			ck_assert(el[i]-elSol[i]<DBL_EPSILON);
			ck_assert(az[i]-azSol[i]<DBL_EPSILON);
			ck_assert(iono[i]-ionoSol[i]<DBL_EPSILON);
			ck_assert(trop[i]-tropSol[i]<DBL_EPSILON);
		}
		for (i = 0; i < 4; i++) {
			ck_assert(pos[i]-posSol[i]<DBL_EPSILON);
		}
		for (i = 0; i < 5; i++) {
			ck_assert(dop[i]-dopSol[i] < DBL_EPSILON);
		}
		for (i = 0; i < 3; i++) {
			free(satPos[i]);
		}
		free(satPos);

	}END_TEST

Suite *leastSquarePos_suite(void) {

	Suite *s;
	TCase *tc_core;

	s = suite_create("leastSquarePos");

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

	s = leastSquarePos_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
