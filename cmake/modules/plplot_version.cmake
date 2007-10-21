# For now, must set this by hand before each release.  This value affects
# at least the documentation build and perhaps other parts of the release.
set(RELEASE_DATE "2007-10-21")

# Version data that need review and possible modification for each release.

set(VERSION 5.8.0-RC1)

# CPack version numbers for release tarball name.
set(CPACK_PACKAGE_VERSION_MAJOR 5)
set(CPACK_PACKAGE_VERSION_MINOR 8)
set(CPACK_PACKAGE_VERSION_PATCH 0)

# PLplot library version information.

# Rules: 
# (1) If a backwards incompatible API change has been made in the library
#     API (e.g., if old compiled and linked applications will no longer work)
#     then increment SOVERSION and zero the corresponding minor and patch
#     numbers just before release.
# (2) If the library changes are limited to additions to the API, then
#     then leave SOVERSION alone, increment the minor number and zero the
#     patch number just before release.
# (3) If the library changes are limited to implementation changes with 
#     no API changes at all, then leave SOVERSION and minor number alone, and
#     increment the patch number just before the release.
# (4) If there are no library source code changes at all, then leave all
#     library version numbers the same for the release.

set(csirocsa_SOVERSION 0)
set(csirocsa_VERSION ${csirocsa_SOVERSION}.0.1)

set(csironn_SOVERSION 0)
set(csironn_VERSION ${csironn_SOVERSION}.0.1)

set(plplot_SOVERSION 9)
set(plplot_VERSION ${plplot_SOVERSION}.3.0)

set(plplotcxx_SOVERSION 9)
set(plplotcxx_VERSION ${plplotcxx_SOVERSION}.2.1)

set(plplotf77_SOVERSION 9)
set(plplotf77_VERSION ${plplotf77_SOVERSION}.1.1)

set(plplotf95_SOVERSION 9)
set(plplotf95_VERSION ${plplotf95_SOVERSION}.1.1)

set(plplotgnome2_SOVERSION 0)
set(plplotgnome2_VERSION ${plplotgnome2_SOVERSION}.0.0)

set(tclmatrix_SOVERSION 9)
set(tclmatrix_VERSION ${tclmatrix_SOVERSION}.2.0)

set(plplottcltk_SOVERSION 9)
set(plplottcltk_VERSION ${plplottcltk_SOVERSION}.2.0)

set(plplotwxwidgets_SOVERSION 0)
set(plplotwxwidgets_VERSION ${plplotwxwidgets_SOVERSION}.0.0)

set(plplotada_SOVERSION 0)
set(plplotada_VERSION ${plplotada_SOVERSION}.0.0)

# According to man package, PLPLOT_TCL_VERSION (used in the various
# pkgIndex.tcl scripts) should be strictly numbers alternativing with
# decimal points with the first number (the major number) signifying
# incompatible changes in the scripts.  To keep our life simple, let us
# use the major number from the package version for now.

set(PLPLOT_TCL_VERSION 5)

