Fortran 95 bindings for PLplot
------------------------------

dd. 25 november 2005

Introduction
------------
The Fortran 95 bindings for PLplot have been set up with the following
considerations in mind:
- reuse the bindings for FORTRAN 77 if possible
- provide alternative calls for the various routines to take advantage
  of the features of Fortran 95, most notably, to simplify the interface
- provide a module that takes care of all the details

Fortran 90 versus Fortran 95
----------------------------
Because the Fortran 95 standard is supported by most if not all current
Fortran compilers, it seemed logical to target this standard, rather
than Fortran 90. The differences are small enough though not to matter
too much for PLplot.

To stress that Fortran 95 is the target, we use .f95 as the extension
for the examples. Note that the extension is not important for
most compilers: it merely indicates that the freeform source format is
used.

PLplot module
-------------
The PLplot module defines:
- Several parameters for use with the PLplot library
- Interfaces for all the routines that can be used with Fortran 95
  programs
- Several alternative interfaces for various routines

The parameters are:
- PLFLT or PLF for short
  This is the KIND parameter for all floating-point variables and values
  used by the PLplot library. See the example below for its use

- PL_PARSE_FULL
  A parameter used in the call to PLPARSEOPTS

- PL_PI
  The approximation of the number "pi". It is used in many examples
  and because it is often defined in other modules as well, we have
  given it a more specific name.
  Should you prefer "PI" (or "pi") then use the module like this:

     program myprog
        use plplot, pi => pl_pi
        ...

- PL_GRID_CSA, PL_GRID_DTLI, PL_GRID_NNI, PL_GRID_NNIDW, PL_GRID_NNLI,
  PL_GRID_NNAIDW
  Parameters that select the gridding algorithm for PLGRIDDATA

- PL_JUST_NONE, PL_JUST_OPTIMAL, PL_JUST_SAME, PL_JUST_ISOMETRIC:
  The justification of axes in PLENV. The names should be more or
  less self-explanatory.

- PL_AXIS_NOTHING    - no decoration for the axis
  PL_AXIS_BOX_ONLY   - only the axles
  PL_AXIS_NORMAL     = ordinary axes
  PL_AXIS_ATZERO     = include lines through 0-coordinates
  PL_AXIS_MAJORGRID  = include grid for major tickmarks
  PL_AXIS_MINORGRID  = include grid for major and minor tickmarks
  PL_AXIS_LOGX       = use logarithmic X axis
  PL_AXIS_LOGY       = use logarithmic Y axis
  PL_AXIS_LOGXY      = both axes logarithmic

  You can combine the logarithmic options with the others
  (except PL_AXIS_NOTHING and PL_AXIS_BOX_ONLY) by simply
  adding them: PL_AXIS_LOGX+AXIS_MAJORGRID for instance

To illustrate the alternative interfaces for various routines,
here are the possibilities to call PLBIN

The C API reads:

void
plbin(PLINT nbin, PLFLT *x, PLFLT *y, PLINT center);

Because in Fortran 95, the size of the array can be queried via the SIZE
intrinsic function, we can drop the nbin argument. And while C has no
specific logical data type, Fortran has. So there are four ways to call
PLBIN from Fortran 95:

   logical                        :: center
   integer                        :: icenter, nbin
   real(kind=plflt), dimension(:) :: x, y

   !
   ! Fortran 95, method 1 (compatible with FORTRAN 77)
   !
   icenter = 1
   nbin    = size(x)
   call plbin( nbin, x, y, icenter )

   !
   ! Fortran 95, method 2
   !
   center = .true.
   call plbin( x, y, center )  ! Drop the argument nbin, use a logical for center

   !
   ! Fortran 95, method 3
   !
   icenter = 1
   call plbin( x, y, icenter )  ! No nbin, use an integer for center

   !
   ! Fortran 95, method 4
   !
   center = .true.
   call plbin( nbin, x, y, center )  ! Use nbin, use a logical for center

Method 2 is preferred, as it is most "Fortran 95" in character.

