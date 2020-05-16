add_executable(testUtils testUtils.cpp)
target_link_libraries(testUtils libeasypt)
add_test(Utils testUtils)

add_executable(testNumbers testNumbers.cpp)
target_link_libraries(testNumbers libeasypt)
add_test(Numbers testNumbers)

add_executable(testConversions testConversions.cpp)
target_link_libraries(testConversions libeasypt)
add_test(Conversions testConversions)

add_executable(testBasic1 testBasic1.cpp)
target_link_libraries(testBasic1 libeasypt)
add_test(Basic1 testBasic1 -file testBasic1.ez)

add_executable(testBasic2 testBasic2.cpp)
target_link_libraries(testBasic2 libeasypt)
add_test(Basic2 testBasic2 -file testBasic2.ez)