/** 
 * This header file defines the information about all the SV's (space vehicles)
 * needed for the receiver to function. All data types in here are prefixed with
 * svi (Space Vehicle Information), and this data will be global.
 * Further, this data structure is incomplete - we need to add ephemeris etc.
 * It will eventually be updated by the *com* thread.
 *
 */

#ifndef __SVI_H
#define __SVI_H

#include "customDataTypes.h"
#include <pthread.h>
#define SVIMAXSIGPERBLK 3	// Max number of signals per block

// ----------------------------- Define Signal ----------------------------
struct chn_info_s;
// A pointer to this data structure is needed
enum svi_signame_e {
	SIG_NONE, SIG_GPSL1CA, SIG_GPSL2CM, SIG_GPSL2CL, SIG_GLOL1OF, SIG_GLOL2OF
};
enum svi_modname_e {
	MOD_NONE, MOD_BPSK, MOD_TMBPSK
};
enum svi_navname_e {
	NAV_NONE, NAV_GPSLEG, NAV_GPSCNAV, NAV_GLOLEG
};
struct svi_sig_s {
	enum svi_signame_e sigName;	// Signal name
	enum svi_modname_e modName;	// Modulation name
	enum svi_navname_e navName;	// Navigation format name
	fl64 codeRate;	// code rate
	ui32 codeLen;
	pi8 (*codeFun)(ui32, ui32);// a pointer to a function that takes 2 ui32 and returns a si8
	si32 (*acqFun)(struct chn_info_s *);// a pointer to a function that takes a pointer to a struct and returns a si32
	ui32 acqThreshold;		// Threshold to use for acquisition
	bool isPilot;		// pilot or data signal?
	bool tmPart;			// true: first subchip, false: second
	fl64 dataRate;		// data rate
};

// ------------------------- Define Constellation -------------------------
enum svi_cstname_e {
	CST_NONE, CST_GPS, CST_GLO
};
struct svi_cst_s {
	enum svi_cstname_e cstName;
};

// ----------------------------- Define Block -----------------------------
enum svi_blkname_e {
	BLK_NONE,
	BLK_GPSI,
	BLK_GPSII,
	BLK_GPSIIA,
	BLK_GPSIIR,
	BLK_GPSIIRM,
	BLK_GPSIIF,
	BLK_GLOL,
	BLK_GLOM,
	BLK_GLOMp,
	BLK_GLOK1
};
struct svi_blk_s {
	enum svi_blkname_e blkName;
	struct svi_sig_s *sigPtrs[SVIMAXSIGPERBLK];
};

// --------------------------- Define Satellite ---------------------------
struct svi_sat_s {
	si32 prn;
	si32 svn;
	struct svi_cst_s *cstPtr;	// pointer to constellation
	struct svi_blk_s *blkPtr;	// pointer to block
};

// --------------------------- Define Satellite ---------------------------
struct svi_s {
	pthread_mutex_t mutex;	// Mutex
	ui16 numSig;	// Number of signals
	ui16 numCst;	// Number of constellations
	ui16 numBlk;	// Number of blocks
	ui16 numSat;	// Number of satellites
	struct svi_sig_s * sig;	// Pointer to array of signals
	struct svi_cst_s * cst;	// Pointer to array of constellations
	struct svi_blk_s * blk;	// Pointer to array of blocks
	struct svi_sat_s * sat;	// Pointer to array of satellites
};

// ------------------------- Function prototypes --------------------------
int svi_init(struct svi_s *);		// Initialize SV info
void svi_free(struct svi_s *);		// Release dynamically allocated memory
#endif
