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
    add_executable(${FILE} ${PROJECT_SOURCE_DIR}/src/samplers/${FILE}.cpp)
    target_include_directories(${FILE} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/")

    IF (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/authors/${FILE})
        FILE(GLOB_RECURSE ADDITIONNAL_CODE
                    "${CMAKE_CURRENT_SOURCE_DIR}/authors/${FILE}/*.cpp"
            )
        target_sources(${FILE} PRIVATE ${ADDITIONNAL_CODE})
        target_include_directories(${FILE} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/authors)
    ENDIF()

    target_link_libraries(${FILE} PRIVATE spdlog::spdlog CLI11::CLI11)

    install(TARGETS ${FILE}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
ENDFOREACH(FILE)