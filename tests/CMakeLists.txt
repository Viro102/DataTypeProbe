add_executable(example_usage example.cpp)
target_link_libraries(example_usage PRIVATE ADTLib)
target_include_directories(example_usage PRIVATE ${CMAKE_SOURCE_DIR}/include)
