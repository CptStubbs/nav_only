/*
 * test_utils.c
 *
 *  Created on: May 13, 2015
 *      Author: sean
 */

/*
 * test_e_r_corr.c
 *
 *  Created on: May 13, 2015
 *      Author: sean
 */

#include "../../src/include/utils.h"
#include "../../src/include/customDataTypes.h"
#include <check.h>
#include <conf_nav.h>
#include <stdlib.h>
#include <float.h>
#include <stdio.h>

START_TEST(test_correct_code_norm)
	{
		fl64 vector[3] = { 1, 2, 3 };
		fl64 size = 3;
		fl64 vecNorm = 0;
		vecNorm = norm(vector, size);
		ck_assert((vecNorm - 3.7416573868) < DBL_EPSILON);
	}END_TEST

START_TEST(test_correct_code_dot)
	{
		fl64 A[3] = { 1, 2, 3 };
		fl64 B[3] = { 1, 2, 3 };
		fl64 size = 3;
		fl64 vecDot = 0;
		vecDot = dot(A, B, size);
		ck_assert((vecDot - 14) < DBL_EPSILON);
	}END_TEST

START_TEST(test_correct_code_matInverse)
	{
		fl64 ** A;
		fl64 ** Ainv;
		fl64 Sol[4][4] = { { -1, 0, 2, -1 }, { 0.39, -0.01, -0.97, 0.79 }, {
				0.34, -0.06, 0.18, -0.26 }, { 0.05, 0.05, -0.15, 0.05 } };
		fl64 size = 4;
		si32 retval = 0;
		si32 i = 0, j = 0;
		fl64 A1[4] = { 1, 2, 3, 4 };
		fl64 A2[4] = { 3, 3, 2, 23 };
		fl64 A3[4] = { 2, 3, 2, 3 };
		fl64 A4[4] = { 2, 4, 1, 2 };
		A = (fl64**) malloc(size * sizeof(fl64*));
		A[0] = A1;
		A[1] = A2;
		A[2] = A3;
		A[3] = A4;
		Ainv = (fl64**) malloc(size * sizeof(fl64*));
		for (i = 0; i < size; i++) {
			Ainv[i] = (fl64*) malloc(size * sizeof(fl64));
		}
		retval = matInverse(Ainv, A, size);
		ck_assert_int_eq(retval, EXIT_SUCCESS);
		for (i = 0; i < size; i++) {
			for (j = 0; j < size; j++) {
				ck_assert(Ainv[i][j]-Sol[i][j]<DBL_EPSILON);
			}
		}
		free(A);
		for (i = 0; i < size; i++) {
			free(Ainv[i]);
		}
		free(Ainv);
	}END_TEST

START_TEST(test_correct_code_matDeterminant)
	{
		fl64 ** A;
		fl64 det = 0;
		fl64 size = 3;
		si32 retval = 0;
		fl64 A1[3] = { 8, 1, 6 };
		fl64 A2[3] = { 3, 5, 7 };
		fl64 A3[3] = { 4, 9, 2 };
		A = (fl64**) malloc(3 * sizeof(fl64*));
		A[0] = A1;
		A[1] = A2;
		A[2] = A3;
		retval = matDeterminant(&det, A, size);
		ck_assert_int_eq(retval, EXIT_SUCCESS);
		ck_assert((det - (-360)) < DBL_EPSILON);
		free(A);
	}END_TEST

