if(TARGET CGAL::CGAL)
    return()
endif()

message(STATUS "Third-party (external): creating target 'CGAL::CGAL'")

include(FetchContent)
FetchContent_Declare(
    cgal
    GIT_REPOSITORY https://github.com/CGAL/cgal.git
    GIT_TAG v5.5.2
    GIT_SHALLOW TRUE
    )
    
FetchContent_MakeAvailable(cgal)
