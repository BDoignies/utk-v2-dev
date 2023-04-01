SET(Samplers
    Whitenoise
    Stratified
    HexagonalGrid
    RegularGrid
    Rank1
    Halton
    Hammersley
    Faure
    NRooks
    Niederreiter
    Korobov
    Kronecker
    DartThrowing
    AAPatterns
    ART
    CMJ
    FastPoisson
    Fattal
    ForcedRandom
    VanDerCorput
    Step
    SinglePeak
    Penrose
    ProjectiveBlueNoise
    PMJ
    LDBN
    LutLDBN
    Sobol
)

FOREACH(FILE ${Samplers})
    add_executable(${FILE} ${FILE}.cpp)
    target_include_directories(${FILE} PRIVATE "${PROJECT_SOURCE_DIR}/include/")

    IF (EXISTS ${PROJECT_SOURCE_DIR}/authors/${FILE})
        FILE(GLOB_RECURSE ADDITIONNAL_CODE
                    "${PROJECT_SOURCE_DIR}/authors/${FILE}/*.cpp"
            )
        target_sources(${FILE} PRIVATE ${ADDITIONNAL_CODE})
        target_include_directories(${FILE} PRIVATE ${PROJECT_SOURCE_DIR}/authors)
    ENDIF()

    target_link_libraries(${FILE} PRIVATE spdlog::spdlog CLI11::CLI11)

    install(TARGETS ${FILE}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
ENDFOREACH(FILE)

# ===========================================
#  Samplers requiring additional libraries  #
# ===========================================

# Threads is looked for in main cmake
IF(OpenMP_FOUND)
    # Lloyd
    add_executable(Lloyd Lloyd.cpp ${PROJECT_SOURCE_DIR}/authors/Lloyd/Delaunay_psm.cpp)
    
    target_include_directories(Lloyd PRIVATE "${PROJECT_SOURCE_DIR}/include/")
    target_include_directories(Lloyd PRIVATE "${PROJECT_SOURCE_DIR}/authors/")
    target_link_libraries(Lloyd PRIVATE spdlog::spdlog CLI11::CLI11 OpenMP::OpenMP_CXX)

    install(TARGETS Lloyd
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
ELSEIF()
    message("OpenMP not found, Lloyd will not be compiled. ")
ENDIF()

IF (SuiteSparse_FOUND)
    add_executable(BoundedBNOT  BoundedBNOT.cpp)
    add_executable(PeriodicBNOT PeriodicBNOT.cpp)

    target_include_directories(BoundedBNOT PRIVATE "${PROJECT_SOURCE_DIR}/include/")
    target_include_directories(BoundedBNOT PRIVATE "${PROJECT_SOURCE_DIR}/authors/")
    target_link_libraries(BoundedBNOT PRIVATE spdlog::spdlog CLI11::CLI11)
    target_link_libraries(BoundedBNOT PRIVATE CGAL::CGAL ${SUITESPARSE_LIBRARIES})
    
    target_include_directories(PeriodicBNOT PRIVATE "${PROJECT_SOURCE_DIR}/include/")
    target_include_directories(PeriodicBNOT PRIVATE "${PROJECT_SOURCE_DIR}/authors/")
    target_link_libraries(PeriodicBNOT PRIVATE spdlog::spdlog CLI11::CLI11)
    target_link_libraries(PeriodicBNOT PRIVATE CGAL::CGAL ${SUITESPARSE_LIBRARIES})
    
    install(TARGETS BoundedBNOT
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

    install(TARGETS PeriodicBNOT
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
ELSE()
    message("SuiteSparse not found. Can not compile BNOT")
ENDIF()
