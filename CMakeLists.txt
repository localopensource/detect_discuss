cmake_minimum_required(VERSION 3.10)
project(lanedetection_ai)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(CURL REQUIRED)

add_executable(lanedetection detect.cpp)

target_link_libraries(lanedetection
    ${OpenCV_LIBS}
    CURL::libcurl
)