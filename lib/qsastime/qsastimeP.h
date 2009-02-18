#ifndef _QSASTIMEP_H_
#define _QSASTIMEP_H_
/* Declaration of private functions within libqsastime that are needed
   for testing of libqsastime, but which are not normally needed for anything
   else */

#include "qsastime.h"
QSASTIMEDLLIMPEXP int setFromUT(int year, int month, int day, int hour, int min, double sec, MJDtime *MJD, int forceJulian);
QSASTIMEDLLIMPEXP void breakDownMJD(int *year, int *month, int *day, int *hour, int *min, double *sec, const MJDtime *MJD, int forceJulian);
QSASTIMEDLLIMPEXP size_t strfMJD(char * buf, size_t len, const char *format, const MJDtime *MJD, int forceJulian);
QSASTIMEDLLIMPEXP void normalize_MJD(MJDtime *MJDout, const MJDtime *MJDin);
QSASTIMEDLLIMPEXP const char * getDayOfWeek(const MJDtime *MJD);
QSASTIMEDLLIMPEXP const char * getLongDayOfWeek( const MJDtime *MJD);
QSASTIMEDLLIMPEXP const char * getMonth(int m);
QSASTIMEDLLIMPEXP const char * getLongMonth(int m);
QSASTIMEDLLIMPEXP void getYAD(int *year, int *ifleapyear, int *doy, const MJDtime *MJD, int forceJulian);

#endif
