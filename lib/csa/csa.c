/******************************************************************************
 *
 * File:           csa.c
 *
 * Created:        16/10/2002
 *
 * Author:         Pavel Sakov
 *                 CSIRO Marine Research
 *
 * Purpose:        2D data approximation with bivariate cubic spline.
 *                 A set of library functions + standalone utility.
 *
 * Description:    See J. Haber, F. Zeilfelder, O.Davydov and H.-P. Seidel,
 *                 Smooth approximation and rendering of large scattered data
 *                 sets, in  ``Proceedings of IEEE Visualization 2001''
 *                 (Th.Ertl, K.Joy and A.Varshney, Eds.), pp.341-347, 571,
 *                 IEEE Computer Society, 2001. 
 *                 http://www.uni-giessen.de/www-Numerische-Mathematik/
 *                        davydov/VIS2001.ps.gz
 *                 http://www.math.uni-mannheim.de/~lsmath4/paper/
 *                        VIS2001.pdf.gz
 *
 * Revisions:      None
 *
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "version.h"
#include "csa_internal.h"
#include "nan.h"

#define NPASTART 5              /* Number of Points Allocated at Start */

/* default algorithm parameters */
#define NMIN_DEF 3
#define NMAX_DEF 40
#define K_DEF 140

extern int errno;

int csa_verbose = 0;

static void csa_quit(char* format, ...)
{
    va_list args;

    fflush(stdout);             /* just in case -- to have the exit message
                                 * last */

    fprintf(stderr, "error: csa: ");
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    exit(1);
}

static double** dalloc2d(int n1, int n2)
{
    unsigned int size;
    double* p;
    double** pp;
    int i;

    assert(n1 > 0);
    assert(n2 > 0);
    size = n1 * n2;
    assert(size <= UINT_MAX);
    if ((p = calloc(size, sizeof(double))) == NULL)
        csa_quit("dalloc2d(): %s\n", strerror(errno));

    size = n2 * sizeof(double*);
    if ((pp = malloc(size)) == NULL)
        csa_quit("dalloc2d(): %s\n", strerror(errno));
    for (i = 0; i < n2; i++)
        pp[i] = p + i * n1;

    return pp;
}

static void dfree2d(double** pp)
{
    double* p;

    p = pp[0];
    assert(pp != NULL);
    free(pp);
    assert(p != NULL);
    free(p);
}

static void*** palloc2d(int n1, int n2)
{
    unsigned int size;
    void** p;
    void*** pp;
    int i;

    assert(n1 > 0);
    assert(n2 > 0);
    size = n1 * n2;
    assert(size <= UINT_MAX);
    if ((p = calloc(size, sizeof(void*))) == NULL)
        csa_quit("palloc2d(): %s\n", strerror(errno));

    size = n2 * sizeof(void*);
    if ((pp = malloc(size)) == NULL)
        csa_quit("palloc2d(): %s\n", strerror(errno));
    for (i = 0; i < n2; ++i, p += n1)
        pp[i] = p;

    return pp;
}

static void pfree2d(void*** pp)
{
    void** p;

    p = pp[0];
    assert(pp != NULL);
    free(pp);
    assert(p != NULL);
    free(p);
}

static triangle* triangle_create(square* s, point vertices[], int index)
{
    triangle* t = malloc(sizeof(triangle));

    t->parent = s;
    memcpy(t->vertices, vertices, sizeof(point) * 3);
    t->middle.x = (vertices[0].x + vertices[1].x + vertices[2].x) / 3.0;
    t->middle.y = (vertices[0].y + vertices[1].y + vertices[2].y) / 3.0;
    t->h = s->parent->h;
    t->index = index;

    t->r = 0.0;
    t->points = 0;
    t->nallocated = 0;
    t->npoints = 0;
    t->hascoeffs = 0;
    t->primary = 0;
    t->order = -1;

    return t;
}

static void triangle_addpoint(triangle* t, point* p)
{
    if (t->nallocated == t->npoints) {
        if (t->nallocated == 0) {
            t->points = malloc(NPASTART * sizeof(point*));
            t->nallocated = NPASTART;
        } else {
            t->points = realloc(t->points, t->nallocated * 2 * sizeof(point*));
            t->nallocated *= 2;
        }
    }

    t->points[t->npoints] = p;
    t->npoints++;
}

static void triangle_destroy(triangle* t)
{
    if (t->points != NULL)
        free(t->points);
    free(t);
}

/* Calculates barycentric coordinates of a point.
 * Takes into account that possible triangles are rectangular, with the right 
 * angle at t->vertices[0], the vertices[1] vertex being in 
 * (-3*PI/4) + (PI/2) * t->index direction from vertices[0], and
 * vertices[2] being at (-5*PI/4) + (PI/2) * t->index.
 */
static void triangle_calculatebc(triangle* t, point* p, double bc[])
{
    double dx = p->x - t->vertices[0].x;
    double dy = p->y - t->vertices[0].y;

    if (t->index == 0) {
        bc[1] = (dy - dx) / t->h;
        bc[2] = -(dx + dy) / t->h;
    } else if (t->index == 1) {
        bc[1] = (dx + dy) / t->h;
        bc[2] = (dy - dx) / t->h;
    } else if (t->index == 2) {
        bc[1] = (dx - dy) / t->h;
        bc[2] = (dx + dy) / t->h;
    } else {
        bc[1] = -(dx + dy) / t->h;
        bc[2] = (dx - dy) / t->h;
    }
    bc[0] = 1.0 - bc[1] - bc[2];
}

static square* square_create(csa* parent, double xmin, double ymin, int i, int j)
{
    int ii;

    square* s = malloc(sizeof(square));
    double h = parent->h;

    s->parent = parent;
    s->i = i;
    s->j = j;

    s->points = NULL;
    s->nallocated = 0;
    s->npoints = 0;

    s->primary = 0;

    for (ii = 0; ii < 4; ++ii) {
        point vertices[3];

        vertices[0].x = xmin + h / 2.0;
        vertices[0].y = ymin + h / 2.0;
        vertices[1].x = xmin + h * (((ii + 1) % 4) / 2);        /* 0 1 1 0 */
        vertices[1].y = ymin + h * (((ii + 2) % 4) / 2);        /* 1 1 0 0 */
        vertices[2].x = xmin + h * (ii / 2);    /* 0 0 1 1 */
        vertices[2].y = ymin + h * (((ii + 1) % 4) / 2);        /* 0 1 1 0 */
        s->triangles[ii] = triangle_create(s, vertices, ii);
    }

    for (ii = 0; ii < 25; ++ii)
        s->coeffs[ii] = NaN;

    return s;
}

