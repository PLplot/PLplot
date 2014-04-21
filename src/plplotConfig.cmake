if(PLPLOT_USE_QT5)
  # Find Qt5 components that we need.
  find_package(Qt5 5.2.0 COMPONENTS Svg Gui PrintSupport)
endif(PLPLOT_USE_QT5)

# Find export files in same directory location as present file.
include(${CMAKE_CURRENT_LIST_DIR}/export_plplot.cmake)
