add_subdirectory(parser)

set_source_files_properties(parser/gram.c PROPERTIES LANGUAGE CXX)
set_source_files_properties(parser/lex.yy.c PROPERTIES LANGUAGE CXX)

add_library(libeasypt OBJECT console.cpp core.cpp name.cpp nobject.cpp number.cpp externalFunction.cpp
osDependent.cpp runtime.cpp stack.cpp objectPtrImpl.cpp file.cpp coroutine.cpp childProcess.cpp
library/consoleObj.cpp library/Accessible.cpp library/Object.cpp library/Array.cpp
library/File.cpp library/String.cpp library/Number.cpp library/Boolean.cpp library/Class.cpp
library/Promise.cpp library/Time.cpp library/Tcp.cpp library/ChildProcess.cpp library/Buffer.cpp
parser/gram.c parser/lex.yy.c parser/gramUtility.cpp parser/treeParser.cpp parser/Node.cpp)

target_include_directories(libeasypt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/parser)
target_include_directories(libeasypt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_include_directories(libeasypt SYSTEM PUBLIC parser/fix)
else ()
    target_link_libraries(libeasypt ${CMAKE_DL_LIBS})
endif ()

find_package(Threads REQUIRED)
target_link_libraries(libeasypt Threads::Threads)

add_executable(easypt main.cpp)
target_link_libraries(easypt libeasypt)

add_subdirectory(library)

install(TARGETS easypt RUNTIME DESTINATION bin COMPONENT binaries)