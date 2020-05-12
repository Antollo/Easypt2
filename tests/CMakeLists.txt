include_directories(${CMAKE_SOURCE_DIR}/src)
#file(GLOB_RECURSE SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")
#list(REMOVE_ITEM SRC "${CMAKE_SOURCE_DIR}/src/main.cpp")

set(SRC ${CMAKE_SOURCE_DIR}/src/console.cpp ${CMAKE_SOURCE_DIR}/src/core.cpp ${CMAKE_SOURCE_DIR}/src/name.cpp
${CMAKE_SOURCE_DIR}/src/nobject.cpp ${CMAKE_SOURCE_DIR}/src/number.cpp ${CMAKE_SOURCE_DIR}/src/osDependent.cpp
${CMAKE_SOURCE_DIR}/src/parser.cpp ${CMAKE_SOURCE_DIR}/src/runtime.cpp ${CMAKE_SOURCE_DIR}/src/stack.cpp
${CMAKE_SOURCE_DIR}/src/statement.cpp ${CMAKE_SOURCE_DIR}/src/token.cpp ${CMAKE_SOURCE_DIR}/src/objectPtrImpl.cpp
${CMAKE_SOURCE_DIR}/src/file.cpp ${CMAKE_SOURCE_DIR}/src/library/consoleObj.cpp ${CMAKE_SOURCE_DIR}/src/library/operators.cpp
${CMAKE_SOURCE_DIR}/src/library/Object.cpp ${CMAKE_SOURCE_DIR}/src/library/Array.cpp ${CMAKE_SOURCE_DIR}/src/library/File.cpp
${CMAKE_SOURCE_DIR}/src/library/String.cpp ${CMAKE_SOURCE_DIR}/src/library/Number.cpp ${CMAKE_SOURCE_DIR}/src/library/Boolean.cpp
${CMAKE_SOURCE_DIR}/src/library/Class.cpp ${CMAKE_SOURCE_DIR}/src/library/Promise.cpp ${CMAKE_SOURCE_DIR}/src/library/Time.cpp)

add_executable (testUtils testUtils.cpp ${SRC})
add_test(Utils testUtils)

add_executable (testDummyExpressions testDummyExpressions.cpp ${SRC})
add_test(DummyExpressions testDummyExpressions)

add_executable (testNumbers testNumbers.cpp ${CMAKE_SOURCE_DIR}/src/console.cpp ${CMAKE_SOURCE_DIR}/src/osDependent.cpp)
add_test(Numbers testNumbers)

add_executable (testConversions testConversions.cpp ${SRC})
add_test(Conversions testConversions)

add_executable (testBasic1 testBasic1.cpp ${SRC})
add_test(Basic1 testBasic1 -file testBasic1.ez)

add_executable (testBasic2 testBasic2.cpp ${SRC})
add_test(Basic2 testBasic2 -file testBasic2.ez)