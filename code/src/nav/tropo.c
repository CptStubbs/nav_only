/*
 * tropo.c
 *
 *  Created on: May 21, 2015
 *      Author: sean
 */

#include "customDataTypes.h"
#include "tropo.h"
#include <math.h>
#include <string.h>

void tropo(fl64 * ddr, fl64 sinel, fl64 hsta, fl64 p, fl64 tkel, fl64 hum,
		fl64 hp, fl64 htkel, fl64 hhum) {

	/* semi-major axis of earth ellipsoid */
	fl64 a_e = 6378.137;
	fl64 b0 = 7.839257e-5;
	fl64 tlapse = -6.5;
	fl64 tkhum = tkel + tlapse * (hhum - htkel);
	fl64 atkel = 7.5 * (tkhum - 273.15) / (237.3 + tkhum - 273.15);
	fl64 e0 = 0.0611 * hum * pow(10, atkel);
	fl64 tksea = tkel - tlapse * htkel;
	fl64 em = -978.77 / (2.8704e6 * tlapse * 1.0e-5);
	fl64 tkelh = tksea + tlapse * hhum;
	fl64 e0sea = e0 * pow((tksea / tkelh), (4 * em));
	fl64 tkelp = tksea + tlapse * hp;
	fl64 psea = p * pow((tksea / tkelp), em);
	pi8 * done;
	si32 i = 0;
	fl64 refsea = 77.624e-6 / tksea;
	fl64 htop = 1.1385e-5 / refsea;
	fl64 ref = refsea * pow(((htop - hsta) / htop), 4);
	fl64 tropo = 0;
	fl64 rtop = 0;
	fl64 a = 0;
	fl64 b = 0;
	fl64 rn[8];
	fl64 alpha[8];
	fl64 dr = 0;
	if (sinel < 0) {
		sinel = 0;
	}
	refsea = refsea * psea;

	done = "FALSE";

	while (1) {
		rtop = pow((a_e + htop), 2)
				- pow((a_e + hsta), 2) * (1 - pow(sinel, 2));

		/*check to see if geometry is crazy */
		if (rtop < 0) {
			rtop = 0;
		}

		rtop = sqrt(rtop) - (a_e + hsta) * sinel;
		a = -sinel / (htop - hsta);
		b = -b0 * (1 - pow(sinel, 2)) / (htop - hsta);

		for (i = 0; i < 8; i++) {
			rn[i] = pow(rtop, (i + 1));
		}

		alpha[0] = 2 * a;
		alpha[1] = 2 * pow(a, 2) + 4 * b / 3;
		alpha[2] = a * (pow(a, 2) + 3 * b);
		alpha[3] = pow(a, 4) / 5 + 2.4 * pow(a, 2) * b + 1.2 * pow(b, 2);
		alpha[4] = 2 * a * b * (pow(a, 2) + 3 * b) / 3;
		alpha[5] = pow(b, 2) * (6 * pow(a, 2) + 4 * b) * 1.428571e-1;
		alpha[6] = 0;
		alpha[7] = 0;

		if (pow(b, 2) > 1.0e-35) {
			alpha[6] = a * pow(b, 3) / 2;
			alpha[7] = pow(b, 4) / 9;
		}

		dr = rtop;
		for (i = 0; i < 8; i++) {
			dr += alpha[i] * rn[i];
		}
		tropo = tropo + dr * ref * 1000;

		if (strcmp(done, "TRUE") == 0) {
			*ddr = tropo;
			break;
		}

		done = "TRUE";
		refsea = (371900.0e-6 / tksea - 12.92e-6) / tksea;
		htop = 1.1385e-5 * (1255 / tksea + 0.05) / refsea;
		ref = refsea * e0sea * pow(((htop - hsta) / htop), 4);

	}
}
