/*
  Copyright (C) 2009 Alan W. Irwin

  This file is part of PLplot.

  PLplot is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Library Public License as published
  by the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  PLplot is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with PLplot; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

*/
#include "qsastime.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST01 0x1
#define TEST02 0x2
#define TEST03 0x4
#define TEST04 0x8
#define TEST05 0x10
#define TEST06 0x20
#define TEST07 0x40
#define TEST08 0x80
#define TEST09 0x100
#define TEST10 0x200
#define TEST11 0x400
#define TEST12 0x800
#define TEST13 0x1000
#define TEST14 0x2000
#define TEST15 0x4000
#define TEST16 0x8000

/* Recommended (by Linux timegm man page) POSIX equivalent of Linux timegm C library function */
my_timegm(struct tm *tm)
{
  time_t ret;
  char *tz;

  tz = getenv("TZ");
  setenv("TZ", "", 1);
  tzset();
  ret = mktime(tm);
  if (tz)
    setenv("TZ", tz, 1);
  else
    unsetenv("TZ");
  tzset();
  return ret;
}


/* Test program to do extensive comparisons between setFromUT, breakDownMJD,
   and strfMJD and the closest corresponding _Linux_ C library routines,
   timegm, gmtime, and strftime.  */

int main()
{
  char buf[360];
  char buf1[360];
  int year, month, day, hour, min;
  double sec;
  int year1, month1, day1, hour1, min1;
  double sec1;
  struct tm tm;
  struct tm *ptm = &tm;
  time_t secs_past_epoch, secs_past_epoch1, delta_secs, max_delta_secs, secs, secs1, secs2;
  
  MJDtime MJD1, *pMJD1 = &MJD1;
  MJDtime MJD2;
  static int MJD_1970 = 40587; /* MJD for Jan 01, 1970 00:00:00 */
  double jd;
  int test_choice;

  printf("sizeof(time_t) = %d\n",(int)sizeof(time_t));
  if(sizeof(time_t) < 8) {
    printf("tests abandoned because time_t too small on this platform to run this programme\n");
    return 1;
  }

  printf("sizeof(int) = %d\n",(int)sizeof(int));
  if(sizeof(int) !=4) {
    printf("tests abandoned because int must be 32-bits to test this library properly for how well it will potentially perform on 32-bit platforms\n");
    return 2;
  }
  /* strftime affected by locale so force 0 timezone for this complete test. */
  setenv("TZ", "", 1);
  tzset();
  
  /* choose test(s) to be run using bit-pattern in test_choice that is
     input from stdin. */
  scanf("%i", &test_choice);

  if(test_choice & TEST01) {
    printf("Test 01 of calendar dates in vicinity of Julian day epoch \n");

    for (year=-4717; year<=-4707; year+=1) {
      month = 0;
      day = 1;
      hour = 12;
      min = 0;
      sec = 0.;

      ptm->tm_year = year-1900;
      ptm->tm_mon = month;
      ptm->tm_mday = day;
      ptm->tm_hour = hour;
      ptm->tm_min = min;
      ptm->tm_sec = (int) sec;
      /* Start of Julian proleptic section of test in loop. */
      printf("input and output (strfMJD, Julian proleptic calendar) date/time\n");
      printf("%d-%02d-%02dT%02d:%02d:%018.15fZ\n", year, month+1, day, hour, min, sec);
      setFromUT(year, month, day, hour, min, sec, pMJD1, 1);
      strfMJD(&(buf[0]), 360, "%Y-%m-%dT%H:%M:%SZ\n", pMJD1, 1);
      printf("%s", buf);
      jd = 2400000.5 + pMJD1->base_day + pMJD1->time_sec/86400.;
      printf("setFromUT JD = %25.16f days\n", jd);
      breakDownMJD(&year1, &month1, &day1, &hour1, &min1, &sec1, pMJD1, 1);
      if(year1-year != 0 || month1-month != 0 || day1-day != 0 || hour1-hour != 0 || min1-min !=0 || secs1-sec != 0.) {
	printf("output date calculated with breakDownMJD for Julian proleptic calendar = %d-%02d-%02dT%02d:%02d:%018.15fZ\n", year, month+1, day, hour, min, sec);
	printf("test1 failed with inconsistency between setFromUT and breakDownMJD for Julian proleptic calendar/n");
	return 1;
      }
      /* Start of Gregorian proleptic section of test in loop. */
      printf("input and output (strftime), and output (strfMJD, Gregorian proleptic calendar) date/time\n");
      printf("%d-%02d-%02dT%02d:%02d:%018.15fZ\n", year, month+1, day, hour, min, sec);
      strftime(&(buf[0]), 360, "%Y-%m-%dT%H:%M:%SZ\n", ptm);
      printf("%s", buf);
      setFromUT(year, month, day, hour, min, sec, pMJD1, -1);
      strfMJD(&(buf[0]), 360, "%Y-%m-%dT%H:%M:%SZ\n", pMJD1, -1);
      printf("%s", buf);
      jd = 2400000.5 + pMJD1->base_day + pMJD1->time_sec/86400.;
      printf("setFromUT JD = %25.16f days\n", jd);
      breakDownMJD(&year1, &month1, &day1, &hour1, &min1, &sec1, pMJD1, -1);
      if(year1-year != 0 || month1-month != 0 || day1-day != 0 || hour1-hour != 0 || min1-min !=0 || secs1-sec != 0.) {
	printf("output date calculated with breakDownMJD for Gregorian proleptic calendar = %d-%02d-%02dT%02d:%02d:%018.15fZ\n", year1, month1+1, day1, hour1, min1, sec1);
	printf("test1 failed with inconsistency between setFromUT and breakDownMJD for Gregorian proleptic calendar\n");
	return 1;
      }
    }
  }
  return 0;
 // test wide range of years that almost integer overflows on each end
  // and which definitely overflow the MJD integer.
  //for (year=-2000000000; year<=2000000000; year+=1000000000) {
  // test small range which nevertheless exceeds 32-bit date range
  //for (year=1900; year<=2100; year++) {
  // Span year epoch
  //for (year=-10; year<=10; year+=1) {
  // Span MJD epoch.
  //for (year=1855; year<=1861; year+=1) {
  // test reduced range of years that just barely misses overflowing
  // the MJD integer.  e.g., 6000000 overflows it. 
  //for (year=-5000000; year<=5000000; year+=1) {
  max_delta_secs = 0;
  ptm->tm_year = 2008-1900;
  secs1 = my_timegm(ptm);
  ptm->tm_year = 2010-1900;
  secs2 = my_timegm(ptm);
  for (secs=secs1; secs<=secs2; secs+=1) {
    pMJD1->base_day = MJD_1970 + secs/86400;
    pMJD1->time_sec = secs % 86400;
    breakDownMJD(&year, &month, &day, &hour, &min, &sec, pMJD1, 0);
    ptm = gmtime(&secs);
    if(year-ptm->tm_year-1900 !=0 || month-ptm->tm_mon !=0 || day-ptm->tm_mday !=0 || hour-ptm->tm_hour !=0 || min-ptm->tm_min !=0 || ((int) sec)-ptm->tm_sec !=0) {
      printf("ptm-> year+1900, month+1, day, hour, minute, sec = %d-%02d-%02dT%02d:%02d:%02dZ\n", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
      printf("non-zero (dyear, dmonth, dday, dhour, dmin, or d (int) sec) = (%d, %d, %d, %d, %d, %d)  so take early exit", year-ptm->tm_year-1900, month-ptm->tm_mon, day-ptm->tm_mday, hour-ptm->tm_hour, min-ptm->tm_min, ((int) sec)-ptm->tm_sec);
      return 1;
    }
    if(sec - (int) sec !=0) {
      printf("ptm-> year+1900, month+1, day, hour, minute, sec = %d-%02d-%02dT%02d:%02d:%02dZ\n", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
      printf("non-zero fractional part of sec = %f so take early exit\n", sec - (int) sec);
      return 2;
    }
   
    if(0) {
    ptm->tm_year = year-1900;
    //printf("year = %d\n", year);

    // Test to compare setFromUT and my_timegm results.
    setFromUT(year, month, day, hour, min, sec, pMJD1, -1);
    secs_past_epoch1 = (time_t) (86400.*((double)pMJD1->base_day - (double) MJD_1970) + pMJD1->time_sec);
    if(pMJD1->time_sec != 0.) {
      printf("non-zero pMJD1->time_sec value is %25.16f seconds so take early exit.\n", pMJD1->time_sec);
      return 2;
    }
    secs_past_epoch = my_timegm(ptm);
    delta_secs = abs(secs_past_epoch1-secs_past_epoch);
    max_delta_secs = (max_delta_secs > delta_secs ? max_delta_secs: delta_secs);
    }
     
    if(0) {
    printf("setFromUT secs_past_epoch = %lld seconds\n", secs_past_epoch1);
    printf("my_timegm secs_past_epoch = %lld seconds\n", secs_past_epoch);
    printf("delta secs_past_epoch = %d seconds\n", (secs_past_epoch1 - secs_past_epoch));
    strftime(&(buf[0]), 360, "strftime gives %Y-%m-%d %H:%M:%S", ptm);
    printf("%s\n", buf);

    ptm = gmtime(&secs_past_epoch);
    printf("ptm-> year, month, day, hour, minute, sec = %d-%d-%d %d:%d:%d\n", ptm->tm_year+1900, ptm->tm_mon, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    
    ptm->tm_mon = month;
    ptm->tm_mday = day;
    ptm->tm_hour = hour;
    ptm->tm_min = min;
    ptm->tm_sec = (int) sec;

    /* next step for wide range of years loop alternative would overflow year so get out of loop this way. */
    if(year==2000000000) {
      break;
      }
    }
  }
  printf("max_delta_secs = %lld\n", max_delta_secs);
  return 0;
}
