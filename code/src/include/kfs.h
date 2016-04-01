/**
 * This is the KalmanFilterSolution (kfs) shared data structure.
 * It consists of an ECEF XYZ position and time, useful quality
 * indicators, etc.
 *
 * Exempt:
 *	mutex is used to lock the data structure when reading/writing
 * Scalar:
 * 	newSch is set true by kal, false by sch (kal will warn if not cleared)
 * 	newMea is set true by kal, false by mea (kal will warn if not cleared)
 * 	X				- X position estimate [m]
 * 	Y				- Y position estimate [m]
 * 	Z				- Z position estimate [m]
 * 	Xvel		- X velocity estimate [m/s]
 * 	Yvel		- Y velocity estimate [m/s]
 * 	Zvel		- Z velocity estimate [m/s]
 * 	Xvar		- Estimated variance of the X position [m^2]
 * 	Yvar 		- Estimated variance of the Y position [m^2]
 * 	Zvar 		- Estimated variance of the Z position [m^2]
 * 	Trcv 		- Receiver time of week (at the sample number quoted below) [s]
 *  Tbias 	- Estimated frequency offset of the sampling clock [Hz]
 *  Tdrift 	- Estimated derivative of frequency [Hz^2]
* 	Tweek		- Weeks since Jan 6, 1980 [number of Weeks]
 * 	smp 		- Integer sample number [samples]
 * 	frc 		- Fractional sample number [2^-32 samples]
 * 	validL1	- L1C/A measurements used in kalman filter solution
 * 	validL2	- L2C   measurements used in kalman filter solution
 *
 *      Author: staffan
 **/

#ifndef SRC_INCLUDE_KFS_H_
	#define SRC_INCLUDE_KFS_H_
	#include "customDataTypes.h"							/// Standard types
	#include <pthread.h>											/// mutex locks

	#define KFS_SCALARS \
		X(bool, 	"%d", 		newKal) \
		X(bool, 	"%d", 		newMea) \
		X(fl64, 	"%f",			validPos) \
		X(fl64, 	"%f",			X) \
		X(fl64, 	"%f",			Y) \
		X(fl64, 	"%f",			Z) \
		X(fl64, 	"%f",			Xvel) \
		X(fl64, 	"%f",			Yvel) \
		X(fl64, 	"%f",			Zvel) \
		X(fl64, 	"%f",			Xvar) \
		X(fl64, 	"%f",			Yvar) \
		X(fl64, 	"%f",			Zvar) \
		X(fl64,		"%f",			Trcv) \
		X(fl64, 	"%f",			Tbias) \
		X(fl64, 	"%f",			Tdrift) \
		X(ui32,		"%u",			Tweek) \
		X(ui64,		"%llu",		smp) \
		X(ui32,		"%u",			frc) \
		X(ui32,		"h%08X",	validL1) \
		X(ui32,		"h%08X",	validL2)

	typedef struct {
		/// Fields exempt from log explosion
		pthread_mutex_t mutex;
		/// Scalar fields
		#define X(type,format,name) type name;
			KFS_SCALARS
		#undef X
//		/// Vector fields fields
//		#define X(type,format,name) type name[CONF_LSS_LEN];
//			LSS_VECTORS
//		#undef X
	} kfs_s;

#endif /* SRC_INCLUDE_KFS_H_ */
