cmake_minimum_required(VERSION 3.0)
project("SimpleRPC")
message("project name: ${CMAKE_PROJECT_NAME}")
message("install dir: ${CMAKE_INSTALL_PREFIX}")

set(CMAKE_CXX_COMPILER "C:/MinGW/bin/gcc")
if("$ENV{BLDOPT}" STREQUAL "DEBUG")
	set(CMAKE_BUILD_TYPE Debug)
else() 
	set(CMAKE_BUILD_TYPE Release)
endif()

file(GLOB_RECURSE SOURCE_FILES ../src/*.c)
message("set vars: ${SOURCE_FILES}")

add_executable(rpc ${SOURCE_FILES})
target_link_libraries(rpc PUBLIC pthread)
