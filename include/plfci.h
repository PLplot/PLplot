/*
  This header file contains the lookup tables used for converting between
  FCIs (font characterization integers) and font names for both the standard
  35 type 1 fonts, and TrueType fonts.

  Copyright (C) 2005  Alan W. Irwin

  This file is part of PLplot.
  
  PLplot is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Library Public License as published
  by the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  PLplot is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Library General Public License for more details.
  
  You should have received a copy of the GNU Library General Public License
  along with PLplot; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/


/* There are no good choices for script fonts for Type 1 so I just
 * defaulted to the Times variants in that case. */

/* There is only one Type 1 symbol font in the standard 35 so use
 * it for all variations. */

/* N.B. When updating this table by hand be sure to keep it in 
 * ascending order in fci! 
 */
#define N_Type1Lookup 30
const FCI_to_FontName_Table Type1Lookup[N_Type1Lookup] = {
     {0x10000000, "Helvetica"},
     {0x10000001, "Times-Roman"},
     {0x10000002, "Courier"},
     {0x10000003, "Times-Roman"},
     {0x10000004, "Symbol"},
     {0x10000010, "Helvetica-Oblique"},
     {0x10000011, "Times-Italic"},
     {0x10000012, "Courier-Oblique"},
     {0x10000013, "Times-Italic"},
     {0x10000014, "Symbol"},
     {0x10000020, "Helvetica-Oblique"},
     {0x10000021, "Times-Italic"},
     {0x10000022, "Courier-Oblique"},
     {0x10000023, "Times-Italic"},
     {0x10000024, "Symbol"},
     {0x10000100, "Helvetica-Bold"},
     {0x10000101, "Times-Bold"},
     {0x10000102, "Courier-Bold"},
     {0x10000103, "Times-Bold"},
     {0x10000104, "Symbol"},
     {0x10000110, "Helvetica-BoldOblique"},
     {0x10000111, "Times-BoldItalic"},
     {0x10000112, "Courier-BoldOblique"},
     {0x10000113, "Times-BoldItalic"},
     {0x10000114, "Symbol"},
     {0x10000120, "Helvetica-BoldOblique"},
     {0x10000121, "Times-BoldItalic"},
     {0x10000122, "Courier-BoldOblique"},
     {0x10000123, "Times-BoldItalic"},
     {0x10000124, "Symbol"}
};

/* N.B. When updating this table by hand be sure to keep it in 
 * ascending order in fci! 
 */
#define N_TrueTypeLookup 30
const FCI_to_FontName_Table TrueTypeLookup[N_TrueTypeLookup] = {
     {0x10000000, PL_FREETYPE_SANS},
     {0x10000001, PL_FREETYPE_SERIF},
     {0x10000002, PL_FREETYPE_MONO},
     {0x10000003, PL_FREETYPE_SCRIPT},
     {0x10000004, PL_FREETYPE_SYMBOL},
     {0x10000010, PL_FREETYPE_SANS_ITALIC},
     {0x10000011, PL_FREETYPE_SERIF_ITALIC},
     {0x10000012, PL_FREETYPE_MONO_ITALIC},
     {0x10000013, PL_FREETYPE_SCRIPT_ITALIC},
     {0x10000014, PL_FREETYPE_SYMBOL_ITALIC},
     {0x10000020, PL_FREETYPE_SANS_OBLIQUE},
     {0x10000021, PL_FREETYPE_SERIF_OBLIQUE},
     {0x10000022, PL_FREETYPE_MONO_OBLIQUE},
     {0x10000023, PL_FREETYPE_SCRIPT_OBLIQUE},
     {0x10000024, PL_FREETYPE_SYMBOL_OBLIQUE},
     {0x10000100, PL_FREETYPE_SANS_BOLD},
     {0x10000101, PL_FREETYPE_SERIF_BOLD},
     {0x10000102, PL_FREETYPE_MONO_BOLD},
     {0x10000103, PL_FREETYPE_SCRIPT_BOLD},
     {0x10000104, PL_FREETYPE_SYMBOL_BOLD},
     {0x10000110, PL_FREETYPE_SANS_BOLD_ITALIC},
     {0x10000111, PL_FREETYPE_SERIF_BOLD_ITALIC},
     {0x10000112, PL_FREETYPE_MONO_BOLD_ITALIC},
     {0x10000113, PL_FREETYPE_SCRIPT_BOLD_ITALIC},
     {0x10000114, PL_FREETYPE_SYMBOL_BOLD_ITALIC},
     {0x10000120, PL_FREETYPE_SANS_BOLD_OBLIQUE},
     {0x10000121, PL_FREETYPE_SERIF_BOLD_OBLIQUE},
     {0x10000122, PL_FREETYPE_MONO_BOLD_OBLIQUE},
     {0x10000123, PL_FREETYPE_SCRIPT_BOLD_OBLIQUE},
     {0x10000124, PL_FREETYPE_SYMBOL_BOLD_OBLIQUE}
};
