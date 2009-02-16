#ifndef _QSASTIMEP_H_
#define _QSASTIMEP_H_
/* Declaration of private functions within libqsastime that are needed
   for testing of libqsastime, but which are not normally needed for anything
   else */

#include "qsastime.h"
QSASTIMEDLLIMPEXP void normalize_MJD(MJDtime *MJDout, const MJDtime *MJDin);
QSASTIMEDLLIMPEXP const char * getDayOfWeek(const MJDtime *MJD);
QSASTIMEDLLIMPEXP const char * getLongDayOfWeek( const MJDtime *MJD);
QSASTIMEDLLIMPEXP const char * getMonth(int m);
QSASTIMEDLLIMPEXP const char * getLongMonth(int m);
QSASTIMEDLLIMPEXP void getYAD(int *year, int *ifleapyear, int *doy, const MJDtime *MJD, int forceJulian);

#endif
