Project(UnicornTK)
cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_BUILD_TYPE "Release")

set(UTK_DATA_PATH "${PROJECT_SOURCE_DIR}/data")

add_definitions(-DUTK_DATA_PATH=\"${UTK_DATA_PATH}\")
add_definitions(-D_USE_MATH_DEFINES) # Windows... Oh dear Windows...

option(UTK_LOG "Enable utk logging" ON)
option(UTK_DEBUG "Enable utk debug logs" OFF)

if (UTK_LOG)
    add_definitions(-DUTK_LOG)

    if (NOT UTK_DEBUG)
        add_definitions(-DUTK_LOG_NO_DEBUG)
    endif()
endif()

# Finding libraries
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(openmp)
include(spdlog)
include(cli11)
include(cgal)
include(suitesparse)


### UTK COMPILATION ###
add_subdirectory(src/samplers)
add_subdirectory(src/metrics)