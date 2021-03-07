add_executable(testUtils testUtils.cpp)
target_link_libraries(testUtils libeasypt)
add_test(NAME Utils COMMAND testUtils WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(testNumbers testNumbers.cpp)
target_link_libraries(testNumbers libeasypt)
add_test(NAME Numbers COMMAND testNumbers WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(testConversions testConversions.cpp)
target_link_libraries(testConversions libeasypt)
add_test(NAME Conversions COMMAND testConversions WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(testRunner testRunner.cpp)
target_link_libraries(testRunner libeasypt)

set(inputs 1 2 3 4 5)
foreach (input ${inputs})
    add_test(
        NAME "e2eTest${input}"
        COMMAND "../bin/testRunner" -file "e2e/in/${input}.ez" -compare "e2e/out/${input}.out"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests
    )
endforeach()

add_test(
    NAME "examples"
    COMMAND "../bin/easypt" -file "e2e/in/examples.ez"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests
)
set_tests_properties("examples" PROPERTIES PASS_REGULAR_EXPRESSION "ok")