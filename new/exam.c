Date: Wed, 1 Apr 92 18:54:19 GMT
From: <amr@egr.duke.edu>
Message-Id: <9204011854.AA05008@ferry.egr.duke.edu>
To: mjl@fusion.ph.utexas.edu
Subject: Hi

Hi Maurice,
  How are things?  I don't know if you have announced an official
release date for plplot yet, but I hope to send you a new
improved NeXT driver in the next couple of days.

  I'm also attaching a program that points out a bug in plbox3
(actually I guess that it is plxybx).  I haven't had a chance
to track it down.  Compile the program and run it and everything
looks fine, but recompile with CASE1 defined and the labels on
the x-axis disappear.  There is actually more than one problem
with the 3D axis labelling code, but I've managed to get around
most of them.  If you don't have time to look at this, don't
worry about it.  I don't need it.  I just wanted to warn you
that there are some bugs that need to be fixed at some time.

Tony

p.s. The program below is a stand-alone version of 
program I use to plot some data computed by another
program.  I cut a lot of stuff out of the original
program, but enough remains to illustrate the problem.

===========================================================

/* This program plots the output of the RDW1S program. */
/* The plplot library is used. */

#include <stdio.h>
#include <math.h>
#include "plplot.h"

#define  MAXLINE     100
/* Range and Depth "resolution" */
#define  RANGERES    500.
#define  DEPTHRES       6.

#define max(a,b) 		(a<b ? b : a)
#define min(a,b) 		(a>b ? b : a)

int main(int argc, char **argv)
{
   float SrcDepInit, SrcDepIncr;
   int NumSrcDeps;
   float RangeInit, RangeIncr;
   int NumRanges;
   float EleDepMin, EleDepIncr;
   int NumElements;
   int NumW1;
   float W1init, W1incr;
   float W1act;
   float frequency, factor, DFactorDB;
   int Noise;
   int NumBins;
   int ADepPt, ARanPt, binw;
   int rr, dr, ni, nj;
   int i, j, l, pltfile;
   int popen1, popen2;
   int matdim[2];
   float *x, *y, **z, **znorm, norm;
   float *nx, *ny, **nz;
   float RangeMaxProb, ADepMaxProb, MaxProb;
   float linpos, delpos;
   char line[MAXLINE], FileName[MAXLINE];
   char command[MAXLINE];
   float Rinit1, Rinit2;
   int NumR1, NumR2, NumB1, NumB2;
   FILE *Infile, *Normfile;
   FILE *Infile1, *Infile2;
   FILE *popen();

   /* Check for range or depth reduction and double file normalization */
   popen1 = 0; popen2 = 0;
   rr = 2; dr = 2;

   pltfile = 1;
  
   Infile = Infile1; Normfile = Infile2;
  
   Rinit2 = 175000.;
   NumR2 = 3000;

   SrcDepInit = 0.; SrcDepIncr = 6.; NumSrcDeps = 100;
   RangeInit = 100000.; RangeIncr = 25.; NumRanges = 3000;
   W1init = 0.; W1incr = 0.; NumW1 = 1;
   EleDepMin = 400.; EleDepIncr = 7.5; NumElements = 120;
   frequency = 200.; DFactorDB = 100.; Noise = 0;
   ADepPt = 51; ARanPt = 2501;
   W1act = 0.; binw = 20;

   NumR1 = NumRanges;
   Rinit1 = RangeInit;
   NumB1 = NumR1/binw;
   NumB2 = NumR2/binw;
   NumRanges += NumR1;

   NumBins = NumRanges/binw;

   matdim[0] = NumSrcDeps;
   matdim[1] = NumBins;
   x = (float *)matrix_alloc(sizeof(float),1,&matdim[1]);
   y = (float *)matrix_alloc(sizeof(float),1,&matdim[0]);
   z = (float **)matrix_alloc(sizeof(float),2,matdim);

     matdim[0] = NumSrcDeps/dr;
     matdim[1] = NumBins/rr;
     nx = (float *)matrix_alloc(sizeof(float),1,&matdim[1]);
     ny = (float *)matrix_alloc(sizeof(float),1,&matdim[0]);
     nz = (float **)matrix_alloc(sizeof(float),2,matdim);

   for(i=0; i<NumBins; i++)
      x[i] = (RangeInit+i*binw*RangeIncr)/1000.;

   for(j=0; j<NumSrcDeps; j++)
      y[j] = SrcDepInit+j*SrcDepIncr;

     for(i=0; i<NumBins/rr; i++)
        nx[i] = x[i*rr];

     for(j=0; j<NumSrcDeps/dr; j++)
        ny[j] = y[j*dr];

   norm = -10000.;
      for(j=0; j<NumSrcDeps; j++) {
         for(i=0; i<NumB1; i++) {
            z[j][i] = fabs((NumSrcDeps/2 -j)*(NumBins/2 - i));
            norm = max(norm,z[j][i]);
         }
      }
      for(j=0; j<NumSrcDeps; j++) {
         for(i=NumB1; i<NumBins; i++) {
            z[j][i] = fabs((NumSrcDeps/2 -j)*(NumBins/2 - i));
            norm = max(norm,z[j][i]);
         }
      }

   for(i=0; i<NumBins; i++) {
      for(j=0; j<NumSrcDeps; j++) {
         z[j][i] = z[j][i]/(4.*norm);
      }
   }

   for(i=0; i<NumBins/rr; i++) {
      for(j=0; j<NumSrcDeps/dr; j++) {
         nz[j][i] = 0.;
         for(ni=0; ni<rr; ni++) {
            for(nj=0; nj<dr; nj++) {
               nz[j][i] += z[j*dr+nj][i*rr+ni];
            }
         }
         nz[j][i] = 0.6*fabs(nz[j][i]-1.);
      }
   }

   plstart("xwin",1,1);
   pladv(0);
   plvpor((PLFLT)0.15,(PLFLT)0.9,(PLFLT)0.3,(PLFLT)0.9);
   plwind((PLFLT)-6.,(PLFLT)6.,(PLFLT)-2.5,(PLFLT)7.5);
   plschr((PLFLT)0.,(PLFLT).7);
   plsmin((PLFLT)0.,(PLFLT).6);
   plsmaj((PLFLT)0.,(PLFLT).6);
   plw3d((PLFLT)7.,(PLFLT)7.,(PLFLT)5.,(PLFLT)0.,
         (PLFLT)600.,(PLFLT)100.,(PLFLT)250.,
         (PLFLT)0.,(PLFLT)1.,(PLFLT)20.,(PLFLT)-110.);
   plwid(3);
#ifdef CASE1
   plbox3("binstu","Depth (m)",(PLFLT)150.,0,"binstu","Range (km)",(PLFLT)25.,0,
          "bcdntu","Loc. Prob.",(PLFLT)0.0,0);
#else
   plbox3("binstu","Depth (m)",(PLFLT)0.,0,"binstu","Range (km)",(PLFLT)25.,0,
          "bcdntu","Loc. Prob.",(PLFLT)0.0,0);
#endif
   plwid(1);
   plot3d(ny,nx,nz,NumSrcDeps/dr,NumBins/rr,3,1);
   plend();
}
