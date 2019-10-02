message(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src)
file(GLOB SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM SRC "${CMAKE_SOURCE_DIR}/src/main.cpp")

add_executable (testUtils testUtils.cpp ${SRC})
add_test(Utils testUtils)

add_executable (testDummyExpressions testDummyExpressions.cpp ${SRC})
add_test(DummyExpressions testDummyExpressions)

add_executable (testNumbers testNumbers.cpp)
add_test(Numbers testNumbers)

add_executable (testConversions testConversions.cpp ${SRC})
add_test(Conversions testConversions)

add_executable (testBasic1 testBasic1.cpp ${SRC})
add_test(Basic1 testBasic1 -file testBasic1.ez)