static void square_destroy(square* s)
{
    int i;

    for (i = 0; i < 4; ++i)
        triangle_destroy(s->triangles[i]);
    if (s->points != NULL)
        free(s->points);
    free(s);
}

static void square_addpoint(square* s, point* p)
{
    if (s->nallocated == s->npoints) {
        if (s->nallocated == 0) {
            s->points = malloc(NPASTART * sizeof(point*));
            s->nallocated = NPASTART;
        } else {
            s->points = realloc(s->points, s->nallocated * 2 * sizeof(point*));
            s->nallocated *= 2;
        }
    }

    s->points[s->npoints] = p;
    s->npoints++;
}

csa* csa_create()
{
    csa* a = malloc(sizeof(csa));

    a->verbose = 0;

    a->xmin = DBL_MAX;
    a->xmax = -DBL_MAX;
    a->ymin = DBL_MAX;
    a->ymax = -DBL_MAX;

    a->points = malloc(NPASTART * sizeof(point*));
    a->nallocated = NPASTART;
    a->npoints = 0;

    a->ni = 0;
    a->nj = 0;
    a->h = NaN;
    a->squares = NULL;

    a->npt = 0;
    a->pt = NULL;

    a->nmin = NMIN_DEF;
    a->nmax = NMAX_DEF;
    a->k = K_DEF;

    return a;
}

void csa_destroy(csa* a)
{
    int i, j;

    if (a->squares != NULL) {
        for (j = 0; j < a->nj; ++j)
            for (i = 0; i < a->ni; ++i)
                square_destroy(a->squares[j][i]);
        pfree2d((void***) a->squares);
    }
    if (a->pt != NULL)
        free(a->pt);
    if (a->points != NULL)
        free(a->points);
    free(a);
}

void csa_addpoints(csa* a, int n, point points[])
{
    int na = a->nallocated;
    int i;

    assert(a->squares == NULL);
    /*
     * (can be called prior to squarization only) 
     */

    while (na < a->npoints + n)
        na *= 2;
    if (na != a->nallocated) {
        a->points = realloc(a->points, na * sizeof(point*));
        a->nallocated = na;
    }

    for (i = 0; i < n; ++i) {
        point* p = &points[i];

        a->points[a->npoints] = p;
        a->npoints++;

        if (p->x < a->xmin)
            a->xmin = p->x;
        if (p->x > a->xmax)
            a->xmax = p->x;
        if (p->y < a->ymin)
            a->ymin = p->y;
        if (p->y > a->ymax)
            a->ymax = p->y;
    }
}

/* Marks the squares containing "primary" triangles by setting "primary" flag 
 * to 1.
 */
static void csa_setprimaryflag(csa* a)
{
    square*** squares = a->squares;
    int nj1 = a->nj - 1;
    int ni1 = a->ni - 1;
    int i, j;

    for (j = 1; j < nj1; ++j) {
        for (i = 1; i < ni1; ++i) {
            if (squares[j][i]->npoints > 0) {
                if ((i + j) % 2 == 0) {
                    squares[j][i]->primary = 1;
                    squares[j - 1][i - 1]->primary = 1;
                    squares[j + 1][i - 1]->primary = 1;
                    squares[j - 1][i + 1]->primary = 1;
                    squares[j + 1][i + 1]->primary = 1;
                } else {
                    squares[j - 1][i]->primary = 1;
                    squares[j + 1][i]->primary = 1;
                    squares[j][i - 1]->primary = 1;
                    squares[j][i + 1]->primary = 1;
                }
            }
        }
    }
}

/* Splits the data domain in a number of squares.
 */
static void csa_squarize(csa* a, int n)
{
    int nps[7] = { 0, 0, 0, 0, 0, 0 };  /* stats on number of points per
                                         * square */
    double dx = a->xmax - a->xmin;
    double dy = a->ymax - a->ymin;
    int npoints = a->npoints;
    double h;
    int i, j, ii, nadj;

    if (csa_verbose) {
        fprintf(stderr, "squarizing csa:\n");
        fflush(stderr);
    }

    assert(a->squares == NULL);
    /*
     * (can be done only once) 
     */

    h = sqrt(dx * dy * n / npoints);    /* square edge size */
    if (dx < h)
        h = dy * n / npoints;
    if (dy < h)
        h = dx * n / npoints;
    a->h = h;

    a->ni = ceil(dx / h) + 2;
    a->nj = ceil(dy / h) + 2;

    if (csa_verbose) {
        fprintf(stderr, "  %d x %d squares\n", a->ni, a->nj);
        fflush(stderr);
    }

    /*
     * create squares 
     */
    a->squares = (square***) palloc2d(a->ni, a->nj);
    for (j = 0; j < a->nj; ++j)
        for (i = 0; i < a->ni; ++i)
            a->squares[j][i] = square_create(a, a->xmin + h * (i - 1), a->ymin + h * (j - 1), i, j);

    /*
     * map points to squares 
     */
    for (ii = 0; ii < npoints; ++ii) {
        point* p = a->points[ii];

        i = floor((p->x - a->xmin) / h) + 1;
        j = floor((p->y - a->ymin) / h) + 1;
        square_addpoint(a->squares[j][i], p);
    }

    /*
     * mark relevant squares with no points 
     */
    csa_setprimaryflag(a);

    /*
     * Create a list of "primary" triangles, for which spline coefficients
     * will be calculated directy (by least squares method), without using
     * C1 smoothness condiftions. 
     */
    a->pt = malloc((a->ni / 2 + 1) * a->nj * sizeof(triangle*));
    for (j = 0, ii = 0, nadj = 0; j < a->nj; ++j) {
        for (i = 0; i < a->ni; ++i) {
            square* s = a->squares[j][i];

            if (s->npoints > 0) {
                int nn = s->npoints / 5;

                if (nn > 6)
                    nn = 6;
                nps[nn]++;
                ii++;
            }
            if (s->primary && s->npoints == 0)
                nadj++;
            if (s->primary) {
                a->pt[a->npt] = s->triangles[0];
                s->triangles[0]->primary = 1;
                a->npt++;
            }
        }
    }

    if (csa_verbose) {
        fprintf(stderr, "  %d non-empty squares\n", ii);
        fprintf(stderr, "  %d primary squares\n", a->npt);
        fprintf(stderr, "  %d primary squares with no data\n", nadj);
        fprintf(stderr, "  %.2f points per square \n", (double) a->npoints / ii);
    }

    if (csa_verbose == 2) {
        for (i = 0; i < 6; ++i)
            fprintf(stderr, "  %d-%d points -- %d squares\n", i * 5, i * 5 + 4, nps[i]);
        fprintf(stderr, "  %d or more points -- %d squares\n", i * 5, nps[i]);
    }

    if (csa_verbose == 2) {
        fprintf(stderr, " j\\i");
        for (i = 0; i < a->ni; ++i)
            fprintf(stderr, "%3d ", i);
        fprintf(stderr, "\n");
        for (j = a->nj - 1; j >= 0; --j) {
            fprintf(stderr, "%3d ", j);
            for (i = 0; i < a->ni; ++i) {
                square* s = a->squares[j][i];

                if (s->npoints > 0)
                    fprintf(stderr, "%3d ", s->npoints);
                else
                    fprintf(stderr, "  . ");
            }
            fprintf(stderr, "\n");
        }
    }

    if (csa_verbose)
        fflush(stderr);
}

