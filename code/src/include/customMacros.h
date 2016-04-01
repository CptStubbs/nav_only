/*
 * customMacros.h
 *
 *  Created on: Oct 2, 2015
 *      Author: staffan
 */

#ifndef SRC_INCLUDE_CUSTOMMACROS_H_
	#define SRC_INCLUDE_CUSTOMMACROS_H_

	/// Sizes of vectors, arrays and higher dimension objects:
	/// 1D vector
	#define length_of_vector(name) \
		sizeof(name)/sizeof(name[0])
	/// 2D matrix
	#define rows_of_matrix(name)       \
			(sizeof(name   ) / sizeof(name[0][0]) / columns_of_matrix(name))
	#define columns_of_matrix(name)    \
			(sizeof(name[0]) / sizeof(name[0][0]))

	/// Degrees to radians
	#define DEGTORAD 	\
		M_PI/180.0
	/// Radians to degrees
	#define RADTODEG 	\
		180.0/M_PI
     #define str(s) #s
     #define xstr(s) str(s)
#endif /* SRC_INCLUDE_CUSTOMMACROS_H_ */
