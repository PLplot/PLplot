/******************************************************************************
 *
 * File:           nncommon.c
 *
 * Created:        04/08/2000
 *
 * Author:         Pavel Sakov
 *                 CSIRO Marine Research
 *
 * Purpose:        Common stuff for NN interpolation library
 *
 * Description:    None
 *
 * Revisions:      15/11/2002 PS: Changed name from "utils.c"
 *
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <math.h>
#include <values.h>
#include <string.h>
#include "nan.h"
#include "delaunay.h"

#define BUFSIZE 1024

int nn_verbose = 0;
int nn_test_vertice = -1;
NN_ALGORITHM nn_algorithm = SIBSON;

#include "version.h"

void nn_quit(char* format, ...)
{
    va_list args;

    fflush(stdout);             /* just in case, to have the exit message
                                 * last */

    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    exit(1);
}

/* I am not 100% happy with this procedure as it allows loss of precision for
 * small "A"s (see the code) in nearly singular cases. Still, there should be
 * very minor practical difference (if any), so we use this version for now to
 * avoid unnecessary code complication.
 */
int circle_build(point* p0, point* p1, point* p2, circle* c)
{
    double A = p0->x * p1->y + p1->x * p2->y + p2->x * p0->y - p2->x * p1->y - p0->x * p2->y - p1->x * p0->y;
    double r1 = p0->x * p0->x + p0->y * p0->y;
    double r2 = p1->x * p1->x + p1->y * p1->y;
    double r3 = p2->x * p2->x + p2->y * p2->y;
    double B = r1 * p1->y + r2 * p2->y + r3 * p0->y - r3 * p1->y - r1 * p2->y - r2 * p0->y;
    double C = r1 * p1->x + r2 * p2->x + r3 * p0->x - r3 * p1->x - r1 * p2->x - r2 * p0->x;
    double D = r1 * p1->x * p2->y + r2 * p2->x * p0->y + r3 * p0->x * p1->y - r3 * p1->x * p0->y - r1 * p2->x * p1->y - r2 * p0->x * p2->y;

    if (A == 0.0)
        return 0;
    c->x = B / 2.0 / A;
    c->y = -C / 2.0 / A;
    c->r = sqrt(fabs(((B * B + C * C) / 4.0 / A + D) / A));

    return 1;
}

/* This procedure has taken it final shape after a number of tries. The problem
 * was to have the calculated and stored radii being the same if (x,y) is
 * exactly on the circle border (i.e. not to use FCPU extended precision in
 * the radius calculation). This may have little effect in practice but was
 * important in some tests when both input and output data were placed
 * in rectangular grid nodes.
 */
int circle_contains(circle* c, point* p)
{
    return hypot(c->x - p->x, c->y - p->y) <= c->r;
}

/* Smoothes the input point array by averaging the input x,y and z values
 * for each cell within virtual rectangular nx by ny grid. The corners of the
 * grid are created from min and max values of the input array. It also frees
 * the original array and returns results and new dimension via original
 * data and size pointers. 
 *
 * @param pn Pointer to number of points (input/output)
 * @param ppoints Pointer to array of points (input/output) [*pn]
 * @param nx Number of x nodes in decimation
 * @param ny Number of y nodes in decimation
 */