/* Returns all squares intersecting with a square with center in t->middle
 * and edges of length 2*t->r parallel to X and Y axes.
 */
static void getsquares(csa* a, triangle* t, int* n, square*** squares)
{
    int imin = floor((t->middle.x - t->r - a->xmin) / t->h);
    int imax = ceil((t->middle.x + t->r - a->xmin) / t->h);
    int jmin = floor((t->middle.y - t->r - a->ymin) / t->h);
    int jmax = ceil((t->middle.y + t->r - a->ymin) / t->h);
    int i, j;

    if (imin < 0)
        imin = 0;
    if (imax >= a->ni)
        imax = a->ni - 1;
    if (jmin < 0)
        jmin = 0;
    if (jmax >= a->nj)
        jmax = a->nj - 1;

    *n = 0;
    (*squares) = malloc((imax - imin + 1) * (jmax - jmin + 1) * sizeof(square*));

    for (j = jmin; j <= jmax; ++j) {
        for (i = imin; i <= imax; ++i) {
            square* s = a->squares[j][i];

            if (s->npoints > 0) {
                (*squares)[*n] = a->squares[j][i];
                (*n)++;
            }
        }
    }
}

static double distance(point* p1, point* p2)
{
    return hypot(p1->x - p2->x, p1->y - p2->y);
}

/* Thins data by creating an auxiliary regular grid and for leaving only
 * the most central point within each grid cell.
 * (I follow the paper here. It is possible that taking average -- in terms of
 * both value and position -- of all points within a cell would be a bit more
 * robust.)
 */
static void thindata(triangle* t, int nmax)
{
    csa* a = t->parent->parent;
    int imax = ceil(sqrt((double) (nmax * 3 / 2)));
    square*** squares = (square***) palloc2d(imax, imax);
    double h = t->r * 2.0 / imax;
    double h2 = h / 2.0;
    double xmin = t->middle.x - t->r;
    double ymin = t->middle.y - t->r;
    int i, j, ii;

    for (j = 0; j < imax; ++j)
        for (i = 0; i < imax; ++i)
            squares[j][i] = square_create(a, xmin + h * i, ymin + h * j, i, j);

    for (ii = 0; ii < t->npoints; ++ii) {
        point* p = t->points[ii];
        int i = floor((p->x - xmin) / h);
        int j = floor((p->y - ymin) / h);
        square* s = squares[j][i];

        if (s->npoints == 0)
            square_addpoint(s, p);
        else {                  /* npoints == 1 */

            point pmiddle = { xmin + h * i + h2, ymin + h * j + h2 };

            if (distance(s->points[0], &pmiddle) > distance(p, &pmiddle))
                s->points[0] = p;
        }
    }

    t->npoints = 0;
    for (j = 0; j < imax; ++j) {
        for (i = 0; i < imax; ++i) {
            square* s = squares[j][i];

            if (squares[j][i]->npoints != 0)
                triangle_addpoint(t, s->points[0]);
            square_destroy(s);
        }
    }

    pfree2d((void***) squares);
    imax++;
}

/* Finds data points to be used in calculating spline coefficients for each 
 * primary triangle.
 */
static void csa_attachpoints(csa* a)
{
    int nmin = a->nmin;
    int nmax = a->nmax;
    int nincreased = 0;
    int nthinned = 0;
    int i;

    assert(a->npt > 0);

    if (csa_verbose) {
        fprintf(stderr, "distributing data points:\n  ");
        fflush(stderr);
    }

    for (i = 0; i < a->npt; ++i) {
        triangle* t = a->pt[i];
        int increased = 0;

        if (csa_verbose) {
            fprintf(stderr, ".");
            fflush(stderr);
        }

        t->r = t->h * 1.25;
        while (1) {
            int nsquares = 0;
            square** squares = NULL;
            int ii;

            getsquares(a, t, &nsquares, &squares);
            for (ii = 0; ii < nsquares; ++ii) {
                square* s = squares[ii];
                int iii;

                for (iii = 0; iii < s->npoints; ++iii) {
                    point* p = s->points[iii];

                    if (distance(p, &t->middle) <= t->r)
                        triangle_addpoint(t, p);
                }
            }

            free(squares);

            if (t->npoints < nmin) {
                if (!increased) {
                    increased = 1;
                    nincreased++;
                }
                t->r *= 1.25;
                t->npoints = 0;
            } else if (t->npoints > nmax) {
                nthinned++;
                thindata(t, nmax);
                if (t->npoints > nmin)
                    break;
                else {
                    /*
                     * Sometimes you have too much data, you thin it and -- 
                     * oops -- you have too little. This is not a frequent
                     * event, so let us not bother to put a new subdivision. 
                     */
                    t->r *= 1.25;
                    t->npoints = 0;
                }
            } else
                break;
        }
    }

    if (csa_verbose) {
        fprintf(stderr, "\n  %d sets enhanced, %d sets thinned\n", nincreased, nthinned);
        fflush(stderr);
    }
}

static int n2q(int n)
{
    assert(n >= 3);

    if (n >= 10)
        return 3;
    else if (n >= 6)
        return 2;
    else                        /* n == 3 */
        return 1;
}

/* Borrowed from Numerical Recipes. Stripped from unnecessary stuff like
 * matrices, vectors etc. and converted to double precision.
 *
 * Given a matrix a[0..m-1][0..n-1], this routine computes its singular value
 * decomposition, A = U.W.V' The matrix U replaces a on output. The
 * diagonal matrix of singular values W is output as a vector w[0..n-1]. The
 * matrix V (not the transpose V') is output as v[0..n-1][0..n-1].
 */
