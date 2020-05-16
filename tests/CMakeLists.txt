include_directories(../src)
include_directories(../src/parser)

set_source_files_properties(../src/parser/gram.c PROPERTIES LANGUAGE CXX)
set_source_files_properties(../src/parser/lex.yy.c PROPERTIES LANGUAGE CXX)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    include_directories(SYSTEM parser)
endif ()

set(SRC ../src/console.cpp ../src/core.cpp ../src/name.cpp
../src/nobject.cpp ../src/number.cpp ../src/osDependent.cpp
../src/runtime.cpp ../src/stack.cpp ../src/objectPtrImpl.cpp
../src/file.cpp ../src/library/consoleObj.cpp ../src/library/operators.cpp
../src/library/Object.cpp ../src/library/Array.cpp ../src/library/File.cpp
../src/library/String.cpp ../src/library/Number.cpp ../src/library/Boolean.cpp
../src/library/Class.cpp ../src/library/Promise.cpp ../src/library/Time.cpp
../src/parser/gram.c ../src/parser/lex.yy.c ../src/parser/gramUtility.cpp 
../src/parser/treeParser.cpp ../src/parser/Node.cpp)

add_executable (testUtils testUtils.cpp ${SRC})
add_test(Utils testUtils)

#add_executable (testDummyExpressions testDummyExpressions.cpp ${SRC})
#add_test(DummyExpressions testDummyExpressions)

add_executable (testNumbers testNumbers.cpp ${SRC})
add_test(Numbers testNumbers)

add_executable (testConversions testConversions.cpp ${SRC})
add_test(Conversions testConversions)

add_executable (testBasic1 testBasic1.cpp ${SRC})
add_test(Basic1 testBasic1 -file testBasic1.ez)

add_executable (testBasic2 testBasic2.cpp ${SRC})
add_test(Basic2 testBasic2 -file testBasic2.ez)