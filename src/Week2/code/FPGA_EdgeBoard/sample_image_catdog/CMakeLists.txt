cmake_minimum_required(VERSION 3.5.1)
project(paddle_edgeboard)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

add_definitions(-DPADDLE_MOBILE_FPGA_V1)
add_definitions(-DPADDLE_MOBILE_FPGA)

set(PADDLE_LIB_DIR "${PROJECT_SOURCE_DIR}/lib" )
set(EASYDL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include" )
set(PADDLE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/paddle-mobile" )

set(APP_NAME "paddle_edgeboard" )

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC)


find_package(OpenCV QUIET COMPONENTS core videoio highgui imgproc imgcodecs ml video)
include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})
#list(APPEND Caffe_LINKER_LIBS ${OpenCV_LIBS})
message(STATUS "OpenCV found (${OpenCV_CONFIG_PATH})，${OpenCV_LIBS}")
#add_definitions(-DUSE_OPENCV)

include_directories(${EASYDL_INCLUDE_DIR})
include_directories(${PADDLE_INCLUDE_DIR})
LINK_DIRECTORIES(${PADDLE_LIB_DIR})

add_executable(${APP_NAME} ${SRC})
target_link_libraries(${APP_NAME} paddle-mobile)
target_link_libraries(${APP_NAME} ${OpenCV_LIBS} ) 