static void svdcmp(double** a, int n, int m, double w[], double** v)
{
    double* rv1 = malloc(n * sizeof(double));
    int i, j, k, l = -1;
    double anorm, c, f, g, h, s, scale, x, y, z;

    /*
     * Householder reduction to bidiagonal form 
     */
    anorm = g = s = scale = 0.0;
    for (i = 0; i < n; i++) {
        l = i + 1;
        rv1[i] = scale * g;
        g = s = scale = 0.0;
        if (i <= m - 1) {
            for (k = i; k < m; k++)
                scale += fabs(a[k][i]);
            if (scale != 0.0) {
                for (k = i; k < m; k++) {
                    a[k][i] /= scale;
                    s += a[k][i] * a[k][i];
                }
                f = a[i][i];
                g = -copysign(sqrt(s), f);
                h = f * g - s;
                a[i][i] = f - g;
                for (j = l; j < n; j++) {
                    for (s = 0.0, k = i; k < m; k++)
                        s += a[k][i] * a[k][j];
                    f = s / h;
                    for (k = i; k < m; k++)
                        a[k][j] += f * a[k][i];
                }
                for (k = i; k < m; k++)
                    a[k][i] *= scale;
            }
        }
        w[i] = scale * g;
        g = s = scale = 0.0;
        if (i < m && i != n - 1) {
            for (k = l; k < n; k++)
                scale += fabs(a[i][k]);
            if (scale != 0.0) {
                for (k = l; k < n; k++) {
                    a[i][k] /= scale;
                    s += a[i][k] * a[i][k];
                }
                f = a[i][l];
                g = -copysign(sqrt(s), f);
                h = f * g - s;
                a[i][l] = f - g;
                for (k = l; k < n; k++)
                    rv1[k] = a[i][k] / h;
                for (j = l; j < m; j++) {
                    for (s = 0.0, k = l; k < n; k++)
                        s += a[j][k] * a[i][k];
                    for (k = l; k < n; k++)
                        a[j][k] += s * rv1[k];
                }
                for (k = l; k < n; k++)
                    a[i][k] *= scale;
            }
        }
        {
            double tmp = fabs(w[i]) + fabs(rv1[i]);

            anorm = (anorm > tmp) ? anorm : tmp;
        }
    }

    /*
     * Accumulation of right-hand transformations 
     */
    for (i = n - 1; i >= 0; i--) {
        if (i < n - 1) {
            if (g != 0.0) {
                for (j = l; j < n; j++)
                    /*
                     * Double division to avoid underflow 
                     */
                    v[j][i] = (a[i][j] / a[i][l]) / g;
                for (j = l; j < n; j++) {
                    for (s = 0.0, k = l; k < n; k++)
                        s += a[i][k] * v[k][j];
                    for (k = l; k < n; k++)
                        v[k][j] += s * v[k][i];
                }
            }
            for (j = l; j < n; j++)
                v[i][j] = v[j][i] = 0.0;
        }
        v[i][i] = 1.0;
        g = rv1[i];
        l = i;
    }

    /*
     * Accumulation of left-hand transformations 
     */
    for (i = (m < n) ? m - 1 : n - 1; i >= 0; i--) {
        l = i + 1;
        g = w[i];
        for (j = l; j < n; j++)
            a[i][j] = 0.0;
        if (g) {
            g = 1.0 / g;
            for (j = l; j < n; j++) {
                for (s = 0.0, k = l; k < m; k++)
                    s += a[k][i] * a[k][j];
                f = (s / a[i][i]) * g;
                for (k = i; k < m; k++)
                    a[k][j] += f * a[k][i];
            }
            for (j = i; j < m; j++)
                a[j][i] *= g;
        } else
            for (j = i; j < m; j++)
                a[j][i] = 0.0;
        ++a[i][i];
    }

    /*
     * Diagonalization of the bidagonal form: loop over singular values, and 
     * over allowed iterations 
     */
    for (k = n - 1; k >= 0; k--) {
        int its;

        for (its = 1; its <= 30; its++) {
            int flag = 1;
            int nm = -1;

            for (l = k; l >= 0; l--) {  /* test for splitting */
                nm = l - 1;     /* rv1[0] is always zero */
                if ((fabs(rv1[l]) + anorm) == anorm) {
                    flag = 0;
                    break;
                }
                if ((fabs(w[nm]) + anorm) == anorm)
                    break;
            }
            if (flag) {
                c = 0.0;        /* cancellation of rv1[l], if l > 1 */
                s = 1.0;
                for (i = l; i <= k; i++) {
                    f = s * rv1[i];
                    rv1[i] = c * rv1[i];
                    if ((fabs(f) + anorm) == anorm)
                        break;
                    g = w[i];
                    h = hypot(f, g);
                    w[i] = h;
                    h = 1.0 / h;
                    c = g * h;
                    s = -f * h;
                    for (j = 0; j < m; j++) {
                        y = a[j][nm];
                        z = a[j][i];
                        a[j][nm] = y * c + z * s;
                        a[j][i] = z * c - y * s;
                    }
                }
            }
            z = w[k];
            if (l == k) {       /* convergence */
                if (z < 0.0) {  /* sing. val. is made non-negative */
                    w[k] = -z;
                    for (j = 0; j < n; j++)
                        v[j][k] = -v[j][k];
                }
                break;
            }
            if (its == 30)
                csa_quit("svdcmp(): no convergence in 30 iterations");
            x = w[l];           /* shift from bottom 2-by-2 minor */
            nm = k - 1;
            y = w[nm];
            g = rv1[nm];
            h = rv1[k];
            f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
            g = hypot(f, 1.0);
            f = ((x - z) * (x + z) + h * ((y / (f + copysign(g, f))) - h)) / x;

            /*
             * Next QR transformation 
             */
            c = s = 1.0;
            for (j = l; j <= nm; j++) {
                int jj;

                i = j + 1;
                g = rv1[i];
                y = w[i];
                h = s * g;
                g = c * g;
                z = hypot(f, h);
                rv1[j] = z;
                c = f / z;
                s = h / z;
                f = x * c + g * s;
                g = g * c - x * s;
                h = y * s;
                y *= c;
                for (jj = 0; jj < n; jj++) {
                    x = v[jj][j];
                    z = v[jj][i];
                    v[jj][j] = x * c + z * s;
                    v[jj][i] = z * c - x * s;
                }
                z = hypot(f, h);
                w[j] = z;       /* rotation can be arbitrary if z = 0 */
                if (z != 0.0) {
                    z = 1.0 / z;
                    c = f * z;
                    s = h * z;
                }
                f = c * g + s * y;
                x = c * y - s * g;
                for (jj = 0; jj < m; jj++) {
                    y = a[jj][j];
                    z = a[jj][i];
                    a[jj][j] = y * c + z * s;
                    a[jj][i] = z * c - y * s;
                }
            }
            rv1[l] = 0.0;
            rv1[k] = f;
            w[k] = x;
        }
    }

    free(rv1);
}

/* Least squares fitting via singular value decomposition.
 */
