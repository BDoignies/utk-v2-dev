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
    # Compile external code disable and has an entry in the external folder
    IF (EXISTS ${UTK_EXTERNALS_FOLDER}/${FILE} AND NOT UTK_BUILD_EXTERNAL_CODE)
        CONTINUE()
    ENDIF()

    add_executable(${FILE} ${FILE}.cpp)
    target_include_directories(${FILE} PRIVATE "${UTK_INCLUDE_DIR}")

    IF (EXISTS ${UTK_EXTERNALS_FOLDER}/${FILE})
        FILE(GLOB_RECURSE ADDITIONNAL_CODE
                    "${UTK_EXTERNALS_FOLDER}/${FILE}/*.cpp"
            )
        target_sources(${FILE} PRIVATE ${ADDITIONNAL_CODE})
        target_include_directories(${FILE} PRIVATE ${UTK_EXTERNALS_FOLDER})
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
IF(OpenMP_FOUND AND UTK_BUILD_EXTERNAL_CODE)
    # Lloyd
    add_executable(Lloyd Lloyd.cpp  ${UTK_EXTERNALS_FOLDER}/Lloyd/Delaunay_psm.cpp)
    
    target_include_directories(Lloyd PRIVATE "${UTK_INCLUDE_DIR}")
    target_include_directories(Lloyd PRIVATE "${UTK_EXTERNALS_FOLDER}/")
    target_link_libraries(Lloyd PRIVATE spdlog::spdlog CLI11::CLI11 OpenMP::OpenMP_CXX)

    install(TARGETS Lloyd
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
ELSE()
    # message("Lloyd not compiled.")
ENDIF()

IF (CGAL_FOUND AND SuiteSparse_FOUND AND UTK_BUILD_EXTERNAL_CODE)
    add_executable(BoundedBNOT  BoundedBNOT.cpp)
    add_executable(PeriodicBNOT PeriodicBNOT.cpp)

    target_include_directories(BoundedBNOT PRIVATE "${UTK_INCLUDE_DIR}")
    target_include_directories(BoundedBNOT PRIVATE "${UTK_EXTERNALS_FOLDER}/")
    target_link_libraries(BoundedBNOT PRIVATE spdlog::spdlog CLI11::CLI11)
    target_link_libraries(BoundedBNOT PRIVATE CGAL::CGAL ${SUITESPARSE_LIBRARIES})
    
    target_include_directories(PeriodicBNOT PRIVATE "${UTK_INCLUDE_DIR}")
    target_include_directories(PeriodicBNOT PRIVATE "${UTK_EXTERNALS_FOLDER}/")
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
    # message("BNOT Not compiled")
ENDIF()
