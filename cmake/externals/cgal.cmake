# CGAL and fetch content does not seem to work...
# 2 hours of test did not show any successes...
# Even after finding the good targets, the include path are not set properly...
# And, I found no examples on the internet that uses fetch content with CMake
# even for projects that uses other library with the mecanism :

# Ex :
# https://gitlab.vci.rwth-aachen.de:9000/gkobsik/triangular-interpolants/-/blob/5576fc39f375eac027ae943c538b841f907ec7e2/CMakeLists.txt

# if(TARGET CGAL::CGAL)
#     return()
# endif()
# 
# message(STATUS "Third-party (external): creating target 'CGAL::CGAL'")
# 
# include(FetchContent)
# FetchContent_Declare(
#     CGAL
#     GIT_REPOSITORY https://github.com/CGAL/cgal.git
#     GIT_TAG v5.5.2
#     GIT_SHALLOW TRUE
#     )
# 
# FetchContent_MakeAvailable(CGAL)
# 
# FetchContent_GetProperties(CGAL)
# IF (NOT CGAL_POPULATED)
#     message("Manual populate")
#     FetchContent_Populate(CGAL)
#     add_subdirectory(${CGAL_SOURCE_DIR} ${CGAL_BINARY_DIR})
# ENDIF()
find_package(CGAL)
