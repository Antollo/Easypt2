add_subdirectory(parser)

set_source_files_properties(parser/gram.c PROPERTIES LANGUAGE CXX)
set_source_files_properties(parser/lex.yy.c PROPERTIES LANGUAGE CXX)

add_executable(easypt2 console.cpp core.cpp main.cpp name.cpp nobject.cpp number.cpp
osDependent.cpp runtime.cpp stack.cpp objectPtrImpl.cpp file.cpp
library/consoleObj.cpp library/operators.cpp library/Object.cpp library/Array.cpp
library/File.cpp library/String.cpp library/Number.cpp library/Boolean.cpp library/Class.cpp
library/Promise.cpp library/Time.cpp
parser/gram.c parser/lex.yy.c parser/gramUtility.cpp parser/treeParser.cpp parser/Node.cpp)

target_include_directories(easypt2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/parser)
target_include_directories(easypt2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_include_directories(easypt2 SYSTEM PRIVATE parser)
endif ()

# Having fun with Profile-Guided Optimizations
# set_target_properties(easypt2 PROPERTIES COMPILE_FLAGS "/GL")
# set_target_properties(easypt2 PROPERTIES LINK_FLAGS "/LTCG /GENPROFILE")
# set_target_properties(easypt2 PROPERTIES LINK_FLAGS "/LTCG /USEPROFILE")

install(TARGETS easypt2 RUNTIME DESTINATION bin COMPONENT binaries)