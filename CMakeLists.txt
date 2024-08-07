cmake_minimum_required(VERSION 3.2)
project(glLib)

# GL lib
file(GLOB_RECURSE GL_LIB_SOURCES "GL_Lib/src/*.cpp")

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

include_directories(GL_Lib/src)
add_executable(glLib ${GL_LIB_SOURCES})

target_link_libraries(glLib PRIVATE ${OPENGL_LIBRARIES} glfw m)

# Tester
project(Tester)

file(GLOB_RECURSE TESTER_SOURCES "Tester/src/*.cpp")

include_directories(Tester/src)
add_executable(Tester ${TESTER_SOURCES})
