# Stub subroutines for octave file plplot_octave.cc.
# This file was automatically generated for octave by wrap_octave on
# Sat Jan  6 18:17:58 2001 from
# plplot.h.
#
# It was also massaged to add online documentation
# extracted from some PLplot distribution files

1;

function pl_setcontlabelformat(lexp, sigdig)
% pl_setcontlabelformat(lexp, sigdig)
%
  plplot_octave(0, lexp, sigdig);
endfunction

function pl_setcontlabelparam(offset, size, spacing, active)
% pl_setcontlabelparam(offset, size, spacing, active)
%
  plplot_octave(1, offset, size, spacing, active);
endfunction

function pladv(page)
% pladv(page)
%
%   Original PLplot call documentation:
%
%   pladv(sub)
%   **********
% 
%   pladv
% 
%   Advances to the next subpage if sub=0, performing a page advance if there
%   are no remaining subpages on the current page. If subwindowing isn't being
%   used, pladv(0) will always advance the page. If sub>0, PLplot switches to
%   the specified subpage. Note that this allows you to overwrite a plot on the
%   specified subpage; if this is not what you intended, use plclr followed by
%   plpage to first advance the page. This routine is called automatically
%   (with sub=0) by plenv, but if plenv is not used, pladv must be called after
%   initializing PLplot but before defining the viewport.
% 
%   sub (PLINT, input): Specifies the subpage number (starting from 1 in the
%   top left corner and increasing along the rows) to which to advance. Set to
%   zero to advance to the next subpage.
% 
% 
  plplot_octave(2, page);
endfunction

function plaxes(x0, y0, xopt, xtick, nxsub, yopt, ytick, nysub)
% plaxes(x0, y0, xopt, xtick, nxsub, yopt, ytick, nysub)
%
% This functions similarly to plbox except that the origin of the axes  is placed at the user-specified point x0, y0  
  plplot_octave(3, x0, y0, xopt, xtick, nxsub, yopt, ytick, nysub);
endfunction

function plbin(x, y, center)
% plbin(x, y, center)
%
%   Original PLplot call documentation:
%
%   plbin(nbin, x, y, cen)
%   **********************
% 
%   plbin
% 
%   Plots a histogram consisting of n bins. The value associated with the
%   i'th bin is placed in x[i], and the number of points in the bin is placed
%   in y[i]. For proper operation, the values in x[i] must form a strictly
%   increasing sequence. If center is false, x[i] is the left-hand edge of the
%   i'th bin, and if center is true, the bin boundaries are placed midway
%   between the values in the x array. Also see plhist for drawing histograms
%   from unbinned data.
% 
%   nbin (PLINT, input): Number of bins (i.e., number of values in _x and _y
%   arrays.)
% 
%   x (PLFLT *, input): Pointer to array containing values associated with
%   bins. These must form a strictly increasing sequence.
% 
%   y (PLFLT *, input): Pointer to array containing number of points in bin.
%   This is a PLFLT (instead of PLINT) array so as to allow histograms of
%   probabilities, etc.
% 
%   cen (PLINT, input): Indicates whether the values in x represent the
%   lower bin boundaries (cen=0) or whether the bin boundaries are to be midway
%   between the x values (cen=1). If the values in x are equally spaced and
%   cen=1, the values in x are the center values of the bins.
% 
% 
  plplot_octave(4, x, y, center);
endfunction

function plbop()
% plbop()
%
% Start new page.  Should only be used with pleop.  
  plplot_octave(5);
endfunction

function plbox(xopt, xtick, nxsub, yopt, ytick, nysub)
% plbox(xopt, xtick, nxsub, yopt, ytick, nysub)
%
%   Original PLplot call documentation:
%
%   plbox(xopt, xtick, nxsub, yopt, ytick, nysub)
%   *********************************************
% 
%   plbox
% 
%   Draws a box around the currently defined viewport, and labels it with
%   world coordinate values appropriate to the window. Thus plbox should only
%   be called after defining both viewport and window. The character strings
%   xopt and yopt specify how the box should be drawn as described below. If
%   ticks and/or subticks are to be drawn for a particular axis, the tick
%   intervals and number of subintervals may be specified explicitly, or they
%   may be defaulted by setting the appropriate arguments to zero.
% 
%   xopt (char *, input): Pointer to character string specifying options for
%   horizontal axis. The string can include any combination of the following
%   letters (upper or lower case) in any order:
% 
%    o a: Draws axis, X-axis is horizontal line y=0, and Y-axis is vertical line
%      x=0.
%    o b: Draws bottom (X) or left (Y) edge of frame.
%    o c: Draws top (X) or right (Y) edge of frame.
%    o g: Draws a grid at the major tick interval.
%    o i: Inverts tick marks, so they are drawn outwards, rather than inwards.
%    o l: Labels axis logarithmically. This only affects the labels, not the
%      data, and so it is necessary to compute the logarithms of data points
%      before passing them to any of the drawing routines.
%    o m: Writes numeric labels at major tick intervals in the unconventional
%      location (above box for X, right of box for Y).
%    o n: Writes numeric labels at major tick intervals in the conventional
%      location (below box for X, left of box for Y).
%    o s: Enables subticks between major ticks, only valid if t is also
%      specified.
%    o t: Draws major ticks.
% 
%   xtick (PLFLT, input): World coordinate interval between major ticks on
%   the x axis. If it is set to zero, PLplot automatically generates a suitable
%   tick interval.
% 
%   nxsub (PLINT, input): Number of subintervals between major x axis ticks
%   for minor ticks. If it is set to zero, PLplot automatically generates a
%   suitable minor tick interval.
% 
%   yopt (char *, input): Pointer to character string specifying options for
%   vertical axis. The string can include any combination of the letters
%   defined above for xopt, and in addition may contain:
% 
%    o v: Write numeric labels for vertical axis parallel to the base of the
%      graph, rather than parallel to the axis.
% 
%   ytick (real, input): World coordinate interval between major ticks on
%   the y axis. If it is set to zero, PLplot automatically generates a suitable
%   tick interval.
% 
%   nysub (PLINT, input): Number of subintervals between major y axis ticks
%   for minor ticks. If it is set to zero, PLplot automatically generates a
%   suitable minor tick interval.
% 
% 
  plplot_octave(6, xopt, xtick, nxsub, yopt, ytick, nysub);
endfunction

function plbox3(xopt, xlabel, xtick, nsubx, yopt, ylabel, ytick, nsuby, zopt, zlabel, ztick, nsubz)
% plbox3(xopt, xlabel, xtick, nsubx, yopt, ylabel, ytick, nsuby, zopt, zlabel, ztick, nsubz)
%
%   Original PLplot call documentation:
%
%   plbox3(xopt, xlabel, xtick, nxsub, yopt, ylabel, ytick, nysub, zopt, zlabel,
%   ****************************************************************************
%   ztick, nzsub)
%   *************
% 
%   plbox3
% 
%   Draws axes, numeric and text labels for a three-dimensional surface plot.
%   For a more complete description of three-dimensional plotting 3-d Plots.
% 
%   xopt (char *, input): Pointer to character string specifying options for
%   the x axis. The string can include any combination of the following letters
%   (upper or lower case) in any order:
% 
%    o b: Draws axis at base, at height z=zmin where zmin is defined by call to
%      plw3d. This character must be specified in order to use any of the other
%      options.
%    o i: Inverts tick marks, so they are drawn downwards, rather than upwards.
%    o l: Labels axis logarithmically. This only affects the labels, not the
%      data, and so it is necessary to compute the logarithms of data points
%      before passing them to any of the drawing routines.
%    o n: Writes numeric labels at major tick intervals.
%    o s: Enables subticks between major ticks, only valid if t is also
%      specified.
%    o t: Draws major ticks.
%    o u: If this is specified, the text label for the axis is written under the
%      axis.
% 
%   xlabel (char *, input): Pointer to character string specifying text
%   label for the x axis. It is only drawn if u is in the xopt string.
% 
%   xtick (PLFLT, input): World coordinate interval between major ticks on
%   the x axis. If it is set to zero, PLplot automatically generates a suitable
%   tick interval.
% 
%   nxsub (PLINT, input): Number of subintervals between major x axis ticks
%   for minor ticks. If it is set to zero, PLplot automatically generates a
%   suitable minor tick interval.
% 
%   yopt (char *, input): Pointer to character string specifying options for
%   the y axis. The string is interpreted in the same way as xopt.
% 
%   ylabel (char *, input): Pointer to character string specifying text
%   label for the y axis. It is only drawn if u is in the yopt string.
% 
%   ytick (PLFLT, input): World coordinate interval between major ticks on
%   the y axis. If it is set to zero, PLplot automatically generates a suitable
%   tick interval.
% 
%   nysub (PLINT, input): Number of subintervals between major y axis ticks
%   for minor ticks. If it is set to zero, PLplot automatically generates a
%   suitable minor tick interval.
% 
%   zopt (char *, input): Pointer to character string specifying options for
%   the z axis. The string can include any combination of the following letters
%   (upper or lower case) in any order:
% 
%    o b: Draws z axis to the left of the surface plot.
%    o c: Draws z axis to the right of the surface plot.
%    o d: Draws grid lines parallel to the x-y plane behind the figure. These
%      lines are not drawn until after plot3d or plmesh are called because of
%      the need for hidden line removal.
%    o i: Inverts tick marks, so they are drawn away from the center.
%    o l: Labels axis logarithmically. This only affects the labels, not the
%      data, and so it is necessary to compute the logarithms of data points
%      before passing them to any of the drawing routines.
%    o m: Writes numeric labels at major tick intervals on the right-hand
%      verical axis.
%    o n: Writes numeric labels at major tick intervals on the left-hand verical
%      axis.
%    o s: Enables subticks between major ticks, only valid if t is also
%      specified.
%    o t: Draws major ticks.
%    o u: If this is specified, the text label is written beside the left-hand
%      axis.
%    o v: If this is specified, the text label is written beside the right-hand
%      axis.
% 
%   zlabel (char *, input): Pointer to character string specifying text
%   label for the z axis. It is only drawn if u or v are in the zopt string.
% 
%   ztick (PLFLT, input): World coordinate interval between major ticks on
%   the z axis. If it is set to zero, PLplot automatically generates a suitable
%   tick interval.
% 
%   nzsub (PLINT, input): Number of subintervals between major z axis ticks
%   for minor ticks. If it is set to zero, PLplot automatically generates a
%   suitable minor tick interval.
% 
% 
  plplot_octave(7, xopt, xlabel, xtick, nsubx, yopt, ylabel, ytick, nsuby, zopt, zlabel, ztick, nsubz);