void points_thin(int* pn, point** ppoints, int nx, int ny)
{
    int n = *pn;
    point* points = *ppoints;
    double xmin = MAXDOUBLE;
    double xmax = -MAXDOUBLE;
    double ymin = MAXDOUBLE;
    double ymax = -MAXDOUBLE;
    int nxy = nx * ny;
    double* sumx = calloc(nxy, sizeof(double));
    double* sumy = calloc(nxy, sizeof(double));
    double* sumz = calloc(nxy, sizeof(double));
    int* count = calloc(nxy, sizeof(int));
    double stepx = 0.0;
    double stepy = 0.0;
    int nnew = 0;
    point* pointsnew = NULL;
    int i, j, ii;

    if (nn_verbose)
        fprintf(stderr, "thinned: %d points -> ", *pn);

    if (nx < 1 || ny < 1) {
        free(points);
        *ppoints = NULL;
        *pn = 0;
        if (nn_verbose)
            fprintf(stderr, "0 points");
        return;
    }

    for (ii = 0; ii < n; ++ii) {
        point* p = &points[ii];

        if (p->x < xmin)
            xmin = p->x;
        if (p->x > xmax)
            xmax = p->x;
        if (p->y < ymin)
            ymin = p->y;
        if (p->y > ymax)
            ymax = p->y;
    }

    stepx = (nx > 1) ? (xmax - xmin) / nx : 0.0;
    stepy = (ny > 1) ? (ymax - ymin) / ny : 0.0;

    for (ii = 0; ii < n; ++ii) {
        point* p = &points[ii];
        int index;

        /*
         * Following is the portion of the code which really depends on the
         * floating point particulars. Do not be surprised if different
         * compilers/options give different results here. 
         */
        i = (nx == 1) ? 0 : (p->x - xmin) / stepx;
        j = (ny == 1) ? 0 : (p->y - ymin) / stepy;

        if (i == nx)
            i--;
        if (j == ny)
            j--;
        index = i + j * nx;
        sumx[index] += p->x;
        sumy[index] += p->y;
        sumz[index] += p->z;
        count[index]++;
    }

    for (j = 0; j < ny; ++j) {
        for (i = 0; i < nx; ++i) {
            int index = i + j * nx;

            if (count[index] > 0)
                nnew++;
        }
    }

    pointsnew = malloc(nnew * sizeof(point));

    ii = 0;
    for (j = 0; j < ny; ++j) {
        for (i = 0; i < nx; ++i) {
            int index = i + j * nx;
            int nn = count[index];

            if (nn > 0) {
                point* p = &pointsnew[ii];

                p->x = sumx[index] / nn;
                p->y = sumy[index] / nn;
                p->z = sumz[index] / nn;
                ii++;
            }
        }
    }

    if (nn_verbose)
        fprintf(stderr, "%d points\n", nnew);

    free(sumx);
    free(sumy);
    free(sumz);
    free(count);

    free(points);
    *ppoints = pointsnew;
    *pn = nnew;
}

/* Generates rectangular grid nx by ny using min and max x and y values from
 * the input point array. Allocates space for the output point array, be sure
 * to free it when necessary!
 *
 * @param n Number of points
 * @param points Array of points [n]
 * @param nx Number of x nodes
 * @param ny Number of y nodes
 * @param nout Pointer to number of output points
 * @param pout Pointer to array of output points [*nout]
 */
void points_generate1(int nin, point pin[], int nx, int ny, int* nout, point** pout)
{
    double xmin = MAXDOUBLE;
    double xmax = -MAXDOUBLE;
    double ymin = MAXDOUBLE;
    double ymax = -MAXDOUBLE;
    double stepx, stepy;
    double x0, xx, yy;
    int i, j, ii;

    if (nx < 1 || ny < 1) {
        *pout = NULL;
        *nout = 0;
        return;
    }

    for (ii = 0; ii < nin; ++ii) {
        point* p = &pin[ii];

        if (p->x < xmin)
            xmin = p->x;
        if (p->x > xmax)
            xmax = p->x;
        if (p->y < ymin)
            ymin = p->y;
        if (p->y > ymax)
            ymax = p->y;
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

/* Generates rectangular grid nx by ny using specified min and max x and y 
 * values. Allocates space for the output point array, be sure to free it
 * when necessary!
 *
 * @param xmin Min x value
 * @param xmax Max x value
 * @param ymin Min y value
 * @param ymax Max y value
 * @param nx Number of x nodes
 * @param ny Number of y nodes
 * @param nout Pointer to number of output points
 * @param pout Pointer to array of output points [*nout]
 */
void points_generate2(double xmin, double xmax, double ymin, double ymax, int nx, int ny, int* nout, point** pout)
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
void points_read(FILE* f, int dim, int* n, point** points)
{
    char buf[BUFSIZE];
    char seps[] = " ,;\t";
    char* token;
    int i;

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
}
