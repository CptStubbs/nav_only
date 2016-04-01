/*
 * findEleAzi.h
 *
 *  Created on: Oct 14, 2015
 *      Author: daehee
 */

#ifndef SRC_NAV_FINDELEAZI_H_
#define SRC_NAV_FINDELEAZI_H_

si32 findEleAzi(fl64 pos[4], fl64 * el, fl64 * az, fl64 ** satpos, si32 nmbOfSatellites, si32 * activeChnList);

#endif /* SRC_NAV_FINDELEAZI_H_ */
