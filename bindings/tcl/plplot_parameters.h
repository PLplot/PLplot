// Do not edit this generated file.  Instead, check its consistency
// with the #defines in bindings/swig-support/plplotcapi.i using the
// (Unix) target "check_tcl_parameters".  If that target reports an
// inconsistency (via a cmp message) between the generated
// plplot_parameters.h_compare file in the build tree and
// plplot_parameters.h in the source tree, then copy
// plplot_parameters.h_compare on top of plplot_parameters.h and
// check in that result.

//  This file contains a function to set the various global variables
//  used by PLplot

static void set_plplot_parameters( Tcl_Interp *interp )
{
    Tcl_Eval( interp, "namespace eval ::PLPLOT { \n\
\n\
# obsolete\n\
variable PLESC_SET_RGB [expr 1]\n\
\n\
# obsolete\n\
variable PLESC_ALLOC_NCOL [expr 2]\n\
\n\
# obsolete\n\
variable PLESC_SET_LPB [expr 3]\n\
\n\
# handle window expose\n\
variable PLESC_EXPOSE [expr 4]\n\
\n\
# handle window resize\n\
variable PLESC_RESIZE [expr 5]\n\
\n\
# handle window redraw\n\
variable PLESC_REDRAW [expr 6]\n\
\n\
# switch to text screen\n\
variable PLESC_TEXT [expr 7]\n\
\n\
# switch to graphics screen\n\
variable PLESC_GRAPH [expr 8]\n\
\n\
# fill polygon\n\
variable PLESC_FILL [expr 9]\n\
\n\
# handle DI command\n\
variable PLESC_DI [expr 10]\n\
\n\
# flush output\n\
variable PLESC_FLUSH [expr 11]\n\
\n\
# handle Window events\n\
variable PLESC_EH [expr 12]\n\
\n\
# get cursor position\n\
variable PLESC_GETC [expr 13]\n\
\n\
# set window parameters\n\
variable PLESC_SWIN [expr 14]\n\
\n\
# configure double buffering\n\
variable PLESC_DOUBLEBUFFERING [expr 15]\n\
\n\
# set xor mode\n\
variable PLESC_XORMOD [expr 16]\n\
\n\
# AFR: set compression\n\
variable PLESC_SET_COMPRESSION [expr 17]\n\
\n\
# RL: clear graphics region\n\
variable PLESC_CLEAR [expr 18]\n\
\n\
# RL: draw dashed line\n\
variable PLESC_DASH [expr 19]\n\
\n\
# driver draws text\n\
variable PLESC_HAS_TEXT [expr 20]\n\
\n\
# handle image\n\
variable PLESC_IMAGE [expr 21]\n\
\n\
# plimage related operations\n\
variable PLESC_IMAGEOPS [expr 22]\n\
\n\
# convert PLColor to device color\n\
variable PLESC_PL2DEVCOL [expr 23]\n\
\n\
# convert device color to PLColor\n\
variable PLESC_DEV2PLCOL [expr 24]\n\
\n\
# set BG, FG colors\n\
variable PLESC_SETBGFG [expr 25]\n\
\n\
# alternate device initialization\n\
variable PLESC_DEVINIT [expr 26]\n\
\n\
# get used backend of (wxWidgets) driver\n\
variable PLESC_GETBACKEND [expr 27]\n\
\n\
# get ready to draw a line of text\n\
variable PLESC_BEGIN_TEXT [expr 28]\n\
\n\
# render a character of text\n\
variable PLESC_TEXT_CHAR [expr 29]\n\
\n\
# handle a text control character (super/subscript, etc.)\n\
variable PLESC_CONTROL_CHAR [expr 30]\n\
\n\
# finish a drawing a line of text\n\
variable PLESC_END_TEXT [expr 31]\n\
\n\
# start rasterized rendering\n\
variable PLESC_START_RASTERIZE [expr 32]\n\
\n\
# end rasterized rendering\n\
variable PLESC_END_RASTERIZE [expr 33]\n\
\n\
# render an arc\n\
variable PLESC_ARC [expr 34]\n\
\n\
# render a gradient\n\
variable PLESC_GRADIENT [expr 35]\n\
\n\
# set drawing mode\n\
variable PLESC_MODESET [expr 36]\n\
\n\
# get drawing mode\n\
variable PLESC_MODEGET [expr 37]\n\
\n\
# font change in the text stream\n\
variable PLTEXT_FONTCHANGE [expr 0]\n\
\n\
# superscript in the text stream\n\
variable PLTEXT_SUPERSCRIPT [expr 1]\n\
\n\
# subscript in the text stream\n\
variable PLTEXT_SUBSCRIPT [expr 2]\n\
\n\
# back-char in the text stream\n\
variable PLTEXT_BACKCHAR [expr 3]\n\
\n\
# toggle overline in the text stream\n\
variable PLTEXT_OVERLINE [expr 4]\n\
\n\
# toggle underline in the text stream\n\
variable PLTEXT_UNDERLINE [expr 5]\n\
\n\
\n\
variable ZEROW2B [expr 1]\n\
\n\
\n\
variable ZEROW2D [expr 2]\n\
\n\
\n\
variable ONEW2B [expr 3]\n\
\n\
\n\
variable ONEW2D [expr 4]\n\
\n\
# device coordinates\n\
variable PLSWIN_DEVICE [expr 1]\n\
\n\
# world coordinates\n\
variable PLSWIN_WORLD [expr 2]\n\
\n\
# The x-axis\n\
variable PL_X_AXIS [expr 1]\n\
\n\
# The y-axis\n\
variable PL_Y_AXIS [expr 2]\n\
\n\
# The z-axis\n\
variable PL_Z_AXIS [expr 3]\n\
\n\
# Obsolete\n\
variable PL_OPT_ENABLED [expr 0x0001]\n\
\n\
# Option has an argment\n\
variable PL_OPT_ARG [expr 0x0002]\n\
\n\
# Don't delete after processing\n\
variable PL_OPT_NODELETE [expr 0x0004]\n\
\n\
# Make invisible\n\
variable PL_OPT_INVISIBLE [expr 0x0008]\n\
\n\
# Processing is disabled\n\
variable PL_OPT_DISABLED [expr 0x0010]\n\
\n\
# Call handler function\n\
variable PL_OPT_FUNC [expr 0x0100]\n\
\n\
# Set *var = 1\n\
variable PL_OPT_BOOL [expr 0x0200]\n\
\n\
# Set *var = atoi(optarg)\n\
variable PL_OPT_INT [expr 0x0400]\n\
\n\
# Set *var = atof(optarg)\n\
variable PL_OPT_FLOAT [expr 0x0800]\n\
\n\
# Set var = optarg\n\
variable PL_OPT_STRING [expr 0x1000]\n\
\n\
# For backward compatibility\n\
variable PL_PARSE_PARTIAL [expr 0x0000]\n\
\n\
# Process fully & exit if error\n\
variable PL_PARSE_FULL [expr 0x0001]\n\
\n\
# Don't issue messages\n\
variable PL_PARSE_QUIET [expr 0x0002]\n\
\n\
# Don't delete options after\n\
variable PL_PARSE_NODELETE [expr 0x0004]\n\
\n\
# Show invisible options\n\
variable PL_PARSE_SHOWALL [expr 0x0008]\n\
\n\
# Obsolete\n\
variable PL_PARSE_OVERRIDE [expr 0x0010]\n\
\n\
# Program name NOT in *argv[0]..\n\
variable PL_PARSE_NOPROGRAM [expr 0x0020]\n\
\n\
# Set if leading dash NOT required\n\
variable PL_PARSE_NODASH [expr 0x0040]\n\
\n\
# Skip over unrecognized args\n\
variable PL_PARSE_SKIP [expr 0x0080]\n\
\n\
\n\
variable PL_FCI_MARK [expr 0x80000000]\n\
\n\
\n\
variable PL_FCI_IMPOSSIBLE [expr 0x00000000]\n\
\n\
\n\
variable PL_FCI_HEXDIGIT_MASK [expr 0xf]\n\
\n\
\n\
variable PL_FCI_HEXPOWER_MASK [expr 0x7]\n\
\n\
\n\
variable PL_FCI_HEXPOWER_IMPOSSIBLE [expr 0xf]\n\
\n\
\n\
variable PL_FCI_FAMILY [expr 0x0]\n\
\n\
\n\
variable PL_FCI_STYLE [expr 0x1]\n\
\n\
\n\
variable PL_FCI_WEIGHT [expr 0x2]\n\
\n\
\n\
variable PL_FCI_SANS [expr 0x0]\n\
\n\
\n\
variable PL_FCI_SERIF [expr 0x1]\n\
\n\
\n\
variable PL_FCI_MONO [expr 0x2]\n\
\n\
\n\
variable PL_FCI_SCRIPT [expr 0x3]\n\
\n\
\n\
variable PL_FCI_SYMBOL [expr 0x4]\n\
\n\
\n\
variable PL_FCI_UPRIGHT [expr 0x0]\n\
\n\
\n\
variable PL_FCI_ITALIC [expr 0x1]\n\
\n\
\n\
variable PL_FCI_OBLIQUE [expr 0x2]\n\
\n\
\n\
variable PL_FCI_MEDIUM [expr 0x0]\n\
\n\
\n\
variable PL_FCI_BOLD [expr 0x1]\n\
\n\
\n\
variable PL_MAXKEY [expr 16]\n\
\n\
# Max number of windows/page tracked\n\
variable PL_MAXWINDOWS [expr 64]\n\
\n\
\n\
variable PL_NOTSET [expr -42]\n\
\n\
\n\
variable PL_PI 3.1415926535897932384\n\
\n\
\n\
variable PLESC_DOUBLEBUFFERING_ENABLE [expr 1]\n\
\n\
\n\
variable PLESC_DOUBLEBUFFERING_DISABLE [expr 2]\n\
\n\
\n\
variable PLESC_DOUBLEBUFFERING_QUERY [expr 3]\n\
\n\
\n\
variable PL_BIN_DEFAULT [expr 0x0]\n\
\n\
\n\
variable PL_BIN_CENTRED [expr 0x1]\n\
\n\
\n\
variable PL_BIN_NOEXPAND [expr 0x2]\n\
\n\
\n\
variable PL_BIN_NOEMPTY [expr 0x4]\n\
\n\
# Bivariate Cubic Spline approximation\n\
variable GRID_CSA [expr 1]\n\
\n\
# Delaunay Triangulation Linear Interpolation\n\
variable GRID_DTLI [expr 2]\n\
\n\
# Natural Neighbors Interpolation\n\
variable GRID_NNI [expr 3]\n\
\n\
# Nearest Neighbors Inverse Distance Weighted\n\
variable GRID_NNIDW [expr 4]\n\
\n\
# Nearest Neighbors Linear Interpolation\n\
variable GRID_NNLI [expr 5]\n\
\n\
# Nearest Neighbors Around Inverse Distance Weighted\n\
variable GRID_NNAIDW [expr 6]\n\
\n\
\n\
variable PL_HIST_DEFAULT [expr 0x00]\n\
\n\
\n\
variable PL_HIST_NOSCALING [expr 0x01]\n\
\n\
\n\
variable PL_HIST_IGNORE_OUTLIERS [expr 0x02]\n\
\n\
\n\
variable PL_HIST_NOEXPAND [expr 0x08]\n\
\n\
\n\
variable PL_HIST_NOEMPTY [expr 0x10]\n\
\n\
\n\
variable PL_POSITION_LEFT [expr 0x1]\n\
\n\
\n\
variable PL_POSITION_RIGHT [expr 0x2]\n\
\n\
\n\
variable PL_POSITION_TOP [expr 0x4]\n\
\n\
\n\
variable PL_POSITION_BOTTOM [expr 0x8]\n\
\n\
\n\
variable PL_POSITION_INSIDE [expr 0x10]\n\
\n\
\n\
variable PL_POSITION_OUTSIDE [expr 0x20]\n\
\n\
\n\
variable PL_POSITION_VIEWPORT [expr 0x40]\n\
\n\
\n\
variable PL_POSITION_SUBPAGE [expr 0x80]\n\
\n\
\n\
variable PL_LEGEND_NONE [expr 0x1]\n\
\n\
\n\
variable PL_LEGEND_COLOR_BOX [expr 0x2]\n\
\n\
\n\
variable PL_LEGEND_LINE [expr 0x4]\n\
\n\
\n\
variable PL_LEGEND_SYMBOL [expr 0x8]\n\
\n\
\n\
variable PL_LEGEND_TEXT_LEFT [expr 0x10]\n\
\n\
\n\
variable PL_LEGEND_BACKGROUND [expr 0x20]\n\
\n\
\n\
variable PL_LEGEND_BOUNDING_BOX [expr 0x40]\n\
\n\
\n\
variable PL_LEGEND_ROW_MAJOR [expr 0x80]\n\
\n\
\n\
variable PL_COLORBAR_LABEL_LEFT [expr 0x1]\n\
\n\
\n\
variable PL_COLORBAR_LABEL_RIGHT [expr 0x2]\n\
\n\
\n\
variable PL_COLORBAR_LABEL_TOP [expr 0x4]\n\
\n\
\n\
variable PL_COLORBAR_LABEL_BOTTOM [expr 0x8]\n\
\n\
\n\
variable PL_COLORBAR_IMAGE [expr 0x10]\n\
\n\
\n\
variable PL_COLORBAR_SHADE [expr 0x20]\n\
\n\
\n\
variable PL_COLORBAR_GRADIENT [expr 0x40]\n\
\n\
\n\
variable PL_COLORBAR_CAP_NONE [expr 0x80]\n\
\n\
\n\
variable PL_COLORBAR_CAP_LOW [expr 0x100]\n\
\n\
\n\
variable PL_COLORBAR_CAP_HIGH [expr 0x200]\n\
\n\
\n\
variable PL_COLORBAR_SHADE_LABEL [expr 0x400]\n\
\n\
\n\
variable PL_COLORBAR_ORIENT_RIGHT [expr 0x800]\n\
\n\
\n\
variable PL_COLORBAR_ORIENT_TOP [expr 0x1000]\n\
\n\
\n\
variable PL_COLORBAR_ORIENT_LEFT [expr 0x2000]\n\
\n\
\n\
variable PL_COLORBAR_ORIENT_BOTTOM [expr 0x4000]\n\
\n\
\n\
variable PL_COLORBAR_BACKGROUND [expr 0x8000]\n\
\n\
\n\
variable PL_COLORBAR_BOUNDING_BOX [expr 0x10000]\n\
\n\
\n\
variable PL_DRAWMODE_UNKNOWN [expr 0x0]\n\
\n\
\n\
variable PL_DRAWMODE_DEFAULT [expr 0x1]\n\
\n\
\n\
variable PL_DRAWMODE_REPLACE [expr 0x2]\n\
\n\
\n\
variable PL_DRAWMODE_XOR [expr 0x4]\n\
\n\
# draw lines parallel to the X axis\n\
variable DRAW_LINEX [expr 0x001]\n\
\n\
# draw lines parallel to the Y axis\n\
variable DRAW_LINEY [expr 0x002]\n\
\n\
# draw lines parallel to both the X and Y axis\n\
variable DRAW_LINEXY [expr 0x003]\n\
\n\
# draw the mesh with a color dependent of the magnitude\n\
variable MAG_COLOR [expr 0x004]\n\
\n\
# draw contour plot at bottom xy plane\n\
variable BASE_CONT [expr 0x008]\n\
\n\
# draw contour plot at top xy plane\n\
variable TOP_CONT [expr 0x010]\n\
\n\
# draw contour plot at surface\n\
variable SURF_CONT [expr 0x020]\n\
\n\
# draw sides\n\
variable DRAW_SIDES [expr 0x040]\n\
\n\
# draw outline for each square that makes up the surface\n\
variable FACETED [expr 0x080]\n\
\n\
# draw mesh\n\
variable MESH [expr 0x100]\n\
}" );
}
