/** 
  This is the configuration file for shared settings
 **/

#ifndef __CONF_H
#define __CONF_H

#include <complex.h>

#define CONF_NUMCHN 12																/// NUMber of CHaNnels (divisible with 4)
#define CONF_NUMBLK 4																	/// NUMber of BLocKs (should be 4)
#define CONF_NUMCPB CONF_NUMCHN/CONF_NUMBLK						/// NUMber of Channels Per Block

#define NUMTAPS 3														/// Number of correlators
#define MAX_LEN_FILENAME 256								/// Maximum length of filename string

#define SAMPLINGRATE (ui64 )6.864e6		/*Sampling frequency, default is a 40MHz sampling rate*/
#ifdef USECOMP 
#define DATATYPE csi8
#else 
#define DATATYPE si8 						/*Defines file type 8 = schar*/
#endif
#define DATATYPESIZE sizeof(DATATYPE) 		/*Defines file type 8 = schar*/
#define CARRFREQ 2.1912e6					/*Intermediate frequency*/
#define NUMSAMPLE1MSEC SAMPLINGRATE/1000	/*Number of samples in one ms*/
#define NUMSAMPLEINBUFF 6178 // ( (NUMSAMPLE1MSEC / 10) * 9)//6178 65536 //2^16, should be more than 1msec of data (depends on sampling freq), more has more latency
#define NUMBUFFERS 36000	//number of msec to process, only approx value REALLY is the number of data buffers to process
#define CARRDOPPSEARCH  5000  //the freq search space is +/- this value from the nominal
#define CARRPHASEINVERSION 0
#define PRNCODELENGTH 1023   //Lenght of the PRN code
#define L2CODELENGTH 10230 * 2 //Contains both CM and CL
#define CARRSTEP 500.00
#define CODEFREQ 1.023e6
#define INTTIME  0.001 								/// [s], integration time
// TODO Convert this to make file
#define MODE 0// 1 if use direct tracking and 0 if use acquisition
#define SAMPLESPERCODE round(SAMPLINGRATE * PRNCODELENGTH / CODEFREQ)
#define GPSPI 				3.1415926535898  	//Pi used in the GPS ICD
#define L1FREQ 1575.42e6


#define PDICODE 0.001
#define PDICARR 0.001
#define EARLYLATESPC 0.5
#define NRSECREAD 40000
#define APTFILEINTEVAL 100*60*60*6

#define NAVDEBUG 1	// NAV debugging mode: run with APT

#define LOGCOR 0 // Log the correlator data to a file
#define LOGAPT 0 // Log the navigation data to a file
#define LOGKML 0 // Log the KML data for Google Earth
#define LOGRNX 1 // Log the LSS structure in navigation


#if NAVDEBUG
#define LOGAPT 0 // Log the navigation data to a file
#endif //#IF NAVDEBUG

#endif
