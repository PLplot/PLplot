/* $Id$
 * $Log$
 * Revision 1.1  1993/07/16 22:38:12  mjl
 * These include functions to shade between contour levels (currently using
 * a pattern fill).  Contributed by Wesley Ebisuzaki.
 *
*/

/* copyright 1993 Wesley Ebisuzaki */

#include "plplotP.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define NEG  1
#define POS  8
#define OK   0
#define UNDEF 64

/*
 * simple shading program
 * can be used to shade contour plots or alone
 */

#define linear(val1, val2, level)  ((level - val1) / (val2 - val1))

static PLFLT sh_max, sh_min;
static int min_points, max_points, n_point;
static int min_pts[4], max_pts[4];
static PLINT pen_col_sh, pen_col_min, pen_col_max;
static PLINT pen_wd_sh, pen_wd_min, pen_wd_max;

static void set_cond(register int *cond, register PLFLT *a, 
		register char *defined, register PLINT n) {

	if (defined) {
		while (n--) {
			if (*defined++ == 0) *cond++ = UNDEF;
			else if (*a < sh_min) *cond++ = NEG;
			else if (*a > sh_max) *cond++ = POS;
			else *cond++ = OK;
			a++;
		}
	}
	else {
		while (n--) {
			if (*a < sh_min) *cond++ = NEG;
			else if (*a > sh_max) *cond++ = POS;
			else *cond++ = OK;
			a++;
		}
	}
}

static int find_interval(PLFLT a0, PLFLT a1, PLINT c0, PLINT c1, PLFLT *x) {
	/* two points x(0) = a0, (condition code c0)
		      x(1) = a1, (condition code c1)
	   return interval on the line that are shaded

	   returns 0 : no points to be shaded
		  1 : x[0] <= x < 1 is the interval
		  2 : x[0] <= x <= x[1] < 1 interval to be shaded
		 n_point, max_points, min_points are incremented
		 location of min/max_points are stored
	 */
	register int n;

	n = 0;
	if (c0 == OK) {
		x[n++] = 0.0;
		n_point++;
	}
	if (c0 == c1) return n;

	if (c0 == NEG || c1 == POS) {
		if (c0 == NEG) {
			x[n++] = linear(a0, a1, sh_min);
			min_pts[min_points++] = n_point++;
		}
		if (c1 == POS) {
			x[n++] = linear(a0, a1, sh_max);
			max_pts[max_points++] = n_point++;
		}
	}
	if (c0 == POS || c1 == NEG) {
		if (c0 == POS) {
			x[n++] = linear(a0, a1, sh_max);
			max_pts[max_points++] = n_point++;
		}
		if (c1 == NEG) {
			x[n++] = linear(a0, a1, sh_min);
			min_pts[min_points++] = n_point++;
		}
	}
	return n;
}

static void poly(void (*fill)(), PLFLT *x, PLFLT *y, PLINT v1, PLINT v2, 
	PLINT v3, PLINT v4) {
	/* draws a polygon from points in x[] and y[] */
	/* point selected by v1..v4 */

	register PLINT n = 0;
	PLFLT xx[4], yy[4];

	if (fill == NULL) return;
	if (v1 >= 0) {
		xx[n] = x[v1];
		yy[n++] = y[v1];
	}
	if (v2 >= 0) {
		xx[n] = x[v2];
		yy[n++] = y[v2];
	}
	if (v3 >= 0) {
		xx[n] = x[v3];
		yy[n++] = y[v3];
	}
	if (v4 >= 0) {
		xx[n] = x[v4];
		yy[n++] = y[v4];
	}
	(*fill)(n, xx, yy);
}
/*
 *
 * find a big rectangle for shading
 *
 * 2 goals: minimize calls to (*fill)()
 *  keep ratio 1:3 for biggest rectangle
 *
 * only called by plshade()
 *
 * assumed that a 1 x 1 square already fits
 *
 * c[] = condition codes
 * ny = c[1][0] == c[ny]  (you know what I mean)
 *
 * returns ix, iy = length of rectangle in grid units
 *
 * ix < dx - 1
 * iy < dy - 1
 *
 * If iy == 1 -> ix = 1 (so that cond code can be set to skip)
 *
 */
#define RATIO 3

#define COND(x,y) cond_code[x*ny + y]

