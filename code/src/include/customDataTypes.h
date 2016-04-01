#ifndef __CUSTOMDATATYPES_H
	#define __CUSTOMDATATYPES_H
	#include <complex.h>
	#ifdef __arm__
		/* typedefs for ARM */
		typedef char 														pi8;			/// "Plain char", different from si8 and ui8 (staffan hates C)
		typedef unsigned char 									ui8;			/// "Unsigned char"
		typedef signed char 										si8; 			/// "Signed char"
		typedef unsigned short 									ui16;			/// unsigned 16 bit value
		typedef complex short									csi16;			/// complex signed 16 bit value
		typedef signed short 										si16;			/// signed 16 bit value
		typedef unsigned long int 							ui32;			/// unsigned 32 bit integer
		typedef complex int 								csi32;			/// complex signed 32 bit integer
		typedef signed long int 								si32;			/// signed 32 bit integer
		typedef long long unsigned int 					ui64;			/// unsigned 64 bit integer
		typedef long long signed int 						si64;			/// signed 64 bit integer
		typedef float 													fl32;			/// 32 bit float
		typedef complex float										cf32;			/// 32 bit complex float (64 bits in total)
		typedef double 													fl64;			/// 64 bit float
		typedef complex double									cf64;			/// 64 bit complex float (128 bits in total)
		typedef complex signed char 						csi8; 		/// TODO - Remove (This does not work - only floats can be complex)
		typedef double complex 									fl64comp;	/// TODO - Remove
	#else
		/* typedefs for x86-64 */
		typedef double fl64;
		typedef float fl32;
		typedef long long int si64;
		typedef long long unsigned int ui64;
		typedef complex int csi32;
		typedef int si32;
		typedef unsigned int ui32;
		typedef short si16;
		typedef complex short csi16;
		typedef unsigned short ui16;
		typedef char pi8;
		typedef signed char si8; /* TODO better name. Refactor all the si8s to i8s*/
		typedef complex signed char csi8; /* TODO better name. Refactor all the si8s to i8s*/
		typedef unsigned char ui8;
		typedef double complex fl64comp;
	#endif

	typedef enum {
		false, true
	} bool;





	/// TODO - remove i8, it does not belong
#endif
