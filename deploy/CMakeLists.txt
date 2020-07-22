# set core files
file(GLOB DEEPSORT_CORE_FILES deepsort.h deepsort.cpp)
file(GLOB DEEPSORT_COMMON_FILES common/*.h common/*.cpp)

source_group(deepsort_common FILES ${DEEPSORT_COMMON_FILES})

# yh_mnn dll library
add_library(deepsort SHARED ${DEEPSORT_CORE_FILES} ${DEEPSORT_COMMON_FILES})
target_link_libraries(deepsort ${OpenCV_LIBS})