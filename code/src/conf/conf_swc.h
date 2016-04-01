#ifndef SRC_CONF_CONF_SWC_H_
#define SRC_CONF_CONF_SWC_H_
extern int make_ISO_compiler_happy;
#define SWC_MILLISEC 2
#define DATAFILEIN "/archive/MAX2769_L1_20150828_20150831_fs_6864e6_if21912e6_schar_1TB.bin"

//DATAFILEOUT
#define DATAFILEOUT "/dev/null"


//OUTPUT File settings

//DATAFILEOUTNAV
#define DATAFILEOUTNAV "/dev/null"

//DATAFILEOUTLOCK
#define DATAFILEOUTLOCK "/tmp/PRNLock.bin"



#ifdef USEFILE
#define APTOUT "../../input/timingaptBinaries_%d_%d.bin"
#define KMLOUT "../../output/googleEarth.kml"
#define RNXOUT "../../output/rnxBinaries_%d_%d.bin"
#endif


//SINCOSCA
// For Daehee & Sara
#define DATAFILESIN "/tmp/largeFiles/LookupTables/sinint.bin"
#define DATAFILECOS	"/tmp/largeFiles/LookupTables/cosint.bin"
#define DATAFILECATABLE "/tmp/largeFiles/LookupTables/casat%d.dat"
#define DATAFILECSTABLE "/tmp/largeFiles/LookupTables/cssat%d.dat"


// This function prints the last received error messaged and the current line
#define ERRORCHECK(a)\
		if (a)\
		{\
			perror("Error Message: ");\
			fprintf(stderr, "Error at %s line %d\n", __FILE__, __LINE__);\
			return EXIT_FAILURE;\
		}


#endif /* SRC_CONF_CONF_SWC_H_ */

