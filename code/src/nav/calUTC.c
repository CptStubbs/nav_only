/*
 * calUTC.c
 *
 *  Created on: May 18, 2015
 *      Author: sean
 */

#include "customDataTypes.h"
#include "calUTC.h"

#include "initEph.h"
#include <math.h>

void calUTC(fl64 * utcTime, almanac_s almanac, fl64 gpsTime, fl64 WN) {

	fl64 delta_utcTime = 0;
	fl64 tempUtcTime = 0;
	fl64 W = 0;
	fl64 tLSF = almanac.WNlsf * 604800 + (almanac.DN * 86400);
	fl64 tGPS = almanac.WNt * 604800 + gpsTime;
	fl64 dnGPS = gpsTime / 86400;
	fl64 dtimeSpan[2];

	dtimeSpan[0] = fmod(almanac.DN + 3.0 / 4.0, 7);
	dtimeSpan[1] = fmod(almanac.DN + 5.0 / 4.0, 7);

	if (tGPS < tLSF) {
		delta_utcTime =
				almanac.deltaTls + almanac.A0
						+ almanac.A1
								* (gpsTime - almanac.t_ot
										+ 604800 * (WN - almanac.WNt));
		if (dnGPS > dtimeSpan[1] || dnGPS < dtimeSpan[0]) {
			tempUtcTime = fmod(gpsTime - delta_utcTime, 86400);
		} else {
			W = fmod((gpsTime - delta_utcTime - 43200), 86400) + 43200;
			tempUtcTime = fmod(W,
					(86400 + 86400 + almanac.deltaTlsf - almanac.deltaTls));
		}
	} else {
		delta_utcTime =
				almanac.deltaTlsf + almanac.A0
						+ almanac.A1
								* (gpsTime - almanac.t_ot
										+ 604800 * (WN - almanac.WNt));
		tempUtcTime = fmod(gpsTime - delta_utcTime, 86400);

	}

	*utcTime = tempUtcTime;
}

