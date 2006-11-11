set(SIZE "\${SIZE}")
configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/index.html.in.in
${CMAKE_CURRENT_BINARY_DIR}/index.html.in @ONLY
)
