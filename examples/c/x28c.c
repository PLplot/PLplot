/* $Id$

	plmtex3, plptex3 demo.
*/

#include "plcdemos.h"
/* Choose these values to correspond to tick marks. */
#define XPTS 2
#define YPTS 2
#define NREVOLUTION 16
#define NROTATION 8
#define NSHEAR 8

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
     omega, sin_omega, cos_omega, domega;
   int i,j;
   /* Allocate and define the minimal x, y, and z to insure 3D box */
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

   /* Parse and process command line arguments */
   
   (void) plparseopts(&argc, argv, PL_PARSE_FULL);
   
   plinit();

   /* Demonstrate inclination and shear capability pattern. */
   
   pladv(0);
   plvpor(-0.15, 1.15, -0.05, 1.05);
   plwind(-1.2, 1.2, -0.8, 1.5);
   plw3d(1.0, 1.0, 1.0, xmin, xmax, ymin, ymax, zmin, zmax, 
	 20., 45.);
   
   plcol0(2);
   plbox3("b", "", xmax-xmin, 0,
	  "b", "", ymax-ymin, 0,
	  "bcd", "", zmax-zmin, 0);

   /* z = zmin. */
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

   /* x = xmax. */
   plschr(0., 1.0);
   for (i = 0; i < NREVOLUTION; i++) {
      omega = 2.*M_PI*((PLFLT)i/(PLFLT)NREVOLUTION);
      sin_omega = sin(omega);
      cos_omega = cos(omega);
      x_inclination = 0.;
      y_inclination = -0.5*yrange*cos_omega;
      z_inclination = 0.5*zrange*sin_omega;
      x_shear = 0.;
      y_shear = 0.5*yrange*sin_omega;
      z_shear = 0.5*zrange*cos_omega;
      plptex3(
	      xmax, ymid, zmid,
	      x_inclination, y_inclination, z_inclination,
	      x_shear, y_shear, z_shear, 
	      0.0, "  revolution");
   }

   /* y = ymax. */
   plschr(0., 1.0);
   for (i = 0; i < NREVOLUTION; i++) {
      omega = 2.*M_PI*((PLFLT)i/(PLFLT)NREVOLUTION);
      sin_omega = sin(omega);
      cos_omega = cos(omega);
      x_inclination = 0.5*xrange*cos_omega;
      y_inclination = 0.;
      z_inclination = 0.5*zrange*sin_omega;
      x_shear = -0.5*xrange*sin_omega;
      y_shear = 0.;
      z_shear = 0.5*zrange*cos_omega;
      plptex3(
	      xmid, ymax, zmid,
	      x_inclination, y_inclination, z_inclination,
	      x_shear, y_shear, z_shear, 
	      0.0, "  revolution");
   }

   /* Draw minimal 3D grid to finish defining the 3D box. */
   plmesh(x, y, z, XPTS, YPTS, DRAW_LINEXY);

   /* Demonstrate rotation of string around its axis. */
   pladv(0);
   plvpor(-0.15, 1.15, -0.05, 1.05);
   plwind(-1.2, 1.2, -0.8, 1.5);
   plw3d(1.0, 1.0, 1.0, xmin, xmax, ymin, ymax, zmin, zmax, 
	 20., 45.);
   
   plcol0(2);
   plbox3("b", "", xmax-xmin, 0,
	  "b", "", ymax-ymin, 0,
	  "bcd", "", zmax-zmin, 0);

   /* y = ymax. */
   plschr(0., 1.0);
   x_inclination = 1.;
   y_inclination = 0.;
   z_inclination = 0.;
   x_shear = 0.;
   for (i = 0; i < NROTATION; i++) {
      omega = 2.*M_PI*((PLFLT)i/(PLFLT)NROTATION);
      sin_omega = sin(omega);
      cos_omega = cos(omega);
      y_shear = 0.5*yrange*sin_omega;
      z_shear = 0.5*zrange*cos_omega;
      plptex3(
	      xmid, ymax, zmax -(zmax-0.2)*((PLFLT)i/(PLFLT)(NROTATION-1)),
	      x_inclination, y_inclination, z_inclination,
	      x_shear, y_shear, z_shear,
	      0.5, "rotation for y = y#dmax#u");
   }

   /* x = xmax. */
   plschr(0., 1.0);
   x_inclination = 0.;
   y_inclination = -1.;
   z_inclination = 0.;
   y_shear = 0.;
   for (i = 0; i < NROTATION; i++) {
      omega = 2.*M_PI*((PLFLT)i/(PLFLT)NROTATION);
      sin_omega = sin(omega);
      cos_omega = cos(omega);
      x_shear = 0.5*xrange*sin_omega;
      z_shear = 0.5*zrange*cos_omega;
      plptex3(
	      xmax, ymid, zmax -(zmax-0.2)*((PLFLT)i/(PLFLT)(NROTATION-1)),
	      x_inclination, y_inclination, z_inclination,
	      x_shear, y_shear, z_shear,
	      0.5, "rotation for x = x#dmax#u");
   }

   /* z = zmin. */
   plschr(0., 1.0);
   x_inclination = 1.;
   y_inclination = 0.;
   z_inclination = 0.;
   x_shear = 0.;
   for (i = 0; i < NROTATION; i++) {
      omega = 2.*M_PI*((PLFLT)i/(PLFLT)NROTATION);
      sin_omega = sin(omega);
      cos_omega = cos(omega);
      y_shear = 0.5*yrange*cos_omega;
      z_shear = 0.5*zrange*sin_omega;
      plptex3(
	      xmid, ymax -(ymax-0.2)*((PLFLT)i/(PLFLT)(NROTATION-1)), zmin,
	      x_inclination, y_inclination, z_inclination,
	      x_shear, y_shear, z_shear,
	      0.5, "rotation for z = z#dmin#u");
   }

   /* Draw minimal 3D grid to finish defining the 3D box. */
   plmesh(x, y, z, XPTS, YPTS, DRAW_LINEXY);

   /* Demonstrate shear of string along its axis. */
   /* Work around xcairo and pngcairo (but not pscairo) problems for 
    * shear vector too close to axis of string. (N.B. no workaround
    * would be domega = 0.) */
   domega = 0.05;
   pladv(0);
   plvpor(-0.15, 1.15, -0.05, 1.05);
   plwind(-1.2, 1.2, -0.8, 1.5);
   plw3d(1.0, 1.0, 1.0, xmin, xmax, ymin, ymax, zmin, zmax, 
	 20., 45.);
   
   plcol0(2);
   plbox3("b", "", xmax-xmin, 0,
	  "b", "", ymax-ymin, 0,
	  "bcd", "", zmax-zmin, 0);

   /* y = ymax. */
   plschr(0., 1.0);
   x_inclination = 1.;
   y_inclination = 0.;
   z_inclination = 0.;
   y_shear = 0.;
   for (i = 0; i < NSHEAR; i++) {
      omega = domega + 2.*M_PI*((PLFLT)i/(PLFLT)NSHEAR);
      sin_omega = sin(omega);
      cos_omega = cos(omega);
      x_shear = 0.5*yrange*sin_omega;
      z_shear = 0.5*zrange*cos_omega;
      plptex3(
	      xmid, ymax, zmax -(zmax-0.2)*((PLFLT)i/(PLFLT)(NSHEAR-1)),
	      x_inclination, y_inclination, z_inclination,
	      x_shear, y_shear, z_shear,
	      0.5, "shear for y = y#dmax#u");
   }

   /* x = xmax. */
   plschr(0., 1.0);
   x_inclination = 0.;
   y_inclination = -1.;
   z_inclination = 0.;
   x_shear = 0.;
   for (i = 0; i < NSHEAR; i++) {
      omega = domega + 2.*M_PI*((PLFLT)i/(PLFLT)NSHEAR);
      sin_omega = sin(omega);
      cos_omega = cos(omega);
      y_shear = -0.5*xrange*sin_omega;
      z_shear = 0.5*zrange*cos_omega;
      plptex3(
	      xmax, ymid, zmax -(zmax-0.2)*((PLFLT)i/(PLFLT)(NSHEAR-1)),
	      x_inclination, y_inclination, z_inclination,
	      x_shear, y_shear, z_shear,
	      0.5, "shear for x = x#dmax#u");
   }

   /* z = zmin. */
   plschr(0., 1.0);
   x_inclination = 1.;
   y_inclination = 0.;
   z_inclination = 0.;
   z_shear = 0.;
   for (i = 0; i < NSHEAR; i++) {
      omega = domega + 2.*M_PI*((PLFLT)i/(PLFLT)NSHEAR);
      sin_omega = sin(omega);
      cos_omega = cos(omega);
      y_shear = 0.5*yrange*cos_omega;
      x_shear = 0.5*zrange*sin_omega;
      plptex3(
	      xmid, ymax -(ymax-0.2)*((PLFLT)i/(PLFLT)(NSHEAR-1)), zmin,
	      x_inclination, y_inclination, z_inclination,
	      x_shear, y_shear, z_shear,
	      0.5, "shear for z = z#dmin#u");
   }

   /* Draw minimal 3D grid to finish defining the 3D box. */
   plmesh(x, y, z, XPTS, YPTS, DRAW_LINEXY);

   /* Demonstrate plmtex3 axis labelling capability */
   pladv(0);
   plvpor(-0.15, 1.15, -0.05, 1.05);
   plwind(-1.2, 1.2, -0.8, 1.5);
   plw3d(1.0, 1.0, 1.0, xmin, xmax, ymin, ymax, zmin, zmax, 
	 20., 45.);
   
   plcol0(2);
   plbox3("b", "", xmax-xmin, 0,
	  "b", "", ymax-ymin, 0,
	  "bcd", "", zmax-zmin, 0);

   plschr(0., 1.0);
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
   /* Draw minimal 3D grid to finish defining the 3D box. */
   plmesh(x, y, z, XPTS, YPTS, DRAW_LINEXY);

   /* Clean up. */
   free((void *) x);
   free((void *) y);
   plFree2dGrid(z, XPTS, YPTS);
   plend();
   exit(0);
}
