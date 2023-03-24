Project(UnicornTK)
cmake_minimum_required (VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)


MESSAGE(STATUS "C++17 enabled by cmake. ")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_BUILD_TYPE "Release")

set(UTK_DATA_PATH "${PROJECT_SOURCE_DIR}/data")

add_definitions(-DUTK_DATA_PATH=\"${UTK_DATA_PATH}\")
add_definitions(-D_USE_MATH_DEFINES) # Windows... Oh dear Windows...

# Finding libraries
# include(openmp)
# include(pybind11)
# include(spdlog)
# include(cli11)
# include(cgal)

add_subdirectory(src)
