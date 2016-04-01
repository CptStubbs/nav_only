/** 
 This is the configuration file for the chn thread
 **/
#ifndef SRC_CONF_CONF_LSS_H_
	#define SRC_CONF_CONF_LSS_H_

	extern int make_ISO_compiler_happy;

	#define CHN_ASN_ACTIVE 	0x00000000FFFFFFFF	/// Active satellites (launched and working)
	#define CHN_ASN_VISIBLE 0x00000000FFFFFFFF	/// Visible satellites

	#define DLLDAMPINGRATIO 0.7
	#define DLLNOISEBW 2
	#define PLLDAMPINGRATIO 0.7
	#define PLLNOISEBW 25
	#define PDICODE 0.001
	#define PDICARR 0.001
	#define EARLYLATESPC 0.5

	#define MYPRN {7, 28,	30,	8,	13,	11,	19}
	#define MYPRNCODEPHASE {1, 5947,1372,5955,5144,1169,4468}  //1269
	#define MYPRNCARRFREQ {2190643.21,2192913.30,2192549.89,2189398.61,2194138.76,2193487.07,2188905.13}
	#define PRNLISTLENGTH 7
	//#define CARRDOPPSEARCH 5000  //the freq search space is +/- this value from the nominal
	#define CARRSTEPACQ 600  //the step size for the acquisition carrier freq search
	#define SAMPLESIN1_5CHIPS 10  //samples in 1.5 chips, depends on sampling freq
	#define	MYPRNCODEFREQ 1023000.0
	#define ACQTHRESHOLD (ui64) 2.5e8//9e7//2e8 for all the other ones.

	#define CARRERRTHRESH 0.075
  #define VSMTHRESH 35
	#define VSMINT 200

	/// Lock parameters
	#define CHN_LCK_FALSEPHASETHRESHOLD 			15.5 			///	[Hz], Kaplan 2nd p235
	#define CHN_LCK_FALSEPHASEINTTIME					1.0				///	[s], Kaplan 2nd p235

	/// Lock parameters
	#define CHN_LCK_FALSEPHASETHRESHOLD 			15.5 			///	[Hz], Kaplan 2nd p235
	#define CHN_LCK_FALSEPHASEINTTIME					1.0				///	[s], Kaplan 2nd p235
	/// Acquisition parameters
	#define CHN_ACQ_MAXCARROFFSET							2000.0		/// Single sided
	#define CHN_SIG_CARRCODERATIO							1540.0		/// 1575.42MHz/1.023Mcps
	#define CHN_ACQ_MAXCODEOFFSET CHN_ACQ_MAXCARROFFSET/CHN_SIG_CARRCODERATIO
	/// Tracking parameters
	#define CHN_TRK_CARRTIME									0.001			/// Integration time
	#define CHN_TRK_CODETIME									0.001			/// Integration time
	#define CHN_TRK_CARRBW										25.0			/// [Hz], Carrier tracking bandwidth
	#define CHN_TRK_CODEBW										2.0				/// [Hz], Code tracking bandwidth
#endif /* SRC_CONF_CONF_LSS_H_ */

