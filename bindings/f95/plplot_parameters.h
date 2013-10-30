!     Do not edit this generated file.  Instead, check its consistency
!     with the #defines in bindings/swig-support/plplotcapi.i using the
!     (Unix) target "check_f95_parameters".  If that target reports an
!     inconsistency (via a cmp message) between the generated
!     plplot_parameters.h_compare file in the build tree and
!     plplot_parameters.h in the source tree, then copy
!     plplot_parameters.h_compare on top of plplot_parameters.h and
!     check in that result.

      integer, parameter :: PLESC_SET_RGB = 1 ! obsolete
      integer, parameter :: PLESC_ALLOC_NCOL = 2 ! obsolete
      integer, parameter :: PLESC_SET_LPB = 3 ! obsolete
      integer, parameter :: PLESC_EXPOSE = 4 ! handle window expose
      integer, parameter :: PLESC_RESIZE = 5 ! handle window resize
      integer, parameter :: PLESC_REDRAW = 6 ! handle window redraw
      integer, parameter :: PLESC_TEXT = 7 ! switch to text screen
      integer, parameter :: PLESC_GRAPH = 8 ! switch to graphics screen
      integer, parameter :: PLESC_FILL = 9 ! fill polygon
      integer, parameter :: PLESC_DI = 10 ! handle DI command
      integer, parameter :: PLESC_FLUSH = 11 ! flush output
      integer, parameter :: PLESC_EH = 12 ! handle Window events
      integer, parameter :: PLESC_GETC = 13 ! get cursor position
      integer, parameter :: PLESC_SWIN = 14 ! set window parameters
      integer, parameter :: PLESC_PLFLTBUFFERING = 15 ! configure PLFLT buffering
      integer, parameter :: PLESC_XORMOD = 16 ! set xor mode
      integer, parameter :: PLESC_SET_COMPRESSION = 17 ! AFR: set compression
      integer, parameter :: PLESC_CLEAR = 18 ! RL: clear graphics region
      integer, parameter :: PLESC_DASH = 19 ! RL: draw dashed line
      integer, parameter :: PLESC_HAS_TEXT = 20 ! driver draws text
      integer, parameter :: PLESC_IMAGE = 21 ! handle image
      integer, parameter :: PLESC_IMAGEOPS = 22 ! plimage related operations
      integer, parameter :: DRAW_LINEX = 1 ! draw lines parallel to the X axis
      integer, parameter :: DRAW_LINEY = 2 ! draw lines parallel to the Y axis
      integer, parameter :: DRAW_LINEXY = 3 ! draw lines parallel to both the X and Y axes
      integer, parameter :: MAG_COLOR = 4 ! draw the mesh with a color dependent of the magnitude
      integer, parameter :: BASE_CONT = 8 ! draw contour plot at bottom xy plane
      integer, parameter :: TOP_CONT = 1*16 ! draw contour plot at top xy plane
      integer, parameter :: SURF_CONT = 2*16 ! draw contour plot at surface
      integer, parameter :: DRAW_SIDES = 4*16 ! draw sides
      integer, parameter :: FACETED = 8*16 ! draw outline for each square that makes up the surface
      integer, parameter :: MESH = 1*16*16 ! draw mesh
      integer, parameter :: PL_BIN_DEFAULT = 0 
      integer, parameter :: PL_BIN_CENTRED = 1 
      integer, parameter :: PL_BIN_NOEXPAND = 2 
      integer, parameter :: PL_BIN_NOEMPTY = 4 
      integer, parameter :: PL_HIST_DEFAULT = 0 
      integer, parameter :: PL_HIST_NOSCALING = 1 
      integer, parameter :: PL_HIST_IGNORE_OUTLIERS = 2 
      integer, parameter :: PL_HIST_NOEXPAND = 8 
      integer, parameter :: PL_HIST_NOEMPTY = 16 
      integer, parameter :: PL_POSITION_LEFT = 1 
      integer, parameter :: PL_POSITION_RIGHT = 2 
      integer, parameter :: PL_POSITION_TOP = 4 
      integer, parameter :: PL_POSITION_BOTTOM = 8 
      integer, parameter :: PL_POSITION_INSIDE = 16 
      integer, parameter :: PL_POSITION_OUTSIDE = 32 
      integer, parameter :: PL_POSITION_VIEWPORT = 64 
      integer, parameter :: PL_POSITION_SUBPAGE = 128 
      integer, parameter :: PL_LEGEND_NONE = 1 
      integer, parameter :: PL_LEGEND_COLOR_BOX = 2 
      integer, parameter :: PL_LEGEND_LINE = 4 
      integer, parameter :: PL_LEGEND_SYMBOL = 8 
      integer, parameter :: PL_LEGEND_TEXT_LEFT = 16 
      integer, parameter :: PL_LEGEND_BACKGROUND = 32 
      integer, parameter :: PL_LEGEND_BOUNDING_BOX = 64 
      integer, parameter :: PL_LEGEND_ROW_MAJOR = 128 
      integer, parameter :: PL_COLORBAR_LABEL_LEFT = 1 
      integer, parameter :: PL_COLORBAR_LABEL_RIGHT = 2 
      integer, parameter :: PL_COLORBAR_LABEL_TOP = 4 
      integer, parameter :: PL_COLORBAR_LABEL_BOTTOM = 8 
      integer, parameter :: PL_COLORBAR_IMAGE = 1*16 
      integer, parameter :: PL_COLORBAR_SHADE = 2*16 
      integer, parameter :: PL_COLORBAR_GRADIENT = 4*16 
      integer, parameter :: PL_COLORBAR_CAP_NONE = 8*16 
      integer, parameter :: PL_COLORBAR_CAP_LOW = 1*16*16 
      integer, parameter :: PL_COLORBAR_CAP_HIGH = 2*16*16 
      integer, parameter :: PL_COLORBAR_SHADE_LABEL = 4*16*16 
      integer, parameter :: PL_COLORBAR_ORIENT_RIGHT = 8*16*16 
      integer, parameter :: PL_COLORBAR_ORIENT_TOP = 1*16*16*16 
      integer, parameter :: PL_COLORBAR_ORIENT_LEFT = 2*16*16*16 
      integer, parameter :: PL_COLORBAR_ORIENT_BOTTOM = 4*16*16*16 
      integer, parameter :: PL_COLORBAR_BACKGROUND = 8*16*16*16 
      integer, parameter :: PL_COLORBAR_BOUNDING_BOX = 1*16*16*16*16 
      integer, parameter :: PLSWIN_DEVICE = 1 ! device coordinates
      integer, parameter :: PLSWIN_WORLD = 2 ! world coordinates
      integer, parameter :: PL_X_AXIS = 1 ! The x-axis
      integer, parameter :: PL_Y_AXIS = 2 ! The y-axis
      integer, parameter :: PL_Z_AXIS = 3 ! The z-axis
      integer, parameter :: PL_OPT_ENABLED = 1 ! Obsolete
      integer, parameter :: PL_OPT_ARG = 2 ! Option has an argment
      integer, parameter :: PL_OPT_NODELETE = 4 ! Don't delete after processing
      integer, parameter :: PL_OPT_INVISIBLE = 8 ! Make invisible
      integer, parameter :: PL_OPT_DISABLED = 1*16 ! Processing is disabled
      integer, parameter :: PL_OPT_FUNC = 1*16*16 ! Call handler function
      integer, parameter :: PL_OPT_BOOL = 2*16*16 ! Set *var = 1
      integer, parameter :: PL_OPT_INT = 4*16*16 ! Set *var = atoi(optarg)
      integer, parameter :: PL_OPT_FLOAT = 8*16*16 ! Set *var = atof(optarg)
      integer, parameter :: PL_OPT_STRING = 1*16*16*16 ! Set var = optarg
      integer, parameter :: PL_PARSE_PARTIAL = 0 ! For backward compatibility
      integer, parameter :: PL_PARSE_FULL = 1 ! Process fully & exit if error
      integer, parameter :: PL_PARSE_QUIET = 2 ! Don't issue messages
      integer, parameter :: PL_PARSE_NODELETE = 4 ! Don't delete options after
      integer, parameter :: PL_PARSE_SHOWALL = 8 ! Show invisible options
      integer, parameter :: PL_PARSE_OVERRIDE = 1*16 ! Obsolete
      integer, parameter :: PL_PARSE_NOPROGRAM = 2*16 ! Program name NOT in *argv[0]..
      integer, parameter :: PL_PARSE_NODASH = 4*16 ! Set if leading dash NOT required
      integer, parameter :: PL_PARSE_SKIP = 8*16 ! Skip over unrecognized args
      integer, parameter :: PL_FCI_MARK = ishft(1,31) 
      integer, parameter :: PL_FCI_IMPOSSIBLE = 0 
      integer, parameter :: PL_FCI_HEXDIGIT_MASK = 15 
      integer, parameter :: PL_FCI_HEXPOWER_MASK = 7 
      integer, parameter :: PL_FCI_HEXPOWER_IMPOSSIBLE = 15 
      integer, parameter :: PL_FCI_FAMILY = 0 
      integer, parameter :: PL_FCI_STYLE = 1 
      integer, parameter :: PL_FCI_WEIGHT = 2 
      integer, parameter :: PL_FCI_SANS = 0 
      integer, parameter :: PL_FCI_SERIF = 1 
      integer, parameter :: PL_FCI_MONO = 2 
      integer, parameter :: PL_FCI_SCRIPT = 3 
      integer, parameter :: PL_FCI_SYMBOL = 4 
      integer, parameter :: PL_FCI_UPRIGHT = 0 
      integer, parameter :: PL_FCI_ITALIC = 1 
      integer, parameter :: PL_FCI_OBLIQUE = 2 
      integer, parameter :: PL_FCI_MEDIUM = 0 
      integer, parameter :: PL_FCI_BOLD = 1 
      integer, parameter :: PL_MAXKEY = 16 
      integer, parameter :: PL_MAXWINDOWS = 64 ! Max number of windows/page tracked
      real(kind=plflt), parameter :: PL_NOTSET = -42.0_plflt
      real(kind=plflt), parameter :: PL_PI = 3.1415926535897932384_plflt
      real(kind=plflt), parameter :: PL_TWOPI = 2.0_plflt*PL_PI
      integer, parameter :: PLESPLFLTBUFFERING_ENABLE = 1 
      integer, parameter :: PLESPLFLTBUFFERING_DISABLE = 2 
      integer, parameter :: PLESPLFLTBUFFERING_QUERY = 3 
      integer, parameter :: GRID_CSA = 1 ! Bivariate Cubic Spline approximation
      integer, parameter :: GRID_DTLI = 2 ! Delaunay Triangulation Linear Interpolation
      integer, parameter :: GRID_NNI = 3 ! Natural Neighbors Interpolation
      integer, parameter :: GRID_NNIDW = 4 ! Nearest Neighbors Inverse Distance Weighted
      integer, parameter :: GRID_NNLI = 5 ! Nearest Neighbors Linear Interpolation
      integer, parameter :: GRID_NNAIDW = 6 ! Nearest Neighbors Around Inverse Distance Weighted
