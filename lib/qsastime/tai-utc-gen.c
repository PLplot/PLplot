/*  $Id$

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

/* Program for generating data structure used for containing tai-utc
   conversion information (linear transforms and leap seconds).

   The program assumes that argv[1] will be the input file, and
   argv[2] the output file.  This works cross-platform without
   worrying about shell redirects of stdin and stdout that are
   not accessible on Windows, apparently.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*--------------------------------------------------------------------------*\
 *   Function-like macro definitions
 \*--------------------------------------------------------------------------*/

#define MemError1(a) do {fprintf(stderr,"MEMORY ERROR %d\n" a "\n",__LINE__);exit(__LINE__);}while(0)

const char header[]=""\
  "/*\n"\
  "  This file is part of PLplot.\n"\
  "  \n"\
  "  PLplot is free software; you can redistribute it and/or modify\n"\
  "  it under the terms of the GNU General Library Public License as published\n"\
  "  by the Free Software Foundation; either version 2 of the License, or\n"\
  "  (at your option) any later version.\n"\
  "  \n"\
  "  PLplot is distributed in the hope that it will be useful,\n"\
  "  but WITHOUT ANY WARRANTY; without even the implied warranty of\n"\
  "  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"\
  "  GNU Library General Public License for more details.\n"\
  "  \n"\
  "  You should have received a copy of the GNU Library General Public License\n"\
  "  along with PLplot; if not, write to the Free Software\n"\
  "  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA\n"\
  "  \n"\
  "  \n"\
  "  This header file contains the table containing the linear transforms \n"\
  "  for converting between TAI and UTC.\n"\
  "  It is an automatically generated file, so please do\n"\
  "  not edit it directly. Make any changes to tai-utc.dat then use\n"\
  "  tai-utc-gen to recreate this header file.\n"\
  "  \n"\
  "  tai-utc.dat contains four essential fields to represent the following\n"\
  "  formula for the linear transformation between TAI and UTC: \n"\
  "    TAI-UTC (seconds) = offset1 + (MJD-offset2)*slope\n"\
  "  There are four essential fields per line in tai-utc.dat to represent\n"\
  "  this formula.  They are the Julian date (TAI) where the linear\n"\
  "  transformation implied by the line is first applied;\n"\
  "  offset1 (seconds); offset2 (days), and slope (secs/day).\n"\
  "  \n"\
  "*/";

int main (int argc, char *argv[])
{
  FILE *fr, *fw;
  char readbuffer[256];
  int *MJDstart=NULL;
  double *offset1=NULL;
  int *offset2=NULL;
  double *slope=NULL;
  int jd;
  int i=0;
  int number_of_lines=0;
  char d1[20], d2[20], d3[20], d4[20], d5[20];

  if ((fr=fopen(argv[1],"r"))==NULL) {
    fprintf(stderr, "Cannot open first file as readable\n");
    exit(1);
  }

  if ((fw=fopen(argv[2],"w"))==NULL) {
    fprintf(stderr, "Cannot open second file as writable\n");
    exit(1);
  }

  /*
   *   Work out how many lines we have all up
   */

  while((fgets(readbuffer,255,fr)!=NULL))
    {
      ++number_of_lines;
    }

  /*
   *   Allocate memory to the arrays which will hold the data
   */

  if ((MJDstart=(int *)calloc(number_of_lines, (size_t)sizeof(int)))==NULL)
    MemError1("Allocating memory to the MJDstart table");

  if ((offset1=(double *)calloc(number_of_lines, (size_t)sizeof(double)))==NULL)
    MemError1("Allocating memory to the offset1 table");

  if ((offset2=(int *)calloc(number_of_lines, (size_t)sizeof(int)))==NULL)
    MemError1("Allocating memory to the offset2 table");

  if ((slope=(double *)calloc(number_of_lines, (size_t)sizeof(double)))==NULL)
    MemError1("Allocating memory to the slope table");

  rewind(fr);   /* Go back to the start of the file */

  /*
   *    Read in line by line, and copy the numbers into our arrays
   */

  while((fgets(readbuffer,255,fr)!=NULL))
    {
      //      sscanf(readbuffer,"%*s %*s %*s %*s %*s %f %*s %f %*s %*s %*s %5d %*s %*s %f",&jd, (double *)&offset1[i], (int *)&offset2[i], (double *)&slope[i]);
      sscanf(readbuffer,"%*s %*s %*s %*s %d.5 %*s %lf %*s %*s %*s %*s %d.) X %lf", (int *) &jd, (double *)&offset1[i],(int *)&offset2[i], (double *)&slope[i]);
      /* Should be exact since all jd's in the file are integer+0.5 */
      MJDstart[i] = jd - 2400000;
      i++;
    }

  fclose(fr);

/*
 *   Write the data out to file ready to be included in our source
 */


  fprintf(fw, "%s\n",header);

  fprintf(fw, "const int number_of_entries_in_hershey_to_unicode_table=%d;\n\n",number_of_lines);

  fprintf(fw, "typedef struct {\n\tunsigned int Hershey;\n\tPLUNICODE Unicode;\n\tchar Font;\n} Hershey_to_Unicode_table;\n\n");
  fprintf(fw, "const Hershey_to_Unicode_table hershey_to_unicode_lookup_table[%d] = {\n",number_of_lines);

  for (i=0;i<number_of_lines;i++) {
    fprintf(fw,"{%d, %f, %d, %f}\n", (int)MJDstart[i], (double)offset1[i], (int)offset2[i], (double)slope[i]);
    }

  fprintf(fw,"\n};\n");

  fclose(fw);
  free(MJDstart);
  free(offset1);
  free(offset2);
  free(slope);

  return(0);
}

