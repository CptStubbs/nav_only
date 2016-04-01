/*
 * utils.c
 *
 *  Created on: May 13, 2015
 *      Author: sean
 */

#include <conf_nav.h>
#include <math.h>
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>

fl64 norm(fl64* vector, si32 size) {

	si32 i = 0;
	fl64 tmpPow = 0;
	fl64 result = 0;
	for (i = 0; i < size; i++) {
		tmpPow += pow(vector[i], 2);
	}
	result = sqrt(tmpPow);
	return result;

}

fl64 dot(fl64 * A, fl64 * B, si32 size) {
	si32 i = 0;
	fl64 result = 0;
	for (i = 0; i < size; i++) {
		result += A[i] * B[i];
	}
	return result;
}

si32 findfl64(fl64 ** result, si32 * resSize, fl64 * vector, si32 size,
		pi8 * operation, fl64 target, pi8 * howMany) {
	si32 i = 0;
	si32 count = 0;
	si32 lastTrack = 0;
	si32 breakFirst = 0;
	si32 loadAll = 0;
	/*si32 inda;*/
	fl64 * tempResult;
	opsChoice_e choice;
	tempResult = (fl64*) malloc(size * sizeof(fl64)); /*! the last 1ms data in the buffer. it includes un-processed samples which*/
	ERRORCHECK(tempResult == NULL);
	if (strcmp(howMany, "first") == 0) {
		breakFirst = 1;

	} else if (strcmp(howMany, "last") != 0) {
		loadAll = 1;
	}

	if (strcmp(operation, "==") == 0) {
		choice = eq;
	} else if (strcmp(operation, "!=") == 0) {
		choice = neq;
	} else if (strcmp(operation, ">=") == 0) {
		choice = geq;
	} else if (strcmp(operation, "<=") == 0) {
		choice = leq;
	} else if (strcmp(operation, ">") == 0) {
		choice = g;
	} else if (strcmp(operation, "<") == 0) {
		choice = l;
	} else {
		choice = eq;
	}
	for (i = 0; i < size; i++) {
		if ((breakFirst == 1) && (count == 1)) {
			break;
		}
		switch (choice) {
		case l:
			if (vector[i] < target) {
				if (loadAll == 1) {
					tempResult[count] = i;
				} else {
					lastTrack = i;
				}
				count++;
			}
			break;
		case g:
			if (vector[i] > target) {
				if (loadAll == 1) {
					tempResult[count] = i;
				} else {
					lastTrack = i;
				}
				count++;

			}
			break;
		case eq:
			if (vector[i] == target) {
				if (loadAll == 1) {
					tempResult[count] = i;
				} else {
					lastTrack = i;
				}
				count++;
			}
			break;
		case neq:
			if (vector[i] != target) {
				if (loadAll == 1) {
					tempResult[count] = i;
				} else {
					lastTrack = i;
				}
				count++;
			}
			break;
		case geq:
			if (vector[i] >= target) {
				if (loadAll == 1) {
					tempResult[count] = i;
				} else {
					lastTrack = i;
				}
				count++;
			}
			break;
		case leq:
			if (vector[i] <= target) {
				if (loadAll == 1) {
					tempResult[count] = i;
				} else {
					lastTrack = i;
				}
				count++;
			}
			break;
		default:
			break;
		}
	}
	if (count > 0) {
		if (strcmp(howMany, "last") == 0) {
			tempResult = (fl64*) realloc(tempResult, 1 * sizeof(fl64)); /*! the last 1ms data in the buffer. it includes un-processed samples which*/
			ERRORCHECK(tempResult == NULL);
			tempResult[0] = lastTrack;
			*resSize = 1;
		} else if (breakFirst == 1) {
			tempResult = (fl64*) realloc(tempResult, 1 * sizeof(fl64)); /*! the last 1ms data in the buffer. it includes un-processed samples which*/
			ERRORCHECK(tempResult == NULL);
			*resSize = 1;
			/*Allocate the new vector*/
			count = 0;
			for (i = 0; i < size; i++) {
				if (count == 1) {
					break;
				}
				switch (choice) {
				case l:
					if (vector[i] < target) {
						count++;
						tempResult[0] = i;
					}
					break;
				case g:
					if (vector[i] > target) {
						count++;
						tempResult[0] = i;
					}
					break;
				case eq:
					if (vector[i] == target) {
						count++;
						tempResult[0] = i;
					}
					break;
				case neq:
					if (vector[i] != target) {
						count++;
						tempResult[0] = i;
					}
					break;
				case geq:
					if (vector[i] >= target) {
						count++;
						tempResult[0] = i;
					}
					break;
				case leq:
					if (vector[i] <= target) {
						count++;
						tempResult[0] = i;
					}
					break;
				default:
					break;
				}
			}
		} else {
			tempResult = (fl64*) realloc(tempResult, count * sizeof(fl64)); /*! the last 1ms data in the buffer. it includes un-processed samples which*/
			ERRORCHECK(tempResult == NULL);
			*resSize = count;
		}
	} else {

		tempResult = NULL;
		*resSize = 0;
	}

	*result = tempResult;
	return EXIT_SUCCESS;
}

