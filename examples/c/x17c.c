/* jc: This is a much reworked version of x17c.c, a non-working demo
 * on the distribution.
 */

/* $Id$
<<<<<<< x17c.c
 * $Log$
 * Revision 1.5.2.2  2001/01/22 09:09:02  rlaboiss
 * Merge of DEBIAN and v5_0_1 branches (conflicts are still to be solved)
 *
 * Revision 1.5.2.1  2001/01/22 09:05:31  rlaboiss
 * Debian stuff corresponding to package version 4.99j-11
 *
 * Revision 1.5  1995/06/01  21:40:14  mjl
 * All C demo files: changed file inclusion to use quotes instead of angle
 * brackets so that dependencies are retained during development.
 *
 * Revision 1.4  1995/04/12  08:19:02  mjl
 * Changes to all C demos: now include "plcdemos.h" to get all startup
 * definitions and includes that are useful to share between them.
 *
 * Revision 1.3  1995/03/16  23:18:59  mjl
 * All example C programs: changed plParseInternalOpts() call to plParseOpts().
 *
 * Revision 1.2  1994/08/05  09:28:42  mjl
 * Put in an early bail-out since it's not working yet.
 *
 * Revision 1.1  1994/04/08  12:08:54  mjl
 * Preliminary stab at a strip chart demo (doesn't work yet).
=======
>>>>>>> 1.10
*/

/* Plots a simple stripchart with four pens.
 * Eventually I want a really cool demo here: slowly evolving plots of
 * say density and temperature.  These only need to get updated every so
 * often.  And then at the bottom some strip charts of energy or such
 * that are continually updated.
 * Try using -db on command line
 */

<<<<<<< x17c.c
#include "plcdemos.h"
#include <poll.h>
=======
#include "plplot/plcdemos.h"
#ifdef HAS_POLL
#include <poll.h>
#else
#include <unistd.h>
#endif
>>>>>>> 1.10

