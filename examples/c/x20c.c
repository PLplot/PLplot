/* $Id$

	plimage demo

*/

#include "plplot/plcdemos.h"
#include "plplot/plevent.h"
#include <math.h>

#define XDIM 160
#define YDIM 120

int
main(int argc, char *argv[])
{
  PLFLT x[XDIM], y[YDIM], **z, **r;
  int i, j, st;
  static PLGraphicsIn gin;
  PLFLT xi, yi, xe, ye, start;
    
  FILE *fp;
  int width, height, num_col;
  char ver[80];
  unsigned char *img;
  PLFLT **img_f;

  PLINT n_col = 255;
  PLINT rr[255], gg[255], bb[255];

  /*
    Bugs in plimage():
     -if the plot window is not totally visible, plimage() fails!
       (creating an image instead of retrieving the existing one will
        obliterate any plot already made, so this is no solution)
     -at high magnifications, the left and right edge are ragged, try
        ./x20c -dev xwin -wplt 0.3,0.3,0.6,0.6 -ori 0.5
     
    Bugs in x20c.c:
     -if the window is resized after a selection is made on "lena", when
      making a new selection the old one will re-appear.
     -sometimes the selected area will be twisted after the selection finishes!?!
  */

  /* Parse and process command line arguments */

  plParseOpts(&argc, argv, PL_PARSE_FULL);

  /* Initialize plplot */

  plinit();

  plAlloc2dGrid(&z, XDIM, YDIM);
  plAlloc2dGrid(&r, XDIM, YDIM);

  if (0) { /* view image border pixels */
    plenv(1., (PLFLT) XDIM, 1., (PLFLT) YDIM, 1, -2); /* no plot box */

    /* build a square -- diagnostics */
    for (i=0; i<XDIM; i++)
      z[i][YDIM-1] = 1.; /* right */
    for (i=0; i<XDIM; i++)
      z[i][0] = 1.; /* left */
  
    for (i=0; i<YDIM; i++)
      z[0][i] = 1.; /* top */
    for (i=0; i<YDIM; i++)
      z[XDIM-1][i] = 1.; /* botton */

    pllab("...around a blue square."," ","A red border should appear...");
    plimage(z, XDIM, YDIM,
	    1., (PLFLT) XDIM, 1., (PLFLT) YDIM,
	    1., (PLFLT) XDIM, 1., (PLFLT) YDIM);
    pladv(0);
  }

  plcol0(2); /* draw a yellow plot box, useful for diagnostics! :( */
  plenv(0., 2.*PI, 0, 3.*PI, 1, 0);

  for (i=0; i<XDIM; i++)
    x[i] = i*2.*PI/(XDIM-1);
  for (i=0; i<YDIM; i++)
    y[i] = i*3.*PI/(YDIM-1);

  for (i=0; i<XDIM; i++)
    for (j=0; j<YDIM; j++) {
      r[i][j] = sqrt(x[i]*x[i]+y[j]*y[j])+1e-3;
      z[i][j] = sin(r[i][j]) / (r[i][j]);
    }

  plimage(z, XDIM, YDIM, 0., 2.*PI, 0, 3.*PI, 0., 2.*PI, 0, 3.*PI); 

  pladv(0);
  plFree2dGrid(z, XDIM, YDIM);
  plFree2dGrid(r, XDIM, YDIM);

  /* set gray colormap */
  for (i=0; i<=n_col; i++)
    rr[i] = gg[i] = bb[i] = i;

  plscmap1(rr, gg, bb, n_col);

  /* naive grayscale binary ppm reading. If you know how to, improve it */
  fp = fopen("lena.pgm","rb");
  fscanf(fp,"%s\n", ver); /* version */
  /* printf("version: %s\n", ver);*/

  while((i=fgetc(fp)) == '#') {
    fgets(ver, 80, fp); /* comments */
    /* printf("%s", ver); */
  }
  ungetc(i, fp);

  fscanf(fp,"%d%d%d", &width, &height, &num_col); /* width, height num colors */
  /* printf("width=%d height=%d\n", width, height); */

  img = (unsigned char *) malloc(width*height*sizeof(char));
  plAlloc2dGrid(&img_f, width, height);

  fread(img, sizeof(char), width*height, fp);
  fclose(fp);

  for (i=0; i<width; i++)
    for (j=0; j<height; j++)
      img_f[i][j] = img[(height-j)*width+i];

  plenv(1., width, 1., height, 1, -1);
  pllab("Set and drag Button 1 to (re)set selection, Button 2 to finish."," ","Lena...");
  plimage(img_f, width, height, 1., width, 1., height, 1., width, 1., height);

  xi = 200.; xe = 330.;
  yi = 280.; ye = 220.;

  plxormod(1, &st); /* enter xor mode to draw a selection rectangle */
  if (st) { /* driver has xormod capability */
    start = 0;

    while(1) {
      PLFLT sx[5], sy[5];

      plxormod(0, &st);
      plGetCursor(&gin);
      plxormod(1, &st);

      if (gin.button == 1) {
	xi = gin.wX; yi = gin.wY;
	if (start)
	  plline(5, sx, sy); /* clear previous rectangle */

	start = 0;

	sx[0] = xi; sy[0] = yi;
	sx[4] = xi; sy[4] = yi;
      }

      if (gin.state && 0x100) {
	xe = gin.wX; ye = gin.wY;
	if (start)
	  plline(5, sx, sy); /* clear previous rectangle */

	start = 1;
  
	sx[2] = xe; sy[2] = ye;
	sx[1] = xe; sy[1] = yi;
	sx[3] = xi; sy[3] = ye;
	plline(5, sx, sy); /* draw new rectangle */
      }

      if (gin.button == 3 || gin.keysym == PLK_Return || gin.keysym == 'Q') {
	if (start)
	  plline(5, sx, sy); /* clear previous rectangle */
	break;
      }      
    }
    plxormod(0, &st); /* leave xor mod */

    if (xe < xi) {
      PLFLT t;
      t=xi; xi=xe; xe=t;
    }

    if (yi < ye) {
      PLFLT t;
      t=yi; yi=ye; ye=t;
    }

    if (gin.keysym == 'Q')
      exit(0);
  }  
  /* 
     I'm unable to continue, clearing the plot and advancing to the next
     one, without hiting the enter key, or pressing the button... help!

     Forcing the xwin driver to leave locate mode and destroying the
     xhairs (in GetCursorCmd()) solves some problems, but I still have
     to press the enter key or press Button-2 to go to next plot, even
     if a pladv() is not present!  Using plbop() solves the problem, but
     it shouldn't be needed! 
  */

  /* plbop(); */

  /* 
     plspause(0), pladv(0), plspause(1), also works,
     but the above question remains.
     With this approach, the previous pause state is lost,
     as there is no API call to get its current state.
  */

  plspause(0);
  pladv(0);

  plimage(img_f, width, height, 1., width, 1., height, xi, xe, ye, yi);

  plspause(1);
  pladv(0);

  plenv(xi, xe, ye, yi, 1, -1);
  plimage(img_f, width, height, 1., width, 1., height, xi, xe, ye, yi);
  pladv(0);

  plFree2dGrid(img_f, width, height);
  free(img);

  plend();
  exit(0);
}
