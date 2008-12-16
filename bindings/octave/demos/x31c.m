## $Id$
##
##  Copyright (C) 2008 Alan W. Irwin
##  Copyright (C) 2008 Andrew Ross
##
##  set/get tester
##
##  This file is part of PLplot.
##
##  PLplot is free software; you can redistribute it and/or modify
##  it under the terms of the GNU General Library Public License as published
##  by the Free Software Foundation; either version 2 of the License, or
##  (at your option) any later version.
##  
##  PLplot is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU Library General Public License for more details.
##  
##  You should have received a copy of the GNU Library General Public License
##  along with PLplot; if not, write to the Free Software
##  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
##  

1;

function ret = ix31c

  if (!exist("plinit"))
    plplot_stub
  endif

  if(isempty(sprintf("%s",plgdev')))
    plsdev("xwin");
  endif

  PL_NOTSET = -42.0;

  r1 = [0, 255];
  g1 = [255, 0];
  b1 = [0, 0];
  a1 = [1.0, 1.0];

  ## Parse and process command line arguments

  ## (void) plparseopts(&argc, argv, PL_PARSE_FULL);

  ## Test setting / getting page size
  [xp, yp, xleng, yleng, xoff, yoff] = plgpage();
  xp2 = xp*0.9;
  yp2 = yp*0.9;
  xleng2 = floor(xleng*0.9);
  yleng2 = floor(yleng*0.9);
  plspage(xp2, yp2, xleng2, yleng2, xoff, yoff);
  [xp, yp, xleng, yleng, xoff, yoff] = plgpage();
  if (xp != xp2 || yp != yp2 || xleng != xleng2 || yleng != yleng2)
    fputs("plgpage test failed\n",stderr);
    plend();
    ret = 1; return;
  endif

  plscompression(1);
  compression = plgcompression();
  if (compression != 1) 
    fputs("plgcompression test failed\n",stderr);
    plend();
    ret = 1; return;
  endif

  [fam, num, bmax] = plgfam();
  plsfam(1,1,100000);
  [fam1, num1, bmax1] = plgfam();
  if (fam1 != 1 || num1 != 1 || bmax1 != 100000) 
    fputs("plgfam test failed\n",stderr);
    plend();
    ret = 1; return;
  endif
  plsfam(fam, num, bmax);

  ## Initialize plplot

  plinit();

  plscolor(1);

  plscol0(1, 255, 0, 0);

  plscmap1(r1',g1',b1');
  plscmap1a(r1',g1',b1',a1');
  
  level = plglevel();
  if (level != 1)
    fprintf(stderr,"plglevel test failed. Level is %d, but 1 expected.\n",level);
    plend();
    ret = 1; return;
  endif

  pladv(0);
  plvpor(0.0, 1.0, 0.0, 1.0);

  plwind(0.2, 0.3, 0.4, 0.5);
  [xmin, xmax, ymin, ymax] = plgvpw();
  if (xmin != 0.2 || xmax != 0.3 || ymin != 0.4 || ymax != 0.5)
    fputs("plgvpw test failed\n",stderr);
    plend();
    ret = 1; return;
  endif

  [xmin, xmax, ymin, ymax] = plgvpd();
  if (xmin != 0.0 || xmax != 1.0 || ymin != 0.0 || ymax != 1.0)
    fputs("plgvpd test failed\n",stderr);
    plend();
    ret = 1; return;
  endif

  ## Get world coordinates for 0.5,0.5 which is in the middle of 
  ## the window
  [wx,wy,win] = plcalc_world(0.5,0.5);
  if (abs(wx-0.25)>1.0E-5 || abs(wy-0.45)>1.0E-5)
    fputs("plcalc_world test failed\n",stderr);
    plend();
    ret = 1; return;    
  endif

  ## Retrieve and print the name of the output file (if any)
  fnam = plgfnam();
  printf("Output file name is %s\n",fnam);

  ## Set and get the number of digits used to display axis labels
  ## Note digits is currently ignored in pls[xyz]ax and 
  ## therefore it does not make sense to test the returned 
  ## value.
  plsxax(3,0);
  [digmax,digits] = plgxax();
  if (digmax != 3)
    fputs("plgxax test failed\n",stderr);
    plend();
    ret = 1; return;
  endif

  plsyax(3,0);
  [digmax,digits] = plgyax();
  if (digmax != 3)
    fputs("plgyax test failed\n",stderr);
    plend();
    ret = 1; return;
  endif

  plszax(3,0);
  [digmax,digits] = plgzax();
  if (digmax != 3)
    fputs("plgzax test failed\n",stderr);
    plend();
    ret = 1; return;
  endif

  plsdidev(0.05, PL_NOTSET, 0.0, 0.0);
  [mar, aspect, jx, jy] = plgdidev();
  if (mar != 0.05 || jx != 0.0 || jy != 0.0)
    fputs("plgdidev test failed\n",stderr);
    plend();
    ret = 1; return;
  endif

  plsdiori(1.0);
  ori = plgdiori();
  if (ori != 1.0)
    fputs("plgdiori test failed\n",stderr);
    plend();
    ret = 1; return;
  endif

  plsdiplt(0.1, 0.1, 0.9, 0.9);
  [xmin, ymin, xmax, ymax] = plgdiplt();
  if (xmin != 0.1 || xmax != 0.9 || ymin != 0.1 || ymax != 0.9)
    fputs("plgdiplt test failed\n",stderr);
    plend();
    ret = 1; return;
  endif

  plsdiplz(0.1, 0.1, 0.9, 0.9);
  [xmin, ymin, xmax, ymax] = plgdiplt();
  if (xmin != 0.1+0.8*0.1 || xmax != 0.1+0.8*0.9 || ymin != 0.1+0.8*0.1 || ymax != 0.1+0.8*0.9)
    fputs("plsdiplz test failed\n",stderr);
    plend();
    ret = 1; return;
  endif

  plscolbg(0,0,0);
  [r, g, b] = plgcolbg();
  if (r != 0 || g != 0 || b != 0)
    fputs("plgcolbg test failed\n",stderr);
    plend();
    ret = 1; return;
  endif

  plscolbga(0,0,0,1.0);
  [r, g, b, a] = plgcolbga();
  if (r != 0 || g != 0 || b != 0 || a != 1.0)
    fputs("plgcolbga test failed\n",stderr);
    plend();
    ret = 1; return;
  endif

  plend();
  ret = 0;

endfunction

ix31c
