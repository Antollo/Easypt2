if (WIN32)
    execute_process(
        COMMAND "$ENV{TEMP}/flex/bin/flex.exe" scan.l
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    execute_process(
        COMMAND "$ENV{TEMP}/LLnextgen/LLnextgen/LLnextgen.exe" gram.g
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
else()
    execute_process(
        COMMAND flex scan.l
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    execute_process(
        COMMAND LLnextgen gram.g
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()