static void big_recl(int *cond_code, register int ny, int dx, int dy, 
	int *ix, int *iy) {

	int ok_x, ok_y, j;
	register int i, x, y;
	register int *cond;

	/* ok_x = ok to expand in x direction */
	/* x = current number of points in x direction */

	ok_x = ok_y = 1;
	x = y = 2;

	while (ok_x || ok_y) {
#ifdef RATIO
		if (RATIO*x <= y || RATIO*y <= x) break;
#endif
		if (ok_y) {
			/* expand in vertical */
			ok_y = 0;
			if (y == dy) continue;
			cond = &COND(0,y);
			for (i = 0; i < x; i++) {
				if (*cond != OK) break;
				cond += ny;
			}
			if (i == x) {
				/* row is ok */
				y++;
				ok_y = 1;
			}
		}
		if (ok_x) {
			if (y == 2) break;
			/* expand in x direction */
			ok_x = 0;
			if (x == dx) continue;
			cond = &COND(x,0);
			for (i = 0; i < y; i++) {
				if (*cond++ != OK) break;
			}
			if (i == y) {
				/* column is OK */
				x++;
				ok_x = 1;
			}
		}
	}

	/* found the largest rectangle of 'ix' by 'iy' */
	*ix = --x;
	*iy = --y;
	
	/* set condition code to UNDEF in interior of rectangle */

	for (i = 1; i < x; i++) {
		cond = &COND(i,1);
		for (j = 1; j < y; j++) {
			*cond++ = UNDEF;
		}
	}
}

static void draw_boundary(PLINT slope, PLFLT *x, PLFLT *y) {
	/*
	 * draw boundaries of contour regions
	 * based on min_pts[], and max_pts[]
	 */
	int i;

	if (pen_col_min != 0 && pen_wd_min != 0 && min_points != 0) {
		plcol(pen_col_min);
		plwid(pen_wd_min);
		if (min_points == 4 && slope == 0) {
			/* swap points 1 and 3 */
			i = min_pts[1];
			min_pts[1] = min_pts[3];
			min_pts[3] = i;
		}
		pljoin(x[min_pts[0]],y[min_pts[0]],x[min_pts[1]], y[min_pts[1]]);
		if (min_points == 4) {
			pljoin(x[min_pts[2]],y[min_pts[2]],x[min_pts[3]],
				y[min_pts[3]]);
		}
	}
	if (pen_col_max != 0 && pen_wd_max != 0 && max_points != 0) {
		plcol(pen_col_max);
		plwid(pen_wd_max);
		if (max_points == 4 && slope == 0) {
			/* swap points 1 and 3 */
			i = max_pts[1];
			max_pts[1] = max_pts[3];
			max_pts[3] = i;
		}
		pljoin(x[max_pts[0]],y[max_pts[0]],x[max_pts[1]], y[max_pts[1]]);
		if (max_points == 4) {
			pljoin(x[max_pts[2]],y[max_pts[2]],x[max_pts[3]],
				y[max_pts[3]]);
		}
	}
	plcol(pen_col_sh);
	plwid(pen_wd_sh);
}