endfunction

function plcol0(icol0)
% plcol0(icol0)
%
% Set color, map 0.  Argument is eger between 0 and 15.  
  plplot_octave(8, icol0);
endfunction

function plcol1(col1)
% plcol1(col1)
%
% Set color, map 1.  Argument is a float between 0. and 1.  
  plplot_octave(9, col1);
endfunction

function plcpstrm(iplsr, flags)
% plcpstrm(iplsr, flags)
%
% Copies state parameters from the reference stream to the current stream.  
  plplot_octave(10, iplsr, flags);
endfunction

function plend()
% plend()
%
%   Original PLplot call documentation:
%
%   plend()
%   *******
% 
%   plend
% 
%   Ends a plotting session, tidies up all the output files, switches
%   interactive devices back into text mode and frees up any memory that was
%   allocated. Must be called before end of program.
% 
% 
  plplot_octave(11);
endfunction

function plend1()
% plend1()
%
%   Original PLplot call documentation:
%
%   plend1()
%   ********
% 
%   plend1
% 
%   Ends a plotting session for the current output stream only. See plsstrm
%   for more info.
% 
% 
  plplot_octave(12);
endfunction

function plenv(xmin, xmax, ymin, ymax, just, axis)
% plenv(xmin, xmax, ymin, ymax, just, axis)
%
%   Original PLplot call documentation:
%
%   plenv(xmin, xmax, ymin, ymax, just, axis)
%   *****************************************
% 
%   plenv
% 
%   Sets up plotter environment for simple graphs by calling pladv and
%   setting up viewport and window to sensible default values. plenv leaves
%   enough room around most graphs for axis labels and a title. When these
%   defaults are not suitable, use the individual routines plvspa, plvpor, or
%   plvasp for setting up the viewport, plwind for defining the window, and
%   plbox for drawing the box.
% 
%   xmin (PLFLT, input): Value of x at left-hand edge of window.
% 
%   xmax (PLFLT, input): Value of x at right-hand edge of window.
% 
%   ymin (PLFLT, input): Value of y at bottom edge of window.
% 
%   ymax (PLFLT, input): Value of y at top edge of window.
% 
%   just (PLINT, input): If just=0, the x and y axes are scaled
%   independently to use as much of the screen as possible, but if just=1, the
%   scales of the x and y axes are made equal.
% 
%   axis (PLINT, input): Controls drawing of the box around the plot:
% 
%    o -2: No box or annotation.
%    o -1: Draw box only.
%    o 0: Draw box, labelled with coordinate values around edge.
%    o 1: In addition to box and labels, draw the two axes x=0 and y=0.
%    o 2: As for axis=1, but also draw a grid at the major tick interval.
%    o 10: Logarithmic x axis, linear y axis.
%    o 11: Logarithmic x axis, linear y axis and draw line y=0.
%    o 20: Linear x axis, logarithmic y axis.
%    o 21: Linear x axis, logarithmic y axis and draw line x=0.
%    o 30: Logarithmic x and y axes.
% 
% 
  plplot_octave(13, xmin, xmax, ymin, ymax, just, axis);
endfunction

function pleop()
% pleop()
%
% End current page.  Should only be used with plbop.  
  plplot_octave(14);
endfunction

function plerrx(xmin, xmax, y)
% plerrx(xmin, xmax, y)
%
%   Original PLplot call documentation:
%
%   plerrx(n, xmin, xmax, y)
%   ************************
% 
%   plerrx
% 
%   Draws a set of n horizontal error bars, the i'th error bar extending from
%   xmin[i] to xmax[i] at y coordinate y[i]. The terminals of the error bar are
%   of length equal to the minor tick length (settable using plsmin).
% 
%   n (PLINT, input): Number of error bars to draw.
% 
%   xmin (PLFLT *, input): Pointer to array with x coordinates of left-hand
%   endpoint of error bars.
% 
%   xmax (PLFLT *, input): Pointer to array with x coordinates of right-hand
%   endpoint of error bars.
% 
%   y (PLFLT *, input): Pointer to array with y coordinates of error bar.
% 
% 
  plplot_octave(15, xmin, xmax, y);
endfunction

function plerry(x, ymin, ymax)
% plerry(x, ymin, ymax)
%
%   Original PLplot call documentation:
%
%   plerry(n, x, ymin, ymax)
%   ************************
% 
%   plerry
% 
%   Draws a set of n vertical error bars, the i'th error bar extending from
%   ymin[i] to ymax[i] at x coordinate x[i]. The terminals of the error bar are
%   of length equal to the minor tick length (settable using plsmin).
% 
%   n (PLINT, input): Number of error bars to draw.
% 
%   x (PLFLT *, input): Pointer to array with x coordinates of error bars.
% 
%   ymin (PLFLT *, input): Pointer to array with y coordinates of lower
%   endpoint of error bars.
% 
%   ymax (PLFLT *, input): Pointer to array with y coordinate of upper
%   endpoint of error bar.
% 
% 
  plplot_octave(16, x, ymin, ymax);
endfunction

function plfamadv()
% plfamadv()
%
% Advance to the next family file on the next new page  
  plplot_octave(17);
endfunction

function plfill(x, y)
% plfill(x, y)
%
%   Original PLplot call documentation:
%
%   plfill(n, x, y)
%   ***************
% 
%   plfill
% 
%   Fills the polygon defined by the n points (x[i], y[i]) using the pattern
%   defined by plpsty or plpat. The routine will automatically close the
%   polygon between the last and first vertices. If multiple closed polygons
%   are passed in x and y then plfill will fill in between them.
% 
%   n (PLINT, input): Number of vertices in polygon.
% 
%   x (PLFLT *, input): Pointer to array with x coordinates of vertices.
% 
%   y (PLFLT *, input): Pointer to array with y coordinates of vertices.
% 
% 
  plplot_octave(18, x, y);
endfunction

function plfill3(x, y, z)
% plfill3(x, y, z)
%
  plplot_octave(19, x, y, z);
endfunction

function plflush()
% plflush()
%
% Flushes the output stream.  Use sparingly, if at all.  
  plplot_octave(20);
endfunction

function plfont(ifont)
% plfont(ifont)
%
%   Original PLplot call documentation:
%
%   plfont(font)
%   ************
% 
%   plfont
% 
%   Sets the default character font for subsequent character drawing. Also
%   affects symbols produced by plpoin.This routine has no effect unless the
%   extended character set is loaded (see plfontld).
% 
%   font (PLINT, input): Specifies the font:
% 
%    o 1: Normal font (simplest and fastest)
%    o 2: Roman font
%    o 3: Italic font
%    o 4: Script font
% 
% 
  plplot_octave(21, ifont);
endfunction

function plfontld(fnt)
% plfontld(fnt)
%
%   Original PLplot call documentation:
%
%   plfontld(set)
%   *************
% 
%   plfontld
% 
%   Sets the character set to use for subsequent character drawing. May be
%   called before calling initializing PLplot.
% 
%   set (PLINT, input): Specifies the character set to load:
% 
%    o 0: Standard character set
%    o 1: Extended character set
% 
% 
  plplot_octave(22, fnt);
endfunction

function [p_def, p_ht] = plgchr()
% [p_def, p_ht] = plgchr()
%
% Get character default height and current -scaled- height  
  [p_def, p_ht] = plplot_octave(23);
endfunction

function [r, g, b] = plgcol0(icol0)
% [r, g, b] = plgcol0(icol0)
%
% Returns 8 bit RGB values for given color from color map 0  
  [r, g, b] = plplot_octave(24, icol0);
endfunction

