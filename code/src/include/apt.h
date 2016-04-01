/**
 This is the apt (amplitude, phase, time) interface between chn and nav.
 It consists of tracking information (such as correlator values and sample numbers).
 chn will store tracking information, and when APT_LEN values for one prn have been
 gathered, send them off to nav.

 nav should poll the newApt flag, and when a new entry is available, grab the data
 and process it. We will use mutex locks to ensure integrity between the threads.
 **/

#ifndef SRC_INCLUDE_APT_H_
	#define SRC_INCLUDE_APT_H_
	#include "customDataTypes.h"							/// Standard types
	#include "conf_apt.h"											/// APT_LEN and APT_NUM is set here

	#include <pthread.h>											/// mutex locks
	#include <stdio.h>

	#define APT_EMT_SCALARS \
		X(bool, 	"uint32", 		newApt) \
		X(ui32, 	"uint32",			prn) \
		X(ui16, 	"uint16",			id) \
		X(ui16, 	"uint16",			len) \

	// TODO - rename lock to mutex
	// TODO - rename lockLoss to lock (and make it match the chn definition)

	#define APT_EMT_VECTORS \
		X(fl64,		"double",			dopp) \
		X(fl64,		"double",			carr) \
		X(fl64,		"double",			VSM)\
		X(fl64,		"double",			lockTime)\
		X(ui64,		"uint64",		absoluteSample) \
		X(fl32,		"float",			absSamFrac) \
		X(si32,		"int32",			P_I) \
		X(si32,     "int32",           L2P_I)

	// TODO - start using smp/frc instead of absoluteSample/absSamFrac
	// TODO - remove absoluteSample/absSamFrac

	typedef struct {
		/// Standard fields
		#define X(type,format,name) type name;
			APT_EMT_SCALARS
		#undef X
		pthread_mutex_t lock;										/// Pthread lock
		#define X(type,format,name) type name[APT_LEN];
			APT_EMT_VECTORS
		#undef X
	} apt_emt_s;

	/// Sara/Daehee - you can add new fields by modifying the macros above

	typedef struct {
		ui16 indRead;									/// Index to the next one to read for nav
		ui16 indWrite;								/// Index to the next one to write for chn
		apt_emt_s emt[APT_NUM];				/// Array of apt_s
	} apt_s;

	// Additional fields that may be added later
	// fl32	codeFreq[APT_LEN];								// Code Frequency
	// fl32	carrFreq[APT_LEN];								// Carrier Frequency
	// fl32	carrPhase[APT_LEN];								// Carrier Phase
	// si32	prn;															// Code index, will eventually be changed to a pointer to a svi_sig_s
	// fl32	cnoValue;													// Latest C/N_0 estimate
	// ui64	cnoSample;												// Sample number of the latest C/N_0 estimate
	// ui32	pessLockCnt;											// Pessimistic carrier lock counter
	// ui32	optiLockCnt;											// Optimistic carrier lock counter
	/*!
	 * \brief Function to write the apt headers on all new apt files
	 * \param fid the file to write the header in
	 *
	 */
	void writeaptemtHeader(FILE * fid);

#endif /* SRC_INCLUDE_APT_H_ */
