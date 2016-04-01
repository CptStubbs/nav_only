#ifndef __FSM_H
#define __FSM_H

#include "customDataTypes.h"

// States - each entry corresponds to a function and an argument
typedef struct fsms_s {
	si32 (*fun)(void *);
	void * arg;
} fsms_s;

// State transitions - each entry corresponds to a transition between states
typedef struct fsmt_s {
	ui32 src; 							// State source
	ui32 dst;							// State destination
	ui32 cnd;							// Condition
	bool (*fun)(void *); 	// Condition
	void *arg;  						// Argument to condition call
} fsmt_s;

// State machine - each entry corresponds to a finite state machine
typedef struct {
	ui32 state;					// Current state
	ui32 flags;
	si32 numS;						// Number of states
	si32 numT;						// Number of transitions
	struct fsms_s *S;			// Pointer to array of states, length numS
	struct fsmt_s *T;			// Pointer to array of transitions, length numT
} fsm_s;

// Functions
si32 fsm_update(fsm_s *);
bool cnd_true(void *);			// Always returns true
bool cnd_chance(void *);// Given a pointer to a fl32, returns true if rand<=fl32
#endif
