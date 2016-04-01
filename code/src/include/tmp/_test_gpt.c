#include "gpt.h"
#include "svi.h"
#include <stdio.h>
#include <stdlib.h>

// Space Vehicle Information
struct svi_s svi;	
// Gain, phase and time data structure
struct gpt_s gpt;

void update_gpt(ui32 cnt){
	si32 n;

	ui32 ind = cnt % GPTBUFLEN1;	// address to write to
	pthread_mutex_lock(&gpt.mutex);

	gpt.arr[ind].cnt = cnt;			// update counter
	for (n=0;n<GPTBUFLEN0;n++){
		gpt.arr[ind].val[n].corr_real = cnt*100+n*10+1;
		gpt.arr[ind].val[n].corr_imag = cnt*100+n*10+2;
		gpt.arr[ind].val[n].cycle_int = cnt*100+n*10+3;
		gpt.arr[ind].val[n].cycle_frc = cnt*100+n*10+4;
		gpt.arr[ind].val[n].carr_freq = cnt*100+n*10+5;
		gpt.arr[ind].val[n].code_freq = cnt*100+n*10+6;
	};
	pthread_mutex_unlock(&gpt.mutex);
	return;
};

void print_gpt(void){
	si32 m,n;
	struct gpt_val_s *x;
	pthread_mutex_lock(&gpt.mutex);
	for (m=0;m<GPTBUFLEN1;m++){
		printf("%3d %3u %3u\n",m,gpt.arr[m].stream_id,gpt.arr[m].cnt);
		for (n=0;n<GPTBUFLEN0;n++){
			x = &gpt.arr[m].val[n]; 
			printf("%3u %3u %3u %3u %+6lld %+6lld\n",x->corr_real,x->corr_imag,x->cycle_int,x->cycle_frc,x->carr_freq,x->code_freq);
		};		
	};
	pthread_mutex_unlock(&gpt.mutex);
	printf("\n");
	return;
};

int main(void){
	// Initialize svi data structure
	svi_init(&svi);
	// add information to the gpt structure
	update_gpt(0);
	print_gpt();
	update_gpt(1);
	print_gpt();
	update_gpt(2);
	print_gpt();
	update_gpt(3);
	print_gpt();
	return EXIT_SUCCESS;
};