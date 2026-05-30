cmake_minimum_required(VERSION 3.16)
project(adas-vision-pipeline VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")

# Find packages
find_package(OpenCV 4 REQUIRED)
find_package(Threads REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Add subdirectories
add_subdirectory(camera)
add_subdirectory(detection)
add_subdirectory(tracking)
add_subdirectory(calibration)
add_subdirectory(visualization)
add_subdirectory(benchmarks)

# Main executable
add_executable(adas_pipeline
    main.cpp
)

# Link libraries
target_link_libraries(adas_pipeline
    camera_module
    detection_module
    tracking_module
    calibration_module
    visualization_module
    benchmarks_module
    ${OpenCV_LIBS}
    Threads::Threads
)

# Installation
install(TARGETS adas_pipeline DESTINATION bin)
