#include "fsm.h"
#include <stdio.h>
#include <stdlib.h>

si32 dummy(void * inp) {
	ui32 * val = (ui32 *) inp;	// Cast the input to a ptr to a ui32
	printf("Dummy: %u, ",*val);
	return EXIT_SUCCESS;
};

int main() {
	ui32 k;
	ui32 dum = 0;		// pointer to a dummy value
	ui32 num = 42;	// pointer to an almost dummy value
	
	// A real example
	fsm_s fsm;											// Declare Finite State Machine
	fsm.state = 0;									// Current State	(Initialize to IDLE)
	fsm.numS = 3;										// Number of states
	fsm.numT = 5;										// Number of transitions

	struct fsms_s S[fsm.numS];			// Declare array of states
	k = 0;
	S[k].fun = dummy;		S[k].arg = &dum; 		k++;
	S[k].fun = dummy;		S[k].arg = &dum; 		k++;
	S[k].fun = dummy;		S[k].arg = &dum; 		k++;
	fsm.S = S;

	struct fsmt_s T[fsm.numT];			// Declare array of transitions
	fl32 p50 = 0.50;								// A constant that will come in handy
	fl32 p25 = 0.25;								// A constant that will come in handy
	k = 0;
	T[k].src = 0;	T[k].dst = 1;	T[k].fun = cnd_chance;	T[k].arg = &p50;	k++;
	T[k].src = 0;	T[k].dst = 0;	T[k].fun = cnd_true;		T[k].arg = &num;	k++;
	T[k].src = 1;	T[k].dst = 2;	T[k].fun = cnd_chance;	T[k].arg = &p25;	k++;
	T[k].src = 1;	T[k].dst = 0;	T[k].fun = cnd_true;		T[k].arg = &num;	k++;
	T[k].src = 2;	T[k].dst = 0;	T[k].fun = cnd_true;		T[k].arg = &num;	k++;
	fsm.T = T;

	printf("\n\n");
	for (k=0;k<50;k++){
		printf("state: %d, ",fsm.state);
		if (fsm_update(&fsm) == EXIT_FAILURE){
			return EXIT_FAILURE;
		};
		printf("\b\b\n");
	};
	return 0;
};
  