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
  char *img;
  PLFLT *img_f;

  PLINT n_col = 255;
  PLINT rr[256], gg[256], bb[256];

/* Parse and process command line arguments */

  plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

  plinit();
  plcol0(2); /* draw a yellow border box, useful for diagnostics! */
  plenv(-1., 1., -1., 1., 1, -1);

  for (i=0; i<XDIM; i++)
    x[i] = cos(i*4.*M_PI/XDIM);
  for (i=0; i<YDIM; i++)
    y[i] = sin(i*4.*M_PI/YDIM);

  for (i=0; i<XDIM; i++)
    for (j=0; j<YDIM; j++)
      z[i*YDIM+j] = x[i]*y[j];

  plimage(z, XDIM, YDIM, -1., 1., -1., 1., -1., 1., -1., 1.);
  pladv(0);

  /* set gray colormap */
  for (i=0; i<n_col; i++)
    rr[i] = gg[i] = bb[i] = i;

  plscmap1(rr, gg, bb, n_col);

  plimage(z, XDIM, YDIM, -1., 1., -1., 1., -1., 1., -1., 1.);
  pladv(0);

  /* naive grayscale binary ppm reading. If you know how to, improve it */
  fp = fopen("lena.pgm","r");
  fscanf(fp,"%s\n", ver); /* version */
  printf("version: %s\n", ver);
  i = fgetc(fp);
  ungetc(i, fp);
  if (i == '#') {
    fgets(ver, 80, fp); /* comments */
    printf("%s", ver);
  }
  fscanf(fp,"%d%d%d", &width, &height, &num_col); /* width, height num colors */
  printf("width=%d height=%d num_color=%d\n", width, height, num_col);

  img = (char *) malloc(width*height*sizeof(char));
  img_f = (PLFLT *) malloc(width*height*sizeof(PLFLT));

  fread(img, sizeof(char), width*height, fp);
  fclose(fp);

  for (i=0; i<width*height; i++)
    img_f[i] = 128. - img[i]; /* convert to float and complement ?! */

  plimage(img_f, width, height, -1., 1., -1., 1., -1, 1., -1., 1.);

  plend();
  exit(0);
}
