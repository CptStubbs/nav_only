/*
 * main.c
 *
 *  Created on: May 7, 2015
 *      Author: sean
 */
#include <stdio.h>
#include <stdlib.h>

#include "initEphSch.h"
#include "schMain.h"
#include "readRINEX.h"
#include "loadLEO.h"

int main() {
	ephSch_s eph;
	leoOrb_s leoOrb;
//	gaut_s gautTemp;
	fl64 currTime = 352800;

	initEph(&eph);

	readRINEX(&eph,currTime);
	loadLEO(&leoOrb);

	return EXIT_SUCCESS;
}
