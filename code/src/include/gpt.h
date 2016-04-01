#ifndef __GPT_H
#define __GPT_H

#include <pthread.h>
#include "customDataTypes.h"
#define GPTBUFLEN0 2	// 8 values per dump
#define GPTBUFLEN1 2	// 4 long array

// Lowest level of gpt data
struct gpt_val_s {
	ui32 corr_real;	// Prompt inphase
	ui32 corr_imag;	// Prompt quadrature
	ui32 cycle_int;	// Sample Start of code, integer part
	ui32 cycle_frc; 	// Sample Start of code, fractional part
	si64 carr_freq;	// Carrier frequency
	si64 code_freq;	// Code frequency
};

// Middle level of gpt data
struct gpt_arr_s {
	struct svi_sig_s * sig;	// Pointer to signal
	ui32 stream_id;
	ui32 cnt;	// Counter that increases every time gpt is updated
	struct gpt_val_s val[GPTBUFLEN0];
};

// Top level of gpt data
struct gpt_s {
	pthread_mutex_t mutex;
	struct gpt_arr_s arr[GPTBUFLEN1];
};

#endif
