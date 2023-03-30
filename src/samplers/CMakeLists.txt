SET(SRCs
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

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

FOREACH(FILE ${SRCs})
    SET(ADDITIONNAL_CODE "")

    IF (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/${FILE})
        FILE(GLOB_RECURSE ADDITIONNAL_CODE
                    "${CMAKE_CURRENT_SOURCE_DIR}/src/${FILE}/*.cpp"
            )
    ENDIF()

    add_executable(${FILE} exe/${FILE}.cpp ${ADDITIONNAL_CODE})
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
IF(Threads_FOUND)
    # Lloyd
    add_executable(Lloyd exe/Lloyd.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/Lloyd/Delaunay_psm.cpp)
    target_link_libraries(Lloyd PRIVATE Threads::Threads spdlog::spdlog CLI11::CLI11)

    install(TARGETS Lloyd
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
ELSEIF()
    message("Threads not found, Lloyd will not be compiled. ")
ENDIF()

# ===========================================
#              "Misc Samplers"              #
# ===========================================

add_subdirectory(misc)
