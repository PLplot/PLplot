/* $Id$
 * $Log$
 * Revision 1.4  2000/12/18 21:01:50  airwin
 * Change to new style plplot/*.h header file locations.
 *
 * Revision 1.3  1999/06/25 04:18:47  furnish
 * Add the c_ namespace control thing to plmap and plmeridians, which
 * somehow slipped in without this protection.
 *
 * Revision 1.2  1994/08/25  04:04:49  mjl
 * Eliminated unnecessary variable decls.
 *
 * Revision 1.1  1994/07/29  20:26:10  mjl
 * Function to plot map backgrounds, read from the specified map data file.
 * Contributed by Wesley Ebisuzaki.
 *
*/

/*	plmap.c

	Continental Outline and Political Boundary Backgrounds

	Some plots need a geographical background such as the global
	surface temperatures or the population density.  The routine
	plmap() will draw one of the following backgrounds: continental
	outlines, political boundaries, the United States, and the United
	States with the continental outlines.  The routine plmeridians()
	will add the latitudes and longitudes to the background.  After
	the background has been drawn, one can use a contour routine or a
	symbol plotter to finish off the plot.

	Copyright 1991,1993,1994  Wesley Ebisuzaki

	This program can be freely distributed, sale except for media
	costs is prohibited.
*/

#include "plplot/plplotP.h"

/*----------------------------------------------------------------------*\
 * void plmap(void (*mapform)(PLINT, PLFLT *, PLFLT *), char *type,
 *            PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat);
 *
 * plot continental outline in world coordinates
 *
 * v1.4: machine independant version
 * v1.3: replaced plcontinent by plmap, added plmeridians
 * v1.2: 2 arguments:  mapform, type of plot
 * v1.1: change buffersize for amiga - faster reads
 *
 * mapform(PLINT n, PLFLT *x, PLFLT *y) is a routine to transform the
 * coordinate longitudes and latitudes to a plot coordinate system.  By
 * using this transform, we can change from a longitude, latitude
 * coordinate to a polar stereographic project, for example.  Initially,
 * x[0]..[n-1] are the longitudes and y[0]..y[n-1] are the corresponding
 * latitudes.  After the call to mapform(), x[] and y[] should be replaced
 * by the corresponding plot coordinates.  If no transform is desired,
 * mapform can be replaced by NULL.
 * 
 * type is a character string. The value of this parameter determines the
 * type of background. The possible values are,
 *
 * 	"globe"		continental outlines
 * 	"usa"		USA and state boundaries
 * 	"cglobe"	continental outlines and countries
 * 	"usaglobe"	USA, state boundaries and continental outlines
 * 
 * minlong, maxlong are the values of the longitude on the left and right
 * side of the plot, respectively. The value of minlong must be less than
 * the values of maxlong, and the values of maxlong-minlong must be less
 * or equal to 360.
 * 
 * minlat, maxlat are the minimum and maximum latitudes to be plotted on
 * the background.  One can always use -90.0 and 90.0 as the boundary
 * outside the plot window will be automatically eliminated.  However, the
 * program will be faster if one can reduce the size of the background
 * plotted.
\*----------------------------------------------------------------------*/

#define MAP_FILE ".map"
#define OFFSET (180*100)
#define SCALE 100.0
#define W_BUFSIZ	(32*1024)

void
c_plmap( void (*mapform)(PLINT, PLFLT *, PLFLT *), char *type,
         PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat )
{
    PLINT wrap, sign;
    int i, j;
    PLFLT bufx[200], bufy[200], x[2], y[2];
    short int test[400];
    register FILE *in;
    char filename[100];

    unsigned char n_buff[2], buff[800];
    int n;
    long int t;

#ifdef AMIGA
    int error;
    char *io_buffer;
#endif	

    /*
     * read map outline 
     */
    strcpy(filename,type);
    strcat(filename,MAP_FILE);

    if ((in = plLibOpen(filename)) == NULL)
	return;

#ifdef AMIGA
    /* increase the size of the IO buffer = faster reads */
    /* ANSI C -- however haven't found it necessary on UNIX */
    io_buffer = (char *) malloc(W_BUFSIZ);
    if (io_buffer == NULL) {
	fprintf(stderr, "ran out of memory: plmap\n");
	return;
    }
    error = setvbuf(in,io_buffer,_IOFBF,W_BUFSIZ);
    if (error != 0) {
	fprintf(stderr, "plmap: setvbuf err %d\n",error);
	free(io_buffer);
	return;
    }
#endif

    for (;;) {
	/* read in # points in segment */
	if (fread(n_buff, sizeof (unsigned char), 2, in) == 0) break;
	n = (n_buff[0] << 8) + n_buff[1];
	if (n == 0) break;

	fread(buff, sizeof (unsigned char), 4*n, in);
	if (n == 1) continue;

	for (j = i = 0; i < n; i++, j += 2) {
	    t = (buff[j] << 8) + buff[j+1];
	    bufx[i] = (t - OFFSET) / SCALE;
	}
	for (i = 0; i < n; i++, j += 2) {
	    t = (buff[j] << 8) + buff[j+1];
	    bufy[i] = (t - OFFSET) / SCALE;
	}

	for (i = 0; i < n; i++) {
	    while (bufx[i] < minlong) {
		bufx[i] += 360.0;
	    }
	    while (bufx[i] > maxlong) {
		bufx[i] -= 360.0;
	    }
	}

	/* remove last 2 points if both outside of domain and won't plot */

	while (n > 1) {
	    if ((bufx[n-1] < minlong && bufx[n-2] < minlong) ||
		(bufx[n-1] > maxlong && bufx[n-2] > maxlong) ||
		(bufy[n-1] < minlat && bufy[n-2] < minlat) ||
		(bufy[n-1] > maxlat && bufy[n-2] > maxlat)) {
		n--;
	    }
	    else {
		break;
	    }
	}
	if (n <= 1) continue;

	wrap = 0;
	/* check for wrap around problems */
	for (i = 0; i < n-1; i++) {
	    test[i] = (abs(bufx[i]-bufx[i+1]) > 30.0);
	    if (test[i]) wrap = 1;
	}

	if (wrap == 0) {	
	    if (mapform != NULL) (*mapform)(n,bufx,bufy);
	    plline(n,bufx,bufy);
	}
	else {
	    for (i = 0; i < n-1; i++) {
		x[0] = bufx[i];
		x[1] = bufx[i+1];
		y[0] = bufy[i];
		y[1] = bufy[i+1];
		if (test[i] == 0) {
		    if (mapform != NULL) (*mapform)(2,x,y);
		    plline(2,x,y);
		}
		else {
		    /* segment goes off the edge */
		    sign = (x[1] > x[0]) ? 1 : -1;
		    x[1] -= sign * 360.0;
		    if (mapform != NULL) (*mapform)(2,x,y);
		    plline(2,x,y);
		    x[0] = bufx[i];
		    x[1] = bufx[i+1];
		    y[0] = bufy[i];
		    y[1] = bufy[i+1];
		    x[0] += sign * 360.0;
		    if (mapform != NULL) (*mapform)(2,x,y);
		    plline(2,x,y);
		}
	    }
	}
    }
#ifdef AMIGA
    free(io_buffer);
#endif
}

