# - Try to find MNN
#
# The following variables are optionally searched for defaults
#  NCNN_DIR:            Base directory where all NCNN components are found
#
# The following are set after configuration is done:
#  NCNN_FOUND
#  NCNN_INCLUDE_DIRS
#  NCNN_LIBRARIES
#  NCNN_LIBRARY_DIRS

include(FindPackageHandleStandardArgs)

set(NCNN_DIR "/home/huanyuan/code/ncnn/build/" CACHE PATH "Folder contains package NCNN")

find_path(NCNN_INCLUDE_DIR ncnn/net.h
  HINTS ${NCNN_DIR}
  PATH_SUFFIXES install/include)

find_library(NCNN_LIBRARY ncnn
  HINTS ${NCNN_DIR} 
  PATH_SUFFIXES install/lib)

find_package_handle_standard_args(
  NCNN DEFAULT_MSG NCNN_INCLUDE_DIR NCNN_LIBRARY)

if(NCNN_FOUND)
  set(NCNN_INCLUDE_DIRS ${NCNN_INCLUDE_DIR})
  set(NCNN_LIBRARIES ${NCNN_LIBRARY})
  include_directories(${NCNN_INCLUDE_DIRS})
endif()