si32 findsi32(si32 ** result, si32 * resSize, si32 * vector, si32 size,
		pi8 * operation, si32 target, pi8 * howMany) {
	si32 i = 0;
	si32 count = 0;
	si32 lastTrack = 0;
	si32 breakFirst = 0;
	si32 loadAll = 0;
	/*si32 inda;*/
	si32 * tempResult;
	enum opsChoice {
		eq = 1, neq = 2, geq = 3, leq = 4, g = 5, l = 6
	};
	enum opsChoice choice;
	tempResult = (si32*) malloc(size * sizeof(si32)); /*! the last 1ms data in the buffer. it includes un-processed samples which*/
	ERRORCHECK(tempResult == NULL);

	if (strcmp(howMany, "first") == 0) {
		breakFirst = 1;

	} else if (strcmp(howMany, "last") != 0) {
		loadAll = 1;
	}

	if (strcmp(operation, "==") == 0) {
		choice = eq;
	} else if (strcmp(operation, "!=") == 0) {
		choice = neq;
	} else if (strcmp(operation, ">=") == 0) {
		choice = geq;
	} else if (strcmp(operation, "<=") == 0) {
		choice = leq;
	} else if (strcmp(operation, ">") == 0) {
		choice = g;
	} else if (strcmp(operation, "<") == 0) {
		choice = l;
	} else {
		choice = eq;
	}
	for (i = 0; i < size; i++) {
		if ((breakFirst == 1) && (count == 1)) {
			break;
		}
		switch (choice) {
		case l:
			if (vector[i] < target) {
				if (loadAll == 1) {
					tempResult[count] = i;
				} else {
					lastTrack = i;
				}
				count++;
			}
			break;
		case g:
			if (vector[i] > target) {
				if (loadAll == 1) {
					tempResult[count] = i;
				} else {
					lastTrack = i;
				}
				count++;

			}
			break;
		case eq:
			if (vector[i] == target) {
				if (loadAll == 1) {
					tempResult[count] = i;
				} else {
					lastTrack = i;
				}
				count++;
			}
			break;
		case neq:
			if (vector[i] != target) {
				if (loadAll == 1) {
					tempResult[count] = i;
				} else {
					lastTrack = i;
				}
				count++;
			}
			break;
		case geq:
			if (vector[i] >= target) {
				if (loadAll == 1) {
					tempResult[count] = i;
				} else {
					lastTrack = i;
				}
				count++;
			}
			break;
		case leq:
			if (vector[i] <= target) {
				if (loadAll == 1) {
					tempResult[count] = i;
				} else {
					lastTrack = i;
				}
				count++;
			}
			break;
		default:
			break;
		}
	}
	if (count > 0) {
		if (strcmp(howMany, "last") == 0) {
			tempResult = (si32*) realloc(tempResult, 1 * sizeof(si32)); /*! the last 1ms data in the buffer. it includes un-processed samples which*/
			ERRORCHECK(tempResult == NULL);
			tempResult[0] = lastTrack;
			*resSize = 1;
		} else if (breakFirst == 1) {
			tempResult = (si32*) realloc(tempResult, 1 * sizeof(si32)); /*! the last 1ms data in the buffer. it includes un-processed samples which*/
			ERRORCHECK(tempResult == NULL);
			*resSize = 1;
			/*Allocate the new vector*/
			count = 0;
			for (i = 0; i < size; i++) {
				if (count == 1) {
					break;
				}
				switch (choice) {
				case l:
					if (vector[i] < target) {
						count++;
						tempResult[0] = i;
					}
					break;
				case g:
					if (vector[i] > target) {
						count++;
						tempResult[0] = i;
					}
					break;
				case eq:
					if (vector[i] == target) {
						count++;
						tempResult[0] = i;
					}
					break;
				case neq:
					if (vector[i] != target) {
						count++;
						tempResult[0] = i;
					}
					break;
				case geq:
					if (vector[i] >= target) {
						count++;
						tempResult[0] = i;
					}
					break;
				case leq:
					if (vector[i] <= target) {
						count++;
						tempResult[0] = i;
					}
					break;
				default:
					break;
				}
			}
		} else {
			tempResult = (si32*) realloc(tempResult, count * sizeof(si32)); /*! the last 1ms data in the buffer. it includes un-processed samples which*/
			ERRORCHECK(tempResult == NULL);
			*resSize = count;
		}
	} else {

		tempResult = NULL;
		*resSize = 0;
	}

	*result = tempResult;
	return EXIT_SUCCESS;
}

