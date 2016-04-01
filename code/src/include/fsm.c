#include "fsm.h"
#include <stdlib.h>
#include <stdio.h>

si32 fsm_update(fsm_s * fsm) {
	// Perform desired action in this state
	fsm->S[fsm->state].fun(fsm->S[fsm->state].arg);

	// Switch to the next state 
	si32 k;
	bool t;
	for (k = 0; k < fsm->numT; k++) {			// Loop over all transitions
		if (fsm->state == fsm->T[k].src) {	// check if state==src
			t = (fsm->T[k].cnd & fsm->flags) == fsm->T[k].cnd;
			//t = fsm->T[k].fun(fsm->T[k].arg);	// Evaluate function
			if (t == true) {
				fsm->state = fsm->T[k].dst;
				return EXIT_SUCCESS;
			};
		};
	};
	// Add error message here
	return EXIT_FAILURE;
}

bool cnd_true(void * a) {
	(void) a;	/// Suppressing unused variable warning
	printf("True, ");
	return true;
}

bool cnd_chance(void * inp) {	// Actually (fl32 * inp)
	fl32 *val = (fl32 *) inp;
	si32 r = rand();	// between 0 and RAND_MAX
	fl32 f = ((fl32) r) / ((fl32) RAND_MAX);	// between 0.0 and 1.0
	printf("%4.2f (%4.2f), ", f, *val);
	return f <= *val;
}
