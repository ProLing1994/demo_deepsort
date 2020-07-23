# set core files
file(GLOB DEEPSORT_CORE_FILES deepsort.h deepsort.cpp)
file(GLOB DEEPSORT_COMMON_FILES common/*.h common/*.cpp)
file(GLOB DEEPSORT_MUNKRES_FILES munkres/*.h munkres/*.cpp)
file(GLOB DEEPSORT_UTILS_FILES utils/*.h utils/*.cpp)
file(GLOB NCNN_NET_FILES net/*.h net/*.cpp)

source_group(deepsort_common FILES ${DEEPSORT_COMMON_FILES})
source_group(deepsort_munkres FILES ${DEEPSORT_MUNKRES_FILES})
source_group(deepsort_utils FILES ${DEEPSORT_UTILS_FILES})

# deepsort dll library
add_library(deepsort SHARED ${DEEPSORT_CORE_FILES} ${DEEPSORT_COMMON_FILES} ${DEEPSORT_MUNKRES_FILES} ${DEEPSORT_UTILS_FILES})
target_link_libraries(deepsort ${OpenCV_LIBS})

# add test
add_executable(demo demo.cpp ${NCNN_NET_FILES})
target_link_libraries(demo deepsort ${OpenCV_LIBS} ${NCNN_LIBRARIES})
target_link_libraries(demo -fopenmp)