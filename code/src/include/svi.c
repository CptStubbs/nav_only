#include <stdlib.h>
#include "svi.h"		// Corresponding header file
#include "cde.h"		// Function pointers to code generations 
//#include "acq.h"		// Function pointers to acquisition 
#include <stdlib.h>	
#include <stdio.h>

/**
 * This file defines the required functions related to the global data structure
 * svi_s (space vehicle information_struct). Everything unique to satellites
 * should go here. The data structure will typically be updated by the *com*
 * thread (as instructed by the ground stations), and the ephemeris part (to be
 * added) will often be updated by the nav thread.
 */

// output from gcc_satInfo.m - This should be modified to use binary files 
si32 prnvec[59] = { 1, 2, 4, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 95, 100, 101, 102,
		103, 105, 106, 107, 109, 111, 116, 117, 118, 119, 120, 121, 122, 123,
		124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134 };
si32 svnvec[59] = { 63, 61, 34, 50, 67, 48, 68, 40, 46, 58, 43, 41, 55, 56, 53,
		54, 59, 51, 45, 47, 60, 65, 62, 26, 66, 44, 57, 64, 52, 23, 712, 714,
		715, 716, 717, 719, 720, 721, 723, 725, 730, 733, 734, 731, 732, 735,
		736, 737, 738, 701, 742, 743, 744, 745, 746, 747, 754, 755, 702 };
si32 blkvec[59] = { BLK_GPSIIF, BLK_GPSIIR, BLK_GPSIIA, BLK_GPSIIRM, BLK_GPSIIF,
		BLK_GPSIIRM, BLK_GPSIIF, BLK_GPSIIA, BLK_GPSIIR, BLK_GPSIIRM,
		BLK_GPSIIR, BLK_GPSIIR, BLK_GPSIIRM, BLK_GPSIIR, BLK_GPSIIRM,
		BLK_GPSIIR, BLK_GPSIIR, BLK_GPSIIR, BLK_GPSIIR, BLK_GPSIIR, BLK_GPSIIR,
		BLK_GPSIIF, BLK_GPSIIF, BLK_GPSIIA, BLK_GPSIIF, BLK_GPSIIR, BLK_GPSIIRM,
		BLK_GPSIIF, BLK_GPSIIRM, BLK_GPSIIA, BLK_GLOM, BLK_GLOM, BLK_GLOM,
		BLK_GLOM, BLK_GLOM, BLK_GLOM, BLK_GLOM, BLK_GLOM, BLK_GLOM, BLK_GLOM,
		BLK_GLOM, BLK_GLOM, BLK_GLOM, BLK_GLOM, BLK_GLOM, BLK_GLOM, BLK_GLOM,
		BLK_GLOM, BLK_GLOM, BLK_GLOK1, BLK_GLOM, BLK_GLOM, BLK_GLOM, BLK_GLOM,
		BLK_GLOM, BLK_GLOM, BLK_GLOM, BLK_GLOMp, BLK_GLOK1 };
si32 cstvec[59] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };

// Initialize signals
int svi_initSig(struct svi_s *S) {
	pthread_mutex_lock(&S->mutex);
	// Allocate memory
	S->numSig = 6;		// Number of signals (including SIG_NONE)
	S->sig = malloc(S->numSig * sizeof(struct svi_sig_s));
	if (S->sig == NULL) {
		printf("svi_initSig failed. Malloc returned NULL\n");
		pthread_mutex_unlock(&S->mutex);
		return EXIT_FAILURE;
	};
	si32 k = 0;
	S->sig[k].sigName = SIG_NONE;
	S->sig[k].modName = MOD_NONE;
	S->sig[k].navName = NAV_NONE;
	S->sig[k].codeRate = 0.0;
	S->sig[k].codeLen = 0;
	S->sig[k].codeFun = &cde_NONE;
	//S->sig[k].acqFun 				= &acq_NONE;
	S->sig[k].acqThreshold = 1 << 31;
	S->sig[k].isPilot = false;
	S->sig[k].tmPart = false;		// does not matter
	S->sig[k].dataRate = 0.0;
	// GPS L1 C/A
	k = k + 1;
	S->sig[k].sigName = SIG_GPSL1CA;
	S->sig[k].modName = MOD_BPSK;
	S->sig[k].navName = NAV_GPSLEG;
	S->sig[k].codeRate = 1.023e6;
	S->sig[k].codeLen = 1023;
	S->sig[k].codeFun = &cde_GPSL1CA;
	//S->sig[k].acqFun 	= &acq_GPSL1CA;
	S->sig[k].acqThreshold = 1500000;	// unsure what is a good number here
	S->sig[k].isPilot = false;
	S->sig[k].tmPart = true;		// does not matter
	S->sig[k].dataRate = 50.0;
	// GPS L2CM
	k = k + 1;
	S->sig[k].sigName = SIG_GPSL2CM;
	S->sig[k].modName = MOD_TMBPSK;
	S->sig[k].navName = NAV_GPSCNAV;
	S->sig[k].codeRate = 1.023e6;
	S->sig[k].codeLen = 10230;
	S->sig[k].codeFun = &cde_NONE;
	//S->sig[k].acqFun 	= &acq_NONE;
	S->sig[k].acqThreshold = 1 << 31;
	S->sig[k].isPilot = false;
	S->sig[k].tmPart = true;		// not sure
	S->sig[k].dataRate = 50.0;
	// GPS L2CL
	k = k + 1;
	S->sig[k].sigName = SIG_GPSL2CL;
	S->sig[k].modName = MOD_TMBPSK;
	S->sig[k].navName = NAV_NONE;
	S->sig[k].codeRate = 1.023e6;
	S->sig[k].codeLen = 767250;
	S->sig[k].codeFun = &cde_NONE;
	//S->sig[k].acqFun 	= &acq_NONE;
	S->sig[k].acqThreshold = 1 << 31;
	S->sig[k].isPilot = true;
	S->sig[k].tmPart = false;	// not sure
	S->sig[k].dataRate = 0.0;
	// GLO L1 OF
	k = k + 1;
	S->sig[k].sigName = SIG_GLOL1OF;
	S->sig[k].modName = MOD_BPSK;
	S->sig[k].navName = NAV_GLOLEG;
	S->sig[k].codeRate = 0.511e6;
	S->sig[k].codeLen = 511;
	S->sig[k].codeFun = &cde_NONE;
	//S->sig[k].acqFun 	= &acq_NONE;
	S->sig[k].acqThreshold = 1 << 31;
	S->sig[k].isPilot = false;
	S->sig[k].tmPart = true;
	S->sig[k].dataRate = 50.0;
	// GLO L2 OF
	k = k + 1;
	S->sig[k].sigName = SIG_GLOL2OF;
	S->sig[k].modName = MOD_BPSK;
	S->sig[k].navName = NAV_GLOLEG;
	S->sig[k].codeRate = 0.511e6;
	S->sig[k].codeLen = 511;
	S->sig[k].codeFun = &cde_NONE;
	//S->sig[k].acqFun 	= &acq_NONE;
	S->sig[k].acqThreshold = 1 << 31;
	S->sig[k].isPilot = false;
	S->sig[k].tmPart = true;
	S->sig[k].dataRate = 50.0;
	pthread_mutex_unlock(&S->mutex);
	return EXIT_SUCCESS;
}

// Initialize constellations
int svi_initCst(struct svi_s * S) {
	pthread_mutex_lock(&S->mutex);
	S->numCst = 3;	// Number of constellations (including CST_NONE)
	S->cst = malloc(S->numCst * sizeof(struct svi_cst_s));
	if (S->cst == NULL) {
		printf("svi_initCst failed. Malloc returned NULL\n");
		pthread_mutex_unlock(&S->mutex);
		return EXIT_FAILURE;
	};
	S->cst[0].cstName = CST_NONE;
	S->cst[1].cstName = CST_GPS;
	S->cst[2].cstName = CST_GLO;
	pthread_mutex_unlock(&S->mutex);
	return EXIT_SUCCESS;
}