function [r, g, b] = plgcolbg()
% [r, g, b] = plgcolbg()
%
% Returns the background color by 8 bit RGB value  
  [r, g, b] = plplot_octave(25);
endfunction

function p_dev = plgdev()
% p_dev = plgdev()
%
  p_dev = plplot_octave(26);
endfunction

function [p_mar, p_aspect, p_jx, p_jy] = plgdidev()
% [p_mar, p_aspect, p_jx, p_jy] = plgdidev()
%
% etrieve current window into device space
  [p_mar, p_aspect, p_jx, p_jy] = plplot_octave(27);
endfunction

function p_rot = plgdiori()
% p_rot = plgdiori()
%
% Get plot orientation  
  p_rot = plplot_octave(28);
endfunction

function [p_xmin, p_ymin, p_xmax, p_ymax] = plgdiplt()
% [p_xmin, p_ymin, p_xmax, p_ymax] = plgdiplt()
%
% Retrieve current window into plot space  
  [p_xmin, p_ymin, p_xmax, p_ymax] = plplot_octave(29);
endfunction

function [p_fam, p_num, p_bmax] = plgfam()
% [p_fam, p_num, p_bmax] = plgfam()
%
%   Original PLplot call documentation:
%
%   plgfam(fam, num, bmax)
%   **********************
% 
%   plgfam
% 
%   Gets information about current family file, if familying is enabled. See
%   Section [*] for more information.
% 
%   fam (PLINT *, output): Pointer to variable with the boolean family flag
%   value. If nonzero, familying is enabled.
% 
%   num (PLINT *, output): Pointer to variable with the current family file
%   number.
% 
%   bmax (PLINT *, output): Pointer to variable with the maximum file size
%   (in bytes) for a family file.
% 
% 
  [p_fam, p_num, p_bmax] = plplot_octave(30);
endfunction

function fnam = plgfnam()
% fnam = plgfnam()
%
%   Original PLplot call documentation:
%
%   plgfnam(fnam)
%   *************
% 
%   plgfnam
% 
%   Gets the current output file name, if applicable.
% 
%   fnam (char *, output): Pointer to file name string.
% 
% 
  fnam = plplot_octave(31);
endfunction

function p_level = plglevel()
% p_level = plglevel()
%
% et the (current) run level
  p_level = plplot_octave(32);
endfunction

function [p_xp, p_yp, p_xleng, p_yleng, p_xoff, p_yoff] = plgpage()
% [p_xp, p_yp, p_xleng, p_yleng, p_xoff, p_yoff] = plgpage()
%
%   Original PLplot call documentation:
%
%   plgpage(xp, yp, xleng, yleng, xoff, yoff)
%   *****************************************
% 
%   plgpage
% 
%   Gets the current page configuration.
% 
%   xp (PLINT *, output): Pointer to number of pixels in x.
% 
%   yp (PLINT *; output): Pointer to number of pixels in y.
% 
%   xleng (PLINT *, output): Pointer to x page length value.
% 
%   yleng (PLINT *, output): Pointer to y page length value.
% 
%   xoff (PLINT *, output): Pointer to x page offset.
% 
%   yoff (PLINT *, output): Pointer to y page offset.
% 
% 
  [p_xp, p_yp, p_xleng, p_yleng, p_xoff, p_yoff] = plplot_octave(33);
endfunction

function plgra()
% plgra()
%
%   Original PLplot call documentation:
%
%   plgra()
%   *******
% 
%   plgra
% 
%   Sets an interactive device to graphics mode, used in conjunction with
%   pltext to allow graphics and text to be interspersed. On a device which
%   supports separate text and graphics windows, this command causes control to
%   be switched to the graphics window. If already in graphics mode, this
%   command is ignored. It is also ignored on devices which only support a
%   single window or use a different method for shifting focus. See also
%   pltext.
% 
% 
  plplot_octave(34);
endfunction

function [xmin, xmax, ymin, ymax] = plgspa()
% [xmin, xmax, ymin, ymax] = plgspa()
%
%   Original PLplot call documentation:
%
%   plgspa(xmin, xmax, ymin, ymax)
%   ******************************
% 
%   plgspa
% 
%   Gets the size of the current subpage in millimeters measured from the
%   bottom left hand corner of the output device page or screen. Can be used in
%   conjunction with plsvpa for setting the size of a viewport in absolute
%   coordinates (millimeters).
% 
%   xmin (PLFLT *, output): Pointer to variable with position of left hand
%   edge of subpage in millimeters.
% 
%   xmax (PLFLT *, output): Pointer to variable with position of right hand
%   edge of subpage in millimeters.
% 
%   ymin (PLFLT *, output): Pointer to variable with position of bottom edge
%   of subpage in millimeters.
% 
%   ymax (PLFLT *, output): Pointer to variable with position of top edge of
%   subpage in millimeters.
% 
% 
  [xmin, xmax, ymin, ymax] = plplot_octave(35);
endfunction

function p_strm = plgstrm()
% p_strm = plgstrm()
%
%   Original PLplot call documentation:
%
%   plgstrm(strm)
%   *************
% 
%   plgstrm
% 
%   Gets the number of the current output stream. See also plsstrm.
% 
%   strm (PLINT *, output): Pointer to current stream value.
% 
% 
  p_strm = plplot_octave(36);
endfunction

function p_ver = plgver()
% p_ver = plgver()
%
% Get the current library version number  
  p_ver = plplot_octave(37);
endfunction

function [p_digmax, p_digits] = plgxax()
% [p_digmax, p_digits] = plgxax()
%
%   Original PLplot call documentation:
%
%   plgxax(digmax, digits)
%   **********************
% 
%   plgxax
% 
%   Returns current values of the digmax and digits flags for the x axis.
%   digits is updated after the plot is drawn, so so this routine should only
%   be called _a_f_t_e_r the call to plbox (or plbox3) is complete. Annotation for
%   more information.
% 
%   digmax (PLINT *, output): Pointer to variable with the maximum number of
%   digits. If nonzero, the printed label will be switched to a floating point
%   representation when the number of digits exceeds digmax.
% 
%   digits (PLINT *, output): Pointer to variable with the actual number of
%   digits for the numeric labels (x axis) from the last plot.
% 
% 
  [p_digmax, p_digits] = plplot_octave(38);
endfunction

function [p_digmax, p_digits] = plgyax()
% [p_digmax, p_digits] = plgyax()
%
%   Original PLplot call documentation:
%
%   plgyax(digmax, digits)
%   **********************
% 
%   plgyax
% 
%   Identical to plgxax, except that arguments are flags for y axis. See the
%   description of plgxax for more detail.
% 
% 
  [p_digmax, p_digits] = plplot_octave(39);
endfunction

function [p_digmax, p_digits] = plgzax()
% [p_digmax, p_digits] = plgzax()
%
%   Original PLplot call documentation:
%
%   plgzax(digmax, digits)
%   **********************
% 
%   plgzax
% 
%   Identical to plgxax, except that arguments are flags for z axis. See the
%   description of plgxax for more detail.
% 
% 
  [p_digmax, p_digits] = plplot_octave(40);
endfunction

function plhist(data, datmin, datmax, nbin, oldwin)
% plhist(data, datmin, datmax, nbin, oldwin)
%
%   Original PLplot call documentation:
%
%   plhist(n, data, datmin, datmax, nbin, oldwin)
%   *********************************************
% 
%   plhist
% 
%   Plots a histogram from n data points stored in the array data. This
%   routine bins the data into nbin bins equally spaced between datmin and
%   datmax, and calls plbin to draw the resulting histogram. Parameter oldwin
%   allows the histogram either to be plotted in an existing window or causes
%   plhist to call plenv with suitable limits before plotting the histogram.
% 
%   n (PLINT, input): Number of data points.
% 
%   data (PLFLT *, input): Pointer to array with values of the n data
%   points.
% 
%   datmin (PLFLT, input): Left-hand edge of lowest-valued bin.
% 
%   datmax (PLFLT, input): Right-hand edge of highest-valued bin.
% 
%   nbin (PLINT, input): Number of (equal-sized) bins into which to divide
%   the interval xmin to xmax.
% 
%   oldwin (PLINT, input): If one, the histogram is plotted in the
%   currently-defined window, and if zero, plenv is called automatically before
%   plotting.
% 
% 
  plplot_octave(41, data, datmin, datmax, nbin, oldwin);
endfunction

function plhls(h, l, s)
% plhls(h, l, s)
%
% Set current color -map 0- by hue, lightness, and saturation.  
  plplot_octave(42, h, l, s);
endfunction

function plinit()
% plinit()
%
%   Original PLplot call documentation:
%
%   plinit()
%   ********
% 
%   plinit
% 
%   Initializing the plotting package. The program prompts for the device
%   keyword or number of the desired output device. Hitting a RETURN in
%   response to the prompt is the same as selecting the first device. plinit
%   will issue no prompt if either the device was specified previously (via
%   command line flag or the plsdev function), or if only one device is enabled
%   when PLplot is installed. If subpages have been specified, the output
%   device is divided into nx by ny sub-pages, each of which may be used
%   independently. If plinit is called again during a program, the previously
%   opened file will be closed. The subroutine pladv is used to advance from
%   one subpage to the next.
% 
% 
  plplot_octave(43);
