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
/* Test program to do extensive comparisons between setFromUT, breakDownMJD,
   and strfMJD and the closest corresponding _Linux_ C library routines,
   timegm, gmtime, and strftime.  */

int main()
{
  char buf[360];
  char buf1[360];
  int year;
  int month=0;
  int day = 1;
  int hour = 0;
  int min = 0;
  double sec = 0.;
  struct tm tm;
  struct tm *ptm = &tm;
  time_t secs_past_epoch, secs_past_epoch1;
  
  MJDtime MJD1, *pMJD1 = &MJD1;
  MJDtime MJD2;
  static int MJD_1970 = 40587; /* MJD for Jan 01, 1970 00:00:00 */
  printf("sizeof(int) = %d\n",(int)sizeof(int));
  printf("sizeof(time_t) = %d\n",(int)sizeof(time_t));
  if(sizeof(time_t) < 8) {
    printf("time_t too small on this platform to run this programme");
    return 1;
  }

  /* strftime affected by locale so force 0 timezone for this complete test. */
  setenv("TZ", "", 1);
  tzset();
  
  // test wide range of years that almost integer overflows on each end
  // and which definitely overflow the MJD integer.
  //for (year=-2000000000; year<=2000000000; year+=1000000000) {
  // test small range which nevertheless exceeds 32-bit date range
  //for (year=1900; year<=2100; year++) {
  // test reduced range of years that just barely misses overflowing
  // the MJD integer.  e.g., 6000000 overflows it. 
  //for (year=-5000000; year<=5000000; year+=1000000) {
  for (year=-10; year<=10; year+=1) {
    ptm->tm_year = year-1900;
    ptm->tm_mon = month;
    ptm->tm_mday = day;
    ptm->tm_hour = hour;
    ptm->tm_min = min;
    ptm->tm_sec = (int) sec;

    printf("year = %d\n", year);
    setFromUT(year, month, day, hour, min, sec, pMJD1, -1);
    secs_past_epoch1 = (time_t) (86400.*((double)pMJD1->base_day - (double) MJD_1970) + pMJD1->time_sec);
    secs_past_epoch = timegm(ptm);
    printf("delta secs_past_epoch = %d seconds\n", (secs_past_epoch1 - secs_past_epoch));
    strftime(&(buf[0]), 360, "strftime gives %Y-%m-%d %H:%M:%S", ptm);
    printf("%s\n", buf);

    ptm = gmtime(&secs_past_epoch);
    printf("ptm-> year, month, day, hour, minute, sec = %d-%d-%d %d:%d:%d\n", ptm->tm_year+1900, ptm->tm_mon, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    
    /* next step for wide range of years loop alternative would overflow year so get out of loop this way. */
    if(year==2000000000) {
      break;
      }

  }
  return 0;
}
