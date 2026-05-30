add_library(visualization_module
    visualizer.cpp
)

target_include_directories(visualization_module PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(visualization_module
    ${OpenCV_LIBS}
)
