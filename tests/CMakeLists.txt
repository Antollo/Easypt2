add_executable(testUtils testUtils.cpp)
target_link_libraries(testUtils libeasypt)
add_test(Utils testUtils)

add_executable(testNumbers testNumbers.cpp)
target_link_libraries(testNumbers libeasypt)
add_test(Numbers testNumbers)

add_executable(testConversions testConversions.cpp)
target_link_libraries(testConversions libeasypt)
add_test(Conversions testConversions)

add_executable(testRunner testRunner.cpp)
target_link_libraries(testRunner libeasypt)

set(inputs 1 2 3 4)
foreach (input ${inputs})
    add_test("e2eTest${input}" testRunner
    -file "e2e/in/${input}.ez" -compare "e2e/out/${input}.out")
endforeach()

add_test(NAME "examples" COMMAND easypt -file "e2e/in/5.ez")
set_tests_properties("examples" PROPERTIES PASS_REGULAR_EXPRESSION "ok")