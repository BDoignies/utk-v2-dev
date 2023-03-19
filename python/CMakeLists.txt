project(pyutk)

cmake_minimum_required (VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0048 NEW)

add_subdirectory(externals/pybind11)

# Sampler that have author code 
FILE(GLOB_RECURSE
    SAMPLERS_CPP
        ../src/samplers/src/AAPatterns/*.cpp
        ../src/samplers/src/ART/*.cpp
        ../src/samplers/src/FastPoisson/*.cpp
        ../src/samplers/src/Fattal/*.cpp
        ../src/samplers/src/ForcedRandom/*.cpp
        ../src/samplers/src/Kronecker/*
        ../src/samplers/src/LutLDBN/*.cpp
        ../src/samplers/src/Penrose/*.cpp
        ../src/samplers/src/PMJ/*.cpp
        ../src/samplers/src/STEP/*.cpp
)

# TODO : 
# Lloyd (that depends on PTHREAD)

set(UTK_DATA_PATH "${PROJECT_SOURCE_DIR}/../data")
add_definitions(-DUTK_DATA_PATH=\"${UTK_DATA_PATH}\")

pybind11_add_module(pyutk
    pyUTK.cpp
    ${SAMPLERS_CPP}
)

target_include_directories(pyutk PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/)
target_link_libraries(pyutk PRIVATE pybind11::module)