si32 matDeterminant(
/* OUTPUT */
fl64 * det,
/* INPUT */
fl64 ** mat, si32 size) {
	fl64 determinant = 0;
	fl64 tempDeterminant = 0;
	fl64 ** tempMat;
	si32 i = 0, j = 0, k = 0;
	if (size == 1) {
		determinant = mat[0][0];
	} else if (size == 2) {
		determinant = ((mat[0][0] * mat[1][1]) - (mat[0][1] * mat[1][0]));
	} else {
		tempMat = (fl64 **) malloc((size - 1) * sizeof(fl64*));
		ERRORCHECK(tempMat == NULL);
		for (i = 0; i < size - 1; i++) {
			tempMat[i] = (fl64 *) malloc((size - 1) * sizeof(fl64));
			ERRORCHECK(tempMat[i] == NULL);

		}
		for (i = 0; i < size; i++) {
			for (j = 0; j < size - 1; j++) {
				for (k = 0; k < size - 1; k++) {
					if (k >= i) {
						tempMat[j][k] = mat[j + 1][k + 1];

					} else {
						tempMat[j][k] = mat[j + 1][k];
					}
				}
			}
			if (matDeterminant(&tempDeterminant, tempMat,
					size - 1) == EXIT_FAILURE) {
				return EXIT_FAILURE;
			}
			determinant += pow(-1, i) * mat[0][i] * tempDeterminant;
		}
		for (i = 0; i < size - 1; i++) {
			free(tempMat[i]);
		}
		free(tempMat);
	}

	*det = determinant;
	return EXIT_SUCCESS;
}

si32 matMult(fl64 ** result, fl64 ** A, si32 ARowSize, si32 AColSize, fl64 ** B,
		si32 BRowSize, si32 BColSize) {

	si32 i = 0, j = 0, k = 0;
	fl64 sum = 0;
	if (AColSize != BRowSize) {
		fprintf(stderr, "Cannot multiply two arrays, size mismatch ");
		return EXIT_FAILURE;
	}
	for (i = 0; i < ARowSize; i++) {
		for (j = 0; j < BColSize; j++) {
			sum = 0;
			for (k = 0; k < BRowSize; k++) {
				sum += A[i][k] * B[k][j];
			}
			result[i][j] = sum;
		}
	}

	return EXIT_SUCCESS;
}

