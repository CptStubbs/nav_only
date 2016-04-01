/*
 * calcDateTime.c
 *
 *  Created on: May 21, 2015
 *      Author: sean
 */

#include "calcDateTime.h"
#include "customDataTypes.h"
#include <math.h>

void calcDateTime(calcTimeStruct * calcTime, fl64 weekNumber, fl64 TOW) {

	fl64 regularYear[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304,
			334, 365 };
	fl64 leapYear[] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335,
			366 };

	fl64 mjd = weekNumber * 7 + floor(TOW / SECPERDAY) + JAN61980;
	fl64 fmjd = fmod(TOW, SECPERDAY) / SECPERDAY;

	/* Find eYear Day of Year Hours Minutes and Seconds */
	fl64 daysFromJan11901 = mjd - JAN11901;
	/*1461 = 365+365+365+366 */
	fl64 num4years = floor(daysFromJan11901 / 1461);
	fl64 yearsSoFar = 1901 + 4 * num4years;
	fl64 daysLeft = daysFromJan11901 - 1461 * num4years;
	/* % The second term accounts for a leap year */
	fl64 lastFewYears = floor(daysLeft / 365) - floor(daysLeft / 1460);
	fl64 year = floor(yearsSoFar + lastFewYears);
	fl64 dayofYear = floor(daysLeft - 365 * lastFewYears + 1);
	fl64 hour = floor(fmjd * 24);
	fl64 minute = floor(fmjd * 1440 - hour * 60);
	fl64 second = fmjd * SECPERDAY - hour * 3600 - minute * 60;
	si32 month = 0;
	fl64 dayofMonth = 0;
	si32 guess = (si32) floor(dayofYear * MONPERDAY + 1);
	si32 more = 0;

	if ((si32) floor(fmod(year, 4)) == 0
			&& ((si32) floor(fmod(year, 100)) != 0
					|| (si32) floor(fmod(year, 400)) == 0)) {
		if (dayofYear - leapYear[guess] > 0) {
			more = 1;
		}
		month = guess + more;
		dayofMonth = dayofYear - leapYear[month];

	} else {
		if ((dayofYear - regularYear[guess]) > 0) {
			more = 1;
		}
		month = guess + more;
		dayofMonth = dayofYear - regularYear[month];

	}
	calcTime->year = year;
	calcTime->dayofYear = dayofYear;
	calcTime->month = month;
	calcTime->day = dayofMonth;
	calcTime->hour = hour;
	calcTime->minute = minute;
	calcTime->second = second;

}