endfunction

function pljoin(x1, y1, x2, y2)
% pljoin(x1, y1, x2, y2)
%
%   Original PLplot call documentation:
%
%   pljoin(x1, y1, x2, y2)
%   **********************
% 
%   pljoin
% 
%   Joins the point (x1, y1) to (x2, y2).
% 
%   x1 (PLFLT, input): x coordinate of first point.
% 
%   y1 (PLFLT, input): y coordinate of first point.
% 
%   x2 (PLFLT, input): x coordinate of second point.
% 
%   y2 (PLFLT, input): y coordinate of second point.
% 
% 
  plplot_octave(44, x1, y1, x2, y2);
endfunction

function pllab(xlabel, ylabel, tlabel)
% pllab(xlabel, ylabel, tlabel)
%
%   Original PLplot call documentation:
%
%   pllab(xlabel, ylabel, tlabel)
%   *****************************
% 
%   pllab
% 
%   Routine for writing simple labels. Use plmtex for more complex labels.
% 
%   xlabel (char *, input): Label for horizontal axis.
% 
%   ylabel (char *, input): Label for vertical axis.
% 
%   tlabel (char *, input): Title of graph.
% 
% 
  plplot_octave(45, xlabel, ylabel, tlabel);
endfunction

function pllightsource(x, y, z)
% pllightsource(x, y, z)
%
  plplot_octave(46, x, y, z);
endfunction

function plline(x, y)
% plline(x, y)
%
%   Original PLplot call documentation:
%
%   plline(n, x, y)
%   ***************
% 
%   plline
% 
%   Draws n-1 line segments joining points (x[i], y[i]).
% 
%   n (PLINT, input): Number of points to join.
% 
%   x (PLFLT *, input): Pointer to array with x coordinates of points.
% 
%   y (PLFLT *, input): Pointer to array with y coordinates of points.
% 
% 
  plplot_octave(47, x, y);
endfunction

function plline3(x, y, z)
% plline3(x, y, z)
%
% Draws a line in 3 space.   
  plplot_octave(48, x, y, z);
endfunction

function pllsty(lin)
% pllsty(lin)
%
%   Original PLplot call documentation:
%
%   pllsty(n)
%   *********
% 
%   pllsty
% 
%   This sets the line style according to one of eight predefined patterns
%   (also see plstyl).
% 
%   n (PLINT, input): Integer value between 1 and 8.
% 
% 
  plplot_octave(49, lin);
endfunction

function p_strm = plmkstrm()
% p_strm = plmkstrm()
%
% Creates a new stream and makes it the default.   
  p_strm = plplot_octave(50);
endfunction

function plmtex(side, disp, pos, just, text)
% plmtex(side, disp, pos, just, text)
%
%   Original PLplot call documentation:
%
%   plmtex(side, disp, pos, just, text)
%   ***********************************
% 
%   plmtex
% 
%   Writes text at a specified position relative to the viewport boundaries.
%   Text may be written inside or outside the viewport, but is clipped at the
%   subpage boundaries. The reference point of a string lies along a line
%   passing through the string at half the height of a capital letter. The
%   position of the reference point along this line is determined by just, and
%   the position of the reference point relative to the viewport is set by disp
%   and pos.
% 
%   side (char *, input): Specifies the side of the viewport along which the
%   text is to be written. The string must be one of:
% 
%    o b: Bottom of viewport.
%    o l: Left of viewport, text written parallel to edge.
%    o lv: Left of viewport, text written at right angles to edge.
%    o r: Right of viewport, text written parallel to edge.
%    o rv: Right of viewport, text written at right angles to edge.
%    o t: Top of viewport.
% 
%   disp (PLFLT, input): Position of the reference point of string, measured
%   outwards from the specified viewport edge in units of the current character
%   height. Use negative disp to write within the viewport.
% 
%   pos (PLFLT, input): Position of the reference point of string along the
%   specified edge, expressed as a fraction of the length of the edge.
% 
%   just (PLFLT, input): Specifies the position of the string relative to
%   its reference point. If just=0, the reference point is at the left and if
%   just=1, it is at the right of the string. Other values of just give
%   intermediate justifications.
% 
%   text (char *, input): The string to be written out.
% 
% 
  plplot_octave(51, side, disp, pos, just, text);
endfunction

function plpat(inc, del)
% plpat(inc, del)
%
%   Original PLplot call documentation:
%
%   plpat(nlin, inc, del)
%   *********************
% 
%   plpat
% 
%   Sets the area fill pattern. The pattern consists of 1 or 2 sets of
%   parallel lines with specified inclinations and spacings. The arguments to
%   this routine are the number of sets to use (1 or 2) followed by two
%   pointers to integer arrays (of 1 or 2 elements) specifying the inclinations
%   in tenths of a degree and the spacing in micrometers. (also see plpsty)
% 
%    nlin (PLINT, input): Number of sets of lines making up the pattern,
%   either 1 or 2.
% 
%    inc (PLINT *, input): Pointer to array with nlin elements. Specifies the
%   line inclination in tenths of a degree. (Should be between -900 and 900).
% 
%    del (PLINT *, input): Pointer to array with nlin elements. Specify the
%   spacing in micrometers between the lines making up the pattern.
% 
% 
  plplot_octave(52, inc, del);
endfunction

function plpoin(x, y, code)
% plpoin(x, y, code)
%
%   Original PLplot call documentation:
%
%   plpoin(n, x, y, code)
%   *********************
% 
%   plpoin
% 
%   Marks out a set of n points at positions (x(i), y(i)), using the symbol
%   defined by code. If code is between 32 and 127, the symbol is simply the
%   printable ASCII character in the default font.
% 
%   n (PLINT, input): Number of points to be marked.
% 
%   x (PLFLT *, input): Pointer to array with set of x coordinate values for
%   the points.
% 
%   y (PLFLT *, input): Pointer to array with set of y coordinate values for
%   the points.
% 
%   code (PLINT, input): Code number for the symbol to be plotted.
% 
% 
  plplot_octave(53, x, y, code);
endfunction

function plpoin3(x, y, z, code)
% plpoin3(x, y, z, code)
%
% Draws a series of points in 3 space.  
  plplot_octave(54, x, y, z, code);
endfunction

function plprec(setp, prec)
% plprec(setp, prec)
%
%   Original PLplot call documentation:
%
%   plprec(set, prec)
%   *****************
% 
%   plprec
% 
%   Sets the number of places after the decimal point in numeric labels.
% 
%   set (PLINT, input):  If set is equal to 0 then PLplot automatically
%   determines the number of places to use after the decimal point in numeric
%   labels (like those used to label axes). If set is 1 then prec sets the
%   number of places.
% 
%   prec (PLINT, input): The number of characters to draw after the decimal
%   point in numeric labels.
% 
% 
  plplot_octave(55, setp, prec);
endfunction

function plpsty(patt)
% plpsty(patt)
%
%   Original PLplot call documentation:
%
%   plpsty(n)
%   *********
% 
%   plpsty
% 
%   Select one of eight predefined area fill patterns to use (also see
%   plpat).
% 
%   n (PLINT *, input): The desired pattern.
% 
% 
  plplot_octave(56, patt);
endfunction

function plptex(x, y, dx, dy, just, text)
% plptex(x, y, dx, dy, just, text)
%
%   Original PLplot call documentation:
%
%   plptex(x, y, dx, dy, just, text)
%   ********************************
% 
%   plptex
% 
%   Writes text at a specified position and inclination within the viewport.
%   Text is clipped at the viewport boundaries. The reference point of a string
%   lies along a line passing through the string at half the height of a
%   capital letter. The position of the reference point along this line is
%   determined by just, the reference point is placed at world coordinates (x,
%   y) within the viewport. The inclination of the string is specified in terms
%   of differences of world coordinates making it easy to write text parallel
%   to a line in a graph.
% 
%   x (PLFLT, input): x coordinate of reference point of string.
% 
%   y (PLFLT, input): y coordinate of reference point of string.
% 
%   dx (PLFLT, input): Together with dy, this specifies the inclination of
%   the string. The baseline of the string is parallel to a line joining (x, y)
%   to (x+dx, y+dy).
% 
%   dy (PLFLT, input): Together with dx, this specifies the inclination of
%   the string.
% 
%   just (PLFLT, input): Specifies the position of the string relative to
%   its reference point. If just=0, the reference point is at the left and if
%   just=1, it is at the right of the string. Other values of just give
%   intermediate justifications.
% 
%   text (char *, input): The string to be written out.
% 
% 
  plplot_octave(57, x, y, dx, dy, just, text);
endfunction

function plreplot()
% plreplot()
%
% Replays contents of plot buffer to current device/file.  
  plplot_octave(58);
endfunction

function plrgb(r, g, b)
% plrgb(r, g, b)
%
% Set line color by red, green, blue from  0. to 1.  
  plplot_octave(59, r, g, b);
endfunction

function plrgb1(r, g, b)
% plrgb1(r, g, b)
%
% Set line color by 8 bit RGB values.  
  plplot_octave(60, r, g, b);
endfunction