si32 matInverse(fl64 ** matinv, fl64 * mat[], si32 size) {
	si32 i = 0, j = 0, k = 0, l = 0;
	fl64 det = 0;
	fl64 tempDet = 0;
	fl64 ** tempMat;

	/* Find the determinant */
	if (matDeterminant(&det, mat, size) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	if ((det <= DBL_EPSILON) && det >= (-DBL_EPSILON)) {
		fprintf(stderr,
				"Cannot invert this matrix as the determinate is zero\n");
		return EXIT_FAILURE;
	}
	tempMat = (fl64 **) malloc((size - 1) * sizeof(fl64*));
	ERRORCHECK(tempMat == NULL);

	for (i = 0; i < size - 1; i++) {
		tempMat[i] = (fl64*) malloc((size - 1) * sizeof(fl64));
		ERRORCHECK(tempMat[i] == NULL);
	}
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			/*! Build the matrix of minors */
			for (k = 0; k < size - 1; k++) {
				for (l = 0; l < size - 1; l++) {
					if ((k >= i && l >= j)) {
						tempMat[k][l] = mat[k + 1][l + 1];
					} else if ((k >= i && l < j)) {
						tempMat[k][l] = mat[k + 1][l];
					} else if ((k < i && l >= j)) {
						tempMat[k][l] = mat[k][l + 1];
					} else {
						tempMat[k][l] = mat[k][l];
					}
				}
			}
			/* Find the determinant of the sub matrix */
			if (matDeterminant(&tempDet, tempMat, size - 1) == EXIT_FAILURE) {
				return EXIT_FAILURE;
			}
			/*! Take the matrix of cofactors into account
			 *  and adjugate
			 */
			matinv[j][i] = pow(-1, i + j) * tempDet / det;

		}
	}

	for (i = 0; i < size - 1; i++) {
		free(tempMat[i]);
	}
	free(tempMat);
	return EXIT_SUCCESS;
}

si32 interp1(fl64 * Yval, fl64 * Xmat, fl64 * Ymat, si32 sizeXmat, fl64 Xval) {

	fl64 * index_a;
	fl64 * index_b;
	si32 index_aSize = 0;
	si32 index_bSize = 0;
	fl64 interpVal1 = 0;
	fl64 interpVal2 = 0;
	fl64 interp1Xval = 0;
	fl64 interp2Xval = 0;
	/*Linearly interpolate between two points.*/
	findfl64(&index_a, &index_aSize, Xmat, sizeXmat, "<=", Xval, "last");
	findfl64(&index_b, &index_bSize, Xmat, sizeXmat, ">=", Xval, "first");
	interpVal1 = Ymat[(si32) index_a[0]];
	interpVal2 = Ymat[(si32) index_b[0]];
	interp1Xval = Xmat[(si32) index_a[0]];
	interp2Xval = Xmat[(si32) index_b[0]];
	/*Linear interpolation formula*/
	*Yval = interpVal1
			+ ((interpVal2 - interpVal1) / (interp2Xval - interp1Xval))
					* (Xval - interp1Xval);
	free(index_a);
	free(index_b);
	return EXIT_SUCCESS;
}

ui64 bin2dec(pi8 * binString) {
	si32 inda = 0;
	si32 rem = 0;
	ui64 n = 0;
	ui64 addr = 0;
	ui64 intNumber = 0;

	if (strlen(binString) > LONGLENGTH) {
		if (binString[0] == '0') {
			addr = 0;
		} else {
			addr = pow(2, strlen(binString) - 1);
		}
		intNumber = bin2dec(&binString[1]) + addr;
		return intNumber;
	}

	n = atol(binString);
	inda = 0;

	/*Convert binary to decimal, tested and should work */
	while (n != 0) {
		rem = n % 10;/*TODO FIX so that it takes leading zeros!!!!*/
		n /= 10;
		intNumber += rem * pow(2, inda);
		inda++;
	}/*end while loop*/
	return intNumber;
}

