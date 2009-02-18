#ifndef _QSASTIME_H_
#define _QSASTIME_H_
/*
  This software originally contributed under the LGPL in January 2009 to
  PLplot by the
  Cluster Science Centre
  QSAS team,
  Imperial College, London
  Copyright (C) 2009 Imperial College, London

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

/* MJD measures from the start of 17 Nov 1858 */
	
/* These utilities use the Gregorian calendar after 4 Oct 1582 (Julian) i.e. from 15 Oct 1582 Gregorian
   Note C libraries use Gregorian only from 14 Sept 1752
   More detailed discussion can be found at http://aa.usno.navy.mil/data/docs/JulianDate.php
   These routines have been compared with the results of the US Naval Observatory online converter.
   Modified Julian Date (MJD) = Julian Date (JD) - 2400000.5
	
   In all routines, specifying a day, hour, minute or second field greater than would be valid is
   handled with modulo arithmetic and safe.
   Thus 2006-12-32 00:62:00.0 will safely, and correctly, be treated as 2007-01-01 01:02:00.0
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* include header file for dll definitions */
#include "qsastimedll.h"


typedef struct MJDtimeStruct
{
  /*
    MJD starts at 0h, so truncating MJD always gives the same day whatever the time (unlike JD).
    The MJD base day is arbitrary, i.e. seconds can be greater than one day or even negative.
  */
	
  int base_day; /* integer part of MJD used as default */
  double time_sec; /* seconds from start of base_day */
	
}MJDtime;

typedef struct QSASConfigStruct
{

  /* Values used to define the transformation between broken down time
     and continuous time for the public API of libqsastime, 
     continuous_time_qsas, broken_down_time_qsas, and strfqsas.*/
     
  /* scale multiplies the continuous time variable to convert the units to
     days. */
  double scale;

  /* offset1 and offset2 (in days) specifies the amount to add to the
     scaled continuous time to derive the MJD time value that is used
     internally by libqsastime.  Normally epoch1 is an integral
     value (which can be exactly stored in a double for a very wide
     range of integers) and offset2 is normally a non-integral value
     whose absolute value is less than 1.  This arrangement allows the
     continuous time variable in the API to be stored as a single double
     without compromising numerical precision if epoch1 and epoch2
     are chosen wisely. */
  double offset1, offset2;

  /* The various bits of ccontrol are used as independent switches to 
     control optional additional corrections which define the
     transformation between continuous time and broken-down time.

     If bit 0 (the lowest order bit of ccontrol) is 1 the Julian
     proleptic calendar is used for broken-down time. Otherwise the
     Gregorian proleptic calendar is used for broken-down time.

     If bit 1 is 1, an additional correction for the difference
     between atomic-clock based times and UTC is applied to the broken-down
     times.

     We reserve other bits of ccontrol for future use. */
  int ccontrol;
  
}QSASConfig;

QSASTIMEDLLIMPEXP_DATA(QSASConfig) *qsasconfig;

/* externally accessible functions */
QSASTIMEDLLIMPEXP void configqsas(double scale, double offset1, double offset2, int ccontrol);
QSASTIMEDLLIMPEXP void closeqsas(void);
QSASTIMEDLLIMPEXP int ctimeqsas(int year, int month, int day, int hour, int min, double sec, double * ctime);
QSASTIMEDLLIMPEXP void btimeqsas(int *year, int *month, int *day, int *hour, int *min, double *sec, double ctime);
QSASTIMEDLLIMPEXP size_t strfqsas(char * buf, size_t len, const char *format, double ctime);

#endif
