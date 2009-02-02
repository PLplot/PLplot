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
 #include <MJDtime.h>
 #include <time.h>
 #include <stdio.h>
 #include <stdlib.h>
	/* Test program to run MJDtime */


int main()
{
	char buf[360];
	char copy[360];
	int y = 2004;
	int m = 1;
	int d = 23;
	int hour = 13;
	int min =39;
	double sec = 2.345678901;
	const char *ISOstring;
	double epoch;
	int doy;
	double zeroTest;
	
	MJDtime MJD1;
	MJDtime MJD2;
	
	
	printf("Start date/time components: %d-%d-%d %d:%d:%13.11g\n", y, m, d, hour, min, sec);
	
	setFromUT(y, m, d, hour, min, sec, &MJD2, 0);
	
	breakDownMJD(&y,&m,&d,&hour,&min,&sec, &MJD2, 0);
	printf("date/time components: %d-%d-%d %d:%d:%13.11g\n\n", y, m, d, hour, min, sec );
	
	printf("MJD = %d, seconds = %17.15g\n", MJD2.base_day, MJD2.time_sec);
	printf( " MJD = %18.10f \n", getMJD(&MJD2));
	printf( " JD = %18.10f \n\n", getJD(&MJD2));
	
	ISOstring = getISOString(&MJD2, 0);
	printf("ISO string = '%s'\n\n" , ISOstring);

	strfMJD(&(buf[0]), 360, "%Y-%m-%d %H:%M:%S%.",  &MJD2, 0);
	printf("strfMJD:ISO equiv:   '%s'\n", buf);
	strfMJD(&(buf[0]), 360, "%Y-%m-%dT%H:%M:%S%4Z",  &MJD2, 0);
	printf("strfMJD:ISO equiv:  '%s'\n", buf);
	
	doy = getDOY(&MJD2, 0);
	setFromDOY(y, doy, hour, min, sec, &MJD1, 0);
	
	zeroTest = getDiffSecs(&MJD1, &MJD2);
	printf("difference MJD (month/day) - MJD(doy) '%g'\n\n" , zeroTest);

	hour += 24;
	d -= 1;
	setFromUT(y, m, d, hour, min, sec, &MJD2, 0);
	zeroTest = getDiffDays(&MJD1, &MJD2);
	printf("difference MJD (d-1, h+24) - MJD(d, h) '%g'\n\n" , zeroTest);
	
	epoch = getCDFepoch(&MJD2);
	printf("CDF epoch sec %18.3f\n", epoch);
	setFromCDFepoch(epoch, &MJD2);
	printf("from CDF ISO string (CDF epoch is accurate to msec only) = '%s'\n" , getISOString(&MJD2, 1));
    
	printf("Day of week is/was %s\n\n", getDayOfWeek(&MJD2));
	
	ISOstring = getISOString(&MJD1,0);
	printf("ISO string = '%s'\n\n" , ISOstring);
	setFromISOstring(ISOstring, &MJD1);
	printf("for %s, MJD = %d, seconds = %17.11g\n", ISOstring, MJD1.base_day, MJD1.time_sec);
	ISOstring = getISOString(&MJD1,1);
	printf("ISO string = '%s'\n\n" , ISOstring);
	setFromISOstring(ISOstring, &MJD1);
	printf("for %s, MJD = %d, seconds = %17.11g\n\n", ISOstring, MJD1.base_day, MJD1.time_sec);

/* try julian/gregorian changeover */
	y = 1752;
	m = 9;
	d = 15;
	hour = 0;
	
	setFromUT(y, m, d, hour, min, sec, &MJD1, 0);
	strcpy(&(copy[0]), getISOString(&MJD1,0)); /* copy because getISOString() returns a pointer to a static string */
	printf("Gregorian = '%s'\n" , &(copy[0]));
	setFromUT(y, m, d, hour, min, sec, &MJD1, 1); /* set from Julian date */
	printf("%s Julian = '%s' Gregorian, (give us back our 11 days)\n" ,getISOString(&MJD1,1), &(copy[0]));


/* Compare formatting from strftime() */


	size_t used = strfMJD(&(buf[0]), 360, "   strfMJD():\n   --------\n '%+' \n %c\n %D %F \n %j \n %r \n %s \n %v\n\n",  &MJD2, 0);
	printf("chars %d for \n%s\n" , used, buf);

	/* seconds since 01 Jan 1970 Gregorian for strftime use */				
	time_t localt = MJD2.time_sec + (MJD2.base_day - 40587) * 86400;
	struct tm *ptm;
	ptm = gmtime(&localt);
	strftime(&(buf[0]), 360, "  strftime(): (invalid before 1970)\n   ------\n '%+' \n %c\n %D %F \n %j \n %r \n %s \n %v", ptm);
	printf("%s\n" , buf);

}

