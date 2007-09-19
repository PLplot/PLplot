/* $Id$

	plmtex3, plptex3 demo.
*/

#include "plcdemos.h"
/* Choose these values to correspond to tick marks. */
#define XPTS 2
#define YPTS 2
#define NREVOLUTION 16
#define NROTATION 8

/*--------------------------------------------------------------------------*\
 * main
 *
 * Demonstrates plotting text in 3D.
\*--------------------------------------------------------------------------*/


int main(int argc, char *argv[])
{
   PLFLT *x, *y, **z,
     xmin=0., xmax=1.0, xmid = 0.5*(xmax + xmin), xrange = xmax - xmin,
     ymin=0., ymax=1.0, ymid = 0.5*(ymax + ymin), yrange = ymax - ymin,
     zmin=0., zmax=1.0, zmid = 0.5*(zmax + zmin), zrange = zmax - zmin,
     x_inclination, y_inclination, z_inclination,
     x_shear, y_shear, z_shear,
     omega, sin_omega, cos_omega;
   int i,j;
   /* Parse and process command line arguments */
   
   (void) plparseopts(&argc, argv, PL_PARSE_FULL);
   
   plinit();

   /* Allocate data structures */
   x = (PLFLT *) calloc(XPTS, sizeof(PLFLT));
   y = (PLFLT *) calloc(YPTS, sizeof(PLFLT));

   plAlloc2dGrid(&z, XPTS, YPTS);
   for (i = 0; i < XPTS; i++) {
      x[i] = xmin + (double) i * (xmax-xmin)/ (double) (XPTS-1);
   }

   for (j = 0; j < YPTS; j++)
     y[j] = ymin + (double) j * (ymax-ymin)/ (double) (YPTS-1);

   for (i = 0; i < XPTS; i++) {
      for (j = 0; j < YPTS; j++) {
	 z[i][j] = 0.;
      }
   }

   pladv(0);
   plvpor(-0.15, 1.15, -0.05, 1.05);
   plwind(-1.2, 1.2, -0.8, 1.5);
   plw3d(1.0, 1.0, 1.0, xmin, xmax, ymin, ymax, zmin, zmax, 
	 40.0, 45);
   
   plcol0(2);
   plbox3("b", "", xmax-xmin, 0,
	  "b", "", ymax-ymin, 0,
	  "bcd", "", zmax-zmin, 0);
   plmtex3("xp", 3.0, 0.5, 0.5, "Arbitrarily displaced");
   plmtex3("xp", 4.5, 0.5, 0.5, "primary X-axis label");
   plmtex3("xs", -2.5, 0.5, 0.5, "Arbitrarily displaced");
   plmtex3("xs", -1.0, 0.5, 0.5, "secondary X-axis label");
   plmtex3("yp", 3.0, 0.5, 0.5, "Arbitrarily displaced");
   plmtex3("yp", 4.5, 0.5, 0.5, "primary Y-axis label");
   plmtex3("ys", -2.5, 0.5, 0.5, "Arbitrarily displaced");
   plmtex3("ys", -1.0, 0.5, 0.5, "secondary Y-axis label");
   plmtex3("zp", 4.5, 0.5, 0.5, "Arbitrarily displaced");
   plmtex3("zp", 3.0, 0.5, 0.5, "primary Z-axis label");
   plmtex3("zs", -2.5, 0.5, 0.5, "Arbitrarily displaced");
   plmtex3("zs", -1.0, 0.5, 0.5, "secondary Z-axis label");
   
   plschr(0., 1.0);
   for (i = 0; i < NREVOLUTION; i++) {
      omega = 2.*M_PI*((PLFLT)i/(PLFLT)NREVOLUTION);
      sin_omega = sin(omega);
      cos_omega = cos(omega);
      x_inclination = 0.5*xrange*cos_omega;
      y_inclination = 0.5*yrange*sin_omega;
      z_inclination = 0.;
      x_shear = -0.5*xrange*sin_omega;
      y_shear = 0.5*yrange*cos_omega;
      z_shear = 0.;
      plptex3(
	      xmid, ymid, zmin,
	      x_inclination, y_inclination, z_inclination,
	      x_shear, y_shear, z_shear, 
	      0.0, "  revolution");
   }

   plschr(0., 3.0);
   for (i = 0; i < NROTATION; i++) {
      omega = 2.*M_PI*((PLFLT)i/(PLFLT)NROTATION);
      sin_omega = sin(omega);
      cos_omega = cos(omega);
      x_inclination = 1.;
      y_inclination = 0.;
      z_inclination = 0.;
      x_shear = 0.;
      y_shear = 0.5*yrange*sin_omega;
      z_shear = 0.5*zrange*cos_omega;
      plptex3(
	      xmid, ymax, zmax -(zmax-0.2)*((PLFLT)i/(PLFLT)(NROTATION-1)),
	      x_inclination, y_inclination, z_inclination,
	      x_shear, y_shear, z_shear,
	      0.5, "rotation");
   }

   plschr(0., 1.0);
/*   plptex3(xmin, 0.0, 0.0,  1.0, 0.0, 0.0,  0.0, 0.2, 0.0,  0.0,
	   "seven(a)");
   plptex3(xmin, 0.0, 0.5,  1.0, 0.0, 0.0,  0.1, 0.2, 0.0,  0.0,
	   "eight(a)");
   plptex3(xmin, 0.0, 1.0,  1.0, 0.0, 0.0,  0.2, 0.2, 0.0,  0.0,
	   "nine(a)");
   plptex3(xmin, 0.0, 1.5,  1.0, 0.0, 0.0,  0.3, 0.2, 0.0,  0.0,
	   "ten(a)");
   plptex3(xmin, 0.0, 2.0,  1.0, 0.0, 0.0,  0.4, 0.2, 0.0,  0.0,
	   "eleven(a)");
   plptex3(xmin, 1.0, 0.0,  1.0, 0.0, 0.0,  0.0, 0.0, 0.2,  0.0,
	   "seven(b)");
   plptex3(xmin, 1.0, 0.5,  1.0, 0.0, 0.0,  0.0, 0.1, 0.2,  0.0,
	   "eight(b)");
   plptex3(xmin, 1.0, 1.0,  1.0, 0.0, 0.0,  0.0, 0.2, 0.2,  0.0,
	   "nine(b)");
   plptex3(xmin, 1.0, 1.5,  1.0, 0.0, 0.0,  0.0, 0.3, 0.2,  0.0,
	   "ten(b)");
   plptex3(xmin, 1.0, 2.0,  1.0, 0.0, 0.0,  0.0, 0.4, 0.2,  0.0,
	   "eleven(b)"); */
   /* Draw 3D grid (where the grid lines correspond to tick marks because
    * of XPTS, YPTS choice) at Z = 0. */
   plmesh(x, y, z, XPTS, YPTS, DRAW_LINEXY);
   
   /* Clean up. */
   free((void *) x);
   free((void *) y);
   plFree2dGrid(z, XPTS, YPTS);
   plend();
}
