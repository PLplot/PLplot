Fortran 95 bindings for PLplot
------------------------------

dd. 25 november 2005

Introduction
------------
The Fortran 95 bindings for PLplot have been set up with the following
considerations in mind:
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
   ! Fortran 95, method 1
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

Linking DLLs on Windows
-----------------------
On Windows it is necessary to specify which routines and functions in
a DLL should be exported, i.e. made visible to external programs. While
there compiler-specific ways to do that within the source code we prefer
to do it via so-called linker definition files (.def).

Basically, these files contain a list of all the subroutines and functions
that need to be exported, one name per line. The difficulty is, however,
that these names are the names as constructed by the compiler. Each compiler
is free to use its own scheme for turning the name found in the Fortran code
into a "linker" name".

For the Compaq Visual Fortran the scheme is this:

subroutine/function Name( a, b ) ==> _NAME@8

   module plplotp
   contains
   subroutine/function Name( a, b ) ==> _PLPLOTP_mp_NAME@8
   end module

where the @8 represents the number of bytes in the argument list (the
hidden argument representing the length of string arguments must be
included in that count)

For the MinGW gfortran compiler the scheme is somewhat simpler:

   subroutine/function Name( a, b ) ==> name_
   subroutine/function My_Name( a, b ) ==> my_name__

   module plplotp
   contains
   subroutine/function Name( a, b ) ==> __plplotp_MOD_name
   subroutine/function My_Name( a, b ) ==> __plplotp_MOD_my_name
   end module

For the Cygwin gfortran compiler all symbols are automatically exported. There
is no need for a linker definition file.

One way to find out what the internally produced names are is to examine the
object file (.obj) that is produced by the compiler.

Notes:
- The compiler adds the name of the module to the name of the routine
- The MinGW compiler does not append underscores for routines living
  in a module, but it does for routines outside a module
- All routines that are mentioned in an interface block must be exported:

  The four routines in this fragment must all be exported via the
  linker definition file, even though only the interface name is useable
  in the Fortran code (the routines themselves are not visible to a
  Fortran program because of the private clause):

     interface plvect
        module procedure plvectors_0
        module procedure plvectors_1
        module procedure plvectors_2
        module procedure plvectors_tr
     end interface
     private :: plvectors_0, plvectors_1, plvectors_2, plvectors_tr

  So the definition file has these lines:

     __plplotp_MOD_plvectors_0
     __plplotp_MOD_plvectors_1
     __plplotp_MOD_plvectors_2
     __plplotp_MOD_plvectors_tr

  (but no line for the interface name as that is stored in the
  module intermediate file used by the compiler)

