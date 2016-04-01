/*
 * loadLEO.c
 *
 *  Created on: Nov 11, 2015
 *      Author: erin
 */
#include <stdlib.h>
#include <stdio.h>

#include "customDataTypes.h"
#include "loadLEO.h"
#include "conf_sch.h"

void loadLEO(leoOrb_s * leoOrb){
	char fileX[] = "../../../largeFiles/test_sch/x.bin";
	char fileY[] = "../../../largeFiles/test_sch/y.bin";
	char fileZ[] = "../../../largeFiles/test_sch/z.bin";
	char fileVx[] = "../../../largeFiles/test_sch/vx.bin";
	char fileVy[] = "../../../largeFiles/test_sch/vy.bin";
	char fileVz[] = "../../../largeFiles/test_sch/vz.bin";
	char fileAx[] = "../../../largeFiles/test_sch/ax.bin";
	char fileAy[]	= "../../../largeFiles/test_sch/ay.bin";
	char fileAz[] = "../../../largeFiles/test_sch/az.bin";
	char fileBias[] = "../../../largeFiles/test_sch/bias.bin";
	char fileDrift[] = "../../../largeFiles/test_sch/drift.bin";

	getValues(fileX,leoOrb,1);
	getValues(fileY,leoOrb,2);
	getValues(fileZ,leoOrb,3);
	getValues(fileVx,leoOrb,4);
	getValues(fileVy,leoOrb,5);
	getValues(fileVz,leoOrb,6);
	getValues(fileAx,leoOrb,7);
	getValues(fileAy,leoOrb,8);
	getValues(fileAz,leoOrb,9);
	getValues(fileBias,leoOrb,10);
	getValues(fileDrift,leoOrb,11);

}

void getValues(char * filePath, leoOrb_s * leoOrb, int type){
	fl64 temp;
	FILE *leoState;
	int i = 0;

	leoState = fopen(filePath,"rb");
	if(!leoState){
		printf("Not able to open binary file.\n");
	}

	switch(type){
	case 1:
		while(1 == fread(&temp,sizeof(fl64),1,leoState)){
			leoOrb->x_m[i]=temp*KM2M;
			i++;
		}
		break;
	case 2:
		while(1 == fread(&temp,sizeof(fl64),1,leoState)){
			leoOrb->y_m[i]=temp*KM2M;
			i++;
		}
		break;
	case 3:
		while(1 == fread(&temp,sizeof(fl64),1,leoState)){
			leoOrb->z_m[i]=temp*KM2M;
			i++;
		}
		break;
	case 4:
		while(1 == fread(&temp,sizeof(fl64),1,leoState)){
			leoOrb->vx_m[i]=temp*KM2M;
			i++;
		}
		break;
	case 5:
		while(1 == fread(&temp,sizeof(fl64),1,leoState)){
			leoOrb->vy_m[i]=temp*KM2M;
			i++;
		}
		break;
	case 6:
		while(1 == fread(&temp,sizeof(fl64),1,leoState)){
			leoOrb->vz_m[i]=temp*KM2M;
			i++;
		}
		break;
	case 7:
		while(1 == fread(&temp,sizeof(fl64),1,leoState)){
			leoOrb->ax_m[i]=temp*KM2M;
			i++;
		}
		break;
	case 8:
		while(1 == fread(&temp,sizeof(fl64),1,leoState)){
			leoOrb->ay_m[i]=temp*KM2M;
			i++;
		}
		break;
	case 9:
		while(1 == fread(&temp,sizeof(fl64),1,leoState)){
			leoOrb->az_m[i]=temp*KM2M;
			i++;
		}
		break;
	case 10:
		while(1 == fread(&temp,sizeof(fl64),1,leoState)){
			leoOrb->clk_bias[i]=temp;
			i++;
		}
		break;
	case 11:
		while(1 == fread(&temp,sizeof(fl64),1,leoState)){
			leoOrb->clk_bias[i]=temp;
			i++;
		}
		break;
	default:
		printf("getValues: Error in reading LEO state values.\n");
	}
	fclose(leoState);
	return;
}
