cmake_minimum_required(VERSION 3.12)
Project(UnicornTK)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_BUILD_TYPE "Release")

option(UTK_LOG "Enable utk logging" ON)

# Disable by default, otherwise everything is downloaded when ccmake-ing
option(UTK_BUILD_EXTERNAL_CODE "Enable building of External codes" OFF)
option(UTK_BUILD_CGAL "Enable download and 'building' of CGAL and its dependencies" OFF)
option(UTK_USE_OPENMP "Enable USE of OpenMP" OFF)

# Finding libraries
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(spdlog)
include(cli11)

if (UTK_LOG)
    add_definitions(-DUTK_LOG)
endif()

if (UTK_USE_OPENMP)
    include(openmp)
endif()

if (UTK_BUILD_CGAL)
    include(cgal)
    include(suitesparse)
endif()

set(UTK_DATA_PATH "${PROJECT_SOURCE_DIR}/data")
set(UTK_EXTERNALS_FOLDER "${PROJECT_SOURCE_DIR}/externals/")
set(UTK_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/")

add_definitions(-DUTK_DATA_PATH=\"${UTK_DATA_PATH}\")
add_definitions(-D_USE_MATH_DEFINES) # Windows... Oh dear Windows...

### UTK COMPILATION ###
add_subdirectory(src/samplers)
add_subdirectory(src/metrics)
