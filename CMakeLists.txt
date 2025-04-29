cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ADTLib CXX)
add_library(ADTLib INTERFACE)
target_include_directories(ADTLib INTERFACE include)

# Fetch and include Catch2
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.8.1
)
FetchContent_MakeAvailable(Catch2)

# Tests
include(CTest)
include(Catch)
enable_testing()
add_subdirectory(tests)

target_link_libraries(ADTLib INTERFACE Catch2::Catch2)
        
add_compile_options(-Wall -fdiagnostics-color=always)
