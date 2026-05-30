add_library(detection_module
    lane_detector.cpp
    vehicle_detector.cpp
    sign_detector.cpp
)

target_include_directories(detection_module PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(detection_module
    ${OpenCV_LIBS}
)
