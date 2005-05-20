/*
  This header file contains the lookup table used for converting between
  FCIs (font characterization integers) and font names for TrueType fonts.

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
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

/* N.B. When updating this table by hand be sure to keep it in 
 * ascending order in fci! 
 */
#define N_TrueTypeLookup 30
static const FCI_to_FontName_Table TrueTypeLookup[N_TrueTypeLookup] = {
     {PL_FCI_MARK | 0x000, PL_FREETYPE_SANS},
     {PL_FCI_MARK | 0x001, PL_FREETYPE_SERIF},
     {PL_FCI_MARK | 0x002, PL_FREETYPE_MONO},
     {PL_FCI_MARK | 0x003, PL_FREETYPE_SCRIPT},
     {PL_FCI_MARK | 0x004, PL_FREETYPE_SYMBOL},
     {PL_FCI_MARK | 0x010, PL_FREETYPE_SANS_ITALIC},
     {PL_FCI_MARK | 0x011, PL_FREETYPE_SERIF_ITALIC},
     {PL_FCI_MARK | 0x012, PL_FREETYPE_MONO_ITALIC},
     {PL_FCI_MARK | 0x013, PL_FREETYPE_SCRIPT_ITALIC},
     {PL_FCI_MARK | 0x014, PL_FREETYPE_SYMBOL_ITALIC},
     {PL_FCI_MARK | 0x020, PL_FREETYPE_SANS_OBLIQUE},
     {PL_FCI_MARK | 0x021, PL_FREETYPE_SERIF_OBLIQUE},
     {PL_FCI_MARK | 0x022, PL_FREETYPE_MONO_OBLIQUE},
     {PL_FCI_MARK | 0x023, PL_FREETYPE_SCRIPT_OBLIQUE},
     {PL_FCI_MARK | 0x024, PL_FREETYPE_SYMBOL_OBLIQUE},
     {PL_FCI_MARK | 0x100, PL_FREETYPE_SANS_BOLD},
     {PL_FCI_MARK | 0x101, PL_FREETYPE_SERIF_BOLD},
     {PL_FCI_MARK | 0x102, PL_FREETYPE_MONO_BOLD},
     {PL_FCI_MARK | 0x103, PL_FREETYPE_SCRIPT_BOLD},
     {PL_FCI_MARK | 0x104, PL_FREETYPE_SYMBOL_BOLD},
     {PL_FCI_MARK | 0x110, PL_FREETYPE_SANS_BOLD_ITALIC},
     {PL_FCI_MARK | 0x111, PL_FREETYPE_SERIF_BOLD_ITALIC},
     {PL_FCI_MARK | 0x112, PL_FREETYPE_MONO_BOLD_ITALIC},
     {PL_FCI_MARK | 0x113, PL_FREETYPE_SCRIPT_BOLD_ITALIC},
     {PL_FCI_MARK | 0x114, PL_FREETYPE_SYMBOL_BOLD_ITALIC},
     {PL_FCI_MARK | 0x120, PL_FREETYPE_SANS_BOLD_OBLIQUE},
     {PL_FCI_MARK | 0x121, PL_FREETYPE_SERIF_BOLD_OBLIQUE},
     {PL_FCI_MARK | 0x122, PL_FREETYPE_MONO_BOLD_OBLIQUE},
     {PL_FCI_MARK | 0x123, PL_FREETYPE_SCRIPT_BOLD_OBLIQUE},
     {PL_FCI_MARK | 0x124, PL_FREETYPE_SYMBOL_BOLD_OBLIQUE}
};
