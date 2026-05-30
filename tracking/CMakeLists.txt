add_library(tracking_module
    object_tracker.cpp
    kalman_filter.cpp
)

target_include_directories(tracking_module PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(tracking_module
    ${OpenCV_LIBS}
)