START_TEST(test_correct_code_matMult)
	{
		fl64 ** A;
		fl64 ** B;
		fl64 ** result;
		fl64 Sol[3][3] = { { 10, 20, 30 }, { 10, 20, 30 }, { 10, 20, 30 } };
		fl64 ARowSize = 3;
		fl64 AColSize = 4;
		fl64 BRowSize = 4;
		fl64 BColSize = 3;
		si32 retval = 0;
		si32 i = 0, j = 0;
		si32 tmp = 0;
		A = (fl64**) malloc(ARowSize * sizeof(fl64*));
		B = (fl64**) malloc(BRowSize * sizeof(fl64*));
		result = (fl64**) malloc(ARowSize * sizeof(fl64*));
		for (i = 0; i < ARowSize; i++) {
			A[i] = (fl64*) malloc(AColSize * sizeof(fl64));
			result[i] = (fl64*) malloc(BColSize * sizeof(fl64));
			tmp = 1;
			for (j = 0; j < AColSize; j++) {
				A[i][j] = tmp;
				tmp++;
			}
		}
		for (i = 0; i < BRowSize; i++) {
			B[i] = (fl64*) malloc(BColSize * sizeof(fl64));

			tmp = 1;
			for (j = 0; j < BColSize; j++) {
				B[i][j] = tmp;
				tmp++;
			}
		}

		retval = matMult(result, A, ARowSize, AColSize, B, BRowSize, BColSize);
		ck_assert_int_eq(retval, EXIT_SUCCESS);
		for (i = 0; i < ARowSize; i++) {
			for (j = 0; j < BColSize; j++) {
				ck_assert((result[i][j]-Sol[i][j]) < DBL_EPSILON);
			}
		}
		for (i = 0; i < ARowSize; i++) {
			free(A[i]);
			free(result[i]);
		}
		for (i = 0; i < BRowSize; i++) {
			free(B[i]);
		}
		free(A);
		free(B);
		free(result);
	}END_TEST

START_TEST(test_correct_code_strrep)
	{
		pi8 * outString;
		pi8 * input = "abbcbbd";
		pi8 * find = "bb";
		pi8 * replace = "q";
		pi8 * goal = "aqcqd";
		outString = strrep(input, find, replace);

		ck_assert(outString != NULL);
		ck_assert_int_eq(strcmp(outString, goal), 0);
		free(outString);
	}END_TEST
START_TEST(test_correct_code_xcorr)
	{
		si32 * result;
		si8 input1[10] = { 0, 1, 0, 1, 1, 1, 0, 0, 0, 0 };
		si32 input1Size = 10;
		si8 input2[5] = { 0, 1, 1, 1, 0 };
		si32 input2Size = 5;
		si32 trueVal[10] = { 1, -1, 5, 1, -1, -3, -2, -1, 0, 1 };
		si32 inda = 0;
		result = xcorr(input1, input1Size, input2, input2Size);
		ck_assert(result != NULL);

		for (inda = 0; inda < 10; inda++) {
			ck_assert_int_eq(trueVal[inda], result[inda]);

		}
		free(result);
	}END_TEST

START_TEST(test_correct_code_find)
	{

		fl64 * result = NULL;
		si32 resSize = 0;
		fl64 vector[10] = { 2, 3, 4, 2, 6, 7, 8, 9, 0, 0 };
		fl64 trueVal[3] = { 5, 6, 7 };
		si32 size = 10;
		pi8 * operation = ">";
		fl64 target = 6;
		pi8 * howMany = "all";
		si32 inda = 0;
		findfl64(&result, &resSize, vector, size, operation, target, howMany);
		ck_assert(result != NULL);

		for (inda = 0; inda < resSize; inda++) {
			ck_assert_int_eq(trueVal[inda], result[inda]);
		}
		free(result);
	}END_TEST

START_TEST(test_correct_code_interp1)
	{
		fl64 Yval = 0;
		fl64 Xmat[9] = { 0, 0.785398163397448, 1.5707963267949,
				2.35619449019234, 3.14159265358979, 3.92699081698724,
				4.71238898038469, 5.49778714378214, 6.28318530717959 };
		fl64 Ymat[9] = { 0, 0.707106781186547, 1, 0.707106781186548,
				1.22464679914735e-16, -0.707106781186547, -1,
				-0.707106781186548, -2.44929359829471e-16 };
		fl64 trueVal = 0.973598; //0.9736
		si32 sizeXmat = 9;
		fl64 Xval = 1.5;
		interp1(&Yval, Xmat, Ymat, sizeXmat, Xval);

		ck_assert((trueVal-Yval)<DBL_EPSILON);

	}END_TEST

