//---------------------------------------------------------------------------//
// $Id$
//---------------------------------------------------------------------------//
//
//---------------------------------------------------------------------------//
// Copyright (C) 2003 Andrew Ross <andrewr@coriolis.greenend.org.uk>
// This file is part of PLplot.
//
// This file is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// This file is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with the file; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
//---------------------------------------------------------------------------//
//
//---------------------------------------------------------------------------//
// Implementation of PLplot example 2 in C++.
//---------------------------------------------------------------------------//

#include <cstdlib>
#include <cctype>
#include <iostream>
#include <cmath>

#include "plstream.h"

class x02 {

public:
  x02(int, char **);

private:
  // Class data
  plstream *pls;


};

   
x02::x02( int argc, char ** argv ) {

  int i, j;
  double vmin, vmax;
  char text[3];

  // plplot initialization
  
  pls = new plstream();

  // Divide screen into 16 regions.
  
  pls->ssub(4, 4);
  
  // Parse and process command line arguments.
  pls->ParseOpts( &argc, argv, PL_PARSE_FULL );

  // Initialize PLplot.
  pls->init();

  pls->font(4);

  for (i = 0; i <= 15; i++) {
    pls->col0(i);

    sprintf(text, "%d", i);

    pls->adv(0);
    vmin = 0.1;
    vmax = 0.9;
    for (j = 0; j <= 2; j++) {
      pls->wid(j + 1);
      pls->vpor(vmin, vmax, vmin, vmax);
      pls->wind(0.0, 1.0, 0.0, 1.0);
      pls->box("bc", 0.0, 0, "bc", 0.0, 0);
      vmin = vmin + 0.1;
      vmax = vmax - 0.1;
    }
    pls->wid(1);
    pls->ptex(0.5, 0.5, 1.0, 0.0, 0.5, text);
  }

  //pls->end();
  delete pls;

}

int main( int argc, char ** argv ) {
  x02 *x = new x02( argc, argv );

}


//---------------------------------------------------------------------------//
//                              End of x02.cc
//---------------------------------------------------------------------------//