// Initialize blocks
int svi_initBlk(struct svi_s *S) {
	pthread_mutex_lock(&S->mutex);
	S->numBlk = 11;	// Number of satellite blocks (include BLK_NONE)
	S->blk = malloc(S->numBlk * sizeof(struct svi_blk_s));
	if (S->blk == NULL) {
		printf("svi_initBlk failed. Malloc returned NULL\n");
		pthread_mutex_unlock(&S->mutex);
		return EXIT_FAILURE;
	};
	si32 k = 0;
	S->blk[k].blkName = BLK_NONE;
	S->blk[k].sigPtrs[0] = &S->sig[SIG_NONE];
	S->blk[k].sigPtrs[1] = &S->sig[SIG_NONE];
	S->blk[k].sigPtrs[2] = &S->sig[SIG_NONE];
	// GPS block I
	k++;
	S->blk[k].blkName = BLK_GPSI;
	S->blk[k].sigPtrs[0] = &S->sig[SIG_GPSL1CA];
	S->blk[k].sigPtrs[1] = &S->sig[SIG_NONE];
	S->blk[k].sigPtrs[2] = &S->sig[SIG_NONE];
	// GPS block II
	k++;
	S->blk[k].blkName = BLK_GPSII;
	S->blk[k].sigPtrs[0] = &S->sig[SIG_GPSL1CA];
	S->blk[k].sigPtrs[1] = &S->sig[SIG_NONE];
	S->blk[k].sigPtrs[2] = &S->sig[SIG_NONE];
	// GPS block IIA
	k++;
	S->blk[k].blkName = BLK_GPSIIA;
	S->blk[k].sigPtrs[0] = &S->sig[SIG_GPSL1CA];
	S->blk[k].sigPtrs[1] = &S->sig[SIG_NONE];
	S->blk[k].sigPtrs[2] = &S->sig[SIG_NONE];
	// GPS block IIR
	k++;
	S->blk[k].blkName = BLK_GPSIIR;
	S->blk[k].sigPtrs[0] = &S->sig[SIG_GPSL1CA];
	S->blk[k].sigPtrs[1] = &S->sig[SIG_NONE];
	S->blk[k].sigPtrs[2] = &S->sig[SIG_NONE];
	// GPS block IIR-M	- first with L2C
	k++;
	S->blk[k].blkName = BLK_GPSIIRM;
	S->blk[k].sigPtrs[0] = &S->sig[SIG_GPSL1CA];
	S->blk[k].sigPtrs[1] = &S->sig[SIG_GPSL2CM];
	S->blk[k].sigPtrs[2] = &S->sig[SIG_GPSL2CL];
	// GPS block IIF
	k++;
	S->blk[k].blkName = BLK_GPSIIF;
	S->blk[k].sigPtrs[0] = &S->sig[SIG_GPSL1CA];
	S->blk[k].sigPtrs[1] = &S->sig[SIG_GPSL2CM];
	S->blk[k].sigPtrs[2] = &S->sig[SIG_GPSL2CL];
	// GLO legacy
	k++;
	S->blk[k].blkName = BLK_GLOL;
	S->blk[k].sigPtrs[0] = &S->sig[SIG_GLOL1OF];
	S->blk[k].sigPtrs[1] = &S->sig[SIG_GLOL2OF];

	S->blk[k].sigPtrs[2] = &S->sig[SIG_NONE];
	// GLO M
	k++;
	S->blk[k].blkName = BLK_GLOM;
	S->blk[k].sigPtrs[0] = &S->sig[SIG_GLOL1OF];
	S->blk[k].sigPtrs[1] = &S->sig[SIG_GLOL2OF];
	S->blk[k].sigPtrs[2] = &S->sig[SIG_NONE];
	// GLO M+
	k++;
	S->blk[k].blkName = BLK_GLOMp;
	S->blk[k].sigPtrs[0] = &S->sig[SIG_GLOL1OF];
	S->blk[k].sigPtrs[1] = &S->sig[SIG_GLOL2OF];
	S->blk[k].sigPtrs[2] = &S->sig[SIG_NONE];
	// GLO K1
	k++;
	S->blk[k].blkName = BLK_GLOK1;
	S->blk[k].sigPtrs[0] = &S->sig[SIG_GLOL1OF];
	S->blk[k].sigPtrs[1] = &S->sig[SIG_GLOL2OF];
	S->blk[k].sigPtrs[2] = &S->sig[SIG_NONE];
	pthread_mutex_unlock(&S->mutex);
	return EXIT_SUCCESS;
}

// Initialize satellites
int svi_initSat(struct svi_s * S) {
	pthread_mutex_lock(&S->mutex);
	S->numSat = 59;	// Number of satellites (including)
	si32 m = 0;
	// Allocate memory
	S->sat = malloc(S->numSat * sizeof(struct svi_sat_s));
	if (S->sat == NULL) {
		printf("svi_initSat failed. Malloc returned NULL\n");
		pthread_mutex_unlock(&S->mutex);
		return EXIT_FAILURE;
	};
	// Update SVN information
	for (m = 0; m < S->numSat; m++) {
		// PRN
		S->sat[m].prn = prnvec[m];
		// SVN
		S->sat[m].svn = svnvec[m];
		// CST
		S->sat[m].cstPtr = &S->cst[cstvec[m]];
		// Block
		S->sat[m].blkPtr = &S->blk[blkvec[m]];
	};
	pthread_mutex_unlock(&S->mutex);
	return EXIT_SUCCESS;
}

// Initialize svi
int svi_init(struct svi_s * S) {
	// Initialize Signals
	if (svi_initSig(S) != EXIT_SUCCESS) {
		printf("svi_init failed.\n");
		return EXIT_FAILURE;
	};
	// Initialize Constellations
	if (svi_initCst(S) != EXIT_SUCCESS) {
		printf("svi_init failed.\n");
		return EXIT_FAILURE;
	};
	// Initialize Blocks
	if (svi_initBlk(S) != EXIT_SUCCESS) {
		printf("svi_init failed.\n");
		return EXIT_FAILURE;
	};
	// Initialize Satellites
	if (svi_initSat(S) != EXIT_SUCCESS) {
		printf("svi_init failed.\n");
		return EXIT_FAILURE;
	};
	return EXIT_SUCCESS;
}

// Release dynamically allocated memory 
void svi_free(struct svi_s * S) {
	free(S->sig);
	free(S->cst);
	free(S->blk);
	free(S->sat);
}

//int main(){
//	return EXIT_SUCCESS;
//}

