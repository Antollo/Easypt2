add_library(glfw INTERFACE)
add_library(glew INTERFACE)

if(MSVC)
    target_link_libraries(glfw INTERFACE "$ENV{TEMP}/glfw/glfw-3.3.3.bin.WIN64/lib-vc2019/glfw3_mt.lib")
    target_include_directories(glfw INTERFACE "$ENV{TEMP}/glfw/glfw-3.3.3.bin.WIN64/include")

    target_link_libraries(glew INTERFACE "$ENV{TEMP}/glew/glew-2.1.0/lib/Release/x64/glew32s.lib")
    target_include_directories(glew INTERFACE "$ENV{TEMP}/glew/glew-2.1.0/include")
else()
    target_link_libraries(glfw INTERFACE "/tmp/glfw/glfw-3.3.3/src/libglfw3.a")
    target_include_directories(glfw INTERFACE "/tmp/glfw/glfw-3.3.3/include")

    target_link_libraries(glew INTERFACE "/tmp/glew/glew-2.1.0/lib/libGLEW.a")
    target_include_directories(glew INTERFACE "/tmp/glew/glew-2.1.0/include")
endif()

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

add_library(opengl SHARED Opengl.cpp ../number.cpp)
SET_TARGET_PROPERTIES(opengl PROPERTIES PREFIX "")
target_link_libraries(opengl PUBLIC OpenGL::GL)
target_link_libraries(opengl PUBLIC glfw)
target_link_libraries(opengl PUBLIC glew)