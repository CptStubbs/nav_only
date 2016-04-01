/**
 * This is the LeastSquareSolution (lss) shared data structure.
 * It consists of an ECEF XYZ position and time, and also pseduoranges and
 * carrier phase values used to compute it.
 *
 * Exempt:
 *	mutex is used to lock the data structure when reading/writing
 * Scalar:
 * 	newKal is set true by nav, false by kal (nav will warn if not cleared)
 * 	validPos is true when we have a valid position solution
 * 	X,Y,Z is ECEF XYZ coordinates
 * 	rtow is the receiver time of week (at the sample number quoted below)
 * 	week is the GPS week number
 * 	smp is the integer sample number, frc the fractional part
 * 	valid1 indicates the L1C/A measurements used in position solution
 * 	valid2 indicates the L2C measurements used in position solution
 * Vectors:
 * 	pr1 is the L1C/A pseudoranges
 * 	cp1 is the L1C/A carrier phases
 * 	pr2 is the L2C pseudoranges
 * 	cp2 is the L2C carrier phases
 *
 *      Author: staffan
 **/

#ifndef SRC_INCLUDE_RNX_H_
	#define SRC_INCLUDE_RNX_H_
	#include "customDataTypes.h"							/// Standard types
	//#include "conf_lss.h"
	#include "conf.h"

	#include <pthread.h>											/// mutex locks
	#include <stdio.h>

	#define RNX_SCALARS \
		X(fl64, 	"double",		rtow) \
		X(fl64, 	"double",		X) \
		X(fl64, 	"double",		Y) \
		X(fl64, 	"double",		Z) \
		X(fl64, 	"double",		dt) \
		X(fl64, 	"double",		Vx) \
		X(fl64, 	"double",		Vy) \
		X(fl64, 	"double",		Vz) \
		X(fl64, 	"double",		dtRate) \
		X(fl64, 	"double",		HDOP) \
		X(fl64, 	"double",		VDOP) \
		X(fl64, 	"double",		TDOP) \
		X(ui64,		"uint64",		smp) \
		X(fl32,		"float",		frc) \
		X(ui32, 	"uint32",		numSat) \
		X(ui32,		"uint32",	valid1) \
		X(ui32,		"uint32",	valid2) \
		X(bool, 	"uint32", 		newKal) \
		X(bool, 	"uint32", 		newMea) \
		X(bool, 	"uint32",		validPos) \
		X(ui32,		"uint32",		week)

	#define RNX_VECTORS \
		X(fl64, 	"double",	pr1) \
		X(fl64, 	"double",	cp1) \
		X(fl64, 	"double",	dp1) \
		X(fl64, 	"double",		cno1) \
		X(fl64, 	"double",	pr2) \
		X(fl64, 	"double",	cp2) \
		X(fl64, 	"double",	dp2) \
		X(fl64, 	"double",		cno2) \
		X(fl64, 	"double",		Xsv) \
		X(fl64, 	"double",		Ysv) \
		X(fl64, 	"double",		Zsv) \
		X(fl64, 	"double",		svClk) \
		X(bool, 	"uint32",		validPRN) \
		X(ui32, 	"uint32",		prn)

	typedef struct {
		/// Fields exempt from log explosion
		//pthread_mutex_t mutex; This does not currently work as expected on the arm
		/// Scalar fields
		#define X(type,format,name) type name;
			RNX_SCALARS
		#undef X
		/// Vector fields fields
		#define X(type,format,name) type name[CONF_NUMCHN];
			RNX_VECTORS
		#undef X
	} rnx_s;

	/*!
	 * \brief Function to write the rnx headers on all new rnx files
	 * \param fid the file to write the header in
	 *
	 */
	void writernxHeader(FILE * fid);


#endif /* SRC_INCLUDE_RNX_H_ */
