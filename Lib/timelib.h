/*
File:	 timelib.h
Version: 1.0
Date:    18.10.2017
*/

#ifndef _TIMELIB_H_
#define _TIMELIB_H_

struct _DateTime
{
	unsigned short Day, Month, Year;
	unsigned short Hour, Min, Sec, mSec;
};

#ifdef __cplusplus
extern "C" {
#endif
double DateTimeToDaysJ2000(struct _DateTime *stDateTime);
double DateTimeToUnixTime(struct _DateTime *stDateTime);
double StarsTime(struct _DateTime *curDateTime, short zone);
long long int DateTimeToTimeAVU(struct _DateTime *stDateTime);
#ifdef __cplusplus
}
#endif

#endif
