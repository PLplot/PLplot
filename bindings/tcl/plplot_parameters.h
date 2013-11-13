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
variable PLESC_SET_RGB 1\n\
\n\
# obsolete\n\
variable PLESC_ALLOC_NCOL 2\n\
\n\
# obsolete\n\
variable PLESC_SET_LPB 3\n\
\n\
# handle window expose\n\
variable PLESC_EXPOSE 4\n\
\n\
# handle window resize\n\
variable PLESC_RESIZE 5\n\
\n\
# handle window redraw\n\
variable PLESC_REDRAW 6\n\
\n\
# switch to text screen\n\
variable PLESC_TEXT 7\n\
\n\
# switch to graphics screen\n\
variable PLESC_GRAPH 8\n\
\n\
# fill polygon\n\
variable PLESC_FILL 9\n\
\n\
# handle DI command\n\
variable PLESC_DI 10\n\
\n\
# flush output\n\
variable PLESC_FLUSH 11\n\
\n\
# handle Window events\n\
variable PLESC_EH 12\n\
\n\
# get cursor position\n\
variable PLESC_GETC 13\n\
\n\
# set window parameters\n\
variable PLESC_SWIN 14\n\
\n\
# configure double buffering\n\
variable PLESC_DOUBLEBUFFERING 15\n\
\n\
# set xor mode\n\
variable PLESC_XORMOD 16\n\
\n\
# AFR: set compression\n\
variable PLESC_SET_COMPRESSION 17\n\
\n\
# RL: clear graphics region\n\
variable PLESC_CLEAR 18\n\
\n\
# RL: draw dashed line\n\
variable PLESC_DASH 19\n\
\n\
# driver draws text\n\
variable PLESC_HAS_TEXT 20\n\
\n\
# handle image\n\
variable PLESC_IMAGE 21\n\
\n\
# plimage related operations\n\
variable PLESC_IMAGEOPS 22\n\
\n\
# convert PLColor to device color\n\
variable PLESC_PL2DEVCOL 23\n\
\n\
# convert device color to PLColor\n\
variable PLESC_DEV2PLCOL 24\n\
\n\
# set BG, FG colors\n\
variable PLESC_SETBGFG 25\n\
\n\
# alternate device initialization\n\
variable PLESC_DEVINIT 26\n\
\n\
# get used backend of (wxWidgets) driver\n\
variable PLESC_GETBACKEND 27\n\
\n\
# get ready to draw a line of text\n\
variable PLESC_BEGIN_TEXT 28\n\
\n\
# render a character of text\n\
variable PLESC_TEXT_CHAR 29\n\
\n\
# handle a text control character (super/subscript, etc.)\n\
variable PLESC_CONTROL_CHAR 30\n\
\n\
# finish a drawing a line of text\n\
variable PLESC_END_TEXT 31\n\
\n\
# start rasterized rendering\n\
variable PLESC_START_RASTERIZE 32\n\
\n\
# end rasterized rendering\n\
variable PLESC_END_RASTERIZE 33\n\
\n\
# render an arc\n\
variable PLESC_ARC 34\n\
\n\
# render a gradient\n\
variable PLESC_GRADIENT 35\n\
\n\
# set drawing mode\n\
variable PLESC_MODESET 36\n\
\n\
# get drawing mode\n\
variable PLESC_MODEGET 37\n\
\n\
# font change in the text stream\n\
variable PLTEXT_FONTCHANGE 0\n\
\n\
# superscript in the text stream\n\
variable PLTEXT_SUPERSCRIPT 1\n\
\n\
# subscript in the text stream\n\
variable PLTEXT_SUBSCRIPT 2\n\
\n\
# back-char in the text stream\n\
variable PLTEXT_BACKCHAR 3\n\
\n\
# toggle overline in the text stream\n\
variable PLTEXT_OVERLINE 4\n\
\n\
# toggle underline in the text stream\n\
variable PLTEXT_UNDERLINE 5\n\
\n\
# device coordinates\n\
variable PLSWIN_DEVICE 1\n\
\n\
# world coordinates\n\
variable PLSWIN_WORLD 2\n\
\n\
# The x-axis\n\
variable PL_X_AXIS 1\n\
\n\
# The y-axis\n\
variable PL_Y_AXIS 2\n\
\n\
# The z-axis\n\
variable PL_Z_AXIS 3\n\
\n\
# Obsolete\n\
variable PL_OPT_ENABLED 0x0001\n\
\n\
# Option has an argment\n\
variable PL_OPT_ARG 0x0002\n\
\n\
# Don't delete after processing\n\
variable PL_OPT_NODELETE 0x0004\n\
\n\
# Make invisible\n\
variable PL_OPT_INVISIBLE 0x0008\n\
\n\
# Processing is disabled\n\
variable PL_OPT_DISABLED 0x0010\n\
\n\
# Call handler function\n\
variable PL_OPT_FUNC 0x0100\n\
\n\
# Set *var = 1\n\
variable PL_OPT_BOOL 0x0200\n\
\n\
# Set *var = atoi(optarg)\n\
variable PL_OPT_INT 0x0400\n\
\n\
# Set *var = atof(optarg)\n\
variable PL_OPT_FLOAT 0x0800\n\
\n\
# Set var = optarg\n\
variable PL_OPT_STRING 0x1000\n\
\n\
# For backward compatibility\n\
variable PL_PARSE_PARTIAL 0x0000\n\
\n\
# Process fully & exit if error\n\
variable PL_PARSE_FULL 0x0001\n\
\n\
# Don't issue messages\n\
variable PL_PARSE_QUIET 0x0002\n\
\n\
# Don't delete options after\n\
variable PL_PARSE_NODELETE 0x0004\n\
\n\
# Show invisible options\n\
variable PL_PARSE_SHOWALL 0x0008\n\
\n\
# Obsolete\n\
variable PL_PARSE_OVERRIDE 0x0010\n\
\n\
# Program name NOT in *argv[0]..\n\
variable PL_PARSE_NOPROGRAM 0x0020\n\
\n\
# Set if leading dash NOT required\n\
variable PL_PARSE_NODASH 0x0040\n\
\n\
# Skip over unrecognized args\n\
variable PL_PARSE_SKIP 0x0080\n\
\n\
\n\
variable PL_FCI_MARK 0x80000000\n\
\n\
\n\
variable PL_FCI_IMPOSSIBLE 0x00000000\n\
\n\
\n\
variable PL_FCI_HEXDIGIT_MASK 0xf\n\
\n\
\n\
variable PL_FCI_HEXPOWER_MASK 0x7\n\
\n\
\n\
variable PL_FCI_HEXPOWER_IMPOSSIBLE 0xf\n\
\n\
\n\
variable PL_FCI_FAMILY 0x0\n\
\n\
\n\
variable PL_FCI_STYLE 0x1\n\
\n\
\n\
variable PL_FCI_WEIGHT 0x2\n\
\n\
\n\
variable PL_FCI_SANS 0x0\n\
\n\
\n\
variable PL_FCI_SERIF 0x1\n\
\n\
\n\
variable PL_FCI_MONO 0x2\n\
\n\
\n\
variable PL_FCI_SCRIPT 0x3\n\
\n\
\n\
variable PL_FCI_SYMBOL 0x4\n\
\n\
\n\
variable PL_FCI_UPRIGHT 0x0\n\
\n\
\n\
variable PL_FCI_ITALIC 0x1\n\
\n\
\n\
variable PL_FCI_OBLIQUE 0x2\n\
\n\
\n\
variable PL_FCI_MEDIUM 0x0\n\
\n\
\n\
variable PL_FCI_BOLD 0x1\n\
\n\
\n\
variable PL_MAXKEY 16\n\
\n\
# Max number of windows/page tracked\n\
variable PL_MAXWINDOWS 64\n\
\n\
\n\
variable PL_NOTSET -42\n\
\n\
\n\
variable PL_PI 3.1415926535897932384\n\
\n\
\n\
variable PLESC_DOUBLEBUFFERING_ENABLE 1\n\
\n\
\n\
variable PLESC_DOUBLEBUFFERING_DISABLE 2\n\
\n\
\n\
variable PLESC_DOUBLEBUFFERING_QUERY 3\n\
\n\
\n\
variable PL_BIN_DEFAULT 0x0\n\
\n\
\n\
variable PL_BIN_CENTRED 0x1\n\
\n\
\n\
variable PL_BIN_NOEXPAND 0x2\n\
\n\
\n\
variable PL_BIN_NOEMPTY 0x4\n\
\n\
# Bivariate Cubic Spline approximation\n\
variable GRID_CSA 1\n\
\n\
# Delaunay Triangulation Linear Interpolation\n\
variable GRID_DTLI 2\n\
\n\
# Natural Neighbors Interpolation\n\
variable GRID_NNI 3\n\
\n\
# Nearest Neighbors Inverse Distance Weighted\n\
variable GRID_NNIDW 4\n\
\n\
# Nearest Neighbors Linear Interpolation\n\
variable GRID_NNLI 5\n\
\n\
# Nearest Neighbors Around Inverse Distance Weighted\n\
variable GRID_NNAIDW 6\n\
\n\
\n\
variable PL_HIST_DEFAULT 0x00\n\
\n\
\n\
variable PL_HIST_NOSCALING 0x01\n\
\n\
\n\
variable PL_HIST_IGNORE_OUTLIERS 0x02\n\
\n\
\n\
variable PL_HIST_NOEXPAND 0x08\n\
\n\
\n\
variable PL_HIST_NOEMPTY 0x10\n\
\n\
\n\
variable PL_POSITION_LEFT 0x1\n\
\n\
\n\
variable PL_POSITION_RIGHT 0x2\n\
\n\
\n\
variable PL_POSITION_TOP 0x4\n\
\n\
\n\
variable PL_POSITION_BOTTOM 0x8\n\
\n\
\n\
variable PL_POSITION_INSIDE 0x10\n\
\n\
\n\
variable PL_POSITION_OUTSIDE 0x20\n\
\n\
\n\
variable PL_POSITION_VIEWPORT 0x40\n\
\n\
\n\
variable PL_POSITION_SUBPAGE 0x80\n\
\n\
\n\
variable PL_LEGEND_NONE 0x1\n\
\n\
\n\
variable PL_LEGEND_COLOR_BOX 0x2\n\
\n\
\n\
variable PL_LEGEND_LINE 0x4\n\
\n\
\n\
variable PL_LEGEND_SYMBOL 0x8\n\
\n\
\n\
variable PL_LEGEND_TEXT_LEFT 0x10\n\
\n\
\n\
variable PL_LEGEND_BACKGROUND 0x20\n\
\n\
\n\
variable PL_LEGEND_BOUNDING_BOX 0x40\n\
\n\
\n\
variable PL_LEGEND_ROW_MAJOR 0x80\n\
\n\
\n\
variable PL_COLORBAR_LABEL_LEFT 0x1\n\
\n\
\n\
variable PL_COLORBAR_LABEL_RIGHT 0x2\n\
\n\
\n\
variable PL_COLORBAR_LABEL_TOP 0x4\n\
\n\
\n\
variable PL_COLORBAR_LABEL_BOTTOM 0x8\n\
\n\
\n\
variable PL_COLORBAR_IMAGE 0x10\n\
\n\
\n\
variable PL_COLORBAR_SHADE 0x20\n\
\n\
\n\
variable PL_COLORBAR_GRADIENT 0x40\n\
\n\
\n\
variable PL_COLORBAR_CAP_NONE 0x80\n\
\n\
\n\
variable PL_COLORBAR_CAP_LOW 0x100\n\
\n\
\n\
variable PL_COLORBAR_CAP_HIGH 0x200\n\
\n\
\n\
variable PL_COLORBAR_SHADE_LABEL 0x400\n\
\n\
\n\
variable PL_COLORBAR_ORIENT_RIGHT 0x800\n\
\n\
\n\
variable PL_COLORBAR_ORIENT_TOP 0x1000\n\
\n\
\n\
variable PL_COLORBAR_ORIENT_LEFT 0x2000\n\
\n\
\n\
variable PL_COLORBAR_ORIENT_BOTTOM 0x4000\n\
\n\
\n\
variable PL_COLORBAR_BACKGROUND 0x8000\n\
\n\
\n\
variable PL_COLORBAR_BOUNDING_BOX 0x10000\n\
\n\
\n\
variable PL_DRAWMODE_UNKNOWN 0x0\n\
\n\
\n\
variable PL_DRAWMODE_DEFAULT 0x1\n\
\n\
\n\
variable PL_DRAWMODE_REPLACE 0x2\n\
\n\
\n\
variable PL_DRAWMODE_XOR 0x4\n\
\n\
# draw lines parallel to the X axis\n\
variable DRAW_LINEX 0x001\n\
\n\
# draw lines parallel to the Y axis\n\
variable DRAW_LINEY 0x002\n\
\n\
# draw lines parallel to both the X and Y axis\n\
variable DRAW_LINEXY 0x003\n\
\n\
# draw the mesh with a color dependent of the magnitude\n\
variable MAG_COLOR 0x004\n\
\n\
# draw contour plot at bottom xy plane\n\
variable BASE_CONT 0x008\n\
\n\
# draw contour plot at top xy plane\n\
variable TOP_CONT 0x010\n\
\n\
# draw contour plot at surface\n\
variable SURF_CONT 0x020\n\
\n\
# draw sides\n\
variable DRAW_SIDES 0x040\n\
\n\
# draw outline for each square that makes up the surface\n\
variable FACETED 0x080\n\
\n\
# draw mesh\n\
variable MESH 0x100\n\
}" );
}
