add_executable(easypt2 console.cpp core.cpp main.cpp name.cpp nobject.cpp number.cpp
osDependent.cpp parser.cpp runtime.cpp stack.cpp statement.cpp token.cpp objectPtrImpl.cpp
file.cpp library/consoleObj.cpp library/operators.cpp library/Object.cpp library/Array.cpp
library/File.cpp library/String.cpp library/Number.cpp library/Boolean.cpp)

# Having fun with Profile-Guided Optimizations
# set_target_properties(easypt2 PROPERTIES COMPILE_FLAGS "/GL")
# set_target_properties(easypt2 PROPERTIES LINK_FLAGS "/LTCG /GENPROFILE")
# set_target_properties(easypt2 PROPERTIES LINK_FLAGS "/LTCG /USEPROFILE")

install(TARGETS easypt2 RUNTIME DESTINATION bin COMPONENT binaries)