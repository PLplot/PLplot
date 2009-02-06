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
	
	/* These utilities use the Gregorian calendar after 4 Oct 1582 (Julian) i.e. from 15 Oct 1582 Gregoriam
	 Note C libraries use Gregorian only from 14 Sept 1752
	 More detailed discussion can be found at http://aa.usno.navy.mil/data/docs/JulianDate.php
	 These routines have been compared with the results of the US Naval Observatory online converter.
	 Modified Julian Date (MJD) = Julian Date (JD) - 2400000.5
	
	 In all routines, specifying a day, hour, minute or second field greater than would be valid is
	 handled with modulo arithmetic and safe.
	 Thus 2006-12-32 00:62:00.0 will safely, and correctly, be treated as 2007-01-01 01:02:00.0
	
	*/
#include <ctype.h>
#include "qsastime.h"

static double SecInDay = 86400; /* we ignore leap seconds */
static int MJD_1970 = 40587; /* MJD for Jan 01, 1970 00:00:00 */
static const int MonthStartDOY[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
static const int MonthStartDOY_L[] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
	
int setFromUT(int year, int month, int day, int hour, int min, double sec, MJDtime *MJD, int forceJulian)
{	
	/* convert Gregorian date plus time to MJD */
	/* MJD measures from the start of 17 Nov 1858 */
	
	/* the int flag Julian forces use of Julian  calendar whatever the year */
	/* default is to use Gregorian after 4 Oct 1582 (Julian) i.e. from 15 Oct 1582 Gregorian */
	/* Note C libraries use Gregorian only from 14 Sept 1752 onwards */

        int leaps, lastyear;

	/* Approximate precaution to avoid overflowing integer portion of 
	   MJD.  MJD epoch is 1858-11-17.  The months, days, etc., portion
	   of this calculation are only included to guard against
	   extremely large values being used for some/all of them. */
	if(abs(365.25*(year-1858) + 12.*(month-10) + (day-17) + hour/24. 
	       + min/1440. + sec/86400.) > 2.e9)
	  return 1;
	
	if(year <= 0)
	{
		/* count leap years on Julian Calendar */
		/* MJD for Jan 1 0000 (correctly Jan 01, BCE 1) is  - 678943, count from there */
		/* negative CE (AD) years convert to BCE (BC) as  BCE = 1 - CE, e.g. 2 BCE = -1 CE */
		
		leaps = year / 4 - 1 ; /* (note leaps is negative here and year 0 (1 BCE) was a leap year */
		if(year%4 == 0)
			MJD->base_day = year * 365 + leaps + MonthStartDOY_L[month-1] + day - 678943;
		else
			MJD->base_day = year * 365 + leaps + MonthStartDOY[month-1] + day - 678943;

	}
	else if(year < 1582 || (year == 1582 && month < 10) || (year == 1582 && month == 10 && day < 15) || forceJulian)
	{
		/* count leap years on Julian Calendar */
		/* MJD for Jan 1 0000 (correctly Jan 01, BCE 1) is  - 678943, count from there */
	
		leaps = (year -1 ) / 4;
		if(year%4 == 0)
			MJD->base_day = year * 365 + leaps + MonthStartDOY_L[month-1] + day - 678943;
		else
			MJD->base_day = year * 365 + leaps + MonthStartDOY[month-1] + day - 678943;
	}
	else
	{
		/* count leap years Gregorian Calendar - modern dates */
		/* Algorithm below for  17 Nov 1858 (0 MJD) gives */
		/* leaps = 450 and hence base_day of 678941, so subtract it to give MJD day  */
		
		lastyear = year - 1;
		leaps = lastyear / 4 - lastyear / 100 + lastyear / 400;
		if( (year%4 == 0 && year%100 != 0) || (year%4 == 0 && year%400 == 0) )
			MJD->base_day = year * 365 + leaps + MonthStartDOY_L[month-1] + day - 678941;
		else
			MJD->base_day = year * 365 + leaps + MonthStartDOY[month-1] + day - 678941;
	
	}	
		
	MJD->time_sec = sec + ( (double) min  +  (double) hour * 60. ) * 60.;

	if(MJD->time_sec >= SecInDay)
	{
		int extraDays = (int) (MJD->time_sec / SecInDay);
		MJD->base_day += extraDays;
		MJD->time_sec -= extraDays * SecInDay;
	}
	
	return 0;
}

const char * getDayOfWeek( const MJDtime *MJD)
{
	static char *dow = {"Wed\0Thu\0Fri\0Sat\0Sun\0Mon\0Tue"};
	int d = MJD->base_day % 7;
	return &(dow[d*4]);
}

const char * getLongDayOfWeek( const MJDtime *MJD)
{
	static char *dow = {"Wednesday\0Thursday\0\0Friday\0\0\0\0Saturday\0\0Sunday\0\0\0\0Monday\0\0\0\0Tuesday"};
	int d = MJD->base_day % 7;
	return &(dow[d*10]);
}

const char * getMonth( int m)
{
	static char *months = {"Jan\0Feb\0Mar\0Apr\0May\0Jun\0Jul\0Aug\0Sep\0Oct\0Nov\0Dec"};
	return &(months[(m-1)*4]);
}

const char * getLongMonth( int m)
{
	static char *months = {"January\0\0\0February\0\0March\0\0\0\0\0April\0\0\0\0\0May\0\0\0\0\0\0\0June\0\0\0\0\0\0July\0\0\0\0\0\0August\0\0\0\0September\0October\0\0\0November\0\0December"};
	return &(months[(m-1)*10]);
}


int getDOY(const MJDtime *MJD, int forceJulian)
{	
	/* Get from Day Of Year  */
	int doy, year;
	
	int extra_days,j,lastyear;
	
	if(MJD->time_sec >= 0)
	{
		extra_days  = (int) (MJD->time_sec / SecInDay);
	}
	else
	{
		/* allow for negative seconds push into previous day even if less than 1 day */
		extra_days = (int) (MJD->time_sec / SecInDay) - 1 ;
	}
	

	j = MJD->base_day + extra_days;
	
	if( j <= -678943) {
	
		/* BCE dates */

		j += 678943;
		if( j > 0)
		{
			/* must be in year BCE 1 (CE year 0) */
			year = 0;
			/* subtract nothing from j */
		}
		else
		{
			/* negative years */
			year = (int) ((float)j / 365.25) -1;
			doy = j - (int)(year  * 365.25);
		}
		
	}
	else if( j < -100840 || forceJulian == 1)
	{
		/* Julian Dates */
		 j += 678943;
		
		 year = (int) ((float)j / 365.25);
	
		 doy = j - (int)(year * 365.25);
		
	}
	else
	{
		/* Gregorian Dates */
		j += 678941;
				
		 year = (int) ((float)j / 365.2425);
		 lastyear = year - 1;
		 doy = j - year * 365 - lastyear / 4 + lastyear / 100 - lastyear / 400;
		
	}

	return doy;	
}

void breakDownMJD(int *year, int *month, int *day, int *hour, int *min, double *sec, const MJDtime *MJD, int forceJulian)
{ 	
	/* Convert MJD struct into date/time elements */
	/* Note year 0 CE (AD) [1 BCE (BC)] is a leap year */
	/* There are 678943 days from year 0 to MJD(0)   */
	
        int extra_days,j,lastyear;
        double seconds;
	
	if(MJD->time_sec >= 0)
	{
		extra_days  = (int) (MJD->time_sec / SecInDay);
	}
	else
	{
		/* allow for negative seconds push into previous day even if less than 1 day */
		extra_days = (int) (MJD->time_sec / SecInDay) - 1 ;
	}


	j = MJD->base_day + extra_days;
	
	if( j <= -678943) {
	
		/* BCE dates */

		j += 678943;
		if( j > 0)
		{
			/* must be in year BCE 1 (CE year 0) */
			*year = 0;
			/* subtract nothing from j */
		}
		else
		{
			/* negative years */
			*year = (int) ((float)j / 365.25) -1;
			j = j - (int)((*year)  * 365.25);
		}
		
		 /* j is now always positive */
		 *month = 0;
		
		 if(*year%4 != 0)
		 {
			while(j > MonthStartDOY[*month])
			{
				(*month)++;
				if(*month == 12) break;
			}
			*day = j - MonthStartDOY[*month -1];
		 }
		 else
		 {
			/* put this year's leap day back as it is done here */
			j++;
			while(j > MonthStartDOY_L[*month])
			{
				(*month)++;
				if(*month == 12) break;
			}
			*day = j - MonthStartDOY_L[*month -1];
		 }
	}
	else if( j < -100840 || forceJulian == 1)
	{
		/* Julian Dates */
		 j += 678943;
		
		 *year = (int) ((float)j / 365.25);
	
		 j = j - (int)(*year * 365.25);
		
		 *month = 0;
		 if(*year%4 != 0)
		 {
			while(j > MonthStartDOY[*month])
			{
				(*month)++;
				if(*month == 12) break;
			}
			*day = j - MonthStartDOY[*month -1];
		 }
		 else
		 {
			/* put leap day back for this year as done here */
			j++;
			while(j > MonthStartDOY_L[*month])
			{
				(*month)++;
				if(*month == 12) break;
			}
			*day = j - MonthStartDOY_L[*month -1];
		 }
	}
	else
	{
		/* Gregorian Dates */
		j += 678941;
				
		 *year = (int) ((float)j / 365.2425);
		 lastyear = *year - 1;
		 j = j - *year * 365 - lastyear / 4 + lastyear / 100 - lastyear / 400;
		
		 *month = 0;
		 if((*year%4 == 0 && *year%100 != 0) || (*year%4 == 0 && *year%400 == 0) )
		 {
			while(j > MonthStartDOY_L[*month])
			{
				(*month)++;
				if(*month == 12) break;
			}
			*day = j - MonthStartDOY_L[*month -1];
		 }
		 else
		 {
			while(j > MonthStartDOY[*month])
			{
				(*month)++;
				if(*month == 12) break;
			}
			*day = j - MonthStartDOY[*month -1];
		 }

	}

	/* Time part */
	
	seconds = MJD->time_sec - extra_days * SecInDay;
	*hour = (int)( seconds / 3600.);
	seconds -= (double) *hour * 3600.;
	*min = (int) ( seconds / 60.);
	*sec =  seconds - (double) *min * 60.;
}

size_t strfMJD(char * buf, size_t len, const char *format, const MJDtime *MJD, int forceJulian)
{
	/* Format a text string according to the format string.
	   Uses the same syntax as strftime() but does not use current locale.
	   The null terminator is included in len for safety. */
	
        int year, month, day, hour, min, ysign, sec1, second,d,y;
	int i, count,secsSince1970;
	int nplaces,fmtlen,slen;
	char * ptr;
	double sec,sec_fraction;
	int w,doy,days_in_wk1;
	const char *dayText;
	const char *monthText;
	char DateTime[80];
	size_t posn = 0;
	size_t last = len -1;
	buf[last] = '\0';
	buf[0] = '\0'; /* force overwrite of old buffer since strnctat() used hereafter */
	
	breakDownMJD(&year, &month, &day, &hour, &min, &sec,  MJD, forceJulian);
	if(year < 0)
	{
		ysign = 1;
		year =- year;
	}
	else ysign = 0;	
	
	second = (int) sec;
	sec1 = (int)sec/10;
	sec -= (double) sec1*10;
	
	/* Read format string, character at a time */
	fmtlen = strlen(format);
	i=0;
	while(i<fmtlen)
	{
		char next = format[i];
		if( next == '%')
		{
			/* format character or escape */
			i++;
			next =  format[i];
			if(next == '%')
			{
				/* escaped %, pass it on */
				buf[posn] = next;
				posn++;
				if(posn >= last) return posn;
			}
			else if(next == 'a')
			{
				/* short day name */
				dayText = getDayOfWeek(MJD);
				strncat(&(buf[posn]), dayText, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'A')
			{
				/* long day name */
				dayText = getLongDayOfWeek(MJD);
				strncat(&(buf[posn]), dayText, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'b' || next == 'h')
			{
				/* short month name */
				monthText = getMonth(month);
				strncat(&(buf[posn]), monthText, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'B')
			{
				/* long month name */
				monthText = getLongMonth(month);
				strncat(&(buf[posn]), monthText, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'c')
			{
				/* Date and Time with day of week */
				dayText = getDayOfWeek(MJD);
				monthText = getMonth(month);
				if(ysign == 0)
					sprintf(DateTime, "%s %s %02d %02d:%02d:%02d %04d", dayText, monthText, day, hour, min, second, year );
				else
					sprintf(DateTime, "%s %s %02d %02d:%02d:%02d -%04d", dayText, monthText, day, hour, min, second, year );
					
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'C')
			{
				/*  year / 100 so, e.g. 1989 is 20th century but comes out as 19 */
				int century = year / 100;
				if(ysign == 0)
					sprintf(DateTime, "%02d", century );
				else
					sprintf(DateTime, "-%02d", century+1 );
					
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'd')
			{
				/* day of month (01 - 31) */
				sprintf(DateTime, "%02d", day);
					
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'D')
			{
				/* month/day/year */
				int y = year %100;
				if(ysign == 0)
					sprintf(DateTime, "%02d/%02d/%02d", month, day, y );
				else
					sprintf(DateTime, "%02d/%02d/-%02d", month, day, y );
					
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'e')
			{
				/* day of month ( 1 - 31) */
				if(day < 10)
					sprintf(DateTime, " %01d", day);
				else
					sprintf(DateTime, "%02d", day);
					
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'F')
			{
				/* year-month-day */
				if(ysign == 0)
					sprintf(DateTime, "%04d-%02d-%02d", year, month, day );
				else
					sprintf(DateTime, "-%04d-%02d-%02d", year, month, day );
					
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'H')
			{
				/* hour, 24 hour clock (00 - 23) */
				sprintf(DateTime, "%02d", hour);
					
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'I')
			{
				/* hour, 12 hour clock (01 - 12) */
				if(hour == 0) sprintf(DateTime, "%02d", hour+12);
				else if(hour > 12) 	 sprintf(DateTime, "%02d", hour-12);
				else  sprintf(DateTime, "%02d", hour);
				
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'j')
			{
				/* day of year */
				int doy = getDOY(MJD, forceJulian);
				sprintf(DateTime, "%03d", doy);
				
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'k')
			{
				/* hour, 24 hour clock ( 0 - 23) */
				if(hour < 10)
					sprintf(DateTime, " %01d", hour);
				else
					sprintf(DateTime, "%02d", hour);
					
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'l')
			{
				/* hour, 12 hour clock ( 1 - 12) */
				if(hour == 0) sprintf(DateTime, "%02d", hour+12);
				else if(hour < 10) sprintf(DateTime, " %01d", hour);
				else if(hour <= 12) sprintf(DateTime, "%02d", hour);
				else if(hour < 22)  sprintf(DateTime, " %01d", hour-12);
				else sprintf(DateTime, "%02d", hour-12);
				
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'm')
			{
				/* month (01 - 12) */
				sprintf(DateTime, "%02d", month);
				
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'M')
			{
				/* minute (00 - 59) */
				sprintf(DateTime, "%02d", min);
				
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'n')
			{
				/*  newline */
				buf[posn] = '\n';
				posn++;
				if(posn >= last) return posn;
			}
			else if(next == 'p')
			{
				/* am/pm on12 hour clock  */
				if(hour < 0) sprintf(DateTime, "AM");
				else  sprintf(DateTime, "PM");
				
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'r')
			{
					/* hour:min:sec AM, 12 hour clock (01 - 12):(00 - 59):(00 - 59) (AM - PM) */
				if(hour == 0) sprintf(DateTime, "%02d:%02d:%02d AM", hour+12, min, second);
				else if(hour > 12) 	 sprintf(DateTime, "%02d:%02d:%02d PM", hour-12, min, second);
				else if(hour == 12)  sprintf(DateTime, "%02d:%02d:%02d PM", hour, min, second);
				else  sprintf(DateTime, "%02d:%02d:%02d AM", hour, min, second);
				
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'R')
			{
				/* hour:min, 24 hour clock (00 - 23):(00 - 59) */
				sprintf(DateTime, "%02d:%02d", hour, min);
					
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'S')
			{
				/* second (00 - 59) */
				sprintf(DateTime, "%02d", second);
				
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 's')
			{
				/* seconds since 01 Jan 1970 Gregorian */
			        secsSince1970 = (int)(MJD->time_sec + (MJD->base_day - MJD_1970) * SecInDay);
				sprintf(DateTime, "%d", secsSince1970);
				
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 't')
			{
				/*  tab */
				buf[posn] = '\t';
				posn++;
				if(posn >= last) return posn;
			}
			else if(next == 'T')
			{
				/* hour:min:sec, 24 hour clock (00 - 23):(00 - 59):(00 - 59) */
				sprintf(DateTime, "%02d:%02d:%02d", hour, min, second);
					
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'U')
			{
				/* week of year as a number,  (00 - 53) start of week is Sunday */
				doy = getDOY(MJD, forceJulian);
				days_in_wk1 = (MJD->base_day - doy - 4) % 7;
				
				w = (doy + 6 - days_in_wk1) / 7;
				
				sprintf(DateTime, "%02d", w);
				
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'u')
			{
				/* weekday as a number,  0 = Monday */
				d = 1 + (MJD->base_day - 5) % 7;

				sprintf(DateTime, "%01d", d);
				
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'v')
			{
				/* day-MonthName-year day of month ( 1 - 31) - month (Jan ... Dec) - year (yyyy) */
				
				monthText = getMonth(month);
				
				if(ysign == 0)
				{
					if(day < 10)
						sprintf(DateTime, " %01d-%s-%04d", day, monthText, year);
					else
						sprintf(DateTime, "%02d-%s-%04d", day, monthText, year);
				}
				else
				{
					if(day < 10)
						sprintf(DateTime, " %01d-%s-(-)%04d", day, monthText, year);
					else
						sprintf(DateTime, "%02d-%s-(-)%04d", day, monthText, year);
				}
				
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'V')
			{
				/* week of year as a number,  (01 - 53) start of week is Monday and first week has at least 3 days in year */
				int doy = getDOY(MJD, forceJulian);
				int days_in_wk1 = (MJD->base_day - doy - 3) % 7;
				
				if(days_in_wk1 <= 3) w = (doy +6 - days_in_wk1) / 7; /* ensure first week has at least 3 days in this year */
				else w = 1 + (doy + 6 - days_in_wk1) / 7;
				
				if(w == 0) w = 53;
				sprintf(DateTime, "%02d", w);
				
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'w')
			{
				/* weekday as a number,  0 = Sunday */
				d = (MJD->base_day - 4) % 7;

				sprintf(DateTime, "%01d", d);
				
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'W')
			{
				/* week of year as a number,  (00 - 53) start of week is Monday */
				doy = getDOY(MJD, forceJulian);
				days_in_wk1 = (MJD->base_day - doy - 3) % 7;
				
				w =  (doy +6 - days_in_wk1) / 7;
				
				sprintf(DateTime, "%02d", w);
				
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'x')
			{
				/* date string */
				dayText = getDayOfWeek(MJD);
				monthText = getMonth(month);
				if(ysign == 0)
					sprintf(DateTime, "%s %s %02d, %04d", dayText, monthText, day, year );
				else
					sprintf(DateTime, "%s %s %02d, -%04d", dayText, monthText, day, year );
					
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'X')
			{
				/* time string */
				sprintf(DateTime, "%02d:%02d:%02d", hour, min, second );
						
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'y')
			{
				/* 2 digit year */
				y = year %100;
				
				if(ysign == 0)
					sprintf(DateTime, "%02d", y );
				else
					sprintf(DateTime, "-%02d", y );
				
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'Y')
			{
				/* 4 digit year */
				if(ysign == 0)
					sprintf(DateTime, "%04d", year );
				else
					sprintf(DateTime, "-%04d", year );
				
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'Z')
			{
				/* time zone and calendar, alwaus UTC */
				if(year < 1582 || (year == 1582 && month < 10) || (year == 1582 && month == 10 && day < 15) || forceJulian == 1)
					strncat(&(buf[posn]), "UTC Julian", last - posn);
				else
					strncat(&(buf[posn]), "UTC Gregorian", last - posn);
					
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == 'z')
			{
				/* time zone, always UTC */
				strncat(&(buf[posn]), "+0000", last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if(next == '+')
			{
				/* date and time */
				dayText = getDayOfWeek(MJD);
				monthText = getMonth(month);
				if(ysign == 0)
					sprintf(DateTime, "%s %s %02d %02d:%02d:%02d UTC %04d",  dayText, monthText, day, hour, min, second, year );
				else
					sprintf(DateTime, "%s %s %02d %02d:%02d:%02d UTC -%04d", dayText, monthText, day, hour, min, second, year );
					
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
			else if( isdigit(next) != 0 )
			{
				nplaces = strtol(&(format[i]), NULL, 10);
				/* numeric value is number of decimal places ( > 0 ) */
				sec_fraction = sec - (double) second;

				for(count=0; count<nplaces; count++) sec_fraction *= 10;
				sprintf(DateTime, ".%d",  (int) sec_fraction);
				
				/* append 0 to pad to length */
				slen = strlen(DateTime);
				while(slen < nplaces+1)
				{
					DateTime[slen] = '0';
					slen++;
					DateTime[slen] = '\0';
				}
				strncat(&(buf[posn]), DateTime, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}	
			else if( next == '.' )
			{
				/* fractional part of seconds to maximum available accuracy */
				sec_fraction = sec - (double) second;
				sprintf(DateTime, "%-11.9f",  sec_fraction);
				while( ( ptr = strrchr(&(DateTime[0]), ' ')) != NULL)  ptr[0] ='\0'; /* remove trailing white space */
				slen = strlen(DateTime) -1;
				while( DateTime[slen] == '0' && DateTime[slen-1] != '.') {
					DateTime[slen] ='\0'; /* remove trailing zeros */
					slen --;
				}
				
				ptr = strchr(DateTime, '.'); /* remove possible lead 0 */
				strncat(&(buf[posn]), ptr, last - posn);
				posn = strlen(buf);
				if(posn >= last) return posn;
			}
		}
		else
		{
			/* regular multi-byte character, pass it on */
			buf[posn] = next;
			posn++;
			if(posn >= last) return posn;
		}
		buf[posn] = '\0';
		i++;
	}
	return posn;
}
