      !
      ! Parameters connecting a mnemonic name to the
      ! constants used by various routines
      !
      integer, parameter :: PL_PARSE_FULL = 1

      integer, parameter :: PL_GRID_CSA    = 1
      integer, parameter :: PL_GRID_DTLI   = 2
      integer, parameter :: PL_GRID_NNI    = 3
      integer, parameter :: PL_GRID_NNIDW  = 4
      integer, parameter :: PL_GRID_NNLI   = 5
      integer, parameter :: PL_GRID_NNAIDW = 6

      integer, parameter :: PL_JUST_NONE      = -1
      integer, parameter :: PL_JUST_OPTIMAL   =  0
      integer, parameter :: PL_JUST_SAME      =  1
      integer, parameter :: PL_JUST_ISOMETRIC =  2

      integer, parameter :: PL_AXIS_NOTHING    = -2
      integer, parameter :: PL_AXIS_BOX_ONLY   = -1
      integer, parameter :: PL_AXIS_NORMAL     =  0
      integer, parameter :: PL_AXIS_ATZERO     =  1
      integer, parameter :: PL_AXIS_MAJORGRID  =  2
      integer, parameter :: PL_AXIS_MINORGRID  =  3
      integer, parameter :: PL_AXIS_LOGX       = 10
      integer, parameter :: PL_AXIS_LOGY       = 20
      integer, parameter :: PL_AXIS_LOGXY      = 30

      integer, parameter :: PL_FONT_STANDARD   =  0
      integer, parameter :: PL_FONT_EXTENDED   =  1

      real(kind=plflt), parameter :: PL_NOTSET = -42.0_plflt

      !
      ! TODO:
      ! Parameters for fill patterns and symbols
      !

      real(kind=plflt), parameter :: PL_PI = 3.1415926535897932384_plflt

