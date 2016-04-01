/** 
 This is the configuration file for the apt (chn->nav) data structure
 **/

#ifndef _SRC_CONF_CONF_APT_H
	#define SRC_CONF_CONF_APT_H
	extern int make_ISO_compiler_happy;

	#include "conf.h"

	#define APT_LEN 	100	/// Number of correlator values to gather in chn before sending to nav
	#define APT_NUM 	CONF_NUMCHN	/// Number of entries in array (related to maximum acceptable latency) WAS 2* but changed

#endif /* SRC_CONF_CONF_LSS_H */