function plschr(def, scale)
% plschr(def, scale)
%
%   Original PLplot call documentation:
%
%   plschr(def, scale)
%   ******************
% 
%   plschr
% 
%   This sets up the size of all subsequent characters drawn. The actual
%   height of a character is the product of the default character size and a
%   scaling factor.
% 
%   def (PLFLT, input): The default height of a character in millimeters,
%   should be set to zero if the default height is to remain unchanged.
% 
%   scale (PLFLT, input): Scale factor to be applied to default to get
%   actual character height.
% 
% 
  plplot_octave(61, def, scale);
endfunction

function plscmap0(r, g, b)
% plscmap0(r, g, b)
%
% Set color map 0 colors by 8 bit RGB values  
  plplot_octave(62, r, g, b);
endfunction

function plscmap0n(ncol0)
% plscmap0n(ncol0)
%
% Set number of colors in cmap 0  
  plplot_octave(63, ncol0);
endfunction

function plscmap1(r, g, b)
% plscmap1(r, g, b)
%
% Set color map 1 colors by 8 bit RGB values  
  plplot_octave(64, r, g, b);
endfunction

function plscmap1l(itype, intensity, coord1, coord2, coord3, rev)
% plscmap1l(itype, intensity, coord1, coord2, coord3, rev)
%
% Set color map 1 colors using a piece-wise linear relationship between  intensity [0,1] -cmap 1 index- and position in HLS or RGB color space.  
  plplot_octave(65, itype, intensity, coord1, coord2, coord3, rev);
endfunction

function plscmap1n(ncol1)
% plscmap1n(ncol1)
%
% Set number of colors in cmap 1  
  plplot_octave(66, ncol1);
endfunction

function plscol0(icol0, r, g, b)
% plscol0(icol0, r, g, b)
%
% Set a given color from color map 0 by 8 bit RGB value  
  plplot_octave(67, icol0, r, g, b);
endfunction

function plscolbg(r, g, b)
% plscolbg(r, g, b)
%
% Set the background color by 8 bit RGB value  
  plplot_octave(68, r, g, b);
endfunction

function plscolor(color)
% plscolor(color)
%
% Used to globally turn color output on/off  
  plplot_octave(69, color);
endfunction

function plsdev(devname)
% plsdev(devname)
%
% Set the device -keyword- name  
  plplot_octave(70, devname);
endfunction

function plsdidev(mar, aspect, jx, jy)
% plsdidev(mar, aspect, jx, jy)
%
% Set window into device space using margin, aspect ratio, and  justification  
  plplot_octave(71, mar, aspect, jx, jy);
endfunction

function plsdimap(dimxmin, dimxmax, dimymin, dimymax, dimxpmm, dimypmm)
% plsdimap(dimxmin, dimxmax, dimymin, dimymax, dimxpmm, dimypmm)
%
% Set up transformation from metafile coordinates.  
  plplot_octave(72, dimxmin, dimxmax, dimymin, dimymax, dimxpmm, dimypmm);
endfunction

function plsdiori(rot)
% plsdiori(rot)
%
% Set plot orientation, specifying rotation in units of pi/2.  
  plplot_octave(73, rot);
endfunction

function plsdiplt(xmin, ymin, xmax, ymax)
% plsdiplt(xmin, ymin, xmax, ymax)
%
% Set window into plot space  
  plplot_octave(74, xmin, ymin, xmax, ymax);
endfunction

function plsdiplz(xmin, ymin, xmax, ymax)
% plsdiplz(xmin, ymin, xmax, ymax)
%
% Set window into plot space incrementally -zoom-  
  plplot_octave(75, xmin, ymin, xmax, ymax);
endfunction

function plsesc(esc)
% plsesc(esc)
%
% Set the escape character for text strings.  
  plplot_octave(76, esc);
endfunction

function plsfam(fam, num, bmax)
% plsfam(fam, num, bmax)
%
%   Original PLplot call documentation:
%
%   plsfam(fam, num, bmax)
%   **********************
% 
%   plsfam
% 
%   Sets variables dealing with output file familying. Does nothing if
%   familying not supported by the driver. This routine, if used, must be
%   called before initializing PLplot. See Section [*] for more information.
% 
%   fam (PLINT, input): Family flag (boolean). If nonzero, familying is
%   enabled.
% 
%   num (PLINT, input): Current family file number.
% 
%   bmax (PLINT, input): Maximum file size (in bytes) for a family file.
% 
% 
  plplot_octave(77, fam, num, bmax);
endfunction

function plsfnam(fnam)
% plsfnam(fnam)
%
%   Original PLplot call documentation:
%
%   plsfnam(fnam)
%   *************
% 
%   plsfnam
% 
%   Sets the current output file name, if applicable. If the file name has
%   not been specified and is required by the driver, the user will be prompted
%   for it. If using the X-windows output driver, this sets the display name.
%   This routine, if used, must be called before initializing PLplot.
% 
%   fnam (char *, input): Pointer to file name string.
% 
% 
  plplot_octave(78, fnam);
endfunction

function plsmaj(def, scale)
% plsmaj(def, scale)
%
%   Original PLplot call documentation:
%
%   plsmaj(def, scale)
%   ******************
% 
%   plsmaj
% 
%   This sets up the length of the major ticks. The actual length is the
%   product of the default length and a scaling factor as for character height.
% 
%   def (PLFLT, input): The default length of a major tick in millimeters,
%   should be set to zero if the default length is to remain unchanged.
% 
%   scale (PLFLT, input): Scale factor to be applied to default to get
%   actual tick length.
% 
% 
  plplot_octave(79, def, scale);
endfunction

function plsmin(def, scale)
% plsmin(def, scale)
%
%   Original PLplot call documentation:
%
%   plsmin(def, scale)
%   ******************
% 
%   plsmin
% 
%   This sets up the length of the minor ticks and the length of the
%   terminals on error bars. The actual length is the product of the default
%   length and a scaling factor as for character height.
% 
%   def (PLFLT, input): The default length of a minor tick in millimeters,
%   should be set to zero if the default length is to remain unchanged.
% 
%   scale (PLFLT, input): Scale factor to be applied to default to get
%   actual tick length.
% 
% 
  plplot_octave(80, def, scale);
endfunction

function plsori(ori)
% plsori(ori)
%
%   Original PLplot call documentation:
%
%   plsori(ori)
%   ***********
% 
%   plsori
% 
%   Sets the current orientation. If ori is equal to zero (default) then
%   landscape is used (x axis is parallel to the longest edge of the page),
%   otherwise portrait is used. This option is not supported by all output
%   drivers (in particular, most interactive screen drivers ignore the
%   orientation). This routine, if used, must be called before initializing
%   PLplot.
% 
%   ori (PLINT, input): Orientation value.
% 
% 
  plplot_octave(81, ori);
endfunction

function plspage(xp, yp, xleng, yleng, xoff, yoff)
% plspage(xp, yp, xleng, yleng, xoff, yoff)
%
%   Original PLplot call documentation:
%
%   plspage(xp, yp, xleng, yleng, xoff, yoff)
%   *****************************************
% 
%   plspage
% 
%   Sets the page configuration (optional). Not all parameters recognized by
%   all drivers. The X-window driver uses the length and offset parameters to
%   determine the window size and location. This routine, if used, must be
%   called before initializing PLplot.
% 
%   xp (PLINT *, output):  Number of pixels, x.
% 
%   yp (PLINT *; output):  Number of pixels, y.
% 
%   xleng (PLINT *, output):  Page length, x.
% 
%   yleng (PLINT *, output):  Page length, y.
% 
%   xoff (PLINT *, output):  Page offset, x.
% 
%   yoff (PLINT *, output):  Page offset, y.
% 
% 
  plplot_octave(82, xp, yp, xleng, yleng, xoff, yoff);
endfunction

function plspause(pause)
% plspause(pause)
%
% Set the pause -on end-of-page- status  
  plplot_octave(83, pause);
endfunction

function plsstrm(strm)
% plsstrm(strm)
%
%   Original PLplot call documentation:
%
%   plsstrm(strm)
%   *************
% 
%   plsstrm
% 
%   Sets the number of the current output stream. The stream number defaults
%   to 0 unless changed by this routine. The first use of this routine must be
%   followed by a call initializing PLplot (e.g. plstar).
% 
%   strm (PLINT *, output): Pointer to current stream number.
% 
% 
  plplot_octave(84, strm);
endfunction

function plssub(nx, ny)
% plssub(nx, ny)
%
% Set the number of subwindows in x and y  
  plplot_octave(85, nx, ny);
endfunction

function plssym(def, scale)
% plssym(def, scale)
%
%   Original PLplot call documentation:
%
%   plssym(def, scale)
%   ******************
% 
%   plssym
% 
%   This sets up the size of all subsequent symbols drawn by plpoin and
%   plsym. The actual height of a symbol is the product of the default symbol
%   size and a scaling factor as for the character height.
% 
%   def (PLFLT, input): The default height of a symbol in millimeters,
%   should be set to zero if the default height is to remain unchanged.
% 
%   scale (PLFLT, input): Scale factor to be applied to default to get
%   actual symbol height.
% 
% 
  plplot_octave(86, def, scale);
endfunction