/*----------------------------------------------------------------------*\
 * void plmeridians(void (*mapform)(PLINT, PLFLT *, PLFLT *), 
 *		    PLFLT dlong, PLFLT dlat, PLFLT minlong, PLFLT maxlong, 
 *		    PLFLT minlat, PLFLT maxlat);
 *
 * Plot the latitudes and longitudes on the background.  The lines 
 * are plotted in the current color and line style.
 * 
 * mapform(PLINT n, PLFLT *x, PLFLT *y) is a routine to transform the
 * coordinate longitudes and latitudes to a plot coordinate system.  By
 * using this transform, we can change from a longitude, latitude
 * coordinate to a polar stereographic project, for example.  Initially,
 * x[0]..x[n-1] are the longitudes and y[0]..y[n-1] are the corresponding
 * latitudes.  After the call to mapform(), x[] and y[] should be replaced
 * by the corresponding plot coordinates.  If no transform is desired,
 * mapform can be replaced by NULL.
 * 
 * dlat, dlong are the interval in degrees that the latitude and longitude
 * lines are to be plotted. 
 * 
 * minlong, maxlong are the values of the longitude on the left and right
 * side of the plot, respectively. The value of minlong must be less than
 * the values of maxlong, and the values of maxlong-minlong must be less
 * or equal to 360.
 * 
 * minlat, maxlat are the minimum and maximum latitudes to be plotted on
 * the background.  One can always use -90.0 and 90.0 as the boundary
 * outside the plot window will be automatically eliminated.  However, the
 * program will be faster if one can reduce the size of the background
 * plotted.
\*----------------------------------------------------------------------*/

#define NSEG 100

void 
c_plmeridians( void (*mapform)(PLINT, PLFLT *, PLFLT *), 
               PLFLT dlong, PLFLT dlat,
               PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat )
{
    PLFLT yy, xx, temp, x[2], y[2], dx, dy;

    if (minlong > maxlong) {
	temp = minlong;
	minlong = maxlong;
	maxlong = temp;
    }
    if (minlat > maxlat) {
	temp = minlat;
	minlat = maxlat;
	maxlat = temp;
    }
    dx = (maxlong - minlong) / NSEG;
    dy = (maxlat - minlat) / NSEG;

    /* latitudes */

    for (yy = dlat * ceil(minlat/dlat); yy <= maxlat; yy += dlat) {
	if (mapform == NULL) {
	    y[0] = y[1] = yy;
	    x[0] = minlong;
	    x[1] = maxlong;
	    plline(2,x,y);
	}
	else {
	    for (xx = minlong; xx < maxlong; xx += dx) {
	        y[0] = y[1] = yy;
		x[0] = xx;
		x[1] = xx + dx;
	 	(*mapform)(2,x,y);
		plline(2,x,y);
	    }
	}
    }

    /* longitudes */
 
    for (xx = dlong * ceil(minlong/dlong); xx <= maxlong; xx += dlong) {
        if (mapform == NULL) {
            x[0] = x[1] = xx;
            y[0] = minlat;
            y[1] = maxlat;
            plline(2,x,y);
        }
        else {
            for (yy = minlat; yy < maxlat; yy += dy) {
                x[0] = x[1] = xx;
                y[0] = yy;
                y[1] = yy + dy;
                (*mapform)(2,x,y);
                plline(2,x,y);
            }
        }
    }
}
