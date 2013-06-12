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
      integer :: DRAW_LINEX ! draw lines parallel to the X axis
      data DRAW_LINEX / z'01' /
      integer :: DRAW_LINEY ! draw lines parallel to the Y axis
      data DRAW_LINEY / z'02' /
      integer :: DRAW_LINEXY ! draw lines parallel to both the X and Y axes
      data DRAW_LINEXY / z'03' /
      integer :: MAG_COLOR ! draw the mesh with a color dependent of the magnitude
      data MAG_COLOR / z'04' /
      integer :: BASE_CONT ! draw contour plot at bottom xy plane
      data BASE_CONT / z'08' /
      integer :: TOP_CONT ! draw contour plot at top xy plane
      data TOP_CONT / z'10' /
      integer :: SURF_CONT ! draw contour plot at surface
      data SURF_CONT / z'20' /
      integer :: DRAW_SIDES ! draw sides
      data DRAW_SIDES / z'40' /
      integer :: FACETED ! draw outline for each square that makes up the surface
      data FACETED / z'80' /
      integer :: MESH ! draw mesh
      data MESH / z'100' /
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
      integer, parameter :: PL_COLORBAR_IMAGE = 16 
      integer, parameter :: PL_COLORBAR_SHADE = 32 
      integer, parameter :: PL_COLORBAR_GRADIENT = 64 
      integer, parameter :: PL_COLORBAR_CAP_NONE = 128 
      integer, parameter :: PL_COLORBAR_CAP_LOW = 256
      integer, parameter :: PL_COLORBAR_CAP_HIGH = 512 
      integer, parameter :: PL_COLORBAR_SHADE_LABEL = 1024
      integer, parameter :: PL_COLORBAR_ORIENT_RIGHT = 2048
      integer, parameter :: PL_COLORBAR_ORIENT_TOP = 4096
      integer, parameter :: PL_COLORBAR_ORIENT_LEFT = 8192
      integer, parameter :: PL_COLORBAR_ORIENT_BOTTOM = 16384
      integer, parameter :: PL_COLORBAR_BACKGROUND = 32768
      integer, parameter :: PL_COLORBAR_BOUNDING_BOX = 65536
      integer, parameter :: PLSWIN_DEVICE = 1 ! device coordinates
      integer, parameter :: PLSWIN_WORLD = 2 ! world coordinates
      integer, parameter :: PL_X_AXIS = 1 ! The x-axis
      integer, parameter :: PL_Y_AXIS = 2 ! The y-axis
      integer, parameter :: PL_Z_AXIS = 3 ! The z-axis
      integer :: PL_OPT_ENABLED ! Obsolete
      data PL_OPT_ENABLED / z'0001' /
      integer :: PL_OPT_ARG ! Option has an argment
      data PL_OPT_ARG / z'0002' /
      integer :: PL_OPT_NODELETE ! Don't delete after processing
      data PL_OPT_NODELETE / z'0004' /
      integer :: PL_OPT_INVISIBLE ! Make invisible
      data PL_OPT_INVISIBLE / z'0008' /
      integer :: PL_OPT_DISABLED ! Processing is disabled
      data PL_OPT_DISABLED / z'0010' /
      integer :: PL_OPT_FUNC ! Call handler function
      data PL_OPT_FUNC / z'0100' /
      integer :: PL_OPT_BOOL ! Set *var = 1
      data PL_OPT_BOOL / z'0200' /
      integer :: PL_OPT_INT ! Set *var = atoi(optarg)
      data PL_OPT_INT / z'0400' /
      integer :: PL_OPT_FLOAT ! Set *var = atof(optarg)
      data PL_OPT_FLOAT / z'0800' /
      integer :: PL_OPT_STRING ! Set var = optarg
      data PL_OPT_STRING / z'1000' /
      integer :: PL_PARSE_PARTIAL ! For backward compatibility
      data PL_PARSE_PARTIAL / z'0000' /
      integer :: PL_PARSE_FULL ! Process fully & exit if error
      data PL_PARSE_FULL / z'0001' /
      integer :: PL_PARSE_QUIET ! Don't issue messages
      data PL_PARSE_QUIET / z'0002' /
      integer :: PL_PARSE_NODELETE ! Don't delete options after
      data PL_PARSE_NODELETE / z'0004' /
      integer :: PL_PARSE_SHOWALL ! Show invisible options
      data PL_PARSE_SHOWALL / z'0008' /
      integer :: PL_PARSE_OVERRIDE ! Obsolete
      data PL_PARSE_OVERRIDE / z'0010' /
      integer :: PL_PARSE_NOPROGRAM ! Program name NOT in *argv[0]..
      data PL_PARSE_NOPROGRAM / z'0020' /
      integer :: PL_PARSE_NODASH ! Set if leading dash NOT required
      data PL_PARSE_NODASH / z'0040' /
      integer :: PL_PARSE_SKIP ! Skip over unrecognized args
      data PL_PARSE_SKIP / z'0080' /
      integer, parameter :: PL_FCI_MARK = ishft(1,31) 
      integer :: PL_FCI_IMPOSSIBLE 
      data PL_FCI_IMPOSSIBLE / z'00000000' /
      integer :: PL_FCI_HEXDIGIT_MASK 
      data PL_FCI_HEXDIGIT_MASK / z'f' /
      integer :: PL_FCI_HEXPOWER_MASK 
      data PL_FCI_HEXPOWER_MASK / z'7' /
      integer :: PL_FCI_HEXPOWER_IMPOSSIBLE 
      data PL_FCI_HEXPOWER_IMPOSSIBLE / z'f' /
      integer :: PL_FCI_FAMILY 
      data PL_FCI_FAMILY / z'0' /
      integer :: PL_FCI_STYLE 
      data PL_FCI_STYLE / z'1' /
      integer :: PL_FCI_WEIGHT 
      data PL_FCI_WEIGHT / z'2' /
      integer :: PL_FCI_SANS 
      data PL_FCI_SANS / z'0' /
      integer :: PL_FCI_SERIF 
      data PL_FCI_SERIF / z'1' /
      integer :: PL_FCI_MONO 
      data PL_FCI_MONO / z'2' /
      integer :: PL_FCI_SCRIPT 
      data PL_FCI_SCRIPT / z'3' /
      integer :: PL_FCI_SYMBOL 
      data PL_FCI_SYMBOL / z'4' /
      integer :: PL_FCI_UPRIGHT 
      data PL_FCI_UPRIGHT / z'0' /
      integer :: PL_FCI_ITALIC 
      data PL_FCI_ITALIC / z'1' /
      integer :: PL_FCI_OBLIQUE 
      data PL_FCI_OBLIQUE / z'2' /
      integer :: PL_FCI_MEDIUM 
      data PL_FCI_MEDIUM / z'0' /
      integer :: PL_FCI_BOLD 
      data PL_FCI_BOLD / z'1' /
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
