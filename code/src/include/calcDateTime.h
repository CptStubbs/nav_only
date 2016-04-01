/*
 * calcDateTime.h
 *
 *  Created on: May 12, 2015
 *      Author: sean
 */

#ifndef NAV_CALCDATETIME_H_
#define NAV_CALCDATETIME_H_

#include "customDataTypes.h"

typedef struct {
	fl64 year;
	fl64 dayofYear;
	fl64 month;
	fl64 day;
	fl64 hour;
	fl64 minute;
	fl64 second;

} calcTimeStruct;

/*! Modified Julian Date (MJD) for the initial epoch of GPS Time */
#define JAN61980  44244
/*! MJD for 0 hours, Jan 1, 1901 */
#define JAN11901  15385
/*! Number of seconds in a day */
#define SECPERDAY  86400.0
/*! Approx. number of months in a day */
#define MONPERDAY  0.032

/*!
 * \brief Calculate time in year,month,day,hour,minute,second
 * from the GPS time (week, TOW)
 * Not UTC time
 * \param calcTime - time structure (year, month, day, hour, minute, second)
 * \param weekNumber - GPS week number
 * \param TOW - GPS time of week
 */
void calcDateTime(
/* Output */
calcTimeStruct * calcTime,
/* Input */
fl64 weekNumber, fl64 TOW);

#endif /* NAV_CALCDATETIME_H_ */