static void lsq(double** A, int ni, int nj, double* z, double* w, double* sol)
{
    double** V = dalloc2d(ni, ni);
    double** B = dalloc2d(nj, ni);
    int i, j, ii;

    svdcmp(A, ni, nj, w, V);

    for (j = 0; j < ni; ++j)
        for (i = 0; i < ni; ++i)
            V[j][i] /= w[i];
    for (i = 0; i < ni; ++i) {
        double* v = V[i];

        for (j = 0; j < nj; ++j) {
            double* a = A[j];
            double* b = &B[i][j];

            for (ii = 0; ii < ni; ++ii)
                *b += v[ii] * a[ii];
        }
    }
    for (i = 0; i < ni; ++i)
        sol[i] = 0.0;
    for (i = 0; i < ni; ++i)
        for (j = 0; j < nj; ++j)
            sol[i] += B[i][j] * z[j];

    dfree2d(B);
    dfree2d(V);
}

/*
 *  square->coeffs[]:
 *
 *   ---------------------
 *  | 3    10    17    24 |
 *  |    6    13    20    |
 *  | 2     9    16    23 |
 *  |    5    12    19    |
 *  | 1     8    15    22 |
 *  |    4    11    18    |
 *  | 0     7    14    21 |
 *   ---------------------
 */

/* Calculates spline coefficients in each primary triangle by least squares
 * fitting to data attached by csa_attachpoints().
 */
static void csa_findprimarycoeffs(csa* a)
{
    int n[4] = { 0, 0, 0, 0 };
    int i;

    if (csa_verbose)
        fprintf(stderr, "finding spline coefficients for primary triangles:\n  ");

    for (i = 0; i < a->npt; ++i) {
        triangle* t = a->pt[i];
        int npoints = t->npoints;
        point** points = t->points;
        double* z = malloc(npoints * sizeof(double));
        int q = n2q(t->npoints);
        int ok = 1;
        double b[10];
        double b1[6];
        int ii;

        if (csa_verbose) {
            fprintf(stderr, ".");
            fflush(stderr);
        }

        for (ii = 0; ii < npoints; ++ii)
            z[ii] = points[ii]->z;

        do {
            double bc[3];
            double wmin, wmax;

            if (!ok)
                q--;

            assert(q >= 0);

            if (q == 3) {
                double** A = dalloc2d(10, npoints);
                double w[10];

                for (ii = 0; ii < npoints; ++ii) {
                    double* aii = A[ii];
                    double tmp;

                    triangle_calculatebc(t, points[ii], bc);

                    /*
                     *  0   1   2   3   4   5   6   7   8   9 
                     * 300 210 201 120 111 102 030 021 012 003 
                     */
                    tmp = bc[0] * bc[0];
                    aii[0] = tmp * bc[0];
                    tmp *= 3.0;
                    aii[1] = tmp * bc[1];
                    aii[2] = tmp * bc[2];
                    tmp = bc[1] * bc[1];
                    aii[6] = tmp * bc[1];
                    tmp *= 3.0;
                    aii[3] = tmp * bc[0];
                    aii[7] = tmp * bc[2];
                    tmp = bc[2] * bc[2];
                    aii[9] = tmp * bc[2];
                    tmp *= 3.0;
                    aii[5] = tmp * bc[0];
                    aii[8] = tmp * bc[1];
                    aii[4] = bc[0] * bc[1] * bc[2] * 6.0;
                }

                lsq(A, 10, npoints, z, w, b);

                wmin = w[0];
                wmax = w[0];
                for (ii = 1; ii < 10; ++ii) {
                    if (w[ii] < wmin)
                        wmin = w[ii];
                    else if (w[ii] > wmax)
                        wmax = w[ii];
                }
                if (wmin < wmax / a->k)
                    ok = 0;

                dfree2d(A);

            } else if (q == 2) {
                double** A = dalloc2d(6, npoints);
                double w[6];

                for (ii = 0; ii < npoints; ++ii) {
                    double* aii = A[ii];

                    triangle_calculatebc(t, points[ii], bc);

                    /*
                     *  0   1   2   3   4   5 
                     * 200 110 101 020 011 002 
                     */

                    aii[0] = bc[0] * bc[0];
                    aii[1] = bc[0] * bc[1] * 2.0;
                    aii[2] = bc[0] * bc[2] * 2.0;
                    aii[3] = bc[1] * bc[1];
                    aii[4] = bc[1] * bc[2] * 2.0;
                    aii[5] = bc[2] * bc[2];
                }

                lsq(A, 6, npoints, z, w, b1);

                wmin = w[0];
                wmax = w[0];
                for (ii = 1; ii < 6; ++ii) {
                    if (w[ii] < wmin)
                        wmin = w[ii];
                    else if (w[ii] > wmax)
                        wmax = w[ii];
                }
                if (wmin < wmax / a->k)
                    ok = 0;
                else {          /* degree raising */
                    ok = 1;
                    b[0] = b1[0];
                    b[1] = (b1[0] + 2.0 * b1[1]) / 3.0;
                    b[2] = (b1[0] + 2.0 * b1[2]) / 3.0;
                    b[3] = (b1[3] + 2.0 * b1[1]) / 3.0;
                    b[4] = (b1[1] + b1[2] + b1[4]) / 3.0;
                    b[5] = (b1[5] + 2.0 * b1[2]) / 3.0;
                    b[6] = b1[3];
                    b[7] = (b1[3] + 2.0 * b1[4]) / 3.0;
                    b[8] = (b1[5] + 2.0 * b1[4]) / 3.0;
                    b[9] = b1[5];
                }

                dfree2d(A);

            } else if (q == 1) {
                double** A = dalloc2d(3, npoints);
                double w[3];

                for (ii = 0; ii < npoints; ++ii) {
                    double* aii = A[ii];

                    triangle_calculatebc(t, points[ii], bc);

                    aii[0] = bc[0];
                    aii[1] = bc[1];
                    aii[2] = bc[2];
                }

                lsq(A, 3, npoints, z, w, b1);

                wmin = w[0];
                wmax = w[0];
                for (ii = 1; ii < 3; ++ii) {
                    if (w[ii] < wmin)
                        wmin = w[ii];
                    else if (w[ii] > wmax)
                        wmax = w[ii];
                }
                if (wmin < wmax / a->k)
                    ok = 0;
                else {          /* degree raising */
                    ok = 1;
                    b[0] = b1[0];
                    b[1] = (2.0 * b1[0] + b1[1]) / 3.0;
                    b[2] = (2.0 * b1[0] + b1[2]) / 3.0;
                    b[3] = (2.0 * b1[1] + b1[0]) / 3.0;
                    b[4] = (b1[0] + b1[1] + b1[2]) / 3.0;
                    b[5] = (2.0 * b1[2] + b1[0]) / 3.0;
                    b[6] = b1[1];
                    b[7] = (2.0 * b1[1] + b1[2]) / 3.0;
                    b[8] = (2.0 * b1[2] + b1[1]) / 3.0;
                    b[9] = b1[2];
                }

                dfree2d(A);
            } else if (q == 0) {
                double** A = dalloc2d(1, npoints);
                double w[1];

                for (ii = 0; ii < npoints; ++ii)
                    A[ii][0] = 1.0;

                lsq(A, 1, npoints, z, w, b1);

                ok = 1;
                b[0] = b1[0];
                b[1] = b1[0];
                b[2] = b1[0];
                b[3] = b1[0];
                b[4] = b1[0];
                b[5] = b1[0];
                b[6] = b1[0];
                b[7] = b1[0];
                b[8] = b1[0];
                b[9] = b1[0];

                dfree2d(A);
            }
        } while (!ok);

        n[q]++;
        t->order = q;

        {
            square* s = t->parent;
            double* coeffs = s->coeffs;

            coeffs[12] = b[0];
            coeffs[9] = b[1];
            coeffs[6] = b[3];
            coeffs[3] = b[6];
            coeffs[2] = b[7];
            coeffs[1] = b[8];
            coeffs[0] = b[9];
            coeffs[4] = b[5];
            coeffs[8] = b[2];
            coeffs[5] = b[4];
        }

        free(z);
    }

    if (csa_verbose) {
        fprintf(stderr, "\n  3rd order -- %d sets\n", n[3]);
        fprintf(stderr, "  2nd order -- %d sets\n", n[2]);
        fprintf(stderr, "  1st order -- %d sets\n", n[1]);
        fprintf(stderr, "  0th order -- %d sets\n", n[0]);
        fflush(stderr);
    }

    if (csa_verbose == 2) {
        int j;

        fprintf(stderr, " j\\i");
        for (i = 0; i < a->ni; ++i)
            fprintf(stderr, "%2d ", i);
        fprintf(stderr, "\n");
        for (j = a->nj - 1; j >= 0; --j) {
            fprintf(stderr, "%2d  ", j);
            for (i = 0; i < a->ni; ++i) {
                square* s = a->squares[j][i];

                if (s->triangles[0]->primary)
                    fprintf(stderr, "%2d ", s->triangles[0]->order);
                else
                    fprintf(stderr, " . ");
            }
            fprintf(stderr, "\n");
        }
    }
}

