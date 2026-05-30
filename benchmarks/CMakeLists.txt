add_library(benchmarks_module
    fps_counter.cpp
    benchmark_suite.cpp
)

target_include_directories(benchmarks_module PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
