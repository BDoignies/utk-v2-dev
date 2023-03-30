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
include(cmake/externals/openmp.cmake)
include(cmake/externals/spdlog.cmake)
include(cmake/externals/cli11.cmake)
include(cmake/externals/cgal.cmake)
include(cmake/externals/suitesparse.cmake)


### UTK COMPILATION ###
include(cmake/utk/Samplers.cmake)
include(cmake/utk/Metrics.cmake)