function plstar(nx, ny)
% plstar(nx, ny)
%
%   Original PLplot call documentation:
%
%   plstar(nx, ny)
%   **************
% 
%   plstar
% 
%   Initializing the plotting package. The program prompts for the device
%   keyword or number of the desired output device. Hitting a RETURN in
%   response to the prompt is the same as selecting the first device. If only
%   one device is enabled when PLplot is installed, plstar will issue no
%   prompt. The output device is divided into nx by ny sub-pages, each of which
%   may be used independently. The subroutine pladv is used to advance from one
%   subpage to the next.
% 
%   nx (PLINT, input): Number of subpages to divide output page in the
%   horizontal direction.
% 
%   ny (PLINT, input): Number of subpages to divide output page in the
%   vertical direction.
% 
% 
  plplot_octave(87, nx, ny);
endfunction

function plstart(devname, nx, ny)
% plstart(devname, nx, ny)
%
%   Original PLplot call documentation:
%
%   plstart(device, nx, ny)
%   ***********************
% 
%   plstart
% 
%   Alternative to plstar for initializing the plotting package. The device
%   name keyword for the desired output device must be supplied as an argument.
%   The device keywords are the same as those printed out by plstar. If the
%   requested device is not available, or if the input string is empty or
%   begins with ``?'', the prompted startup of plstar is used. This routine
%   also divides the output device into nx by ny sub-pages, each of which may
%   be used independently. The subroutine pladv is used to advance from one
%   subpage to the next.
% 
%   device (char *, input): Device name (keyword) of the required output
%   device. If NULL or if the first character is a ``?'', the normal (prompted)
%   startup is used.
% 
%   nx (PLINT, input): Number of subpages to divide output page in the
%   horizontal direction.
% 
%   ny (PLINT, input): Number of subpages to divide output page in the
%   vertical direction.
% 
% 
  plplot_octave(88, devname, nx, ny);
endfunction

function plstripa(id, pen, x, y)
% plstripa(id, pen, x, y)
%
  plplot_octave(89, id, pen, x, y);
endfunction

function plstripd(id)
% plstripd(id)
%
  plplot_octave(90, id);
endfunction

function plstyl(nms, mark, space)
% plstyl(nms, mark, space)
%
%   Original PLplot call documentation:
%
%   plstyl(nels, mark, space)
%   *************************
% 
%   plstyl
% 
%   This sets up the line style for all lines subsequently drawn. A line
%   consists of segments in which the pen is alternately down and up. The
%   lengths of these segments are passed in the arrays mark and space
%   respectively. The number of mark-space pairs is specified by nels. In order
%   to return the line style to the default continuous line, plstyl should be
%   called with nels=0.(also see pllsty)
% 
%   nels (PLINT, input): The number of mark and space elements in a line.
%   Thus a simple broken line can be obtained by setting nels=1. A continuous
%   line is specified by setting nels=0.
% 
%   mark (PLINT *, input): Pointer to array with the lengths of the segments
%   during which the pen is down, measured in micrometers.
% 
%   space (PLINT *, input): Pointer to array with the lengths of the
%   segments during which the pen is up, measured in micrometers.
% 
% 
  plplot_octave(91, nms, mark, space);
endfunction

function plsvpa(xmin, xmax, ymin, ymax)
% plsvpa(xmin, xmax, ymin, ymax)
%
%   Original PLplot call documentation:
%
%   plsvpa(xmin, xmax, ymin, ymax)
%   ******************************
% 
%   plsvpa
% 
%   Alternate routine to plvpor for setting up the viewport. This routine
%   should be used only if the viewport is required to have a definite size in
%   millimeters. The routine plgspa is useful for finding out the size of the
%   current subpage.
% 
%   xmin (PLFLT, input): The distance of the left-hand edge of the viewport
%   from the left-hand edge of the subpage in millimeters.
% 
%   xmax (PLFLT, input): The distance of the right-hand edge of the viewport
%   from the left-hand edge of the subpage in millimeters.
% 
%   ymin (PLFLT, input): The distance of the bottom edge of the viewport
%   from the bottom edge of the subpage in millimeters.
% 
%   ymax (PLFLT, input): The distance of the top edge of the viewport from
%   the top edge of the subpage in millimeters.
% 
% 
  plplot_octave(92, xmin, xmax, ymin, ymax);
endfunction

function plsxax(digmax, digits)
% plsxax(digmax, digits)
%
%   Original PLplot call documentation:
%
%   plsxax(digmax, digits)
%   **********************
% 
%   plsxax
% 
%   Returns current values of the digmax and digits flags for the x axis.
%   digits is updated after the plot is drawn, so so this routine should only
%   be called _a_f_t_e_r the call to plbox (or plbox3) is complete. Annotation for
%   more information.
% 
%   digmax (PLINT, output): Variable to set the maximum number of digits. If
%   nonzero, the printed label will be switched to a floating point
%   representation when the number of digits exceeds digmax.
% 
%   digits (PLINT, input): Field digits value. Currently, changing its value
%   here has no effect since it is set only by plbox or plbox3. However, the
%   user may obtain its value after a call to plbox by calling plgxax.
% 
% 
  plplot_octave(93, digmax, digits);
endfunction

function plsyax(digmax, digits)
% plsyax(digmax, digits)
%
%   Original PLplot call documentation:
%
%   plsyax(digmax, digits)
%   **********************
% 
%   plsyax
% 
%   Identical to plsxax, except that arguments are flags for y axis. See the
%   description of plsxax for more detail.
% 
% 
  plplot_octave(94, digmax, digits);
endfunction

function plsym(x, y, code)
% plsym(x, y, code)
%
%   Original PLplot call documentation:
%
%   plsym(n, x, y, code)
%   ********************
% 
%   plsym
% 
%   Marks out a set of n points at positions (x[i], y[i]), using the symbol
%   defined by code. The code is interpreted as an index in the Hershey font
%   tables.
% 
%   n (PLINT, input): Number of points to be marked.
% 
%   x (PLFLT *, input): Pointer to array with set of x coordinate values for
%   the points.
% 
%   y (PLFLT *, input): Pointer to array with set of y coordinate values for
%   the points.
% 
%   code (PLINT, input): Code number for the symbol to be plotted.
% 
% 
  plplot_octave(95, x, y, code);
endfunction

function plszax(digmax, digits)
% plszax(digmax, digits)
%
%   Original PLplot call documentation:
%
%   plszax(digmax, digits)
%   **********************
% 
%   plszax
% 
%   Identical to plsxax, except that arguments are flags for z axis. See the
%   description of plsxax for more detail.
% 
% 
  plplot_octave(96, digmax, digits);
endfunction

function pltext()
% pltext()
%
%   Original PLplot call documentation:
%
%   pltext()
%   ********
% 
%   pltext
% 
%   Sets an interactive device to text mode, used in conjunction with plgra
%   to allow graphics and text to be interspersed. On a device which supports
%   separate text and graphics windows, this command causes control to be
%   switched to the text window. This can be useful for printing diagnostic
%   messages or getting user input, which would otherwise interfere with the
%   plots. The user _m_u_s_t switch back to the graphics window before issuing plot
%   commands, as the text (or console) device will probably become quite
%   confused otherwise. If already in text mode, this command is ignored. It is
%   also ignored on devices which only support a single window or use a
%   different method for shifting focus (also plgra).
% 
% 
  plplot_octave(97);
endfunction

function plvasp(aspect)
% plvasp(aspect)
%
%   Original PLplot call documentation:
%
%   plvasp(aspect)
%   **************
% 
%   plvasp
% 
%   Sets the viewport so that the ratio of the length of the y axis to that
%   of the x axis is equal to aspect.
% 
%   aspect (PLFLT, input): Ratio of length of y axis to length of x axis.
% 
% 
  plplot_octave(98, aspect);
endfunction

function plvpas(xmin, xmax, ymin, ymax, aspect)
% plvpas(xmin, xmax, ymin, ymax, aspect)
%
%   Original PLplot call documentation:
%
%   plvpas(xmin, xmax, ymin, ymax, aspect)
%   **************************************
% 
%   plvpas
% 
%   Device-independent routine for setting up the viewport. The viewport is
%   chosen to be the largest with the given aspect ratio that fits within the
%   specified region (in terms of normalized subpage coordinates). This routine
%   is functionally equivalent to plvpor when a ``natural'' aspect ratio (0.0)
%   is chosen. Unlike plvasp, this routine reserves no extra space at the edges
%   for labels.
% 
%   xmin (PLFLT, input): The normalized subpage coordinate of the left-hand
%   edge of the viewport.
% 
%   xmax (PLFLT, input): The normalized subpage coordinate of the right-hand
%   edge of the viewport.
% 
%   ymin (PLFLT, input): The normalized subpage coordinate of the bottom
%   edge of the viewport.
% 
%   ymax (PLFLT, input): The normalized subpage coordinate of the top edge
%   of the viewport.
% 
%   aspect (PLFLT, input): Ratio of length of y axis to length of x axis.
% 
% 
  plplot_octave(99, xmin, xmax, ymin, ymax, aspect);
endfunction

