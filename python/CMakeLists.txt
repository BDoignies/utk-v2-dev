project(pyutk)

cmake_minimum_required (VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0048 NEW)

set(THREADS_PREFER_PTHREAD_FLAG ON)
include(../cmake/externals/openmp.cmake)
include(../cmake/externals/spdlog.cmake)
include(../cmake/externals/pybind11.cmake)
# Sampler that have author code 
FILE(GLOB_RECURSE
    SAMPLERS_CPP
        ../authors/*.cppk
)

# TODO : 
# Lloyd (that depends on PTHREAD)

set(UTK_DATA_PATH "${PROJECT_SOURCE_DIR}/../data")
add_definitions(-DUTK_DATA_PATH=\"${UTK_DATA_PATH}\")

pybind11_add_module(pyutk
    pyUTK.cpp
    ${SAMPLERS_CPP}
)


option(UTK_LOG "Enable utk logging" ON)
option(UTK_DEBUG "Enable utk debug logs" OFF)

if (UTK_LOG)
    add_definitions(-DUTK_LOG)

    if (NOT UTK_DEBUG)
        add_definitions(-DUTK_LOG_NO_DEBUG)
    endif()
endif()


# target_include_directories(pyutk PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/)
target_include_directories(pyutk PRIVATE ../include/)
target_include_directories(pyutk PRIVATE ../authors/)

target_link_libraries(pyutk PRIVATE pybind11::module spdlog::spdlog)

if (OpenMP_FOUND)
    message("Linking with OpenMP")
    target_link_libraries(pyutk PRIVATE OpenMP::OpenMP_CXX)
    target_compile_definitions(pyutk PRIVATE -DUTK_USE_OPENMP)
endif()

if (Threads_FOUND)
    target_link_libraries(pyutk PRIVATE Threads::Threads)
endif()
