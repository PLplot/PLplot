/* $Id$

   set/get tester
*/

#include "plcdemos.h"

/*--------------------------------------------------------------------------*\
 * main
 *
 * Demonstrates absolute positioning of graphs on a page.
 \*--------------------------------------------------------------------------*/

int
main(int argc, const char *argv[])
{
  PLFLT xmin, xmax, ymin, ymax, wx, wy;
  PLFLT mar, aspect, jx, jy, ori;  
  PLINT win, level, digmax, digits, compression;
  PLFLT xp, yp, xp2, yp2;
  PLINT xleng, yleng, xoff, yoff, xleng2, yleng2;
  PLINT fam, num, bmax, r, g, b;
  PLFLT a;
  PLINT r1[] = {0, 255};
  PLINT g1[] = {255, 0};
  PLINT b1[] = {0, 0};
  PLFLT a1[] = {1.0, 1.0};
  char fnam[80];

  /* Parse and process command line arguments */

  (void) plparseopts(&argc, argv, PL_PARSE_FULL);

  /* Test setting / getting page size */
  plgpage(&xp, &yp, &xleng, &yleng, &xoff, &yoff);
  xp2 = xp*0.9;
  yp2 = yp*0.9;
  xleng2 = (PLINT)(xleng*0.9);
  yleng2 = (PLINT)(yleng*0.9);
  plspage(xp2, yp2, xleng2, yleng2, xoff, yoff);
  plgpage(&xp, &yp, &xleng, &yleng, &xoff, &yoff);
  if (xp != xp2 || yp != yp2 || xleng != xleng2 || yleng != yleng2) {
    fputs("plgpage test failed\n",stderr);
    plend();
    exit(1);
  }

  plscompression(1);
  plgcompression(&compression);
  if (compression != 1) {
    fputs("plgcompression test failed\n",stderr);
    plend();
    exit(1);
  }

  plsfam(1,1,100000);
  plgfam(&fam, &num, &bmax);
  if (fam != 1 || num != 1 || bmax != 100000) {
    fputs("plgfam test failed\n",stderr);
    plend();
    exit(1);
  }

  /* Initialize plplot */

  plinit();

  plscolor(1);

  plscol0(1, 255, 0, 0);

  plscmap1(r1,g1,b1,2);
  plscmap1a(r1,g1,b1,a1,2);
  
  plglevel(&level);
  if (level != 1) {
    fprintf(stderr,"plglevel test failed. Level is %d, but 1 expected.\n",level);
    plend();
    exit(1);
  }

  pladv(0);
  plvpor(0.0, 1.0, 0.0, 1.0);

  plwind(0.2, 0.3, 0.4, 0.5);
  plgvpw(&xmin, &xmax, &ymin, &ymax);
  if (xmin != 0.2 || xmax != 0.3 || ymin != 0.4 || ymax != 0.5) {
    fputs("plgvpw test failed\n",stderr);
    plend();
    exit(1);
  }

  plgvpd(&xmin, &xmax, &ymin, &ymax);
  if (xmin != 0.0 || xmax != 1.0 || ymin != 0.0 || ymax != 1.0) {
    fputs("plgvpd test failed\n",stderr);
    plend();
    exit(1);
  }

  /* Get world coordinates for 0.5,0.5 which is in the middle of 
   * the window */
  plcalc_world(0.5,0.5,&wx,&wy,&win);
  if (fabs(wx-0.25)>1.0E-5 || fabs(wy-0.45)>1.0E-5) {
    fputs("plcalc_world test failed\n",stderr);
    plend();
    exit(1);    
  }

  /* Retrieve and print the name of the output file (if any) */
  plgfnam(fnam);
  printf("Output file name is %s\n",fnam);

  /* Set and get the number of digits used to display axis labels */
  /* Note digits is currently ignored in pls[xyz]ax and 
     therefore it does not make sense to test the returned 
     value */
  plsxax(3,0);
  plgxax(&digmax,&digits);
  if (digmax != 3) {
    fputs("plgxax test failed\n",stderr);
    plend();
    exit(1);
  }

  plsyax(3,0);
  plgyax(&digmax,&digits);
  if (digmax != 3) {
    fputs("plgyax test failed\n",stderr);
    plend();
    exit(1);
  }

  plszax(3,0);
  plgzax(&digmax,&digits);
  if (digmax != 3) {
    fputs("plgzax test failed\n",stderr);
    plend();
    exit(1);
  }

  plsdidev(0.05, PL_NOTSET, 0.0, 0.0);
  plgdidev(&mar, &aspect, &jx, &jy);
  if (mar != 0.05 || jx != 0.0 || jy != 0.0) {
    fputs("plgdidev test failed\n",stderr);
    plend();
    exit(1);
  }

  plsdiori(1.0);
  plgdiori(&ori);
  if (ori != 1.0) {
    fputs("plgdiori test failed\n",stderr);
    plend();
    exit(1);
  }

  plsdiplt(0.1, 0.1, 0.9, 0.9);
  plgdiplt(&xmin, &ymin, &xmax, &ymax);
  if (xmin != 0.1 || xmax != 0.9 || ymin != 0.1 || ymax != 0.9) {
    fputs("plgdiplt test failed\n",stderr);
    plend();
    exit(1);
  }

  plsdiplz(0.1, 0.1, 0.9, 0.9);
  plgdiplt(&xmin, &ymin, &xmax, &ymax);
  if (xmin != 0.1+0.8*0.1 || xmax != 0.1+0.8*0.9 || ymin != 0.1+0.8*0.1 || ymax != 0.1+0.8*0.9) {
    fputs("plsdiplz test failed\n",stderr);
    plend();
    exit(1);
  }

  plscolbg(0,0,0);
  plgcolbg(&r, &g, &b);
  if (r != 0 || g != 0 || b != 0) {
    fputs("plgcolbg test failed\n",stderr);
    plend();
    exit(1);
  }

  plscolbga(0,0,0,1.0);
  plgcolbga(&r, &g, &b, &a);
  if (r != 0 || g != 0 || b != 0 || a != 1.0) {
    fputs("plgcolbga test failed\n",stderr);
    plend();
    exit(1);
  }

  plend();
  exit(0);
}
