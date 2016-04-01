/*
 * utils.h
 *
 *  Created on: May 13, 2015
 *      Author: sean
 */

#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_

#include "customDataTypes.h"
#include <stdio.h>
/*!
 * \brief return the norm of a vector. Same as Matlab norm.
 * \return the norm of the vector
 * \param vector - The vector you want to normalize
 * \param size - The size of the normalize vector.
 */
fl64 norm(
/* Output */
/* Input */
fl64* vector, si32 size);
/*!
 * \brief return the dot product of two vectors. Same as Matlab dot.
 * \return the dot product of the vectors
 * \param A the first vector in the dot product
 * \param B the second vector in the dot product
 * \param size The size of the two vectors. Size much match
 */

fl64 dot(
/* Output */
/* Input */
fl64 * A, fl64 * B, si32 size);

typedef enum {
	eq = 1, neq = 2, geq = 3, leq = 4, g = 5, l = 6
} opsChoice_e; /* Create a enumeration that can only */
/*!
 * \brief This is find, allways returns the vector of row col that satisfies the requirement
 * \return EXIT_SUCCESS on SUCCESS, EXIT_FAILURE otherwise
 * \param result a pointer a pointer that will be filled with the results
 * \param resSize - pointer to the size of the result vector
 * \param vector the vector that you want to check
 * \param size the length of the vector
 * \param operation - what operation you wish to do options are (==, ~=, <=, >=, <, >)
 * \param target the value to compare against the vector
 * \param howMany - a string describing how many results to return
 */

si32 findfl64(/* Output */
fl64 ** result, si32 * resSize,
/* Input */
fl64 * vector, si32 size, pi8 * operation, fl64 target, pi8 * howMany);

/*!
 * \brief This is find, allways returns the vector of row col that satisfies the requirement
 * \return EXIT_SUCCESS on SUCCESS, EXIT_FAILURE otherwise
 * \param result a pointer a pointer that will be filled with the results
 * \param resSize - pointer to the size of the result vector
 * \param vector the vector that you want to check
 * \param size the length of the vector
 * \param operation - what operation you wish to do options are (==, ~=, <=, >=, <, >)
 * \param target the value to compare against the vector
 * \param howMany - a string describing how many results to return
 */
si32 findsi32(/* Output */
si32 ** result, si32 * resSize,
/* Input */
si32 * vector, si32 size, pi8 * operation, si32 target, pi8 * howMany);

/*!
 * \brief inverts a matrix of arbitrary size.
 * \return EXIT_SUCCESS on SUCCESS and EXIT_FAILURE otherwise
 * \param matinv the inverted matrix same size as the origional
 * \param mat  the matrix to invert
 * \param size - The size of the matrix (size x size)
 */

si32 matInverse(
/* OUTPUT */
fl64 * matinv[],
/* INPUT */
fl64 * mat[], si32 size);

/*!
 * \brief find the determinant of an arbitrary matrix.
 * \return EXIT_SUCCESS on SUCCESS and EXIT_FAILURE otherwise
 * \param det the determinant of the matrix
 * \param mat  the matrix to find the determinant of
 * \param size - The size of the matrix (size x size)
 */
si32 matDeterminant(
/* OUTPUT */
fl64 * det,
/* INPUT */
fl64 ** mat, si32 size);

/*!
 * \brief multiply two arbitrary matrices,
 *  note result has to be the size (ArowxBcolumn)
 * \return EXIT_SUCCESS on SUCCESS and EXIT_FAILURE otherwise
 * \param result pointer to the output result array
 * \param A the first matrix to multiply
 * \param ARowSize the number of rows of the A array
 * \param AColSize the number of columns of the A array
 * \param B the second matrix to multiply
 * \param BRowSize the number of rows of the B array
 * \param BColSize the number of columns of the B array
 *
 */

si32 matMult(
/* OUTPUT */
fl64 ** result,
/* INPUT */
fl64 ** A, si32 ARowSize, si32 AColSize, fl64 ** B, si32 BRowSize,
		si32 BColSize);

/*!
 * \brief linearly interpolate to an index given a vector of X and Y.
 * \return EXIT_SUCCESS on SUCCESS and EXIT_FAILURE otherwise
 * \param Yval the interpolated y-value
 * \param Xmat the vector of Xvalues
 * \param Ymat the vector of Yvalues
 * \param sizeXmat size of the Xmat vector -> same for Ymat.
 * \param Xval the index of where to interpolate
 */

si32 interp1(
/* OUTPUT */
fl64 * Yval,
/* INPUT */
fl64 * Xmat, fl64 * Ymat, si32 sizeXmat, fl64 Xval);

/* TODO: Document me */
ui64 bin2dec(pi8 * binString);

/*!
 * \brief replace all instances of sub1 with sub2 in a string.
 * Mallocs the output
 * \return a mallocd string. check for NULL
 * \param input - input string to replace on
 * \param sub1 - substring to find
 * \param sub2 - substring to replace
 */
pi8* strrep(
/* Input */
pi8* input, pi8* sub1, pi8* sub2);
/*! \brief cross correlation function
 * \param input1 first matrix
 * \param input1Size  size of the first matrix
 * \param input2 second matrix
 * \param input2Size size of the second matrix.
 * \return an allocated result.
 */
si32* xcorr(si8 * input1, si32 input1Size, si8 * input2, si32 input2Size);

/*! \brief reverse a subsection of an array of si32 values
 *  \param inputArray the array to reverse
 *  \param left left value to reverse
 *  \param right right value to end reverse
 */
void reversesi32(si32 * inputArray, si32 left, si32 right);

/*! \brief reverse a subsection of an array of fl64 values
 *  \param inputArray the array to reverse
 *  \param left left value to reverse
 *  \param right right value to end reverse
 */
void reversefl64(fl64 * inputArray, si32 left, si32 right);

/*!
 * \brief rotate an array of si32 around a given index
 * \param inputArray the array to rotate
 * \param size the size of the input array
 * \param index the index to rotate around
 *
 */
void rotatesi32(si32 * inputArray, si32 size, si32 index);

/*!
 * \brief rotate an array of fl64 around a given index
 * \param inputArray the array to rotate
 * \param size the size of the input array
 * \param index the index to rotate around
 *
 */
void rotatefl64(fl64 * inputArray, si32 size, si32 index);

/*!
 * \brief Function to write the headers on all new generated files
 * \param fid the file to write the header in
 *
 */
void writeHeader(FILE * fid);


#endif /* INCLUDE_UTILS_H_ */
