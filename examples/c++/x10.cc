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
// Implementation of PLplot example 10 in C++.
//---------------------------------------------------------------------------//

#include <cstdlib>
#include <cctype>
#include <iostream>
#include <cmath>

#include "plstream.h"

using namespace std;

class x10 {

public:
  x10(int, char**);

private:
  plstream *pls;

};

x10::x10( int argc, char **argv ) {

  pls = new plstream();

  // Parse and process command line arguments.
  
  pls->ParseOpts( &argc, argv, PL_PARSE_FULL );

  // Initialize plplot.
  
  pls->init();

  pls->adv(0);
  pls->vpor(0.0, 1.0, 0.0, 1.0);
  pls->wind(0.0, 1.0, 0.0, 1.0);
  pls->box("bc", 0.0, 0, "bc", 0.0, 0);

  pls->svpa(50.0, 150.0, 50.0, 100.0);
  pls->wind(0.0, 1.0, 0.0, 1.0);
  pls->box("bc", 0.0, 0, "bc", 0.0, 0);
  pls->ptex(0.5, 0.5, 1.0, 0.0, 0.5, "BOX at (50,150,50,100)");
  //  pls->end();
  delete pls;
}



int main( int argc, char **argv ) 
{
  x10 *x = new x10( argc, argv );
}


//---------------------------------------------------------------------------//
//                              End of x10.cc
//---------------------------------------------------------------------------//