function plvpor(xmin, xmax, ymin, ymax)
% plvpor(xmin, xmax, ymin, ymax)
%
%   Original PLplot call documentation:
%
%   plvpor(xmin, xmax, ymin, ymax)
%   ******************************
% 
%   plvpor
% 
%   Device-independent routine for setting up the viewport. This defines the
%   viewport in terms of normalized subpage coordinates which run from 0.0 to
%   1.0 (left to right and bottom to top) along each edge of the current
%   subpage. Use the alternate routine plsvpa in order to create a viewport of
%   a definite size.
% 
%   xmin (PLFLT, input): The normalized subpage coordinate of the left-hand
%   edge of the viewport.
% 
%   xmax (PLFLT, input): The normalized subpage coordinate of the right-hand
%   edge of the viewport.
% 
%   ymin (PLFLT, input): The normalized subpage coordinate of the bottom
%   edge of the viewport.
% 
%   ymax (PLFLT, input): The normalized subpage coordinate of the top edge
%   of the viewport.
% 
% 
  plplot_octave(100, xmin, xmax, ymin, ymax);
endfunction

function plvsta()
% plvsta()
%
%   Original PLplot call documentation:
%
%   plvsta()
%   ********
% 
%   plvsta
% 
%   Sets up a standard viewport, leaving a left-hand margin of seven
%   character heights, and four character heights around the other three sides.
% 
% 
  plplot_octave(101);
endfunction

function plw3d(basex, basey, height, xmin0, xmax0, ymin0, ymax0, zmin0, zmax0, alt, az)
% plw3d(basex, basey, height, xmin0, xmax0, ymin0, ymax0, zmin0, zmax0, alt, az)
%
%   Original PLplot call documentation:
%
%   plw3d(basex, basey, height, xmin, xmax, ymin, ymax, zmin, zmax, alt, az)
%   ************************************************************************
% 
%   plw3d
% 
%   Sets up a window for a three-dimensional surface plot within the
%   currently defined two-dimensional window. The enclosing box for the surface
%   plot defined by xmin, xmax, ymin, ymax, zmin and zmax in user-coordinate
%   space is mapped into a box of world coordinate size basex by basey by
%   height so that xmin maps to -basex/2, xmax maps to basex/2, ymin maps to
%   -basey/2, ymax maps to basey/2, zmin maps to 0 and zmax maps to height. The
%   resulting world-coordinate box is then viewed by an observer at altitude
%   alt and azimuth az. This routine must be called before plbox3 or plot3d.
%   For a more complete description of three-dimensional plotting 3-d Plots.
% 
%   basex (PLFLT, input): The x coordinate size of the world-coordinate box.
% 
%   basey (PLFLT, input): The y coordinate size of the world-coordinate box.
% 
%   height (PLFLT, input): The z coordinate size of the world-coordinate
%   box.
% 
%   xmin (PLFLT, input): The minimum user x coordinate value.
% 
%   xmax (PLFLT, input): The maximum user x coordinate value.
% 
%   ymin (PLFLT, input): The minimum user y coordinate value.
% 
%   ymax (PLFLT, input): The maximum user y coordinate value.
% 
%   zmin (PLFLT, input): The minimum user z coordinate value.
% 
%   zmax (PLFLT, input): The maximum user z coordinate value.
% 
%   alt (PLFLT, input): The viewing altitude in degrees above the xy plane.
% 
%   az (PLFLT, input): The viewing azimuth in degrees. When az=0, the
%   observer is looking face onto the zx plane, and as az is increased, the
%   observer moves clockwise around the box when viewed from above the xy
%   plane.
% 
% 
  plplot_octave(102, basex, basey, height, xmin0, xmax0, ymin0, ymax0, zmin0, zmax0, alt, az);
endfunction

function plwid(width)
% plwid(width)
%
%   Original PLplot call documentation:
%
%   plwid(width)
%   ************
% 
%   plwid
% 
%   Sets the pen width.
% 
%   width (PLINT, input): The desired pen width. The pen width must be
%   between 1 and a device dependent maximum value.
% 
% 
  plplot_octave(103, width);
endfunction

function plwind(xmin, xmax, ymin, ymax)
% plwind(xmin, xmax, ymin, ymax)
%
%   Original PLplot call documentation:
%
%   plwind(xmin, xmax, ymin, ymax)
%   ******************************
% 
%   plwind
% 
%   Sets up the world coordinates of the edges of the viewport.
% 
%   xmin (PLFLT, input): The world x coordinate of the left-hand edge of the
%   viewport.
% 
%   xmax (PLFLT, input): The world x coordinate of the right-hand edge of
%   the viewport.
% 
%   ymin (PLFLT, input): The world y coordinate of the bottom edge of the
%   viewport.
% 
%   ymax (PLFLT, input): The world y coordinate of the top edge of the
%   viewport.
% 
% 
  plplot_octave(104, xmin, xmax, ymin, ymax);
endfunction

function plxormod(icol0)
% plxormod(icol0)
%
  plplot_octave(105, icol0);
endfunction

function [retval, state, keysym, button, string, pX, pY, dX, dY, wX, wY] = plGetCursor()
% [retval, state, keysym, button, string, pX, pY, dX, dY, wX, wY] = plGetCursor()
%
% Wait for graphics input event and translate to world coordinates  
  [retval, state, keysym, button, string, pX, pY, dX, dY, wX, wY] = plplot_octave(106);
endfunction

function [retval, x, y] = plTranslateCursor(x_in, y_in)
% [retval, x, y] = plTranslateCursor(x_in, y_in)
%
% Translates relative device coordinates to world coordinates.   
  [retval, x, y] = plplot_octave(107, x_in, y_in);
endfunction

function plcol(icol0)
% plcol(icol0)
%
%   Original PLplot call documentation:
%
%   plcol(color)
%   ************
% 
%   plcol
% 
%   Sets the color for subsequent lines.
% 
%   color (PLINT, input): Integer representing the color. The defaults at
%   present are (these may change):
% 
%   1. black (default background)
%   2. red
%   3. yellow
%   4. green
%   5. aquamarine
%   6. pink
%   7. wheat
%   8. grey
%   9. brown
%  10. blue
%  11. BlueViolet
%  12. cyan
%  13. turquoise
%  14. magenta
%  15. salmon
%  16. white (default foreground)
% 
% 
  plplot_octave(108, icol0);
endfunction

function plcont(f, kx, lx, ky, ly, clevel, tr)
% plcont(f, kx, lx, ky, ly, clevel, tr)
%
%   Original PLplot call documentation:
%
%   plcont(z, nx, ny, kx, lx, ky, ly, clevel, nlevel, pltr)
%   *******************************************************
% 
%   plcont
% 
%   Draws a contour plot of the data in z[nx][ny], using the nlevel contour
%   levels specified by clevel. Only the region of the array from kx to lx and
%   from ky to ly is plotted out. A transformation routine pltr is used to map
%   indicies within the array to the world coordinates. Contour plots for more
%   information.
% 
%   z (PLFLT **, input): Pointer to a vectored two-dimensional array
%   containing data to be contoured.
% 
%   nx, ny (PLINT, input): Physical dimensions of array z.
% 
%   kx, lx (PLINT, input): Range of x indicies to consider.
% 
%   ky, ly (PLINT, input): Range of y indicies to consider.
% 
%   clevel (PLFLT *, input): Pointer to array specifying levels at which to
%   draw contours.
% 
%   nlevel (PLINT, input): Number of contour levels to draw.
% 
%   pltr (void *, input): Pointer to function that defines transformation
%   between indicies in array z and the world coordinates (C only). Two
%   transformation functions are provided: xform, for simple linear mappings,
%   and pltr0, if no mapping is required. xform is not actually compiled into
%   the PLplot library due to the need for external data; instead you must
%   include it as part of your program. The function should have the form:
% 
%    pltr(x, y, tx, ty)
% 
%   x, y (PLFLT, input):  Specifies the position in the array through which
%   the contour runs in terms of the array indicies.
% 
%   tx, ty (PLFLT *, output): Pointers to the world coordinates
%   corresponding to the point (x, y).
% 
%   tr (PLFLT *, input): Pointer to 6-element array specifying coefficients
%   to use in the xform transformation function (Fortran only). Since this is
%   somewhat inflexible, the user is recommended to call either of plcon0,
%   plcon1, or plcon2 instead.
% 
% 
  plplot_octave(109, f, kx, lx, ky, ly, clevel, tr);
endfunction

function plcont0(f, kx, lx, ky, ly, clevel)
% plcont0(f, kx, lx, ky, ly, clevel)
%
  plplot_octave(110, f, kx, lx, ky, ly, clevel);
endfunction

function plcont1(f, kx, lx, ky, ly, clevel, xg, yg)
% plcont1(f, kx, lx, ky, ly, clevel, xg, yg)
%
  plplot_octave(111, f, kx, lx, ky, ly, clevel, xg, yg);
endfunction

function plcont2(f, kx, lx, ky, ly, clevel, xg, yg)
% plcont2(f, kx, lx, ky, ly, clevel, xg, yg)
%
  plplot_octave(112, f, kx, lx, ky, ly, clevel, xg, yg);
endfunction