void plshade(PLFLT *a, PLINT nx, PLINT ny, char *defined, PLFLT left, 
	PLFLT right, PLFLT bottom, PLFLT top, void (*mapform)(), 
	PLFLT shade_min, PLFLT shade_max, 
	PLINT sh_color, PLINT sh_width, PLINT min_color, PLINT min_width,
	PLINT max_color, PLINT max_width, void (*fill)(), PLINT rectangular) {

	PLINT n, slope, ix, iy;
	int nx1,ny1, count, i, j;
	PLFLT *a0, *a1, dx, dy, b;
	PLFLT x[8], y[8], xp[2];
	PLINT plctestez(PLFLT *, PLINT, PLINT, PLINT, PLINT, PLFLT);

	int *c, *c0, *c1;

	if (nx <= 0 || ny <= 0) plexit("plshade: illegal args");
	if (shade_min >= shade_max) return;
	if (mapform == NULL) rectangular = 1;
	pen_col_sh = sh_color;
	pen_col_min = min_color;
	pen_col_max = max_color;
	pen_wd_sh = sh_width;
	pen_wd_min = min_width;
	pen_wd_max = max_width;

	plstyl((PLINT) 0, NULL, NULL);
	plcol(pen_col_sh);
	plwid(pen_wd_sh);

	/* alloc space for condition codes */
	if ((c = (int *) malloc(nx * ny * sizeof(int))) == NULL)
		plexit("ran out of memory");
	sh_min = shade_min;
	sh_max = shade_max;

	set_cond(c, a, defined, nx*ny);

	ny1 = ny - 1;
	nx1 = nx - 1;
	dx = (right - left) / (nx-1);
	dy = (top - bottom) / (ny-1);
	a0 = a;
	a1 = a + ny;
	c0 = c;
	c1 = c + ny;

	for (ix = 0; ix < nx1; ix++, a0 = a1, a1 += ny, 
				c0 = c1, c1 += ny, left += dx) {

		for (iy = 0; iy < ny1; iy++) {
			count = c0[iy] + c0[iy+1] + c1[iy] + c1[iy+1];
			if (count >= UNDEF) continue;
			if (count  == 4 * POS) continue;
			if (count == 4 * NEG) continue;
			b = bottom + iy * dy;

			if (count == 4*OK) {
				/* find bigest rectangle that fits */
				if (rectangular) {
					big_recl(c0+iy,ny,nx-ix,ny-iy,&i,&j);
				}
				else {
					i = j = 1;
				}
				x[0] = x[1] = left;
				x[2] = x[3] = left + i*dx;
				y[0] = y[3] = b;
				y[1] = y[2] = b + j*dy;
				if (mapform) (*mapform)((PLINT) 4, x, y);
				if (fill) (*fill)((PLINT) 4, x, y);
				iy += j - 1;
				continue;
			}
			n_point = min_points = max_points = 0;
			n = find_interval(a0[iy],a0[iy+1],c0[iy],c0[iy+1],xp);
			for (j = 0; j < n; j++) {
				x[j] = left;
				y[j] = b + dy * xp[j];
			}

			i = find_interval(a0[iy+1],a1[iy+1],c0[iy+1],c1[iy+1],xp);
			for (j = 0; j < i; j++) {
				x[j+n] = left + dx * xp[j];
				y[j+n] = b + dy;
			}
			n += i;

			i = find_interval(a1[iy+1],a1[iy],c1[iy+1],c1[iy],xp);
			for (j = 0; j < i; j++) {
				x[n+j] = left + dx;
				y[n+j] = b + dy * (1.0 - xp[j]);
			}
			n += i;

			i = find_interval(a1[iy],a0[iy],c1[iy],c0[iy],xp);
			for (j = 0; j < i; j++) {
				x[n+j] = left + dx * (1.0 - xp[j]);
				y[n+j] = b;
			}
			n += i;

			if (mapform) (*mapform)(n, x, y);
			if (min_points == 4)
				slope = plctestez(a,nx,ny,ix,iy,shade_min);
			if (max_points == 4)
				slope = plctestez(a,nx,ny,ix,iy,shade_max);

			/* special cases: check number of times
			   a contour is in a box */

			switch((min_points << 3) + max_points) {
			case 000:
			case 020:
			case 002:
			case 022:
				if (fill) (*fill)(n, x, y);
				break;
			case 040:	/* 2 contour lines in box */
			case 004:
				if (n != 6) fprintf(stderr,"plshade err n=%d !6", n);

				if (slope == 1 && c0[iy] == OK) {
					if (fill) (*fill)(n,x,y);
				}
				else if (slope == 1) {
					poly(fill,x,y,0,1,2,-1);
					poly(fill,x,y,3,4,5,-1);
				}
				else if (c0[iy+1] == OK) {
					if (fill) (*fill)(n,x,y);
				}
				else {
					poly(fill,x,y,0,1,5,-1);
					poly(fill,x,y,2,3,4,-1);
				}
				break;
			case 044:
				if (n != 8) fprintf(stderr,"plshade err n=%d !8", n);
				if (fill == NULL) break;
				if (slope == 1) {
					poly(fill,x,y,0,1,2,3);
					poly(fill,x,y,4,5,6,7);
				}
				else {
					poly(fill,x,y,0,1,6,7);
					poly(fill,x,y,2,3,4,5);
				}
				break;
			case 024:
			case 042:
				/* 3 contours */
				if (max_points == 4) i = NEG;
				else i = POS;

				if (c0[iy] == i) {
					slope = NEG;
					poly(fill,x,y,0,1,5,6);
					poly(fill,x,y,2,3,4,-1);
				}
				else if (c0[iy+1] == i) {
					slope = POS;
					poly(fill,x,y,0,1,2,3);
					poly(fill,x,y,4,5,6,-1);
				}
				else if (c1[iy+1] == i) {
					slope = NEG;
					poly(fill,x,y,0,1,6,-1);
					poly(fill,x,y,2,3,4,5);
				}
				else if (c1[iy] == i) {
					slope = POS;
					poly(fill,x,y,0,1,2,-1);
					poly(fill,x,y,3,4,5,6);
				}
				break;
			default:
				fprintf(stderr,"prog err switch\n");
				break;
			}
			draw_boundary(slope,x,y);
		}
	}
	free(c);
}
