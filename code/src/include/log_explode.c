/*
 * log_explode.c
 *
 * Contains the main function and several helper functions
 *  Created on: Oct 7, 2015
 *      Author: staffan
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customDataTypes.h"
//#include "ccr_explode.h"
#include "apt_emt_explode.h"
#include "lss_explode.h"
#include "conf.h"
#include "conf_log.h"
#include <dirent.h>
#include <sys/stat.h>

int log_explodeMain(){
	// find all files that start with ccr
	DIR * dir = opendir(CONF_LOG_PATH);
	struct dirent *de;
	struct stat s;
	char fileName[MAX_LEN_FILENAME];
	while((de = readdir(dir)) !=NULL)
	{
		snprintf(fileName,MAX_LEN_FILENAME,"%s/%s",CONF_LOG_PATH,de->d_name);

		stat(fileName, &s);
		if ((strcmp(de->d_name,"ccr.bin")==0) && (s.st_size>0)){
			printf("Exploding '%s' is disabled.\n",fileName);
//			ccr_explode("ccr");
		}	else if ((strcmp(de->d_name,"apt_emt.bin")==0) && (s.st_size>0)){
				printf("Exploding '%s'.\n",fileName);
				apt_emt_explode("apt_emt");
		}	else if ((strcmp(de->d_name,"lss.bin")==0) && (s.st_size>0)){
				printf("Exploding '%s'.\n",fileName);
				lss_explode("lss");
		}
	}
	return EXIT_SUCCESS;
}

void fopenLogs(ui32 * ret, FILE ** fd, char * fileNameBase, char * name, char * type, char * mode){
	/// Create filename
	char fileName[MAX_LEN_FILENAME];
	snprintf(fileName,MAX_LEN_FILENAME-1,"%s/%s_%s.%s", CONF_LOG_PATH, fileNameBase, name, type);
//	printf("%s\t",fileName);																			/// Print fileNames (debugging)

	/// Open file and check validity
	*fd = fopen(fileName, mode);																	/// Open file
	if(fd == NULL){																								/// Make sure that the file opened correctly
		fprintf(stderr,"\nCould not open '%s'! \n",fileName);				/// Print warning otherwise,
		ret += EXIT_FAILURE;																				/// And exit with error
		return;
	}

//	printf("%llX\n",(long long int) *fd);													/// Print address of file pointer (debugging)
	return;
}

int purgeLogFiles(char * fileNameBase){
	ui32 a,b,c;
	/// Check length of LOG_PATH
	a = strlen(CONF_LOG_PATH);
//	printf("|%s| = %d\n",LOG_PATH,a);
	if (a > (MAX_LEN_FILENAME/2)){
		fprintf(stderr,"Error: log_explodec/purgeLogFiles - Length of 'LOG_PATH' too long\n");
		return EXIT_FAILURE;
	}

	/// Check length of fileNameBase
	b = strlen(fileNameBase);
//	printf("|%s| = %d\n",fileNameBase,b);
	if (b > (MAX_LEN_FILENAME/2)){
		fprintf(stderr,"Error: log_explodec/purgeLogFiles - Length of 'fileNameBase' too long\n");
		return EXIT_FAILURE;
	}

	/// Make sure that fileNameBase does not include a file separator (dangerous)
	char * ind = strpbrk(fileNameBase, "/");
	if (ind != NULL){
		fprintf(stderr,"Error: log_explodec/purgeLogFiles - File separator not allowed in 'fileNameBase'\n");
		return EXIT_FAILURE;
	}

	char cmd[MAX_LEN_FILENAME];	/// Allocate command string
	snprintf(cmd,MAX_LEN_FILENAME,"rm %s/%s* > /dev/null 2>&1",CONF_LOG_PATH, fileNameBase);

	/// Check length of fileNameBase
	c = strlen(cmd);
//	printf("|%s| = %d\n",cmd,c);
	if (c > (MAX_LEN_FILENAME)){
		fprintf(stderr,"Error: log_explodec/purgeLogFiles - Length of 'cmd' too long\n");
		return EXIT_FAILURE;
	}

	/// Execute command
	if (system(cmd) == -1){
		fprintf(stderr,"Error: log_explodec/purgeLogFiles - cmd was not successful\n");
		fprintf(stderr,"cmd: '%s'\n",cmd);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


