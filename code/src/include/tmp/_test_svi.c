#include "svi.h"
#include <stdio.h>
#include <stdlib.h>

// The svi data structure is global, and protected using a mutex
struct svi_s svi;	
	
void printSig(struct svi_sig_s * S){
	switch (S->sigName) {
		case SIG_NONE:		printf("NONE       "); 	break;
		case SIG_GPSL1CA:	printf("GPS_L1_C/A "); 	break;
		case SIG_GPSL2CM:	printf("GPS_L2_CM  "); 	break;
		case SIG_GPSL2CL:	printf("GPS_L2_CM  "); 	break;
		case SIG_GLOL1OF:	printf("GLO_L1_OF  "); 	break;
		case SIG_GLOL2OF:	printf("GLO_L2_OF  "); 	break;
	};
	switch (S->modName) {
		case MOD_NONE:		printf("NONE   "); 		break;
		case MOD_BPSK:		printf("BPSK   "); 		break;
		case MOD_TMBPSK:	printf("TMBPSK "); 		break;
	};
	switch (S->navName) {
		case NAV_NONE:		printf("NONE     "); 	break;
		case NAV_GPSLEG:	printf("GPS_LEG  "); 	break;
		case NAV_GPSCNAV:	printf("GPS_CNAV "); 	break;
		case NAV_GLOLEG:	printf("GLO_LEG  "); 	break;
	};
	return;
};

void printBlk(struct svi_blk_s * S){
	switch (S->blkName) {
		case BLK_NONE:		printf("NONE     "); 	break;
		case BLK_GPSI:		printf("GPS_I    "); 	break;
		case BLK_GPSII:		printf("GPS_II   "); 	break;
		case BLK_GPSIIA:	printf("GPS_IIA  "); 	break;
		case BLK_GPSIIR:	printf("GPS_IIR  "); 	break;
		case BLK_GPSIIRM:	printf("GPS_IIRM "); 	break;
		case BLK_GPSIIF:	printf("GPS_IIF  "); 	break;
		case BLK_GLOL:		printf("GLO_L    "); 	break;
		case BLK_GLOM:		printf("GLO_M    "); 	break;
		case BLK_GLOMp:		printf("GLO_Mp   "); 	break;
		case BLK_GLOK1:		printf("GLO_K1   "); 	break;
	};
	return;
};

void printCst(struct svi_cst_s * S){
	switch (S->cstName) {
		case CST_NONE:		printf("NONE "); 		break;
		case CST_GPS:		printf("GPS  "); 		break;
		case CST_GLO:		printf("GLO  "); 		break;
	};
	return;
};

void printSat(struct svi_sat_s * S){
	printf("%3d %3d ",S->prn,S->svn);
	printCst(S->cstPtr);
	printBlk(S->blkPtr);
	si32 m;
	for (m=0;m<SVIMAXSIGPERBLK;m++){
		printf(" | ");
		printSig(S->blkPtr->sigPtrs[m]);
	}
	printf("\n");
};

si32 main(void){
	// Initialize svi data structure
	svi_init(&svi);
	// Print svi data structure
	printf("%3s %3s %3s %4s %9s > Signals\n","ind","prn","svn","cst","blk");
	
	pthread_mutex_lock(&svi.mutex);
	si32 m;
	for (m=0;m<svi.numSat;m++){
		printf("%2d: ",m);
		printSat(&svi.sat[m]);
	};
	pthread_mutex_unlock(&svi.mutex);
	// Free memory 
	svi_free(&svi);
	return EXIT_SUCCESS;
};