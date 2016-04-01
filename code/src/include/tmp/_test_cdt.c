#include <stdio.h>
#include <stdlib.h>
#include "customDataTypes.h"

int main(void){
	printf("---------------- Print Type Sizes ---------------\n");
	printf("fl64: %lu\n",8*sizeof(fl64));
	printf("fl32: %lu\n",8*sizeof(fl32));
	printf("si64: %lu\n",8*sizeof(si64));
	printf("ui64: %lu\n",8*sizeof(ui64));
	printf("si32: %lu\n",8*sizeof(si32));
	printf("ui32: %lu\n",8*sizeof(ui32));
	printf("si16: %lu\n",8*sizeof(si16));
	printf("ui16: %lu\n",8*sizeof(ui16));
	printf("si8:  %lu\n",8*sizeof(pi8));
	printf("ui8:  %lu\n",8*sizeof(ui8));
	printf("\n\n");
	return EXIT_SUCCESS;
};