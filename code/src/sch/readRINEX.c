/*
 * readRINEX.c
 *
 *  Created on: Nov 4, 2015
 *      Author: erin
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "readRINEX.h"
#include "conf_sch.h"
#include "customDataTypes.h"
#include "initEphSch.h"


void readRINEX(ephSch_s * eph, fl64 t){
	FILE * fid;
	char buffer[90],gnss[3];
	char * prn;
	int year,month,day,hour,min,sec,satID;
	double a_f0,a_f1,a_f2;

	if((fid = fopen("../../../largeFiles/test_sch/brdm0010.15p","r"))==NULL){
		printf("readRINEX: Error in opening ephemeris file\n");
		return;
	}

	for(int i = 1; i<=5; i++){
		if(fgets(buffer,sizeof(buffer),fid)==NULL){
			printf("readRINEX: Error reading ephemeris file\n");
			return;
		}
	}

	getTimeTransfer(fid,eph);

	while((fscanf(fid,"%s %d %d %d %d %d %d %lf %lf %lf",gnss,&year,&month,&day,
				&hour,&min,&sec,&a_f0,&a_f1,&a_f2))!= EOF){

		prn = gnss+1;
		satID = atoi(prn);

		switch(gnss[0]){
			case 'G':
				parseGPSEntry(fid,eph,t,satID,a_f0,a_f1,a_f2);
				break;
			case 'R':
				parseGLOEntry(fid,eph,t,satID,a_f0,a_f1,a_f2);
				break;
			case 'E':
				parseGALEntry(fid,eph,t,satID,a_f0,a_f1,a_f2);
				break;
			case 'C':
				parseBDSEntry(fid,eph,t,satID,a_f0,a_f1,a_f2);
				break;
			case 'J':
				parseQZSEntry(fid,eph,t,satID,a_f0,a_f1,a_f2);
				break;
			case 'S':
				parseSBAEntry(fid,eph,t,satID,a_f0,a_f1,a_f2);
				break;
			default:
				printf("readRINEX: Error in ephemeris entry.\n");
		}
	}

	fclose(fid);
}

void parseGPSEntry(FILE * fileID,ephSch_s * eph, fl64 t, int satID, double a_f0,
		double a_f1, double a_f2){

	int validEph;
	float IODE,t_oe,TOW,codes,weekno,L2flag,svaccur,svhealth,IODC,fitInt;
	double C_rs,deltan,M_0,C_uc,e,C_us,sqrtA,C_ic,omega_0,C_is,i_0,C_rc,
		omega,omegaDot,iDot,T_GD;
	navGPS_s navGPSTemp;

	if((fscanf(fileID,"%f %lf %lf %lf",&IODE,&C_rs,&deltan,&M_0))!=4){
		printf("parseGPSEntry: Error reading 2nd line of entry\n");
		return;
	}
	if((fscanf(fileID,"%lf %lf %lf %lf",&C_uc,&e,&C_us,&sqrtA))!=4){
		printf("parseGPSEntry: Error reading 3rd line of entry\n");
		return;
	}
	if((fscanf(fileID,"%f %lf %lf %lf",&t_oe,&C_ic,&omega_0,&C_is))!=4){
		printf("parseGPSEntry: Error reading 4th line of entry\n");
		return;
	}
	if((fscanf(fileID,"%lf %lf %lf %lf",&i_0,&C_rc,&omega,&omegaDot))!=4){
		printf("parseGPSEntry: Error reading 5th line of entry\n");
		return;
	}
	if((fscanf(fileID,"%lf %f %f %f",&iDot,&codes,&weekno,&L2flag))!=4){
		printf("parseGPSEntry: Error reading 6th line of entry\n");
		return;
	}
	if((fscanf(fileID,"%f %f %lf %f",&svaccur,&svhealth,&T_GD,&IODC))!=4){
		printf("parseGPSEntry: Error reading 7th line of entry\n");
		return;
	}
	if((fscanf(fileID,"%f %f",&TOW,&fitInt))!=2){
		printf("parseGPSEntry: Error reading 8th line of entry\n");
		return;
	};

	validEph = checkEphEntry(t,t_oe,'G');
	if(validEph == 1){
		navGPSTemp.a_f0 = (fl64)a_f0;
		navGPSTemp.a_f1 = (fl64)a_f1;
		navGPSTemp.a_f2 = (fl64)a_f2;
		navGPSTemp.IODE = (ui32)IODE;
		navGPSTemp.C_rs = (fl64)C_rs;
		navGPSTemp.deltan = (fl64)deltan;
		navGPSTemp.M_0 = (fl64)M_0;
		navGPSTemp.C_uc = (fl64)C_uc;
		navGPSTemp.e = (fl64)e;
		navGPSTemp.C_us = (fl64)C_us;
		navGPSTemp.sqrtA = (fl64)sqrtA;
		navGPSTemp.t_oe = (fl32)t_oe;
		navGPSTemp.C_ic = (fl64)C_ic;
		navGPSTemp.omega_0 = (fl64)omega_0;
		navGPSTemp.C_is = (fl64)C_is;
		navGPSTemp.i_0 = (fl64)i_0;
		navGPSTemp.C_rc = (fl64)C_rc;
		navGPSTemp.omega = (fl64)omega;
		navGPSTemp.omegaDot = (fl64)omegaDot;
		navGPSTemp.iDot = (fl64)iDot;
		navGPSTemp.codes = (ui32)codes;
		navGPSTemp.weekno = (ui32)weekno;
		navGPSTemp.L2flag = (ui32)L2flag;
		navGPSTemp.svaccur = (ui32)svaccur;
		navGPSTemp.svhealth = (ui32)svhealth;
		navGPSTemp.T_GD = (fl64)T_GD;
		navGPSTemp.IODC = (ui32)IODC;
		navGPSTemp.TOW = (fl32)TOW;
		navGPSTemp.fitInt = (ui32)fitInt;
		eph->navGPS[satID] = navGPSTemp;
	}
}

void parseGLOEntry(FILE * fileID, ephSch_s * eph, fl64 t, int satID, double tau_n,
		double gam, double TOD){

	int validEph;
	float svhealth,freqNum,AOOI;
	double xDis,xVel,xAcc,yDis,yVel,yAcc,zDis,zVel,zAcc;
	navGLO_s navGLOTemp;

	if((fscanf(fileID,"%lf %lf %lf %f",&xDis,&xVel,&xAcc,&svhealth))!=4){
		printf("parseGLOEntry: Error reading 2nd line of entry\n");
		return;
	}
	if((fscanf(fileID,"%lf %lf %lf %f",&yDis,&yVel,&yAcc,&freqNum))!=4){
		printf("parseGLOEntry: Error reading 3rd line of entry\n");
		return;
	}
	if((fscanf(fileID,"%lf %lf %lf %f",&zDis,&zVel,&zAcc,&AOOI))!=4){
		printf("parseGLOEntry: Error reading 4th line of entry\n");
		return;
	}

	validEph = checkEphEntry(t,TOD,'R');
	if(validEph == 1){
		navGLOTemp.tau_n = (fl64)tau_n;
		navGLOTemp.gam = (fl64)gam;
		navGLOTemp.TOD = (fl32)TOD;
		navGLOTemp.xDis = (fl64)xDis;
		navGLOTemp.xVel = (fl64)xVel;
		navGLOTemp.xAcc = (fl64)xAcc;
		navGLOTemp.svhealth = (ui32)svhealth;
		navGLOTemp.yDis = (fl64)yDis;
		navGLOTemp.yVel = (fl64)yVel;
		navGLOTemp.yAcc = (fl64)yAcc;
		navGLOTemp.freqNum = (ui32)freqNum;
		navGLOTemp.zDis = (fl64)zDis;
		navGLOTemp.zVel = (fl64)zVel;
		navGLOTemp.zAcc = (fl64)zAcc;
		navGLOTemp.AOOI = (ui32)AOOI;
		navGLOTemp.tau_c = (fl64)eph->glut.tauC;
		navGLOTemp.tb = (fl32)TOD+900.0;	//TODO: Temporary fix of tb ephemeris definition
		eph->navGLO[satID] = navGLOTemp;
	}
}

void parseGALEntry(FILE * fileID, ephSch_s * eph, fl64 t, int satID, double a_f0,
		double a_f1, double a_f2){
	int validEph;
	float IODE,t_oe,dataSource,weekno,SISA,svhealth,TOW;
	double C_rs,deltan,M_0,C_uc,e,C_us,sqrtA,C_ic,omega_0,C_is,i_0,
		C_rc,omega,omegaDot,iDot,BGD_E5A,BGD_E5B;
	navGAL_s navGALTemp;

	if((fscanf(fileID,"%f %lf %lf %lf",&IODE,&C_rs,&deltan,&M_0))!=4){
		printf("parseGALEntry: Error reading 2nd line of entry\n");
		return;
	}
	if((fscanf(fileID,"%lf %lf %lf %lf",&C_uc,&e,&C_us,&sqrtA))!=4){
		printf("parseGALEntry: Error reading 3rd line of entry\n");
		return;
	}
	if((fscanf(fileID,"%f %lf %lf %lf",&t_oe,&C_ic,&omega_0,&C_is))!=4){
		printf("parseGALEntry: Error reading 4th line of entry\n");
		return;
	}
	if((fscanf(fileID,"%lf %lf %lf %lf",&i_0,&C_rc,&omega,&omegaDot))!=4){
		printf("parseGALEntry: Error reading 5th line of entry\n");
		return;
	}
	if((fscanf(fileID,"%lf %f %f",&iDot,&dataSource,&weekno))!=3){
		printf("parseGALEntry: Error reading 6th line of entry\n");
		return;
	}
	if((fscanf(fileID,"%f %f %lf %lf",&SISA,&svhealth,&BGD_E5A,&BGD_E5B))!=4){
		printf("parseGALEntry: Error reading 7th line of entry\n");
		return;
	}
	if((fscanf(fileID,"%f",&TOW))!=1){
		printf("parseGALEntry: Error reading 8th line of entry\n");
		return;
	}

	validEph = checkEphEntry(t,t_oe,'E');
		if(validEph == 1){
			navGALTemp.a_f0 = (fl64)a_f0;
			navGALTemp.a_f1 = (fl64)a_f1;
			navGALTemp.a_f2 = (fl64)a_f2;
			navGALTemp.IODE = (ui32)IODE;
			navGALTemp.C_rs = (fl64)C_rs;
			navGALTemp.deltan = (fl64)deltan;
			navGALTemp.M_0 = (fl64)M_0;
			navGALTemp.C_uc = (fl64)C_uc;
			navGALTemp.sqrtA = (fl64)sqrtA;
			navGALTemp.t_oe	= (fl32)t_oe;
			navGALTemp.C_ic = (fl64)C_ic;
			navGALTemp.omega_0 = (fl64)omega_0;
			navGALTemp.C_is = (fl64)C_is;
			navGALTemp.i_0 = (fl64)i_0;
			navGALTemp.C_rc = (fl64)C_rc;
			navGALTemp.omega = (fl64)omega;
			navGALTemp.omegaDot = (fl64)omegaDot;
			navGALTemp.iDot = (fl64)iDot;
			navGALTemp.dataSource = (ui32)dataSource;
			navGALTemp.weekno = (ui32)weekno;
			navGALTemp.SISA = (ui32)SISA;
			navGALTemp.svhealth = (ui32)svhealth;
			navGALTemp.BGD_E5A = (fl64)BGD_E5A;
			navGALTemp.BGD_E5B = (fl64)BGD_E5B;
			navGALTemp.TOW = (fl32)TOW;
			eph->navGAL[satID] = navGALTemp;
		}
}

void parseBDSEntry(FILE * fileID, ephSch_s * eph, fl64 t,int satID, double a_f0,
		double a_f1, double a_f2){

	int validEph;
	float IODE,t_oe,spare1,weekno,spare2,svaccur,TOW,IODC;
	double C_rs,deltan,M_0,C_uc,e,C_us,sqrtA,C_ic,omega_0,C_is,i_0,
		C_rc,omega,omegaDot,iDot,satH1,tgd1,tgd2;
	navBDS_s navBDSTemp;

	if((fscanf(fileID,"%f %lf %lf %lf",&IODE,&C_rs,&deltan,&M_0))!=4){
		printf("parseBDSEntry: Error reading 2nd line of entry\n");
		return;
	}
	if((fscanf(fileID,"%lf %lf %lf %lf",&C_uc,&e,&C_us,&sqrtA))!=4){
		printf("parseBDSEntry: Error reading 3rd line of entry\n");
		return;
	}
	if((fscanf(fileID,"%f %lf %lf %lf",&t_oe,&C_ic,&omega_0,&C_is))!=4){
		printf("parseBDSEntry: Error reading 4th line of entry\n");
		return;
	}
	if((fscanf(fileID,"%lf %lf %lf %lf",&i_0,&C_rc,&omega,&omegaDot))!=4){
		printf("parseBDSEntry: Error reading 5th line of entry\n");
		return;
	}
	if((fscanf(fileID,"%lf %f %f %f",&iDot,&spare1,&weekno,&spare2))!=4){
		printf("parseBDSEntry: Error reading 6th line of entry\n");
		return;
	}
	if((fscanf(fileID,"%f %lf %lf %lf",&svaccur,&satH1,&tgd1,&tgd2))!=4){
		printf("parseBDSEntry: Error reading 7th line of entry\n");
		return;
	}
	if((fscanf(fileID,"%f %f",&TOW,&IODC))!=2){
		printf("parseBDSEntry: Error reading 8th line of entry\n");
		return;
	}

	validEph = checkEphEntry(t,t_oe,'C');
		if(validEph == 1){
			navBDSTemp.a_f0 = (fl64)a_f0;
			navBDSTemp.a_f1 = (fl64)a_f1;
			navBDSTemp.a_f2 = (fl64)a_f2;
			navBDSTemp.IODE = (ui32)IODE;
			navBDSTemp.C_rs = (fl64)C_rs;
			navBDSTemp.deltan = (fl64)deltan;
			navBDSTemp.M_0 = (fl64)M_0;
			navBDSTemp.C_uc = (fl64)C_uc;
			navBDSTemp.e = (fl64)e;
			navBDSTemp.C_us = (fl64)C_us;
			navBDSTemp.sqrtA = (fl64)sqrtA;
			navBDSTemp.t_oe = (fl32)t_oe;
			navBDSTemp.C_ic = (fl64)C_ic;
			navBDSTemp.omega_0 = (fl64)omega_0;
			navBDSTemp.C_is = (fl64)C_is;
			navBDSTemp.i_0 = (fl64)i_0;
			navBDSTemp.C_rc = (fl64)C_rc;
			navBDSTemp.omega = (fl64)omega;
			navBDSTemp.omegaDot = (fl64)omegaDot;
			navBDSTemp.iDot = (fl64)iDot;
			navBDSTemp.weekno = (ui32)weekno;
			navBDSTemp.svaccur = (ui32)svaccur;
			navBDSTemp.satH1 = (fl64)satH1;
			navBDSTemp.tgd1 = (fl64)tgd1;
			navBDSTemp.tgd2 = (fl64)tgd2;
			navBDSTemp.TOW = (fl32)TOW;
			navBDSTemp.IODC = (ui32)IODC;
			eph->navBDS[satID] = navBDSTemp;
		}
}

void parseQZSEntry(FILE * fileID, ephSch_s * eph, fl64 t, int satID,
		double a_f0, double a_f1, double a_f2){

		int validEph;
		float IODE,t_oe,TOW,codes,weekno,spare1,svaccur,svhealth,IODC,spare2;
		double C_rs,deltan,M_0,C_uc,e,C_us,sqrtA,C_ic,omega_0,C_is,i_0,
		C_rc,omega,omegaDot,iDot,T_GD;
		navQZS_s navQZSTemp;

		if((fscanf(fileID,"%f %lf %lf %lf",&IODE,&C_rs,&deltan,&M_0))!=4){
			printf("parseQZSEntry: Error reading 2nd line of entry\n");
			return;
		}
		if((fscanf(fileID,"%lf %lf %lf %lf",&C_uc,&e,&C_us,&sqrtA))!=4){
			printf("parseQZSEntry: Error reading 3rd line of entry\n");
			return;
		}
		if((fscanf(fileID,"%f %lf %lf %lf",&t_oe,&C_ic,&omega_0,&C_is))!=4){
			printf("parseQZSEntry: Error reading 4th line of entry\n");
			return;
		}
		if((fscanf(fileID,"%lf %lf %lf %lf",&i_0,&C_rc,&omega,&omegaDot))!=4){
			printf("parseQZSEntry: Error reading 5th line of entry\n");
			return;
		}
		if((fscanf(fileID,"%lf %f %f %f",&iDot,&codes,&weekno,&spare1))!=4){
			printf("parseQZSEntry: Error reading 6th line of entry\n");
			return;
		}
		if((fscanf(fileID,"%f %f %lf %f",&svaccur,&svhealth,&T_GD,&IODC))!=4){
			printf("parseQZSEntry: Error reading 7th line of entry\n");
			return;
		}
		if((fscanf(fileID,"%f %f",&TOW,&spare2))!=2){
			printf("parseQZSEntry: Error reading 8th line of entry\n");
			return;
		};

		validEph = checkEphEntry(t,t_oe,'J');
		if(validEph == 1){
			navQZSTemp.a_f0 = (fl64)a_f0;
			navQZSTemp.a_f1 = (fl64)a_f1;
			navQZSTemp.a_f2 = (fl64)a_f2;
			navQZSTemp.IODE = (ui32)IODE;
			navQZSTemp.C_rs = (fl64)C_rs;
			navQZSTemp.deltan = (fl64)deltan;
			navQZSTemp.M_0 = (fl64)M_0;
			navQZSTemp.C_uc = (fl64)C_uc;
			navQZSTemp.e = (fl64)e;
			navQZSTemp.C_us = (fl64)C_us;
			navQZSTemp.sqrtA = (fl64)sqrtA;
			navQZSTemp.t_oe = (fl32)t_oe;
			navQZSTemp.C_ic = (fl64)C_ic;
			navQZSTemp.omega_0 = (fl64)omega_0;
			navQZSTemp.C_is = (fl64)C_is;
			navQZSTemp.i_0 = (fl64)i_0;
			navQZSTemp.C_rc = (fl64)C_rc;
			navQZSTemp.omega = (fl64)omega;
			navQZSTemp.omegaDot = (fl64)omegaDot;
			navQZSTemp.iDot = (fl64)iDot;
			navQZSTemp.codes = (ui32)codes;
			navQZSTemp.weekno = (ui32)weekno;
			navQZSTemp.svaccur = (ui32)svaccur;
			navQZSTemp.svhealth = (ui32)svhealth;
			navQZSTemp.T_GD = (fl64)T_GD;
			navQZSTemp.IODC = (ui32)IODC;
			navQZSTemp.TOW = (fl32)TOW;
			eph->navQZS[satID] = navQZSTemp;
		}
}

void parseSBAEntry(FILE * fileID, ephSch_s * eph, fl64 t, int satID,
		double aGf0, double aGf1, double TOW){

	int validEph;
	float svhealth,URA,IODN;
	double xDis,xVel,xAcc,yDis,yVel,yAcc,zDis,zVel,zAcc;
		navSBA_s navSBATemp;

		if((fscanf(fileID,"%lf %lf %lf %f",&xDis,&xVel,&xAcc,&svhealth))!=4){
			printf("parseSBAEntry: Error reading 2nd line of entry\n");
			return;
		}
		if((fscanf(fileID,"%lf %lf %lf %f",&yDis,&yVel,&yAcc,&URA))!=4){
			printf("parseSBAEntry: Error reading 3rd line of entry\n");
			return;
		}
		if((fscanf(fileID,"%lf %lf %lf %f",&zDis,&zVel,&zAcc,&IODN))!=4){
			printf("parseSBAEntry: Error reading 4th line of entry\n");
			return;
		}

		validEph = checkEphEntry(t,TOW,'S');
		if(validEph == 1){
			navSBATemp.aGf0 = (fl64)aGf0;
			navSBATemp.aGf1 = (fl64)aGf1;
			navSBATemp.TOW = (fl32)TOW;
			navSBATemp.xDis = (fl64)xDis;
			navSBATemp.xVel = (fl64)xVel;
			navSBATemp.xAcc = (fl64)xAcc;
			navSBATemp.svhealth = (ui32)svhealth;
			navSBATemp.yDis = (fl64)yDis;
			navSBATemp.yVel = (fl64)yVel;
			navSBATemp.yAcc = (fl64)yAcc;
			navSBATemp.URA = (ui32)URA;
			navSBATemp.zDis = (fl64)zDis;
			navSBATemp.zVel = (fl64)zVel;
			navSBATemp.zAcc = (fl64)zAcc;
			navSBATemp.IODN = (ui32)IODN;
			eph->navSBA[satID] = navSBATemp;
		}
}

int checkEphEntry(fl64 currTime, float t_oe, char gnss){
	int valid = 0;
	float updateRate;

	switch(gnss){
	case 'G':
		updateRate = EPHUPDATEGPS;
		break;
	case 'R':
		updateRate = EPHUPDATEGLO;
		break;
	case 'E':
		updateRate = EPHUPDATEGAL;
		break;
	case 'C':
		updateRate = EPHUPDATEBDS;
		break;
	case 'J':
		updateRate = EPHUPDATEQZS;
		break;
	case 'S':
		updateRate = EPHUPDATESBA;
		break;
	default :
		printf("checkEphEntry: Invalid constellation.\n");
		updateRate = 0;
	}
	if((currTime-t_oe)< updateRate){
		if((currTime-t_oe)>=0){
			valid = 1;
		}
	}
	return valid;
}

void getTimeTransfer(FILE * fileID,ephSch_s * eph){
	char trans[4],buff1[4],buff2[6],buff3[7],buff4[3],buff5[2];
	double f1,f2;
	float i3,i4;
	int lsTemp;

	do {
		if((fscanf(fileID,"%s %lf %lf %f %f %s %s %s",trans,&f1,&f2,&i3,&i4,buff1,
				buff2,buff1)) != 8){
			printf("getTimeTransfer: Error reading time transfer parameters\n");
			return;
		}

		if(!strcmp(trans,"BDUT")){
			bdut_s bdutTemp;
			bdutTemp.A0_UTC = (fl64)f1;
			bdutTemp.A1_UTC = (fl64)f2;
			bdutTemp.T = (si32)i3;
			bdutTemp.W1 = (si32)i4;
			bdutTemp.W2 = 0;
			eph->bdut = bdutTemp;
		}
		else if (!strcmp(trans,"GAUT")){
			gaut_s gautTemp;
			gautTemp.a0 = (fl64)f1;
			gautTemp.a1 = (fl64)f2;
			gautTemp.T = (si32)i3;
			gautTemp.W1 = (si32)i4;
			gautTemp.W2 = 0;
			eph->gaut = gautTemp;
		}
		else if (!strcmp(trans,"GLGP")){
			glgp_s glgpTemp;
			glgpTemp.tauGPS = (fl64)f1;
			glgpTemp.a1 = (fl64)f2;
			glgpTemp.T = (si32)i3;
			glgpTemp.W1 = (si32)i4;
			glgpTemp.W2 = 0;
			eph->glgp = glgpTemp;
		}
		else if (!strcmp(trans,"GLUT")){
			glut_s glutTemp;
			glutTemp.tauC = (fl64)f1;
			glutTemp.a1 = (fl64)f2;
			glutTemp.T = (si32)i3;
			glutTemp.W1 = (si32)i4;
			glutTemp.W2 = 0;
			eph->glut = glutTemp;
		}
		else if (!strcmp(trans,"GPGA")){
			gpga_s gpgaTemp;
			gpgaTemp.A0G = (fl64)f1;
			gpgaTemp.A1G = (fl64)f2;
			gpgaTemp.T = (si32)i3;
			gpgaTemp.W1 = (si32)i4;
			gpgaTemp.W2 = 0;
			eph->gpga = gpgaTemp;
		}
		else if (!strcmp(trans,"GPUT")){
			gput_s gputTemp;
			gputTemp.a0 = (fl64)f1;
			gputTemp.a1 = (fl64)f2;
			gputTemp.T = (si32)i3;
			gputTemp.W1 = (si32)i4;
			gputTemp.W2 = 0;
			eph->gput = gputTemp;
		}
		else if (!strcmp(trans,"QZUT")){
			qzut_s qzutTemp;
			qzutTemp.a0 = (fl64)f1;
			qzutTemp.a1 = (fl64)f2;
			qzutTemp.T = (si32)i3;
			qzutTemp.W1 = (si32)i4;
			qzutTemp.W2 = 0;
			eph->qzut = qzutTemp;
		}
		else if(!strcmp(trans,"SBUT")){
			sbut_s sbutTemp;
			sbutTemp.a0 = (fl64)f1;
			sbutTemp.a1 = (fl64)f2;
			sbutTemp.T = (si32)i3;
			sbutTemp.W1 = (si32)i4;
			sbutTemp.W2 = 0;
			eph->sbut = sbutTemp;
		}
		else if(!strcmp(trans,"QZGP")){
			qzgp_s qzgpTemp;
			qzgpTemp.a0 = (fl64)f1;
			qzgpTemp.a1 = (fl64)f2;
			qzgpTemp.T = (si32)i3;
			qzgpTemp.W1 = (si32)i4;
			qzgpTemp.W2 = 0;
			eph->qzgp = qzgpTemp;
		}
		else {
			printf("getTimeTransfer: Unknown constellation\n");
		}
	} while(strcmp(trans,"QZUT"));

	if((fscanf(fileID,"%d %s %s",&lsTemp,buff1,buff3))!=3){
		printf("getTimeTransfer: Error reading leap seconds\n");
		return;
	}
	eph->leapSeconds = lsTemp;

	if((fscanf(fileID,"%s %s %s",buff4,buff5,buff2))!=3){
		printf("getTimeTransfer: Error reading end of header\n");
		return;
	}
}
