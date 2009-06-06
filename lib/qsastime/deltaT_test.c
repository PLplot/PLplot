#include "deltaT.h"
#include <stdio.h>
int dsplint(double *xa, double *ya, double *y2a,
	    int n, double x, double *y, double *dy, double *d2y);
int main()
{
  int iyear;
  double year, delta, ddelta, d2delta;
  for (iyear = -1000; iyear <= 2000; ++iyear) {
    year = (double) iyear;
    dsplint(xspline, yspline, y2spline, number_of_entries_in_spline_tables, year, &delta, &ddelta, &d2delta);
    printf("%9.0f.%9.0f.%15.6E%15.6E\n", year, delta, ddelta, d2delta);
  }
    return 0;
}

