#ifndef _QSASTIMEP_H_
#define _QSASTIMEP_H_
/* Declaration of private functions within libqsastime that are needed
   for testing of libqsastime, but which are not normally needed for anything
   else */

#include "qsastime.h"
int setFromUT(int year, int month, int day, int hour, int min, double sec, MJDtime *MJD, int forceJulian);
void breakDownMJD(int *year, int *month, int *day, int *hour, int *min, double *sec, const MJDtime *MJD, int forceJulian);
size_t strfMJD(char * buf, size_t len, const char *format, const MJDtime *MJD, int forceJulian);
void normalize_MJD(MJDtime *MJDout, const MJDtime *MJDin);
const char * getDayOfWeek(const MJDtime *MJD);
const char * getLongDayOfWeek( const MJDtime *MJD);
const char * getMonth(int m);
const char * getLongMonth(int m);
void getYAD(int *year, int *ifleapyear, int *doy, const MJDtime *MJD, int forceJulian);

#endif