function plcont2p(f, kx, lx, ky, ly, clevel, xg, yg)
% plcont2p(f, kx, lx, ky, ly, clevel, xg, yg)
%
  plplot_octave(113, f, kx, lx, ky, ly, clevel, xg, yg);
endfunction

function plmesh(x, y, z, opt)
% plmesh(x, y, z, opt)
%
%   Original PLplot call documentation:
%
%   plmesh(x, y, z, nx, ny, opt)
%   ****************************
% 
%   plmesh
% 
%   Plots a surface mesh within the environment set up by plw3d. The surface
%   is defined by the two-dimensional array z  , the point z[i][j] being the
%   value of the function at (x[i], y[j]). Note that the points in arrays x and
%   y do not need to be equally spaced, but must be stored in ascending order.
%   The parameter opt controls the way in which the surface is displayed. For
%   further details 3-d Plots.
% 
%   x (PLFLT *, input): Pointer to set of x coordinate values at which the
%   function is evaluated.
% 
%   y (PLFLT *, input): Pointer to set of y coordinate values at which the
%   function is evaluated.
% 
%   z (PLFLT **, input): Pointer to a vectored two-dimensional array with
%   set of function values.
% 
%   nx (PLINT, input): Number of x values at which function is evaluated.
% 
%   ny (PLINT, input): Number of y values at which function is evaluated.
% 
%   opt (PLINT, input): Determines the way in which the surface is
%   represented:
% 
%    o 1: Lines are drawn showing z as a function of x for each value of y[j].
%    o 2: Lines are drawn showing z as a function of y for each value of x[i].
%    o 3: Network of lines is drawn connecting points at which function is
%      defined.
% 
%   mx (PLINT, input): Length of array in x direction, for plotting
%   subarrays (Fortran only).
% 
% 
  plplot_octave(114, x, y, z, opt);
endfunction

function plot3d(x, y, z, opt, side)
% plot3d(x, y, z, opt, side)
%
%   Original PLplot call documentation:
%
%   plot3d(x, y, z, nx, ny, opt, side)
%   **********************************
% 
%   plot3d
% 
%   Plots a three dimensional surface plot within the environment set up by
%   plw3d. The surface is defined by the two-dimensional array z  , the point
%   z[i][j] being the value of the function at (x[i], y[j]). Note that the
%   points in arrays x and y do not need to be equally spaced, but must be
%   stored in ascending order. The parameter opt controls the way in which the
%   surface is displayed. For further details 3-d Plots.
% 
%   x (PLFLT *, input): Pointer to set of x coordinate values at which the
%   function is evaluated.
% 
%   y (PLFLT *, input): Pointer to set of y coordinate values at which the
%   function is evaluated.
% 
%   z (PLFLT **, input): Pointer to a vectored two-dimensional array with
%   set of function values.
% 
%   nx (PLINT, input): Number of x values at which function is evaluated.
% 
%   ny (PLINT, input): Number of y values at which function is evaluated.
% 
%   opt (PLINT, input): Determines the way in which the surface is
%   represented:
% 
%    o 1: Lines are drawn showing z as a function of x for each value of y[j].
%    o 2: Lines are drawn showing z as a function of y for each value of x[i].
%    o 3: Network of lines is drawn connecting points at which function is
%      defined.
% 
%   side (PLINT, input): Flag to indicate whether or not ``sides'' should be
%   draw on the figure. If side=0 no sides are drawn, otherwise the sides are
%   drawn.
% 
%   mx (PLINT, input): Length of array in x direction, for plotting
%   subarrays (Fortran only).
% 
% 
  plplot_octave(115, x, y, z, opt, side);
endfunction

function plotsh3d(x, y, z, side)
% plotsh3d(x, y, z, side)
%
  plplot_octave(116, x, y, z, side);
endfunction

function plpoly3(x, y, z, draw, clockwise)
% plpoly3(x, y, z, draw, clockwise)
%
% Draws a polygon in 3 space.   
  plplot_octave(117, x, y, z, draw, clockwise);
endfunction

function plshade(a, defined, left, right, bottom, top, shade_min, shade_max, sh_cmap, sh_color, sh_width, min_color, min_width, max_color, max_width, rectangular, tr)
% plshade(a, defined, left, right, bottom, top, shade_min, shade_max, sh_cmap, sh_color, sh_width, min_color, min_width, max_color, max_width, rectangular, tr)
%
% Shade region.  
  plplot_octave(118, a, defined, left, right, bottom, top, shade_min, shade_max, sh_cmap, sh_color, sh_width, min_color, min_width, max_color, max_width, rectangular, tr);
endfunction

function plshade1(a, defined, left, right, bottom, top, shade_min, shade_max, sh_cmap, sh_color, sh_width, min_color, min_width, max_color, max_width, rectangular, xg, yg)
% plshade1(a, defined, left, right, bottom, top, shade_min, shade_max, sh_cmap, sh_color, sh_width, min_color, min_width, max_color, max_width, rectangular, xg, yg)
%
% Shade region.   
  plplot_octave(119, a, defined, left, right, bottom, top, shade_min, shade_max, sh_cmap, sh_color, sh_width, min_color, min_width, max_color, max_width, rectangular, xg, yg);
endfunction

function plshade2(a, defined, left, right, bottom, top, shade_min, shade_max, sh_cmap, sh_color, sh_width, min_color, min_width, max_color, max_width, rectangular, xg, yg)
% plshade2(a, defined, left, right, bottom, top, shade_min, shade_max, sh_cmap, sh_color, sh_width, min_color, min_width, max_color, max_width, rectangular, xg, yg)
%
  plplot_octave(120, a, defined, left, right, bottom, top, shade_min, shade_max, sh_cmap, sh_color, sh_width, min_color, min_width, max_color, max_width, rectangular, xg, yg);
endfunction

function id = plstripc(xspec, yspec, xmin, xmax, xjump, ymin, ymax, xlpos, ylpos, y_ascl, acc, colbox, collab, colline, styline, legline1, legline2, legline3, legline4, labx, laby, labtop)
% id = plstripc(xspec, yspec, xmin, xmax, xjump, ymin, ymax, xlpos, ylpos, y_ascl, acc, colbox, collab, colline, styline, legline1, legline2, legline3, legline4, labx, laby, labtop)
%
  id = plplot_octave(121, xspec, yspec, xmin, xmax, xjump, ymin, ymax, xlpos, ylpos, y_ascl, acc, colbox, collab, colline, styline, legline1, legline2, legline3, legline4, labx, laby, labtop);
endfunction

function plClearOpts()
% plClearOpts()
%
% Clear external option table info structure.  
  plplot_octave(122);
endfunction

function retval = plGetFlt(s)
% retval = plGetFlt(s)
%
% Prompts human to input a float in response to given message.  
  retval = plplot_octave(123, s);
endfunction

function retval = plGetInt(s)
% retval = plGetInt(s)
%
  retval = plplot_octave(124, s);
endfunction

function [p_r, p_g, p_b] = plHLS_RGB(h, l, s)
% [p_r, p_g, p_b] = plHLS_RGB(h, l, s)
%
  [p_r, p_g, p_b] = plplot_octave(125, h, l, s);
endfunction

function plOptUsage()
% plOptUsage()
%
% Print usage & syntax message.  
  plplot_octave(126);
endfunction

function retval = plParseOpts(p_argc, argv, mode)
% retval = plParseOpts(p_argc, argv, mode)
%
% Process options list using current options info.  
  retval = plplot_octave(127, p_argc, argv, mode);
endfunction

function [p_h, p_l, p_s] = plRGB_HLS(r, g, b)
% [p_h, p_l, p_s] = plRGB_HLS(r, g, b)
%
  [p_h, p_l, p_s] = plplot_octave(128, r, g, b);
endfunction

function plResetOpts()
% plResetOpts()
%
% Reset external option table info structure.  
  plplot_octave(129);
endfunction

function retval = plSetOpt(opt, optarg)
% retval = plSetOpt(opt, optarg)
%
% Process input strings, treating them as an option and argument pair.  
  retval = plplot_octave(130, opt, optarg);
endfunction

function plSetUsage(program_string, usage_string)
% plSetUsage(program_string, usage_string)
%
% Set the strings used in usage and syntax messages.  
  plplot_octave(131, program_string, usage_string);
endfunction

function pldid2pc(xmin, ymin, xmax, ymax)
% pldid2pc(xmin, ymin, xmax, ymax)
%
% Converts input values from relative device coordinates to relative plot  coordinates.  
  plplot_octave(132, xmin, ymin, xmax, ymax);
endfunction

function pldip2dc(xmin, ymin, xmax, ymax)
% pldip2dc(xmin, ymin, xmax, ymax)
%
% Converts input values from relative plot coordinates to relative  device coordinates.  
  plplot_octave(133, xmin, ymin, xmax, ymax);
endfunction

function plsError(errcode, errmsg)
% plsError(errcode, errmsg)
%
% Set the variables to be used for storing error info
  plplot_octave(134, errcode, errmsg);
endfunction

function plsxwin(window_id)
% plsxwin(window_id)
%
% Set inferior X window  
  plplot_octave(135, window_id);
endfunction

