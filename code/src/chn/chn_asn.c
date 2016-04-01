#include "chn_asn.h"
#include <stdlib.h>
#include "conf_chn.h"

/// Find which prn to switch to
ui8 chn_asn_update(chn_asn_s * asn) {
	//ui64 available, tmp;
	//ui32 m;
	asn->lastCheck=asn->lastCheck+1; //increase lastCheck
	asn->lastCheck = asn->lastCheck % 32; // make sure it only goes between 0 and 31.
	while (asn->trackSats[asn->lastCheck] == 0) { //0 means tracking right now, 1 means needs to be tracked.
		//if it is 0 we need to check the next prn
		asn->lastCheck++; //increment lastCheck
		asn->lastCheck = asn->lastCheck % 32; // make sure it only goes between 0 and 31.
	}
	asn->trackSats[asn->lastCheck] = 0; //set tracker to 0.
	return 0;
}


