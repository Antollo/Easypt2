set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
# Force MSVC to place artifacts where I want it
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})

add_executable(easypt2 console.cpp core.cpp main.cpp name.cpp nobject.cpp number.cpp
osDependent.cpp parser.cpp runtime.cpp stack.cpp statement.cpp token.cpp objectPtrImpl.cpp
file.cpp library/consoleObj.cpp library/operators.cpp library/Object.cpp library/Array.cpp
library/File.cpp library/String.cpp library/Number.cpp)

# Having fun with Profile-Guided Optimizations
# set_target_properties(easypt2 PROPERTIES COMPILE_FLAGS "/GL")
# set_target_properties(easypt2 PROPERTIES LINK_FLAGS "/LTCG /GENPROFILE")
# set_target_properties(easypt2 PROPERTIES LINK_FLAGS "/LTCG /USEPROFILE")