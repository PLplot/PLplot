add_subdirectory(www) 

if(BUILD_DOC)
  add_subdirectory(bin) 
endif(BUILD_DOC)

if(BUILD_DOC OR PREBUILT_DOC)
  add_subdirectory(src) 
endif(BUILD_DOC OR PREBUILT_DOC)

