/* $Id$

	plimage demo

*/

#include "plplot/plcdemos.h"
#include <math.h>

#define XDIM 200
#define YDIM 300

void
plimage( PLFLT **data, PLINT nx, PLINT ny, 
	 PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax ,
	 PLFLT Dxmin, PLFLT Dxmax, PLFLT Dymin, PLFLT Dymax);

int
main(int argc, char *argv[])
{
  PLFLT x[XDIM], y[YDIM], **z;
  int i, j;
  static PLGraphicsIn gin;
  PLFLT xi, yi, xe, ye, start;
    
  FILE *fp;
  int width, height, num_col;
  char ver[80];
  unsigned char *img;
  PLFLT **img_f;

  PLINT n_col = 255;
  PLINT rr[n_col], gg[n_col], bb[n_col];

/* Parse and process command line arguments */

  plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

  plinit();
  plenv(-1., 1., -1., 1., 1, -2); /* no plot box */

  plAlloc2dGrid(&z, XDIM, YDIM);

  /* build a square -- diagnostics */
  for (i=0; i<XDIM; i++)
    z[i][YDIM-1] = 1.; /* right */
  for (i=0; i<XDIM; i++)
    z[i][0] = 1.; /* left */
  
  for (i=0; i<YDIM; i++)
    z[0][i] = 1.; /* top */
  for (i=0; i<YDIM; i++)
    z[XDIM-1][i] = 1.; /* botton */

  /*
    for (i=0; i<XDIM; i++) {
    for (j=0; j<YDIM; j++)
      printf("%d ", (int) z[i][j]);
    printf("\n");}
  */

  pllab("...around a blue square."," ","Should appear a red border...");
  plimage(z, XDIM, YDIM, -1., 1., -1., 1., -1., 1., -1., 1.);
  pladv(0);

  plcol0(2); /* draw a yellow plot box, useful for diagnostics! :( */
  plenv(-1., 1., -1., 1., 1, -1);

  for (i=0; i<XDIM; i++)
    x[i] = cos(i*4.*PI/XDIM);
  for (i=0; i<YDIM; i++)
    y[i] = sin(i*4.*PI/YDIM);

  for (i=0; i<XDIM; i++)
    for (j=0; j<YDIM; j++)
      z[i][j] = x[i]*y[j];

  pllab("...around the plot."," ","Should appear a yellow box...");
  plimage(z, XDIM, YDIM, -1., 1., -1., 1., -1., 1., -1., 1.);
  pladv(0);
  plFree2dGrid(z, XDIM, YDIM);

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
  printf("width=%d height=%d\n", width, height);

  img = (unsigned char *) malloc(width*height*sizeof(char));
  plAlloc2dGrid(&img_f, width, height);

  fread(img, sizeof(char), width*height, fp);
  fclose(fp);

  for (i=0; i<width; i++)
    for (j=0; j<height; j++)
      img_f[i][j] = img[(height-j)*width+i];

  plenv(1., width, 1., height, 1, -1);
  pllab("Set and drag Button 1 to (re)set selection, Button 3 to finish."," ","Lena...");
  plimage(img_f, width, height, 1., width, 1., height, 1., width, 1., height);

  plxormod(1); /* enter xor mode to draw a selection rectangle */
  start = 0;
  xi = ye = 0.;
  xe = width/2.; yi = height/2.;

  while(1) {
    PLFLT sx[5], sy[5];

    plGetCursor(&gin);

    if (gin.button == 1) {
      xi = gin.wX; yi = gin.wY;
      if (start)
	plline(5, sx, sy); /* clear previous rectangle */
      start = 0;

      sx[0] = xi; sy[0] = yi;
      sx[4] = xi; sy[4] = yi;
    }

    if (gin.state == 0x100) {
      xe = gin.wX; ye = gin.wY;
      if (start)
	plline(5, sx, sy); /* clear previous rectangle */
      start = 1;
  
      sx[2] = xe; sy[2] = ye;
      sx[1] = xe; sy[1] = yi;
      sx[3] = xi; sy[3] = ye;
      plline(5, sx, sy); /* draw new rectangle */
    }

    if (gin.button == 3) {
      if (start)
	plline(5, sx, sy); /* clear previous rectangle */
      printf("end\n");
      break;
    }      
  }
  plxormod(0); /* leave xor mod */

  /* I'm unable to continue without hiting the enter key, or pressing the button... help!*/
  //plclear();plflush();
  pladv(0);
  
  plimage(img_f, width, height, 1., width, 1., height, xi, xe, ye, yi);
  pladv(0);

  plenv(xi, xe, ye, yi, 1, -1);
  plimage(img_f, width, height, 1., width, 1., height, xi, xe, ye, yi);
  pladv(0);

  plend();

  plFree2dGrid(img_f, width, height);
  free(img);

  exit(0);
}
