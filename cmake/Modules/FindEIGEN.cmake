# - Try to find EIGEN
#
# The following variables are optionally searched for defaults
#  EIGEN_DIR:            Base directory where all EIGEN components are found
#
# The following are set after configuration is done:
#  EIGEN_FOUND
#  EIGEN_INCLUDE_DIRS
#  EIGEN_LIBRARIES
#  EIGEN_LIBRARY_DIRS

include(FindPackageHandleStandardArgs)

set(EIGEN_DIR "/home/workspace/RMAI/yuanhuan/code/eigen-git-mirror/build/" CACHE PATH "Folder contains package EIGEN")

find_path(EIGEN_INCLUDE_DIR Eigen/Dense
  HINTS ${EIGEN_DIR}
  PATH_SUFFIXES install/include/eigen3)

find_package_handle_standard_args(
  EIGEN DEFAULT_MSG EIGEN_DIR)

if(EIGEN_FOUND)
  set(EIGEN_INCLUDE_DIRS ${EIGEN_INCLUDE_DIR})
  include_directories(${EIGEN_INCLUDE_DIRS})
endif()