/* Find spline coefficients in (adjacent to primary triangles) secondary
 * triangles from C1 smoothness conditions.
 */
static void csa_findsecondarycoeffs(csa* a)
{
    square*** squares = a->squares;
    int ni = a->ni;
    int nj = a->nj;
    int ii;

    if (csa_verbose) {
        fprintf(stderr, "propagating spline coefficients to the remaining triangles:\n");
        fflush(stderr);
    }

    /*
     * red 
     */
    for (ii = 0; ii < a->npt; ++ii) {
        triangle* t = a->pt[ii];
        square* s = t->parent;
        int i = s->i;
        int j = s->j;
        double* c = s->coeffs;
        double* cl = (i > 0) ? squares[j][i - 1]->coeffs : NULL;
        double* cb = (j > 0) ? squares[j - 1][i]->coeffs : NULL;
        double* cbl = (i > 0 && j > 0) ? squares[j - 1][i - 1]->coeffs : NULL;
        double* ca = (j < nj - 1) ? squares[j + 1][i]->coeffs : NULL;
        double* cal = (j < nj - 1 && i > 0) ? squares[j + 1][i - 1]->coeffs : NULL;

        c[7] = 2.0 * c[4] - c[1];
        c[11] = 2.0 * c[8] - c[5];
        c[15] = 2.0 * c[12] - c[9];

        c[10] = 2.0 * c[6] - c[2];
        c[13] = 2.0 * c[9] - c[5];
        c[16] = 2.0 * c[12] - c[8];

        c[19] = 2.0 * c[15] - c[11];

        if (cl != NULL) {
            cl[21] = c[0];
            cl[22] = c[1];
            cl[23] = c[2];
            cl[24] = c[3];

            cl[18] = c[0] + c[1] - c[4];
            cl[19] = c[1] + c[2] - c[5];
            cl[20] = c[2] + c[3] - c[6];

            cl[17] = 2.0 * cl[20] - cl[23];
            cl[14] = 2.0 * cl[18] - cl[22];
        }

        if (cb != NULL) {
            cb[3] = c[0];
            cb[10] = c[7];

            cb[6] = c[0] + c[7] - c[4];
            cb[2] = 2.0 * cb[6] - cb[10];
        }

        if (cbl != NULL) {
            cbl[23] = cb[2];
            cbl[24] = cb[3];

            cbl[20] = cb[2] + cb[3] - cb[6];
            cbl[17] = cl[14];
        }

        if (ca != NULL) {
            ca[0] = c[3];
            ca[7] = c[10];

            ca[4] = c[3] + c[10] - c[6];
            ca[1] = 2.0 * ca[4] - ca[7];
        }

        if (cal != NULL) {
            cal[21] = c[3];
            cal[22] = ca[1];

            cal[18] = ca[0] + ca[1] - ca[4];
            cal[14] = cl[17];
        }
    }

    /*
     * blue 
     */
    for (ii = 0; ii < a->npt; ++ii) {
        triangle* t = a->pt[ii];
        square* s = t->parent;
        int i = s->i;
        int j = s->j;
        double* c = s->coeffs;
        double* cr = (i < ni - 1) ? squares[j][i + 1]->coeffs : NULL;
        double* car = (i < ni - 1 && j < nj - 1) ? squares[j + 1][i + 1]->coeffs : NULL;
        double* cbr = (i < ni - 1 && j > 0) ? squares[j - 1][i + 1]->coeffs : NULL;

        if (car != NULL)
            cr[13] = car[7] + car[14] - car[11];

        if (cbr != NULL)
            cr[11] = cbr[10] + cbr[17] - cbr[13];

        if (cr != NULL)
            cr[5] = c[22] + c[23] - c[19];
    }

    /*
     * green & yellow 
     */
    for (ii = 0; ii < a->npt; ++ii) {
        triangle* t = a->pt[ii];
        square* s = t->parent;
        int i = s->i;
        int j = s->j;
        double* cr = (i < ni - 1) ? squares[j][i + 1]->coeffs : NULL;

        if (cr != NULL) {
            cr[9] = (cr[5] + cr[13]) / 2.0;
            cr[8] = (cr[5] + cr[11]) / 2.0;
            cr[15] = (cr[11] + cr[19]) / 2.0;
            cr[16] = (cr[13] + cr[19]) / 2.0;
            cr[12] = (cr[8] + cr[16]) / 2.0;
        }
    }

    if (csa_verbose) {
        fprintf(stderr, "checking that all coefficients have been set:\n");
        fflush(stderr);
    }

    for (ii = 0; ii < ni * nj; ++ii) {
        square* s = squares[0][ii];
        double* c = s->coeffs;
        int i;

        if (s->npoints == 0)
            continue;
        for (i = 0; i < 25; ++i)
            if (isnan(c[i]))
                fprintf(stderr, "  squares[%d][%d]->coeffs[%d] = NaN\n", s->j, s->i, i);
    }
}