pi8* strrep(pi8* input, pi8* sub1, pi8* sub2) {

	/*  The worst case is when every char of input is replaced by sub */
	pi8 * tempBuffer;
	/* Point to the start of the tempBuffer */
	pi8 * tempStart;
	pi8 * sub1loc;
	pi8 * outString;
	/* New insertion point*/
	pi8 * ins;
	si32 tempLength = strlen(input) * strlen(sub2);
	/* Number of replacements */
	si32 count = 0;
	si32 distance = 0;
	/* Length of the output buffer */
	si32 outLength = 0;

	tempStart = tempBuffer = (pi8*) malloc((tempLength + 1) * sizeof(pi8));
	if (tempBuffer == NULL) {
		return NULL;
	}
	if ((sub1loc = strstr(input, sub1)) == NULL) {
		outLength = strlen(input);
		outString = (pi8*) calloc((outLength + 1), sizeof(pi8));
		if (outString == NULL) {
			return NULL;
		}
		strncpy(outString, input, outLength);
	} else {
		strncpy(tempBuffer, input, tempLength);
		/*Find all the instances of str1*/
		ins = input;
		for (count = 0; (sub1loc = strstr(ins, sub1)) != NULL; ++count) {
			ins = sub1loc + strlen(sub1);
		}
		outLength = (strlen(input) + (strlen(sub2) - strlen(sub1)) * count);
		outString = (pi8*) calloc((outLength + 1), sizeof(pi8));
		if (outString == NULL) {
			return NULL;
		}
		while (count > 0) {
			ins = strstr(input, sub1);
			distance = ins - input;
			tempBuffer = strncpy(tempBuffer, input, distance) + distance;
			tempLength -= distance;
			tempBuffer = strncpy(tempBuffer, sub2, tempLength) + strlen(sub2);
			tempLength -= strlen(sub2);
			input += distance + strlen(sub1); /* move to next "end of rep" */
			count--;
		}
		strncpy(tempBuffer, input, tempLength);
		strncat(outString, tempStart, outLength);

	}
	free(tempStart);
	return outString;
}

si32* xcorr(si8 * input1, si32 input1Size, si8 * input2, si32 input2Size) {
	si32 * result;
	/* TODO Fix variable names */
	si32 temp = 0;
	si32 temp1 = 0;
	si32 temp2 = 0;
	si32 inde = 0;
	si32 indf = 0;
	si32 count = 0;

	/*Blah, mallocs*/
	result = (si32*) malloc((input1Size) * sizeof(si32)); /*! the last 1ms data in the buffer. it includes un-processed samples which*/
	if (result == NULL) {
		return NULL;
	}

	temp = input1Size - 1;
	temp2 = temp - input2Size;
	for (inde = 0; inde < input1Size; inde++) {

		temp1 = input2Size;
		for (indf = 0, count = inde; indf < input2Size; indf++, count++) {
			temp1 -= (input1[count] ^ input2[indf]) << 1;
		}
		if (inde > temp2) {
			input2Size--;
		}
		result[inde] = temp1;
	}

	return result;
}

void reversesi32(si32 * inputArray, si32 left, si32 right) {
	/* Yay pointer arithmetic */
	si32 * leftp = inputArray + left;
	si32 * rightp = inputArray + right;
	while (leftp < rightp) {
		/* In place swap */
		*rightp = *rightp ^ *leftp;
		*leftp = *rightp ^ *leftp;
		*rightp = *rightp ^ *leftp;
		leftp++;
		rightp--;
	}

}

void reversefl64(fl64 * inputArray, si32 left, si32 right) {
	/* Yay pointer arithmetic */
	fl64 * leftp = inputArray + left;
	fl64 * rightp = inputArray + right;
	fl64 temp = 0;
	while (leftp < rightp) {
		temp = *rightp;
		*rightp = *leftp;
		*leftp = temp;
		leftp++;
		rightp--;
	}

}

void rotatesi32(si32 * inputArray, si32 size, si32 index) {
	reversesi32(inputArray, 0, size - 1);
	reversesi32(inputArray, 0, index - 1);
	reversesi32(inputArray, index, size - 1);
}

void rotatefl64(fl64 * inputArray, si32 size, si32 index) {
	reversefl64(inputArray, 0, size - 1);
	reversefl64(inputArray, 0, index - 1);
	reversefl64(inputArray, index, size - 1);
}

void writeHeader(FILE * fid){
	fprintf(fid, "Pyxis File Header:\t");
#if defined(GITVERSION) && defined(GITBRANCH)
	fprintf(fid, "Version %s branch %s\t",GITVERSION,GITBRANCH);
#endif
#if defined(CCNAME) && defined(__VERSION__)
	fprintf(fid, "Compiler %s version %s\t", CCNAME, __VERSION__);
#endif
#if defined(HARDWAREBUILD) && defined(BASEBANDVERSION)
	fprintf(fid, "Hardware %s baseband %s\t", HARDWAREBUILD, BASEBANDVERSION);
#endif
	fprintf(fid, "\r\n");

}