/*--------------------------------------------------------------------------*\
 * main program
\*--------------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
<<<<<<< x17c.c
    PLINT id1, id2, n, autoy, acc, nsteps = 1000;
    PLFLT y1, y2, y3, y4, ymin, ymax, xlab, ylab;
    PLFLT t, tmin, tmax, tjump, dt, noise;
    PLINT colbox, collab, colline[4], styline[4];
    char *legline[4], toplab[20];

/* plplot initialization */
/* Parse and process command line arguments */

    (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

// If db is used the plot is much more smooth. However, because of the
// async X behaviour, one does not have a real-time scripcharter.
//    plSetOpt("db", ""); 
    plSetOpt("np", "");

/* User sets up plot completely except for window and data 
 * Eventually settings in place when strip chart is created will be
 * remembered so that multiple strip charts can be used simultaneously.
 */

/* Specify some reasonable defaults for ymin and ymax */
/* The plot will grow automatically if needed (but not shrink) */

    ymin = -1.0;
    ymax =  1.0;

/* Specify initial tmin and tmax -- this determines length of window. */
/* Also specify maximum jump in t */
/* This can accomodate adaptive timesteps */

    tmin = 0.;
    tmax = 10.;
    tjump = 0.3;	// percentage of plot to jump

/* Axes options same as plbox. */
/* Only automatic tick generation and label placement allowed */
/* Eventually I'll make this fancier */

    colbox = 1;
    collab = 3;
    styline[0] = colline[0] = 2;	// pens color and line style
    styline[1] = colline[1] = 3;
    styline[2] = colline[2] = 4;
    styline[3] = colline[3] = 5;    

    legline[0] = "sum";				// pens legend
    legline[1] = "sin";
    legline[2] = "sin*noi";
    legline[3] = "sin+noi";

    xlab = 0.; ylab = 0.25;	// legend position 

    autoy = 0;	// autoscale y
    acc = 1;	// dont scrip, accumulate

/* Initialize plplot */

    plinit();
    pladv(0);    
    plvsta();    

    plstripc(&id1, "bcnst", "bcnstv",
	     tmin, tmax, tjump, ymin, ymax,
		xlab, ylab,
		autoy, acc,
	    colbox, collab,
		colline, styline, legline, 
	    "t", "", "Strip chart demo"); 

    autoy = 0;	// autoscale y
    acc = 1;	// accumulate

/* This is to represent a loop over time */
/* Let's try a random walk process */

    y1 = y2 = y3 = y4 = 0.0;
    dt = 0.1;

    for (n = 0; n < nsteps; n++) {
		poll(0,0,10);	// wait a little (10 ms) to simulate time elapsing
		t = (double)n * dt;
		noise = drand48() - 0.5;
		y1 = y1 + noise;
		y2 = sin(t*3.1415926/18.);
		y3 = y2 * noise;
		y4 = y2 + noise/3.;
	
	// there is no need for all pens to have the same number of points
	// or beeing equally time spaced.
		
		if (n%2)	
			plstripa(id1, 0, t, y1);
		if (n%3)
			plstripa(id1, 1, t, y2);
		if (n%4)
			plstripa(id1, 2, t, y3);
		if (n%5)
			plstripa(id1, 3, t, y4);
    }

/* Destroy strip chart and it's memory */

    plstripd(id1);
    plend();
    exit(0);
=======
  PLINT id1, id2, n, autoy, acc, nsteps = 1000;
  PLFLT y1, y2, y3, y4, ymin, ymax, xlab, ylab;
  PLFLT t, tmin, tmax, tjump, dt, noise;
  PLINT colbox, collab, colline[4], styline[4];
  char *legline[4], toplab[20];
  double drand48(void);

  /* plplot initialization */
  /* Parse and process command line arguments */

  (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

  /* If db is used the plot is much more smooth. However, because of the
     async X behaviour, one does not have a real-time scripcharter.
  */
  plSetOpt("db", "");
  plSetOpt("np", "");

  /* User sets up plot completely except for window and data 
   * Eventually settings in place when strip chart is created will be
   * remembered so that multiple strip charts can be used simultaneously.
   */

  /* Specify some reasonable defaults for ymin and ymax */
  /* The plot will grow automatically if needed (but not shrink) */

  ymin = -0.1;
  ymax = 0.1;

  /* Specify initial tmin and tmax -- this determines length of window. */
  /* Also specify maximum jump in t */
  /* This can accomodate adaptive timesteps */

  tmin = 0.;
  tmax = 10.;
  tjump = 0.3;	/* percentage of plot to jump */

  /* Axes options same as plbox. */
  /* Only automatic tick generation and label placement allowed */
  /* Eventually I'll make this fancier */

  colbox = 1;
  collab = 3;
  styline[0] = colline[0] = 2;	/* pens color and line style */
  styline[1] = colline[1] = 3;
  styline[2] = colline[2] = 4;
  styline[3] = colline[3] = 5;    

  legline[0] = "sum";				/* pens legend */
  legline[1] = "sin";
  legline[2] = "sin*noi";
  legline[3] = "sin+noi";

  xlab = 0.; ylab = 0.25;	/* legend position */

  autoy = 1;	/* autoscale y */
  acc = 1;	/* don't scrip, accumulate */

  /* Initialize plplot */

  plinit();
  pladv(0);    
  plvsta();    

  plstripc(&id1, "bcnst", "bcnstv",
	   tmin, tmax, tjump, ymin, ymax,
	   xlab, ylab,
	   autoy, acc,
	   colbox, collab,
	   colline, styline, legline, 
	   "t", "", "Strip chart demo"); 

  autoy = 0;	/* autoscale y */
  acc = 1;	/* accumulate */

  /* This is to represent a loop over time */
  /* Let's try a random walk process */

  y1 = y2 = y3 = y4 = 0.0;
  dt = 0.1;

  for (n = 0; n < nsteps; n++) {
#ifdef HAS_POLL
    poll(0,0,10);
#else
    usleep(10000);	/* wait a little (10 ms) to simulate time elapsing */
#endif
    t = (double)n * dt;
    noise = drand48() - 0.5;
    y1 = y1 + noise;
    y2 = sin(t*3.1415926/18.);
    y3 = y2 * noise;
    y4 = y2 + noise/3.;
	
    /* There is no need for all pens to have the same number of
       points or beeing equally time spaced. */
		
    if (n%2)	
      plstripa(id1, 0, t, y1);
    if (n%3)
      plstripa(id1, 1, t, y2);
    if (n%4)
      plstripa(id1, 2, t, y3);
    if (n%5)
      plstripa(id1, 3, t, y4);
    pleop();  /* use double buffer (-db on command line) */
  }

  /* Destroy strip chart and it's memory */

  plstripd(id1);
  plend();
  exit(0);
>>>>>>> 1.10
}
