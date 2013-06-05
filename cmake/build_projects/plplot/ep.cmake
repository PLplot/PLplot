# Configure build of PLplot dependencies.

include(shapelib/ep.cmake)
include(libqhull/ep.cmake)
include(libharu/ep.cmake)
include(libagg/ep.cmake)

# Build PLplot itself.
# (Not implemented yet, but create empty custom target with correct
# target dependencies as if that target were actually a PLplot build).
add_custom_target(build_plplot)
add_dependencies(build_plplot 
  build_shapelib
  build_libqhull
  build_libharu
  build_libagg
  )



