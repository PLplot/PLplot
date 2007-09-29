//---------------------------------------------------------------------------//
// $Id$
// Multi-lingual version of the first page of example 4.
//---------------------------------------------------------------------------//
//
//---------------------------------------------------------------------------//
// Copyright (C) 2006  Alan W. Irwin
// Copyright (C) 2006  Andrew Ross 
//
// Thanks to the following for providing translated strings for this example:
// Valery Pipin (Russian)
//
// This file is part of PLplot.
//
// PLplot is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// PLplot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with PLplot; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
//---------------------------------------------------------------------------//

//  This example designed just for devices (e.g., psttfc and the
//  cairo-related devices) that use the pango and fontconfig libraries. The
//  best choice of glyph is selected by fontconfig and automatically rendered
//  by pango in way that is sensitive to complex text layout (CTL) language
//  issues for each unicode character in this example. Of course, you must
//  have the appropriate TrueType fonts installed to have access to all the
//  required glyphs.
//
//  Translation instructions: The strings to be translated are given by
//  x_label, y_label, alty_label, title_label, and line_label below.  The
//  encoding used must be UTF-8.
//
//  The following strings to be translated involve some scientific/mathematical
//  jargon which is now discussed further to help translators.
//
//  (1) dB is a decibel unit, see http://en.wikipedia.org/wiki/Decibel .
//  (2) degrees is an angular measure, see
//        http://en.wikipedia.org/wiki/Degree_(angle) .
//  (3) low-pass filter is one that transmits (passes) low frequencies.
//  (4) pole is in the mathematical sense, see
//      http://en.wikipedia.org/wiki/Pole_(complex_analysis) .  "Single Pole"
//      means a particular mathematical transformation of the filter function has
//      a single pole, see
//      http://ccrma.stanford.edu/~jos/filters/Pole_Zero_Analysis_I.html .
//      Furthermore, a single-pole filter must have an inverse square decline
//      (or -20 db/decade). Since the filter plotted here does have that
//      characteristic, it must by definition be a single-pole filter, see also
//      http://www-k.ext.ti.com/SRVS/Data/ti/KnowledgeBases/analog/document/faqs/1p.htm
//  (5) decade represents a factor of 10, see
//      http://en.wikipedia.org/wiki/Decade_(log_scale) .


//---------------------------------------------------------------------------//
// Implementation of PLplot example 26 in C++.
//---------------------------------------------------------------------------//

#include "plc++demos.h"

#ifdef USE_NAMESPACE
using namespace std;
#endif


static char *x_label[] = {
  "Frequency",
  "Частота",
  NULL
};

static char *y_label[] = {
  "Amplitude (dB)",
  "Амплитуда (dB)",
  NULL
};

static char *alty_label[] = {
  "Phase shift (degrees)",
  "Фазовый сдвиг (градусы)",
  NULL
};

static char *title_label[] = {
  "Single Pole Low-Pass Filter",
  "Однополюсный Низко-Частотный Фильтр",
  NULL
};

static char *line_label[] = {
  "-20 dB/decade",
  "-20 dB/десяток",
  NULL
};

class x26 {

public:
  x26(int, char **);
  void plot1(int, char*, char*, char*, char*, char*);

private:
  // Class data
  plstream *pls;


};


x26::x26( int argc, char ** argv ) {

  int i;

  // plplot initialization

  pls = new plstream();

  // Parse and process command line arguments.
  pls->parseopts( &argc, argv, PL_PARSE_FULL );

  // Initialize PLplot.
  pls->init();
  pls->font(2);

  // Make log plots using two different styles.

  i = 0;
  while (x_label[i] != NULL) {
      plot1(0, x_label[i], y_label[i], alty_label[i], title_label[i],
                line_label[i]);
      i++;
  }

  // pls->end();
  delete pls;
}

// Log-linear plot.

void x26::plot1( int type, char *x_label, char *y_label, char *alty_label,
           char *title_label, char *line_label ) {
  int i;
  PLFLT *freql = new PLFLT[101];
  PLFLT *ampl  = new PLFLT[101];
  PLFLT *phase = new PLFLT[101];
  PLFLT f0, freq;

  pls->adv(0);

  // Set up data for log plot.

  f0 = 1.0;
  for (i = 0; i <= 100; i++) {
    freql[i] = -2.0 + i / 20.0;
    freq = pow(10.0, (double)freql[i]);
    ampl[i] = 20.0 * log10(1.0 / sqrt(1.0 + pow((double)(freq / f0), 2.)));
    phase[i] = -(180.0 / M_PI) * atan((double)(freq / f0));
  }

  pls->vpor(0.15, 0.85, 0.1, 0.9);
  pls->wind(-2.0, 3.0, -80.0, 0.0);

  // Try different axis and labelling styles.

  pls->col0(1);
  switch (type) {
  case 0:
    pls->box("bclnst", 0.0, 0, "bnstv", 0.0, 0);
    break;
  case 1:
    pls->box("bcfghlnst", 0.0, 0, "bcghnstv", 0.0, 0);
    break;
  }

  // Plot ampl vs freq.

  pls->col0(2);
  pls->line(101,freql, ampl);
  pls->col0(1);
  pls->ptex(1.6, -30.0, 1.0, -20.0, 0.5, line_label);

  // Put labels on.

  pls->col0(1);
  pls->mtex("b", 3.2, 0.5, 0.5, x_label);
  pls->mtex("t", 2.0, 0.5, 0.5, title_label);
  pls->col0(2);
  pls->mtex("l", 5.0, 0.5, 0.5, y_label);

  // For the gridless case, put phase vs freq on same plot.

  if (type == 0) {
    pls->col0(1);
    pls->wind(-2.0, 3.0, -100.0, 0.0);
    pls->box("", 0.0, 0, "cmstv", 30.0, 3);
    pls->col0(3);
    pls->line(101,freql, phase);
    pls->col0(3);
    pls->mtex("r", 5.0, 0.5, 0.5, alty_label);
  }

  delete[] freql;
  delete[] ampl;
  delete[] phase;
}


int main( int argc, char ** argv ) {
  x26 *x = new x26( argc, argv );

  delete x;
}


//---------------------------------------------------------------------------//
//                              End of x26.cc
//---------------------------------------------------------------------------//
