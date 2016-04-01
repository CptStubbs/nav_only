#ifndef __ACQUIRE_H
	#define __ACQUIRE_H

	#include "customDataTypes.h"
	#include "fsm.h"
	#include "svi.h"
	
	struct acquire_inp_s{
		si32	prn;
		fl64	fs;
		fl64	freqlo;
		fl64	freqhi;
	};
	
	struct acquire_out_s{
		fl64	peak;
		bool	found;
		fl64	codephase;
		fl64	codefreq;
		fl64	carrphase;
		fl64	carrfreq;
	};
	
	si32 acquire(struct svi_s *, struct acquire_inp_s *, struct acquire_out_s *);
	
#endif