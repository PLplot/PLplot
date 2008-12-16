// $Id:$
//
//   set/get tester
//
// Copyright (C) 2008 Andrew Ross
// Copyright (C) 2008 Alan Irwin
//
// This file is part of PLplot.
//
// PLplot is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Library Public License as published
// by the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// PLplot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with PLplot; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//

#include "plc++demos.h"

#ifdef USE_NAMESPACE
using namespace std;
#endif 

class x31 {
public:
  x31(int, const char**);

private:
  plstream *pls;
};

//--------------------------------------------------------------------------
// main
//
// Demonstrates absolute positioning of graphs on a page.
//--------------------------------------------------------------------------

x31::x31(int argc, const char *argv[])
{
  PLFLT xmin, xmax, ymin, ymax, wx, wy;
  PLFLT mar, aspect, jx, jy, ori;  
  PLINT win, level, digmax, digits, compression;
  PLFLT xp, yp, xp2, yp2;
  PLINT xleng, yleng, xoff, yoff, xleng2, yleng2;
  PLINT fam1, num1, bmax1, fam, num, bmax, r, g, b;
  PLFLT a;
  PLINT r1[] = {0, 255};
  PLINT g1[] = {255, 0};
  PLINT b1[] = {0, 0};
  PLFLT a1[] = {1.0, 1.0};
  char fnam[80];

  pls = new plstream();

  // Parse and process command line arguments

  pls->parseopts(&argc, argv, PL_PARSE_FULL);

  // Test setting / getting page size 
  pls->gpage(xp, yp, xleng, yleng, xoff, yoff);
  xp2 = xp*0.9;
  yp2 = yp*0.9;
  xleng2 = (PLINT)(xleng*0.9);
  yleng2 = (PLINT)(yleng*0.9);
  pls->spage(xp2, yp2, xleng2, yleng2, xoff, yoff);
  pls->gpage(xp, yp, xleng, yleng, xoff, yoff);
  if (xp != xp2 || yp != yp2 || xleng != xleng2 || yleng != yleng2) {
    fputs("plgpage test failed\n",stderr);
    delete pls;
    exit(1);
  }

  pls->scompression(1);
  pls->gcompression(compression);
  if (compression != 1) {
    fputs("plgcompression test failed\n",stderr);
    delete pls;
    exit(1);
  }

  // Test family functions - need to restore defaults to ensure 
  // that test scripts work ok.
  pls->gfam(fam, num, bmax);
  pls->sfam(1,1,100000);
  pls->gfam(fam1, num1, bmax1);
  if (fam1 != 1 || num1 != 1 || bmax1 != 100000) {
    fputs("plgfam test failed\n",stderr);
    delete pls;
    exit(1);
  }
  pls->sfam(fam,num,bmax);

  // Initialize plplot

  pls->init();

  pls->scolor(1);

  pls->scol0(1, 255, 0, 0);

  pls->scmap1(r1,g1,b1,2);
  pls->scmap1a(r1,g1,b1,a1,2);
  
  pls->glevel(level);
  if (level != 1) {
    fprintf(stderr,"plglevel test failed. Level is %d, but 1 expected.\n",level);
    delete pls;
    exit(1);
  }

  pls->adv(0);
  pls->vpor(0.0, 1.0, 0.0, 1.0);

  pls->wind(0.2, 0.3, 0.4, 0.5);
  pls->gvpw(xmin, xmax, ymin, ymax);
  if (xmin != 0.2 || xmax != 0.3 || ymin != 0.4 || ymax != 0.5) {
    fputs("plgvpw test failed\n",stderr);
    delete pls;
    exit(1);
  }

  pls->gvpd(xmin, xmax, ymin, ymax);
  if (xmin != 0.0 || xmax != 1.0 || ymin != 0.0 || ymax != 1.0) {
    fputs("plgvpd test failed\n",stderr);
    delete pls;
    exit(1);
  }

  // Get world coordinates for 0.5,0.5 which is in the middle of 
  // the window
  pls->calc_world(0.5,0.5,wx,wy,win);
  if (fabs(wx-0.25)>1.0E-5 || fabs(wy-0.45)>1.0E-5) {
    fputs("plcalc_world test failed\n",stderr);
    delete pls;
    exit(1);    
  }

  // Retrieve and print the name of the output file (if any)
  pls->gfnam(fnam);
  printf("Output file name is %s\n",fnam);

  // Set and get the number of digits used to display axis labels
  // Note digits is currently ignored in pls[xyz]ax and 
  // therefore it does not make sense to test the returned 
  // value
  pls->sxax(3,0);
  pls->gxax(digmax,digits);
  if (digmax != 3) {
    fputs("plgxax test failed\n",stderr);
    delete pls;
    exit(1);
  }

  pls->syax(3,0);
  pls->gyax(digmax,digits);
  if (digmax != 3) {
    fputs("plgyax test failed\n",stderr);
    delete pls;
    exit(1);
  }

  pls->szax(3,0);
  pls->gzax(digmax,digits);
  if (digmax != 3) {
    fputs("plgzax test failed\n",stderr);
    delete pls;
    exit(1);
  }

  pls->sdidev(0.05, PL_NOTSET, 0.0, 0.0);
  pls->gdidev(mar, aspect, jx, jy);
  if (mar != 0.05 || jx != 0.0 || jy != 0.0) {
    fputs("plgdidev test failed\n",stderr);
    delete pls;
    exit(1);
  }

  pls->sdiori(1.0);
  pls->gdiori(ori);
  if (ori != 1.0) {
    fputs("plgdiori test failed\n",stderr);
    delete pls;
    exit(1);
  }

  pls->sdiplt(0.1, 0.1, 0.9, 0.9);
  pls->gdiplt(xmin, ymin, xmax, ymax);
  if (xmin != 0.1 || xmax != 0.9 || ymin != 0.1 || ymax != 0.9) {
    fputs("plgdiplt test failed\n",stderr);
    delete pls;
    exit(1);
  }

  pls->sdiplz(0.1, 0.1, 0.9, 0.9);
  pls->gdiplt(xmin, ymin, xmax, ymax);
  if (xmin != 0.1+0.8*0.1 || xmax != 0.1+0.8*0.9 || ymin != 0.1+0.8*0.1 || ymax != 0.1+0.8*0.9) {
    fputs("plsdiplz test failed\n",stderr);
    delete pls;
    exit(1);
  }

  pls->scolbg(0,0,0);
  pls->gcolbg(r, g, b);
  if (r != 0 || g != 0 || b != 0) {
    fputs("plgcolbg test failed\n",stderr);
    delete pls;
    exit(1);
  }

  pls->scolbga(0,0,0,1.0);
  pls->gcolbga(r, g, b, a);
  if (r != 0 || g != 0 || b != 0 || a != 1.0) {
    fputs("plgcolbga test failed\n",stderr);
    delete pls;
    exit(1);
  }

  delete pls;
  exit(0);
}

int main( int argc, const char **argv )
{
  x31 *x = new x31( argc, argv );

  delete x;
}
