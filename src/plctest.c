/* $Id$
 * $Log$
 * Revision 1.1  1993/07/16 22:38:13  mjl
 * These include functions to shade between contour levels (currently using
 * a pattern fill).  Contributed by Wesley Ebisuzaki.
 *
*/

#include "plplotP.h"
#include <math.h>

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

/*
 * plctest( &(x[0][0]), PLFLT level)
 * where x was defined as PLFLT x[4][4];
 *
 * determines if the contours associated with level have
 * postive slope or negative slope in the box:
 *
 *  (2,3)   (3,3)
 *
 *  (2,2)   (3,2)
 *
 * this is heuristic and can be changed by the user
 *
 * return 1 if positive slope
 *        0 if negative slope
 *
 * algorithmn:
 *      1st test:
 *	find length of contours assuming positive and negative slopes
 *      if the length of the negative slope contours is much bigger
 *	than the positive slope, then the slope is positive.
 *      (and vice versa)
 *      (this test tries to minimize the length of contours)
 *
 *      2nd test:
 *      if abs((top-right corner) - (bottom left corner)) >
 *	   abs((top-left corner) - (bottom right corner)) ) then
 *		return negatiave slope.
 *      (this test tries to keep the slope for different contour levels
 *	the same)
 */

#define X(a,b) (x[a*4+b])
#define linear(val2, val3, level)  ((level - val2) / (val3 - val2))
#define POSITIVE_SLOPE (PLINT) 1
#define NEGATIVE_SLOPE (PLINT) 0
#define RATIO_SQ 6.0

PLINT plctest(PLFLT *x, PLFLT level) {

	double a, b;
	double positive, negative, left, right, top, bottom;

	/* find positions of lines */
	/* top = x coor of top intersection */
	/* bottom = x coor of bottom intersection */
	/* left = y coor of left intersection */
	/* right = y coor of right intersection */

	left = linear(X(1,1), X(1,2), level);
	right = linear(X(2,1), X(2,2), level);
	top = linear(X(1,2), X(2,2), level);
	bottom = linear(X(1,1), X(2,1), level);

	/* positive = sq(length of positive contours) */
	/* negative = sq(length of negative contours) */

	positive = top*top + (1.0-left)*(1.0-left) +
		(1.0-bottom)*(1.0-bottom) + right*right;

	negative = left*left + bottom*bottom +
		(1.0-top)*(1.0-top) + (1.0-right)*(1.0-right);
#ifdef DEBUG
	printf("ctest pos %f neg %f lev %f\n",positive,negative,level);
#endif
	if (RATIO_SQ * positive < negative) return POSITIVE_SLOPE;
	if (RATIO_SQ * negative < positive) return NEGATIVE_SLOPE;

	a = X(1,2) - X(2,1);
	b = X(1,1) - X(2,2);
#ifdef DEBUG
	printf("ctest a %f  b %f\n",a,b);
#endif
	if (fabs(a) > fabs(b)) return NEGATIVE_SLOPE;
	return (PLINT) 0;
}

/*
 * second routine - easier to use
 * fills in x[4][4] and calls plctest
 *
 * test location a[ix][iy] (lower left corner)
 */

PLINT plctestez(PLFLT *a, PLINT nx, PLINT ny, PLINT ix,
	PLINT iy, PLFLT level) {

	PLFLT x[4][4];
	int i, j, ii, jj;


	for (i = 0; i < 4; i++) {
		ii = ix + i - 1;
		ii = max(0, ii);
		ii = min(ii, nx - 1);
		for (j = 0; j < 4; j++) {
			jj = iy + j - 1;
			jj = max(0, jj);
			jj = min(jj, ny - 1);
			x[i][j] = a[ii * ny + jj];
		}
	}
	return plctest( &(x[0][0]), level);
}
