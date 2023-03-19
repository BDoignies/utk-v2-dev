SET(SRCs
    Nebraska
)

FOREACH(FILE ${SRCs})
    add_executable(${FILE} ${FILE}.cpp)

    install(TARGETS ${FILE}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
ENDFOREACH(FILE)
