add_library(calibration_module
    calibrator.cpp
)

target_include_directories(calibration_module PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(calibration_module
    ${OpenCV_LIBS}
)
