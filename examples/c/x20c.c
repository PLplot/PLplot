/* $Id$

	plimage demo

*/

#include "plplot/plcdemos.h"
#include <math.h>

#define XDIM 200
#define YDIM 300

void
plimage( PLFLT *data, PLINT nx, PLINT ny, 
	 PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax ,
	 PLFLT Dxmin, PLFLT Dxmax, PLFLT Dymin, PLFLT Dymax);

int
main(int argc, char *argv[])
{
  PLFLT x[XDIM], y[YDIM], z[XDIM*YDIM];
  int i, j;
  
  FILE *fp;
  int width, height, num_col;
  char ver[80];
  unsigned char *img;
  PLFLT *img_f;

  PLINT n_col = 255;
  PLINT rr[n_col], gg[n_col], bb[n_col];

/* Parse and process command line arguments */

  plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

  plinit();
  plenv(-1., 1., -1., 1., 1, -2); /* no plot box */

  /* build a square -- diagnostics */
  for (i=0; i<XDIM; i++)
    z[i*YDIM+(YDIM-1)] = 1.; /* right */
  for (i=0; i<XDIM; i++)
    z[i*YDIM] = 1.; /* left */
  
  for (i=0; i<YDIM; i++)
    z[i] = 1.; /* top */
  for (i=0; i<YDIM; i++)
    z[(XDIM-1)*YDIM+i] = 1.; /* botton */

  plimage(z, XDIM, YDIM, -1., 1., -1., 1., -1., 1., -1., 1.);
  pllab(" "," ","Should appear a red Square");
  pladv(0);

  plcol0(2); /* draw a yellow plot box, useful for diagnostics! :( */
  plenv(-1., 1., -1., 1., 1, -1);

  for (i=0; i<XDIM; i++)
    x[i] = cos(i*4.*PI/XDIM);
  for (i=0; i<YDIM; i++)
    y[i] = sin(i*4.*PI/YDIM);

  for (i=0; i<XDIM; i++)
    for (j=0; j<YDIM; j++)
      z[i*YDIM+j] = x[i]*y[j];

  plimage(z, XDIM, YDIM, -1., 1., -1., 1., -1., 1., -1., 1.);
  pladv(0);

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
  img_f = (PLFLT *) malloc(width*height*sizeof(PLFLT));

  fread(img, sizeof(char), width*height, fp);
  fclose(fp);

  for (i=0; i<width; i++)
    for (j=0; j<height; j++)
      img_f[i*height+j] = img[(height-j)*width+i];

  plimage(img_f, width, height, -1., 1., -1., 1., -1., 1., -1., 1.);

  plend();
  exit(0);
}
