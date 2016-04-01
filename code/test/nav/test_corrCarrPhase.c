/*
 * test_corrCarrPhase.c
 *
 *  Created on: May 22, 2015
 *      Author: sean
 */

/*
 * test_corrCarrPhase.c
 *
 *  Created on: May 18, 2015
 *      Author: sean
 */

/*
 * test_corrCarrPhase.c
 *
 *  Created on: May 8, 2015
 *      Author: sean
 */

#include "../../src/nav/corrCarrPhase.h"
#include "../../src/include/customDataTypes.h"
#include "../../src/include/initNavSolutions.h"
#include <check.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

START_TEST(test_correct_code)
	{
		navSolutions_s navSolutions;
//		fl64 carrPhase[7][2];
		si32 i = 0;
		fl64 correctedCPSolution[7] = { 118090995.628417, 109287050.443315,
				112378521.973601445556, 108747056.928966, 117209226.825253,
				119873466.332702368498, 118945108.335601016879 };
		si32 numberOfChannels=7;
		initNavSolution(&navSolutions);

		fl64 ** carrPhase;
		carrPhase = (fl64 **) malloc(sizeof(fl64 *) * 7);
		for (i = 0; i < 7; i++) {
			carrPhase[i] = (fl64 *) malloc(sizeof(fl64) * 2);
		}


		carrPhase[0][0] = 75693.037527838;
		carrPhase[0][1] = 78752.8805858194;
		carrPhase[1][0] = 24424.6385696586;
		carrPhase[1][1] = 25417.7021257508;
		carrPhase[2][0] = -35831.0585657648;
		carrPhase[2][1] = -37276.2058779359;
		carrPhase[3][0] = 8447.63195875006;
		carrPhase[3][1] = 8791.55652606419;
		carrPhase[4][0] = -6408.08449008901;
		carrPhase[4][1] = -6659.37316523052;
		carrPhase[5][0] = -35958.0376105839;
		carrPhase[5][1] = -37404.5148728418;
		carrPhase[6][0] = -74386.9125447999;
		carrPhase[6][1] = -77383.542456402;

		/*Rx time */
		navSolutions.rxTime[1] = 504622.999999997;
		navSolutions.rxTime[0] = 504621.999999991;
		/* Raw P */
		navSolutions.channel.rawP[0] = 22471963.1540274;
		navSolutions.channel.rawP[1] = 20796630.3253216;
		navSolutions.channel.rawP[2] = 21384915.1719004;
		navSolutions.channel.rawP[3] = 20693871.9261267;
		navSolutions.channel.rawP[4] = 22304166.781204;
		navSolutions.channel.rawP[5] = 22811163.7670922;
		navSolutions.channel.rawP[6] = 22634495.7210202;
		/* CarrCorrCount */
		navSolutions.channel.carrCorrCount[0] = 3;
		navSolutions.channel.carrCorrCount[1] = 3;
		navSolutions.channel.carrCorrCount[2] = 3;
		navSolutions.channel.carrCorrCount[3] = 3;
		navSolutions.channel.carrCorrCount[4] = 3;
		navSolutions.channel.carrCorrCount[5] = 3;
		navSolutions.channel.carrCorrCount[6] = 3;
		/*correctedCP */
		navSolutions.channel.correctedCP[0] = 118088050.524263;
		navSolutions.channel.correctedCP[1] = 109286172.118663;
		navSolutions.channel.correctedCP[2] = 112380081.859818;
		navSolutions.channel.correctedCP[3] = 108746827.743303;
		navSolutions.channel.correctedCP[4] = 117209592.852832;
		navSolutions.channel.correctedCP[5] = 119875027.548869;
		navSolutions.channel.correctedCP[6] = 118948219.704417;
		/*correctedP*/
		navSolutions.channel.correctedP[0] = 22471964.9011822;
		navSolutions.channel.correctedP[1] = 20796632.0724764;
		navSolutions.channel.correctedP[2] = 21384916.9190553;
		navSolutions.channel.correctedP[3] = 20693873.6732815;
		navSolutions.channel.correctedP[4] = 22304168.5283588;
		navSolutions.channel.correctedP[5] = 22811165.514247;
		navSolutions.channel.correctedP[6] = 22634497.468175;
		/*dtRate */
		navSolutions.dtRate = 21.8340873963768;
		/* Minor fix to make the unit test work closer to the matlab */
		corrCarrPhase(&navSolutions, carrPhase, numberOfChannels);

		for(i=0;i<7;i++){
			ck_assert(navSolutions.channel.correctedCP[i]-correctedCPSolution[i]<DBL_EPSILON);
			ck_assert_int_eq(navSolutions.channel.carrCorrCount[i],4);

		}

		deleteNavSolution(&navSolutions);

		for (i = 0; i < 7; i++) {
			free(carrPhase[i]);
		}
		free(carrPhase);


	}END_TEST

Suite * corrCarrPhase_suite(void) {

	Suite *s;
	TCase *tc_core;

	s = suite_create("corrCarrPhase");

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

	s = corrCarrPhase_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
