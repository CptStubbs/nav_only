/**
 * This file provides random number functionality (including randn) for fl64 and fl32
 *
 */
#ifndef __INCLUDE_RANDOM_H
	#define __INCLUDE_RANDOM_H

	#include "customDataTypes.h"

	/**
	 *	\brief Seed mechanism - use rand_init(time(NULL)) for random, any ui32 constant for
	 *	deterministic results
	 *	\param [in]				seed								seed value
	 */
	void rand_init(ui32 seed);

	/**
	 *	\brief Random values from a uniform [0,1] distribution - fl64
	 *	\return [out]													Random value
	 */
	fl64 rand_fl64();

	/**
	 *	\brief Random values from a uniform [0,1] distribution - fl32
	 *	\return [out]													Random value
	 */
	fl32 rand_fl32();

	/**
	 *	\brief Random values from a normal distribution - fl64
	 *	\return [out]													Random value
	 */
	fl64 randn_fl64();


	/**
	 *	\brief Random values from a normal distribution - fl32
	 *	\return [out]													Random value
	 */
	fl32 randn_fl32();

#endif
