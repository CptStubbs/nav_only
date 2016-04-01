/**
 * @file 			res.c
 * @author 		staffan
 * @date 			09-Nov-2015
 **/

#include <stdlib.h>																		/// Return values
#include "conf.h"																			/// NUMBLK and NUMCPB
#include "res.h"																			/// Corresponding header

int res_init(res_s * res) {
	int m, n;									/// Block and channel counter
	for (m = 0; m < CONF_NUMBLK; m++) {					/// Loop over all blocks
		for (n = 0; n < CONF_NUMCPB; n++) {	/// Loop over all channels in a block
			res->blks[m].chns[n].EI = 0;					/// Clear all values
			res->blks[m].chns[n].EQ = 0;
			res->blks[m].chns[n].PI = 0;
			res->blks[m].chns[n].PQ = 0;
			res->blks[m].chns[n].LI = 0;
			res->blks[m].chns[n].LQ = 0;
			res->blks[m].chns[n].newCorr = false;/// TODO - newCorr should be moved to thr
#ifdef USEL2C
			res->blks[m].chns[n].L2EI = 0;			/// Clear all values
			res->blks[m].chns[n].L2EQ = 0;
			res->blks[m].chns[n].L2PI = 0;
			res->blks[m].chns[n].L2PQ = 0;
			res->blks[m].chns[n].L2LI = 0;
			res->blks[m].chns[n].L2LQ = 0;
#endif
		} /// for (n= ...
	} /// for (m= ...
	return EXIT_SUCCESS;								/// Always successful
}

int res_free(res_s * res) {
	(void) res;							/// Cast to suppress compiler warning
	return EXIT_SUCCESS;								/// Always successful
}
