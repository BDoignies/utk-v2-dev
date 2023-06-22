cmake_minimum_required(VERSION 3.12)
Project(UTKExamples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_BUILD_TYPE "Release")

find_package(UTK PATHS ../)

include_directories(${UTK_INCLUDE_DIR})

add_executable(EvalSamplers all.cpp)
target_link_libraries(EvalSamplers UTK_LIBRARY)