add_library(camera_module
    camera_driver.cpp
)

target_include_directories(camera_module PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(camera_module
    ${OpenCV_LIBS}
)
