SET(SRCs
    L2
    CenteredL2
    Diaphony
    GL2
    UnanchoredL2
    SymmetricL2
    BoundariesStar

    Spectrum
    RadialSpectrum
    PCF
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

FOREACH(FILE ${SRCs})
    add_executable(${FILE} exe/${FILE}.cpp ${ADDITIONNAL_CODE})

    IF (OpenMP_FOUND)
        target_link_libraries(${FILE} PRIVATE OpenMP::OpenMP_CXX)
    ENDIF()

    install(TARGETS ${FILE}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
ENDFOREACH(FILE)