static int i300[] = { 12, 12, 12, 12 };
static int i030[] = { 3, 24, 21, 0 };
static int i003[] = { 0, 3, 24, 21 };
static int i210[] = { 9, 16, 15, 8 };
static int i021[] = { 2, 17, 22, 7 };
static int i102[] = { 4, 6, 20, 18 };
static int i120[] = { 6, 20, 18, 4 };
static int i012[] = { 1, 10, 23, 14 };
static int i201[] = { 8, 9, 16, 15 };
static int i111[] = { 5, 13, 19, 11 };

static int* iall[] = { i300, i030, i003, i210, i021, i102, i120, i012, i201, i111 };

static void csa_sethascoeffsflag(csa* a)
{
    int i, j;

    for (j = 0; j < a->nj; ++j) {
        for (i = 0; i < a->ni; ++i) {
            square* s = a->squares[j][i];
            double* coeffs = s->coeffs;
            int ii;

            for (ii = 0; ii < 4; ++ii) {
                triangle* t = s->triangles[ii];
                int cc;

                for (cc = 0; cc < 10; ++cc)
                    if (isnan(coeffs[iall[cc][ii]]))
                        break;
                if (cc == 10)
                    t->hascoeffs = 1;
            }
        }
    }
}

void csa_calculatespline(csa* a)
{
    csa_squarize(a, 5);
    csa_attachpoints(a);
    csa_findprimarycoeffs(a);
    csa_findsecondarycoeffs(a);
    csa_sethascoeffsflag(a);
}

void csa_approximate_point(csa* a, point* p)
{
    double h = a->h;
    double ii = (p->x - a->xmin) / h + 1.0;
    double jj = (p->y - a->ymin) / h + 1.0;
    int i, j;
    square* s;
    double fi, fj;
    int ti;
    triangle* t;
    double bc[3];

    if (ii < 0 || jj < 0 || ii > a->ni - 1 || jj > a->nj - 1) {
        p->z = NaN;
        return;
    }

    i = floor(ii);
    j = floor(jj);
    s = a->squares[j][i];
    fi = ii - i;
    fj = jj - j;

    if (fj < fi) {
        if (fi + fj < 1.0)
            ti = 3;
        else
            ti = 2;
    } else {
        if (fi + fj < 1.0)
            ti = 0;
        else
            ti = 1;
    }

    t = s->triangles[ti];
    if (!t->hascoeffs) {
        p->z = NaN;
        return;
    }
    triangle_calculatebc(t, p, bc);

    {
        double* c = s->coeffs;
        double bc1 = bc[0];
        double bc2 = bc[1];
        double bc3 = bc[2];
        double tmp1 = bc1 * bc1;
        double tmp2 = bc2 * bc2;
        double tmp3 = bc3 * bc3;

        switch (ti) {
        case 0:
            p->z = c[12] * bc1 * tmp1 + c[3] * bc2 * tmp2 + c[0] * bc3 * tmp3 + 3.0 * (c[9] * tmp1 * bc2 + c[2] * tmp2 * bc3 + c[4] * tmp3 * bc1 + c[6] * bc1 * tmp2 + c[1] * bc2 * tmp3 + c[8] * tmp1 * bc3) + 6.0 * c[5] * bc1 * bc2 * bc3;
            break;
        case 1:
            p->z = c[12] * bc1 * tmp1 + c[24] * bc2 * tmp2 + c[3] * bc3 * tmp3 + 3.0 * (c[16] * tmp1 * bc2 + c[17] * tmp2 * bc3 + c[6] * tmp3 * bc1 + c[20] * bc1 * tmp2 + c[10] * bc2 * tmp3 + c[9] * tmp1 * bc3) + 6.0 * c[13] * bc1 * bc2 * bc3;
            break;
        case 2:
            p->z = c[12] * bc1 * tmp1 + c[21] * bc2 * tmp2 + c[24] * bc3 * tmp3 + 3.0 * (c[15] * tmp1 * bc2 + c[22] * tmp2 * bc3 + c[20] * tmp3 * bc1 + c[18] * bc1 * tmp2 + c[23] * bc2 * tmp3 + c[16] * tmp1 * bc3) + 6.0 * c[19] * bc1 * bc2 * bc3;
            break;
        default:               /* 3 */
            p->z = c[12] * bc1 * tmp1 + c[0] * bc2 * tmp2 + c[21] * bc3 * tmp3 + 3.0 * (c[8] * tmp1 * bc2 + c[7] * tmp2 * bc3 + c[18] * tmp3 * bc1 + c[4] * bc1 * tmp2 + c[14] * bc2 * tmp3 + c[15] * tmp1 * bc3) + 6.0 * c[11] * bc1 * bc2 * bc3;
        }
    }
}

void csa_approximate_points(csa* a, int n, point* points)
{
    int ii;

    for (ii = 0; ii < n; ++ii)
        csa_approximate_point(a, &points[ii]);
}

void csa_setnmin(csa* a, int nmin)
{
    a->nmin = nmin;
}

void csa_setnmax(csa* a, int nmax)
{
    a->nmax = nmax;
}

void csa_setk(csa* a, int k)
{
    a->k = k;
}

#if defined(STANDALONE)

#define NIMAX 2048
#define BUFSIZE 10240

static void points_generate(double xmin, double xmax, double ymin, double ymax, int nx, int ny, int* nout, point** pout)
{
    double stepx, stepy;
    double x0, xx, yy;
    int i, j, ii;

    if (nx < 1 || ny < 1) {
        *pout = NULL;
        *nout = 0;
        return;
    }

    *nout = nx * ny;
    *pout = malloc(*nout * sizeof(point));

    stepx = (nx > 1) ? (xmax - xmin) / (nx - 1) : 0.0;
    stepy = (ny > 1) ? (ymax - ymin) / (ny - 1) : 0.0;
    x0 = (nx > 1) ? xmin : (xmin + xmax) / 2.0;
    yy = (ny > 1) ? ymin : (ymin + ymax) / 2.0;

    ii = 0;
    for (j = 0; j < ny; ++j) {
        xx = x0;
        for (i = 0; i < nx; ++i) {
            point* p = &(*pout)[ii];

            p->x = xx;
            p->y = yy;
            xx += stepx;
            ii++;
        }
        yy += stepy;
    }
}

