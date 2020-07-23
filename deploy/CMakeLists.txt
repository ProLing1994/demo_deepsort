# set core files
file(GLOB DEEPSORT_CORE_FILES deepsort.h deepsort.cpp)
file(GLOB DEEPSORT_COMMON_FILES common/*.h common/*.cpp)
file(GLOB DEEPSORT_MUNKRES_FILES munkres/*.h munkres/*.cpp)
file(GLOB DEEPSORT_UTILS_FILES utils/*.h utils/*.cpp)

source_group(deepsort_common FILES ${DEEPSORT_COMMON_FILES} ${DEEPSORT_MUNKRES_FILES} ${DEEPSORT_UTILS_FILES})

# deepsort dll library
add_library(deepsort SHARED ${DEEPSORT_CORE_FILES} ${DEEPSORT_COMMON_FILES})
target_link_libraries(deepsort ${OpenCV_LIBS})