START_TEST(test_correct_code_reversesi32)
	{
		si32 i = 0;
		si32 testMat[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, -10 };
		si32 ansMat[10] = { -10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
		reversesi32(testMat, 0, 9);

		for (i = 0; i < 10; i++) {
			ck_assert_int_eq(testMat[i], ansMat[i]);
		}

	}END_TEST

START_TEST(test_correct_code_reversefl64)
	{
		si32 i = 0;
		fl64 testMat[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, -10 };
		fl64 ansMat[10] = { -10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
		reversefl64(testMat, 0, 9);

		for (i = 0; i < 10; i++) {
			ck_assert(testMat[i] - ansMat[i] < DBL_EPSILON);
		}

	}END_TEST

START_TEST(test_correct_code_rotatesi32)
	{
		si32 i = 0;
		si32 testMat[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, -10 };
		si32 ansMat[10] = { 9, -10, 1, 2, 3, 4, 5, 6, 7, 8 };
		rotatesi32(testMat, 10, 2);

		for (i = 0; i < 10; i++) {
			ck_assert_int_eq(testMat[i], ansMat[i]);
		}

	}END_TEST

START_TEST(test_correct_code_rotatefl64)
	{
		si32 i = 0;
		fl64 testMat[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, -10 };
		fl64 ansMat[10] = { 9, -10, 1, 2, 3, 4, 5, 6, 7, 8 };
		rotatefl64(testMat, 10, 2);

		for (i = 0; i < 10; i++) {
			ck_assert(testMat[i] - ansMat[i] < DBL_EPSILON);
		}

	}END_TEST

Suite * util_suite(void) {

	Suite *s;
	TCase *tc_norm;
	TCase *tc_dot;
	TCase *tc_det;
	TCase *tc_inv;
	TCase *tc_mult;
	TCase * tc_strrep;
	TCase * tc_xcorr;
	TCase * tc_find;
	TCase * tc_interp1;
	TCase * tc_rev32;
	TCase * tc_rev64;
	TCase * tc_rot32;
	TCase * tc_rot64;

	s = suite_create("Util");

	/*Core test case*/
	tc_norm = tcase_create("Core_norm");
	tc_dot = tcase_create("Core_dot");
	tc_det = tcase_create("Core_determinate");
	tc_inv = tcase_create("Core_inverse");
	tc_mult = tcase_create("Core_multiplicate");
	tc_strrep = tcase_create("Core_string_replace");
	tc_xcorr = tcase_create("Core_cross_correlation");
	tc_find = tcase_create("Core_find");
	tc_interp1 = tcase_create("Core_interp1");
	tc_rev32 = tcase_create("Core_reversesi32");
	tc_rev64 = tcase_create("Core_reversefl64");
	tc_rot32 = tcase_create("Core_rotatesi32");
	tc_rot64 = tcase_create("Core_rotatefl64");

	tcase_add_test(tc_norm, test_correct_code_norm);
	tcase_add_test(tc_dot, test_correct_code_dot);
	tcase_add_test(tc_det, test_correct_code_matDeterminant);
	tcase_add_test(tc_inv, test_correct_code_matInverse);
	tcase_add_test(tc_mult, test_correct_code_matMult);
	tcase_add_test(tc_strrep, test_correct_code_strrep);
	tcase_add_test(tc_xcorr, test_correct_code_xcorr);
	tcase_add_test(tc_find, test_correct_code_find);
	tcase_add_test(tc_interp1, test_correct_code_interp1);
	tcase_add_test(tc_rev32, test_correct_code_reversesi32);
	tcase_add_test(tc_rev64, test_correct_code_reversefl64);
	tcase_add_test(tc_rot32, test_correct_code_rotatesi32);
	tcase_add_test(tc_rot64, test_correct_code_rotatefl64);

	suite_add_tcase(s, tc_norm);
	suite_add_tcase(s, tc_dot);
	suite_add_tcase(s, tc_det);
	suite_add_tcase(s, tc_inv);
	suite_add_tcase(s, tc_mult);
	suite_add_tcase(s, tc_strrep);
	suite_add_tcase(s, tc_xcorr);
	suite_add_tcase(s, tc_find);
	suite_add_tcase(s, tc_interp1);
	suite_add_tcase(s, tc_rev32);
	suite_add_tcase(s, tc_rev64);
	suite_add_tcase(s, tc_rot32);
	suite_add_tcase(s, tc_rot64);

	return s;
}

si32 main(void) {
	si32 number_failed = 0;
	Suite *s;
	SRunner *sr;

	s = util_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