static int str2double(char* token, double* value)
{
    char* end = NULL;

    if (token == NULL) {
        *value = NaN;
        return 0;
    }

    *value = strtod(token, &end);

    if (end == token) {
        *value = NaN;
        return 0;
    }

    return 1;
}

/* Reads array of points from a columnar file.
 *
 * @param f File handle
 * @param dim Number of dimensions (must be 2 or 3)
 * @param n Pointer to number of points (output)
 * @param points Pointer to array of points [*n] (output) (to be freed)
 */
static void points_read(FILE* f, int dim, int* n, point** points)
{
    char buf[BUFSIZE];
    char seps[] = " ,;\t";
    char* token;
    int i;

    if (csa_verbose) {
        fprintf(stderr, "reading points:\n");
        fflush(stderr);
    }

    if (dim < 2 || dim > 3) {
        *n = 0;
        *points = NULL;
        return;
    }

    i = 0;
    while (fgets(buf, BUFSIZE, f) != NULL) {
        double v;

        if (buf[0] == '#')
            continue;
        if ((token = strtok(buf, seps)) == NULL)
            continue;
        if (!str2double(token, &v))
            continue;
        if ((token = strtok(NULL, seps)) == NULL)
            continue;
        if (!str2double(token, &v))
            continue;
        if (dim == 3) {
            if ((token = strtok(NULL, seps)) == NULL)
                continue;
            if (!str2double(token, &v))
                continue;
        }
        i++;
    }

    *n = i;
    if (i == 0) {
        *points = NULL;
        return;
    }
    *points = malloc(i * sizeof(point));

    rewind(f);

    i = 0;
    while (fgets(buf, BUFSIZE, f) != NULL && i < *n) {
        point* p = &(*points)[i];

        if (buf[0] == '#')
            continue;
        if ((token = strtok(buf, seps)) == NULL)
            continue;
        if (!str2double(token, &p->x))
            continue;
        if ((token = strtok(NULL, seps)) == NULL)
            continue;
        if (!str2double(token, &p->y))
            continue;
        if (dim == 2)
            p->z = NaN;
        else {
            if ((token = strtok(NULL, seps)) == NULL)
                continue;
            if (!str2double(token, &p->z))
                continue;
        }
        i++;
    }

    if (csa_verbose) {
        fprintf(stderr, "  %d points\n", i);
        fflush(stderr);
    }
}

static void points_write(int n, point* points)
{
    int i;

    if (csa_verbose)
        printf("Output:\n");

    for (i = 0; i < n; ++i) {
        point* p = &points[i];

        printf("%.15g %.15g %.15g\n", p->x, p->y, p->z);
    }
}

/* *INDENT-OFF* */
static void usage()
{
    printf(
"Usage: csabathy -i <XYZ file> {-o <XY file>|-n <nx>x<ny>} [-v|-V]\n"
"Options:\n"
"  -i <XYZ file>   -- three-column file with points to approximate from\n"
"  -o <XY file>    -- two-column file with points to approximate in\n"
"  -n <nx>x<ny>    -- do generate <nx>x<ny> output rectangular grid\n"
"  -v              -- verbose / version\n"
"  -V              -- very verbose / version\n"
"Description:\n"
"  `csabathy' approximates irregular scalar 2D data in specified points using\n"
"  C1-continuous bivariate cubic spline. The calculated values are written to\n"
"  standard output.\n"
);
    exit(0);
}
/* *INDENT-ON* */

static void version()
{
    printf("csa version %s\n", csa_version);
    exit(0);
}

static void parse_commandline(int argc, char* argv[], FILE** fdata, FILE** fpoints, int* generate_points, int* nx, int* ny)
{
    int i;

    if (argc <= 1)
        usage();

    i = 1;
    while (i < argc) {
        switch (argv[i][1]) {
        case 'i':
            i++;
            if (i >= argc)
                csa_quit("no file name found after -i\n");
            if (*fdata == NULL) {
                *fdata = fopen(argv[i], "r");
                if (*fdata == NULL)
                    csa_quit("%s: %s\n", argv[i], strerror(errno));
            }
            i++;
            break;
        case 'n':
            i++;
            if (*fpoints != NULL) {
                fclose(*fpoints);
                *fpoints = NULL;
            }
            *generate_points = 1;
            if (i >= argc)
                csa_quit("no grid dimensions found after -n\n");
            if (sscanf(argv[i], "%dx%d", nx, ny) != 2)
                csa_quit("could not read grid dimensions after \"-n\"\n");
            if (*nx <= 0 || *nx > NIMAX || *ny <= 0 || *ny > NIMAX)
                csa_quit("invalid size for output grid\n");
            i++;
            break;
        case 'o':
            i++;
            if (i >= argc)
                csa_quit("no file name found after -o\n");
            if (*fpoints == NULL) {
                generate_points = 0;
                *fpoints = fopen(argv[i], "r");
                if (*fpoints == NULL)
                    csa_quit("%s: %s\n", argv[i], strerror(errno));
            }
            i++;
            break;
        case 'v':
            i++;
            csa_verbose = 1;
            break;
        case 'V':
            i++;
            csa_verbose = 2;
            break;
        default:
            usage();
            break;
        }
    }

    if (csa_verbose && argc == 2)
        version();
}

int main(int argc, char* argv[])
{
    FILE* fdata = NULL;
    FILE* fpoints = NULL;
    int nin = 0;
    point* pin = NULL;
    int nout = 0;
    int generate_points = 0;
    point* pout = NULL;
    int nx = -1;
    int ny = -1;
    csa* a = NULL;

    parse_commandline(argc, argv, &fdata, &fpoints, &generate_points, &nx, &ny);

    if (fdata == NULL)
        csa_quit("error: no data file specified\n");

    if (!generate_points && fpoints == NULL)
        csa_quit("error: no output grid specified\n");

    points_read(fdata, 3, &nin, &pin);

    if (nin < 3)
        return 0;

    a = csa_create();
    csa_addpoints(a, nin, pin);
    csa_calculatespline(a);

    if (generate_points)
        points_generate(a->xmin - a->h, a->xmax + a->h, a->ymin - a->h, a->ymax + a->h, nx, ny, &nout, &pout);
    else
        points_read(fpoints, 2, &nout, &pout);

    csa_approximate_points(a, nout, pout);

    points_write(nout, pout);

    csa_destroy(a);
    fclose(fdata);
    if (fpoints != NULL)
        fclose(fpoints);
    free(pin);
    free(pout);

    return 0;
}

#endif                          /* STANDALONE */
